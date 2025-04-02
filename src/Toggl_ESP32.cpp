#if defined(ESP32)

#include "Toggl.h"

Toggl::Toggl()
{
}

// Using built in ESP32 Base64 driver
void Toggl::setAuth(String const & Token)
{

  String TokenHolder{Token + ":api_token"};

  String Encoded = base64::encode(TokenHolder);

  AuthorizationKey = ("Basic " + Encoded);
  return;
}

// Get user data
const String Toggl::getUserData(String Input)
{

  String  payload{};
  String  Output{};
  int16_t HTTP_Code{};

  HTTPClient https;
  https.begin(BaseUrl + "/me", root_ca);
  https.addHeader("Authorization", AuthorizationKey);

  HTTP_Code = https.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {
    StaticJsonDocument<80> filter;
    filter["data"][Input] = true;

    DynamicJsonDocument doc(2 * JSON_OBJECT_SIZE(1) + 60);
    deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

    String TMP_Str = doc["data"][Input];
    Output         = TMP_Str;
  }

  else
  {
    Output = ("Error: " + String(HTTP_Code));
  }

  https.end();
  return Output;
}

const String Toggl::StartTimeEntry(String const & Description, String const & Tags, int const & PID, String const & CreatedWith)
{

  String payload;

  HTTPClient https;
  https.begin(BaseUrl + "/time_entries/start", root_ca);
  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");

  DynamicJsonDocument doc(JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(5 + 1));

  doc["time_entry"]["description"]  = Description;
  doc["time_entry"]["tags"]         = Tags;
  doc["time_entry"]["pid"]          = PID;
  doc["time_entry"]["created_with"] = CreatedWith;

  serializeJson(doc, payload);

  https.POST(payload);
  doc.clear();

  deserializeJson(doc, https.getString());

  String TimeID = doc["data"]["id"];

  doc.clear();

  https.end();

  return TimeID;
}

const String Toggl::StopTimeEntry(TimeEntry const timeEntry)
{

  String HTTP_Code{};
  String workspaceId = String(timeEntry.getWorkspaceId());
  String timeEntryId = String(timeEntry.getId());

  HTTPClient https;
  https.begin(BaseUrl + "/workspaces/" + workspaceId + "/time_entries/" + timeEntryId + "/stop", root_ca);

  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");
  HTTP_Code = String(https.PATCH(" "));
  https.end();

  // TODO: Check if the time entry was stopped correctly

  return HTTP_Code;
}

const String Toggl::CreateTimeEntry(String const & Description, String const & Tags, int const & Duration, String const & Start, int const & PID, String const & CreatedWith, int workspaceID, TimeEntry * timeEntry)
{

  String   payload;
  uint16_t HTTP_Code{};
  String   ret{};

  HTTPClient https;
  if (timeEntry != NULL)
  {
    https.begin(BaseUrl + "/workspaces/" + workspaceID + "/time_entries", root_ca);
    https.addHeader("Authorization", AuthorizationKey, true);
    https.addHeader("Content-Type", " application/json");

    JsonDocument doc;

    doc["description"] = Description;
    // doc["tags"]         = Tags;
    doc["duration"] = Duration;
    doc["start"]    = Start;
    // doc["pid"]          = PID;
    doc["created_with"] = CreatedWith;
    doc["workspace_id"] = workspaceID;

    serializeJson(doc, payload);

    HTTP_Code = https.POST(payload);
    doc.clear();
    ret = String(std::to_string(HTTP_Code).c_str());
    deserializeJson(doc, https.getString());
    timeEntry->fromJson(doc);

    doc.clear();

    https.end();
  }

  return ret;
}

const String Toggl::GetCurrentTimeEntry(TimeEntry * timeEntry)
{
  // https://api.track.toggl.com/api/v9/me/time_entries/current
  int HTTP_Code = 0;

  HTTPClient https;
  https.begin(BaseUrl + "/me/time_entries/current", root_ca);

  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");
  HTTP_Code = https.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {

    JsonDocument doc;

    deserializeJson(doc, https.getString());
    serializeJsonPretty(doc, Serial); // for debugging
    timeEntry->fromJson(doc);
    doc.clear();  
  }
  https.end();

  return String(HTTP_Code);
}

const String Toggl::CreateTag(String const & Name, int const & WID)
{

  String payload;

  HTTPClient https;
  https.begin(BaseUrl + "/tags", root_ca);
  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");

  DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3));

  doc["tag"]["name"] = Name;
  doc["tag"]["wid"]  = WID;

  serializeJson(doc, payload);

  https.POST(payload);

  deserializeJson(doc, https.getString());
  String output = doc["data"]["id"];

  doc.clear();
  // doc.garbageCollect(); // garbageCollect is not required in ArduinoJson v7
  https.end();

  return output;
}

// Returns Workplace ID (WID)
const String Toggl::getWorkSpace()
{

  String   Output{};
  uint16_t HTTP_Code{};

  HTTPClient https;
  https.begin(BaseUrl + "/workspaces", root_ca);
  https.addHeader("Authorization", AuthorizationKey, true);

  HTTP_Code = https.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {

    DynamicJsonDocument doc(1024);

    StaticJsonDocument<50> filter;
    filter[0]["id"]   = true;
    filter[0]["name"] = true;

    deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

    JsonArray arr = doc.as<JsonArray>();

    for (JsonVariant value : arr)
    {

      const int TmpID{value["id"]};
      Output += TmpID;
      Output += "\n";
      String TmpName = value["name"];
      Output += TmpName + "\n" + "\n";
    }
  }

  else
  {
    Output = ("Error: " + String(HTTP_Code));
  }

  https.end();
  return Output;
}

const String Toggl::getProject(int const & WID)
{

  String   Output{};
  uint16_t HTTP_Code{};

  DynamicJsonDocument doc(1024);

  StaticJsonDocument<50> filter;
  filter[0]["id"]   = true;
  filter[0]["name"] = true;
  HTTPClient https;
  https.begin("https://api.track.toggl.com/api/v8/workspaces/" + String(WID) + "/projects", root_ca);
  https.addHeader("Authorization", AuthorizationKey, true);

  HTTP_Code = https.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {

    deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

    JsonArray arr = doc.as<JsonArray>();

    for (JsonVariant value : arr)
    {

      const int TmpID{value["id"]};
      Output += TmpID;
      Output += "\n";
      String TmpName = value["name"];
      Output += TmpName + "\n" + "\n";
    }
  }

  else
  {
    Output = ("Error: " + String(HTTP_Code));
  }

  https.end();
  return Output;
}

/*
//ToDo make the code somewhat nicer.
const int Toggl::getPID(String const& WID ,String const& ProjectName){

  int output{};

  String TMP_String = getProject(WID);

  Serial.println(TMP_String);



  return output;
}
*/

const String Toggl::getTimerData(String Input)
{

  String  payload{};
  String  Output{};
  int16_t HTTP_Code{};

  HTTPClient https;
  https.begin("https://api.track.toggl.com/api/v8/time_entries/current", root_ca);
  https.addHeader("Authorization", AuthorizationKey);

  HTTP_Code = https.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {
    StaticJsonDocument<46> filter;
    filter["data"][Input] = true;

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(4));

    deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

    const String TMP_Str = doc["data"][Input];
    Output               = TMP_Str;
  }

  else
  { // To return the error instead of the data, no idea why the built in espHttpClient "errorToString" only returns blank space when a known error occurs...
    Output = ("Error: " + String(HTTP_Code));
  }

  https.end();
  return Output;
}

// This got to go...
const String Toggl::getCurrentTime(const String Timezone)
{

  int16_t    HTTP_Code{};
  String     Output{};
  HTTPClient http;

  http.begin("https://timeapi.io/api/time/current/zone?timeZone=" + Timezone, timeapi_io_ca);

  HTTP_Code = http.GET();

  if (HTTP_Code >= 200 && HTTP_Code <= 226)
  {
    StaticJsonDocument<46> filter;
    filter["dateTime"] = true;

    const size_t        capacity = JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, http.getString(), DeserializationOption::Filter(filter));

    const String TMP_Str = doc["dateTime"];
    Output               = TMP_Str;
  }

  else
  {
    HTTP_Code;
  }

  http.end();
  return Output;
}

/*
 * Since the duration is in the epoch time format i need to convert it to regular secconds.
 * This is done by taking "current time" + "Duration" resulting in duration in secconds.
 *
 * The JSON request for getting the time when the timer started does not include the time zone....
 *
 * This function makes me cry :'(
 */

// Not even sure if i can do this properly. Il just use the World Time API for now...
const int32_t Toggl::getTimerDuration()
{

  uint32_t      Output{};
  const int32_t Duration = (getTimerData("duration")).toInt();

  if (Duration < 0)
  {
    // Output = getCurrentTime(getTimezone()) + Duration;
  }

  else
  {
    Output = 0;
  }

  return Output;
}

const bool Toggl::isTimerActive()
{

  bool output;

  String wid = getTimerData("wid"); // Just using a filter for less data.

  if (wid != "null")
  {
    output = true;
  }

  else
  {
    output = false;
  }

  return output;
}

const String Toggl::getTimerID()
{

  return getTimerData("id");
}

// ToDo: For all GET requests. Better memory handling
// GET requests for user Data

const uint16_t Toggl::getID()
{

  const uint16_t output = (getUserData("id")).toInt();

  return output;
}

const String Toggl::getApiToken()
{

  return getUserData("api_token");
}

const uint16_t Toggl::getDefaultWid()
{

  const uint16_t output = (getUserData("default_wid")).toInt();

  return output;
}

const String Toggl::getEmail()
{

  return getUserData("email");
}

const String Toggl::getFullName()
{

  return getUserData("fullname");
}

const String Toggl::getJqTimeOfDayFormat()
{

  return getUserData("jquery_timeofday_format");
}

const String Toggl::getJqDateFormat()
{

  return getUserData("jquery_date_format");
}

const String Toggl::getTimeOfDayFormat()
{

  return getUserData("timeofday_format");
}

const String Toggl::getDateFormat()
{

  return getUserData("date_format");
}

const bool Toggl::getStoreStartAndStopTime()
{

  return getUserData("store_start_and_stop_time");
}

const uint16_t Toggl::getBeginningOfWeek()
{

  // Not sure why a uint8_t creates a stack overflow
  const uint16_t output = (getUserData("beginning_of_week")).toInt();

  return output;
}

const String Toggl::getLang()
{

  return getUserData("language");
}

const String Toggl::getDurationFormat()
{

  return getUserData("duration_format");
}

const String Toggl::getAt()
{

  return getUserData("at");
}

const String Toggl::getCreation()
{

  return getUserData("created_at");
}

const String Toggl::getTimezone()
{

  return getUserData("timezone");
}

#endif
