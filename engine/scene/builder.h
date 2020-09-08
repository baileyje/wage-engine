#pragma once

#include <functional>

#include "scene/scene.h"

namespace wage {
  namespace scene {

    typedef std::function<void(Scene&)> SceneBuilder;

  }
}