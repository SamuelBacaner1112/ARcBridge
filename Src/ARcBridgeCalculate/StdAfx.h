// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0A7A6D07_F499_4EFB_B8AB_9543D58D805C__INCLUDED_)
#define AFX_STDAFX_H__0A7A6D07_F499_4EFB_B8AB_9543D58D805C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable:4996)

#include <HGXTToolkit.h>
#include <HGCommon.h>
#include <HGDomyun.h>
#include <HGGridCtrl.h>
#include <HGARoadConfig.h>
#include <HGLineApp.h>
#include <HGXLControl.h>
#include <HGGhostConc.h>
#include <HGBaseConc.h>
#include <HGBaseDraw.h>
#include <HGFem.h>
#include <HGBaseDesign.h>
#include <RCModule.h>
#include <HGSql.h>
#include <HGDBControl.h>
#include <HGBearingInfo.h>
#include <HGCommonBridge.h>

#include <HgStmData.h>
#include <HgStmTemplate.h>
#include <HgStmDesign.h>
#include <HgLsd.h>

// MATH1 : CM2
// #include "math1.h"
// #include "math1ops.h"
// MESHTOOLS : CM2
// #include "meshtools.h"
// #include "meshtools1d.h"
// #include "meshtools2d.h"
// MESHERS : CM2
//#include "surf_remesh_t3.h"
//#include "surf_remesh_q4.h"
// #include "triamesh.h"
// #include "tetramesh.h"
#include <cm2_SurfMesh.h>

using namespace cm2;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0A7A6D07_F499_4EFB_B8AB_9543D58D805C__INCLUDED_)
