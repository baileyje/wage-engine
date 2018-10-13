#include "render/renderer.h"

#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/util.h"
#include "render/material.h"
#include "render/mesh.h"


void errorCallback(int error, const char* description) {
  printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Renderer::~Renderer() {
  delete vao;
  delete mesh;
  delete material;
}

void Renderer::init(Context* context)  {
  printf("Initializing Renderer.\n");
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) {
    printf("GLFW Init Failed\n");
  }  
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1024, 768, "Wage Bro!", NULL, NULL);
  glfwSetKeyCallback(window, keyCallback);   
  screenProjection = glm::ortho(0.0f, (float)1024, 0.0f, (float)768, -1.0f, 1.0f);  
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  defaultShaderProgram = loadJankyShader();

  vao = new VertexArray();
  vao->bind();

  float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  };

  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  };    
  VertexBuffer* vbo = new VertexBuffer(vertices, sizeof(vertices));    
  VertexBufferLayout* layout = new VertexBufferLayout();
  layout->pushFloat(3);
  IndexBuffer* ibo = new IndexBuffer(indices, 6);  
  mesh = new Mesh(vbo, ibo, layout);
  material = new Material(defaultShaderProgram);  
}


void Renderer::update(Context* context) {
  // printf("Updating Renderer.\n");
  if (glfwWindowShouldClose(window)) {
    printf("Close!\n");
    context->shouldStop();
    return;
  }
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  // FAIL_CHECK(glViewport(0, 0, width, height));
  FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  draw(vao, mesh, material);

  FAIL_CHECK(glfwSwapBuffers(window));
  FAIL_CHECK(glfwPollEvents());
}

void Renderer::draw(VertexArray* vao, Mesh* mesh, Material* material) {
  
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 500.0f, 0.0f));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 120.0f, 20.0f));
  glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
  glm::mat4 model = translation * rotate * scale;
  
  glm::mat4 mvp = screenProjection * model;  
  material->setMat4("mvp", &mvp, 1);
  material->bind();
  mesh->bind(vao);
  FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::stop(Context* context) {
  printf("Stopping Renderer.\n");
  delete defaultShaderProgram;
}

void Renderer::deinit(Context* context) {
  printf("Deinitializing Renderer.\n");
  glfwDestroyWindow(window);
  glfwTerminate();
}

Shader* Renderer::loadJankyShader(){
	// TODO: Read the Vertex Shader code from the file
	std::string vertexShaderCode =
    "#version 330 core\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "uniform mat4 mvp;\n"
    "void main() {\n"
    "  gl_Position = mvp * vec4(vertexPosition_modelspace,1);\n"
    "}";

	// TODO: Read the Fragment Shader code from the file
	std::string fragmentShaderCode =
    "#version 330 core\n"
    "out vec3 color;\n" 
    "void main() {\n"
    "  color = vec3(1,1,1);\n"
    "}";

  Shader* shader = new Shader(vertexShaderCode, fragmentShaderCode);
	shader->compile();
  shader->link();
	return shader;
}
