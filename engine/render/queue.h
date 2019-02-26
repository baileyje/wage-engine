#ifndef RENDERER_QUEUE_H
#define RENDERER_QUEUE_H

#include <vector>

#include "render/renderable.h"
#include "render/context.h"
#include "render/queue.h"

namespace wage {

  class RenderQueue {
  
  public:

    RenderQueue() {
      renderables.reserve(100);
    }

    virtual ~RenderQueue() {
      for (auto renderable : renderables) {
        delete renderable;
      }
    }

    virtual void add(Renderable* renderable) {
      renderables.push_back(renderable);
    }

    virtual void cull(RenderContext* context) {
      std::vector<Renderable*> valid;
      Frustum frustum = context->camera()->frustum(context->screenSize(), context->cameraEntity().get<Transform>().get());
      int idx = 0;
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
    }

  private:

    std::vector<Renderable*> renderables;

  };

}

#endif //RENDERER_QUEUE_H