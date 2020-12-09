// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RcBridgeRebarUtil.h: interface for the CRcBridgeRebarUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBRIDGEREBARUTIL_H__1B0A1E0D_D528_4038_A56D_006F8E58BA77__INCLUDED_)
#define AFX_RCBRIDGEREBARUTIL_H__1B0A1E0D_D528_4038_A56D_006F8E58BA77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 철근 자동 판단을 위한 define ////////////////////////
#define REBARTYPE_SU		78
////////////////////////////////////////////////////////////

// 방호벽 철근의 구간수
#define MAX_REBAR_GUGAN		10



static CTwinVectorArray staticTvArr;
static CDPointArray staticPtArr;
static CDDWordArray staticDwArr;

class CARcBridgeDataStd;
class CGuardWallRC;

#define MAKE_RATE_UTIL m_nTypeAddRate==0 ? ((pRB->m_dDia>m_dAddRateDiaOrg) ? 1+m_dAddRateUpper/100 : 1+m_dAddRateLower/100) : (pRB->m_bMainRebar ? 1+m_dAddRateLower/100 : 1+m_dAddRateUpper/100)

typedef CSimpleTypedArray <CRebarJoint> CJointArray;

// CRcBridgeRebar 덩치가 너무 커서 CRcBridgeRebar에서 Util의 성격을 가진 함수들을 따로 뺌(이곳으로)
class AFX_EXT_CLASS CRcBridgeRebarUtil  
{
public:
	void GetXyRebarCR6789OfGueardFence(CGuardWallRC *pGW, CDPointArray &xyArr6, CDPointArray &xyArr7, CDPointArray &xyArr8, CDPointArray &xyArr9, CTwinVectorArray &tvArrRebar);
	// 방호벽
	void SetDataDefaultRebarGuardWall(CGuardWallRC* pGW, double sta, CDPoint vAng);
	void GetTvRebarGuardWall(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0, BOOL bCover=FALSE);
	void GetTvRebarGuardFence(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarCenterGuard(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarCenterGuard2(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarCenterGuardHalf(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarHandRail(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarSoundProof(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarRetainingWall(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0);
	void GetTvRebarFootwayBlock(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0, BOOL bCover = FALSE);
	void GetTvRebarTotalHole(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale = 0, BOOL bCover = FALSE);

	void MakeGuardFenceRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeCenterGuardRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeCenterGuard2RebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeCenterGuardHalfRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeHandRailRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeSoundProofRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeRetainingWallRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeFootwayBlockRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeRailGuardFenceRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);
	void MakeRailTotalHoleRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate = FALSE);

	void MergeRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nIdx1, long nIdx2);
	void GetTvArrUpperSlabByDrawLeft_MainRebar(CTwinVectorArray &tvArr, long nDrawLeft);
	long GetNumMark(CString sMark);
	void SetLengthRebar(CRebar *pRB, long nJointType, double dCover, double dRebarSpace, long nBeta=2, long nIp=1);
	// MakeRebarInfoCycle후 bExist가 FALSE인 것을 완전히 제거하는 함수
	void RemoveNotExistRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC);
	double GetPosRebarMark(CDPointArray &xyArr, int nCount, BOOL bBet);
	double GetPosRebarMark(vector<double> &vec, int nCount);
	double GetPosRebarMark(CTwinVectorArray &tvArr, BOOL bHor, int nCount);

	double GetStationRebarInfoCycle(CRebarInfoCycle *pRC, BOOL bStt);
	// 철근 타입 자동 판단 관련 함수 //////////////////
	void SetRebarType();
	CDDWordArray m_pArrRebarType[REBARTYPE_SU];
//	BOOL GetRebarType(CRebar *pRB, CTwinVectorArray &tvArr, CTwinVectorArray &retTvArr=staticTvArr, CDDWordArray &retDwArr=staticDwArr);
//	DWORD GetDirectVector(CTwinVector tv, BOOL bRevDir);
//	void InvertRebarType(CDDWordArray &arrWord);
	// 철근 타입 자동 판단 관련 함수 //////////////////
	void GetXyDivideLine(CDPointArray &xyArr, CTwinVector tv, double dSttOffset, long nCountMid, double dMidOffset, double dEndOffset);
	void DivideVertMainRebarBySabogang(CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, BOOL bRev=FALSE);
	void AddFromTvArray(CTwinVectorArray &tvArr, CTwinVectorArrayArray *pTvArr, long nIdx);
	void GetJointAtRebarInfoCycleArray(CJointArray &arrJoint, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bParabola=FALSE);
	BOOL GetJointAtRebarInfoCycle(CRebarInfoCycle *pRC1, CRebarInfoCycle *pRC2, CRebarJoint &joint, BOOL bParabola=FALSE);
	void InvertRebarInfoCycle(CRebarInfoCycle *pRC);
	void AddRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrDest, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrSource, BOOL bInvert=FALSE);

	void RemoveMeanLessTvArr(CTwinVectorArrayArray *pTvArrArr);
	void TrimCutEndOfGagak(CTwinVectorArrayArray *pTvArrArr, CTwinVectorArray &tvArrPlane, BOOL bStt, BOOL bLeft, BOOL bFindByLineFirstLine=FALSE);
	void TrimCutEndOfGagak(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrPlane, BOOL bStt, BOOL bLeft, BOOL bFindByLineFirstLine=FALSE, BOOL bHunch=FALSE);

	// 한계상태 방호벽 이음 산출근거용 데이터
	void SetRebarLsdOptionGuardWall(SRebarEnvOption *pSLsdOption, CGuardWallRC *pGW);

	CRcBridgeRebarUtil();
	virtual ~CRcBridgeRebarUtil();
	CARcBridgeDataStd *m_pARcBridgeDataStd;
	CRcBridgeApp *m_pRcBridgeApp;

	// 철근 할증률 설정 옵션
	long	m_nTypeAddRate;	// 0 : 직경별, 1 : 주철근,기타철근
	double	m_dAddRateLower;	// 하위 할증률
	double	m_dAddRateUpper;	// 상위 할증률
	double	m_dAddRateDiaOrg;	// 직경별일경우 기준 직경
	void SyncRebarAddRateOpt();
};

#endif // !defined(AFX_RCBRIDGEREBARUTIL_H__1B0A1E0D_D528_4038_A56D_006F8E58BA77__INCLUDED_)
