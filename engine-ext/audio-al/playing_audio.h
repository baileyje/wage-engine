#pragma once

#include "audio-al/clip.h"
#include "audio-al/util.h"
#include "audio-al/data.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

namespace wage {
  namespace audio {

    class PlayingAudio : public ClipHandle {
    public:
      PlayingAudio(Clip* clip) : clip(clip) {}

      virtual void play() {
        alSourcePlay(source);
      }

      virtual void pause() {
        alSourcePause(source);
      }

      void volume(float volume) {
        clip->volume(volume);
      }
      void pitch(float pitch) {
        clip->pitch(pitch);
      }
      void loop(bool loop) {
        clip->loop(loop);
      }

      void start() {
        auto audio = clip->data();
        alGenBuffers(NUM_BUFFERS, &buffers[0]);
        for (size_t i = 0; i < NUM_BUFFERS; ++i) {
          char buffer[BUFFER_SIZE];
          auto read = audio.input()->read((memory::Byte*)&buffer[0], BUFFER_SIZE);
          alBufferData(buffers[i], audio.format(), &buffer[0], read, audio.sampleRate());
        }
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, (ALfloat)clip->pitch());
        alSourcef(source, AL_GAIN, (ALfloat)clip->volume());
        alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);
        play();
      }

      void cleanup() {
        alDeleteSources(1, &source);
        alDeleteBuffers(NUM_BUFFERS, &buffers[0]);
        clip->data().input()->seek(clip->data().dataStartPosition());
      }

      void ensureBuffers() {
        ALint buffersProcessed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (buffersProcessed <= 0)
          return;
        auto audioData = clip->data();
        auto input = audioData.input();
        while (buffersProcessed--) {
          ALuint buffer;
          alSourceUnqueueBuffers(source, 1, &buffer);
          char data[BUFFER_SIZE];
          auto read = input->read((memory::Byte*)&data[0], BUFFER_SIZE);
          if (clip->loop() && read < BUFFER_SIZE) {
            clip->data().input()->seek(clip->data().dataStartPosition());
          }
          alBufferData(buffer, audioData.format(), data, read, audioData.sampleRate());
          alSourceQueueBuffers(source, 1, &buffer);
        }
      }

      Clip* clip;

      ALuint buffers[NUM_BUFFERS];
      ALuint source;

    private:
    };
  }
}
