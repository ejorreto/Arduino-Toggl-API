# Arduino Toggl API

<!-- ![Passing](https://img.shields.io/badge/build-passing-brightgreen)![GitHub last commit](https://img.shields.io/github/last-commit/JoeyStrandnes/Arduino-Toggl-API)![GitHub issues](https://img.shields.io/github/issues/JoeyStrandnes/Arduino-Toggl-API)![CodeFactor](https://img.shields.io/codefactor/grade/github/JoeyStrandnes/Arduino-Toggl-API) -->

![Banner](https://i.imgur.com/Mr9Amfs.png)



Port of Toggl's official API v9 https://engineering.toggl.com/docs/

***Most used API features are implemented***

This is a modification on the great work from Joey Strandnes porting of the Toggl API v8 to make it work with the v9. I'm moving to a class based implementation to allow integrating this library in bigger projects, and removing some functionalities that should not be handled by an API (like connection management, and time management.)

This is currently a work in progress to add as much functionality as possible, merge requests are welcome to add the missing functions.

Note: I've removed the ESP8266 implementation to focus on the ESP32 one.



# Get User Details

Outdated, check the code for the new methods definitions.

```c++
getID();                    //Returns ID as integer
getApiToken();              //Returns API Token as String
getDefaultWid();            //Return the default Workspace ID as Int
getEmail();                 //Returns Email as String   
getFullName();              //Returns Name of user as String
getJqTimeOfDayFormat();     //Returns Time of day JQ formatting as String
getJqDateFormat();          //Returns date JQ formatting as String
getTimeOfDayFormat();       //Returns Time of day formatting as String
getDateFormat();            //Returns date formatting as String
getStoreStartAndStopTime(); //Returns START/STOP time as bool
getBeginningOfWeek();       //Returns The day the weeks starts on as uint
getLang();                  //Returns The language used as String
getDurationFormat();        //Returns Duration formatting as String
getAt();                    //Returns AT? as String
getCreation();              //Returns Account creation date as String
getTimezone();              //Returns Time zone as string
```
# Create/Start Time Entries

Outdated, check the code for the new methods definitions.

```c++
StartTimeEntry(String Description, String Tags, int PID, String CreatedWith); 	// Returns the timer ID as a String
StopTimeEntry(String ID);	//Returns HTTP error in form of string
CreateTimeEntry(String Description, String Tags, int Duration, String Start, int PID, String CreatedWith); // Returns the timer ID as a String
getTimerDuration();		//Returns the duration of the active timer in secconds. Returns 0 if no timer is active.
getTimerID();			//Returns the active timers ID as String. Returns "null" if no timer is active.
isTimerActive();		//Returns a bool if timer is active.

```

# Misc

Outdated, check the code for the new methods definitions.

```c++
getWorkSpace(); 					// Returns a "pretty" string of all workspaces and ID's
getProject(int const& WID); 		// Returns a "pretty" string of all project is specific workplace
CreateTag(String Name, int WID); 	// Requires the Workspace ID
```



# General Setup

Outdated, check the code for the new methods definitions.

```c++
setAuth(String const Token);                //Creates Basic authentication key
```

# Dependencies
```markdown
ArduinoJson v7
ESP32 default libraries
```
# Examples

Examples of use of the original v8 porting. New ones for this implementation of v9 are pending. Stay tuned.

- [Start Time Entry:](https://github.com/JoeyStrandnes/Arduino-Toggl-API/tree/master/examples/Start_Time_Entry) Runs a timer for ~10s.
- [Create a Time Entry:](https://github.com/JoeyStrandnes/Arduino-Toggl-API/tree/master/examples/Create_Time_Entry)  Create a time entry with duration 1 hour. 
- [Button Timer:](https://github.com/JoeyStrandnes/Arduino-Toggl-API/tree/master/examples/Button_Timer) Starts a timer and LED when a button is pressed. Stops timer and turns of LED when it is pressed again.
- [Advanced Button Timer:](https://github.com/JoeyStrandnes/Arduino-Toggl-API/tree/master/examples/Advanced_Button_Timer) Does everything that the regular button timer does but will "sync" with other active timers. It will know if a timer is started and will know if it is stopped. The LED and button function will adjust acording to the timer status.

