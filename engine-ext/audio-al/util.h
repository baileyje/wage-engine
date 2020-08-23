#pragma once

#include <string>
#include <iostream>

#include "al.h"
#include "alc.h"

namespace wage {
  namespace audio {

#define AL_FAIL_CHECK(cmd) checkAlErrors(__FILE__, __LINE__, #cmd, cmd);

#define ALC_FAIL_CHECK(device, cmd) checkAlCErrors(__FILE__, __LINE__, #cmd, device, cmd);

    ALboolean checkAlErrors(const std::string& filename, const std::uint_fast32_t line, const char* command, ALboolean result) {
      ALCenum error = alGetError();
      if (error != AL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << " - " << command << ")\n";
        switch (error) {
        case AL_INVALID_NAME:
          std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
          break;
        case AL_INVALID_ENUM:
          std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
          break;
        case AL_INVALID_VALUE:
          std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
          break;
        case AL_INVALID_OPERATION:
          std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
          break;
        case AL_OUT_OF_MEMORY:
          std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
          break;
        default:
          std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
      }
      return result;
    }

    ALCboolean checkAlcErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device, const char* command, ALCboolean result) {
      ALCenum error = alcGetError(device);
      if (error != ALC_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << " - " << command << ")\n";
        switch (error) {
        case ALC_INVALID_VALUE:
          std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
          break;
        case ALC_INVALID_DEVICE:
          std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
          break;
        case ALC_INVALID_CONTEXT:
          std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
          break;
        case ALC_INVALID_ENUM:
          std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
          break;
        case ALC_OUT_OF_MEMORY:
          std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
          break;
        default:
          std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
      }
      return result;
    }
  }
}