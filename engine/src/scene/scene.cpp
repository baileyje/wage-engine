#include "scene/scene.h"

#include "scene/manager.h"
#include "core/core.h"

namespace wage {
  namespace scene {
    Scene& Scene::current() {
      return core::Core::Instance->get<scene::Manager>()->currentScene();
    }
  }
}