#pragma once

#include <vector>

#include "core/core.h"
#include "assets/manager.h"
#include "audio/audio.h"
#include "audio-al/clip.h"
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

    struct LoadingAudio {
      Clip* clip;
    };

    class AlAudio : public Audio {

    public:
      void start() {
        assetManager = core::Core::Instance->get<assets::Manager>();
        initAl();

        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          auto loadingIt = loading.begin();
          while (loadingIt != loading.end()) {
            if (loadingIt->clip->loaded()) {
              startPlaying(loadingIt->clip);
              loadingIt = loading.erase(loadingIt);
            } else {
              ++loadingIt;
            }
          }

          auto playingIt = playing.begin();
          while (playingIt != playing.end()) {
            ALint state = AL_PLAYING;
            alGetSourcei(playingIt->source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
              alDeleteSources(1, &playingIt->source);
              alDeleteBuffers(1, &playingIt->buffer);
              playingIt = playing.erase(playingIt);
            } else {
              ++playingIt;
            }
          }
        });
      }

      void play(ClipSpec spec) {
        loading.push_back({assetManager->load<Clip>(spec)});
      }

    private:
      void initAl() {
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
      }

      void startPlaying(Clip* clip) {
        auto audio = clip->data();
        PlayingAudio playing;
        alGenBuffers(1, &playing.buffer);
        alBufferData(playing.buffer, audio.format(), audio.data(), audio.size(), audio.sampleRate());
        alGenSources(1, &playing.source);
        alSourcei(playing.source, AL_BUFFER, (ALint)playing.buffer);
        alSourcePlay(playing.source);
        this->playing.push_back(playing);
      }

      assets::Manager* assetManager;
      std::vector<LoadingAudio> loading;
      std::vector<PlayingAudio> playing;
    };
  } // namespace audio
} // namespace wage