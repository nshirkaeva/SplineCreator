#include "../pch.h"
#include "CurveInterface.h"

void CurveBesier::setPoints( const AcGePoint3dArray& inputPoints )
{
  curve.clear();
  for ( auto& it : inputPoints )
  {
    Point point;
    point.x = it.x;
    point.y = it.y;
    point.z = it.z;
    curve.push_back( point );
  }
}

bool CurveBesier::point( double t, NcGePoint3d& newPoint )
{
  auto newPnt = point( curve, t, curve.size() - 1, curve.size() - 1 );
  newPoint = { newPnt.x, newPnt.y, newPnt.z };

  return true;
}

Point CurveBesier::point( const vPoints& curve, double t, size_t n, size_t i )
{
  return n == 0 ? curve[ i ] : ( point( curve, t, n - 1, i - 1 ) * ( 1 - t ) + point( curve, t, n - 1, i ) * t );
}

void CurveQuadratic::setPoints( const AcGePoint3dArray& inputPoints )
{
  curve.clear();
  for ( auto& it : inputPoints )
  {
    Point point;
    point.x = it.x;
    point.y = it.y;
    point.z = it.z;
    curve.push_back( point );
  }
}

bool CurveQuadratic::point( double t, NcGePoint3d& newPoint )
{
  Point findPoint;
  if ( !point( curve, t, findPoint ) )
    return false;

  newPoint = { findPoint.x, findPoint.y, findPoint.z };

  return true;
}

void CurveQuadratic::pos( const vPoints& curve, double xval, int* left, int* right )
{
  int i;

  for ( i = 2; i <= curve.size() - 1; i++ )
  {
    if ( xval < curve[ i - 1 ].x )
    {
      *left = i - 1;
      *right = i;
      return;
    }

  }

  *left = curve.size() - 1;
  *right = curve.size();

  return;
}


bool CurveQuadratic::point( const vPoints& curve, double tval, Point& point )
{
  double dif1;
  double dif2;
  int left;
  int right;
  double t1;
  double t2;
  double t3;
  double y1;
  double y2;
  double y3;

  if ( curve.size() != 3 )
    return false;


  Point P0 = curve[ curve.size() - 3 ];
  Point P1 = curve[ curve.size() - 2 ];
  Point P2 = curve[ curve.size() - 1 ];

  tval = (P2.x - P0.x ) * ( tval ) + P0.x;

  pos( curve, tval, &left, &right );
  //
  //  Force LEFT to be odd.
  //
  if ( left % 2 == 0 )
  {
    left = left - 1;
  }
  //
  //  Copy out the three abscissas.
  //
  t1 = curve[ left - 1 ].x;
  t2 = curve[ left ].x;
  t3 = curve[ left + 1 ].x;

  if ( t2 <= t1 || t3 <= t2 )
  {
    return false;
  }
  //
  //  Construct and evaluate a parabolic interpolant for the data
  //  in each dimension.
  //
  y1 = curve[ left - 1 ].y;
  y2 = curve[ left ].y;
  y3 = curve[ left + 1 ].y;

  dif1 = ( y2 - y1 ) / ( t2 - t1 );

  dif2 = ( ( y3 - y1 ) / ( t3 - t1 )
    - ( y2 - y1 ) / ( t2 - t1 ) ) / ( t3 - t2 );

  point = { tval, y1 + ( tval - t1 ) * ( dif1 + ( tval - t2 ) * dif2 ) };
  return true;
}
