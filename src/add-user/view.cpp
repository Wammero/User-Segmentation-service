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

class AddUser final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-add-user";

  AddUser(const userver::components::ComponentConfig& config,
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

        auto name = request_body["name"].As<std::optional<std::string>>();
        auto email = request_body["email"].As<std::optional<std::string>>();

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO users(username, email) VALUES ($1, $2) "
            "ON CONFLICT (email) DO NOTHING "
            "RETURNING user_id",
            name, email
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


void AppendAddUser(userver::components::ComponentList& component_list) {
  component_list.Append<AddUser>();
}

}  // namespace user_segmentation_service
