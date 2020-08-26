#pragma once

#include "assets/asset.h"
#include "audio/clip_spec.h"
#include "audio-al/data.h"
#include "audio-al/wav.h"

namespace wage {
  namespace audio {

    class Clip : public assets::Asset {

    public:
      Clip(ClipSpec spec) : Asset(spec), _volume(spec.volume()), _pitch(spec.pitch()), _loop(spec.loop()) {
      }

      bool onLoad(memory::InputStream* stream) {
        std::cout << "Loading Wave\n";
        _data = loadWav(stream);

        return false;
      }

      AudioData data() {
        return _data;
      }

      /**
       * Get the effective volume for the clip.
       */
      inline float_t volume() {
        return _volume;
      }

      /**
       * Get the effective pitch for the clip.
       */
      inline float_t pitch() {
        return _pitch;
      }

      /**
       * Determine whether this clip should loop.
       */
      inline bool loop() {
        return _loop;
      }

    private:
      AudioData _data;
      float_t _volume = 1.0;
      float_t _pitch = 1.0;
      bool _loop = false;
    };

  }
}
