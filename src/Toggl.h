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

  // Misc
  const String getWorkSpace();
  const String getProject(int const & WID);
  // const int       getPID(String const& WID ,String const& ProjectName);
  const String CreateTag(String const & Name, int const & WID);

  /** Stop a time entry
   * @param timeEntry TimeEntry object to stop
   */
  const String  StopTimeEntry(TimeEntry const timeEntry);

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
  const String  CreateTimeEntry(String const & Description, String const & Tags, int const & Duration, String const & Start, int const & PID, String const & CreatedWith, int workspaceID, TimeEntry * timeEntry);

/**
 * @brief Get the Current Time Entry object
 * 
 * @param timeEntry 
 * @return const String 
 */
  const String GetCurrentTimeEntry(TimeEntry * timeEntry);

  const int32_t getTimerDuration();
  unsigned int getTimerID();
  const bool    isTimerActive();

  // General functionality
  void         setAuth(String const & Token);

private:
  const String getUserData(String Input);
  String       AuthorizationKey{};
  const char * Fingerprint{"41c40c6a907d364b26d40d40d24f0c1b42f126da"}; // Fingerprint valid until 22 April 2021
  const char * root_ca =
      "-----BEGIN CERTIFICATE-----\n"
      "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\n"
      "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
      "-----END CERTIFICATE-----\n";
  const String BaseUrl = "https://api.track.toggl.com/api/v9";

};

#endif
