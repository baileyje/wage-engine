#pragma once

namespace wage {
  namespace input {

    enum class Key {
      unknown,
      space,
      apostrophe,
      comma,
      minus,
      period,
      slash,
      zero,
      one,
      two,
      three,
      four,
      five,
      six,
      seven,
      eight,
      nine,
      semicolon,
      equal,
      a,
      b,
      c,
      d,
      e,
      f,
      g,
      h,
      i,
      j,
      k,
      l,
      m,
      n,
      o,
      p,
      q,
      r,
      s,
      t,
      u,
      v,
      w,
      x,
      y,
      z,
      leftBracket,
      backslash,
      rightBracket,
      graveAccent,
      world1,
      world2,
      escape,
      enter,
      tab,
      backspace,
      insert,
      del,
      right,
      left,
      down,
      up,
      pageUp,
      pageDown,
      home,
      end,
      capsLock,
      scrollLock,
      numLock,
      printScreen,
      pause,
      f1,
      f2,
      f3,
      f4,
      f5,
      f6,
      f7,
      f8,
      f9,
      f10,
      f11,
      f12,
      f13,
      f14,
      f15,
      f16,
      f17,
      f18,
      f19,
      f20,
      f21,
      f22,
      f23,
      f24,
      f25,
      kP0,
      kP1,
      kP2,
      kP3,
      kP4,
      kP5,
      kP6,
      kP7,
      kP8,
      kP9,
      kPDecimal,
      kPDivide,
      kPMultiply,
      kPSubtract,
      kPAdd,
      kPEnter,
      kPEqual,
      leftShift,
      leftControl,
      leftAlt,
      leftSuper,
      rightShift,
      rightControl,
      rightAlt,
      rightSuper,
      menu
    };

  }
}
template <>
class std::hash<wage::input::Key> {
public:
  std::size_t operator()(wage::input::Key const& key) const noexcept {
    return static_cast<std::size_t>(key) - static_cast<std::size_t>(wage::input::Key::unknown);
  }
};