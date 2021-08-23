#include "render-gl/program.h"

#include "memory/allocator.h"

namespace wage {
  namespace render {

    GlProgram* GlProgram::Default = new GlProgram("default.vs", "default.fs");

    GlProgram* GlProgram::Font = new GlProgram("font.vs", "font.fs");

    GlProgram* GlProgram::Sprite = new GlProgram("sprite.vs", "sprite.fs");
  }
}