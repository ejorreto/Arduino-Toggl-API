/*
 * Arduino ESP32 Toggl API v9 Port
 * Using ArduinoJson Version 7
 * Using Toggl API v9
 * https://engineering.toggl.com/docs/
 */

#ifndef TOGGL_H
#define TOGGL_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Dependencies for ESP32
#include "WiFi.h"
#include "time_entry.h"
#include "workspace.h"
#include <HTTPClient.h>
#include <base64.h>

/**
 * @brief Toggl API error codes
 *
 */
typedef enum
{
  TOGGL_API_EC_OK = 0,               /*!< No errors. HTTP operation returned 200 */
  TOGGL_API_EC_FORBIDDEN,            /*!< Forbidden. HTTP operation returned 403 */
  TOGGL_API_EC_NOT_FOUND,            /*!< Not found. HTTP operation returned 404 */
  TOGGL_API_EC_ALREADY_STOPPED,      /*!< Already stopped. HTTP operation returned 409 */
  TOGGL_API_EC_SERVER_ERROR,         /*!< Server error. HTTP operation returned 500 */
  TOGGL_API_EC_TOO_MANY_WORKSPACES,  /*!< Too many workspaces received for the allocated space */
  TOGGL_API_EC_UNKNOWN_ERROR,        /*!< Unknown error. */
  TOGGL_API_EC_NULL_INPUT,           /*!< Null input passed to the function */
  TOGGL_API_EC_NOT_PORTED_TO_API_V9, /*!< Function not ported to API v9 yet */
  TOGGL_API_EC_JSON_ERROR            /*!< JSON error. Error serializing/deserializing data */
} togglApiErrorCode_t;

class Toggl
{
public:
  Toggl();

  // Get the induvidual account settings/data
  const uint16_t getID();
  const String   getApiToken();
  const uint16_t getDefaultWid();
  const String   getEmail();
  const String   getFullName();
  const String   getJqTimeOfDayFormat();
  const String   getJqDateFormat();
  const String   getTimeOfDayFormat();
  const String   getDateFormat();
  const bool     getStoreStartAndStopTime();
  const uint16_t getBeginningOfWeek();
  const String   getLang();
  const String   getDurationFormat();
  const String   getAt();
  const String   getCreation();
  const String   getTimezone();

  /**
   * @brief Get the Workspaces for the current user
   *
   * @param workspaces Array of Workspace objects to store the received workspaces
   * @param maxNumWorkspaces Size of the array of Workspaces, max number of workspaces to use
   * @param numWorkspacesReceived Number of workspaces received
   * @return togglApiErrorCode_t
   */
  togglApiErrorCode_t getWorkSpaces(Workspace * workspaces, uint32_t maxNumWorkspaces, uint32_t * numWorkspacesReceived);

  const String getProject(int const & WID);
  // const int       getPID(String const& WID ,String const& ProjectName);
  const String CreateTag(String const & Name, int const & WID);

  /** Stop a time entry
   * @param timeEntry TimeEntry object to stop
   */
  const String StopTimeEntry(TimeEntry const timeEntry);

  /**
   * @brief Create a Time Entry object
   *
   * @param Description
   * @param Tags
   * @param Duration Duration in seconds. Should be -1 for running timers
   * @param Start
   * @param PID
   * @param CreatedWith Name of the app that creates the time entry
   * @param workspaceID
   * @param timeEntry
   * @return const String
   */
  const String CreateTimeEntry(String const & Description, String const & Tags, int const & Duration, String const & Start, int const & PID, String const & CreatedWith, int workspaceID, TimeEntry * timeEntry);

  /**
   * @brief Get the Current Time Entry object
   *
   * @param timeEntry
   * @return const String
   */
  const String GetCurrentTimeEntry(TimeEntry * timeEntry);

  const int32_t getTimerDuration();
  unsigned int  getTimerID();
  const bool    isTimerActive();

  // General functionality
  void setAuth(String const & Token);

private:
  /**
   * @brief Convert HTTP code to Toggl API error code
   *
   * @param httpCode
   * @return togglApiErrorCode_t
   */
  togglApiErrorCode_t httpCodeToErrorCode(int httpCode);
  const String        getUserData(String Input);
  String              AuthorizationKey{};
  const char *        root_ca =
      "-----BEGIN CERTIFICATE-----\n"
      "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\n"
      "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
      "-----END CERTIFICATE-----\n";
  const String BaseUrl = "https://api.track.toggl.com/api/v9";
};

#endif
