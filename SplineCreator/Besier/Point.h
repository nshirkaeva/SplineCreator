#pragma once

class Point
{
  public:
    Point() { x = NULL; y = NULL; z = NULL; }
    Point( double x, double y ) : x( x ), y( y ), z( z ){}

    double x;
    double y;
    double z;

  Point operator*( double b )
  {
    this->x *= b;
    this->y *= b;
    this->z *= b;

    return *this;
  }

  Point operator/( double b )
  {
    this->x /= b;
    this->y /= b;
    this->z /= b;

    return *this;
  }

  Point operator+( Point b )
  {
    this->x += b.x;
    this->y += b.y;
    this->z += b.z;

    return *this;
  }

  Point operator-( Point b )
  {
    this->x -= b.x;
    this->y -= b.y;
    this->z -= b.z;

    return *this;
  }
};
