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

  void GlRenderer::start() {
    Renderer::start();
    glfwMakeContextCurrent(window->as<GLFWwindow>());
    gladLoadGL();

    GL_FAIL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_FAIL_CHECK(glDepthFunc(GL_LESS));
    GL_FAIL_CHECK(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GL_FAIL_CHECK(glEnable(GL_BLEND));
    GL_FAIL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    const GLubyte* vendor = glGetString(GL_VENDOR);     // Returns the vendor
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    Logger::info("Graphics Vendor: ", vendor);
    Logger::info("Graphics Renderer: ", renderer);

    // Intialize default render assets
    shaderManager.assetManager(assetManager);
    // GlShader::Default->load(assetManager);
    GlProgram::Default->load(assetManager);
    GlProgram::Font->load(assetManager);
    GlProgram::Sprite->load(assetManager);
    textureManager.assetManager(assetManager);
    fontManager.assetManager(assetManager);
    // GlTexture::Default->load(assetManager);
  }

  void GlRenderer::beginRender() {
    GL_FAIL_CHECK(glViewport(0, 0, window->width(), window->height()));
    GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }

  void GlRenderer::renderText(Vector2 position, std::string text, Font font, Color color) {
    uiQueue.add(makeTemp<GlTextRenderable>(&fontManager, position, text, font, color));
  }

  void GlRenderer::renderSprite(Vector2 position, Vector2 size, Color color, Texture texture) {
    uiQueue.add(makeTemp<GlSpriteRenderable>(
        &textureManager, position, size, color, texture));
  }

  void GlRenderer::renderMesh(Reference<Transform> transform, Reference<Mesh> mesh, Reference<Material> material) {
    meshQueue.add(makeTemp<GlMeshRenderable>(
        &vaoManager, &textureManager, *transform, mesh, material));
  }

  void GlRenderer::endRender() {
    GL_FAIL_CHECK(glfwSwapBuffers(window->as<GLFWwindow>()));
    GL_FAIL_CHECK(glfwPollEvents());
  }
}