#include "time_entry.h"

void TimeEntry::fromJson(const JsonDocument json) {
    if (!json.isNull()) {
        if (json.containsKey("id")) id = json["id"].as<unsigned int>();
        if (json.containsKey("workspace_id")) workspace_id = json["workspace_id"].as<int>();
        if (json.containsKey("project_id") && !json["project_id"].isNull()) 
            project_id = json["project_id"].as<int>();
        if (json.containsKey("task_id") && !json["task_id"].isNull()) 
            task_id = json["task_id"].as<int>();
        if (json.containsKey("billable")) billable = json["billable"].as<bool>();
        if (json.containsKey("start")) start = json["start"].as<const char*>();
        if (json.containsKey("stop") && !json["stop"].isNull()) 
            stop = json["stop"].as<const char*>();
        if (json.containsKey("duration")) duration = json["duration"].as<int>();
        if (json.containsKey("description")) description = json["description"].as<const char*>();
        if (json.containsKey("duronly")) duronly = json["duronly"].as<bool>();
        if (json.containsKey("at")) at = json["at"].as<const char*>();
        if (json.containsKey("user_id")) user_id = json["user_id"].as<int>();
        if (json.containsKey("wid")) wid = json["wid"].as<int>();
        if (json.containsKey("project_name") && !json["project_name"].isNull())
            project_name = json["project_name"].as<const char*>();
    }
}

void TimeEntry::toJson(JsonDocument & json) const {
    json.clear();
    json["id"] = id;
    json["workspace_id"] = workspace_id;
    json["project_id"] = nullptr;
    json["task_id"] = nullptr;
    json["billable"] = billable;
    json["start"] = start;
    json["stop"] = nullptr;
    json["duration"] = duration;
    json["description"] = description;
    json["tags"] = nullptr;
    json["tag_ids"] = nullptr;
    json["duronly"] = duronly;
    json["at"] = at;
    json["user_id"] = user_id;
    json["wid"] = wid;
    json["project_name"] = project_name;
}