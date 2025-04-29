#include "project.h"

void Project::fromJson(const JsonDocument& json) {
  if (!json.isNull()) {
      if (json.containsKey("active")) active = json["active"].as<bool>();
      if (json.containsKey("actual_hours")) actual_hours = json["actual_hours"].as<int>();
      if (json.containsKey("actual_seconds")) actual_seconds = json["actual_seconds"].as<int>();
      if (json.containsKey("at")) at = json["at"].as<const char*>();
      if (json.containsKey("auto_estimates") && !json["auto_estimates"].isNull())
          auto_estimates = json["auto_estimates"].as<bool>();
      if (json.containsKey("billable")) billable = json["billable"].as<bool>();
      if (json.containsKey("can_track_time")) can_track_time = json["can_track_time"].as<bool>();
      if (json.containsKey("client_id") && !json["client_id"].isNull()) 
          client_id = json["client_id"].as<int>();
      if (json.containsKey("client_name") && !json["client_name"].isNull())
          client_name = json["client_name"].as<const char*>();
      if (json.containsKey("color")) color = json["color"].as<const char*>();
      if (json.containsKey("created_at")) created_at = json["created_at"].as<const char*>();
      if (json.containsKey("currency") && !json["currency"].isNull())
          currency = json["currency"].as<const char*>();
      if (json.containsKey("estimated_hours") && !json["estimated_hours"].isNull())
          estimated_hours = json["estimated_hours"].as<int>();
      if (json.containsKey("estimated_seconds") && !json["estimated_seconds"].isNull())
          estimated_seconds = json["estimated_seconds"].as<int>();
      if (json.containsKey("fixed_fee") && !json["fixed_fee"].isNull())
          fixed_fee = json["fixed_fee"].as<float>();
      if (json.containsKey("id")) id = json["id"].as<int>();
      if (json.containsKey("is_private")) is_private = json["is_private"].as<bool>();
      if (json.containsKey("name")) name = json["name"].as<const char*>();
      if (json.containsKey("pinned")) pinned = json["pinned"].as<bool>();
      if (json.containsKey("rate") && !json["rate"].isNull())
          rate = json["rate"].as<float>();
      if (json.containsKey("rate_last_updated") && !json["rate_last_updated"].isNull())
          rate_last_updated = json["rate_last_updated"].as<const char*>();
      if (json.containsKey("recurring")) recurring = json["recurring"].as<bool>();
      if (json.containsKey("server_deleted_at") && !json["server_deleted_at"].isNull())
          deleted_at = json["server_deleted_at"].as<const char*>();
      if (json.containsKey("start_date")) start_date = json["start_date"].as<const char*>();
      if (json.containsKey("status")) status = json["status"].as<const char*>();
      if (json.containsKey("template") && !json["template"].isNull())
          template_flag = json["template"].as<bool>();
      if (json.containsKey("template_id") && !json["template_id"].isNull())
          template_id = json["template_id"].as<int>();
      if (json.containsKey("workspace_id")) workspace_id = json["workspace_id"].as<int>();
      if (json.containsKey("wid")) wid = json["wid"].as<int>();
  }
}
void Project::toJson(JsonDocument& json) const {
    json.clear();
    json["active"] = active;
    json["actual_hours"] = actual_hours;
    json["actual_seconds"] = actual_seconds;
    json["at"] = at;
    json["auto_estimates"] = auto_estimates;
    json["billable"] = billable;
    json["can_track_time"] = can_track_time;
    json["client_id"] = client_id;
    json["client_name"] = client_name;
    json["color"] = color;
    json["created_at"] = created_at;
    json["id"] = id;
    json["is_private"] = is_private;
    json["name"] = name;
    json["workspace_id"] = workspace_id;
    json["wid"] = wid;
}