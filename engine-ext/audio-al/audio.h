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

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

namespace wage {
  namespace audio {

    struct PlayingAudio {
      Clip* clip;
      ALuint buffers[NUM_BUFFERS];
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
            auto clip = loadingIt->clip;
            if (clip->loaded()) {
              startPlaying(clip);
              loadingIt = loading.erase(loadingIt);
            } else {
              ++loadingIt;
            }
          }

          auto playingIt = playing.begin();
          while (playingIt != playing.end()) {
            ALint state = AL_PLAYING;
            auto clip = playingIt->clip;
            alGetSourcei(playingIt->source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
              alDeleteSources(1, &playingIt->source);
              alDeleteBuffers(NUM_BUFFERS, &playingIt->buffers[0]);
              clip->data().input()->seek(clip->data().datStartPosition());
              playingIt = playing.erase(playingIt);
            } else {
              ensureBuffers(&(*playingIt));
              ++playingIt;
            }
          }
        });
      }

      void play(ClipSpec spec) {
        loading.push_back({assetManager->load<Clip>(spec, false)});
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
        playing.clip = clip;
        alGenBuffers(NUM_BUFFERS, &playing.buffers[0]);
        for (size_t i = 0; i < NUM_BUFFERS; ++i) {
          char buffer[BUFFER_SIZE];
          auto read = audio.input()->read((memory::Byte*)&buffer[0], BUFFER_SIZE);
          alBufferData(playing.buffers[i], audio.format(), &buffer[0], read, audio.sampleRate());
        }
        alGenSources(1, &playing.source);
        alSourceQueueBuffers(playing.source, NUM_BUFFERS, &playing.buffers[0]);
        alSourcePlay(playing.source);
        this->playing.push_back(playing);
      }

      void ensureBuffers(PlayingAudio* playing) {
        ALint buffersProcessed = 0;
        alGetSourcei(playing->source, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (buffersProcessed <= 0)
          return;
        auto clip = playing->clip;
        auto audioData = clip->data();
        auto input = audioData.input();
        while (buffersProcessed--) {
          ALuint buffer;
          alSourceUnqueueBuffers(playing->source, 1, &buffer);
          char data[BUFFER_SIZE];
          auto read = input->read((memory::Byte*)&data[0], BUFFER_SIZE);
          alBufferData(buffer, audioData.format(), data, read, audioData.sampleRate());
          alSourceQueueBuffers(playing->source, 1, &buffer);
        }
      }

      assets::Manager* assetManager;
      std::vector<LoadingAudio> loading;
      std::vector<PlayingAudio> playing;
    };
  } // namespace audio
} // namespace wage
