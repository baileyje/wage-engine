#pragma once

#include <unordered_set>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"

#include "input/input.h"
#include "input/key.h"
#include "input/key_event.h"
#include "input/key_modifier.h"
#include "input/mouse_button_event.h"
#include "input/mouse_move_event.h"
#include "input/mouse_scroll_event.h"

#include "input-glfw/util.h"

namespace wage {
  namespace input {

    class GlfwInput : public Input {

    public:
      GlfwInput(bool disableCursor = true) : Input(), disableCursor(disableCursor) {}

      ~GlfwInput() {}

      void start() override {
        Input::start();
        auto glfwWindow = window->as<GLFWwindow>();
        glfwSetWindowUserPointer(glfwWindow, this);
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int keyCode, int scancode, int action, int mods) {
          auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
          if (keyCode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            core::Core::Instance->stop();
            return;
          } else if (action == GLFW_PRESS) {
            input->pressed(keyFrom(keyCode));
          } else if (action == GLFW_RELEASE) {
            input->released(keyFrom(keyCode));
          }
          if (input->messaging) {
            auto eventType = KeyEventType::press;
            switch (action) {
            case GLFW_RELEASE:
              eventType = KeyEventType::release;
              break;
            case GLFW_REPEAT:
              eventType = KeyEventType::repeat;
              break;
            }
            Key key = keyFrom(keyCode);
            KeyEvent event(key, eventType, mods);
            input->messaging->send(event);
          }
        });
        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int buttonCode, int action, int mods) {
          auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
          if (action == GLFW_PRESS) {
            input->pressed(buttonFrom(buttonCode));
          } else if (action == GLFW_RELEASE) {
            input->released(buttonFrom(buttonCode));
          }
          auto eventType = action == GLFW_RELEASE ? MouseButtonEvent::Type::release : MouseButtonEvent::Type::press;
          MouseButtonEvent event(buttonFrom(buttonCode), eventType, mods, input->mousePosition());
          input->messaging->send(event);
        });
        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
          auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
          MouseMoveEvent event(math::Vector2(x, y));
          input->messaging->send(event);
        });
        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
          auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
          MouseScrollEvent event(math::Vector2(x, y));
          input->messaging->send(event);
        });
        if (disableCursor) {
          glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
      }

      bool isPressed(Key key) const override {
        if (!window) {
          return false;
        }
        return glfwGetKey(window->as<GLFWwindow>(), codeFrom(key)) == GLFW_PRESS;
      }

      bool isPressed(MouseButton button) const override {
        if (!window) {
          return false;
        }
        return glfwGetMouseButton(window->as<GLFWwindow>(), codeFrom(button)) == GLFW_PRESS;
      }

      math::Vector2 mousePosition() const override {
        if (!window) {
          return math::Vector2();
        }
        double x, y;
        glfwGetCursorPos(window->as<GLFWwindow>(), &x, &y);
        return math::Vector2(x, y);
      }

      private:
        bool disableCursor = true;
    };
  }
}