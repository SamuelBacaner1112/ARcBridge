// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDBStandard.h: interface for the CARcBridgeDBStandard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDBSTANDARD_H__3E685750_581E_401D_BEBF_7FE7CD64328F__INCLUDED_)
#define AFX_ARCBRIDGEDBSTANDARD_H__3E685750_581E_401D_BEBF_7FE7CD64328F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDataStd;
class CARoadOptionStd;
class CRcBridgeApp;
class CRcBridgeData;
class CLineInfoApp;
class CLineInfo;
class CParabola;

class CDimParabola;
class CMarkDomyunSource;
class CDrawWingWall;
class CDrawPlane;
class CDrawCrossSection;
class CDrawVertSection;
class CDrawWall;
class CDrawFooting;
class CDrawExFooting;
class CDrawColumn;
class CDrawApproachSlab;
class CSapData;
class CDrawLine;

class CARcBridgeDrawingOptionData;

#define COMMA2(d)	"("+COMMA(d)+")"
#define COMMA3(d, e)	d==e ? COMMA2(e) : COMMA(d)+COMMA2(e)	

#define MARK_BASEHEIGHT_UPPER	Dom.Always(MARK_BASE_HEIGHT)
#define MARK_BASEHEIGHT_LOWER	Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5)

#include "../ARcBridgeData/ARcBridgeData.h"
static CDoubleArray staticDArr;
class AFX_EXT_CLASS CARcBridgeDBStandard  
{
public:
	// 콘크리트 강도 표기 ///////////
	void SetSelectFckDomText(long nSelectFckDomText);
	long m_nSelectFckDomText;
	CString GetStringTextDimFck(double dFck, double dAggregate, double dSlump);
	///////////////////////////////

	// 블럭 관련
	static CObInsert* InsertHCadBlock(CDomyun *pDomP, const CString& szBlkName, CDPoint xy, double dScale, double dRotDegAng=0, BOOL bHCADOnly=TRUE);
	static CObInsert* InsertHCadBlock(CDomyun *pDomP, const CString& szBlkName, double x, double y, double dScale, double dRotDegAng=0)
	{
		return InsertHCadBlock(pDomP, szBlkName, CDPoint(x,y), dScale, dRotDegAng);	
	}
	static void SetAttrib(CDomyun *pDom, CObInsert *pIns, const CString& szTag, const CString& szText);
	static CDomyun m_HCadSymbolDom;	
	static CStringArray *m_pBlockNameArray;
	static CStringArray *m_pBlockUserNameArray;
	//static BOOL m_bUseHCadBlock;
	static BOOL IsUseHCadBlock();
	static void SetUseHCadBlock(BOOL vUseBlock);
	static CARoadOptionStd* m_pARoadOptionStd;
	static CDomyun* GetHCadSymbolDom();
	static CString m_sMarkForMarkRebar;
	static BOOL m_bViewVectorInfo;
	static BOOL m_bDisplayAngLenStd;	// 사거리 표시 기준 표기
	
	void MarkCenterOfRoad(CDomyun *pDomP, CDPoint xyText, double dLength=0.0, CString strSec=_T(""), BOOL bCenterOfRoad=TRUE);
	// 그리기도구 함수
	void DrawConstJointHor(CDomyun* pDomP, CDPoint xy, double dWidth, double dAng=0);
	void DrawConstJointVer(CDomyun* pDomP, CDPoint xy, double dHeight, BOOL bLeft, double dAng=0);
	void DimRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, CDPoint xyStt, CDPoint xyEnd, BOOL bLeft, long nDimDan, long nDrawLeft=0, BOOL bAliDim=FALSE, BOOL bModifyStt=TRUE, BOOL bCopplingSttEnd=FALSE, BOOL bArcReverse = FALSE);
	void DrawRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bDecreaseSttRebarInFooting=TRUE, CParabola *pPb=NULL);
	void DrawJointArray(CDomyun *pDomP, CJointArray &arrJoint, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
//	void DrawJointByRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC);
	void DrawJoint(CDomyun *pDomP, CRebarJoint &joint, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawCuttingTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine=TRUE);
//	void DrawCuttingTvArrayHorRange(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine);
	void DrawRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bDecreaseSttRebarInFooting=TRUE, CParabola *pPb=NULL);
	void DrawTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bDrawInfo=FALSE, BOOL bDrawDirInfo=FALSE, UINT nTypeDrawOverLapLine=0);
	void DrawLineArray(CDomyun *pDomP, CDPointArray &xyArr);
	double AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDPointArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin=TRUE, BOOL bAli=FALSE, BOOL bInput=FALSE, BOOL bIncludeSttInRepetition=TRUE, BOOL bIncludeEndInRepetition=TRUE, double dApplyAngle=90, BOOL bOrgIsVert=TRUE, CString sMsgStt=_T(""), CParabola *pPb=NULL, double dAccuracy=0);
	double AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDoubleArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin=TRUE, BOOL bInput=FALSE, BOOL bIncludeSttInRepetition=TRUE, BOOL bIncludeEndInRepetition=TRUE, double dApplyAngle=90, BOOL bOrgIsVert=TRUE, CString sMsgStt=_T(""), CParabola *pPb=NULL, double dAccuracy=0);

	void GetDimInfoAtRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bHor, CDPointArray &xyArrDim, CStringArray &sArrInfo, BOOL bMerge, BOOL bApplyJoint, BOOL bApplyEndIs2, BOOL bReverse=FALSE, double dCoverStt=0, double dCoverEnd=0, long nVertDir=0);

	// 맴버 클래스
	CDimParabola*		m_pDimParabola;
	CDrawCrossSection*	m_pDrawCrossSection;
	CDrawVertSection*	m_pDrawVertSection;
	CDrawWall*			m_pDrawWall;
	CDrawFooting*		m_pDrawFooting;
	CDrawExFooting*		m_pDrawExFooting;
	CDrawWingWall*		m_pDrawWingWall;
	CDrawPlane*			m_pDrawPlane;
	CDrawColumn*		m_pDrawColumn;
	CDrawApproachSlab*	m_pDrawAps;
	CDrawLine*			m_pDrawLine;

	// 도면 옵션
	CARcBridgeDrawingOptionData *m_pDrawingOptionData;

	CRcBridgeApp* m_pRcBridge;
	CRcBridgeRebar* m_pRcBridgeRebar;
	CARcBridgeDataStd* m_pARcBridgeDataStd;	
	//CARcBridgeDBStandard(CRcBridgeApp *pRcBridge, CARoadOptionStd *pOptionStd, CARcBridgeDataStd *pARcBridgeDataStd);
	CARcBridgeDBStandard(CRcBridgeRebar *pRcBridge, CARoadOptionStd *pOptionStd, CARcBridgeDataStd *pARcBridgeDataStd);
	virtual ~CARcBridgeDBStandard();

	// 그룹코드 관련
private:
	void GetDimInfoByTypeDim(CDPoint &xyStt, CDPoint &xyEnd, BOOL bHor, long nTypeDim, long nDrawLeft, double dStaCuttingOrg);
	long GetTypeDimRebarInfoCycle(BOOL bHor, CDPoint xyStt, CDPoint xyEnd, long nDrawLeft, double dStaCuttingOrg);
	void SetStd();
	long m_nCurGroupCode;		// 현재 그리는 객체의 그룹코드
	BOOL m_bSettingGroupCode;	// 그룹코드를 적용할지?

public:
	static void SetDisplayAngLenStd(BOOL bDis);
	BOOL IsDivideFrontAndBackWall(long nJ);
	BOOL m_bDrawRebarInfoCycleByHidden;	// 이음만 점선으로 그려야 됨.
	void DrawParabola(CDomyun *pDomP, CParabola *pPb, double dDistStt, double dDistEnd, double dOffset=0);
	void DrawParabola(CDomyun *pDomP, CParabola *pPb);
	BOOL m_bCheckOverLengthRebar;
	void DivideTvArrWithRound(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrHidden, double dLenRoundStt, double dLenRoundEnd);
	void DivideTvArrWithRound(CTwinVectorArrayArray *pTvArr, CTwinVectorArrayArray *pTvArrHidden, double dLenRoundStt, double dLenRoundEnd);
	BOOL IsDrawingLengthInfo();
	void MarkStationJijum(CDomyun *pDomP, long nJijum, CDPoint xyMark);
	void DrawTvArrWidthHidden(CDomyun *pDomP, CTwinVectorArray &tvArr, CLineInfo *pLine);
	BOOL IsViewVectorInfo();
	void MarkRadiusRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, CDRect rectLimit=CDRect(0, 0, 0, 0));
	
	// 도면 그릴때 넘겨받은 도면의 셋팅을 그대로 유지 시킬지? ///////
	// layer설정시 도면 셋팅이 초기화 되므로 엑셀출력시 기껏 설정해놓은 셋팅이 적용되지 않기때문에 
	// 옵션을 둠.
	BOOL m_bApplyDimSettingOfBaseDom;	
	void ApplyDimSetting(CDomyun *pDomPSource, CDomyun *pDomPDest);
	/////////////////////////////////////////////////////////////////
	void GetTvArrTrimByPolygon(CTwinVectorArray &tvArrSource, CTwinVectorArray &tvArrPolygon, BOOL bOutside, CTwinVectorArray &tvArrResult);
	BOOL IsMarkRebar(CString sMark);
	void SetMarkForMarkRebar(CString sMark);
	void DrawTvArrayWidthDashDot(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bHiddenAtStt);
	void DrawTvArrWidthRound(CDomyun *pDomP, CTwinVectorArray &tvArr, double dLenRound, BOOL bRoundAtStt);
	void ModifyInfoAutoDimConc(CStringArray &sArrText, CStringArray &sArrUnderText, BOOL bInput);
	BOOL IsDimByAngleLen();
	CDRect MergeRectAndRect(CDRect rect1, CDRect rect2);
	void GetDimInfoAtDistArray(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CDoubleArray &dArrDist, CString sMark, BOOL bInput, BOOL bMerge, BOOL bHor=TRUE, CDomyun *pDomP=NULL, double dDimLimit=0.0, BOOL bApplySttDistRep=TRUE, BOOL bApplyEndDistRep=TRUE, CDoubleArray &dArrDistText=staticDArr);
	double GetStaCuttingOrg();
	CString GetStringEL(double dEL);
	void GetDimInfoAtPlaceRebarByBlock(CDPointArray &xyArr, CDPointArray &xyArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, CDPoint xyStt, CDPoint xyDir,  BOOL bInput, BOOL bMerge, double dCurAngle=90, double dApplyAngle=90);
	void GetDimInfoAtPlaceRebarByBlock(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, BOOL bInput, BOOL bMerge, double dCurAngle=90, double dApplyAngle=90);
	void DrawPointRebar(CDomyun *pDomP, CDPointArray &xyArr);
	BOOL IsDrawByLineInfo(long nPosInRebarInfoCycle);
	void MarkRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nTypeStt=2, long nTypeEnd=0, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), double dScalePos=0.5, BOOL bInput=TRUE, void *pArrRB=NULL, CParabola *pPb=NULL, BOOL bFindOwner=FALSE);
	void MarkRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nType, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), double dScalePos=0.5, BOOL bInput=TRUE, void *pArrRB=NULL, CParabola *pPb=NULL, CRebarInfoCycle *pRCForMark=NULL);
//	void DrawCuttingTvArraySingle(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rcCut, BOOL bDrawInfo=FALSE);
//	void SettingCurGroupCode(BOOL bVal);
//	void InitCurGroupCode();

	void RotateDomByBridgeSttEndXy(CDomyun *pDomP, BOOL bMoveOrigin=FALSE);
	BOOL IsDrawUpperSlabRebar2DanAll();
};

#endif // !defined(AFX_ARCBRIDGEDBSTANDARD_H__3E685750_581E_401D_BEBF_7FE7CD64328F__INCLUDED_)
