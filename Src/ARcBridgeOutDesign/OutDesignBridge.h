// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutDesignBridge.h: interface for the COutDesignBridge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_)
#define AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Src/ARcBridgeData/ARcBridgeData.h"

class CRcBridgeRebar;
class CARcBridgeCalcStd;
class COutDesignBridge  
{
public:
	void MakeExcel(CXLControl *pXL, long nBri);
	COutDesignBridge();
	virtual ~COutDesignBridge();

	long m_nRowSheet;
	long m_nIndexTitle;

	CARcBridgeOutDesignStd *m_pStd;
	CARcBridgeCalcStd	*m_pCalc;
	
public:
	void MakeExcelTitle(CXLControl *pXL, CRcBridgeRebar *pBri);					//표지
	void MakeExcelCalcFlow(CXLControl *pXL, CRcBridgeRebar *pBri);				//설계흐름도
	void MakeExcelCalcIndex(CXLControl *pXL, CRcBridgeRebar *pBri);				//목차
	void MakeExcelCondition(CXLControl *pXL, CRcBridgeRebar *pBri);				//설계조건
	void MakeExcelLsdExposure(CXLControl *pXL, CRcBridgeRebar *pBri);				//노출등급
	void MakeExcelSection(CXLControl *pXL, CRcBridgeRebar *pBri);				//단면가정
	void MakeExcelCalcLoad(CXLControl *pXL, CRcBridgeRebar *pBri);				//하중산정
	void MakeExcelCalcLoad_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);				//하중산정
	void MakeExcelLoadDiagram(CXLControl *pXL, CRcBridgeRebar *pBri);			//하중재하
	void MakeExcelModeling(CXLControl *pXL, CRcBridgeRebar *pBri);				//모델링해석
	void MakeExcelCombine(CXLControl *pXL, CRcBridgeRebar *pBri);				//하중조합
	void MakeExcelForceSummarySheet(CXLControl *pXL, CRcBridgeRebar *pBri);		//단면력 집계
	void MakeExcelForceSummarySheet_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);		//단면력 집계
	void MakeExcelSectionDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//단면검토
	void MakeExcelUseableDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//사용성검토
	void MakeExcelSluggishAndFatigue(CXLControl *pXL, CRcBridgeRebar *pBri);	//처짐 및 피로검토
	void MakeExcelCornerAndSupportBar(CXLControl *pXL, CRcBridgeRebar *pBri);	//우각부 및 배력철근 검토
	void MakeExcelColumnAndGirder(CXLControl *pXL, CRcBridgeRebar *pBri);		//보,기둥 설계
	void MakeExcelArchRib(CXLControl *pXL, CRcBridgeRebar *pBri);				//Arch 리브 검토
	void MakeExcelFootingSafeCheck(CXLControl *pXL, CRcBridgeRebar *pBri);		//기초안정검토
	void MakeExcelFootingDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//기초설계
	void MakeExcelWingWall(CXLControl *pXL, CRcBridgeRebar *pBri);				//기초설계
	void MakeExcelApproachSlab(CXLControl *pXL, CRcBridgeRebar *pBri);			//접속슬래브설계
	void MakeExcelExpJoint(CXLControl *pXL, CRcBridgeRebar *pBri);				//신축이음 검토
	void MakeExcelCantilever(CXLControl *pXL, CRcBridgeRebar *pBri, HGBOOL bLeft);	// 캔틸레버 검토
	void MakeExcelPile(CXLControl *pXL, CRcBridgeRebar *pBri, long nIndex);		//말뚝검토
//	void MakeExcelSubsidence(CXLControl *pXL, CRcBridgeRebar *pBri);			//부등침하검토
	void MakeExcelCalcResultSummary(CXLControl *pXL, CRcBridgeRebar *pBri);		//해석결과 요약
	void MakeExcelCalcShoeReaction(CXLControl *pXL, CRcBridgeRebar *pBri);		//교량받침 반력산정
	void MakeExcelSTM(CXLControl *pXL, CRcBridgeRebar *pBri, HGINT32 nItem, HGINT32 iKey, HGINT32 nIndexMiddle);			//STM

	void MakeExcelCombine_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);				//하중조합

	void WriteExcelEarthQuakeLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long &nIndexMiddle);
	void MakeExcelSTM_Condition(CXLFormatText *pXLText, CRcBridgeRebar *pBri, CStmModel*pModel, long nColPos, long nIndexTitle, HGINT32 nItem, HGINT32 iKey);

	//우각부
	void MakeExcelSTM_ArcCorner_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_ArcCorner_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	//STM 우각부 단면을 가져오기 위한 함수
	void GetTvArrSTM_Corner_Section(CTwinVectorArray &tvArrCorner, CTwinVectorArray &tvArrDimTopBot , CTwinVectorArray &tvArrDimLeftRight, CRcBridgeRebar *pBri, HGINT32 nPos );

	//브래킷
	void MakeExcelSTM_Bracket_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_Bracket_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);

	//기초 StartEnd
	void MakeExcelSTM_Footing_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_Footing_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
//	void MakeExcelSTM_Footing_Combine(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexTitle, long nIndexMiddle);

	//기초 Mid
	void MakeExcelSTM_FootingMid_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
//	void MakeExcelSTM_FootingMid_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexTitle, long nIndexMiddle, HGINT32 iKey);

private:
	void MakeTableBoringData(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeDiagramBMD_SFD(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeDiagramBMD_SFD_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableMainRebarSummaryCrack(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableMainRebarSummaryCrackLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	// 0: 사용하중, 1: 계수하중, 2: 계수하중(최대편심), 3: 지진하중
	void MakeTableLoadComBo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType, BOOL bIncreaseCombo=FALSE);
	void MakeTableMainRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableShearRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableShearRebarSummaryLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	//
	void MakeExcelCalcShoeReaction_2DFrame(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeReaction_3DPlate(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeReaction_Apply(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeHoriForce(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableSectionForce(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableSectionForceEQ(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableSectionForceEQ_BOX(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bMoment, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableLoadForFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	CString GetStringSectionForceTableAtWall(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd, BOOL b3D, BOOL b3DLoad);
	CString GetStringSectionForceTableAtWallEQ(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd);
	CString GetStringSectionForceTableAtWallEQ_BOX(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, BOOL bMoment, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd);

	void GetKeySttEnd(HGINT32 nItem, HGINT32 &iKeyStt, HGINT32 &iKeyEnd);

	void MakeExcelConditionUseMaterial(CXLFormatText *pXLText, long &nIndexSmall, long &nIndexTiny, long &nTypeNumber, long nT_1, long nT_2);
	void MakeExcelConditionUseMaterialLsd(CXLFormatText *pXLText, CRcBridgeRebar *pBri);
};

#endif // !defined(AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_)