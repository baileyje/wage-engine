#include "input/input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/core.h"
#include "core/logger.h"
#include "platform/platform.h"

#include "input/key_event.h"
#include "input/key.h"
#include "input/key_modifier.h"
#include "input/mouse_button_event.h"
#include "input/mouse_move_event.h"
#include "input/mouse_scroll_event.h"

namespace wage {
  
  Key keyFrom(int keyCode);

  int codeFrom(Key key);

  Input* Input::Instance = new Input();;

  void Input::start() {
    Input::Instance = this;
    messaging = Core::Instance->get<Messaging>();
    auto platform = Core::Instance->get<Platform>();
    window = platform->window();
    auto glfwWindow = window->as<GLFWwindow>();
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int keyCode, int scancode, int action, int mods) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      if (keyCode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        Core::Instance->stop();
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
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }


  bool Input::isPressed(Key key) {
    if (!window) {
      return false;
    }
    return glfwGetKey(window->as<GLFWwindow>(), codeFrom(key)) == GLFW_PRESS;
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

  int codeFrom(Key key) {
    switch(key) {
      case Key::Unknown:
        return GLFW_KEY_UNKNOWN;
      case Key::Space:
        return GLFW_KEY_SPACE;
      case Key::Apostrophe:
        return GLFW_KEY_APOSTROPHE;
      case Key::Comma:
        return GLFW_KEY_COMMA;
      case Key::Minus:
        return GLFW_KEY_MINUS;
      case Key::Period:
        return GLFW_KEY_PERIOD;
      case Key::Slash:
        return GLFW_KEY_SLASH;
      case Key::Zero:
        return GLFW_KEY_0;
      case Key::One:
        return GLFW_KEY_1;
      case Key::Two:
        return GLFW_KEY_2;
      case Key::Three:
        return GLFW_KEY_3;
      case Key::Four:
        return GLFW_KEY_4;
      case Key::Five:
        return GLFW_KEY_5;
      case Key::Six:
        return GLFW_KEY_6;
      case Key::Seven:
        return GLFW_KEY_7;
      case Key::Eight:
        return GLFW_KEY_8;
      case Key::Nine:
        return GLFW_KEY_9;
      case Key::Semicolon:
        return GLFW_KEY_SEMICOLON;
      case Key::Equal:
        return GLFW_KEY_EQUAL;
      case Key::A:
        return GLFW_KEY_A;
      case Key::B:
        return GLFW_KEY_B;
      case Key::C:
        return GLFW_KEY_C;
      case Key::D:
        return GLFW_KEY_D;
      case Key::E:
        return GLFW_KEY_E;
      case Key::F:
        return GLFW_KEY_F;
      case Key::G:
        return GLFW_KEY_G;
      case Key::H:
        return GLFW_KEY_H;
      case Key::I:
        return GLFW_KEY_I;
      case Key::J:
        return GLFW_KEY_J;
      case Key::K:
        return GLFW_KEY_K;
      case Key::L:
        return GLFW_KEY_L;
      case Key::M:
        return GLFW_KEY_M;
      case Key::N:
        return GLFW_KEY_N;
      case Key::O:
        return GLFW_KEY_O;
      case Key::P:
        return GLFW_KEY_P;
      case Key::Q:
        return GLFW_KEY_Q;
      case Key::R:
        return GLFW_KEY_R;
      case Key::S:
        return GLFW_KEY_S;
      case Key::T:
        return GLFW_KEY_T;
      case Key::U:
        return GLFW_KEY_U;
      case Key::V:
        return GLFW_KEY_V;
      case Key::W:
        return GLFW_KEY_W;
      case Key::X:
        return GLFW_KEY_X;
      case Key::Y:
        return GLFW_KEY_Y;
      case Key::Z:
        return GLFW_KEY_Z;
      case Key::LeftBracket:
        return GLFW_KEY_LEFT_BRACKET;
      case Key::Backslash:
        return GLFW_KEY_BACKSLASH;
      case Key::RightBracket:
        return GLFW_KEY_RIGHT_BRACKET;
      case Key::GraveAccent:
        return GLFW_KEY_GRAVE_ACCENT;
      case Key::World1:
        return GLFW_KEY_WORLD_1;
      case Key::World2:
        return GLFW_KEY_WORLD_2;
      case Key::Escape:
        return GLFW_KEY_ESCAPE;
      case Key::Enter:
        return GLFW_KEY_ENTER;
      case Key::Tab:
        return GLFW_KEY_TAB;
      case Key::Backspace:
        return GLFW_KEY_BACKSPACE;
      case Key::Insert:
        return GLFW_KEY_INSERT;
      case Key::Delete:
        return GLFW_KEY_DELETE;
      case Key::Right:
        return GLFW_KEY_RIGHT;
      case Key::Left:
        return GLFW_KEY_LEFT;
      case Key::Down:
        return GLFW_KEY_DOWN;
      case Key::Up:
        return GLFW_KEY_UP;
      case Key::PageUp:
        return GLFW_KEY_PAGE_UP;
      case Key::PageDown:
        return GLFW_KEY_PAGE_DOWN;
      case Key::Home:
        return GLFW_KEY_HOME;
      case Key::End:
        return GLFW_KEY_END;
      case Key::CapsLock:
        return GLFW_KEY_CAPS_LOCK;
      case Key::ScrollLock:
        return GLFW_KEY_SCROLL_LOCK;
      case Key::NumLock:
        return GLFW_KEY_NUM_LOCK;
      case Key::PrintScreen:
        return GLFW_KEY_PRINT_SCREEN;
      case Key::Pause:
        return GLFW_KEY_PAUSE;
      case Key::F1:
        return GLFW_KEY_F1;
      case Key::F2:
        return GLFW_KEY_F2;
      case Key::F3:
        return GLFW_KEY_F3;
      case Key::F4:
        return GLFW_KEY_F4;
      case Key::F5:
        return GLFW_KEY_F5;
      case Key::F6:
        return GLFW_KEY_F6;
      case Key::F7:
        return GLFW_KEY_F7;
      case Key::F8:
        return GLFW_KEY_F8;
      case Key::F9:
        return GLFW_KEY_F9;
      case Key::F10:
        return GLFW_KEY_F10;
      case Key::F11:
        return GLFW_KEY_F11;
      case Key::F12:
        return GLFW_KEY_F12;
      case Key::F13:
        return GLFW_KEY_F13;
      case Key::F14:
        return GLFW_KEY_F14;
      case Key::F15:
        return GLFW_KEY_F15;
      case Key::F16:
        return GLFW_KEY_F16;
      case Key::F17:
        return GLFW_KEY_F17;
      case Key::F18:
        return GLFW_KEY_F18;
      case Key::F19:
        return GLFW_KEY_F19;
      case Key::F20:
        return GLFW_KEY_F20;
      case Key::F21:
        return GLFW_KEY_F21;
      case Key::F22:
        return GLFW_KEY_F22;
      case Key::F23:
        return GLFW_KEY_F23;
      case Key::F24:
        return GLFW_KEY_F24;
      case Key::F25:
        return GLFW_KEY_F25;
      case Key::KP0:
        return GLFW_KEY_KP_0;
      case Key::KP1:
        return GLFW_KEY_KP_1;
      case Key::KP2:
        return GLFW_KEY_KP_2;
      case Key::KP3:
        return GLFW_KEY_KP_3;
      case Key::KP4:
        return GLFW_KEY_KP_4;
      case Key::KP5:
        return GLFW_KEY_KP_5;
      case Key::KP6:
        return GLFW_KEY_KP_6;
      case Key::KP7:
        return GLFW_KEY_KP_7;
      case Key::KP8:
        return GLFW_KEY_KP_8;
      case Key::KP9:
        return GLFW_KEY_KP_9;
      case Key::KPDecimal:
        return GLFW_KEY_KP_DECIMAL;
      case Key::KPDivide:
        return GLFW_KEY_KP_DIVIDE;
      case Key::KPMultiply:
        return GLFW_KEY_KP_MULTIPLY;
      case Key::KPSubtract:
        return GLFW_KEY_KP_SUBTRACT;
      case Key::KPAdd:
        return GLFW_KEY_KP_ADD;
      case Key::KPEnter:
        return GLFW_KEY_KP_ENTER;
      case Key::KPEqual:
        return GLFW_KEY_KP_EQUAL;
      case Key::LeftShift:
        return GLFW_KEY_LEFT_SHIFT;
      case Key::LeftControl:
        return GLFW_KEY_LEFT_CONTROL;
      case Key::LeftAlt:
        return GLFW_KEY_LEFT_ALT;
      case Key::LeftSuper:
        return GLFW_KEY_LEFT_SUPER;
      case Key::RightShift:
        return GLFW_KEY_RIGHT_SHIFT;
      case Key::RightControl:
        return GLFW_KEY_RIGHT_CONTROL;
      case Key::RightAlt:
        return GLFW_KEY_RIGHT_ALT;
      case Key::RightSuper:
        return GLFW_KEY_RIGHT_SUPER;
      case Key::Menu:
        return GLFW_KEY_MENU;
    }
    return GLFW_KEY_UNKNOWN;
  }

}
