#ifndef TIME_ENTRY_H
#define TIME_ENTRY_H

#include <string>
#include <vector>
#include <ArduinoJson.h>

class TimeEntry {
private:
    std::string at;
    bool billable;
    std::string client_name;
    std::string description;
    int duration;
    bool duronly;
    unsigned int id = 0;
    std::vector<std::string> permissions;
    int pid;
    bool project_active;
    bool project_billable;
    std::string project_color;
    int project_id;
    std::string project_name;
    std::vector<JsonObject> shared_with;
    std::string start;
    std::string stop;
    std::vector<int> tag_ids;
    std::vector<std::string> tags;
    int task_id;
    std::string task_name;
    int tid;
    int uid;
    std::string user_avatar_url;
    int user_id;
    std::string user_name;
    int wid;
    int workspace_id;

public:
    // Constructor
    TimeEntry() = default;

    // Getters
    std::string getAt() const { return at; }
    bool isBillable() const { return billable; }
    std::string getClientName() const { return client_name; }
    std::string getDescription() const { return description; }
    int getDuration() const { return duration; }
    bool isDuronly() const { return duronly; }
    unsigned int getId() const { return id; }
    const std::vector<std::string>& getPermissions() const { return permissions; }
    int getPid() const { return pid; }
    bool isProjectActive() const { return project_active; }
    bool isProjectBillable() const { return project_billable; }
    std::string getProjectColor() const { return project_color; }
    int getProjectId() const { return project_id; }
    std::string getProjectName() const { return project_name; }
    std::string getStart() const { return start; }
    std::string getStop() const { return stop; }
    const std::vector<int>& getTagIds() const { return tag_ids; }
    const std::vector<std::string>& getTags() const { return tags; }
    int getTaskId() const { return task_id; }
    std::string getTaskName() const { return task_name; }
    int getWorkspaceId() const { return workspace_id; }

    // Setters
    void setAt(const std::string& value) { at = value; }
    void setBillable(bool value) { billable = value; }
    void setClientName(const std::string& value) { client_name = value; }
    void setDescription(const std::string& value) { description = value; }
    void setDuration(int value) { duration = value; }
    void setId(unsigned int value) { id = value; }
    void setStart(const std::string& value) { start = value; }
    void setStop(const std::string& value) { stop = value; }
    void setWorkspaceId(int value) { workspace_id = value; }
    void setProjectName(const std::string& value) { project_name = value; }


    // JSON serialization methods
    void fromJson(const JsonDocument json);
    void toJson(JsonDocument & json) const;
};

#endif // TIME_ENTRY_H