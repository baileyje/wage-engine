#include "render/renderer.h"

#include <vector>

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/util.h"


void errorCallback(int error, const char* description) {
  printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
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
 
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  defaultShaderProgram = loadJankyShader();
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
  // mat4x4 m, p, mvp;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

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
  
  defaultShaderProgram->bind();

  VertexArray VAO;
  VAO.bind();

  VertexBuffer VBO(vertices, sizeof(vertices));    
  VertexBufferLayout layout;
  layout.pushFloat(3);
  VAO.addBuffer(VBO, layout);
  
  IndexBuffer EBO(indices, 6);
  
  EBO.bind();
  FAIL_CHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

  glfwSwapBuffers(window);
  glfwPollEvents();
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
    "void main() {\n"
    "  gl_Position.xyz = vertexPosition_modelspace; \n"
    "  gl_Position.w = 1.0;\n"
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
