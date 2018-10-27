#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H


// TODO:  Support Euler
class Quaternion {

public:

  Quaternion() : x(0), y(0), z(0), w(0) {}
  
  Quaternion(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
  
  ~Quaternion() {}

  double getX() { return x; }

  double getY() { return y; }

  double getZ() { return z; }

  double getW() { return w; }

  void setX(double value) { x = value; }

  void setY(double value) { y = value; }

  void setZ(double value) { z = value; }

  void setW(double value) { w = value; }

private:

  double x;

  double y;

  double z;
  
  double w;
  
};

#endif //MATH_QUATERNION_H