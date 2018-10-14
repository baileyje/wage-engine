#include "render/util.h"

void checkGLError(const char* command) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    for (; err != GL_NO_ERROR; err = glGetError()) {
      switch (err) {
				case GL_INVALID_ENUM: 
          printf("GL_INVALID_ENUM on: %s\n", command); 
          break;
        case GL_INVALID_OPERATION: 
          printf("GL_INVALID_OPERATION on: %s\n", command); 
          break;
        case GL_INVALID_VALUE: 
          printf("GL_INVALID_VALUE on: %s\n", command); 
          break;
        case GL_OUT_OF_MEMORY: 
          printf("GL_INVALID_VALUE on: %s\n", command); 
          break;          
				default: break;
			}
    }
  }
}