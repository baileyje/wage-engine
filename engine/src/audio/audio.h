#pragma once

#include "core/service.h"
#include "memory/input_stream.h"
#include "audio/clip_spec.h"
#include "audio/handle.h"
#include "math/vector.h"

namespace wage {
  namespace audio {

    /**
     * Engine service for playing audio. This entry point provide a simple interface for submitting an
     * audio clip spec for playing in game.
     */
    class Audio : public core::Service {

    public:
      Audio() : Service("Audio") {}

      ~Audio() {}

      /**
       * Submit an audio clip relative to the listener and start playback as soon as it loads.
       */
      virtual ClipHandle* play(ClipSpec clip) = 0;

      /**
       * Submit an audio clip at a specfic position and start playback as soon as it loads.
       */
      virtual ClipHandle* play(ClipSpec clip, math::Vector3 position) = 0;
    };
  }
}