#ifndef ENTITY_VECTOR_H
#define ENTITY_VECTOR_H

// TODO:  Support V2 and V4
class Vector {

public:

  Vector() : x(0), y(0), z(0) {}

  Vector(double x, double y) : x(x), y(y), z(0) {}
  
  Vector(double x, double y, double z) : x(x), y(y), z(z) {}

  ~Vector() {}

  double getX() { return x; }

  double getY() { return y; }

  double getZ() { return z; }

  void setX(double value) { x = value; }

  void setY(double value) { y = value; }

  void setZ(double value) { z = value; }

private:

  double x;

  double y;

  double z;

};


#endif //ENTITY_VECTOR_H