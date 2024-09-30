#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace user_segmentation_service {

void AppendDeleteUser(userver::components::ComponentList& component_list);

}  // namespace user_segmentation_service
