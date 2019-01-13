#include "input/input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "platform/platform.h"
#include "core/system/context.h"
#include "core/logger.h"
#include "input/key_event.h"
#include "input/key.h"
#include "input/key_modifier.h"
#include "input/mouse_button_event.h"
#include "input/mouse_move_event.h"
#include "input/mouse_scroll_event.h"

namespace wage {
  
  Key keyFrom(int keyCode);

  Input* Input::Instance = new Input();;

  void Input::start(SystemContext* context) {
    Logger::info("Starting Input System");
    Input::Instance = this;
    messaging = context->get<Messaging>();
    Platform* platform = context->get<Platform>();
    window = platform->window();
    auto glfwWindow = window->as<GLFWwindow>();
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int keyCode, int scancode, int action, int mods) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      if (keyCode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
      } else if (action == GLFW_PRESS) {
        input->frameKeys.insert(keyCode);
      } else if (action == GLFW_RELEASE) {
        input->frameKeys.erase(keyCode);
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
    glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      auto eventType = action == GLFW_RELEASE ? MouseButtonEventType::release : MouseButtonEventType::press;
      MouseButtonEvent event(button, eventType, mods);
      input->messaging->send(event);
    });
    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      MouseMoveEvent event(Vector2(x, y));
      input->messaging->send(event);
    });
    glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      MouseScrollEvent event(Vector2(x, y));
      input->messaging->send(event);
    });
    // Input::Instance->setMousePosition(Vector2(1024/2, 768/2));
    glfwSetInputMode(window->as<GLFWwindow>(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void Input::update(SystemContext* context) {
    if (glfwWindowShouldClose(window->as<GLFWwindow>())) {
      context->shouldStop();    
      return;
    }
  }

  bool Input::isPressed(int key) {
    if (!window) {
      return false;
    }
    return frameKeys.count(key) > 0;
  }

  Vector2 Input::mousePosition() {
    if (!window) {
      return Vector2();
    }
    double x, y;    
    glfwGetCursorPos(window->as<GLFWwindow>(), &x, &y);
    return Vector2(x, y); 
  }

  Key keyFrom(int keyCode) {
    switch(keyCode) {
      case GLFW_KEY_UNKNOWN:
        return Key::Unknown;
      case GLFW_KEY_SPACE:
        return Key::Space;
      case GLFW_KEY_APOSTROPHE:
        return Key::Apostrophe;
      case GLFW_KEY_COMMA:
        return Key::Comma;
      case GLFW_KEY_MINUS:
        return Key::Minus;
      case GLFW_KEY_PERIOD:
        return Key::Period;
      case GLFW_KEY_SLASH:
        return Key::Slash;
      case GLFW_KEY_0:
        return Key::Zero;
      case GLFW_KEY_1:
        return Key::One;
      case GLFW_KEY_2:
        return Key::Two;
      case GLFW_KEY_3:
        return Key::Three;
      case GLFW_KEY_4:
        return Key::Four;
      case GLFW_KEY_5:
        return Key::Five;
      case GLFW_KEY_6:
        return Key::Six;
      case GLFW_KEY_7:
        return Key::Seven;
      case GLFW_KEY_8:
        return Key::Eight;
      case GLFW_KEY_9:
        return Key::Nine;
      case GLFW_KEY_SEMICOLON:
        return Key::Semicolon;
      case GLFW_KEY_EQUAL:
        return Key::Equal;
      case GLFW_KEY_A:
        return Key::A;
      case GLFW_KEY_B:
        return Key::B;
      case GLFW_KEY_C:
        return Key::C;
      case GLFW_KEY_D:
        return Key::D;
      case GLFW_KEY_E:
        return Key::E;
      case GLFW_KEY_F:
        return Key::F;
      case GLFW_KEY_G:
        return Key::G;
      case GLFW_KEY_H:
        return Key::H;
      case GLFW_KEY_I:
        return Key::I;
      case GLFW_KEY_J:
        return Key::J;
      case GLFW_KEY_K:
        return Key::K;
      case GLFW_KEY_L:
        return Key::L;
      case GLFW_KEY_M:
        return Key::M;
      case GLFW_KEY_N:
        return Key::N;
      case GLFW_KEY_O:
        return Key::O;
      case GLFW_KEY_P:
        return Key::P;
      case GLFW_KEY_Q:
        return Key::Q;
      case GLFW_KEY_R:
        return Key::R;
      case GLFW_KEY_S:
        return Key::S;
      case GLFW_KEY_T:
        return Key::T;
      case GLFW_KEY_U:
        return Key::U;
      case GLFW_KEY_V:
        return Key::V;
      case GLFW_KEY_W:
        return Key::W;
      case GLFW_KEY_X:
        return Key::X;
      case GLFW_KEY_Y:
        return Key::Y;
      case GLFW_KEY_Z:
        return Key::Z;
      case GLFW_KEY_LEFT_BRACKET:
        return Key::LeftBracket;
      case GLFW_KEY_BACKSLASH:
        return Key::Backslash;
      case GLFW_KEY_RIGHT_BRACKET:
        return Key::RightBracket;
      case GLFW_KEY_GRAVE_ACCENT:
        return Key::GraveAccent;
      case GLFW_KEY_WORLD_1:
        return Key::World1;
      case GLFW_KEY_WORLD_2:
        return Key::World2;
      case GLFW_KEY_ESCAPE:
        return Key::Escape;
      case GLFW_KEY_ENTER:
        return Key::Enter;
      case GLFW_KEY_TAB:
        return Key::Tab;
      case GLFW_KEY_BACKSPACE:
        return Key::Backspace;
      case GLFW_KEY_INSERT:
        return Key::Insert;
      case GLFW_KEY_DELETE:
        return Key::Delete;
      case GLFW_KEY_RIGHT:
        return Key::Right;
      case GLFW_KEY_LEFT:
        return Key::Left;
      case GLFW_KEY_DOWN:
        return Key::Down;
      case GLFW_KEY_UP:
        return Key::Up;
      case GLFW_KEY_PAGE_UP:
        return Key::PageUp;
      case GLFW_KEY_PAGE_DOWN:
        return Key::PageDown;
      case GLFW_KEY_HOME:
        return Key::Home;
      case GLFW_KEY_END:
        return Key::End;
      case GLFW_KEY_CAPS_LOCK:
        return Key::CapsLock;
      case GLFW_KEY_SCROLL_LOCK:
        return Key::ScrollLock;
      case GLFW_KEY_NUM_LOCK:
        return Key::NumLock;
      case GLFW_KEY_PRINT_SCREEN:
        return Key::PrintScreen;
      case GLFW_KEY_PAUSE:
        return Key::Pause;
      case GLFW_KEY_F1:
        return Key::F1;
      case GLFW_KEY_F2:
        return Key::F2;
      case GLFW_KEY_F3:
        return Key::F3;
      case GLFW_KEY_F4:
        return Key::F4;
      case GLFW_KEY_F5:
        return Key::F5;
      case GLFW_KEY_F6:
        return Key::F6;
      case GLFW_KEY_F7:
        return Key::F7;
      case GLFW_KEY_F8:
        return Key::F8;
      case GLFW_KEY_F9:
        return Key::F9;
      case GLFW_KEY_F10:
        return Key::F10;
      case GLFW_KEY_F11:
        return Key::F11;
      case GLFW_KEY_F12:
        return Key::F12;
      case GLFW_KEY_F13:
        return Key::F13;
      case GLFW_KEY_F14:
        return Key::F14;
      case GLFW_KEY_F15:
        return Key::F15;
      case GLFW_KEY_F16:
        return Key::F16;
      case GLFW_KEY_F17:
        return Key::F17;
      case GLFW_KEY_F18:
        return Key::F18;
      case GLFW_KEY_F19:
        return Key::F19;
      case GLFW_KEY_F20:
        return Key::F20;
      case GLFW_KEY_F21:
        return Key::F21;
      case GLFW_KEY_F22:
        return Key::F22;
      case GLFW_KEY_F23:
        return Key::F23;
      case GLFW_KEY_F24:
        return Key::F24;
      case GLFW_KEY_F25:
        return Key::F25;
      case GLFW_KEY_KP_0:
        return Key::KP0;
      case GLFW_KEY_KP_1:
        return Key::KP1;
      case GLFW_KEY_KP_2:
        return Key::KP2;
      case GLFW_KEY_KP_3:
        return Key::KP3;
      case GLFW_KEY_KP_4:
        return Key::KP4;
      case GLFW_KEY_KP_5:
        return Key::KP5;
      case GLFW_KEY_KP_6:
        return Key::KP6;
      case GLFW_KEY_KP_7:
        return Key::KP7;
      case GLFW_KEY_KP_8:
        return Key::KP8;
      case GLFW_KEY_KP_9:
        return Key::KP9;
      case GLFW_KEY_KP_DECIMAL:
        return Key::KPDecimal;
      case GLFW_KEY_KP_DIVIDE:
        return Key::KPDivide;
      case GLFW_KEY_KP_MULTIPLY:
        return Key::KPMultiply;
      case GLFW_KEY_KP_SUBTRACT:
        return Key::KPSubtract;
      case GLFW_KEY_KP_ADD:
        return Key::KPAdd;
      case GLFW_KEY_KP_ENTER:
        return Key::KPEnter;
      case GLFW_KEY_KP_EQUAL:
        return Key::KPEqual;
      case GLFW_KEY_LEFT_SHIFT:
        return Key::LeftShift;
      case GLFW_KEY_LEFT_CONTROL:
        return Key::LeftControl;
      case GLFW_KEY_LEFT_ALT:
        return Key::LeftAlt;
      case GLFW_KEY_LEFT_SUPER:
        return Key::LeftSuper;
      case GLFW_KEY_RIGHT_SHIFT:
        return Key::RightShift;
      case GLFW_KEY_RIGHT_CONTROL:
        return Key::RightControl;
      case GLFW_KEY_RIGHT_ALT:
        return Key::RightAlt;
      case GLFW_KEY_RIGHT_SUPER:
        return Key::RightSuper;
      case GLFW_KEY_MENU:
        return Key::Menu;
    }
    return Key::Unknown;
  }

}
