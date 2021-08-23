#pragma once

#include <functional>

#include "render-vulkan/render_context.h"

namespace wage::render::vulkan {

  typedef std::function<void(VulkanRenderContext* context)> RenderTask;

}