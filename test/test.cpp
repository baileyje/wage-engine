
#include <unistd.h>
#include <iostream>

#include "audio-al/audio.h"
#include "fs/local.h"

using namespace wage;

int main(int argc, char* argv[]) {

  audio::AlAudio audio;

  audio.start();
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  std::cout << path + "/game/resources/audio/sample.wav"
            << "\n";
  wage::fs::LocalFileInputStream wav(path + "/../game/resources/audio/sample.wav");
  audio.play(&wav);
  return 0;
}