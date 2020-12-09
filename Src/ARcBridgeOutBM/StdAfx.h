// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F826948F_F669_457B_9735_7239071D42FD__INCLUDED_)
#define AFX_STDAFX_H__F826948F_F669_457B_9735_7239071D42FD__INCLUDED_

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

#include <HGXTToolkit.h>
#include <HGCommon.h>
#include <HGDomyun.h>
#include <HGLineApp.h>
#include <HGGridCtrl.h>
#include <HGARoadConfig.h>
#include <HGBaseConc.h>
#include <HGBaseDraw.h>
#include <HGXLControl.h>
#include <HGFem.h>
#include <RCModule.h>
#include <HGSql.h>
#include <HGDBControl.h>
#include <HgBearingInfo.h>
#include <HGCommonBridge.h>
#include <HgBaseBM.h>

#include <HgStmData.h>
#include <HgLsd.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F826948F_F669_457B_9735_7239071D42FD__INCLUDED_)
