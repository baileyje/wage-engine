#pragma once

#include <string>

#include "asset/spec.h"

namespace wage {
  namespace audio {

    /**
     * Simple reference to and Audio clip.
     */
    class ClipSpec : public asset::AssetSpec {

    public:
      /**
       * Create a clip with a key and default audio properties. Generally the key is the file name of the clip data.
       */
      ClipSpec(std::string key) : ClipSpec(key, 1.0, 1.0, false) {
      }

      /**
       * Create a clip with a key and various audio properties. Generally the key is the file name of the clip data.
       */
      ClipSpec(std::string key, float_t volume, float_t pitch, bool loop) : AssetSpec("audio", key), _volume(volume), _pitch(pitch), _loop(loop) {
      }

      /**
       * Create a clip with a key and various audio properties. Generally the key is the file name of the clip data.
       */
      ClipSpec(std::string key, bool loop) : ClipSpec(key, 1.0, 1.0, loop) {
      }

      ~ClipSpec() {}

      /**
       * Get the effective volume for the clip.
       */
      inline float_t volume() {
        return _volume;
      }

      /**
       * Set the effective volume for the clip.
       */
      inline void volume(float_t volume) {
        _volume = volume;
      }

      /**
       * Get the effective pitch for the clip.
       */
      inline float_t pitch() {
        return _pitch;
      }

      /**
       * Set the effective pitch for the clip.
       */
      inline void pitch(float_t pitch) {
        _pitch = pitch;
      }

      /**
       * Determine whether this clip should loop.
       */
      inline bool loop() {
        return _loop;
      }

      /**
       * Set the clip to loop.
       */
      inline void loop(bool loop) {
        _loop = loop;
      }

    private:
      float_t _volume = 1.0;
      float_t _pitch = 1.0;
      bool _loop = false;
    };
  }
}