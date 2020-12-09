// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__806C127E_BBCC_4295_8C12_384A58F757D3__INCLUDED_)
#define AFX_STDAFX_H__806C127E_BBCC_4295_8C12_384A58F757D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
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
#include <HGDrawManage.h>
#include <HGException.h>
#include <HGXLControl.h>
#include <HGGhostConc.h>
#include <HGBaseConc.h>
#include <HGBaseDraw.h>
#include <HGFem.h>
#include <HGShareWithADManager.h>
#include <HGSql.h>
#include <HGDBControl.h>
#include <HGCommonBridge.h>
#include <HGShareBridgeInfo.h>
#include <RCModule.h>
#include <HGBearingInfo.h>
#include <HgBaseBM.h>
#include <HgExport.h>
#include "versionInfo.h"

#include <HgStmData.h>
#include <HgModel3D.h>
#include <HgStmRenderer.h>
#include <HgStmTemplate.h>
#include <HgStmUi.h>
#include <HgLsd.h>
#include "HGBaseDesign.h"

// INSERT_ATESTCODE
#include "../../../Engine/BaseEng/include/attcmn.h"

// ======= Lock =================
#include "HGLdk.h"
#include "HGLdkDlg.h"
#include "LockCommon.h"
// =============================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__806C127E_BBCC_4295_8C12_384A58F757D3__INCLUDED_)
