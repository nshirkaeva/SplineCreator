#pragma once
#include "arxHeaders.h"
#include "Besier/CurveInterface.h"
class PolylineJig : public AcEdJig
{
public:
  PolylineJig();
  ~PolylineJig();

  //- Command invoke the jig, call passing a new'd instance of the object to jig
  AcEdJig::DragStatus startJig( AcDbPolyline* pEntityToJig, CurveInterface* newSpline );

protected:
  //- AcEdJig overrides
  //- input sampler
  virtual DragStatus sampler();
  //- jigged entity update
  virtual Adesk::Boolean update();
  //- jigged entity pointer return
  virtual AcDbEntity* entity() const;

  //- Std input to get a point with no rubber band
  AcEdJig::DragStatus GetStartPoint();
  //- Std input to get a point with rubber band from point
  AcEdJig::DragStatus GetNextPoint();

private:
  //- Member variables
  //- current input level, increment for each input
  int mCurrentInputLevel;
  //- Entity being jigged
  AcDbPolyline* mpPolyline;
  //- Array of input points, each level corresponds to the mCurrentInputLevel
  AcGePoint3dArray mInputPoints = {};
  CurveInterface* mpSpline;
  bool flagStop;

private:
  bool renderSpline();
};

