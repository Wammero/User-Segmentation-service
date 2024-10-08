#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "add-segment/view.hpp"
#include "add-user/view.hpp"
#include "get-users/view.hpp"
#include "get-segments/view.hpp"
#include "update-segment/view.hpp"
#include "delete-segment/view.hpp"
#include "delete-user/view.hpp"
#include "add-user-to-segment/view.hpp"
#include "delete-user-from-segment/view.hpp"
#include "distribute-segment/view.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<userver::components::Postgres>("postgres-db-1")
          .Append<userver::clients::dns::Component>();


  user_segmentation_service::AppendAddSegment(component_list);
  user_segmentation_service::AppendAddUser(component_list);
  user_segmentation_service::AppendGetUsers(component_list);
  user_segmentation_service::AppendGetSegments(component_list);
  user_segmentation_service::AppendUpdateSegment(component_list);
  user_segmentation_service::AppendDeleteSegment(component_list);
  user_segmentation_service::AppendDeleteUser(component_list);
  user_segmentation_service::AppendAddUserToSegment(component_list);
  user_segmentation_service::AppendDeleteUserFromSegment(component_list);
  user_segmentation_service::AppendDistributeSegment(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
