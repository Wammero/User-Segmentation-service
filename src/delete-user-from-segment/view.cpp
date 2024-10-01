#include "view.hpp"
#include "../validator/view.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

namespace user_segmentation_service {

namespace {

class DeleteUserFromSegment final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-delete-user-from-segment";

  DeleteUserFromSegment(const userver::components::ComponentConfig& config,
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

        auto user_id = request.GetPathArg("user_id");

        if(!request.HasArg("segment_id")) {
          request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
          return "No arguments.";
        }
        
        auto segment_id = request.GetArg("segment_id");

        if(!Validator::IsLimitValid(segment_id) || !Validator::IsLimitValid(user_id)) {
          request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
          return "Bad request.";
        }


        pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "DELETE FROM user_segments "
            "WHERE user_id = $1 AND segment_id = $2",
            std::stoi(user_id), std::stoi(segment_id)
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


void AppendDeleteUserFromSegment(userver::components::ComponentList& component_list) {
  component_list.Append<DeleteUserFromSegment>();
}

}  // namespace user_segmentation_service
