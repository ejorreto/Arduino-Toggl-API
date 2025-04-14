#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <string>
#include <ArduinoJson.h>

class Workspace {
private:
    int active_users;
    std::string created_at;
    std::string deleted_at;
    int id;
    std::string name;
    int organization_id;
    std::string organization_name;
    std::string role_code;
    int role_id;
    std::string role_name;
    std::string status;
    int total_users;
    std::string updated_at;
    std::string user_status;

public:
    // Constructor
    Workspace() = default;

    // Getters
    int getActiveUsers() const { return active_users; }
    std::string getCreatedAt() const { return created_at; }
    std::string getDeletedAt() const { return deleted_at; }
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getOrganizationId() const { return organization_id; }
    std::string getOrganizationName() const { return organization_name; }
    std::string getRoleCode() const { return role_code; }
    int getRoleId() const { return role_id; }
    std::string getRoleName() const { return role_name; }
    std::string getStatus() const { return status; }
    int getTotalUsers() const { return total_users; }
    std::string getUpdatedAt() const { return updated_at; }
    std::string getUserStatus() const { return user_status; }

    // Setters
    void setActiveUsers(int value) { active_users = value; }
    void setCreatedAt(const std::string& value) { created_at = value; }
    void setDeletedAt(const std::string& value) { deleted_at = value; }
    void setId(int value) { id = value; }
    void setName(const std::string& value) { name = value; }
    void setOrganizationId(int value) { organization_id = value; }
    void setOrganizationName(const std::string& value) { organization_name = value; }
    void setRoleCode(const std::string& value) { role_code = value; }
    void setRoleId(int value) { role_id = value; }
    void setRoleName(const std::string& value) { role_name = value; }
    void setStatus(const std::string& value) { status = value; }
    void setTotalUsers(int value) { total_users = value; }
    void setUpdatedAt(const std::string& value) { updated_at = value; }
    void setUserStatus(const std::string& value) { user_status = value; }

    // JSON serialization methods
    void fromJson(const JsonDocument& json);
    void toJson(JsonDocument& json) const;
};

#endif // WORKSPACE_H