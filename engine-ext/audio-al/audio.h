#pragma once

#include <vector>

#include "core/core.h"
#include "assets/manager.h"
#include "audio/audio.h"
#include "audio-al/clip.h"
#include "audio-al/util.h"
#include "audio-al/data.h"
#include "audio-al/playing_audio.h"

#include "core/logger.h"

namespace wage {
  namespace audio {

    class AlAudio : public Audio {

    public:
      void start() {
        assetManager = core::Core::Instance->get<assets::Manager>();
        initAl();

        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          auto loadingIt = loading.begin();
          while (loadingIt != loading.end()) {
            auto loadingItem = *loadingIt;
            auto clip = loadingItem->clip;
            if (clip->loaded()) {
              loadingItem->start();
              this->playing.push_back(loadingItem);
              loadingIt = loading.erase(loadingIt);
            } else {
              ++loadingIt;
            }
          }

          auto playingIt = playing.begin();
          while (playingIt != playing.end()) {
            auto playingItem = *playingIt;
            ALint state = AL_PLAYING;
            auto clip = playingItem->clip;
            alGetSourcei(playingItem->source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
              playingItem->cleanup();
              playingIt = playing.erase(playingIt);
            } else {
              alSourcef(playingItem->source, AL_PITCH, (ALfloat)clip->pitch());
              alSourcef(playingItem->source, AL_GAIN, (ALfloat)clip->volume());
              playingItem->ensureBuffers();
              ++playingIt;
            }
          }
        });
      }

      ClipHandle* play(ClipSpec spec) {
        auto clip = assetManager->load<Clip>(spec, false);
        auto playing = new PlayingAudio(clip);
        loading.push_back(playing);
        return playing;
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

      assets::Manager* assetManager;
      std::vector<PlayingAudio*> loading;
      std::vector<PlayingAudio*> playing;
    };
  } // namespace audio
} // namespace wage
