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

class GetSegments final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-segments";

  GetSegments(const userver::components::ComponentConfig& config,
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

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT * FROM segments"
        );
        
        userver::formats::json::ValueBuilder products_array(userver::formats::json::Type::kArray);

        for(auto& row : result) {
            userver::formats::json::ValueBuilder json_response(userver::formats::json::Type::kObject);
            json_response["segment_id"] = row["segment_id"].As<int>();
            json_response["segment_name"] = row["segment_name"].As<std::string>();
            json_response["percentage"] = row["percentage"].As<int>();

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


void AppendGetSegments(userver::components::ComponentList& component_list) {
  component_list.Append<GetSegments>();
}

}  // namespace user_segmentation_service
