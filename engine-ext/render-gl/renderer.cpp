#include "render-gl/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/logger.h"
#include "memory/allocator.h"
#include "math/vector.h"

#include "render-gl/mesh_renderable.h"
#include "render-gl/text_renderable.h"
#include "render-gl/sprite_renderable.h"
#include "render-gl/material.h"
#include "render-gl/shader.h"
#include "render-gl/util.h"

namespace wage {
  namespace render {

    void GlRenderer::start() {
      Renderer::start();
      glfwMakeContextCurrent(window->as<GLFWwindow>());
      gladLoadGL();

      GL_FAIL_CHECK(glEnable(GL_DEPTH_TEST));
      GL_FAIL_CHECK(glDepthFunc(GL_LESS));
      GL_FAIL_CHECK(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
      GL_FAIL_CHECK(glEnable(GL_BLEND));
      GL_FAIL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      glfwSwapInterval(0);

      const GLubyte* vendor = glGetString(GL_VENDOR);     // Returns the vendor
      const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
      core::Logger::info("Graphics Vendor: ", vendor);
      core::Logger::info("Graphics Renderer: ", renderer);

      // Initialize default render assets
      meshManager.assetManager(assetManager);
      vaoManager.meshManager(&meshManager);
      GlProgram::Default->load(assetManager);
      GlProgram::Font->load(assetManager);
      GlProgram::Sprite->load(assetManager);
      meshManager.generatePrimitives();
    }

    void GlRenderer::beginRender() {
      GL_FAIL_CHECK(glViewport(0, 0, window->width(), window->height()));
      GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void GlRenderer::renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color) {
      updateFrame()->uiQueue().add<GlTextRenderable>(
          assetManager, position, text, font, color);
    }

    void GlRenderer::renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture) {
      updateFrame()->uiQueue().add<GlSpriteRenderable>(
          assetManager, position, size, color, texture);
    }

    void GlRenderer::renderMesh(math::Transform transform, MeshSpec mesh, MaterialSpec material) {
      auto meshData = meshManager.load(mesh);
      updateFrame()->meshQueue().add<GlMeshRenderable>(assetManager, &vaoManager, transform, meshData, material);
    }

    void GlRenderer::endRender() {
      GL_FAIL_CHECK(glfwSwapBuffers(window->as<GLFWwindow>()));
    }

  }
}