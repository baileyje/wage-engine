
#pragma once

#include <mutex>

#include "math/vector.h"
#include "render/components/material.h"
#include "render/components/mesh.h"
#include "render/components/font.h"
#include "render/queue.h"

namespace wage {
  namespace render {

    /**
     * A single frame to be rendered. There will generally be at least two frames in existence at any given time. The first is the 
     * frame currently being rendered. The second is frame that will be rendered next. The game loop should only have access to the next
     * frame and should have it fully backed prior to the render processing it.
     */
    class Frame {
    public:
      /**
       * Get access to the ui component render queue.
       */
      inline RenderQueue& uiQueue() {
        return _uiQueue;
      }

      /**
       * Get access to the mesh render queue.
       */
      inline RenderQueue& meshQueue() {
        return _meshQueue;
      }

      /**
       * Clear frame data.
       */
      inline void clear() {
        _meshQueue.clear();
        _uiQueue.clear();
      }

      /**
       * Render the frame with with provided render context.
       */
      inline void render(RenderContext* renderContext) {
        renderMeshes(renderContext);
        // TODO: Sprites
        renderUi(renderContext);
      }

    private:
      void renderMeshes(RenderContext* renderContext) {
        _meshQueue.cull(renderContext);
        _meshQueue.sort(renderContext);
        _meshQueue.render(renderContext);
      }

      void renderUi(RenderContext* renderContext) {
        _uiQueue.sort(renderContext);
        _uiQueue.render(renderContext);
      }

      RenderQueue _uiQueue;
      RenderQueue _meshQueue;
    };
  }
}