#pragma once

#include "Point.h"
#include <vector>

using vPoints = std::vector<Point>;

class CurveInterface
{
public:
  CurveInterface() {}
  virtual ~CurveInterface() {}
  virtual void setPoints( const AcGePoint3dArray& inputPoints ) = 0;
  virtual bool point( double t, NcGePoint3d& newPoint ) = 0;
};

class CurveBesier : public CurveInterface
{
public:
  CurveBesier() { curve = {}; }
  virtual ~CurveBesier() {}

  void setPoints( const AcGePoint3dArray& inputPoints ) override;
  bool point( double t, NcGePoint3d& newPoint ) override;

private:
  Point point( const vPoints& curve, double t, size_t n, size_t i );

private:
  vPoints curve;
};

class CurveQuadratic : public CurveInterface
{
public:
  CurveQuadratic() { curve = {}; }
  virtual ~CurveQuadratic() {}

  void setPoints( const AcGePoint3dArray& inputPoints ) override;
  bool point( double t, NcGePoint3d& newPoint ) override;

private:
  void pos( const vPoints& curve, double xval, int* left, int* right );
  bool point( const vPoints& curve, double tval, Point& point );

private:
  vPoints curve;
};
