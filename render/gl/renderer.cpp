#include "render/gl/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/logger.h"

#include "render/gl/mesh_renderable.h"

#include "render/gl/material.h"
#include "render/gl/shader.h"
#include "render/gl/util.h"


namespace wage {

  void GlRenderer::start(SystemContext* context) {
    Renderer::start(context);
    glfwMakeContextCurrent(window->as<GLFWwindow>());
    gladLoadGL();
    glfwSwapInterval(1);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    GL_FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    
    const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    Logger::info("Graphics Vendor: ", vendor);
    Logger::info("Graphics Renderer: ", renderer);
    // Intialize default render assets
    fileSystem = context->get<FileSystem>();
    GlShader::initDefault(fileSystem);  
    GlTexture::Default->load(fileSystem);

    textureManager.setFileSystem(fileSystem);    
  }

  void GlRenderer::beginUpdate() {
    GL_FAIL_CHECK(glViewport(0, 0, window->width(), window->height()));
    GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }

  Renderable* GlRenderer::meshRenderable(EntityReference entity) {
    return new GlMeshRenderable(
      &vaoManager, &textureManager,
      entity->getTransform().getPosition(), entity->getTransform().getScale(), 
      entity->get<Mesh>(), entity->getTransform().worldPorjection(), entity->get<Material>());
  }

  void GlRenderer::endUpdate() {
    GL_FAIL_CHECK(glfwSwapBuffers(window->as<GLFWwindow>()));
    GL_FAIL_CHECK(glfwPollEvents());
  }

}