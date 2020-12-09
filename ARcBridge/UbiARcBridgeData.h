// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// UbiARcBridgeData.h: interface for the CUbiARcBridgeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UBIARCBRIDGEDATA_H__3810075B_8FE6_49C8_8EE4_26D33EEA3F6A__INCLUDED_)
#define AFX_UBIARCBRIDGEDATA_H__3810075B_8FE6_49C8_8EE4_26D33EEA3F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define SOLID_UNKNOW	0
#define SOLID_COPPING	1
#define SOLID_COLUMN	2
#define SOLID_FOOTING	3
#define SOLID_EXFOOTING	4
#define SOLID_PILE		5
#define SOLID_SLAB		6
#define SOLID_GUARDWALL	7

#define SOLID_RCWALL	8
#define SOLID_WINGWALL	9
#define SOLID_APS		10

#define REBAR_UNKNOW	0
#define REBAR_COPPING	1
#define REBAR_COLUMN	2
#define REBAR_FOOTING	3
#define REBAR_EXFOOTING	4
#define REBAR_PILE		5
#define REBAR_SLAB		6
#define REBAR_GUARDWALL	7

#define REBAR_RCWALL	8
#define REBAR_WINGWALL	9
#define REBAR_APS		10

#define REBARCOLOR_MAIN		0
#define REBARCOLOR_SUPPORT	1
#define REBARCOLOR_SHEAR	2
#define REBARCOLOR_DANCHA	3
#define REBARCOLOR_ROUND	4
#define REBARCOLOR_CROSSTIE	5

class CWallApp;
class CPierApp;
class CRcBridgeApp;
class CARcBridgeDataStd;
class CUbiARcBridgeData : public CDraw3DBase  
{
public:
	void MakeRcUpperSlabGriderRebar_SupportRebar(CRcBridgeRebar *pBri, long nJijum);
	void MakeRcUpperSlabGriderRebar_MainRebar(CRcBridgeRebar *pBri, long nJijum);
	void MakeRcUpperSlabGriderRebar(CRcBridgeRebar *pBri, long nJijum);
	BOOL MakeOffsetRebar(CRcBridgeRebar *pBri, CRebarInfoCycle *pRC, long nJijum=-1);
	BOOL GetTvArrSlabPlanSide(CTwinVectorArray &tvArrReturn, CRcBridgeApp *pBri, BOOL bStt);
	void MakeRcDanbuRebar_MainRebar(CRcBridgeRebar *pBri);
	void MakeRcBracketRebar_SupportRebar(CRcBridgeRebar *pBri);
	void MakeRcBracketRebar_MainRebar(CRcBridgeRebar *pBri);
	void MakeRcBracketRebar(CRcBridgeApp *pBri);
	void MakeTvArrByRC(CTwinVectorArray &tvArrReturn, CRebarInfoCycle *pRC);
	BOOL GetTvArrWallLeftSide(CWallApp *pWall, CTwinVectorArray &tvArrLeft);
	void MakeMainRebarBytvArrPlan(CTwinVectorArray &tvArrPlan,CRebarInfoCycle *pRC, long nType, CVector vColor);
	void MakeRcWallTopIsColumn(CRcBridgeApp *pBri, long nWallNum);
	BOOL GetTvArrHunchGuideLine(CTwinVectorArray &tvArrReturn, CRcBridgeApp *pBri, double dSta, CDPoint vAng);
	void MakeRcWallHunch(CRcBridgeApp *pBri, long nNumWall);
	BOOL GetXySlabConstructAct(CTwinVectorArray &tvArrSlabConstruct, CDPointArray &xyArrDist);
	void CalcXyArrAct(CTwinVectorArray &tvArr, CDPointArray &xyArrDist);
	BOOL CheckDist(CDoubleArray dArrDist);
	BOOL TrimByHunchLine(CRcBridgeApp *pBri, CTwinVectorArray &tvArrSlabCrossSection, double dSta, CDPoint vAng, CDPoint vAngWall=CDPoint(0,1));
	void MakeRcInnerHunch(CRcBridgeApp *pBri);
	void ModifyFrontToPlanHunch(CRcBridgeApp *pBri, CUbGeoCurveArray &edgeArr, long nHunchRebar, CDRect rcUpperSlab);
	void MakeRcHunchRebar_SupportRebar(CRcBridgeRebar *pBri);
	void MakeRcHunchRebar_MainRebar(CRcBridgeRebar *pBri);
	void MakeRcHunchRebar(CRcBridgeApp *pBri);
	void MakeRcGaurdFenceRebar_SupportRebar(CRcBridgeApp *pBri, long nHDan);
	void MakeRcGaurdFenceRebar_MainRebar(CRcBridgeApp *pBri, long nHDan);
	void MakeRcSlabConstructRebar(CRcBridgeApp *pBri);
	void MakeRcApsRebar_ShearRebarHunch(CApsApp *pAps);
	void MakeRcApsRebar_SupportRebarHunch(CApsApp *pAps);
	void MakeRcApsRebar_MainRebarHunch(CApsApp *pAps);
	void MakeRcApsRebar_OutRebar(CApsApp *pAps);
	void MakeRcApsRebar_SupportRebar(CApsApp *pAps);
	void MakeRcApsRebar_MainRebar(CApsApp *pAps);
	void MakeRcApsRebar(CApsApp *pAps);
	void ModifySectionToPlanAps(CApsApp *pAps, CUbGeoCurveArray &edgeArr);
	void MakeRcAps(CApsApp *pAps);
	void MakeRcSlabRebar_ShearRebar(CRcBridgeRebar *pBri);
	void MakeRcSlabRebar_SupportRebar(CRcBridgeRebar *pBri);
	void MakeRcSlabRebar_MainRebar(CRcBridgeRebar *pBri);
	void MakeRcSlabRebar(CRcBridgeApp *pBri, double dCurPos);
	void CalcVertSectionHunchHidden(CRcBridgeApp *pBri, long nNumWall, CTwinVectorArray &tvArrColumnFront);
	void MakeRcArchRebar_ShearRebar(CRcBridgeApp *pBri, long nNumWall);
	void MakeRcArchRebar_SupportRebar(CRcBridgeApp *pBri);
	void GetArchRibRebarTvArrByInfoCycle(CRcBridgeRebar *pBri,CRebarInfoCycle *pRC, BOOL bDecreaseSttRebarInFooting, CParabola *pPb, CTwinVectorArray &tvArrArchRib);
	void MakeRcArchRebar_MainRebar(CRcBridgeApp *pBri);
	void MakeRcArchRebar(CRcBridgeApp *pBri, long nNumWall);
	void GetOutWallPlan(CRcBridgeApp *pBri, CTwinVectorArray &tvArrOutWallPlan, BOOL bStt);
	void MakeRcWallRebar_ShearRebar(CRcBridgeApp *pBri);
	void ModifySideToPlanWall(CWallApp *pWall, CUbGeoCurveArray &edgeArr, CTwinVector &tvPlan);
	void MakeRcColumnRebar_ShearRebar(CRcBridgeApp *pBri, long nInWallNum);
	void MakeRcWallRebar_SupportRebar(CRcBridgeApp *pBri);
	void MakeRcWallRebar_MainRebar(CRcBridgeApp *pBri);
	void MakeRcWallRebar(CRcBridgeApp *pBri);
	void MakeRcColumnRebar_SupportRebar(CWallApp *pWall, double dOffset, long nTypeBridge);
	void ModifyFrontToPlanColumn(CWallApp *pWall, CUbGeoCurveArray &edgeArr, CTwinVectorArray &tvArrColumnFront, long nTypeBridge);
	void MakeRcColumnRebar_MainRebar(CRcBridgeApp *pBri, long nInWallNum);
	void MakeRcTopIsColumnRebar_CrossTieRebar(CWallApp *pWall);
	void MakeRcTopIsColumnRebar_RoundRebar(CWallApp *pWall);
	void MakeRcTopIsColumnRebar_MainRebar(CWallApp *pWall);
	void MakeRcTopIsColumnRebar(CWallApp *pWall);
	void MakeRcColumnRebar(CRcBridgeApp *pBri);
	void MakeRcFootingRebar_MainRebarSideSlope(CFootingApp *pFooting);
	void MakeRcFootingRebar_SupportInnerFront(CFootingApp *pFooting);
	void MakeRcFootingRebar_SideRebar(CFootingApp *pFooting);
	void MakeRcFootingRebar_ShearRebar(CFootingApp *pFooting);
	void ModifyFrontToPlan(CTwinVector *pTvPlan, CUbEttRebar *pRebar, CTwinVector *pTvRef=NULL, CUbEttRebar *pRebarRef=NULL);
	void MakeRcFootingRebar_MainRebar(CFootingApp *pFooting);
	BOOL MakeRebar(CUbEttRebar &rebar, CTwinVectorArray &tvArr);
	double GetHeightFootingByDist(CFootingApp *pFooting, BOOL bSide, double dDistFromLeft);
	CVector GetColorRebar(long nType);
	CUbEttRebarBundle* GetRebarBundle(long nGroup);
	CUbEttRebarBundle* GetCurRebarBundle();
	void OffsetRebar(CUbEttRebar &rebar, CVectorArray &vecArrPos, long nType, CVector vColor);
	BOOL MakeRebar(CUbEttRebar &rebar, CRebarInfoCycle *pRC);
	void MakeRcFootingRebar_MainRebarSide(CFootingApp *pFooting);
	void MakeRcFootingRebar(CFootingApp *pFooting, BOOL bDrawFootingFront);

	void MakeRcBracket(CRcBridgeApp *pBri);
	void MakeRcSlabConstructSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri, long nHDan);
	void MakeRcSlabConstructPlan(CUbEttSolid *pSolid, CRcBridgeApp *pBri, long nHDan);
	BOOL GetTvArrSlabConstruct(CTwinVectorArray &tvArrSlabConstruct, CRcBridgeApp *pBri, double dStaStt, CDPoint vAng, long nHDan);
	void MakeRcSlabConstruct(CRcBridgeApp*pBri);
	void MakeRcHunchSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri, CTwinVectorArray &tvArrVertSection, CUbGeoCurveArray &edgeArrLine, BOOL bStt);
	BOOL CalcXyArrMatchArchHidden(CRcBridgeApp *pBri, long nNumWall, CDPoint xyMatch[2]);
	void AddRebar(CUbEttRebar *pRebar, long nType, CString sItemPathName, CVector vColor);
	void MakeRcHunch(CRcBridgeApp *pBri);
	void MakeRcSlab(CRcBridgeApp *pBri);
	void MakeRcSlabSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri);
	void MakeRcSlabPlan(CUbEttSolid *pSolid, CRcBridgeApp *pBri);
	BOOL GetXySlabAct(CRcBridgeApp *pBri, double sta, CDPoint vAng, CDPointArray &xyArrDist/*, CDoubleArray &dArrSta*/);
	void TrimArcTvArr(CTwinVectorArray &tvArrArc, long nTrimNum = 7);
	BOOL GetStationAndDistMatchVertical(CRcBridgeApp *pBri, BOOL bStationStt, CDPointArray &xyArrDist, CDoubleArray &dArrStation, CDoubleArray &dArrDist);
	void MakeRcWall(CRcBridgeApp *pBri);
	void MakeRcColumn(CRcBridgeApp *pBri);
	void MakeRcArch(CRcBridgeApp *pBri, long nNumWall);
	void CalcVertSectionArchHunchHidden(CRcBridgeApp *pBri, long nNumWall, CTwinVectorArray &tvArrColumnFront);
	void MakeRcPile(CExPileApp *pPile);
	void MakeRcFooting(CFootingApp *pFooting, BOOL bDrawFootingFront);
	void MakeRcExFooting(CExFootingApp *pExFooting, BOOL bDrawFootingFront);

	CUbiARcBridgeData();
	virtual ~CUbiARcBridgeData();
	void CreateAllSolid();
	UINT DoWork();

	// 2D -> 3D
	void ModifySideToPlanFooting(CFootingApp *pFooting, CUbGeoCurveArray &edgeArr);
	void ModifyFrontToPlanFooting(CFootingApp *pFooting, CUbGeoCurveArray &edgeArr);
	void ModifySideToPlanExFooting(CExFootingApp *pExFooting, CUbGeoCurveArray &edgeArr);
	void ModifyFrontToPlanExFooting(CExFootingApp *pExFooting, CUbGeoCurveArray &edgeArr);

	// 좌표 변환
	enum TRS_PLANE { PLN_XOY, PLN_YOX, PLN_XOZ, PLN_ZOX, PLN_YOZ, PLN_ZOY };
	void TrsPlane(CUbGeoCurveArray &edgeArr, TRS_PLANE trPln, double dExtraValue=0);
	void TrsPlaneOne(CVector &v1, TRS_PLANE trPln, double dExtraValue);
	void TrsVectorArray(CUbGeoCurveArray &edgeArr, const CTwinVectorArray &tvArr, TRS_PLANE trPln, double dExtraValue);
	void GetVectorArrayFrTvArray(CUbGeoCurveArray &edgeArr, const CTwinVectorArray &tvArr);
	void ConvertTwinVectorArrayToUbEdgeArray(CTwinVectorArray &tvArr, CUbGeoCurveArray &edgeArr);
	void ConvertTwinVectorToUbEdge(CTwinVector *pTv, CUbGeoCurve *pEdge);

	// 공통 사용 함수 /////////
	TopoDS_Face MakeFaceByEdgeArr(CUbGeoCurveArray &edgeArr);
	BOOL MakeWire(CUbGeoCurveArray &edgeArr, TopoDS_Wire &wire);
	void AddSolid(CUbEttSolid* pSolid, long nType);
	void AddSolid(TopoDS_Shape *pShp, long nType, CString sItemPathName);
	TopoDS_Shape GetTopoDS_Shape(CUbGeoCurveArray &edgeArr, CVector vDir);
	TopoDS_Face GetTopoDS_Face(CUbGeoCurveArray &edgeArr);

	// 인덱스
	long m_nIdx;
	long GetIdx(long nType);
	void InitIdx(long nType);
	BOOL m_bBundleRebar;	// 묶음 철근으로 생성할지?
	double m_dRateCreateRebar;

	long m_nIdxRebar;
	long GetIdxRebar(long nType);
	void InitIdxRebar(long nType);
	long m_nIdxRebarForAddEnableRebar;	// 철근 생성 해도 되는지 검사하기 위한 인덱스
	long m_nIdxGroupRebar;				// 그룹 인덱스
	long GetIdxGroupRebar();
	void IncreaseIdxGroupRebar();
	BOOL IsAddEnableRebar();


	CString m_sItemPathNameHeader;
	CARcBridgeDataStd* m_pARcBridgeDataStd;

	CTypedPtrArrayEx <CObArray, CUbEttSolid*> m_arrSolid;				// 솔리드 객체들...
	CTypedPtrArrayEx <CObArray, CUbEttRebar*> m_arrRebar;				// 철근 객체들...
	CTypedPtrArrayEx <CObArray, CUbEttRebarBundle*> m_arrRebarBundle;	// 철근 객체들...
	CMakeRebarDetail *m_pMRD;

	long	m_nDrawKind;
	long	m_nDrawRebar;
	long	m_nDrawBridge;
	/* Test
	void GetTvPlaneBearing(CTwinVectorArray &tvArr, BOOL bSupport);
	void MakeBearing();
	*/
};

#endif // !defined(AFX_UBIARCBRIDGEDATA_H__3810075B_8FE6_49C8_8EE4_26D33EEA3F6A__INCLUDED_)
