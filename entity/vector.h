#ifndef ENTITY_VECTOR_H
#define ENTITY_VECTOR_H

// TODO:  Support V2 and V4
class Vector {

public:

  Vector() : x(0), y(0), z(0) {}

  Vector(float x, float y) : x(x), y(y), z(0) {}
  
  Vector(float x, float y, float z) : x(x), y(y), z(z) {}

  ~Vector() {}

  float x;

  float y;

  float z;
  
  friend Vector operator+(Vector left, const Vector& right) {
    left += right;
    return left;
  }

  inline Vector& operator+=(Vector other) {
    x += other.x;
    y += other.y;
    z += other.z;    
    return *this;
  }

private:

};


#endif //ENTITY_VECTOR_H