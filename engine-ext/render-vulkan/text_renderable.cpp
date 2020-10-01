#include "render-vulkan/text_renderable.h"

#include "render-vulkan/font_manager.h"
#include "render-vulkan/render_context.h"
#include "render-vulkan/pipeline.h"

#include "render-vulkan/font.h"

namespace wage::render {

  TextRenderable::TextRenderable(FontManager* fontManager, math::Vector2 position, std::string text, FontSpec font, component::Color color)
      : fontManager(fontManager), _position(position), text(text), fontSpec(font), color(color.r, color.g, color.b) {}

  math::Vector TextRenderable::position() {
    return _position;
  }

  math::BoundingBox TextRenderable::boundingBox() {
    return math::BoundingBox(_position, math::Vector(1000, 1000, 1000));
  }

  math::BoundingSphere TextRenderable::boundingSphere() {
    auto box = boundingBox();
    float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
    return math::BoundingSphere(box.position, radius);
  }

  void TextRenderable::render(RenderContext* context) {
    Font* font = fontManager->load(fontSpec);
    if (!font->loaded()) {
      return;
    }
    auto vkContext = static_cast<VulkanRenderContext*>(context);
    font->push(vkContext->device, vkContext->commandPool);

    float x = _position.x;

    // // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
      auto glyph = font->glyphFor(*c);
      if (!glyph) continue;
      if (glyph->size != math::Vector2::Zero) {

        glyph->push(vkContext->device, vkContext->commandPool, vkContext->uiPipeline, font->descriptorPool);

        GLfloat xpos = x + glyph->bearing.x;
        GLfloat ypos = (context->screenSize().y - _position.y) - glyph->bearing.y;

        VkBuffer vertexBuffers[] = {glyph->vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(vkContext->commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(vkContext->commandBuffer, glyph->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        // auto pos = glm::vec2(_position.x, _position.y);
        math::Transform transform({xpos, ypos, 0}, math::Vector3::One, math::Vector3::Zero);
        auto matrix = transform.worldProjection().glm();
        vkCmdPushConstants(vkContext->commandBuffer, vkContext->uiPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &matrix);
        vkCmdBindDescriptorSets(vkContext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkContext->uiPipeline->layout, 1, 1, &glyph->texture.descriptorSets[vkContext->imageIndex], 0, nullptr);
        vkCmdDrawIndexed(vkContext->commandBuffer, static_cast<uint32_t>(6), 1, 0, 0, 0);
      }
      x += (glyph->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
  }
}