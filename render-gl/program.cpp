#include "render-gl/program.h"

namespace wage {

  GlProgram* GlProgram::Default = new GlProgram("shaders/default.vs", "shaders/default.fs");
  GlProgram* GlProgram::Font = new GlProgram("shaders/font.vs", "shaders/font.fs");

}