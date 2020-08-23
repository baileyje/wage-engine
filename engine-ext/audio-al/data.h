#pragma once

#include "al.h"
#include "util/types.h"

#include "core/logger.h"

namespace wage {
  namespace audio {

    class AudioData {
    public:
      AudioData() : AudioData(0, 0, 0, 0, nullptr) {
      }

      AudioData(Int8 channels, Int32 sampleRate, Int8 bitsPerSample, ALsizei size, char* data)
          : _channels(channels), _sampleRate(sampleRate), _bitsPerSample(bitsPerSample), _size(size), _data(data) {
      }

      inline Int8 channels() {
        return _channels;
      }

      inline Int32 sampleRate() {
        return _sampleRate;
      }

      inline Int8 bitsPerSample() {
        return _bitsPerSample;
      }

      inline ALsizei size() {
        return _size;
      }

      inline char* data() {
        return _data;
      }

      ALenum format() {
        if (_channels == 1 && _bitsPerSample == 8)
          return AL_FORMAT_MONO8;
        else if (_channels == 1 && _bitsPerSample == 16)
          return AL_FORMAT_MONO16;
        else if (_channels == 2 && _bitsPerSample == 8)
          return AL_FORMAT_STEREO8;
        else if (_channels == 2 && _bitsPerSample == 16)
          return AL_FORMAT_STEREO16;
        else {
          core::Logger::error("Unrecognised wave format: ", _channels, " channels, ", _bitsPerSample, " bps");
          return AL_FORMAT_MONO8;
        }
      }

    private:
      Int8 _channels;
      Int32 _sampleRate;
      Int8 _bitsPerSample;
      ALsizei _size;
      char* _data;
    };

  }
}