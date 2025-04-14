#include "workspace.h"

void Workspace::fromJson(const JsonDocument& json) {
    active_users = json["active_users"] | 0;
    created_at = json["created_at"].as<std::string>();
    deleted_at = json["deleted_at"].as<std::string>();
    id = json["id"] | 0;
    name = json["name"].as<std::string>();
    organization_id = json["organization_id"] | 0;
    organization_name = json["organization_name"].as<std::string>();
    role_code = json["role_code"].as<std::string>();
    role_id = json["role_id"] | 0;
    role_name = json["role_name"].as<std::string>();
    status = json["status"].as<std::string>();
    total_users = json["total_users"] | 0;
    updated_at = json["updated_at"].as<std::string>();
    user_status = json["user_status"].as<std::string>();
}

void Workspace::toJson(JsonDocument& json) const {
    json["active_users"] = active_users;
    json["created_at"] = created_at;
    json["deleted_at"] = deleted_at;
    json["id"] = id;
    json["name"] = name;
    json["organization_id"] = organization_id;
    json["organization_name"] = organization_name;
    json["role_code"] = role_code;
    json["role_id"] = role_id;
    json["role_name"] = role_name;
    json["status"] = status;
    json["total_users"] = total_users;
    json["updated_at"] = updated_at;
    json["user_status"] = user_status;
}