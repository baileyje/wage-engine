#include "input-glfw/input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/core.h"

#include "input/key_event.h"
#include "input/key_modifier.h"
#include "input/mouse_button_event.h"
#include "input/mouse_move_event.h"
#include "input/mouse_scroll_event.h"

namespace wage {

  Key keyFrom(int keyCode);

  int codeFrom(Key key);

  void GlfwInput::start() {
    Input::start();
    auto glfwWindow = window->as<GLFWwindow>();
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int keyCode, int scancode, int action, int mods) {
      auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
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
      auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
      auto eventType = action == GLFW_RELEASE ? MouseButtonEventType::release : MouseButtonEventType::press;
      MouseButtonEvent event(button, eventType, mods);
      input->messaging->send(event);
    });
    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
      auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
      MouseMoveEvent event(Vector2(x, y));
      input->messaging->send(event);
    });
    glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double x, double y) {
      auto input = static_cast<GlfwInput*>(glfwGetWindowUserPointer(window));
      MouseScrollEvent event(Vector2(x, y));
      input->messaging->send(event);
    });
    // glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  bool GlfwInput::isPressed(Key key) {
    if (!window) {
      return false;
    }
    return glfwGetKey(window->as<GLFWwindow>(), codeFrom(key)) == GLFW_PRESS;
  }

  Vector2 GlfwInput::mousePosition() {
    if (!window) {
      return Vector2();
    }
    double x, y;
    glfwGetCursorPos(window->as<GLFWwindow>(), &x, &y);
    return Vector2(x, y);
  }

  Key keyFrom(int keyCode) {
    switch (keyCode) {
    case GLFW_KEY_UNKNOWN:
      return Key::unknown;
    case GLFW_KEY_SPACE:
      return Key::space;
    case GLFW_KEY_APOSTROPHE:
      return Key::apostrophe;
    case GLFW_KEY_COMMA:
      return Key::comma;
    case GLFW_KEY_MINUS:
      return Key::minus;
    case GLFW_KEY_PERIOD:
      return Key::period;
    case GLFW_KEY_SLASH:
      return Key::slash;
    case GLFW_KEY_0:
      return Key::zero;
    case GLFW_KEY_1:
      return Key::one;
    case GLFW_KEY_2:
      return Key::two;
    case GLFW_KEY_3:
      return Key::three;
    case GLFW_KEY_4:
      return Key::four;
    case GLFW_KEY_5:
      return Key::five;
    case GLFW_KEY_6:
      return Key::six;
    case GLFW_KEY_7:
      return Key::seven;
    case GLFW_KEY_8:
      return Key::eight;
    case GLFW_KEY_9:
      return Key::nine;
    case GLFW_KEY_SEMICOLON:
      return Key::semicolon;
    case GLFW_KEY_EQUAL:
      return Key::equal;
    case GLFW_KEY_A:
      return Key::a;
    case GLFW_KEY_B:
      return Key::b;
    case GLFW_KEY_C:
      return Key::c;
    case GLFW_KEY_D:
      return Key::d;
    case GLFW_KEY_E:
      return Key::e;
    case GLFW_KEY_F:
      return Key::f;
    case GLFW_KEY_G:
      return Key::g;
    case GLFW_KEY_H:
      return Key::h;
    case GLFW_KEY_I:
      return Key::i;
    case GLFW_KEY_J:
      return Key::j;
    case GLFW_KEY_K:
      return Key::k;
    case GLFW_KEY_L:
      return Key::l;
    case GLFW_KEY_M:
      return Key::m;
    case GLFW_KEY_N:
      return Key::n;
    case GLFW_KEY_O:
      return Key::o;
    case GLFW_KEY_P:
      return Key::p;
    case GLFW_KEY_Q:
      return Key::q;
    case GLFW_KEY_R:
      return Key::r;
    case GLFW_KEY_S:
      return Key::s;
    case GLFW_KEY_T:
      return Key::t;
    case GLFW_KEY_U:
      return Key::u;
    case GLFW_KEY_V:
      return Key::v;
    case GLFW_KEY_W:
      return Key::w;
    case GLFW_KEY_X:
      return Key::x;
    case GLFW_KEY_Y:
      return Key::y;
    case GLFW_KEY_Z:
      return Key::z;
    case GLFW_KEY_LEFT_BRACKET:
      return Key::leftBracket;
    case GLFW_KEY_BACKSLASH:
      return Key::backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return Key::rightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
      return Key::graveAccent;
    case GLFW_KEY_WORLD_1:
      return Key::world1;
    case GLFW_KEY_WORLD_2:
      return Key::world2;
    case GLFW_KEY_ESCAPE:
      return Key::escape;
    case GLFW_KEY_ENTER:
      return Key::enter;
    case GLFW_KEY_TAB:
      return Key::tab;
    case GLFW_KEY_BACKSPACE:
      return Key::backspace;
    case GLFW_KEY_INSERT:
      return Key::insert;
    case GLFW_KEY_DELETE:
      return Key::del;
    case GLFW_KEY_RIGHT:
      return Key::right;
    case GLFW_KEY_LEFT:
      return Key::left;
    case GLFW_KEY_DOWN:
      return Key::down;
    case GLFW_KEY_UP:
      return Key::up;
    case GLFW_KEY_PAGE_UP:
      return Key::pageUp;
    case GLFW_KEY_PAGE_DOWN:
      return Key::pageDown;
    case GLFW_KEY_HOME:
      return Key::home;
    case GLFW_KEY_END:
      return Key::end;
    case GLFW_KEY_CAPS_LOCK:
      return Key::capsLock;
    case GLFW_KEY_SCROLL_LOCK:
      return Key::scrollLock;
    case GLFW_KEY_NUM_LOCK:
      return Key::numLock;
    case GLFW_KEY_PRINT_SCREEN:
      return Key::printScreen;
    case GLFW_KEY_PAUSE:
      return Key::pause;
    case GLFW_KEY_F1:
      return Key::f1;
    case GLFW_KEY_F2:
      return Key::f2;
    case GLFW_KEY_F3:
      return Key::f3;
    case GLFW_KEY_F4:
      return Key::f4;
    case GLFW_KEY_F5:
      return Key::f5;
    case GLFW_KEY_F6:
      return Key::f6;
    case GLFW_KEY_F7:
      return Key::f7;
    case GLFW_KEY_F8:
      return Key::f8;
    case GLFW_KEY_F9:
      return Key::f9;
    case GLFW_KEY_F10:
      return Key::f10;
    case GLFW_KEY_F11:
      return Key::f11;
    case GLFW_KEY_F12:
      return Key::f12;
    case GLFW_KEY_F13:
      return Key::f13;
    case GLFW_KEY_F14:
      return Key::f14;
    case GLFW_KEY_F15:
      return Key::f15;
    case GLFW_KEY_F16:
      return Key::f16;
    case GLFW_KEY_F17:
      return Key::f17;
    case GLFW_KEY_F18:
      return Key::f18;
    case GLFW_KEY_F19:
      return Key::f19;
    case GLFW_KEY_F20:
      return Key::f20;
    case GLFW_KEY_F21:
      return Key::f21;
    case GLFW_KEY_F22:
      return Key::f22;
    case GLFW_KEY_F23:
      return Key::f23;
    case GLFW_KEY_F24:
      return Key::f24;
    case GLFW_KEY_F25:
      return Key::f25;
    case GLFW_KEY_KP_0:
      return Key::kP0;
    case GLFW_KEY_KP_1:
      return Key::kP1;
    case GLFW_KEY_KP_2:
      return Key::kP2;
    case GLFW_KEY_KP_3:
      return Key::kP3;
    case GLFW_KEY_KP_4:
      return Key::kP4;
    case GLFW_KEY_KP_5:
      return Key::kP5;
    case GLFW_KEY_KP_6:
      return Key::kP6;
    case GLFW_KEY_KP_7:
      return Key::kP7;
    case GLFW_KEY_KP_8:
      return Key::kP8;
    case GLFW_KEY_KP_9:
      return Key::kP9;
    case GLFW_KEY_KP_DECIMAL:
      return Key::kPDecimal;
    case GLFW_KEY_KP_DIVIDE:
      return Key::kPDivide;
    case GLFW_KEY_KP_MULTIPLY:
      return Key::kPMultiply;
    case GLFW_KEY_KP_SUBTRACT:
      return Key::kPSubtract;
    case GLFW_KEY_KP_ADD:
      return Key::kPAdd;
    case GLFW_KEY_KP_ENTER:
      return Key::kPEnter;
    case GLFW_KEY_KP_EQUAL:
      return Key::kPEqual;
    case GLFW_KEY_LEFT_SHIFT:
      return Key::leftShift;
    case GLFW_KEY_LEFT_CONTROL:
      return Key::leftControl;
    case GLFW_KEY_LEFT_ALT:
      return Key::leftAlt;
    case GLFW_KEY_LEFT_SUPER:
      return Key::leftSuper;
    case GLFW_KEY_RIGHT_SHIFT:
      return Key::rightShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return Key::rightControl;
    case GLFW_KEY_RIGHT_ALT:
      return Key::rightAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return Key::rightSuper;
    case GLFW_KEY_MENU:
      return Key::menu;
    }
    return Key::unknown;
  }

  int codeFrom(Key key) {
    switch (key) {
    case Key::unknown:
      return GLFW_KEY_UNKNOWN;
    case Key::space:
      return GLFW_KEY_SPACE;
    case Key::apostrophe:
      return GLFW_KEY_APOSTROPHE;
    case Key::comma:
      return GLFW_KEY_COMMA;
    case Key::minus:
      return GLFW_KEY_MINUS;
    case Key::period:
      return GLFW_KEY_PERIOD;
    case Key::slash:
      return GLFW_KEY_SLASH;
    case Key::zero:
      return GLFW_KEY_0;
    case Key::one:
      return GLFW_KEY_1;
    case Key::two:
      return GLFW_KEY_2;
    case Key::three:
      return GLFW_KEY_3;
    case Key::four:
      return GLFW_KEY_4;
    case Key::five:
      return GLFW_KEY_5;
    case Key::six:
      return GLFW_KEY_6;
    case Key::seven:
      return GLFW_KEY_7;
    case Key::eight:
      return GLFW_KEY_8;
    case Key::nine:
      return GLFW_KEY_9;
    case Key::semicolon:
      return GLFW_KEY_SEMICOLON;
    case Key::equal:
      return GLFW_KEY_EQUAL;
    case Key::a:
      return GLFW_KEY_A;
    case Key::b:
      return GLFW_KEY_B;
    case Key::c:
      return GLFW_KEY_C;
    case Key::d:
      return GLFW_KEY_D;
    case Key::e:
      return GLFW_KEY_E;
    case Key::f:
      return GLFW_KEY_F;
    case Key::g:
      return GLFW_KEY_G;
    case Key::h:
      return GLFW_KEY_H;
    case Key::i:
      return GLFW_KEY_I;
    case Key::j:
      return GLFW_KEY_J;
    case Key::k:
      return GLFW_KEY_K;
    case Key::l:
      return GLFW_KEY_L;
    case Key::m:
      return GLFW_KEY_M;
    case Key::n:
      return GLFW_KEY_N;
    case Key::o:
      return GLFW_KEY_O;
    case Key::p:
      return GLFW_KEY_P;
    case Key::q:
      return GLFW_KEY_Q;
    case Key::r:
      return GLFW_KEY_R;
    case Key::s:
      return GLFW_KEY_S;
    case Key::t:
      return GLFW_KEY_T;
    case Key::u:
      return GLFW_KEY_U;
    case Key::v:
      return GLFW_KEY_V;
    case Key::w:
      return GLFW_KEY_W;
    case Key::x:
      return GLFW_KEY_X;
    case Key::y:
      return GLFW_KEY_Y;
    case Key::z:
      return GLFW_KEY_Z;
    case Key::leftBracket:
      return GLFW_KEY_LEFT_BRACKET;
    case Key::backslash:
      return GLFW_KEY_BACKSLASH;
    case Key::rightBracket:
      return GLFW_KEY_RIGHT_BRACKET;
    case Key::graveAccent:
      return GLFW_KEY_GRAVE_ACCENT;
    case Key::world1:
      return GLFW_KEY_WORLD_1;
    case Key::world2:
      return GLFW_KEY_WORLD_2;
    case Key::escape:
      return GLFW_KEY_ESCAPE;
    case Key::enter:
      return GLFW_KEY_ENTER;
    case Key::tab:
      return GLFW_KEY_TAB;
    case Key::backspace:
      return GLFW_KEY_BACKSPACE;
    case Key::insert:
      return GLFW_KEY_INSERT;
    case Key::del:
      return GLFW_KEY_DELETE;
    case Key::right:
      return GLFW_KEY_RIGHT;
    case Key::left:
      return GLFW_KEY_LEFT;
    case Key::down:
      return GLFW_KEY_DOWN;
    case Key::up:
      return GLFW_KEY_UP;
    case Key::pageUp:
      return GLFW_KEY_PAGE_UP;
    case Key::pageDown:
      return GLFW_KEY_PAGE_DOWN;
    case Key::home:
      return GLFW_KEY_HOME;
    case Key::end:
      return GLFW_KEY_END;
    case Key::capsLock:
      return GLFW_KEY_CAPS_LOCK;
    case Key::scrollLock:
      return GLFW_KEY_SCROLL_LOCK;
    case Key::numLock:
      return GLFW_KEY_NUM_LOCK;
    case Key::printScreen:
      return GLFW_KEY_PRINT_SCREEN;
    case Key::pause:
      return GLFW_KEY_PAUSE;
    case Key::f1:
      return GLFW_KEY_F1;
    case Key::f2:
      return GLFW_KEY_F2;
    case Key::f3:
      return GLFW_KEY_F3;
    case Key::f4:
      return GLFW_KEY_F4;
    case Key::f5:
      return GLFW_KEY_F5;
    case Key::f6:
      return GLFW_KEY_F6;
    case Key::f7:
      return GLFW_KEY_F7;
    case Key::f8:
      return GLFW_KEY_F8;
    case Key::f9:
      return GLFW_KEY_F9;
    case Key::f10:
      return GLFW_KEY_F10;
    case Key::f11:
      return GLFW_KEY_F11;
    case Key::f12:
      return GLFW_KEY_F12;
    case Key::f13:
      return GLFW_KEY_F13;
    case Key::f14:
      return GLFW_KEY_F14;
    case Key::f15:
      return GLFW_KEY_F15;
    case Key::f16:
      return GLFW_KEY_F16;
    case Key::f17:
      return GLFW_KEY_F17;
    case Key::f18:
      return GLFW_KEY_F18;
    case Key::f19:
      return GLFW_KEY_F19;
    case Key::f20:
      return GLFW_KEY_F20;
    case Key::f21:
      return GLFW_KEY_F21;
    case Key::f22:
      return GLFW_KEY_F22;
    case Key::f23:
      return GLFW_KEY_F23;
    case Key::f24:
      return GLFW_KEY_F24;
    case Key::f25:
      return GLFW_KEY_F25;
    case Key::kP0:
      return GLFW_KEY_KP_0;
    case Key::kP1:
      return GLFW_KEY_KP_1;
    case Key::kP2:
      return GLFW_KEY_KP_2;
    case Key::kP3:
      return GLFW_KEY_KP_3;
    case Key::kP4:
      return GLFW_KEY_KP_4;
    case Key::kP5:
      return GLFW_KEY_KP_5;
    case Key::kP6:
      return GLFW_KEY_KP_6;
    case Key::kP7:
      return GLFW_KEY_KP_7;
    case Key::kP8:
      return GLFW_KEY_KP_8;
    case Key::kP9:
      return GLFW_KEY_KP_9;
    case Key::kPDecimal:
      return GLFW_KEY_KP_DECIMAL;
    case Key::kPDivide:
      return GLFW_KEY_KP_DIVIDE;
    case Key::kPMultiply:
      return GLFW_KEY_KP_MULTIPLY;
    case Key::kPSubtract:
      return GLFW_KEY_KP_SUBTRACT;
    case Key::kPAdd:
      return GLFW_KEY_KP_ADD;
    case Key::kPEnter:
      return GLFW_KEY_KP_ENTER;
    case Key::kPEqual:
      return GLFW_KEY_KP_EQUAL;
    case Key::leftShift:
      return GLFW_KEY_LEFT_SHIFT;
    case Key::leftControl:
      return GLFW_KEY_LEFT_CONTROL;
    case Key::leftAlt:
      return GLFW_KEY_LEFT_ALT;
    case Key::leftSuper:
      return GLFW_KEY_LEFT_SUPER;
    case Key::rightShift:
      return GLFW_KEY_RIGHT_SHIFT;
    case Key::rightControl:
      return GLFW_KEY_RIGHT_CONTROL;
    case Key::rightAlt:
      return GLFW_KEY_RIGHT_ALT;
    case Key::rightSuper:
      return GLFW_KEY_RIGHT_SUPER;
    case Key::menu:
      return GLFW_KEY_MENU;
    }
    return GLFW_KEY_UNKNOWN;
  }
}
