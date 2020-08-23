#pragma once

#include "core/service.h"
#include "memory/input_stream.h"

namespace wage {
  namespace audio {

    class Audio : public core::Service {

    public:
      Audio() : Service("Audio") {}

      ~Audio() {}

      void start() {
      }

      virtual void play(memory::InputStream* input) = 0;
    };
  }
}