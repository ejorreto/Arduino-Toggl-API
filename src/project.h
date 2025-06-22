#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include <ArduinoJson.h>

class Project {
private:
    bool active;
    int actual_hours;
    int actual_seconds;
    std::string at;
    bool auto_estimates;
    bool billable;
    bool can_track_time;
    int cid;
    int client_id;
    std::string client_name;
    std::string color;
    std::string created_at;
    std::string deleted_at;  // Add after created_at
    std::string currency;
    JsonObject current_period;
    std::string end_date;
    int estimated_hours;
    int estimated_seconds;
    std::string external_reference;
    float fixed_fee;
    int id;
    std::string integration_ext_id;
    std::string integration_ext_type;
    std::string integration_provider;
    bool is_private;
    std::string name;
    std::string permissions;
    bool pinned;
    float rate;
    std::string rate_last_updated;
    bool recurring;
    std::vector<JsonObject> recurring_parameters;
    std::string start_date;
    std::string status;
    bool template_flag;
    int template_id;
    int total_count;
    int wid;
    int workspace_id;

public:
    // Constructor
    Project() = default;

    // Getters
    bool isActive() const { return active; }
    int getActualHours() const { return actual_hours; }
    int getActualSeconds() const { return actual_seconds; }
    std::string getAt() const { return at; }
    
    bool hasAutoEstimates() const { return auto_estimates; }
    bool isBillable() const { return billable; }
    bool canTrackTime() const { return can_track_time; }
    int getClientId() const { return client_id; }
    std::string getClientName() const { return client_name; }
    std::string getColor() const { return color; }
    std::string getCreatedAt() const { return created_at; }
    std::string getDeletedAt() const { return deleted_at; }  // Add with other getters

    std::string getEndDate() const { return end_date; }
    std::string getExternalReference() const { return external_reference; }
    float getFixedFee() const { return fixed_fee; }
    int getId() const { return id; }
    bool isPrivate() const { return is_private; }
    std::string getName() const { return name; }
    std::string getPermissions() const { return permissions; }
    bool isPinned() const { return pinned; }
    float getRate() const { return rate; }
    bool isRecurring() const { return recurring; }
    std::string getStartDate() const { return start_date; }
    std::string getStatus() const { return status; }
    int getWorkspaceId() const { return workspace_id; }

    // Setters
    void setActive(bool value) { active = value; }
    void setActualHours(int value) { actual_hours = value; }
    void setActualSeconds(int value) { actual_seconds = value; }
    void setAt(const std::string& value) { at = value; }
    void setDeletedAt(const std::string& value) { deleted_at = value; }  // Add with other setters

    void setAutoEstimates(bool value) { auto_estimates = value; }
    void setBillable(bool value) { billable = value; }
    void setClientId(int value) { client_id = value; }
    void setClientName(const std::string& value) { client_name = value; }
    void setColor(const std::string& value) { color = value; }
    void setId(int value) { id = value; }
    void setName(const std::string& value) { name = value; }
    void setWorkspaceId(int value) { workspace_id = value; }

    // JSON serialization methods
    void fromJson(const JsonDocument& json);
    void toJson(JsonDocument& json) const;
};

#endif // PROJECT_H