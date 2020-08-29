#pragma once

#include "math/vector.h"

#include "render/context.h"

namespace wage {
  namespace render {

    /**
     * Virtual class setting up the contract for what a renderable must be for queuing and rendering to function/
     */
    class Renderable {

    public:
      Renderable() {}

      virtual ~Renderable() {}

      /**
       * Render the renderable with the provided render context.
       */
      virtual void render(RenderContext* context) = 0;

      /**
       * Get the position of the renderable in the world.
       */
      virtual math::Vector position() = 0;

      /**
       * Get the bounding box for the renderable in the world.
       */
      virtual math::BoundingBox boundingBox() = 0;

      /**
       * Get the bounding sphere for the renderable in the world.
       */
      virtual math::BoundingSphere boundingSphere() = 0;
    };

  }
}
