#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace user_segmentation_service {

std::string BuildDbRequest(const std::optional<int>& segment_id);

void AppendGetUsers(userver::components::ComponentList& component_list);

}  // namespace user_segmentation_service
