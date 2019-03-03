#ifndef ENTITY_COLOR_H
#define ENTITY_COLOR_H

namespace wage {
  // TODO: Merge into Vector
  class Color {

  public:

    Color() : Color(0, 0, 0, 1) {}

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

}

#endif //ENTITY_COLOR_H