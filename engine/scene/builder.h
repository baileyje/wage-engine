#pragma once

#include <functional>

#include "scene/scene.h"

namespace wage {
  namespace scene {

    /**
     * A scene builder is a function capable of populating the base set of entities and systems for a game scene.
     */
    typedef std::function<void(Scene&)> SceneBuilder;

  }
}