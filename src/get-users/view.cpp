#include "view.hpp"

#include <fmt/format.h>
#include <sstream>
#include <optional>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include <userver/formats/json.hpp>

namespace user_segmentation_service {

namespace {

std::string BuildDbRequest(const std::optional<int>& segment_id) {
    std::ostringstream ss;
    ss << "SELECT users.user_id, users.username, users.email FROM users LEFT JOIN user_segments ON users.user_id = user_segments.user_id ";
    if(segment_id.has_value()) ss << "WHERE user_segments.segment_id = $1 ";
    ss << "GROUP BY users.user_id ";
    return ss.str();
}


class GetUsers final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-users";

  GetUsers(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    
    try {
        std::optional<int> segment_id = std::nullopt;
        if(request.HasArg("segment_id")) {
            segment_id = std::stoi(request.GetArg("segment_id"));
        }

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            BuildDbRequest(segment_id),
            segment_id
        );
        
        userver::formats::json::ValueBuilder products_array(userver::formats::json::Type::kArray);

        for(auto& row : result) {
            userver::formats::json::ValueBuilder json_response(userver::formats::json::Type::kObject);
            json_response["user_id"] = row["user_id"].As<int>();
            json_response["username"] = row["username"].As<std::string>();
            json_response["email"] = row["email"].As<std::string>();

            products_array.PushBack(std::move(json_response));
        }
        
        return userver::formats::json::ToPrettyString(products_array.ExtractValue(),
                                                    {' ', 4});

    } catch (std::exception& ex) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "Bad request.";
    }
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace


void AppendGetUsers(userver::components::ComponentList& component_list) {
  component_list.Append<GetUsers>();
}

}  // namespace user_segmentation_service
