#ifndef RENDERER_QUEUE_H
#define RENDERER_QUEUE_H

#include <vector>

#include "render/renderable.h"
#include "render/context.h"

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
      // TODO: Something smart that makes the world a better place.
      // Use Camera Frustum to cull items
      printf("Pre-Cull: %d\n", renderables.size());
      std::vector<Renderable*> valid;
      Frustum frustum = context->camera()->frustum(context->screenSize());
      std::cout << "Camera: " << context->camera()->getTransform()->getPosition().x << ":" << context->camera()->getTransform()->getPosition().y << ":" << context->camera()->getTransform()->getPosition().z << std::endl;
      // frustum.debug();
      for (auto renderable : renderables) {
        
        if (frustum.contains(renderable->boundingSphere())) {
          valid.push_back(renderable);
        }        
      }
      printf("Post-Cull: %d\n", valid.size());
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

  private:

    std::vector<Renderable*> renderables;

  };

}

#endif //RENDERER_QUEUE_H