#pragma once

#include "math/vector.h"

namespace wage {
  namespace audio {

    /**
     * Handle to a currently playing audio clip. This handle allows some limitted control of the playback 
     * attributes.
     * 
     * Note: Adjusting values on this handle will not change the values in the ClipSpec to create the clip.
     */
    class ClipHandle {
    public:
      /**
       * Start playing the clip.
       */
      virtual void play() = 0;

      /**
       * Pause playback.
       */
      virtual void pause() = 0;

      /**
       * Set the current volume of for the clip.
       */
      virtual void volume(float volume) = 0;

      /**
       * Set the current pitch of for the clip.
       */
      virtual void pitch(float pitch) = 0;

      /**
       * Turn looping off or on for the clip.
       */
      virtual void loop(bool loop) = 0;

      /**
       * Set the current position of the playing sound.
       */
      virtual void position(math::Vector3 position) = 0;

      // TODO: Current runtime state...
    };

  }
}