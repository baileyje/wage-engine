#include "render-gl/program.h"

#include "memory/allocator.h"

namespace wage {
  namespace render {

    GlProgram* GlProgram::Default = memory::Allocator::Assets()->create<GlProgram>("default.vs", "default.fs");

    GlProgram* GlProgram::Font = memory::Allocator::Assets()->create<GlProgram>("font.vs", "font.fs");

    GlProgram* GlProgram::Sprite = memory::Allocator::Assets()->create<GlProgram>("sprite.vs", "sprite.fs");
  }
}