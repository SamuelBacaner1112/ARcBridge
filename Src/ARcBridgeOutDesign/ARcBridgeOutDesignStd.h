// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeOutDesignStd.h: interface for the CARcBridgeOutDesignStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEOUTDESIGNSTD_H__8D621CDB_068C_4F4B_926E_F1109E129FB1__INCLUDED_)
#define AFX_ARCBRIDGEOUTDESIGNSTD_H__8D621CDB_068C_4F4B_926E_F1109E129FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_XLOUT_COMPLETE WM_APP+123

#define XL_DESIGNOUT_COVER			0
#define XL_DESIGNOUT_FLOW			1
#define XL_DESIGNOUT_INDEX			2
#define XL_DESIGNOUT_RESULT_SUMMARY 3
#define XL_DESIGNOUT_CONDITION		4
#define XL_DESIGNOUT_EXPOSURERATING	5
#define XL_DESIGNOUT_SECTION		6
#define XL_DESIGNOUT_MODELING		7
#define XL_DESIGNOUT_CALCLOAD		8
#define XL_DESIGNOUT_LOADDIAGRAM	9
#define XL_DESIGNOUT_COMBINE		10
#define XL_DESIGNOUT_SUMMARY		11
#define XL_DESIGNOUT_SECTION_DESIGN	12
#define XL_DESIGNOUT_USEABLE_DESIGN	13
#define XL_DESIGNOUT_FATIGUE		14
#define XL_DESIGNOUT_CORNER		15
#define XL_DESIGNOUT_GIRDER		16
#define XL_DESIGNOUT_ARCHRIB		17
#define XL_DESIGNOUT_FOOTING_SAFE	18
#define XL_DESIGNOUT_FOOTING_DESIGN	19
#define XL_DESIGNOUT_WINGWALL		20
#define XL_DESIGNOUT_APS			21
#define XL_DESIGNOUT_PILE			22
#define XL_DESIGNOUT_SUBSIDENCE		23
#define XL_DESIGNOUT_REACTION		24
#define XL_DESIGNOUT_EXP_JOINT		25
#define XL_DESIGNOUT_CANTILEVER		26
#define XL_DESIGNOUT_STMARC			27
#define XL_DESIGNOUT_STMBRACKET		28
#define XL_DESIGNOUT_STMFOOT		29
#define XL_DESIGNOUT_STMFOOTMID		30

#define XL_ROWSTT_DESIGN	    0
#define XL_ROWEND_DESIGN	65535

class CARcBridgeDataStd;
class CARoadOptionStd;
class COutDesignDlg;
class COutDesignBridge;
class COutBridgeResister;
class COptionItem;
class COutDesignOptionSheet;
class AFX_EXT_CLASS CARcBridgeOutDesignStd  
{
public:
	CARoadOptionStd* m_pARoadOptionStd;
	CARcBridgeDataStd* m_pARcBridgeDataStd;
	CView* m_pView;
	CDocument *m_pDoc;

	CARcBridgeOutDesignStd();
	virtual ~CARcBridgeOutDesignStd();

	COutDesignBridge *m_pOutBridge;
	COutBridgeResister *m_pOutResister;
	//
	COutDesignDlg *m_pDlg;
	COutDesignOptionSheet *m_pOutDesignOption;

	COptionItem *m_pXLOption;
	COptionItem *m_pEtcOption;
	COptionItem *m_pStmOption;

	long m_nCurBridge;
	void SetExcelFormat(CXLControl *pXL, long nRowStt=XL_ROWSTT_DESIGN, long nRowEnd=XL_ROWEND_DESIGN, long nCellHeight=XFL_ROWHEIGHT, long nTextSize=XFL_TEXTSIZE, BOOL bVert=TRUE);	
};

#endif // !defined(AFX_ARCBRIDGEOUTDESIGNSTD_H__8D621CDB_068C_4F4B_926E_F1109E129FB1__INCLUDED_)
