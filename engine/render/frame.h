
#pragma once

#include <mutex>
#include <chrono>

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

      ~Frame() {
        std::cout << "Destruct frame\n";
      }

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
       * Get access to the mesh render queue.
       */
      inline RenderQueue& wireframeQueue() {
        return _wireframeQueue;
      }

      /**
       * Get access to current render context.
       */
      inline RenderContext* context() {
        return _renderContext;
      }


      /**
       * Clear frame data.
       */
      inline void clear() {
        _meshQueue.clear();
        _uiQueue.clear();
      }

      /**
       * Remove any items from the frame by frustum culling using the render context.
       */
      inline void cull() {
        if (!_renderContext)
          return;
        _meshQueue.cull(_renderContext);
      }

      /**
       * Sort the render queue by z-order.
       */
      inline void sort() {
        if (!_renderContext)
          return;
        _meshQueue.sort(_renderContext);
        _uiQueue.sort(_renderContext);
      }

      inline void prepare(RenderContext* context) {
        if (_renderContext) {
          delete _renderContext;
        }
        _renderContext = context;
        cull();
        sort();
      }

      void renderMeshes() {
        if (!_renderContext)
          return;
        // _meshQueue.sort(renderContext);
        _meshQueue.render(_renderContext);
      }

      void renderWireframes() {
        if (!_renderContext)
          return;
        // _meshQueue.sort(renderContext);
        _wireframeQueue.render(_renderContext);
      }

      void renderUi() {
        if (!_renderContext)
          return;
        // _uiQueue.sort(renderContext);
        _uiQueue.render(_renderContext);
      }

    private:


      RenderQueue _uiQueue;
      RenderQueue _meshQueue;
      RenderQueue _wireframeQueue;

      RenderContext* _renderContext  = nullptr;
    };

  } // namespace render
} // namespace wage
