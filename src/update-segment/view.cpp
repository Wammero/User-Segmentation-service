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

class UpdateSegment final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-update-segment";

  UpdateSegment(const userver::components::ComponentConfig& config,
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

        auto segment_id = request.GetPathArg("segment_id");
        auto segment_name = request_body["name"].As<std::optional<std::string>>();

        if(!segment_name.has_value()) {
          request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
          return "No arguments.";
        }

        pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE segments SET segment_name = $2 "
            "WHERE segment_id = $1",
            std::stoi(segment_id), segment_name
        );

        return "{\"status\": \"OK\"}";
    } catch (std::exception& ex) {
      request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return "Bad request.";
    }
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace


void AppendUpdateSegment(userver::components::ComponentList& component_list) {
  component_list.Append<UpdateSegment>();
}

}  // namespace user_segmentation_service
