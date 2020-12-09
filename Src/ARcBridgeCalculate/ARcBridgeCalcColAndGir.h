// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcColAndGir.h: interface for the CARcBridgeCalcColAndGir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCCOLANDGIR_H__3ADF49F7_11DA_469D_BF2A_10D6182BD484__INCLUDED_)
#define AFX_ARCBRIDGECALCCOLANDGIR_H__3ADF49F7_11DA_469D_BF2A_10D6182BD484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGirderAndColumn;

class AFX_EXT_CLASS CARcBridgeCalcColAndGir  
{
public:
	void CalculateAll();
	CARcBridgeCalcColAndGir();
	virtual ~CARcBridgeCalcColAndGir();

	CARcBridgeCalcStd *m_pStd;

	CTypedPtrArray <CObArray, CGirderAndColumn*> m_pList;
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcJongGir;			// 종단거더단면검토

	void WriteCalcGirder(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bUpperGirder);
	void WriteCalcLowerGirder(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcJongGirder(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcColumn(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);

	BOOL IsExistJongGirder();

	void MakeTableLoadForWallCheck_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteCheckSection(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WriteCheckSection_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri,long &nIndexSmall);
	void WritePMDiagram(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WritePMDiagram_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);

	CConcBasic* GetConcWallForOutWallUsebilityCheck(BOOL bSttWall);

private:
	BOOL IsExistColumn();
	void DrawColumnSection(CXLFormatText *XLText);
	void MakeTableColumnLoad(CXLFormatText *XLText);
	void MakeTableColumnLoadLsd(CXLFormatText *XLText);
	void SetConcData(BOOL bUpperSlabGirder = TRUE);
	void SetConcDataInWall(CConcBasic *pConc, CGirderAndColumn *pGirCol, CWallApp *pWall, ELSDLoadGroupKind eGroupKind=eLSD_COMBINATION_LIMIT);
	void SetConcDataJongGir();
	void SetConcData_BaseInfo(CConcBasic *pConc, BOOL bGirder, BOOL bUpperGirder, BOOL UpAndFront, ELSDLoadGroupKind eGroupKind=eLSD_COMBINATION_LIMIT);
	void DrawGirderSection(CXLFormatText *XLText, long nJijum, BOOL bUpperSlabGirder);
	void DrawJongGirderSection(CXLFormatText *XLText, BOOL bStt, BOOL bLeft, BOOL bGagak);
	CString GetStringCalcLn(long nJijum, BOOL bInnerSpan);
	double GetDistDangerFromColumnCenter(long nJijum);
	double GetMaxAxialForceOnGirder(long nJijum, BOOL bUpper, ETypeEnvComboLsd eEnvType);
	double GetMaxAxialForceOnGirder(long nJijum, double &Shear1, double &Shear2, BOOL bUpper, ETypeEnvComboLsd eEnvType);
	void GetMomentAndAxialOnColumn(long nJijum, long nTypeCheck, double &dAxialReturn, double &dMomentReturn, double &dShearReturn, long &nTypeDesignReturn, long &nComboReturn);
	double GetLengthSpanGirder(long nJijum, BOOL bInnerSpan);
	void MakeDesignList();
	double GetLenSpanJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak);
	double GetMomentJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak);

	void WriteCheckSectionOfJijum_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
};

#endif // !defined(AFX_ARCBRIDGECALCCOLANDGIR_H__3ADF49F7_11DA_469D_BF2A_10D6182BD484__INCLUDED_)
