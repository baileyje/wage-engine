#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "math/vector.h"

#include "render/context.h"

namespace wage {

  class Renderable {

  public:
    Renderable() {}

    virtual ~Renderable() {}

    virtual void render(RenderContext* context) = 0;

    virtual Vector position() = 0;

    virtual BoundingBox boundingBox() = 0;

    virtual BoundingSphere boundingSphere() = 0;
  };
}

#endif //RENDERABLE_H