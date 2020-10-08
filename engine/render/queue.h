#pragma once

#include <vector>
#include <iostream>

#include "memory/allocator.h"
#include "render/renderable.h"
#include "render/context.h"
#include "render/queue.h"

namespace wage {
  namespace render {

    /**
     * Queue used to store a list of renderables to flush to the screen. All items are store sequentially as they are added.
     * When desired, the items can be culled fom the queue using a provided frustum. Eventually z-sorting will be added.
     */
    class RenderQueue {

    public:
      /**
       * Create a new render queue.
       */
      RenderQueue() : allocator("RenderTemp", 1024 * 1024 * 5) {
        renderables.reserve(100);
      }

      // virtual ~RenderQueue() {
      //   allocator.clear();
      // }

      /**
       * Add a renderable to the queue. This will create a new instance of the renderable in the allocator
       * space and will enqueue it for rendering.
       */
      template <typename T, typename... Args>
      void add(Args... args) {
        auto instance = allocator.create<T>(args...);
        renderables.push_back(instance);
      }

      /**
       * Remove any items from the queue by frustum culling using the render context.
       */
      virtual void cull(RenderContext* context) {
        std::vector<Renderable*> valid;
        math::Frustum frustum = context->cameraFrustum();
        for (auto renderable : renderables) {
          if (frustum.contains(renderable->boundingSphere())) {
            valid.push_back(renderable);
          }
        }
        renderables = valid;
      }

      /**
       * Sort the render queue. No Worky.
       */
      virtual void sort(RenderContext* context) {
        // TODO: Something smart that makes the world a better place.
      }

      /**
       * Render the items from the queue to the screen.
       */
      virtual void render(RenderContext* context) {
        for (auto renderable : renderables) {
          renderable->render(context);
        }
      }

      /**
       * Clear the render queue and any memory used by the queue.
       */
      virtual void clear() {
        renderables.clear();
        allocator.clear();
      }

      /**
       * Check the count of the render queue.
       */
      inline size_t count() {
        return renderables.size();
      }

    private:
      std::vector<Renderable*> renderables;

      memory::Allocator allocator;
    };

  }
}