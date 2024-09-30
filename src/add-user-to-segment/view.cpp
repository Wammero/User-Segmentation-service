#include "view.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

namespace user_segmentation_service {

namespace {

class AddUserToSegment final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-add-user-to-segment";

  AddUserToSegment(const userver::components::ComponentConfig& config,
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

        auto request_body =
            userver::formats::json::FromString(request.RequestBody());

        auto user_id = request.GetPathArg("id");
        auto segment_id = request_body["id"].As<std::optional<int>>();

        auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO user_segments(user_id, segment_id) VALUES ($1, $2) "
            "ON CONFLICT (user_id, segment_id) DO NOTHING "
            "RETURNING user_segment_id",
            std::stoi(user_id), segment_id
        );

        if(result.IsEmpty()) {
            return "BAD";
        }

        return "{\"status\": \"OK\"}";
    } catch (std::exception& ex) {
      return ex.what();
    }
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace


void AppendAddUserToSegment(userver::components::ComponentList& component_list) {
  component_list.Append<AddUserToSegment>();
}

}  // namespace user_segmentation_service
