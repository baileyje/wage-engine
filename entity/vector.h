#ifndef ENTITY_VECTOR_H
#define ENTITY_VECTOR_H


class Vector {

public:

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