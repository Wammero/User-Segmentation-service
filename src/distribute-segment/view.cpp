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

class DistributeSegment final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-distribute-segment";

  DistributeSegment(const userver::components::ComponentConfig& config,
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
        auto segment_id = request.GetArg("segment_id");
        auto percentage = request.GetArg("percentage");

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT percentage FROM segments WHERE segment_id = $1",
            std::stoi(segment_id)
        );

        if(result.IsEmpty()) {
            return "BAD";
        }

        int table_percentage = result.AsSingleRow<int>();

        if(table_percentage >= std::stoi(percentage)) {
            return "{\"status\": \"OK\"}";
        }

        pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE segments SET percentage = $1 WHERE segment_id = $2",
            std::stoi(percentage), std::stoi(segment_id)
        );

        auto count = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT COUNT(*) FROM users"
        );
        int total_users = count.AsSingleRow<int>();

        auto current_segment_users = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT COUNT(*) FROM user_segments WHERE segment_id = $1",
            std::stoi(segment_id)
        );

        int target_users_in_segment = std::stoi(percentage) * total_users / 100;

        int users_to_add = target_users_in_segment - current_segment_users.AsSingleRow<int>();

        if (users_to_add > 0) {
            pg_cluster_->Execute(
                userver::storages::postgres::ClusterHostType::kMaster,
                "INSERT INTO user_segments (user_id, segment_id) "
                "SELECT users.user_id, $1 FROM users "
                "LEFT JOIN user_segments ON users.user_id = user_segments.user_id AND user_segments.segment_id = $1 "
                "WHERE user_segments.user_id IS NULL "
                "LIMIT $2",
                std::stoi(segment_id), users_to_add
            );
        }

        return "{\"status\": \"OK\"}";

    } catch (std::exception& ex) {
        return ex.what();
    }
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendDistributeSegment(userver::components::ComponentList& component_list) {
  component_list.Append<DistributeSegment>();
}

}  // namespace user_segmentation_service