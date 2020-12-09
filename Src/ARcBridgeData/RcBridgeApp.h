// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RcBridgeApp.h: interface for the CRcBridgeApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBRIDGEAPP_H__D04D68DD_141E_4EB0_AF5B_E12CFC6882D9__INCLUDED_)
#define AFX_RCBRIDGEAPP_H__D04D68DD_141E_4EB0_AF5B_E12CFC6882D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000,

#include "RcBridgeData.h"

#define SOLVE_SAP_6AND7	0
#define SOLVE_SAP_8		1
#define SOLVE_SAP_9		2
#define SOLVE_SAP_10	3
#define SOLVE_MIDAS		4
#define SOLVE_NEOMAX	5
#define SOLVE_SAP6AND7_STRING	"SAP2000 6~7"
#define SOLVE_SAP8_STRING	"SAP2000 8"
#define SOLVE_SAP9_STRING	"SAP2000 9"
#define SOLVE_SAP10_STRING	"SAP2000 10"
#define SOLVE_MIDAS_STRING	"MIDAS"
#define SOLVE_NEOMAX_STRING	"NEOMAX"

#define DIST_MIN_CROSS_CENTER	1.0
/*
enum TARGET_IN_ONE_SLAB{
	OFF_STT_STT, OFF_STT_SLAB, OFF_STT_SHOE,
	OFF_END_END, OFF_END_SLAB, OFF_END_SHOE,
	OFF_STT_SLABCOVER, OFF_END_SLABCOVER};	// SLABCOVER 슬래브에서 피복 적용된 거리
	*/
static double static_double;

class CLineInfoApp;
static CDPoint staticCDPointRC;
class AFX_EXT_CLASS CRcBridgeApp : public CRcBridgeData  
{
public:
	long GetCountVariableHDanLeft(BOOL bRoadNotSlab);
	BOOL IsExp(double dSta);
	void ModifyVertSectionForWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
//	double	GetHeightDosang(double sta, CDPoint vAng, long nHDan);
//	double	GetHeightDosangAvg(double sta, CDPoint vAng, long nHDan);
	void SetDataDefaultHDanSlab();	// 슬래브 횡단
	void SetDataDefaultHDanRoad();	// 도로횡단, 철도횡단
	void SetDataDefaultHDanNori();	// 성토사면
	void SetDataDefaultHDanPave();	// 포장제원
	void SetDataDefaultHDanLive();	// 상재하중
	void SetDataDefaultHDanRailLive();	// 철도 상재하중
	void SetDataDefaultHDanRail();	// 레일위치
	void SetDataDefaultHDanDistribute();	// 레일위치
	void SetDataDefaultHDanUnderMember();
	void SetDataInitPaveDetailWidth();	// 지중라멘교 세부포장폭 초기화
	void SetDataDefaultEarthAnchor(CFootingApp *pFooting); // 어스앵커 권고안
	void SetDataDefaultPilePos(CFootingApp *pFooting); // 말뚝배치 권고안
	void SetXyPilePos(CFootingApp *pFooting); // 말뚝위치 설정
	CDPoint GetXyPilePos(CFootingApp *pFooting, long nIndexOrg, long nIndexSide);
	void SetDataDefaultFootingModeling(); // 기초모델링 권고안

	/// 구조계산관련 함수
	double GetThickWall(long nJijum, BOOL bAngleApplay=FALSE);
	double GetWidthHunch(long nJijum, BOOL bSttPos, BOOL bUpper=TRUE);
	double GetAnglHunchWithSlabOrWall(long nJijum, BOOL bSttPos, BOOL bSlab, BOOL bUpper=TRUE);
	BOOL IsRigidEndLeftHunch(long nJijum, BOOL bUpper=TRUE);
	double GetDistRigidEndWall(long nJijum, BOOL bUpper=TRUE);
	double GetDistRigidEndSlab(long nJijum, BOOL bSttPos, BOOL bUpperSlab = TRUE);
	double GetLengthModelSpan(long nSpan);
	double GetHeightModelJijum(long nJijum);
	double GetOutValueShearSlabOnJijum(long nJijum,  long nPos, long nTypeDesign, BOOL bUpperSlab=TRUE, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValueSlab(long nSpan,  long nPos, long nTypeDesign, long nTypeForce, BOOL bOutSide=TRUE, BOOL bGirderCalc=FALSE, BOOL bUpperSlab=TRUE, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit, double dDist=0);
	double GetOutValueWall(long nJijum, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit, double dDist=0);
	double GetOutValuePRF(BOOL bStt, long nTypeDesign, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValueSlabEQ(long nSpan,  long nPos, long nTypeForce, BOOL bUpperSlab=TRUE, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit, double dDist=0);
	double GetOutValueWallEQ(long nJijum, long nPos, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit, double dDist=0);
	double GetOutValuePRFEQ(BOOL bStt, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetMomentCorner(BOOL bSttWall, BOOL bUse, BOOL bOutSide, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	long   GetOutValueForFootingDesign(long nJijum, long nTypeCheck, double &dMoment, double &dShear, double &dAxial, BOOL bUseForce, long nJijum_Add=-1, long nJijumReal=-1, BOOL bNoneImpectCase=FALSE);
	long GetOutValueForFootingDesign_Lsd(long nJijum, ETypeEnvComboLsd eEnvTypeLsd, EFootingCheckLsd eCheck, double &dMoment, double &dShear, double &dAxial, long &nIdxModel, long nJijum_Add=-1, long nJijumReal=-1);
	long GetOutValueForFootingLoadCase_Lsd(EFootingCheckLsd eCheck, BOOL bMinFind, long nLoadCaseMax, long nLoadCaseMin, CJoint *pJoint, double &dMoment, double &dShear, double &dAxial);
	long GetOutValueForWallDesing_Lsd(long nJijum, ETypeEnvComboLsd eEnvTypeLsd, EWallCheckLsd eCheck, double &dMoment, double &dShear, double &dAxial, long &nIdxModel, BOOL bApplyCtc=FALSE);
	long   GetOutValueForPileDesign_Usd(long nTypeCombo, long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR);
	long   GetOutValueForPileDesign_Lsd(long nTypeCombo, long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR);
	long   GetOutValueForPileDesign(CFEMManage *pFEM, CString szCombo,long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR);
//	long GetOutValueSlabChange(long nJigan, double &Mmax, double &Mmin, double &Md);
	double GetCalcLenRHunch(long nJijum, BOOL bSttPos, BOOL bInside=TRUE);
	long GetNumberElementArchRibSpanSttCheck(long nSpan, long nTypeDesign, BOOL bCrown=FALSE);
	long GetNumberElementArchRibSpanEndCheck(long nSpan, long nTypeDesign, BOOL bCrown=FALSE);
	double GetThickArchRibForCalc(long nPos, BOOL bReal);			// CHECK_POS_LEFT, CHECK_POS_CENTER, CHECK_POS_RIGHT
	//
	void SetWingWallShellOutput(BOOL bStt, BOOL bLeft);
	double GetOutValueWingWallShell(long nIdxArea, BOOL bStt, BOOL bLeft, BOOL bMoment, BOOL bUse, BOOL bJudge=FALSE);
	double GetOutValueSlabShell(long nSpan,  long nPos, long nTypeDesign, long nTypeForce, BOOL bUpperSlab = TRUE, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValueWallShell(long nJijum, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValueWallShellColumn(long nJijum, long nColumn, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValuePRFShell(BOOL bStt, long nTypeDesign, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	double GetOutValueArchRibShell(long nSpan,  long nPos, long nTypeDesign, long nTypeForce);
	long GetOutValueSlabMaxForFatigue(long nJigan, double &Mmax, double &Mmin);
	void GetOutValueSlabMaxForFatigue_Lsd(long nJigan, long nPos, double &Mmax, double &Mmin);
	//////////////////////////////////////////////////////////////////////////
	void SetDataDefaultShapeStd(long nLeft=0, BOOL bSyncAps=TRUE);
	void SetDataDefaultShapeHunch(long nIdxHunch);
	CLineInfo* GetLineByNumber(long nLine);
	CLineInfo* GetLineBySlab(long nLeft);

	CLineInfo* GetLineBase(BOOL bLineBaseVert=FALSE, BOOL bForPlacingMainRebar=FALSE);
	CLineInfo* GetLineLeftRight(BOOL bLeft);//TRUE=좌측선형, FALSE=우측선형
	CLineInfo* GetLineLeftCenRight(long nLeft,BOOL bLineBaseVert=FALSE, BOOL bForPlacingMainRebar=FALSE);//-1=좌측선형, 0=기준선형, 1=우측선형
	BOOL IsExtLine();
	CString GetStringBridgeType();
	BOOL IsCalc3D(long nTypeLoad);
//	CString GetStringSelect2DFrameTool();
	BOOL IsExpRound(BOOL bStt, BOOL bLeft);

	ETypeEnvComboLsd GetTypeEnvComboLsdFooting(HGINT32 ixKey, long nJijum);

public:
	void GetLenJijumForDim(CDoubleArray &dArr);
	void GetTvVertSectionBracket(CTwinVectorArray &tvArr, BOOL bStt);
	double GetLengthBracket(BOOL bStt);
	double GetLengthBFooting(BOOL bStt);
	CDPoint GetAngleCutGagakEnd(BOOL bStt, BOOL bLeft);
	
	long GetGuardWallTotalLength(CGuardWallRC *pGW, long nPos=2);
	double GetLengthBetweenWall(long nJigan, BOOL bVertDir);
	void ModifyPlaneForGagakRebarMain(CTwinVectorArray &tvArr);
	BOOL IsRightFromRoadCenter();
	void SyncAngleToFootingAngle();
	double GetMaxHeightCentralWall();
	BOOL IsEnableDisplayVertLengthOfJigan();
	BOOL IsSameAngleSttAndEnd();
	void GetTvVertSectionFxJoint(BOOL bStt, BOOL bVertDir);
	BOOL IsCutEndOfExp(BOOL bStt, BOOL bLeft);
	double GetCoverSide_UpperSlab(long nDan, double dSta=0, CDPoint vAng=CDPoint(0, 1), BOOL bApplyAngle=FALSE, BOOL bLeft=TRUE);
	void GetTvSection_Front_ArchRib(CTwinVectorArray &tvArr, BOOL bStt);
	void GetTvPlane_Section_ArchRib(CTwinVectorArray &tvArr, BOOL bStt);
	void GetTvSection_ArchRib(CTwinVectorArray &tvArr, CDPoint xyCen, double dThick, long nDrawLeft);
	void GetTvCrossSection_ArchRib(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrPattern, BOOL bUpper, long nDrawLeft=0);
	void TrimRibByConnectUpperSlab();
	double GetThickArchRib(double dXDisOfUpper, BOOL bCenter=FALSE, BOOL bVerticalSection=FALSE);
	void GetTvCrossSection_ArchRib(CTwinVectorArray &tvArr, double dXDisOfUpper, long nDrawLeft, BOOL bVerticalSection=FALSE, double dHeightBlock=0);
	double GetCoverSideArchRib(long nDan);
	double GetWidthArchRib(long nLeft);
	void GetTvPlaneArchRib(CTwinVectorArray &tvArrPlane, BOOL bUpper, long nDrawLeft);
	BOOL IsGagakForLine(BOOL bStt, BOOL bLeft);
	long GetJijumConnectRibAndSlab();
	double	GetPolygonArea(CDPointArray vArr, long nNum);		// Area of Polygon
	CDPoint GetCentroid(CDPointArray vP, long nNum);			// Centroid of Polygon 
	BOOL IsIncludeOnlyLineInBridge();
	double GetDistCrossSectionAndInWall(long nIdxWall, BOOL bApplyAngle, BOOL bLeft);
	void GetTvRebarOut_Plane_UpperSlabStd(CTwinVectorArray &tvArr, long nDan, BOOL bExp=FALSE, long nLeft=2, BOOL bVert=TRUE, BOOL bCutEnd=TRUE, BOOL bForExpRebar=FALSE);
	void SettingPlane();
	void GetTvPlaneOfExp(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nDan=-1);
	BOOL GetTvGirderLowerJong(CTwinVectorArray &tvArrLower, BOOL bStt, BOOL bLeft, BOOL bGagak);
	double GetStationForInputJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak);
	void GetTvRibBlock();
	void TrimRibByFootingUpper();
	void SetDataDefaultShapeRib();
	void SetDataDefaultPRF();
	CDPoint GetXySlabUpperByOffsetFrStation(double dSta, double dOffset, long nLeft);
	CDPoint GetXyLowerSlabBottomByOffsetFrStation(double dSta, double dOffset, long nLeft);
//	double GetStationCenterByCenterStationForLR(double dSta, long nLeft);
	double GetAngleJijumByStaToSta(double dSta1, double dSta2, double dDistOrg, long nLeft);
	CDPoint GetXyMatchLineLRByCenterStation(double dSta, long nLeft);
	double GetLengthStaToSta(double dSta1, double dSta2, long nLeft);
	double GetLengthSlabReal(long nLeft);	/**< 슬래브의 실길이 */
	double GetStationByCenterStation(double dSta, long nLeft);
	double GetHeightUpperSlab(long nJ, long nLeft, CDPoint vAng);
	double GetHeightLowerSlab(long nJ, long nLeft, CDPoint vAng);
	void SyncBridge_DesignData();
	void ResetFlagForRecoverVertSection();
	void GetTvVertSection_Std(long nLeft, BOOL bApplyAngle, BOOL bWingFront, BOOL bRecovery=FALSE, long nTypeJongdan=-1, BOOL bForBMOfFooting=FALSE, BOOL bNoneRecover=FALSE);
	void SyncBridge_CrossSection_SttEndFooting(BOOL bApplyAngle);
	void SyncWall_ColumnPlanJewon(CWallApp *pWall);
	void SyncWall_JongdanLimitFront(CWallApp *pWall, BOOL bApplyAngle);
	void SyncWall_DesignData(CWallApp *pWall);
	void SyncWall_HunchLength(long nJijum, CWallApp *pWall, BOOL bApplyAngle);
	void GetXyHunchLowerSide(long nIdxHunch, long nLeft, BOOL bVertDir, CDPoint &xyHunchLowerL, CDPoint &xyHunchLowerR, BOOL bUpper=TRUE);
	CDPoint GetVAngHunchLower(long nIdxHunch, long nLeft, BOOL bVertDir);
	double GetAngleJongdan(double dSta);
	void ApplyRoundOfInWallHunch(long nIdxInWall, BOOL bLeft);
	BOOL IsValidStationOfBri();
	void SetSearchIP();
//	void SetDataDefaultPileN(long nJ);
	double GetDiffELSttAndJijumForVertSection(long nJ, BOOL bInner, BOOL bApplyByJongdanSlope=FALSE, BOOL bInWallLR=FALSE);
//	double GetDiffELSttAndJijumForVertSectionBySta(double dSta, BOOL bApplyByJongdanSlope=FALSE);
//	BOOL IsVariableLenHdanAuto(long nHDan, BOOL bRoadNotSlab);// nHDan이 가변구간인지를 점검
	long GetNumHdanVariableLenAutoLeft(BOOL bRoadNotSlab);// 도로중심 좌측 슬래브 중에서 가변구간 번호(nHDan)를 구함
	long GetNumHdanVariableLenAutoRigh(BOOL bRoadNotSlab);// 도로중심 우측 슬래브 중에서 가변구간 번호(nHDan)를 구함
	BOOL IsShoeReaction2DApply(long nJijum, long nShoe);
	double GetShoeHoriForce(long nJijum, long nShoe);
	double GetShoeReactionLsd(long nJijum, long nShoe, long nTypeLoad, BOOL b2DFrame, BOOL bUnitMeter);
	double GetShoeReactionUsd(long nJijum, long nShoe, long nTypeLoad, BOOL b2DFrame, BOOL bUnitMeter);
	double GetShoeReaction(long nJijum, long nShoe, long ixLoadCase, BOOL b2DFrame, BOOL bUnitMeter);
	double GetLengthUgan(long nJ, long nStt);
//	double GetStationSttEndByRealSlabZz(BOOL bStt);
	void GetTvSidePlaneOfSlab(CTwinVectorArray &tvArr, BOOL bLeft, BOOL bExp);
	double GetStationSttEndByRealSlab(BOOL bStt);
	double GetWidthDiffSlabActOrgAndGagak(BOOL bStt, BOOL bLeft, BOOL bInner);
	void GetTvCrossSection_OutWall(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	void GetTvCrossSection_OutWall_Exp(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeftWall, BOOL bInner, BOOL bLeft);
	void GetTvSttEndOfSlab(CTwinVector &tvStt, CTwinVector &tvEnd, BOOL bExp=TRUE);
	void SetArrangeCombine(long nTypeCombo);
	CDPoint GetXyDirExpForRebar(BOOL bStt, BOOL bLeft);
	double GetGirderHeight(long nJijum, BOOL bUpperSlab);
	double GetDistColumnNext(long nJijum, long nColumn, BOOL bLeft);
	double GetStationSmallestWidthOfUpperSlab(BOOL bMax=FALSE, long nLeft=0, BOOL bSlabEnd=TRUE);
	long GetShoePosByStation(double dStation);
	void MakeWallByOutWallOnlyShape(CWallApp &wall, BOOL bStt, BOOL bApplyAngle, BOOL bDefaultSupAndShear=FALSE, double dStation=dSta_Check_NotUse_Minus);
	double GetDistMidOfSlab(double dSta, BOOL bVert=TRUE, BOOL bBaseWidth=TRUE);
	BOOL IsBracket(BOOL bStt);
	void SetDatadefaultShapeInWallHunch(long nIdxInWall);
	void GetTvVertSectionPier(CTwinVectorArray &tvArr, long nJ);
	CBoringData* GetBoringDataJijumBase(long nJijum, BOOL bPilePos=FALSE);
	void SetDataDefaultPileLength(long nJ);
	void GetTvPostWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nTypeBaseLine);
	CTwinVector GetTvBackFillHorLine(BOOL bStt, long nType);
	void GetTvElEarthLine(CTwinVectorArray &tvArr, double dStationStt, double dStationEnd, BOOL bBridgeZone=FALSE);
	void GetTvBackFillDeductOfWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nForUse=0);
	void GetTvReFillDeductOfWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	CTwinVector GetTvBackFill(BOOL bStt, long nForUse=0, BOOL bBack=FALSE);
	void GetTvSungtoLine(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, BOOL bBoho);
	void GetTvTogongLine(CTwinVectorArray &tvArr, CFootingApp *pFooting, CBoringDataEx *pExBoring, BOOL bIncludeUpper=FALSE, BOOL bCrossSection=FALSE, BOOL bMergeLower=FALSE, BOOL bForDraw=FALSE, long nDeductLeft=0, long nJijum = -1);
	void GetTvPlaneInWallHunch(long nIdxInWall, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, BOOL bUpperSlab=TRUE);
	CDPoint GetXyVertSectionByDirVert(CDPoint xy, long nJijum);
	void GetTvCenterPlane_UpperSlab(CTwinVectorArray &tvArr, BOOL bIncludeOutWall, BOOL bIncludeBracket, double dLenExt, BOOL bVert);
	double GetStationBridgeStt(BOOL bLineBaseVert);
	double GetLengthBridge(BOOL bLineBaseVert);
	void GetTvRebarOut_Plane_UpperSlab(CTwinVectorArray &tvArr, long nDan, BOOL bExp=FALSE, long nLeft=2, BOOL bVert=TRUE, BOOL bCutEnd=TRUE);
	void MakeLineBaseVert(BOOL bVert=TRUE);
//	void GetTvPlaneSlabByPart(CTwinVectorArray &tvArr, long nIdxPart);
	void SetCombineEarthQuakeDWORD();
	void SetCombineNormalDWORD();
	long GetJijumWallByStation(double dSta);
	long GetJijumNumberByStation(double dSta);
	double GetStationRibBySpan(long nSpan, long nPos, BOOL bCrown=FALSE);
	void GetTvArrVertSectionOutLine(CTwinVectorArray &tvArr);

	void GetOffsetTvRebarOut_Plane_UpperSlab(CTwinVectorArray &tvArrPlan, double dCover, double dExpSttLeftR, double dExpSttRightR, double dExpEndLeftR, double dExpEndRightR);
	BOOL IsConnect(CTwinVectorArray &tvArr);

	void GetTvPlaneOutWall(CTwinVectorArray &tvArr, BOOL bStt, BOOL bIncludeBracket, BOOL bIncludeOutWallOutter, BOOL bIncludePRF=FALSE);
//	long GetIdxInWallOfFirstColumnType();
	CBoringDataEx* GetBoringDataExByJijumNumber(long nJijum);
	CBoringDataEx* GetBoringDataExByStation(double dStation);
	long GetBoringIndexByStation(double dStation);

	long GetCountSubsidence();
	long	GetSizeCombo(long nTypeDesign, BOOL bRealSize);
	long GetBaseIndexCombo(long nTypeDesign, long IndexComboReal);
	DWORD GetLoadCombo(long nCombo, long nTypeCombo);
	void SetCalcVertLoadFactor();
	void SetDataDefaultCombineUseLoad();
	void SetDataDefaultCombineUltLoad();
	void SetDataDefaultCombineEccLoad();
	void SetDataDefaultCombineEQLoad();
	void SetDataDefaultSelectLoad();
	void SetDataDefaultCover();
//	void SetDataDefaultRebarSymbol();
	void SetDataDefaultBM(long nType); //nType, 0:거푸집 1:나머지
	
	void SetDataDefaultCondition();
	void GetTvScaffoldWingPlane(CTwinVectorArray &tvArrOut, CTwinVectorArray &tvArrIn, CTwinVectorArray &tvArrCenOut, CTwinVectorArray &tvArrCenIn, CTwinVectorArray &tvArrSideOut, CTwinVectorArray &tvArrSideIn, BOOL bStt, BOOL bLeft);
	void GetTvScaffoldSection(CTwinVectorArray &tvArr, BOOL bStt);
	void GetTvScaffoldPlane(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrCen, BOOL bStt);
	BOOL GetTvPostJigan(CTwinVectorArray &tvArrUpper, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, CTwinVectorArray &tvArrMid, CTwinVectorArray &tvArrArch,  CTwinVectorArray &tvArrMid2, long nJigan, CDPoint &LeftEL = CDPoint(0,0), CDPoint &RightEL = CDPoint(0,0));
	void GetRibPost(CTwinVectorArray &tvArrMid);	// 아치교에서 리브 아래부분 동바리 영역
	void GetArchJiganPost(CTwinVectorArray &tvArrMid, long nJigan);		// 아치교에서 지간사이 동바리 영역
	void GetTvPostBracket(CTwinVectorArray &tvArr, BOOL bStt);
	void GetTvPave_VertSection(CTwinVectorArray &tvArr);
	void GetTvPRF_VertSection(CTwinVectorArray &tvArr, BOOL bStt, long nLeft, BOOL bVertDir, BOOL bApplyAngle = TRUE, BOOL bAddTvArr = FALSE); //bAddTvArr : m_tvArrVertSection 에 추가할건지 안할건지.
	void SyncWall(long nJijum, CWallApp *pWall, BOOL bApplyAngle, double dBaseStation=dSta_Check_NotUse_Minus);
	double GetMidCrossSection(BOOL bVert=FALSE);
	CDPoint GetXyDirRotateDomByBridgeSttEndXy();
	void GetTvPlaneSttEndLine(CTwinVectorArray &tvArr, BOOL bStt, BOOL bSlab, double dDistMove=0.0, BOOL bExp=TRUE);
// 	double GetHeightUpperSlab_OldOrNotUse(double dSta, CDPoint vAng=CDPoint(0, 1));
	CDPoint GetAngleJijum(long nJ, BOOL bVertDir=FALSE);
	BOOL IsApplyAngle();
	void SetDataDefaultShapeCns(BOOL bStt, BOOL bLeft, long nIdx, double dFck, double dFy);
	BOOL GetTvHunchLower(CTwinVector &tvLower, long nJ, BOOL bLeft, BOOL bUpperSlabGirder = TRUE);
	long GetJijumNumberSlabEnd(long nSlab);
	long GetJijumNumberSlabStt(long nSlab);
//	long GetQtySlab();
	void GetIndexNearestFixShoe(CDPoint xyShoe ,long &nJijumFix, long &nLeftFix, long &nRowFix, long FromJijum, long ToJijum);
	BOOL IsExistFixedJijum(long nSlab);
	BOOL IsFixedJijum(long nJijum, BOOL bSttShoe);
	BOOL IsOutsideWall();
	void SetDataDefaultShapeAps(BOOL bStt, BOOL bLeft, BOOL bAscon);
	CDPoint GetAngleAzimuthTanOnJijum(long nJ, int nLeft);
	BOOL SetDataDefaultShapeFooting(CFootingApp *pFooting, double dWidthWall, double dSta, long nIdxInWall=-1);
	BOOL IsBriInLine();
	void SyncBridge(long nLeft=0, BOOL bApplyAngle=FALSE, BOOL bSyncAps=TRUE, BOOL bApplyAngleAps=FALSE, BOOL bForBMOfFooting=FALSE);
	void SyncJijumStation();
	void SetDataDefaultShapeTopInWall(long nIdxWall);
	//hgh 작업 20040505
	void	GetXySss(TARGET_IN_ONE_SLAB nTarget, CDPoint* A, BOOL bExp=TRUE);
	void	GetPointArraySlabLowerAct(double sta, CDPoint vAng, CDPointArray& xyArr, BOOL bExp=FALSE);
	void	GetPointArraySlabHunchAct(double sta, CDPoint vAng, CDPointArray& xyArr, BOOL bExp=FALSE);
	double	GetElMaxSlabUpper(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double	GetElMinSlabUpper(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	void	GetXySlabUpperAct(double sta, CDPoint vAng, CDPoint A[30], BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	void	GetXySlabUpperAct90(double sta, CDPoint A[30], BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	void	GetXySlabUpperActAngle(double sta, CDPoint vAng, CDPoint A[30], BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	void	GetXyDosangBase(double sta, CDPoint vAng, long nHDan, CDPoint A[6], BOOL bRoadNotSlab=FALSE);
	CDPoint	GetMatchPavePoint(double staCR, CDPoint xy, double slope, BOOL bRoadNotSlab=FALSE);
	void	GetXyDosang(double staCR, CDPoint vAng, long nHDan, CDPoint A[6], BOOL bRoadNotSlab=FALSE);
	double	GetLengthRailByLineByCenLine(double sta, CDPoint vAng, long nRail);
	BOOL	IsMatchDosangByDis(double sta, CDPoint vAng, double disFrCenter, CDPoint &xyResult);
	long	GetQtyHDanAct(double staCR, CDPoint vAng, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	long	GetNumSlabNodeAct(double staCR, CDPoint vAng, long nHDan, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	//하부슬래브


	BOOL	IsLevelHDanApplyLineSlope(long nHDan, BOOL bRoadNotSlab=FALSE) const;								//각 횡단 횡단 레벨 여부
	double	GetSlopeHDanByStation(double sta, long nHDan, BOOL bRoadNotSlab=FALSE, long nHDanCheckLevel=-1); 
	long	GetNumHDanByDist(double sta, CDPoint vAng, double dist, BOOL bReverse=FALSE);
	double 	GetDisSlabFrCenToHDanLeft(double sta, long nHDan, BOOL bRoadNotSlab=FALSE);
	double 	GetDisSlabLeftAct(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double 	GetDisSlabLeftActByGagak(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double	GetDisSlabRightAct(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double 	GetDisSlabRighActByGagak(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double 	GetWidthSlabActByGagak(double sta, CDPoint vAng, long nLeft=0, BOOL bRoadNotSlab=FALSE);
	CDPoint	GetXySlabEdgeAct(double sta, CDPoint vAng, BOOL bLeft, BOOL bRoadNotSlab=FALSE);
	CDPoint GetXySlabEdgeActByGagak(double sta, CDPoint vAng, BOOL bLeft, BOOL bRoadNotSlab=FALSE);
	BOOL	IsMatchGagak(double sta, CDPoint vAng, BOOL bLeft, BOOL bStt, BOOL bRoadNotSlab, CDPoint &xyResult);
//	double	GetDisSlabLeftLevelAct(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
//	double	GetDisSlabRightLevelAct(double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	double	GetWidthSlabAct(double sta, CDPoint vAng, long nLeft=0, BOOL bHunch=FALSE, BOOL bVertDir=FALSE, BOOL bRoadNotSlab=FALSE, CDPoint vAngByApplyAngle=CDPoint(0, 0));
	double	GetLengthHDanAct(double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, CDPoint &vAngReal=staticCDPointRC);// 현재 스테이션, 각도에서 nHDan 구간의 길이를 구함
	double	GetWidthTotalRoadWay(double sta);
	double	GetWidthTotalRoadWay();
	double	GetMaxDiffWidthByExtLine(BOOL bLeft);

	CDPoint GetXyShoe(long nJ, long nLeft, long nRow);
	CDPoint GetAngleShoe(long nJ, long nLeft, long nRow, long Dir, long nSlab, long FromJijum, long ToJijum);

	double	GetElSlabEndAtBriEnd(BOOL bStt, BOOL bLeft, BOOL bExp=FALSE);
	double	GetElSlabUpperAtRoadCenter(double sta, CDPoint vAng);
	CDPoint	GetXySlabUpperActByDist(double sta, double D, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, BOOL bExtendSlop=FALSE, BOOL bUseRealEL=FALSE);
	CDPoint	GetXySlabUpperActByDistAngle(double sta, double D, CDPoint vAng, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	double	GetStationAtSlabEnd(BOOL bStt, int nLeft, double D=0);
	double	GetStationByTarget(TARGET_IN_ONE_SLAB nTarget, long nLeft, BOOL bIncludeExp);
	double	GetStationByDis(BOOL bStt, double dLenMove, long nLeft, BOOL bIncludeExp);
	double	GetStationByDisOrdinaryLine(double staCR, double disCenter, CDPoint vAngCR, double dLenMove);
	double	GetStationByDisExtLineSimple(double staCR, CDPoint vAngCR, double dLenMove, long nLeft, double dExtDist=0);
	double	GetStationByDisExtLine(double staCR, CDPoint vAngCR, double dLenMove, double dDisExtLine, long nLeft);
	CDPoint	GetAngleByTarget(TARGET_IN_ONE_SLAB nTarget, long nLeft);
	CDPoint	GetAngleByDis(BOOL bStt, double dLenMove, long nLeft);
	CDPoint	GetAngleByDisOrdinaryLine(double staCR, double disCenter, CDPoint vAngCR, double dLenMove);
	CDPoint	GetAngleByDisExtLineSimple(double staCR, CDPoint vAngCR, double dLenMove, long nLeft);
	CDPoint	GetAngleByDisExtLine(double staCR, CDPoint vAngCR, double dLenMove, double dDisExtLine, long nLeft);
	void ModifyBridgeType(BOOL bCheckAps=TRUE);
	double	GetLengthJigan(int nJigan, long nSlabLeft=0, BOOL bVertDir=FALSE, BOOL bIncludeWall=TRUE);
	double	GetStationOnJijum(long nJ, long nSlabLeft, BOOL bVertDir=FALSE);
	double  GetStationOnJijum(long nJijum);
	double	GetStationShoeOnJijum(long nJ, long nSlabLeft, long nLeft, BOOL bVertDir=FALSE);
	CDPoint	GetAngleShoeOnJijum(long nJ, long nSlabLeft, long nLeft);
	void	GetDimArrayShoeDist(CDPointArray& xyArr, int nJ, int nLeft);

//	double GetDistInWall(long nIdxInWall, long nLeft=0, BOOL bVertDir=FALSE);
	int GetCountJijumShoe();
	long GetMaxShoe();
	CString GetStrShoeDirection(long Dir);
	long GetCountJijumBearing();

	CDPoint GetAngleJijumShoe(int nJijum, int bPrev);
	void SetDataDefaultCrossPosition();
	BOOL IsLeftHDan(long nHDan, BOOL bRoadNotSlab=FALSE);
	CString	GetStringTypeHDan(long nHDan, BOOL bWriteLeftRight, BOOL bRoadNotSlab=FALSE);
	void	SetTypeHDanByString(long nHDan, BOOL bWriteLeftRight, CString strType, BOOL bRoadNotSlab=FALSE);
	BOOL IsSkew();

	void SetDataDefaultDistShoe();
	void SetDataDefaultCountShoe();
	void SetDataInitJijum();
	void SetDataDefaultJijumLength();
	void SetDataDefaultJijumAngle();
	void SetDataDefaultGagak();
	void SetDataDefaultExpWidth();

	void MakeExpJoint();
	void SetExpJoint(CString szExpRefName, BOOL bStt, BOOL bRefSetting);
	// 형상 권고안
	void SetDataDefaultInWallType();
	void SetDataDefaultOutWallType();
	BOOL SetDataDefaultShapeBridge(BOOL bSttWall, long nLeft=0);
	BOOL SetDataDefaultShapeInWall(long nIdxWall, BOOL bOnlyDefaultHunch=FALSE, long nLeft=0);
	BOOL KeepHunchCount();
	BOOL KeepInWallCount();
	void GetInfoHunch(long nIdxHunch, long &nIdxInWall, BOOL &bLeft);
	long GetIdxHunchInWall(long nIdxWall, BOOL bLeft=TRUE);
	double GetHeightHunch(long nIdxHunch, BOOL bUpper=TRUE);
	void SetDataDefaultBoxLowerSlab();

	// 선형
	double GetStationBridgeMid();
	double GetWidthSlabFrLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft);// 기준선형 sta, vAng로 만나는 좌우측 선형에서 슬래브 끝단까지의 너비를 구함
	double GetStationMatchLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft);// 기준선형 sta, vAng로 만나는 좌우측 선형중심 스테이션을 구함
	CDPoint GetAngMatchLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft);// 기준선형 sta, vAng로 만나는 좌우측 선형중심 각도를 구함
	double GetDistLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft);// 기준선형 sta, vAng로 좌우측 선형중심까지의 길이를 구함
	CTwinVector GetTvJijum(long nJ, double dLenMove);
//	CTwinVector GetTvExpansionEnd(BOOL bStt, BOOL bLeft);
	
	CDPoint GetXyMatchLineAndSlabeLowerToDist(double dDistFromSlab, CDPoint xy, CDPoint xyDir, double dOffset=-1, long nLeft=0);
	CDPoint GetXyToDistFromSlabUpper(double dStt, double dDist, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE, BOOL bDistIsHor=TRUE);
	CDPoint GetXyToDistFromSlabLower(double dStt, double dDist, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE, BOOL bDistIsHor=TRUE);
	CDPoint GetXyMatchLineAndSlabUpper(CDPoint xy, CDPoint xyDir, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE);
	CDPoint GetXySlabUpperByLenFrSttStation(double dDistFromStt, long nLeft=0, BOOL bVertDir=FALSE);
	CDPoint GetXyMatchLineAndLowerSlab(CDPoint xy, CDPoint xyDir, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE);
	CDPoint GetxyLowerSlabBottomByLenFrSttStation(double dDistFromStt, long nLeft=0, BOOL bVertDir=FALSE);
	CDPoint GetXySectionSttEnd(BOOL bStt, int nLeft = 0, BOOL bVertDir=FALSE);
	CDPoint GetXySectionSttEndIncludePRF(BOOL bStt);
	CDPoint GetXyInWallHunchBottom(long nIdxWall, BOOL bLeft=TRUE, long nLeft=0, BOOL bVertDir=FALSE, BOOL bUpper=TRUE);
	CDPoint GetXyOnJijum(int nJ);

	//하부슬래브
	CDPoint GetXyMatchLineAndLowerSlabLower(CDPoint xy, CDPoint xyDir, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE); //하부슬래브 하면과 교차점.
	CDPoint GetXyToDistFromLowerSlab(double dStt, double dDist, long nLeft=0, double dOffset=-1, BOOL bVertDir=FALSE, BOOL bDistIsHor=TRUE);
	CDPoint GetXyMatchLineAndArchRib(BOOL bUpper, CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir=FALSE);

	// 날개벽 관련
	BOOL IsWingWall();
	BOOL IsWingWall(BOOL bStt, BOOL bLeft);
	BOOL IsWingWallVariableSect(BOOL bStt, BOOL bLeft);
//	BOOL IsWingWallVariablePlane(BOOL bStt, BOOL bLeft);
	CDPoint GetAngleAzimuthJijum(int nJ, BOOL bOnlyDirBySkew=FALSE);
	CDPoint GetAngleAzimuthWingWall(BOOL bStt, BOOL bLeft);
	CDPoint GetAngleWingWallBetweenWall(BOOL bStt, BOOL bLeft);
	void GetTvWingWallBoundAD(BOOL bStt, BOOL bLeft, CTwinVector &tv, BOOL bUpper=FALSE);
	void GetTvWingWallPlane(BOOL bStt, BOOL bInclBracket=TRUE, BOOL bUpper=FALSE, BOOL bInclBlockD=TRUE);
	void GetTvWingWallFront(BOOL bStt, BOOL bLeft, int nOut, CTwinVectorArray &tvArr, BOOL bIncludeOutWall=FALSE, BOOL bIncludeHunch=FALSE, BOOL bOnlySolidLine=FALSE, BOOL bForBM=FALSE);
	void GetTvWingWallSection(BOOL bStt, BOOL bLeft, CTwinVectorArray &tvArr, double dSpare=1000.0);
	void SetDataDefaultWingWallEl();
	void SetDataDefaultWingWallHeightSoil();
	void SetDataDefaultWingWall(BOOL bStt, BOOL bLeft, BOOL bFirst);
	CDPoint GetXyCenterWingWallPlane(BOOL bStt, BOOL bLeft);
	CDPoint GetXySectionWingWallStt(BOOL bStt, BOOL bUpper, BOOL bLowHunch=FALSE);
	CDPoint GetXyPlaneWingWallEnd(BOOL bStt, BOOL bLeft, BOOL bInclBlockD=TRUE);
	double GetElWingWallEnd(BOOL bStt, BOOL bLeft);
	double GetHeightWingWall(BOOL bStt);
	double GetCoverWingWallEnd(BOOL bStt, BOOL bLeft, BOOL bObq=TRUE);
//	int GetCountTotalWingWall();
	double GetLengthWingWallBlockBC(BOOL bStt, BOOL bLeft, BOOL bEL=FALSE);
	BOOL IsExistWingWallBlockA(BOOL bStt, BOOL bLeft);	/**< 날개벽 A구간이 존재하는지? */
	double GetLengthFootingWingWall(BOOL bStt, BOOL bLeft, BOOL bExFoot, BOOL bHeel/*뒷굽*/, int nOut=1);
	CDPoint GetXyPlaneBracket(BOOL bStt, BOOL bLeft);
	void GetTvWingSlopeLine(CTwinVectorArray& tvArr, BOOL bStt, BOOL bLeft, double dEl);			// 지반사면 선 저장
	BOOL IsWingWallTailBottom(BOOL bStt, BOOL bLeft);
	BOOL IsGagakForWing(BOOL bStt, BOOL bLeft);
	CDPoint GetDefaultAngleWingWall(BOOL bStt, BOOL bLeft);
	BOOL IsWingWallOnPRF(BOOL bStt, BOOL bLeft); //부상방지판 상면위에 날개벽(L2)이 있는지?
	CDPoint GetTvArrayWingWallInitialMesh(CTwinVectorArray& tvArrWing, CTwinVectorArray& tvArrFix, CTwinVectorArray& tvArrSK, CDPoint &xyWingWallStt, CDPoint &xyWingWallEnd, CDPoint &xyWingFootStt, CDPoint &xyWingFootEnd, double &dDistThick, long nIndexArea, BOOL bStt, BOOL bLeft);

	// 기초 형상 관련
	CDPoint GetAngleRectFootingEnd(long nJ, BOOL bLeft);
	double GetLengthFootingMargin(long nJ, BOOL bLeft);
	void SetFootingEndShape(long nJ);

	CDPoint GetXySlabHDanLeft(double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	BOOL SetOrgInWall(long nIdxWall, long nLeft=0, BOOL bVertDir=FALSE);
	void GetTvGuardWall(CGuardWallRC* pGW, CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bApplyRoundChamfer=TRUE, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, BOOL bForBM=FALSE);
	void GetTvCrossHunch(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bDrawSlopPattern, long nDrawLeft, BOOL bMirror=FALSE, BOOL bExp=FALSE, CDPoint vAngWall=CDPoint(0, 1), BOOL bCalHeightHunch=TRUE, double dHeightHunch=0);
	void GetTvCrossLowerHunch(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bDrawSlopPattern, long nDrawLeft, BOOL bMirror=FALSE, BOOL bExp=FALSE, CDPoint vAngWall=CDPoint(0, 1), BOOL bCalHeightHunch=TRUE, double dHeightHunch=0);
	void GetTvCrossSection(CTwinVectorArray& tvArr, double sta, CDPoint vAng = CDPoint(0, 1), BOOL bInclFootway=TRUE, BOOL bInclSlabLower=TRUE, BOOL bInclGuardWall=TRUE, long nDrawLeft=0, BOOL bMirror=FALSE, BOOL bIncludePave=TRUE, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, BOOL bOnlyRect=FALSE, BOOL bInclNotch=FALSE);
	void GetTvCrossSectionUnderNori(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bLeft);
	void GetTvCrossSectionLowerSlab(CTwinVectorArray& tvArr, double sta, CDPoint vAng = CDPoint(0, 1), long nDrawLeft=0, BOOL bMirror=FALSE, BOOL bExp=FALSE);
	double GetElevationRoadSectionLeft(double dStation);
	
//	double GetElevationRoadSectionRight(double dStation);
	BOOL GetTvVertSection(long nLeft=0, BOOL bApplyAngle=FALSE, BOOL bWingFront=FALSE, long nTypeJongdan=-1, BOOL bHunchInclude = FALSE);	// 종단면도 좌표구해서 종단좌표를 항상 최신으로 유지하고 있음.
	CDPoint GetAngleByStation(double dSta);
	void GetTvVertSectionAbut(CTwinVectorArray &tvArr, BOOL bStt);

	void GetTvTogongLine(CTwinVectorArray& tvArr, long nJijum, BOOL bCrossSection=FALSE, BOOL bMergeLower=FALSE, BOOL bIncludeUpper=FALSE, BOOL bForDraw=FALSE, long nDeductLeft=0);
	void GetSlopeTogong(CTwinVectorArray& tvArr, double x, double y, CFootingApp* pFooting, CBoringDataEx *pBoring, BOOL bLeft=TRUE);

//	BOOL MarkRecommandRebarJewon(long nRebarPart);

	// 현장타설 말뚝의 존재여부
	BOOL IsExsitHyunTa();
	
	CRcBridgeApp();
	virtual ~CRcBridgeApp();
	void CheckOverLapLineBetweenSubObject();
	void SyncBridgeAps(long nLeft, BOOL bApplyAngle=FALSE);

	/// 구조계산관련 함수
	CJoint* GetJointSlabOnJijum(CFEMManage *pFEM, long nJijum, BOOL bUpperSlab);
	long GetNumberElementSlabSpanSttCheck(long nSpan, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, BOOL bUpperSlab, double dDistShearPos=0, double &dDistStt=static_double);
	long GetNumberElementSlabSpanEndCheck(long nSpan, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, BOOL bUpperSlab, double dDistShearPos=0, double &dDistEnd=static_double);
	long GetNumberElementWallSttCheck(long nJijum, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, double dDistShearPos=0, double &dDistStt=static_double);
	long GetNumberElementWallEndCheck(long nJijum, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, double dDistShearPos=0, double &dDistEnd=static_double);
	long GetNumberElementPRFCheck(long nTypeDesign, BOOL bStt);

private:
	double GetSlopeHDanByDistGagak(double dDist90);
	void SetXySlabUpperAct90_Gagak(double dStation, CDPoint vAng);
	long GetIndexShellNearByJointNumber(CShell *pShell, long nIdxJoint1, long nIdxJoint2);
	void GeMidElementAndPointOnWall(long nTypeDesign, long nJijum, long &nElement, long &nPoint, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	BOOL IsSameMatrixRow(CMatrix Mat, long nRow1, long nRow2);
	
	double GetLengthDefaultAps(BOOL bStt);

	void SetTvPathPlan();
	void SetFootingStaAndEL(CDPoint xyOrgStt, CDPoint xyOrgEnd, BOOL bApplyAngle=FALSE, long nLeft=0);

//{31447
public:
	CDPoint GetAngleByStationForMesh(double dStation, double dDist=0, BOOL bVert=TRUE);
	long GetCountSidePileOrAnchor(CFootingApp *pFooting, long nIndex, BOOL bAnchor);
	long GetIndexPileOrAnchorSttAtSpan(CFootingApp *pFooting, long nSpan, BOOL bEarthAnchor);
	long GetIndexPileOrAnchorEndAtSpan(CFootingApp *pFooting, long nSpan, BOOL bEarthAnchor);
	double GetStationPileOrAnchor(CFootingApp *pFooting, long nIndex, BOOL bAnchor);
	double GetWingWallBackAndSideAttach(CTwinVectorArray & tvArr,long nOut, long nDan, BOOL bLeft); //리턴값: 배면의 헌치를 제외한 수평철근의 실길이, tvArr 지점의 평면도 상의 총 날개벽
	void GetWingWallbackAndSideAttachRebar(CTwinVectorArray & tvArr,long nOut, long nDan, BOOL bStt ,BOOL bUpper, BOOL bLeft); //날개벽 평면도 상의 철근 길이를 tvArr에 넣어줌 bUpper는 보통 TRUE로 설정 bUpper는 보통 TRUE로 설정
	double GetSettleRebarHoriRevision(CTwinVectorArray & tvArr, double dWingBackLen , long nOut, BOOL bStt, BOOL bLeft); //평면도상의 수평철근 정착장을 정면도에 넣기 위해서 수평선상에서 본 값으로 보정시키는것 리턴값 : 보정된 정착장 길이
//}
	double GetHeightBridgeBox(double dStation);
	double GetElevationFootingLower(double dStation, BOOL bUseSection);
	double GetElevationBoxBottom(long nJijum, double dDist=0, BOOL bOutSide=TRUE, BOOL bVerOpt=TRUE);
	void SyncBridgeBoxType(BOOL bApplyAngle, long nTypeJongdan=-1);
	void GetTvBoxLowerExFootFront(long nLeft=0, BOOL bApplyAngle=FALSE);
	void GetTvBoxLowerExFootPlan();
	void GetTvBoxLowerExFootSide(double dStation, CDPoint vAng, long nLeft);

	CStmModel* GetStmModelByKey(long iKey, HGBOOL bOutput=FALSE);
	CStmModel* GetStmModelBracket(BOOL bStt);
	CStmModel* GetStmModelFoot(BOOL bStt, long iCase, long iIndex);
	CStmModel* GetStmModelMidFoot(HGINT32 iInnerJ, long iCase);
	HGINT32 GetStmKeyByStmModel(CStmModel* pStm);
//	CStmModel* GetStmModelCorner(BOOL bStt, BOOL bUpper, BOOL bPlus, long iExtreme=0);
	void SetStmModelByKey(HGINT32 iKey, BOOL bMakeModel, HGBOOL bOutput=FALSE );
	void MakeStmModelSyncAll(HGBOOL bOutput=FALSE);
	
	HGINT32 GetKeyStmModel(eStmModelPos ePos, BOOL bStt=TRUE, BOOL bUpper=TRUE, long iIndex=0, long iCase=0 ,long iMidType=0);
	CString GetStringStmModelNameByKey(HGINT32 iKey, long iDepth=0);

	// (LSD:0001)
	double GetValueFck(EPartOfBridge ePos, BOOL bChangeUnit=FALSE);
	double GetValueFy(EPartOfBridge ePos, BOOL bShearRebar=FALSE, BOOL bChangeUnit=FALSE);
	double GetValueEc(EPartOfBridge ePos, BOOL bChangeUnit=FALSE);
	
	void SetDataDefaultExposureRatingLsd();
	void SetDataDefaultDesignRatingLsd();
	void SetDataDefaultRebarFactorLsd();
	void SetDataDefaultRebarSettleLsd();
	void SetDataDefaultRebarJointLsd();

	//한계상태 추가
	void SetDataDefaultSelectLoadLsd(BOOL bForCantilever=FALSE);
	void SetDataDefaultLoadFactor(BOOL bForCantilever=FALSE);
	void SetDataDefaultComboLSD(long nType, BOOL bForCantilever=FALSE);
	void SetDataDefaultModifiedFactor(long nIdex, BOOL bForCantilever=FALSE); //-1: 전체 0: 슬래브 1: 벽체 2: 내측벽체 3:기초 4:내측기초 5: 날개벽 6: 접속슬래브
	void SetDataDefaultLsdLoad();
	void SetDataDefaultLsdSupportRebar();
	// 기초지반 설정
	void SetDataDefaultBaseGround(HGINT32 ixJijum=-1, BOOL bInit=FALSE);	// -1:전체

	double GetHeightBridgeWall(BOOL bStt);

	HGDOUBLE CalcUnderGroundDesignHeightForWingWall(BOOL bStt, BOOL bLeft);
	HGDOUBLE CalcUnderGroundDesignHeight();
	HGDOUBLE GetUnderGroundPaveHeightTosa();
	HGBOOL GetMinMaxStation(HGDOUBLE &dMinSta, HGDOUBLE &dMaxSta);
	HGBOOL GetTvArrayLaneForPlane(HGINT32 ixHDan, CTwinVectorArray &tvArr, HGBOOL bRoadNotSlab);
	HGBOOL GetTvArrayNoriOutLineForPlane(CTwinVectorArray &tvArr, HGBOOL bLeft);
	CDPoint GetXyLaneForPlane(HGDOUBLE dSta, HGDOUBLE dDist);
	void TrimLaneNoriForPlane(CTwinVectorArray & tvArr);
	
	double GetCtcColumn(long nJijum);
	double GetCoefficientRegionEQ();
	double GetCoefficientDangerEQ();
	double GetCoefficientAccelerateEQ(EGoalOfEarthQuake eGoalEq=eGoalEQ_None);
	double GetFactorCoDanger(EGoalOfEarthQuake eGoalEq=eGoalEQ_None);
	BOOL IsCantileverDesign();

	//방호벽의 설치 유무 ( bRebar(철근): TRUE, 방호벽 일반: FALSE)
	//한개라도 존재할시 : TRUE, 한개도 없을시 FALSE
	BOOL IsExistGuardWall(BOOL bRebarType);
	BOOL IsUseModelCombo(long nModelCombo);

	BOOL IsApplyTankLoad();
};

#endif // !defined(AFX_RCBRIDGEAPP_H__D04D68DD_141E_4EB0_AF5B_E12CFC6882D9__INCLUDED_)
