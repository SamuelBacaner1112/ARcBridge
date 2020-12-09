// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingSlabStandardOutput.h: interface for the CDrawingSlabStandardOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSLABSTANDARDOUTPUT_H__FEF48404_6A1D_4D7D_A511_DB103F0E11AD__INCLUDED_)
#define AFX_DRAWINGSLABSTANDARDOUTPUT_H__FEF48404_6A1D_4D7D_A511_DB103F0E11AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDrawingOptionData;

#define TITLE_SLAB_STANDARD_OUTPUT	"RC슬래브교 표준도"

// 슬래브 표준도
class AFX_EXT_CLASS CDrawingSlabStandardOutput : public CDrawPage  
{
public:
	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	CRcBridgeRebar* GetRcBridgeRebar();
	long m_nBridge;

public:
	CDRect DrawRcStd2ObtusSection(CDomyun *pDom, BOOL bStt, BOOL bLeft);
	CDRect DrawRcStd2ObtusPlan(CDomyun *pDom, BOOL bStt, BOOL bLeft, BOOL bUpper);
	CDRect DrawRcStd2Plan(CDomyun *pDom);
	CDRect DrawRcStd1Plan(CDomyun *pDom);
//	void ChangeTextAtDom(CDomyun *pDom, CString sBefore, CString sAfter);
	void DxfIn(CDomyun *pDom, CString sDxfName, double dScaleHor=1, double dScaleVer=1);
	CDRect DrawRcStd1StandardOutLine(CDomyun *pDom);
	void DrawRebarBM(CDomyun *pDomP, CDRect rcDogak);
	CDPoint DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase, CDRect rcDogak);
	void DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, double dMaxYIn=525, double dMinYIn=60);
	void AddPageOne4(CDomyun *pDomP, CDRect rcDogak);
	CDRect DrawDiagramBMD_SFD(CDomyun *pDomP, BOOL bBMD, BOOL bSFD, BOOL bAXIAL, BOOL bWhenDiagramIsNoneDrawRect, BOOL bInput);
	CDRect DrawRcStd3DetailOutLine(CDomyun *pDom);
	CDRect DrawRcStd3Spacer(CDomyun *pDom);
	CDRect DrawRcStd3DesignMethod(CDomyun *pDom);
//	CObInsert* InsertRcStdBlock(CDomyun *pDomP, const CString &szBlkName, CDPoint xy, double dScaleHor, double dScaleVer, double dRotDegAng, BOOL bHCADOnly);
	CDomyun m_domRcStdStandard;
	CDRect DrawRcStd3GuardWallDetail(CDomyun *pDom, long nHDan);
	CDRect DrawRcStd3GirderDetail(CDomyun *pDom, long nJ);
	CDRect DrawRcStd2MainAssm(CDomyun *pDom, long nCycle);
	CDRect DrawRcStd2Standard(CDomyun *pDom);
	CDRect DrawRcStd1Cross(CDomyun *pDom);
	CDRect DrawRcStd1DesignCondition(CDomyun *pDom);
	void AddPageOne1(CDomyun *pDomP, CDRect rcDogak);
	void AddPageOne2(CDomyun *pDomP, CDRect rcDogak);
	void AddPageOne3(CDomyun *pDomP, CDRect rcDogak);

	void MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar);

	long GetNumberBridge(long nCategoryPageNum);
	CDrawingSlabStandardOutput();
	virtual ~CDrawingSlabStandardOutput();

};

#endif // !defined(AFX_DRAWINGSLABSTANDARDOUTPUT_H__FEF48404_6A1D_4D7D_A511_DB103F0E11AD__INCLUDED_)
