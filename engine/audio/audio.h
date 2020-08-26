#pragma once

#include "core/service.h"
#include "memory/input_stream.h"
#include "audio/clip_spec.h"

namespace wage {
  namespace audio {

    class ClipHandle {
    public:
      virtual void play() = 0;
      virtual void pause() = 0;
      virtual void volume(float volume) = 0;
      virtual void pitch(float pitch) = 0;
      virtual void loop(bool loop) = 0;
      // TODO: Current runtime state...
    };

    class Audio : public core::Service {

    public:
      Audio() : Service("Audio") {}

      ~Audio() {}

      void start() {
      }

      virtual ClipHandle* play(ClipSpec clip) = 0;
    };
  }
}