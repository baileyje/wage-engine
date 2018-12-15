#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL


#include "math/vector.h"
#include "math/quaternion.h"
#include "math/color.h"

namespace wage {

    void checkGLError(const char* command);

    #define GL_FAIL_CHECK(cmd)    \
        do                     \
        {                      \
            cmd;               \
            checkGLError(#cmd); \
        } while(0)              \


    inline glm::vec3 vec3From(Vector* vector) {
        return glm::vec3(vector->x, vector->y, vector->z);
    }

    inline glm::vec3 vec3From(Color* color) {
        return glm::vec3(color->r, color->g, color->b);
    }

    inline glm::vec3 directionFromEulers(Vector vector) {
    float yaw = vector.x;
    float pitch = vector.y;
        return glm::vec3(
            glm::cos(yaw) * glm::cos(pitch),
            glm::sin(yaw) * glm::cos(pitch),
            glm::sin(pitch)
        );
    }

}

#endif //RENDER_UTIL_H
