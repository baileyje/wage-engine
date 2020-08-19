#include "render-gl/program.h"

#include "memory/allocator.h"

namespace wage {
  namespace render {

    GlProgram* GlProgram::Default = memory::make<GlProgram>("default.vs", "default.fs");

    GlProgram* GlProgram::Font = memory::make<GlProgram>("font.vs", "font.fs");

    GlProgram* GlProgram::Sprite = memory::make<GlProgram>("sprite.vs", "sprite.fs");

  }
}