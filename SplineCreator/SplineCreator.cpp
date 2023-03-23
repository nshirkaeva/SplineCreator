// SplineCreator.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "SplineCreator.h"
#include "PolylineJig.h"
#include "Besier/CurveInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void helloNrxCmd()
{
  TCHAR splineMode[ 1 ];
  TCHAR* strMode[] = { L"параболический", L"Безье" };
  if ( acedGetString( NULL, L"\nКакой вид сплайна: параболический или Безье [Q/B] ?:", splineMode ) != RTNORM )
    return;

  acutPrintf(L"\nРендеринг %s сплайна\n", splineMode[ 0 ] == 'Q' ? strMode[ 0 ] : strMode[ 1 ] );

  shared_ptr<CurveInterface> pSpline( nullptr );

  if ( splineMode[ 0 ] == 'Q' )
    pSpline = make_shared<CurveQuadratic>();
  else
    pSpline = make_shared <CurveBesier>();

  AcDbPolyline* pPolyline = new AcDbPolyline();

  auto pPolylineJig = make_shared<PolylineJig>();
  pPolylineJig->startJig( pPolyline, pSpline.get() );
}

void initApp()
{
  ncedRegCmds->addCommand( L"SPLINENRX_GROUP",
    L"_SPLINENRX",
    L"SPLINENRX",
    ACRX_CMD_TRANSPARENT,
    helloNrxCmd );
}

void uninitApp()
{
  ncedRegCmds->removeGroup( L"SPLINENRX_GROUP" );
}

extern "C" __declspec( dllexport ) NcRx::AppRetCode
ncrxEntryPoint( NcRx::AppMsgCode msg, void* pkt )
{
  switch ( msg )
  {
  case NcRx::kInitAppMsg:
    initApp();
    break;
  case NcRx::kUnloadAppMsg:
    uninitApp();
  }
  return NcRx::kRetOK;
}
//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CSplineCreatorApp

BEGIN_MESSAGE_MAP(CSplineCreatorApp, CWinApp)
END_MESSAGE_MAP()


// CSplineCreatorApp construction

CSplineCreatorApp::CSplineCreatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSplineCreatorApp object

CSplineCreatorApp theApp;


// CSplineCreatorApp initialization

BOOL CSplineCreatorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
