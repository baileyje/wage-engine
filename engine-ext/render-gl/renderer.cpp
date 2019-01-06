#include "engine-ext/render-gl/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/logger.h"
#include "engine/memory/allocator.h"
#include "engine/math/vector.h"

#include "engine-ext/render-gl/mesh_renderable.h"
#include "engine-ext/render-gl/text_renderable.h"
#include "engine-ext/render-gl/material.h"
#include "engine-ext/render-gl/shader.h"
#include "engine-ext/render-gl/util.h"


namespace wage {

  void GlRenderer::start(SystemContext* context) {
    Renderer::start(context);
    glfwMakeContextCurrent(window->as<GLFWwindow>());
    gladLoadGL();
    glfwSwapInterval(0);
    
    GL_FAIL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_FAIL_CHECK(glDepthFunc(GL_LESS));
    GL_FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    // GL_FAIL_CHECK(glEnable(  ));
    GL_FAIL_CHECK(glEnable(GL_BLEND));
    GL_FAIL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    Logger::info("Graphics Vendor: ", vendor);
    Logger::info("Graphics Renderer: ", renderer);
    
    // Intialize default render assets
    shaderManager.setAssetManager(assetManager);
    // GlShader::Default->load(assetManager);
    GlProgram::Default->load(assetManager);
    GlProgram::Font->load(assetManager);
    textureManager.setAssetManager(assetManager);
    fontManager.setAssetManager(assetManager);
    // GlTexture::Default->load(assetManager);
  }

  void GlRenderer::beginUpdate() {
    GL_FAIL_CHECK(glViewport(0, 0, window->width(), window->height()));
    GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }

  void GlRenderer::renderText(Vector position, std::string text, Font font, Color color) {
    uiQueue.add(makeTemp<GlTextRenderable>(&fontManager, position, text, font, color));
  }

  void GlRenderer::renderMesh(Transform transform, ComponentReference<Mesh> mesh, ComponentReference<Material> material) {
    meshQueue.add(makeTemp<GlMeshRenderable>(
      &vaoManager, &textureManager, transform, mesh, material
    ));
  }

  void GlRenderer::endUpdate() {
    
    GL_FAIL_CHECK(glfwSwapBuffers(window->as<GLFWwindow>()));
    GL_FAIL_CHECK(glfwPollEvents());
  }

}