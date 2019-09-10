#include "render-gl/util.h"

#include "core/logger.h"

namespace wage { namespace render {

  void checkGLError(const char* command) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      for (; err != GL_NO_ERROR; err = glGetError()) {
        switch (err) {
        case GL_INVALID_ENUM:
          core::Logger::error("GL_INVALID_ENUM on: ", command);
          break;
        case GL_INVALID_OPERATION:
          core::Logger::error("GL_INVALID_OPERATION on: ", command);
          break;
        case GL_INVALID_VALUE:
          core::Logger::error("GL_INVALID_VALUE on: ", command);
          break;
        case GL_OUT_OF_MEMORY:
          core::Logger::error("GL_INVALID_VALUE on: ", command);
          break;
        default:
          break;
        }
      }
      // TODO: Should this happen?
      // exit(1);
    }
  }

} }