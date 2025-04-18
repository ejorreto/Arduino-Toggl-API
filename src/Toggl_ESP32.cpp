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
  // TODO: Not ported to API v9 yet
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

togglApiErrorCode_t Toggl::StopTimeEntry(TimeEntry const timeEntry)
{
  togglApiErrorCode_t errorCode = TOGGL_API_EC_OK;
  uint16_t            HTTP_Code{};
  String              workspaceId = String(timeEntry.getWorkspaceId());
  String              timeEntryId = String(timeEntry.getId());

  HTTPClient https;
  https.begin(BaseUrl + "/workspaces/" + workspaceId + "/time_entries/" + timeEntryId + "/stop", root_ca);
  /* @todo Process https.begin return value */

  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");
  HTTP_Code = https.PATCH(" ");
  https.end();

  /* @todo: Check if the time entry was stopped correctly */
  errorCode = httpCodeToErrorCode(HTTP_Code);
  Serial.println("StopTimeEntry error code: " + String(errorCode));
  return errorCode;
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
    doc["duration"]     = Duration;
    doc["start"]        = Start;
    doc["project_id"]   = PID;
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
    Serial.println("Current time entry ID: " + String(timeEntry->getId()));
    doc.clear();
  }
  https.end();

  return String(HTTP_Code);
}

const String Toggl::CreateTag(String const & Name, int const & WID)
{
  // TODO: Not ported to API v9 yet
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

togglApiErrorCode_t Toggl::getWorkSpaces(Workspace * workspaces, uint32_t maxNumWorkspaces, uint32_t * numWorkspacesReceived)
{
  togglApiErrorCode_t errorCode = TOGGL_API_EC_OK;
  uint16_t            HTTP_Code{};
  uint32_t            workspaceIndex = 0;
  HTTPClient          https;

  if (workspaces == NULL || numWorkspacesReceived == NULL)
  {
    errorCode = TOGGL_API_EC_NULL_INPUT;
  }
  else
  {
    https.begin(BaseUrl + "/workspaces", root_ca);
    /* TODO process https.begin return code */
    https.addHeader("Authorization", AuthorizationKey, true);

    HTTP_Code = https.GET();

    if (HTTP_Code == 200)
    {
      JsonDocument         doc;
      DeserializationError jsonErrorCode = deserializeJson(doc, https.getString());
      if (jsonErrorCode != DeserializationError::Ok)
      {
        Serial.println("Error deserializing JSON: " + String(jsonErrorCode.c_str()));
        errorCode = TOGGL_API_EC_JSON_ERROR;
      }
      else
      {
        // serializeJsonPretty(doc, Serial); // for debugging
        JsonArray data = doc.as<JsonArray>();
        Serial.println("Number of workspaces received: " + String(data.size()));
        /* TODO handle if the number of workspaces received is higher than maxNumWorkspaces */

        for (JsonVariant item : data)
        {
          if (workspaceIndex >= maxNumWorkspaces)
          {
            break;
          }
          workspaces[workspaceIndex].fromJson(item);
          Serial.println("Workspace received: " + String(workspaces[workspaceIndex].getName().c_str()));

          workspaceIndex++;
        }

        *numWorkspacesReceived = workspaceIndex;
        errorCode              = TOGGL_API_EC_OK;
      }
    }
    else
    {
      errorCode = httpCodeToErrorCode(HTTP_Code);
    }

    https.end();
  }

  Serial.println("getWorkSpaces error code: " + String(errorCode));
  return errorCode;
}

const String Toggl::getProject(int const & WID)
{
  // TODO: Not ported to API v9 yet
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

const int32_t Toggl::getTimerDuration()
{
  // TODO This should be calculated from the start time and current time
  return 0;
}

const bool Toggl::isTimerActive()
{
  TimeEntry currentTimeEntry;

  (void)GetCurrentTimeEntry(&currentTimeEntry);

  return (currentTimeEntry.getId() != 0);
}

unsigned int Toggl::getTimerID()
{
  TimeEntry currentTimeEntry;
  GetCurrentTimeEntry(&currentTimeEntry);
  return currentTimeEntry.getId();
}

// ToDo: For all GET requests. Better memory handling
// GET requests for user Data

const uint16_t Toggl::getID()
{
  // TODO: Not ported to API v9 yet
  const uint16_t output = (getUserData("id")).toInt();

  return output;
}

const String Toggl::getApiToken()
{
  // TODO: Not ported to API v9 yet
  return getUserData("api_token");
}

const uint16_t Toggl::getDefaultWid()
{
  // TODO: Not ported to API v9 yet
  const uint16_t output = (getUserData("default_wid")).toInt();

  return output;
}

const String Toggl::getEmail()
{
  // TODO: Not ported to API v9 yet
  return getUserData("email");
}

const String Toggl::getFullName()
{
  // TODO: Not ported to API v9 yet
  return getUserData("fullname");
}

const String Toggl::getJqTimeOfDayFormat()
{
  // TODO: Not ported to API v9 yet
  return getUserData("jquery_timeofday_format");
}

const String Toggl::getJqDateFormat()
{
  // TODO: Not ported to API v9 yet
  return getUserData("jquery_date_format");
}

const String Toggl::getTimeOfDayFormat()
{
  // TODO: Not ported to API v9 yet
  return getUserData("timeofday_format");
}

const String Toggl::getDateFormat()
{
  // TODO: Not ported to API v9 yet
  return getUserData("date_format");
}

const bool Toggl::getStoreStartAndStopTime()
{
  // TODO: Not ported to API v9 yet
  return getUserData("store_start_and_stop_time");
}

const uint16_t Toggl::getBeginningOfWeek()
{
  // TODO: Not ported to API v9 yet
  // Not sure why a uint8_t creates a stack overflow
  const uint16_t output = (getUserData("beginning_of_week")).toInt();

  return output;
}

const String Toggl::getLang()
{
  // TODO: Not ported to API v9 yet
  return getUserData("language");
}

const String Toggl::getDurationFormat()
{
  // TODO: Not ported to API v9 yet
  return getUserData("duration_format");
}

const String Toggl::getAt()
{
  // TODO: Not ported to API v9 yet
  return getUserData("at");
}

const String Toggl::getCreation()
{
  // TODO: Not ported to API v9 yet
  return getUserData("created_at");
}

const String Toggl::getTimezone()
{
  // TODO: Not ported to API v9 yet
  return getUserData("timezone");
}

togglApiErrorCode_t Toggl::httpCodeToErrorCode(int httpCode)
{
  if (httpCode == 200)
  {
    return TOGGL_API_EC_OK;
  }
  else if (httpCode == 403)
  {
    return TOGGL_API_EC_FORBIDDEN;
  }
  else if (httpCode == 404)
  {
    return TOGGL_API_EC_NOT_FOUND;
  }
  else if (httpCode == 409)
  {
    return TOGGL_API_EC_ALREADY_STOPPED;
  }
  else if (httpCode == 500)
  {
    return TOGGL_API_EC_SERVER_ERROR;
  }
  else
  {
    return TOGGL_API_EC_UNKNOWN_ERROR;
  }
}

#endif
