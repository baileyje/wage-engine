#pragma once

#include "assets/asset.h"
#include "audio/clip_spec.h"
#include "audio-al/data.h"
#include "audio-al/wav.h"

namespace wage {
  namespace audio {

    class Clip : public assets::Asset {

    public:
      Clip(ClipSpec spec) : Asset(spec) {
      }

      bool onLoad(memory::InputStream* stream) {
        std::cout << "Loading Wave\n";
        _data = loadWav(stream);

        return false;
      }

      AudioData data() {
        return _data;
      }

    private:
      AudioData _data;
    };

  }
}
