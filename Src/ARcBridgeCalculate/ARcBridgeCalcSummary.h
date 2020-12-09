// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcSummary.h: interface for the CARcBridgeCalcSummary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCSUMMARY_H__EF7F7F18_4C8E_4D24_9F0B_5ADA7754D585__INCLUDED_)
#define AFX_ARCBRIDGECALCSUMMARY_H__EF7F7F18_4C8E_4D24_9F0B_5ADA7754D585__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeCalcSummary  
{
public:
	CARcBridgeCalcSummary();
	virtual ~CARcBridgeCalcSummary();

	CARcBridgeCalcStd *m_pStd;
	void WriteCalcResultSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	CString GetStringMainRebarUse(CConcBasic *pConc, long &nLine);
private:
	void MakeTableCheckFootAndPile(CXLFormatText *XLText);

	void MakeTableCheckFoot_Usd(CXLFormatText *XLText, long nJijum);
	void MakeTableCheckFoot_Lsd(CXLFormatText *XLText, long nJijum);
	void MakeTableCheckPile_Usd(CXLFormatText *XLText, long nJijum);
	void MakeTableCheckPile_Lsd(CXLFormatText *XLText, long nIdxPileCalc);

	void MakeTableCheckFootAndPileBoxType_Usd(CXLFormatText *XLText);
	void MakeTableCheckFootAndPileBoxType_Lsd(CXLFormatText *XLText);
	void MakeTableCheckBeding(CXLFormatText *XLText);
	void MakeTableCheckBeding_Lsd(CXLFormatText *XLText);
	void MakeTableCheckCrack(CXLFormatText *XLText);
	void MakeTableCheckCrack_Lsd(CXLFormatText *XLText);
	void MakeTableCheckSupport(CXLFormatText *XLText);
	void MakeTableCheckSupport_Lsd(CXLFormatText *XLText);
	void MakeTableCheckShear(CXLFormatText *XLText);
	void MakeTableCheckShear_Lsd(CXLFormatText *XLText);
	void MakeTableCheckInSideWall(CXLFormatText *XLText);
	void MakeTableCheckInSideWall_Lsd(CXLFormatText *XLText);

	void DrawSectionMainRebar(CXLFormatText *XLText, long nCycle);
	void DrawWingWallMainRebar(CXLFormatText *XLText, BOOL bStt, BOOL bLeft);

	long GetRowCountAll(long nType=0);
};

#endif // !defined(AFX_ARCBRIDGECALCSUMMARY_H__EF7F7F18_4C8E_4D24_9F0B_5ADA7754D585__INCLUDED_)
