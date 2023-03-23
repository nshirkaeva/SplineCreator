
#include "pch.h"
#include "PolylineJig.h"
#include "Besier/CurveInterface.h"
#include <typeinfo>

//-----------------------------------------------------------------------------
PolylineJig::PolylineJig() : AcEdJig(), mCurrentInputLevel( -1 ), mpPolyline( NULL ), mpSpline( NULL ), flagStop(false)
{
}

PolylineJig::~PolylineJig()
{
}

//-----------------------------------------------------------------------------
AcEdJig::DragStatus PolylineJig::startJig( AcDbPolyline* pEntity, CurveInterface* newSpline )
{
  //- Store the new entity pointer
  mpPolyline = pEntity;
  mpSpline = newSpline;

  //- Setup each input prompt
  ACHAR* inputPrompts[] =
  {
    _T( "\nПервая точка: " ),
    _T( "\nУкажите следующую точку: " ),
  };

  mInputPoints.clear();

  bool appendOk = true;
  flagStop = false;
  AcEdJig::DragStatus status = AcEdJig::kNull;
  mCurrentInputLevel = -1;
  bool quit = false;

  //- Loop the number of inputs
  while ( true )
  {
    ++mCurrentInputLevel;
    if ( flagStop )
    {
      mpPolyline->removeVertexAt( mCurrentInputLevel - 1 );
      appendOk = true;
      break;
    }
    //- Add a new input point to the list of input points
    mInputPoints.append( AcGePoint3d() );
    //- Set the input prompt
    setDispPrompt( mCurrentInputLevel == 0 ? inputPrompts[ 0 ] : inputPrompts[ 1 ] );

    mpPolyline->addVertexAt( mCurrentInputLevel, mInputPoints[ mCurrentInputLevel ].asPoint2d() );

    //- Lets now do the input
    status = drag();
    if ( status != kNormal )
    {
      appendOk = status == kCancel; //keyword
      quit = true;
    }
    else
    {
      appendOk = true;
    }

    //- If to finish
    if ( quit )
      break;
  }

  //- If the input went well
  if ( appendOk )
  {
  //- Append to the database
    append();
  }

  else
    //- Clean up
  {
    delete mpPolyline;
  }

  return ( status );
}

//-----------------------------------------------------------------------------
//- Input sampler
AcEdJig::DragStatus PolylineJig::sampler()
{
  return mCurrentInputLevel == 0 ? GetStartPoint() : GetNextPoint();
}

//-----------------------------------------------------------------------------
//- Jigged entity update
Adesk::Boolean PolylineJig::update()
{
  //- Check the current input number to see which update to do
  if ( mCurrentInputLevel == 0 )
  {
    AcGeMatrix3d UcsToWcsMat;
    acdbUcsMatrix( UcsToWcsMat );

    // Get data from user coordinate system.
    //
    AcGePoint3d orgPt;
    AcGeVector3d xAxis, yAxis, zAxis;
    UcsToWcsMat.getCoordSystem( orgPt, xAxis, yAxis, zAxis );
    mpPolyline->setDatabaseDefaults();
    mpPolyline->setNormal( zAxis );
    mpPolyline->setClosed( false );
    mpPolyline->setPointAt( mCurrentInputLevel, mInputPoints[ mCurrentInputLevel ].asPoint2d() );

    return true;
  }
    // Plane of construction   
    mpPolyline->setPointAt( mCurrentInputLevel , mInputPoints[ mCurrentInputLevel ].asPoint2d() );
    return true;
}

//-----------------------------------------------------------------------------
//- Jigged entity pointer return
AcDbEntity* PolylineJig::entity() const
{
  return ( ( AcDbEntity* )mpPolyline );
}

//-----------------------------------------------------------------------------
//- Std input to get a point with no rubber band
AcEdJig::DragStatus PolylineJig::GetStartPoint()
{
  AcGePoint3d newPnt;
  //- Get the point 
  AcEdJig::DragStatus status = acquirePoint( newPnt );
  //- If valid input
  if ( status == AcEdJig::kNormal )
  {
    //- If there is no difference
    if ( newPnt.isEqualTo( mInputPoints[ mCurrentInputLevel ] ) )
      return ( AcEdJig::kNoChange );
    //- Otherwise update the point
    mInputPoints[ mCurrentInputLevel ] = newPnt;
  }
   return ( status );
}

//-----------------------------------------------------------------------------
//- Std input to get a point with rubber band from point
AcEdJig::DragStatus PolylineJig::GetNextPoint()
{
  AcGePoint3d newPnt;
  //- Get the point 
  auto status = acquirePoint( newPnt, mInputPoints[ mCurrentInputLevel - 1 ] );

  //- If invalid input
  if ( status != AcEdJig::kNormal )
    return status;

  //- If there is no difference
  if ( newPnt.isEqualTo( mInputPoints[ mCurrentInputLevel ] ) )
    return ( AcEdJig::kNoChange );

  mInputPoints[ mCurrentInputLevel ] = newPnt;

  if ( mCurrentInputLevel > 1)
    flagStop = !renderSpline();

    return ( status );
}

bool PolylineJig::renderSpline()
{
  const auto count = 100;
  const auto indexBegin = mCurrentInputLevel + 1;
  const auto indexEnd = indexBegin + count;
  auto t = 1.;
  const auto dt = ( t ) / count;

  mpSpline->setPoints( mInputPoints );

  for ( auto i = indexBegin; i <= indexEnd; ++i )
  {
    NcGePoint3d newPoint;
    if ( mpSpline->point( t, newPoint ) )
    {
      mpPolyline->removeVertexAt( i );
      mpPolyline->addVertexAt( i, newPoint.asPoint2d() );
      t -= dt;
    }
    else
    {
      return false;
    }
  }

  return true;
}
