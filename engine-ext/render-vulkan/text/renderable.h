#pragma once

#include <string>

#include "asset/manager.h"
#include "render/renderable.h"
#include "render/context.h"
#include "render/components/font.h"
#include "render/context.h"
#include "math/vector.h"
#include "component/color.h"

namespace wage::render::vulkan {

  class FontManager;

  class TextRenderable : public Renderable {

  public:
    TextRenderable(FontManager* fontManager, math::Vector2 position, std::string text, FontSpec font, component::Color color);

    virtual math::Vector position();

    virtual math::BoundingBox boundingBox();

    virtual math::BoundingSphere boundingSphere();

    virtual void render(RenderContext* context);

  private:
    FontManager* fontManager;

    math::Vector2 _position;

    std::string text;

    FontSpec fontSpec;

    component::Color color;
  };
}