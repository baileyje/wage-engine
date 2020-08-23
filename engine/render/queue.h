#pragma once

#include <vector>
#include <iostream>

#include "memory/allocator.h"
#include "render/renderable.h"
#include "render/context.h"
#include "render/queue.h"

namespace wage {
  namespace render {

    class RenderQueue {

    public:
      RenderQueue() : allocator("RenderTemp1", 1024 * 1024 * 50) {
        renderables.reserve(100);
      }

      virtual ~RenderQueue() {
        for (auto renderable : renderables) {
          delete renderable;
        }
      }

      template <typename T, typename... Args>
      void add(Args... args) {
        auto instance = allocator.create<T>(args...);
        renderables.push_back(instance);
      }

      virtual void cull(RenderContext* context) {
        std::vector<Renderable*> valid;
        math::Frustum frustum = context->camera()->frustum(context->screenSize(), context->cameraEntity().get<math::Transform>(TransformComponent));
        for (auto renderable : renderables) {
          if (frustum.contains(renderable->boundingSphere())) {
            valid.push_back(renderable);
          }
        }
        renderables = valid;
      }

      virtual void sort(RenderContext* context) {
        // TODO: Something smart that makes the world a better place.
      }

      virtual void render(RenderContext* context) {
        for (auto renderable : renderables) {
          renderable->render(context);
        }
      }

      virtual void clear() {
        renderables.clear();
        allocator.clear();
      }

      inline size_t count() {
        return renderables.size();
      }

    private:
      std::vector<Renderable*> renderables;

      memory::Allocator allocator;
    };

  }
}