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
togglApiErrorCode_t Toggl::getUserData(String Input)
{
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // String  payload{};
  // String  Output{};
  // int16_t HTTP_Code{};

  // HTTPClient https;
  // https.begin(BaseUrl + "/me", root_ca);
  // https.addHeader("Authorization", AuthorizationKey);

  // HTTP_Code = https.GET();

  // if (HTTP_Code >= 200 && HTTP_Code <= 226)
  // {
  //   StaticJsonDocument<80> filter;
  //   filter["data"][Input] = true;

  //   DynamicJsonDocument doc(2 * JSON_OBJECT_SIZE(1) + 60);
  //   deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

  //   String TMP_Str = doc["data"][Input];
  //   Output         = TMP_Str;
  // }

  // else
  // {
  //   Output = ("Error: " + String(HTTP_Code));
  // }

  // https.end();
  // return Output;
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

togglApiErrorCode_t Toggl::CreateTimeEntry(String const & Description, String const & Tags, int const & Duration, String const & Start, int const & projectID, String const & CreatedWith, int workspaceID, TimeEntry * timeEntry)
{
  togglApiErrorCode_t errorCode = TOGGL_API_EC_OK;
  String              payload;
  uint16_t            HTTP_Code{};

  HTTPClient https;
  if (timeEntry != NULL)
  {
    https.begin(BaseUrl + "/workspaces/" + workspaceID + "/time_entries", root_ca);
    /** @todo process https.begin return value */
    https.addHeader("Authorization", AuthorizationKey, true);
    https.addHeader("Content-Type", " application/json");

    JsonDocument doc;

    doc["description"] = Description;
    // doc["tags"]         = Tags;
    doc["duration"] = Duration;
    doc["start"]    = Start;
    if (projectID >= 0)
    {
      doc["project_id"] = projectID;
    }
    doc["created_with"] = CreatedWith;
    doc["workspace_id"] = workspaceID;

    (void)serializeJson(doc, payload);
    /** @todo process serializeJson return value, in case 0 bytes where written to doc */

    HTTP_Code = https.POST(payload);
    if (HTTP_Code == 200)
    {
      /* A new time entry was created successfully and returned, process it and save it in the output variable */
      doc.clear();
      DeserializationError jsonErrorCode = deserializeJson(doc, https.getString());
      if (jsonErrorCode != DeserializationError::Ok)
      {
        Serial.println("Error deserializing JSON: " + String(jsonErrorCode.c_str()));
        errorCode = TOGGL_API_EC_JSON_ERROR;
      }
      else
      {
        // serializeJsonPretty(doc, Serial); // for debugging
        /** @todo check if doc is null? That would mean a null return from Toggl, that should not happen if http error code was 200 */
        timeEntry->fromJson(doc);
        Serial.println("Time entry created with ID: " + String(timeEntry->getId()));
        errorCode = TOGGL_API_EC_OK;
      }
      doc.clear();
    }
    else
    {
      doc.clear();
      Serial.println("Error creating time entry: " + String(HTTP_Code));
      errorCode = httpCodeToErrorCode(HTTP_Code);
    }

    https.end();
  }
  else
  {
    errorCode = TOGGL_API_EC_NULL_INPUT;
  }

  Serial.println("CreateTimeEntry error code: " + String(errorCode));
  return errorCode;
}

togglApiErrorCode_t Toggl::GetCurrentTimeEntry(TimeEntry * timeEntry)
{
  togglApiErrorCode_t errorCode = TOGGL_API_EC_OK;
  int                 HTTP_Code = 0;
  HTTPClient          https;

  https.begin(BaseUrl + "/me/time_entries/current", root_ca);
  /** @todo Process https.begin return value */

  https.addHeader("Authorization", AuthorizationKey, true);
  https.addHeader("Content-Type", " application/json");
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
      if (doc.isNull())
      {
        doc.clear();
        errorCode = TOGGL_API_EC_NO_CURRENT_TIME_ENTRY;
      }
      else
      {
        serializeJsonPretty(doc, Serial); // for debugging
        timeEntry->fromJson(doc);
        Serial.println("Current time entry ID: " + String(timeEntry->getId()));
        doc.clear();
        errorCode = TOGGL_API_EC_OK;
      }
    }
  }
  else
  {
    errorCode = httpCodeToErrorCode(HTTP_Code);
  }

  https.end();

  Serial.println("GetCurrentTimeEntry error code: " + String(errorCode));
  return errorCode;
}

togglApiErrorCode_t Toggl::CreateTag(String const & Name, int const & WID)
{
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  ;
  // /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // String payload;

  // HTTPClient https;
  // https.begin(BaseUrl + "/tags", root_ca);
  // https.addHeader("Authorization", AuthorizationKey, true);
  // https.addHeader("Content-Type", " application/json");

  // DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3));

  // doc["tag"]["name"] = Name;
  // doc["tag"]["wid"]  = WID;

  // serializeJson(doc, payload);

  // https.POST(payload);

  // deserializeJson(doc, https.getString());
  // String output = doc["data"]["id"];

  // doc.clear();
  // // doc.garbageCollect(); // garbageCollect is not required in ArduinoJson v7
  // https.end();

  // return output;
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

togglApiErrorCode_t Toggl::getProject(int const & WID)
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // String   Output{};
  // uint16_t HTTP_Code{};

  // DynamicJsonDocument doc(1024);

  // StaticJsonDocument<50> filter;
  // filter[0]["id"]   = true;
  // filter[0]["name"] = true;
  // HTTPClient https;
  // https.begin("https://api.track.toggl.com/api/v8/workspaces/" + String(WID) + "/projects", root_ca);
  // https.addHeader("Authorization", AuthorizationKey, true);

  // HTTP_Code = https.GET();

  // if (HTTP_Code >= 200 && HTTP_Code <= 226)
  // {

  //   deserializeJson(doc, https.getString(), DeserializationOption::Filter(filter));

  //   JsonArray arr = doc.as<JsonArray>();

  //   for (JsonVariant value : arr)
  //   {

  //     const int TmpID{value["id"]};
  //     Output += TmpID;
  //     Output += "\n";
  //     String TmpName = value["name"];
  //     Output += TmpName + "\n" + "\n";
  //   }
  // }

  // else
  // {
  //   Output = ("Error: " + String(HTTP_Code));
  // }

  // https.end();
  // return Output;
}

togglApiErrorCode_t Toggl::getTimerDuration()
{
  // TODO This should be calculated from the start time and current time
    /** @todo TODO: Not ported to API v9 yet */
    return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return 0;
}

const bool Toggl::isTimerActive()
{
  TimeEntry currentTimeEntry;

  (void)GetCurrentTimeEntry(&currentTimeEntry);

  return (currentTimeEntry.getId() != 0);
}

togglApiErrorCode_t Toggl::getTimerID()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // TimeEntry currentTimeEntry;
  // GetCurrentTimeEntry(&currentTimeEntry);
  // return currentTimeEntry.getId();
}

togglApiErrorCode_t Toggl::getID()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // const uint16_t output = (getUserData("id")).toInt();

  // return output;
}

togglApiErrorCode_t Toggl::getApiToken()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("api_token");
}

togglApiErrorCode_t Toggl::getDefaultWid()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // const uint16_t output = (getUserData("default_wid")).toInt();

  // return output;
}

togglApiErrorCode_t Toggl::getEmail()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("email");
}

togglApiErrorCode_t Toggl::getFullName()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("fullname");
}

togglApiErrorCode_t Toggl::getJqTimeOfDayFormat()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("jquery_timeofday_format");
}

togglApiErrorCode_t Toggl::getJqDateFormat()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("jquery_date_format");
}

togglApiErrorCode_t Toggl::getTimeOfDayFormat()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("timeofday_format");
}

togglApiErrorCode_t Toggl::getDateFormat()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("date_format");
}

togglApiErrorCode_t Toggl::getStoreStartAndStopTime()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("store_start_and_stop_time");
}

togglApiErrorCode_t Toggl::getBeginningOfWeek()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // Not sure why a uint8_t creates a stack overflow
  // const uint16_t output = (getUserData("beginning_of_week")).toInt();

  // return output;
}

togglApiErrorCode_t Toggl::getLang()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("language");
}

togglApiErrorCode_t Toggl::getDurationFormat()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("duration_format");
}

togglApiErrorCode_t Toggl::getAt()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("at");
}

togglApiErrorCode_t Toggl::getCreation()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("created_at");
}

togglApiErrorCode_t Toggl::getTimezone()
{
  /** @todo TODO: Not ported to API v9 yet */
  return TOGGL_API_EC_NOT_PORTED_TO_API_V9;
  // return getUserData("timezone");
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
