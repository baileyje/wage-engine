#pragma once

namespace wage { namespace math {

  class Color {

  public:
    static Color White;

    static Color Black;

    static Color Red;

    static Color Green;

    static Color Blue;

    static Color Clear;

    Color() : Color(0, 0, 0, 1) {}

    Color(float r, float g, float b) : Color(r, g, b, 1) {}

    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    ~Color() {}

    float r;

    float g;

    float b;

    float a;

    friend Color operator+(Color left, const Color& right) {
      left += right;
      return left;
    }

    inline Color& operator+=(Color other) {
      r += other.r;
      g += other.g;
      b += other.b;
      a += other.a;
      return *this;
    }
  };

} }
