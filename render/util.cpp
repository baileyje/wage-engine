#include "render/util.h"

#include "core/logger.h"

namespace wage {

  void checkGLError(const char* command) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      for (; err != GL_NO_ERROR; err = glGetError()) {
        switch (err) {
          case GL_INVALID_ENUM: 
            Logger::error("GL_INVALID_ENUM on: %s\n", command); 
            break;
          case GL_INVALID_OPERATION: 
            Logger::error("GL_INVALID_OPERATION on: %s\n", command); 
            break;
          case GL_INVALID_VALUE: 
            Logger::error("GL_INVALID_VALUE on: %s\n", command); 
            break;
          case GL_OUT_OF_MEMORY: 
            Logger::error("GL_INVALID_VALUE on: %s\n", command); 
            break;          
          default: break;
        }
      }
    }
  }
  
}