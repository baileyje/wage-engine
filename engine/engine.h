#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "math/vector.h"
#include "math/matrix.h"

#include "memory/allocator.h"

#include "core/core.h"

#include "ecs/registry.h"
#include "ecs/system.h"
#include "ecs/system_manager.h"

#include "platform/platform.h"
#include "platform/window.h"

#include "input/input.h"
#include "input/key_event.h"
#include "input/key.h"
#include "input/key_modifier.h"
#include "input/mouse_button_event.h"
#include "input/mouse_move_event.h"
#include "input/mouse_scroll_event.h"

#include "fs/file_system.h"
#include "fs/local.h"

#include "assets/fs_manager.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"

#include "render/renderer.h"
#include "render/mesh_renderer.h"
#include "render/components/mesh.h"
#include "render/components/material.h"

#include "component/camera/perspective_camera.h"
#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"

#include "ui/ui.h"
#include "ui/system.h"

#include "audio/audio.h"

#include "util/random.h"

#endif //ENGINE_H