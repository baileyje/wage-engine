#include "engine-ext/render-gl/util.h"

#include "engine/core/logger.h"

namespace wage {

  void checkGLError(const char* command) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      for (; err != GL_NO_ERROR; err = glGetError()) {
        switch (err) {
          case GL_INVALID_ENUM: 
            Logger::error("GL_INVALID_ENUM on: ", command); 
            break;
          case GL_INVALID_OPERATION: 
            Logger::error("GL_INVALID_OPERATION on: ", command); 
            break;
          case GL_INVALID_VALUE: 
            Logger::error("GL_INVALID_VALUE on: ", command); 
            break;
          case GL_OUT_OF_MEMORY: 
            Logger::error("GL_INVALID_VALUE on: ", command); 
            break;          
          default: break;
        }
      }
      exit(1);
    }
  }
  
}