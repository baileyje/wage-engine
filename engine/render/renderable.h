#pragma once

#include "math/vector.h"

#include "render/context.h"

namespace wage { namespace render {

  class Renderable {

  public:
    Renderable() {}

    virtual ~Renderable() {}

    virtual void render(RenderContext* context) = 0;

    virtual math::Vector position() = 0;

    virtual math::BoundingBox boundingBox() = 0;

    virtual math::BoundingSphere boundingSphere() = 0;
  };

} }
