#pragma once

#include <vector>

#include "core/core.h"
#include "audio/audio.h"
#include "audio-al/util.h"
#include "audio-al/wav.h"
#include "audio-al/data.h"

#include "core/logger.h"

namespace wage {
  namespace audio {

    struct PlayingAudio {
      ALuint buffer;
      ALuint source;
    };

    class AlAudio : public Audio {

    public:
      void start() {
        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device) {
          core::Logger::error("Could not get an audio device.\n");
          return;
        }
        ALCcontext* context = alcCreateContext(device, NULL);
        if (context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
          if (context != NULL)
            alcDestroyContext(context);
          alcCloseDevice(device);
          core::Logger::error("Could not set a context!\n");
          return;
        }
        const ALCchar* name = NULL;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
          name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(device) != AL_NO_ERROR)
          name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        core::Logger::info("Opened Audio Device ", name);

        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          auto it = playing.begin();
          while (it != playing.end()) {
            ALint state = AL_PLAYING;
            alGetSourcei(it->source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
              alDeleteSources(1, &it->source);
              alDeleteBuffers(1, &it->buffer);
              it = playing.erase(it);
            } else {
              ++it;
            }
          }
        });
      }

      void play(memory::InputStream* input) {
        // TODO: Handle audio types other than wav.
        auto audio = loadWav(input);
        PlayingAudio playing;
        alGenBuffers(1, &playing.buffer);
        alBufferData(playing.buffer, audio.format(), audio.data(), audio.size(), audio.sampleRate());
        delete audio.data();
        alGenSources(1, &playing.source);
        alSourcei(playing.source, AL_BUFFER, (ALint)playing.buffer);
        alSourcePlay(playing.source);
        this->playing.push_back(playing);
      }

    private:
      std::vector<PlayingAudio> playing;
    };
  }
}