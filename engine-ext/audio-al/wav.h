
#pragma once

#include "al.h"
#include "util/types.h"
#include "memory/input_stream.h"
#include "audio-al/data.h"

namespace wage {
  namespace audio {

    bool isBigEndian(void) {
      union {
        uint32_t i;
        char c[4];
      } bint = {0x01020304};
      return bint.c[0] == 1;
    }

    Int32 convertToInt(char* buffer, size_t len) {
      Int32 a = 0;
      if (isBigEndian())
        for (size_t i = 0; i < len; ++i)
          reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
      else
        std::memcpy(&a, buffer, len);
      return a;
    }

    AudioData loadWav(memory::InputStream* input) {
      char buffer[4];
      // the RIFF
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read RIFF");
        return {};
      }
      if (std::strncmp(buffer, "RIFF", 4) != 0) {
        core::Logger::error("file is not a valid WAVE file (header doesn't begin with RIFF)");
        return {};
      }
      // the size of the file
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read size of file");
        return {};
      }
      // the WAVE
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read WAVE");
        return {};
      }
      if (std::strncmp(buffer, "WAVE", 4) != 0) {
        core::Logger::error("file is not a valid WAVE file (header doesn't contain WAVE)");
        return {};
      }
      // "fmt/0"
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read fmt/0");
        return {};
      }
      // this is always 16, the size of the fmt data chunk
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read the 16");
        return {};
      }
      // PCM should be 1?
      if (!input->read((memory::Byte*)&buffer[0], 2)) {
        core::Logger::error("could not read PCM");
        return {};
      }
      // the number of channels
      if (!input->read((memory::Byte*)&buffer[0], 2)) {
        core::Logger::error("could not read number of channels");
        return {};
      }
      Int8 channels = convertToInt(buffer, 2);
      // sample rate
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read sample rate");
        return {};
      }
      Int32 sampleRate = convertToInt(buffer, 4);

      // (sampleRate * bitsPerSample * channels) / 8
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read (sampleRate * bitsPerSample * channels) / 8");
        return {};
      }
      // ?? dafaq
      if (!input->read((memory::Byte*)&buffer[0], 2)) {
        core::Logger::error("could not read dafaq");
        return {};
      }
      // bitsPerSample
      if (!input->read((memory::Byte*)&buffer[0], 2)) {
        core::Logger::error("could not read bits per sample");
        return {};
      }
      Int8 bitsPerSample = convertToInt(buffer, 2);
      // data chunk header "data"
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read data chunk header");
        return {};
      }
      if (std::strncmp(buffer, "data", 4) != 0) {
        core::Logger::error("file is not a valid WAVE file (doesn't have 'data' tag)");
        return {};
      }
      // size of data
      if (!input->read((memory::Byte*)&buffer[0], 4)) {
        core::Logger::error("could not read data size");
        return {};
      }
      auto size = convertToInt(buffer, 4);
      return {channels, sampleRate, bitsPerSample, size, input, input->tell()};
    }
  }
}