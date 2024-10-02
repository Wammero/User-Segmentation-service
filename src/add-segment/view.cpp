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

class AddSegment final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-add-segment";

  AddSegment(const userver::components::ComponentConfig& config,
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

      auto json_request_body = request.RequestBody();
      if(json_request_body.empty()) {
          request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
          return "No arguments.";
      }

      auto request_body = userver::formats::json::FromString(json_request_body);

      auto name = request_body["name"].As<std::optional<std::string>>();

      if(!name) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "No arguments.";
      }

      auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO segments(segment_name, percentage) VALUES ($1, $2) "
        "ON CONFLICT (segment_name) DO NOTHING "
        "RETURNING segment_id",
        name, 0
      );

      if(result.IsEmpty()) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "Bad request.";
      }

      return "{\"status\": \"OK\"}";
      } catch (std::exception& ex) {
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "Bad request.";
      }
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace


void AppendAddSegment(userver::components::ComponentList& component_list) {
  component_list.Append<AddSegment>();
}

}  // namespace user_segmentation_service
