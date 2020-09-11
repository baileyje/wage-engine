
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
      // inline void renderContext(RenderContext* context) {
      //   if (_renderContext) {
      //     delete _renderContext;
      //   }
      //   _renderContext = context;
      // }

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

      /**
       * Render the frame with with provided render context.
       */
      inline void render() {
        if (!_renderContext)
          return;
        renderMeshes(_renderContext);
        // TODO: Sprites
        renderUi(_renderContext);
      }

    private:
      void renderMeshes(RenderContext* renderContext) {
        // _meshQueue.sort(renderContext);
        _meshQueue.render(renderContext);
      }

      void renderUi(RenderContext* renderContext) {
        // _uiQueue.sort(renderContext);
        _uiQueue.render(renderContext);
      }

      RenderQueue _uiQueue;
      RenderQueue _meshQueue;

      RenderContext* _renderContext;
    };

    // TODO: TEMP...
    class FrameQueue : public std::queue<Frame*> {
    public:
      FrameQueue(int size) {
        maxSize = size;
      }

      void push(Frame* frame) {
        std::unique_lock<std::mutex> wlck(writerMutex);
        while (full())
          isFull.wait(wlck);
        std::queue<Frame*>::push(frame);
        isEmpty.notify_all();
      }

      bool notEmpty() {
        return !std::queue<Frame*>::empty();
      }

      bool full() {
        return std::queue<Frame*>::size() >= maxSize;
      }

      Frame* pop() {
        std::unique_lock<std::mutex> lck(readerMutex);
        while (std::queue<Frame*>::empty()) {
          isEmpty.wait(lck);
        }
        Frame* value = std::queue<Frame*>::front();
        std::queue<Frame*>::pop();
        if (!full())
          isFull.notify_all();
        return value;
      }

    private:
      int maxSize;
      std::mutex readerMutex;
      std::mutex writerMutex;
      std::condition_variable isFull;
      std::condition_variable isEmpty;
    };
  } // namespace render
} // namespace wage
