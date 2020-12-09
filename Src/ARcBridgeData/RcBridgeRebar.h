// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RcBridgeRebar.h: interface for the CRcBridgeRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBRIDGEREBAR_H__25A06976_64C1_4C1B_85E9_286A733F4F5D__INCLUDED_)
#define AFX_RCBRIDGEREBAR_H__25A06976_64C1_4C1B_85E9_286A733F4F5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RcBridgeApp.h"
#define GAP_JOINT			30		// 주철근과 이음간의 간격

// 철근마크 리턴을 위한 부위 정의 ///////////////////////////////
#define MARK_UPPERSLAB_UPPER	0	// 상부슬래브 상면 철근 마크
#define MARK_UPPERSLAB_LOWER	1	// 상부슬래브 하면 철근 마크
#define MARK_UPPERSLAB_HUNCH	2	// 상부슬래브 헌치부 철근 마크
#define MARK_STTWALL_INNER		3	// 시점측벽체 내측 철근 마크
#define MARK_STTWALL_OUTTER		4	// 시점측벽체 외측 철근 마크
#define MARK_ENDWALL_INNER		5	// 종점측벽체 내측 철근 마크
#define MARK_ENDWALL_OUTTER		6	// 종점측벽체 외측 철근 마크
#define MARK_OUTWALL_HUNCH		7	// 외측벽체 헌치 철근 마크
#define MARK_OBTUSE_UPPER		8	// 상부슬래브 둔각부 상면
#define MARK_OBTUSE_LOWER		9	// 상부슬래브 둔각부 하면
#define MARK_OBTUSE_CROSS		10	// 상부슬래브 둔각부 (횡)
#define MARK_ACUTE_OUTWALL		11	// 외측벽체 예각부 주철근
#define MARK_ARCHRIB_UPPER			12	// 리브 상면
#define MARK_ARCHRIB_LOWER			13	// 리브 하면
#define MARK_LOWERSLAB_UPPER	14	// 하부슬래브 상면 철근 마크
#define MARK_LOWERSLAB_LOWER	15	// 하부슬래브 하면 철근 마크
#define MARK_LOWERSLAB_HUNCH	16	// 하부슬래브 헌치부 철근 마크
#define MARK_OBTUSE_UPPER_LOWSLAB      17	// 하부슬래브 둔각부 상면
#define MARK_OBTUSE_LOWER_LOWSLAB		18	// 하부슬래브 둔각부 하면


/////////////////////////////////////////////////////////////////

// 철근 정의

// 배력철근 ////////////
// 상부슬래브
#define US_REBAR_S_U1		0	// 상부슬래브 배력철근 상면 1단.
#define US_REBAR_S_U2		1	// 상부슬래브 배력철근 상면 2단.
#define US_REBAR_S_U3		2	// 상부슬래브 배력철근 상면 3단.
#define US_REBAR_S_L1		3	// 상부슬래브 배력철근 하면 1단.
#define US_REBAR_S_L2		4	// 상부슬래브 배력철근 하면 2단.
#define US_REBAR_S_L3		5	// 상부슬래브 배력철근 하면 3단.

// 시점측 벽체
#define SW_REBAR_S_O1		6	// 시점측벽체 배력철근 외측 1단.
#define SW_REBAR_S_O2		7	// 시점측벽체 배력철근 외측 2단.
#define SW_REBAR_S_O3		8	// 시점측벽체 배력철근 외측 3단.
#define SW_REBAR_S_I1		9	// 시점측벽체 배력철근 내측 1단.
#define SW_REBAR_S_I2		10	// 시점측벽체 배력철근 내측 2단.
#define SW_REBAR_S_I3		11	// 시점측벽체 배력철근 내측 3단.

// 종점측 벽체
#define EW_REBAR_S_O1		12	// 종점측벽체 배력철근 외측 1단.
#define EW_REBAR_S_O2		13	// 종점측벽체 배력철근 외측 2단.
#define EW_REBAR_S_O3		14	// 종점측벽체 배력철근 외측 3단.
#define EW_REBAR_S_I1		15	// 종점측벽체 배력철근 내측 1단.
#define EW_REBAR_S_I2		16	// 종점측벽체 배력철근 내측 2단.
#define EW_REBAR_S_I3		17	// 종점측벽체 배력철근 내측 3단.

// 벽체 헌치부
#define SW_REBAR_SH			18	// 시점벽체 헌치부 배력철근
#define EW_REBAR_SH			19	// 종점벽체 헌치부 배력철근

// 외측벽체 하단 예각부 배력근
#define SW_REBAR_AC_L		20	// 시점벽체 좌측 예각부 배력철근
#define SW_REBAR_AC_R		21	// 시점벽체 우측 예각부 배력철근
#define EW_REBAR_AC_L		22	// 종점벽체 좌측 예각부 배력철근
#define EW_REBAR_AC_R		23	// 종점벽체 우측 예각부 배력철근

// 상부슬래브 둔각부 배력근
#define US_REBAR_OB_SL		24	// 상부슬래브 시점좌측 둔각부 배력철근
#define US_REBAR_OB_SR		25	// 상부슬래브 시점우측 둔각부 배력철근
#define US_REBAR_OB_EL		26	// 상부슬래브 종점좌측 둔각부 배력철근
#define US_REBAR_OB_ER		27	// 상부슬래브 종점우측 둔각부 배력철근

#define US_REBAR_OB_SLL		28	// 상부슬래브 시점좌측 둔각부 하면 배력철근
#define US_REBAR_OB_SRL		29	// 상부슬래브 시점우측 둔각부 하면 배력철근
#define US_REBAR_OB_ELL		30	// 상부슬래브 종점좌측 둔각부 하면 배력철근
#define US_REBAR_OB_ERL		31	// 상부슬래브 종점우측 둔각부 하면 배력철근

// 아치리브
#define AR_REBAR_S_U1		32	// 아치리브 상면 1단 배력철근
#define AR_REBAR_S_U2		33	// 아치리브 상면 2단 배력철근
#define AR_REBAR_S_U3		34	// 아치리브 상면 3단 배력철근

#define AR_REBAR_S_L1		35	// 아치리브 하면 1단 배력철근
#define AR_REBAR_S_L2		36	// 아치리브 하면 2단 배력철근
#define AR_REBAR_S_L3		37	// 아치리브 하면 3단 배력철근

// 하부슬래브
#define LS_REBAR_S_U1		38	// 하부슬래브 배력철근 상면 1단.
#define LS_REBAR_S_U2		39	// 하부슬래브 배력철근 상면 2단.
#define LS_REBAR_S_U3		40	// 하부슬래브 배력철근 상면 3단.
#define LS_REBAR_S_L1		41	// 하부슬래브 배력철근 하면 1단.
#define LS_REBAR_S_L2		42	// 하부슬래브 배력철근 하면 2단.
#define LS_REBAR_S_L3		43	// 하부슬래브 배력철근 하면 3단.

// 하부슬래브 둔각부 배력근
#define LS_REBAR_OB_SL		44	// 하부슬래브 시점좌측 둔각부 배력철근
#define LS_REBAR_OB_SR		45	// 하부슬래브 시점우측 둔각부 배력철근
#define LS_REBAR_OB_EL		46	// 하부슬래브 종점좌측 둔각부 배력철근
#define LS_REBAR_OB_ER		47	// 하부슬래브 종점우측 둔각부 배력철근

#define LS_REBAR_OB_SLL		48	// 하부슬래브 시점좌측 둔각부 하면 배력철근
#define LS_REBAR_OB_SRL		49	// 하부슬래브 시점우측 둔각부 하면 배력철근
#define LS_REBAR_OB_ELL		50	// 하부슬래브 종점좌측 둔각부 하면 배력철근
#define LS_REBAR_OB_ERL		51	// 하부슬래브 종점우측 둔각부 하면 배력철근

#define SUP_REBAR_SU		52	// 배력철근 개수

// 전단철근 ////////////
#define US_REBAR_V			0	// 상부슬래브 전단철근
#define SW_REBAR_V			1	// 시점측벽체 전단철근
#define EW_REBAR_V			2	// 종점측벽체 전단철근
#define SD_REBAR_V1			3	// 시점 단부 스터럽1
#define SD_REBAR_V2			4	// 시점 단부 스터럽2
#define ED_REBAR_V1			5	// 종점 단부 스터럽1
#define ED_REBAR_V2			6	// 종점 단부 스터럽2
#define AR_REBAR_V			7	// 아치리브 전단철근
#define CO_REBAR_VL			8	// 접합부 스터럽철근 좌측
#define CO_REBAR_VR			9	// 접합부 스터럽철근 우측
#define LS_REBAR_V			10	// 하부슬래브 전단철근
#define SH_REBAR_SU			11	// 전단철근 개수


// 기타철근 ///////////
// 거더 
#define G_REBAR_M_U1		0	// 거더 철근 상면 1단(주철근)
#define G_REBAR_M_U2		1	// 거더 철근 상면 2단(주철근)
#define G_REBAR_M_U3		2	// 거더 철근 상면 3단(주철근)
#define G_REBAR_M_L1		3	// 거더 철근 하면 1단(주철근)
#define G_REBAR_M_L2		4	// 거더 철근 하면 2단(주철근)
#define G_REBAR_M_L3		5	// 거더 철근 하면 3단(주철근)
#define G_REBAR_HORV		6	// 거더 철근 측면(수평전단철근)
#define G_REBAR_HORV_I		7	// 거더 철근 측면(수평전단철근 내측)
#define G_REBAR_ST			8	// 거더 스터럽 철근(수직전단철근)
#define G_REBAR_SUP			9	// 거더 보강철근(주철근 2단부터 아래에 받치는 철근)

// 브라켓
#define BR_REBAR_M			10	// 브라켓 주철근
#define BR_REBAR_S			11	// 브라켓 배력근
#define BR_REBAR_V			12	// 브라켓 전단철근
#define ETC_REBAR_SU		13	// 기타철근 개수

//부상방지저판
#define BF_REBAR_M			14	// 부상방지저판 주철근
#define BF_REBAR_S			15	// 부상방지저판 배력근
#define BF_REBAR_V			16	// 부상방지저판 전단철근

// 교명주 받침
#define MO_REBAR_M1			0	// 교명주 받침 주철근 1
#define MO_REBAR_M2			1	// 교명주 받침 주철근 2
#define MO_REBAR_S_U		2	// 교명주 받침 상단 배력근
#define MO_REBAR_S_S		3	// 교명주 받침 측면 배력근
#define MO_REBAR_S_L		4	// 교명주 받침 하단 배력근
#define MO_REBAR_COUNT		5	// 교명주 받침 철근 개수

#define MIN_REBAR_SPACE 100

typedef CSimpleTypedArray <CRebarJoint> CJointArray;

// 교량 철근 배치 클래스
class AFX_EXT_CLASS CRcBridgeRebar : public CRcBridgeApp
{
public:
	void SerializeEnv(CArchive &ar);
	void SetDataDefaultEnv();
private:
	// 이 함수는 일반 변수 하나를 저장하거나 읽는 함수인데.. ///////
	// 변수 하나하나 모두 비교해서 저장하기 때문에 속도문제가 걸린다.
	// 그래서 SerializeRebar에서만 사용하기를 권고한다.
	// bLoad는 flag에 따라서 읽어야 할지 판단..
	template <class T>
	void SerializeValue(CArchive &ar, T *val, BOOL bLoad=TRUE)
	{
		if(ar.IsStoring()) ar << *val;
		else 
		{
			if(bLoad) ar >> *val;
		}
	} 
	/////////////////////////////////////////////////////////////////
	void SerializeRebar(CArchive &ar);
	void SerializeRebarBox(CArchive &ar);
public:
	long m_nFlag;

public:
	void StretchTvOfTvArr(CTwinVectorArray &tvArr, double dStt, double dEnd);
	BOOL IsRebarMainOneLineUpperSlab(BOOL bLeft, BOOL bStt);	/**< 상부슬래브 주철근 첫번째와 마지막 선형방향 철근이 'ㅡ'자가 되어야 하는지? */
	void MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nType, BOOL bStt, BOOL bLeft, long nJ, BOOL bGetRebar=FALSE);
	void ClearRebarList(long nType);
	void SetDataDefaultRebarJewonEtcStd();
private:
	void SetDataDefaultRebarJewon_UpperSlab_Support();
	void SetDataDefaultRebarJewon_UpperSlab_Shear();
	void SetDataDefaultRebarJewon_LowerSlab_Support();
	void SetDataDefaultRebarJewon_LowerSlab_Shear();
	void SetDataDefaultRebarJewon_OutWall_Support();
	void SetDataDefaultRebarJewon_OutWall_Shear();
	void SetDataDefaultRebarJewon_BFooting_Support();
	void SetDataDefaultRebarJewon_BFooting_Shear();
public:
	void SetDataDefaultRebarJewonMainStd();
	void SetShearRebarJewon(CRebar *pRB, long nTypeShear, double dWidth1, double dWidth2, double dHeight1, double dHeight2, double dDiaShear, double dDiaUpper, double dDiaLower, double dCover);
private:
	void SetDataDefaultRebarJewonFooting();
	void SetDataDefaultRebarJewonUpperSlabDanbu(BOOL bUpper);
	void SetDataDefaultRebarJewonInWall();
	void SetDataDefaultRebarJewonOutWall();
	void SetDataDefaultRebarJewonUpperSlab();
	void SetDataDefaultRebarJewonLowerSlab();
	void SetDataDefaultRebarJewonBFooting();
public:
	void DivideRebarSupport_Plane_UpperSlabExpSection(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrSttLeftAndRight, CTwinVectorArray &tvArrSttLeftOrRight, CTwinVectorArray &tvArrEndLeftAndRight, CTwinVectorArray &tvArrEndLeftOrRight);
	CDPoint GetAngleJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bRetSttAngle);
	BOOL IsExistMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	BOOL IsExistMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	BOOL IsExistMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx);
	BOOL IsJointMainRebarOutWall_Cycle2_Old(BOOL bStt, BOOL bUpper);
	void SetDataDefaultRebarHunch();
	void GetStaRebarSupport_VertSection_SlabHunch(BOOL bUpperSlab, CDoubleArray &dArr);
	void GetTvRebarSupport_Plane_UpperSlab_JijumAngle(CTwinVectorArray &tvArr, CDPointArray &xyArr, long nJ, long nDrawLeft, BOOL bByHunch=FALSE);
	void GetTvRebarSupport_Plane_SlabHunch(CTwinVectorArray &tvArr, BOOL bUpperSlab, long nDrawLeft, long nJ);
	CRebarInfoCycle *GetTvRebarMain_Plane_SlabHunch(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, long nCycle, double dSta, long nLeftRebar, long nDrawLeft, BOOL bUpperSlab = TRUE);
	void GetTvRebarSupport_Plane_SlabGirder(CTwinVectorArray &tvArr, BOOL bUpperSlab, long nJ, long nDan, BOOL bUpper, long nDrawLeft);
	void GetTvCuttingArea_Plane_UpperSlabGirder(CTwinVectorArray &tvArr, long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE);
	void GetTvTrack_Plane(CTwinVectorArray &tvArr, double dStaOfCen, double dDist, double dR, double dLenHor, BOOL bMakeToVer=FALSE);
	void GetTvCuttingArea_Plane_UpperSlab(CTwinVectorArray &tvArr, BOOL bUpper, long nDan);
	void GetTvCuttingArea_Plane_UpperSlab(CTwinVectorArray &tvArr, BOOL bUpper, long nDan, long nJ);
	BOOL IsDanbuRebar(BOOL bStt, long nDan, long nCycle, BOOL bUpper);
	void SetDataDefaultRebarDanbu();
	void GetTvCuttingArea_Side_OutWallHunch(CTwinVectorArray &tvArr, BOOL bStt, long nDrawLeft);
	void GetTvRebarMain_Side_OutWall(CTwinVectorArray &tvArr, BOOL bStt, BOOL bInner, long nCycle, long nDan, long nIdxRC, CTwinVectorArray &tvArrJoint);
	double GetLengthLineByPlaneXy(CDPoint xyStt, CDPoint xyEnd, CLineInfo *pLine=NULL);
	void MakeWallByOutWall(CWallApp &wall, BOOL bStt, BOOL bApplyAngle, BOOL bDefaultSupAndShear=FALSE, BOOL bAcute=FALSE, BOOL bLeft=FALSE, double dStation=dSta_Check_NotUse_Minus);
	CRebarInfoCycle* GetRCMain_SlabHunch(BOOL bUpperSlab, double dSta, long nCycle, BOOL bNotCompareSta=FALSE);
	CRebarInfoCycle* GetRCMain_Slab(BOOL bUpperSlab, double dSta, BOOL bUpper, long nDan, long nCycle, BOOL bNotCompareSta=FALSE);
//	void GetTvLineArrayByCrossTvArrayArea(CTwinVectorArray &tvArrLine, CTwinVectorArray &tvArrArea, BOOL bInner);
//	void GetTvLineByCrossTvArrayArea(CTwinVectorArray &tvArrLine, CTwinVectorArray &tvArrArea, BOOL bInner);
//	void GetTvCuttingArea_Plane_UpperSlabInner(CTwinVectorArray &tvArr, BOOL bLeft, long nIdxInWall);
	void GetTvRebarShear_Plane_UpperSlab(CTwinVectorArray &tvArr, BOOL bForBM=FALSE, BOOL bNormal=TRUE, BOOL bHunch=TRUE, BOOL bRib=TRUE);
	void GetTvRebarSupport_Plane_Slab(BOOL bUpperSlab, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngStt, CTwinVectorArray &tvArrAngEnd, CTwinVectorArray &tvArrRAngStt, CTwinVectorArray &tvArrRAngEnd, BOOL bUpper, long nDan, long nDrawLeft, BOOL bForDimRebarMain=FALSE);
	void GetTvRebarSupport_Plane_Slab_Std(BOOL bUpperSlab, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngStt, CTwinVectorArray &tvArrAngEnd, CTwinVectorArray &tvArrRAngStt, CTwinVectorArray &tvArrRAngEnd, BOOL bUpper, long nDan, long nDrawLeft, BOOL bForDimRebarMain=FALSE);
	void GetStaRebarSupport_VertSection_Slab(CDoubleArray &dArr, BOOL bUpper, long nDan, BOOL bUpperSlab=TRUE);
//	void GetXyArray_Plane_UpperSlab(CDPointArray &xyArr, CDoubleArray &dArrDist, double dSta);
	void GetDistRebarMain_CrossSection_Slab(BOOL bUpperSlab, CDoubleArray &dArr1, CDoubleArray &dArr2, CDoubleArray &dArr3, CDoubleArray &dArr4, CDoubleArray &dArrHunch1, CDoubleArray &dArrHunch2, CDoubleArray &dArrHunch3, CDoubleArray &dArrHunch4, double dSta, BOOL bUpper, long nDan, BOOL bForRightAngleRebar=TRUE, BOOL bCheckRebar=TRUE, BOOL bForArrangePlane=FALSE);
	void GetTvRebarMain_Plane_UpperSlab_TypeNormal(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, BOOL bUpper, long nCycle, long nDan, long nDrawLeft, long nIdx=-1, BOOL bHunch=FALSE, BOOL bSttBracket=FALSE, BOOL bEndBracket=FALSE, CTwinVectorArray &tvArrJoint=staticTvArr, CTwinVectorArray &tvArrHidden=staticTvArr, CTwinVectorArray &tvArrHiddenLeft=staticTvArr, CTwinVectorArray &tvArrHiddenRight=staticTvArr, BOOL bForDrawing=TRUE, CTwinVectorArray &tvArrAngLeftHalf=staticTvArr, CTwinVectorArray &tvArrAngRightHalf=staticTvArr, BOOL bCalDistRebarMain=TRUE);
	void GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, BOOL bUpper, long nCycle, long nDan, long nDrawLeft, long nIdx=-1, BOOL bHunch=FALSE, BOOL bSttBracket=FALSE, BOOL bEndBracket=FALSE, CTwinVectorArray &tvArrJoint=staticTvArr, CTwinVectorArray &tvArrHidden=staticTvArr, CTwinVectorArray &tvArrHiddenLeft=staticTvArr, CTwinVectorArray &tvArrHiddenRight=staticTvArr, BOOL bForDrawing=TRUE, CTwinVectorArray &tvArrAngLeftHalf=staticTvArr, CTwinVectorArray &tvArrAngRightHalf=staticTvArr, BOOL bCalDistRebarMain=TRUE);
	void GetTvRebarSupport_CrossSection_UpperSlabHunch(CTwinVectorArray &tvArr, double dSta, long nDan, long nDrawLeft, BOOL bForDrawCrossSection=TRUE, BOOL bExp=TRUE, BOOL bForRebarBM=FALSE, long nApplyAngle=0, BOOL bMirror=FALSE);
	void GetTvRebarShear_CrossSection_UpperSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArr2, double dSta, long nDrawLeft, BOOL bForMarkRebar=FALSE, BOOL bNotHunch=FALSE, BOOL bGagak=TRUE, BOOL bMirror=FALSE);
	void GetTvVirtualRebarGirder(long nJ, CTwinVector &tvUpper, CTwinVector &tvLower, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	void GetTvRebarMain_Plane_LowerSlab_TypeNormal(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, BOOL bUpper, long nCycle, long nDan, long nDrawLeft, long nIdx=-1, BOOL bHunch=FALSE, BOOL bSttPRF=FALSE, BOOL bEndPRF=FALSE, CTwinVectorArray &tvArrJoint=staticTvArr, CTwinVectorArray &tvArrHidden=staticTvArr, CTwinVectorArray &tvArrHiddenLeft=staticTvArr, CTwinVectorArray &tvArrHiddenRight=staticTvArr, BOOL bForDrawing=TRUE, CTwinVectorArray &tvArrAngLeftHalf=staticTvArr, CTwinVectorArray &tvArrAngRightHalf=staticTvArr, BOOL bCalDistRebarMain=TRUE);
	void GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, BOOL bUpper, long nCycle, long nDan, long nDrawLeft, long nIdx=-1, BOOL bHunch=FALSE, BOOL bSttPRF=FALSE, BOOL bEndPRF=FALSE, CTwinVectorArray &tvArrJoint=staticTvArr, CTwinVectorArray &tvArrHidden=staticTvArr, CTwinVectorArray &tvArrHiddenLeft=staticTvArr, CTwinVectorArray &tvArrHiddenRight=staticTvArr, BOOL bForDrawing=TRUE, CTwinVectorArray &tvArrAngLeftHalf=staticTvArr, CTwinVectorArray &tvArrAngRightHalf=staticTvArr, BOOL bCalDistRebarMain=TRUE);
	void GetTvRebarSupport_CrossSection_LowerSlabHunch(CTwinVectorArray &tvArr, double dSta, long nDan, long nDrawLeft, BOOL bForDrawCrossSection=TRUE, BOOL bExp=TRUE, BOOL bForRebarBM=FALSE, long nApplyAngle=0, BOOL bMirror=FALSE);
	void GetTvRebarShear_CrossSection_LowerSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArr2, double dSta, long nDrawLeft, BOOL bForMarkRebar=FALSE, BOOL bNotHunch=FALSE, BOOL bGagak=TRUE, BOOL bMirror=FALSE);
	double GetCTCRebarSupport_UpperSlabHunch();
	double GetCtcRebarMain(BOOL bForUpperSlab=FALSE);
	double GetCTCRebarShear_UpperSlab();
	double GetCTCRebarShear_LowerSlab();
//	double GetCTCRebarShear_OutWall(BOOL bStt);
//	double GetCTCRebarShear_Bracket(BOOL bStt);
	double GetCTCRebarShear_ArchRib();
	double GetCTCRebarShear_DanbuStirrup(BOOL bUpper, BOOL bStt);
	double GetCTCRebarSupport_OutWall(BOOL bStt, BOOL bInner, long nDan, BOOL bStd=FALSE);
	double GetRebarCtcWingwall(BOOL bStt, BOOL bLeft, long nSect, BOOL bFront, BOOL bMain, long nDan=0);
	double GetCTCRebarPRF_Main();
	double GetDiffStationSupportForVert();
	void SetDataDefaultRebarWingWallOption(BOOL bStt, BOOL bLeft);
	void SetDataDefaultRebarRoundAndCrossTie_VertSection_InWall(long nIdxInWall);
	void SetDataDefaultSetRebarDan(long nType);
	void SetDataDefaultRebarColumnStd(long nJ);
	void SetDataDefaultRebarMainInGirder_Girder(long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	void SetDataDefaultRebarGirder(long nJ, BOOL bDefaultByNum, BOOL bOnlyOrg=FALSE, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bApplyVert=FALSE, BOOL bUpperSlabGirder = TRUE);
//	void SetDataDefaultRebarShear_VertSection_InWall(long nIdxInWall);
	void SetDataDefaultRebarShear_VertSection_OutWall(BOOL bStt);
	void SetDataDefaultRebarShear_VertSection_Std(long nType, long nIdxFooting=0);
	void SetDataDefaultRebarShear_VertSection_UpperSlab(BOOL bLowerSlab=FALSE);
	void SetDataDefaultRebarSupport_VertSection_OutWall(BOOL bStt, BOOL bInner);
	void SetDataDefaultRebarSupport_VertSection_Std(long nType, long nIdxFooting=0);
	void SetDataDefaultRebarSupport_VertSection_UpperSlab(BOOL bUpper, BOOL bLowerSlab=FALSE);
	
	void SetDataDefaultRebarInfoCycle_Std(long nType, long nCycle, BOOL bOnlyCountArray=FALSE, long nDan=0, BOOL bOnlyDiffJoint=FALSE);
	void SetDataDefaultRebarInfoCycle_OutWall(long nCycle, BOOL bDefaultOutter=TRUE, BOOL bDefaultInner=TRUE, BOOL bOnlyCountArray=FALSE, long nDan=0, BOOL bOnlyDiffJoint=FALSE);
	void SetDataDefaultRebarInfoCycle_UpperSlab(long nCycle, BOOL bDefaultUpper=TRUE, BOOL bDefaultLower=TRUE, BOOL bOnlyCountArray=FALSE, long nDan=0, BOOL bOnlyDiffJoint=FALSE);
	void SetDataDefaultRebarInfoCycle_LowerSlab(long nCycle, BOOL bDefaultUpper=TRUE, BOOL bDefaultLower=TRUE, BOOL bOnlyCountArray=FALSE, long nDan=0, BOOL bOnlyDiffJoint=FALSE);

	// 2cycle 권고안
	void SetDataDefaultRebarInfoCycle_UpperSlab_Cycle2(long nCycle, BOOL bOnlyCountArray, BOOL bDefaultUpper=TRUE, BOOL bDefaultLower=TRUE, long nDan=0);
	void SetDataDefaultRebarInfoCycle_OutWall_Cycle2(long nCycle, BOOL bOnlyCountArray, long nDan=0);
	void SetDataDefaultRebarInfoCycle_LowerSlab_Cycle2(long nCycle, BOOL bOnlyCountArray, BOOL bDefaultUpper=TRUE, BOOL bDefaultLower=TRUE, long nDan=0);
	
	void GetXyRebarMain_CrossSection_SlabForPlacingMainRebar(BOOL bUpperSlab, CDPointArray &xyArr1, CDPointArray &xyArrHunch1, CDPointArray &xyArr2, CDPointArray &xyArrHunch2, CDPointArray &xyArr3, CDPointArray &xyArrHunch3, CDPointArray &xyArr4, CDPointArray &xyArrHunch4, double dSta, BOOL bUpper, long nDan, BOOL bForRightAngleRebar=FALSE, BOOL bCheckRebar=TRUE, BOOL bForArrangePlane=FALSE);
	void GetXyRebarMain_CrossSection_UpperSlab(CVectorArray &xyArr, CVectorArray &xyArrHunch, double dSta, BOOL bUpper, long nDan, long nCycle, BOOL bMirror=FALSE, BOOL bUseMatchSta=FALSE);
	void GetXyRebarMain_CrossSection_LowerSlab(CVectorArray &xyArr, CVectorArray &xyArrHunch, double dSta, BOOL bUpper, long nDan, long nCycle, BOOL bMirror=FALSE, BOOL bUseMatchSta=FALSE);
	void GetXyRebarSupport_SlabGirderInner(CDPointArray &xyArr, long nJ, long nUpper=-1, long nDan=-1, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	void GetxyRebarSupport_SlabGirder(CDPointArray &xyArr, CDPointArray &xyArrDirApplyScale, long nType, long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE, BOOL bForDraw=FALSE, double dScaleDom = 50.);
	void GetTvRebarMain_SlabGirder(CTwinVectorArray &tvArr, long nJ, long nUpper=-1, long nDan=-1, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	BOOL GetTvRebarStirrup_SlabGrider(CTwinVectorArray &tvArr, long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bForDraw=FALSE, BOOL bUpperSlabGirder = TRUE, double dScaleDom = 50.);
	long GetXyRebarSupport_VertSection_UpperSlabHunch(CDPointArray &xyArrLeft, CDPointArray &xyArrRight, long nIdxHunchRebar);
	void GetXyRebarSupport_VertSection_UpperSlabDanBu(CDPointArray &xyArr, BOOL bStt);
	void GetXyRebarSupport_VertSection_LowerSlabDanBu(CDPointArray &xyArr, BOOL bStt);
	long GetXyRebarSupport_VertSection_LowerSlabHunch(CDPointArray &xyArrLeft, CDPointArray &xyArrRight, long nIdxHunchRebar);
	CRebarInfoCycle* GetRebarInfoCycleUpperSlab_Hunch(long nCycle, long nIdx);
	CRebarInfoCycle* GetRebarInfoCycleLowerSlab_Hunch(long nCycle, long nIdx);
	void GetTvRebarSupport_CrossSection_UpperSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrLowerLeft, CTwinVectorArray &tvArrLowerRight, double dSta, BOOL bUpper, long nDan, long nDrawLeft, BOOL bForDrawCrossSection=TRUE, BOOL bForRebarBM=FALSE, BOOL bExp=TRUE, long nApplyAngle=0, BOOL bMirror=FALSE);
	void GetTvRebarSupport_CrossSection_LowerSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrLowerLeft, CTwinVectorArray &tvArrLowerRight, double dSta, BOOL bUpper, long nDan, long nDrawLeft, BOOL bForDrawCrossSection=TRUE, BOOL bForRebarBM=FALSE, BOOL bExp=TRUE, long nApplyAngle=0, BOOL bMirror=FALSE);
	void GetTvRebarShear_VertSection_UpperSlabDanbu(CTwinVectorArray &tvArr, BOOL bStt, long nCycle);
	void GetTvRebarShear_VertSection_LowerSlabDanbu(CTwinVectorArray &tvArr, BOOL bStt, long nCycle);

	void GetTvRebarShear_VertSection_OutWall(CTwinVectorArray &tvArr, BOOL bStt, long nVertDir=0);
	void GetTvRebarShear_VertSection_UpperSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrHucnh, CTwinVectorArray &tvArrRib, long nVertDir=0);
	void GetTvRebarShear_VertSection_LowerSlab(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrHucnh, long nVertDir=0);
	
	CRebarInfoCycle *GetRebarInfoCycleOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx);
	void GetXyRebarSupport_VertSection_OutWall(CDPointArray &xyArr, BOOL bStt, BOOL bInner, long nDan, long nVertDir=0, BOOL bAcute=FALSE, BOOL bLeft=TRUE);
	void GetXyRebarSupport_VertSection_UpperSlab(CDPointArray &xyArr, BOOL bUpper, long nDan, long nVertDir=0, BOOL bCalSupRebar=TRUE);
	void GetXyRebarSupport_VertSection_LowerSlab(CDPointArray &xyArr, BOOL bUpper, long nDan, long nVertDir=0, BOOL bCalSupRebar=TRUE);
	void SyncBridgeRebar(BOOL bInput=TRUE, long nVertDir=0, BOOL bMakeRebarInfoCycle=TRUE);
	CRcBridgeRebar();
	virtual ~CRcBridgeRebar();
	CRcBridgeRebar& operator=(const CRcBridgeRebar& Obj);
	void Serialize(CArchive &ar);

	// 주철근 배치 관련 함수 ----------------------------------------
	// rebar info cycle 생성 함수
	// 주철근 생성할때는 반드시 MakeRebarInfoCycleStd를 통해서 생성해야 철근 기호가 제대로 나온다.
	void MakeRebarInfoCycleStd(BOOL bInput, long nVertDir=0);					
	
	void SetConcDataGirder();
	void SetConcDataFooting(BOOL bReverse=FALSE, BOOL bCheckAdd=FALSE);
	void SetConcDataWall();
	void SetConcDataOutWall(CConcBasic *pData, const ETypeEnvComboLsd eTypeEnv, HGINT32 nIndex);
	void SetConcDataSlab();
	void SetConcDataPRF();
	void SetConcDataBasic(CConcBasic *pConc, EPartOfBridge ePos, BOOL UpAndFront, ELSDLoadGroupKind eGroupKind);
	void SetConcDataWingWall();
	BOOL SetDesignForceSlab_Usd(CConcBasic *pConc, long nSpan, long nPos, long nPosCheck, BOOL bUpperSlab, BOOL bInJijum);
	BOOL SetDesignForceSlab_Lsd(CConcBasic *pConc, long nSpan, long nPos, long nPosCheck, BOOL bUpperSlab, BOOL bInJijum, const ETypeEnvComboLsd eTypeEnv);
	void SetDesignForceWall_Usd(CConcBasic *pConc, long nJijum, long nPosCheck, BOOL bGirderCalc, BOOL bOutSideWall);
	void SetDesignForceWall_Lsd(CConcBasic *pConc, long nJijum, long nPosCheck, BOOL bGirderCalc, BOOL bOutSideWall, const ETypeEnvComboLsd eTypeEnv);
	BOOL SetDesignForcePRF_Usd(CConcBasic *pConc, BOOL bStt, long nPosCheck);
	BOOL SetDesignForcePRF_Lsd(CConcBasic *pConc, BOOL bStt, long nPosCheck, const ETypeEnvComboLsd eTypeEnv);

	CConcBasic* GetConcDataFooting(long nJijum, BOOL bFront, BOOL bReverse=FALSE, long nCheckAdd=-1, ETypeEnvComboLsd eTypeEnv=eEnvLimit);
	CConcBasic* GetConcDataInWall(long nIndex, BOOL bAllSection, ETypeEnvComboLsd eTypeEnv=eEnvLimit);
	CConcBasic* GetConcDataWall(long nJijum, long nPos, BOOL bAllSection, BOOL bCheckMomnet=FALSE);
	CConcBasic* GetConcDataWall(long nIndex, BOOL bAllSection, ETypeEnvComboLsd eTypeEnv=eEnvLimit, BOOL bCheckMomnet=FALSE);
	CConcBasic* GetConcDataSlab(long nJigan, long nPos, BOOL bUpperSlab=TRUE);
	CConcBasic* GetConcDataPRF(BOOL bStt);
	CConcBasic* GetConcDataWingWall(BOOL bStt, BOOL bLeft, long nIndex);
	double GetValueJoint(CRebarInfoCycle *pRCCur, double dDiaNext, CRebarInfoCycle *pRCNext=NULL, long nVertDir=0, BOOL bReverse=FALSE);
	double GetStationModifyForRebarCheck(double dSta);
	long GetIndexConcDataWall(long nJijum, long nPos, BOOL bAllSection);

	HGINT32 GetCountValidComboEnvType();
	HGINT32 GetCountConcFooting(HGBOOL bReverse, HGBOOL bCheckAdd) const;
	HGINT32 GetCountConcSlab() const;
	HGINT32 GetCountConcOutWall() const;
	HGINT32 GetCountConcInWall() const;
	HGINT32 GetCountConcPRF() const;
	HGINT32 GetCountConcUpperAndLowerSlab(BOOL bUpperSlab) const;
	

	CConcBasic* GetConcDataSlab_Lsd(HGINT32 ix, CString *pstrType=NULL);
	CConcBasic* GetConcDataWall_Lsd(HGINT32 ix, CString *pstrType=NULL, HGBOOL bAllSection=TRUE);
	CConcBasic* GetConcDataInWall_Lsd(HGINT32 ix, CString *pstrType=NULL, HGBOOL bAllSection=TRUE);
	CConcBasic* GetConcDataPRF_Lsd(HGINT32 ix, CString *pstrType=NULL);
//	CConcBasic* GetConcDataFooting_Lsd(HGINT32 ix, CString *pstrType=NULL);

public:
	void SetConcDataSlab(const ETypeEnvComboLsd eTypeEnv);
	void SetConcDataOutWall(const ETypeEnvComboLsd eTypeEnv);
	void SetConcDataInWall(const ETypeEnvComboLsd eTypeEnv);
	void SetConcDataPRF(const ETypeEnvComboLsd eTypeEnv);
private:
	double GetDistMoveJointByTypeDefaultCycle3(long nTypeDefaultCycle3, double dLenJoint);
	double GetDistMoveJointCycle13_UpperSlab(BOOL bUpper, BOOL bCycle3, long nJigan, long nJoint);
	double GetDistMoveJointCycle13_OutWall(BOOL bStt, BOOL bInner, BOOL bCycle3, long nJoint);
	double GetDistMoveJointCycle13_LowerSlab(BOOL bUpper, BOOL bCycle3, long nJigan, long nJoint);
	void CheckSerialize();
	void MakeRebarInfoCycle_UpperSlab(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// 상부 슬래브 RebarInfoCycle 만듬(cycle1,3).
	void MakeRebarInfoCycle_OutWall(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// 외측벽체 RebarInfoCycle 만듬(cycle1,3).
	void MakeRebarInfoCycle_UpperSlabHunch(long nCycle, BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_UpperDanbu(BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_LowerDanbu(BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_LowerSlab(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// 하부 슬래브 RebarInfoCycle 만듬(cycle1,3).
	void MakeRebarInfoCycle_LowerSlabHunch(long nCycle, BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_OutWallBoxType(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// 함형교 외측벽체 RebarInfoCycle 만듬(cycle1,3).
	void AddOrDeleteRebarInList(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, CRebar *pRB);

public:
	long GetCountRebarDanbu(BOOL bStt, long nCycle, BOOL bUpper=TRUE);
	long GetCountShearRebarBFooting();
	BOOL IsRebarMainBySta(BOOL bUpperSlab, double dSta, long nCycle, BOOL bUpper, long nDan, long &nIdxFind);
	BOOL IsSameRebarObtusAngle(BOOL bStt1, BOOL bLeft1, BOOL bStt2, BOOL bLeft2);
	void GetOwnerRebarSupportObtuseAngle(BOOL bStt, BOOL bLeft, BOOL &bSttOwner, BOOL &bLeftOwner);
	CRebarInfoCycle* GetOwnerRebarInfoCycleObtuseAngle(BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bLowSlab = FALSE);
	CRebarInfoCycle* GetOwnerRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nIdx);
	CRebarInfoCycle* GetOwerRebarInfoCycleSlab(BOOL bUpperSlab, long nCycle, long nDan, BOOL bUpper, long nIdx);
	BOOL IsCoupler(CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	void AddCoupler(CDPointArray &xyArr, long nEa, double dDia);
	void CalcCouplerRebar(CDPointArray &xyArrRoundRebar, CDPointArray &xyArrMainRebar, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	void SaveCountJointMainRebar();
	long GetDanMainRebarUpperSlabExp(long nCycle, BOOL bUpper, BOOL bStt);
	void SetDataDefaultGirderStirrupFirst();
	
	void Clear23DanMainRebar();
	double GetDiffLeftRightDistForGagakSabogang(BOOL bStt, BOOL bLeft);
	
	CString GetStringDiaMu(double dDia);
	BOOL IsFlexibleJointRebar(BOOL bStt, CRebarInfoCycle *pRC);
	BOOL IsObtuseEdgeOfSlab(BOOL bStt, BOOL bLeft);
	BOOL IsObtuseEdgeOfGagak(BOOL bStt, BOOL bLeft);
	BOOL IsNeedGagakRebar(BOOL bStt, BOOL bLeft);
	BOOL IsExistMainRebarUpperSlabExp(BOOL bStt, BOOL bLeft, BOOL bUpper, long nCycle);
	void GetTvRebarShear_Plane_Slab_Real(BOOL bUpperslab, CTwinVectorArray &tvArr);
	CTwinVector ConvertFromVertialRebarToLineRebarAtSabogang(CTwinVector tvRebar, CDPoint vAngStt, CDPoint vAngEnd, BOOL bLeft);
	void GetTvRebarStirrup_VertSection_ConnectSection(CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight);
	double GetDistMoveJointCycle13_ArchRib(BOOL bUpper, BOOL bCycle3, long nJoint);
	void SetDataDefaultCountJointMainArchRib();
	void SetDataDefaultRebarInfoCycleArchRib(long nCycle, BOOL bOnlyCountArray, BOOL bOnlyDiffJoint=FALSE);
	void SetDataDefaultCountJointMainRebarInWall();
	void SetDataDefaultRebarJewon_ArchRib_Shear();
	void SetDataDefaultRebarJewon_ArchRib_Support();
	void GetInfoCuttingCircle_Plane_ArchRib_Shear(CDPoint &xyCen, double &dR);
	void GetTvRebarShear_Plane_ArchRib(CTwinVectorArray &tvArr, long nDrawLeft);
	long GetCountDanMainRebar_ArchRib(BOOL bUpper);
	void GetInfoCuttingCircle_Plane_ArchRib_ForDan(CDPoint &xyCen, double &dR, double &dAngStt, double &dAngEnd, BOOL bUpper, long nDan, long nDrawLeft);
	void GetInfoCuttingCircle_Plane_ArchRib(CDPoint &xyCen, double &dR, double &dAngStt, double &dAngEnd, long nDrawLeft);
	void GetTvRebarShear_CrossSection_ArchRib(CTwinVectorArray &tvArr, double dXDisOfUpper, long nDrawLeft);
	CRebarInfoCycle* GetRebarInfoCycleArchRibByDist(long nCycle, long nDan, BOOL bUpper, double dXDisOfUpper);
	long GetCountDanMainRebar_ArchRib(double dXDisOfUpper, BOOL bUpper);
	CRebarInfoCycle* GetXyRebarMain_CrossSection_ArchRib(CDPointArray &xyArr, double dXDisOfUpper, long nCycle, BOOL bUpper, long nDan, long nDrawLeft);
	CTwinVector GetTvOffset_ArchRib(double dXDisOfUpper, BOOL bUpper, long nDan, long nDrawLeft);
//	CDPoint GetXyRebarMain_ArchRib(BOOL bUpper, BOOL bFirst);
	void GetTvRebarSupport_Plane_ArchRib(CTwinVectorArray &tvArr, BOOL bUpper, long nDan, long nDrawLeft);
	CRebarInfoCycle* GetRebarInfoCycleArchRib(long nCycle, long nDan, BOOL bUpper, long nIdx);
	void GetDistArrayMainRebarArchRib(CDoubleArray &dArrDist, long nCycle, long nDrawLeft);
	void GetTvRebarMain_Plane_ArchRib(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrJoint, long nCycle, BOOL bUpper, long nDan, long nIdx, long nDrawLeft);
	void SetDataDefaultRebarJewonArchRib();
	void SetDataDefaultRebarShear_VertSection_ArchRib();
	void SetDataDefaultRebarSupport_VertSection_ArchRib();
	void GetTvRebarShear_VertSection_ArchRib(CTwinVectorArray &tvArrL, CTwinVectorArray &tvArrR, long nVertDir);
	void GetRebarRib(CParabola &pb, BOOL bUpper, long nDan);
	void GetXyRebarSupport_VertSection_ArchRib(CDPointArray &xyArrL, CDPointArray &xyArrDirL, CDPointArray &xyArrR, CDPointArray &xyArrDirR, BOOL bUpper, long nDan, long nVertDir, BOOL bApplyDir=TRUE);
	void MakeRebarInfoCycle_ArchRib(long nCycle, long nDan, BOOL bUpper, BOOL bInput, long nVertDir);
	double m_dGapJoint;
	BOOL IsExistMainRebarRib(long nCycle, long nDan, BOOL bUpper, long nIdx);
	double GetDistMainRebarRib(long nCycle, long nDan, BOOL bUpper, long nIdx, BOOL bAccumulation);
	long GetDanMainRebarRib(long nCycle, BOOL bUpper);
	double GetDiaMainRebarRib(long nCycle, long nDan, BOOL bUpper, long nIdx);
	void SetDataDefaultRebarJewonOutWall_AcuteAngleSupportRebar();
	void SetDataDefaultRebarJewon_Slab_ObtuseSupportRebar(BOOL bUpperSlab);
	void SetDataDefaultRebarJewonOutWall_AcuteAngle();
	void GetTvRebarSupport_CrossSection_OutWall_Acute(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngle, BOOL bStt, BOOL bLeft);
	void GetTvRebarMain_Side_OutWall_Acute(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	void GetXyRebarSupport_VertSection_OutWallAcuteAngle(CDPointArray &xyArr, BOOL bStt, BOOL bLeft, long nVertDir);
	CTwinVector GetTvOffset_OutWall_AcuteAngle(BOOL bStt, BOOL bLeft, BOOL bVertDir);
	void MakeRebarInfoCycle_ACuteAngle_OutWall(BOOL bInput, long nVertDir);
	void setDataDefaultRebarAcuteAngle();
	void GetXyRebarMain_VertSection_SlabObtuse(CDPointArray &xyArrUpperSlabUpper, CDPointArray &xyArrUpperSlabSide, CDPointArray &xyArrUpperSlabLower, BOOL bStt, BOOL bLeft, BOOL bLowerSlab = FALSE);
	void SetDataDefaultRebarJewon_Slab_Obtuse(BOOL bUpperSlab);
	void GetTvArrPlane_UpperSlab_Obtuse(CTwinVectorArray &tvArrMainRebar, CTwinVectorArray &tvArrSupportRebar, BOOL bStt, BOOL bLeft, long nDan, BOOL bLowerSlab = FALSE);
	void GetTvRebarMain_Plane_Slab_Obtuse(CTwinVectorArrayArray *pTvArrMainRebar, CTwinVectorArray &tvArrSupportRebar, BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bLowerSlab = FALSE);
	CTwinVector GetTvOffset_ObtuseAngle_Slab(BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bVertDir, BOOL bUpperSlab);
	void MakeRebarInfoCycle_ObtuseAngle_Slab(BOOL bInput, long nVertDir, BOOL bUpper);
	void SetDataDefaultRebarObtuseAngle();
	void SetDataDefaultWidthGirder(long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder,  BOOL bUpperSlabGirder = TRUE);
// 	void SetDataDefaultGirderWidth(long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder,  BOOL bUpperSlabGirder = TRUE);
	
	void SetDataDefaultRebarJewon_GirderJong_All();
	void GetTvRebarStirrup_Plane_UpperSlabGirderJong(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, BOOL bGagak);
	void GetTvRebarSupport_Plane_UpperSlabGirderJong(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bUpper, BOOL bConvertLineInfoToTvArr=FALSE);
	void SetDataDefaultYPosOfGirder(long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder,  BOOL bUpperSlabGirder = TRUE);
	CGirderSectionRebar* GetGirderRebarJong(BOOL bStt, BOOL bLeft, BOOL bGagak);
	BOOL IsGirderRebarJong(BOOL bStt, BOOL bLeft, BOOL bGagak);
	double GetGirderWidth(long nJijum, BOOL bUpperSlabGirder = TRUE);
	double GetDiaRebarSupportOutWallHunch(BOOL bStt);
	double GetDiaRebarMainOutWallHunch(long nCycle, BOOL bStt);
	void GetTvRebarSupport_CrossSection_OutWallHunch(CTwinVectorArray &tvArr, BOOL bStt);
	void GetXyRebarSupport_VertSection_OutWallHunch(CDPointArray &xyArr, BOOL bStt, long nVertDir);
	void GetTvRebarMain_Side_OutWallHunch(CTwinVectorArray &tvArr, long nCycle, BOOL bStt);
	void MakeRebarInfoCycle_OutWall_Hunch(long nCycle, BOOL bInput, long nVertDir);
	BOOL IsHunchRebarPlane(double dSta, BOOL bLowerSlab=FALSE);
	void ConvertPlaneLineToTv(CTwinVectorArray &tvArr);
	void GetTvArrUpperSlabByDrawLeft_SupportRebar(CTwinVectorArray &tvArr, long nDrawLeft);
	void SettingTvArrRebarSupportPlane();
	void SettingTvArrRebarMainPlane();
	void SetDataDefaultRangeSupportRebarForSameUpperSlab(long nJ);
	BOOL IsJointMainRebar_OutWall_Cycle2(BOOL bStt, BOOL bInner);
	void SettingVertSection();
	double GetLengthBynVertDir(double dLen, long nVertDir);
	void SettingAngleInfoAtRebarInfoCycle_UpperSlab(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nJ, long nIdxStt, long nIdxEnd, BOOL bVertDir);
	double GetLengthExt23DanRebarOutWall(BOOL bStt, BOOL bInner);
	double GetLengthExt23DanRebarUpperSlab(BOOL bUpper, long nIdx);
	double GetLengthExt23DanRebarLowerSlab(BOOL bUpper, long nIdx);
	void SyncRebar2CycleMainRebarOldAndNew(long nType=-1, BOOL bDefaultUpper=TRUE, BOOL bDefaultLower=TRUE);
	void ApplyExtLength23DanMainRebar_UpperSlab(CRebarInfoCycle* pRC, double dLengthExt23DanRebar);
	void ApplyExtLength23DanMainRebar_OutWall(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, double dLengthExt23DanRebar);
	void ApplyExtLength23DanMainRebar_LowerSlab(CRebarInfoCycle* pRC, double dLengthExt23DanRebar);
	void SetDataDefaultCountJoint(long nCycle=-1);
	void SetDataDefaultRebarShear_VertSection_Bracket();
	BOOL m_bMakedRebarInfoCycleByVert;	// 현재 주철근이 사거리로 만들어 졌는지 직거리로 만들어 졌는지
	BOOL m_bMakedRebarInfoCycleByVertUpperSlab;		// 상부슬래브 주철근이 직거리인지, 사거리인지
	BOOL SetDiaMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx, double dDia, BOOL bMax=FALSE);
	BOOL SetDiaMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, double dDia, BOOL bMax=FALSE);
	BOOL SetDiaMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, double dDia, BOOL bMax=FALSE);
	BOOL SetDiaMainRebarArchRib(long nCycle, long nDan, long nIdx, double dDia);
	double GetDiaRebarStirrupDanbu(BOOL bStt, long nIdx, BOOL bUpper);
	void SetDataDefaultRebarMain_Front_Footing(long nJ, BOOL bMatchWithWall);
	void GetXyRebarMain_CrossSection_SlabExp(CDPointArray &xyArrCycle1, CDPointArray &xyArrCycle2, BOOL bUpperSlab, BOOL bStt, BOOL bLeft, BOOL bUpper, long nDan);
	void GetInfoMainRebarExp_OutWall(BOOL bStt, BOOL bLeft, BOOL bInner, long nCycle, long nIdx, CString &sMark, double &dDia, CString &sDescription, BOOL bIndexWall=TRUE);
	void GetInfoMainRebarExp_UpperSlab(BOOL bStt, BOOL bLeft, BOOL bUpper, long nCycle, long nDan, long nType, CString &sMark, double &dDia, CString &sDescription);
	void GetInfoMainRebarExp_UpperSlabHunch(BOOL bStt, BOOL bLeft, long nCycle, CString &sMark, double &dDia, CString &sDescription);
	void GetInfoMainRebarExp_LowerSlab(BOOL bStt, BOOL bLeft, BOOL bUpper, long nCycle, long nDan, long nType, CString &sMark, double &dDia, CString &sDescription);
	void GetInfoMainRebarExp_LowerSlabHunch(BOOL bStt, BOOL bLeft, long nCycle, CString &sMark, double &dDia, CString &sDescription);
	void GetTvMainRebarOrgOfExp(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	void GetTvRebarMain_CrossSection_OutWallExp(CTwinVectorArray &tvArrCycle1, CTwinVectorArray &tvArrCycle2, CTwinVectorArray &tvArrCycle12, CTwinVectorArray &tvArrCycle22, CTwinVectorArray &tvArrJoint, BOOL bStt, BOOL bLeftWall, BOOL bInner, long nDan, long nIdx, BOOL bLeft);
	void GetDistRebarMain_CrossSection_OutWallExp(CDoubleArray &dArrCycle1, CDoubleArray &dArrCycle2, CDoubleArray &dArrCycle12, CDoubleArray &dArrCycle22, CDoubleArray &dArrCycle1Inner, CDoubleArray &dArrCycle2Inner, BOOL bStt, BOOL bLeft, long nDan, BOOL bUpper);
	double GetDiaMainRebarGagak(BOOL bStt, BOOL bLeft);
	BOOL IsMainRebarJointUpperSlabAndUpperSlab(CRebarInfoCycle *pRC);
	BOOL IsMainRebarJointUpperSlabAndOutWall(CRebarInfoCycle *pRC, BOOL &bStt);
	BOOL IsMainRebarJointLowerSlabAndLowerSlab(CRebarInfoCycle *pRC);
	BOOL IsMainRebarJointLowerSlabAndOutWall(CRebarInfoCycle *pRC, BOOL &bStt);
	BOOL IsEnableJointMainRebar_OutWallOutterAndUpperSlabUpper(BOOL bStt, BOOL bCycle13);
	double GetCTCRebarSupport_Slab(BOOL bUpperSlab, BOOL bUpper, long nDan, BOOL bStd=FALSE);
	double GetExtMainRebarLimitGagak(BOOL bStt, BOOL bLeft);
	
	void GetTvRebarShear_Plane_UpperSlab_Vert(CTwinVectorArray &tvArr, BOOL bForBM, BOOL bNormal, BOOL bHunch, BOOL bRib);
	double GetDistDiffOfMidInVert();
//	void GetTvRebarMain_Plane_UpperSlab_Vertical(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, BOOL bUpper, long nCycle, long nDan, long nDrawLeft, long nIdx, BOOL bHunch, BOOL bSttBracket, BOOL bEndBracket, CTwinVectorArray &tvArrJoint, CTwinVectorArray &tvArrHidden, CTwinVectorArray &tvArrHiddenLeft, CTwinVectorArray &tvArrHiddenRight, BOOL bForDrawing);
	void SetDataDefaultRebarBracket();
	void SetDataDefaultRebarBFooting();
	BOOL IsBracketRebar(BOOL bStt, long nCycle);
	void ApplyAngleToOffsetInfo(CDPointArray &xyArrOffset, double dCurAngle, double dApplyAngle);
	CDPoint GetXyDirRebarMainPlaneForSabogang(long nLeft);
	void GetTvRebarMain_Plane_Slab_Exp(BOOL bUpperSlab, CTwinVectorArrayArray *pTvArrCycle1, CTwinVectorArrayArray *pTvArrCycle2, CTwinVectorArrayArray *pTvArrCycle12, CTwinVectorArrayArray *pTvArrCycle22, BOOL bStt, BOOL bLeft, long nDan, BOOL bUpper, CTwinVectorArray &tvArrCycleHunch1=staticTvArr, CTwinVectorArray &tvArrCycleHunch2=staticTvArr, BOOL bJongGirder=FALSE, CDPointArray &xyArrCycle1=staticPtArr, CDPointArray &xyArrCycle2=staticPtArr, CDPointArray &xyArrDimPoint=staticPtArr);
	void GetTvRebarSupport_CrossSection_UpperSlabGirder(CTwinVectorArray &tvArr, long nJ);
	void GetTvRebarSupport_CrossSection_LowerSlabGirder(CTwinVectorArray &tvArr, long nJ, double dStation, CDPoint vAng, long nDrawLeft);
	void GetTvRebarShear_CrossSection_UpperSlabGirder(CTwinVectorArray &tvArr, long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	void GetTvRebarShear_CrossSection_LowerSlabGirder(CTwinVectorArray &tvArr, long nJ, double dStation, long nDrawLeft, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE);
	CDPoint GetXyVertSectionShoe(long nJ, BOOL bLeftOfJijum);
	long GetTypeMainRebarSlabUpperHunch(long nJ, BOOL bLowerSlab=FALSE);
	void GetTvRebarMain_Plane_SlabGirder(BOOL bUpperSlab, CTwinVectorArray &tvArr, long nJ, long nDrawLeft);
	void GetTvRebarMain_Plane_Slab_Ortho(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrHidden, long nCycle, BOOL bBracket, BOOL bSttBracket, BOOL bGirder, long nIdxGirder, BOOL bSttDanbu, BOOL bEndDanbu, long nDanDanbu, BOOL bForDrawing, BOOL bUpperSlabGirder = TRUE);
	void SetDataDefaultDistLeftDirPlacing(BOOL bHunch=FALSE);
//	void GetTvRebarStirrup_Plane_Girder(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, long nJ, long nDrawLeft, BOOL bUpperSlabGirder);
	BOOL IsGirderRebar(long nJ, BOOL bUpperSlab);
	long GetDirSupportRebar(double dSta, BOOL bUpper);
	void SetDataDefaultDistJijumDirPlacing();
	void GetTvCuttingArea_Plane_SlabHunch(CTwinVectorArray &tvArr, long nJ, BOOL bUpperSlab=TRUE);
	void SetDataDefaultRebarJewonSlabHunch(BOOL bUpper);
	void SetConcData(BOOL bSlab, BOOL bWall, BOOL bFooting, BOOL bWingWall, BOOL bPRF);
	CDPoint GetInfoMainRebar_Wall(long nJ, long nCycle, long nDan, long nUpper, BOOL bInner);
	CDPoint GetInfoMainRebar_Footing(long nJ, BOOL bUpper, long nCycle, long nDan, BOOL bLeft);
	CDPoint GetInfoMainRebar_UpperSlab(BOOL bUpper, long nSpan, long nCycle, long nDan, long nLeft);
	CDPoint GetInfoMainRebar_LowerSlab(BOOL bLower, long nSpan, long nCycle, long nDan, long nLeft);
	CDPoint GetInfoMainRebar_ArchRib(BOOL bUpper, long nSpan, long nCycle, long nDan, long nLeft, BOOL bCrown=FALSE);
	CDPoint GetInfoSupportRebar_UpperSlab(BOOL bUpper, long nSpan, long nDan, long nLeft);
	CDPoint GetInfoSupportRebar_LowerSlab(BOOL bLower, long nSpan, long nDan, long nLeft);
	CDPoint GetInfoSupportRebar_Wall(BOOL bInner, long nJ, long nDan, long nLeft);
	CDPoint GetInfoSupportRebar_ArchRib(BOOL bUpper, long nSpan, long nDan, long nLeft, BOOL bCrown=FALSE);
	CDPoint GetInfoShearRebar_UpperSlab(long nSpan, long nLeft);
	CDPoint GetInfoShearRebar_LowerSlab(long nSpan, long nLeft);
	CDPoint GetInfoShearRebar_Wall(long nJ, long nLeft);
	CDPoint GetInfoShearRebar_ArchRib(long nSpan, long nLeft, BOOL bCrown=FALSE);
	BOOL IsVertDir();
	void MakeLineBaseVirtual();
	void SyncCycleMainRebar(long nCycleSrc, long nCycleDest);
	void GetStaRebarSupport_UpperSlabBracket(CDoubleArray &dArrSta, BOOL bStt, long nVertDir=0);
	void GetTvRebarSupport_Plane_UpperSlabBracket(CTwinVectorArray &tvArr, BOOL bStt, long nVertDir=0);
//	void GetTvRebarMain_Plane_UpperSlabDanBu(CTwinVectorArray &tvArr, BOOL bStt, long nDan, long nCycle);
	void GetTvRebarMain_Plane_UpperSlabBracket(CTwinVectorArray &tvArr, BOOL bStt, long nCycle);
	void GetTvCuttingArea_Plane_UpperSlabBracket(CTwinVectorArray &tvArr);
	void GetTvCuttingArea_Plane_LowerSlabPRF(CTwinVectorArray &tvArr);
	void GetTvRebarMain_Plane_LowerSlabPRF(CTwinVectorArray &tvArr, BOOL bStt, long nCycle);
	void GetTvRebarSupport_Plane_LowerSlabPRF(CTwinVectorArray &tvArr, BOOL bStt, long nVertDir=0);
	void SetDataDefaultRebarJewon_Bracket_All();
	void MakeRebarInfoCycle_OutWall_Bracket(long nVertDir=0);
	void GetXyRebarSupport_VertSection_Bracket(CDPointArray &xyArr, BOOL bStt);
	void GetTvRebarShear_VertSection_Bracket(CTwinVectorArray &tvArr, BOOL bStt);
	void MakeRebarInfoCycle_BFooting(long nVertDir=0);
	void MakeRebarInfoCycle_FootingAll(BOOL bInput=FALSE, long nVertDir=0);
	long GetXyRebarSupport_VertSection_BFooting(CDPointArray &xyArr, BOOL bStt, double dDomScale, BOOL bOnlyLower=FALSE);
	long GetTvRebarShear_VertSection_BFooting(CTwinVectorArray &tvArr, BOOL bStt);
	void SetDataDefaultRebarJewonColumnAndGirderStd();
	void SetDataDefaultRebarJewon_Girder_All(BOOL bUpperSlabGirder = TRUE);
//	void GetXyDirRebarSupport_Plane_UpperSlab(CDPointArray &xyArr, BOOL bUpper, BOOL bLeft);
	void GetTvCuttingArea_UpperSlab_Shear(CTwinVectorArray &tvArr, long nDrawLeft);
	double GetLenShearUpperSlab();
//	double GetLenShearLowerSlab();
//	double GetLenShearOutWall(BOOL bStt);
//	double GetLenShearBracket(BOOL bStt);
	double GetLenShearArchRib();
//	double GetLenShearDanbuStirrup(BOOL bUpper, BOOL bStt);
//	void GetTvRebarShear_CrossSection_OutWall(CTwinVectorArray &tvArr, BOOL bStt);
	CString GetMarkRebarShear(long nType, BOOL bInput, BOOL bForBM, BOOL bOnlyInit, CString sDescription=_T(""));
	CString GetMarkRebarSupport(long nType, BOOL bInput, BOOL bForBM, BOOL bOnlyInit, CString sDescription=_T(""));
	CString GetMarkRebarShearArchRib(long nType, BOOL bInput, BOOL bForBM, BOOL bOnlyInit, CString sDescription=_T(""));
	CString GetMarkRebarSupportArchRib(long nType, BOOL bInput, BOOL bForBM, BOOL bOnlyInit, CString sDescription=_T(""));
	CString GetMarkRebarEtc(long nType, BOOL bInput, BOOL bForBM, BOOL bOnlyInit, CString sDescription=_T(""), BOOL bGirder=FALSE);
	CString GetMarkRebarMain(long nType, BOOL bInput, long nCycle, BOOL bOnlyInit=FALSE, BOOL bForBM=TRUE, CString sDecription=_T(""), long nDan=0);
	CString GetMarkRebarDanbu(BOOL bOnlyInit);
	void GetTv_CrossSection_LowerSlab(CTwinVectorArray &tvArr, long nJ, double dStation, CDPoint vAng, long nDrawLeft);

	CRebar* GetRebarShearByDescription(CString sDescription);
	CRebar* GetRebarSupportByDescription(CString sDescription);
	CRebar* GetRebarEtcByDescription(CString sDescription, BOOL bGirder);
	CRebar* GetRebarMainByDescription(CString sDescription);

	// 이음 개수 가져오는 함수
	long GetCountJointMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner);
	long GetCountJointMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan);
	long GetCountJointMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan);

	// 철근 배치 단수 가져 오는 함수
	long GetDanMainRebarOutWall(long nCycle, BOOL bStt, BOOL bInner);
	long GetDanMainRebarUpperSlab(long nCycle, BOOL bUpper, long nIdx);
	long GetDanMainRebarLowerSlab(long nCycle, BOOL bUpper, long nIdx);
	
	// 이음 간격 가져오는 함수
	double GetDistMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx, BOOL bAccumulation);
	double GetDistMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, BOOL bAccumulation, long nVertDir=0);
	double GetDistMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, BOOL bAccumulation, long nVertDir=0);

	// 주철근 지름 가져오는 함수
	double GetDiaMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx);
	double GetDiaMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	double GetDiaMainRebarUpperSlabHunch(long nCycle, long nJijum);
	double GetDiaMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	double GetDiaMainRebarLowerSlabHunch(long nCycle, long nJijum);
	
	// 노출등급 직경
	double GetDiaLsdExposure_Main(EPartOfBridge ePos, BOOL bUpperFront); //최소피복두께(부착)
	double GetDiaLsdExposure_Shear(EPartOfBridge ePos);

	//주철근 지름 셋팅하는 함수
	BOOL SetDiaMainRebarUpperSlabHunch(long nCycle, long nJijum, double dDia);
	BOOL SetDiaMainRebarLowerSlabHunch(long nCycle, long nJijum, double dDia);

	// 철근이 연결되었는지 판단하는 함수
	BOOL IsJointMainRebarUpperSlab_Lower_Cycle2(double dSta);
	BOOL IsJointMainRebarUpperSlab_Lower_Cycle2(long nJijum);
	BOOL IsJointMainRebarUpperSlab_Upper_Cycle2(long nJigan);

	BOOL IsJointMainRebarLowerSlab_Upper_Cycle2(double dSta);
	BOOL IsJointMainRebarLowerSlab_Upper_Cycle2(long nJijum);
	BOOL IsJointMainRebarLowerSlab_Lower_Cycle2(long nJigan);

	// 주철근 들어가는 기준철근 가져오는 함수
	CTwinVector GetTvOffset_OutWall(long nDan, BOOL bStt, BOOL bInner, BOOL bVertDir);
	CTwinVector GetTvOffset_UpperSlab(long nDan, BOOL bUpper, double dSttSta=-1, double dEndSta=-1, BOOL bVertDir=FALSE, BOOL bApplyFlexibleJoint=TRUE);
	CTwinVector GetTvOffset_LowerSlab(long nDan, BOOL bUpper, double dSttSta=-1, double dEndSta=-1, BOOL bVertDir=FALSE);

	// RebarInfoCycle 가져오는 함수
	CRebarInfoCycle *GetRebarInfoCycleUpperSlab(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleUpperSlabCheckExist(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleLowerSlab(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleLowerSlabCheckExist(long nCycle, long nDan, BOOL bUpper, long nIdx);

	// rebarinfocycle 배열에 해당 문자로 마크스트링을 설정해준다.
	void SetStringMarkToRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nType, long nCycle, BOOL bInput, BOOL bInit=TRUE, long nDan=0);
	void SetStringDescriptionRebarInfoCycle_UpperSlab(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_ArchRib(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_UpperSlabHunch(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle);
	void SetStringDescriptionRebarInfoCycle_OutWall(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bStt, BOOL bInner, long nDan, long nCycle);
	void SetStringDescriptionRebarInfoCycle_LowerSlab(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_LowerSlabHunch(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle);

	

	// 이음 정보 생성하는 함수(여기서는 pArrJoint에 new를 사용해서 할당하므로 
	void MakeJointMainRebarOutWall(CJointArray &arrJoint, long nCycle, long nDan, BOOL bStt, BOOL bInner);
	void MakeJointMainRebarUpperSlab(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper);
	void MakeJointMainRebarLowerSlab(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper);
	void MakeJointMainRebarRib(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper, CParabola *pPb=NULL);

	// 기타 용도 함수
	double GetLenRebarBeforeEdge_UpperSlab_Cycle13(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nVertDir);
	double GetLenRebarAtEdge(double dDia, BOOL bInFooting=FALSE, long nVertDir=0, BOOL bUpperSlab=FALSE, BOOL bBending=FALSE);
	double GetLenRebarBeforeEdge_LowerSlab_Cycle13(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nVertDir);
	CTwinVector GetTvCornerBaseLine(HGBOOL bStt, HGBOOL bUpper);
	// 주철근 배치 관련 함수 ----------------------------------------

	// 주철근 배치 ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	// 철근 배치 옵션 관련
	BOOL m_bRoundAtEdgeRebar;		// 헌치부 철근을 라운드 적용할지?

	// 철근 배치 간격 index
	long m_nIdxCTC_MainRebar;
	double m_dCTC_MainRebar;	// index방식에서 double형으로 변경 index는 사용하지 않음(2005.04.25)

	// 인장, 압축구간을 동일간격으로 배치
	BOOL m_bSameOffsetPlaceTAndCBlock;	
	
	// 이음 개수
	long m_nCountJointMainRebar_UpperSlab_Cycle1[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 상부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_UpperSlab_Cycle2[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 상부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_LowerSlab_Cycle1[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 하부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_LowerSlab_Cycle2[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 하부슬래브 상,하면 각 지간별 이음 개수

	long m_nCountJointMainRebar_OutWall_Cycle1[2][2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_OutWall_Cycle2[2][2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_ArchRib_Cycle1[2][MAX_REBAR_DAN];	// 리브 상하면,1~3단 이음 개수(리브만 특별히 각 단에 대해서 이음을 입력받음)
	long m_nCountJointMainRebar_ArchRib_Cycle2[2][MAX_REBAR_DAN];	// 리브 상하면,1~3단 이음 개수(리브만 특별히 각 단에 대해서 이음을 입력받음)

	// 이음 개수(예전꺼 보관용) - 이음 개수 변경시 변경된 부분만 권고안을 해야 하기 때문에(Seralize는 하지 않음) //////
	long m_nCountJointMainRebar_UpperSlab_Cycle1_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 상부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_UpperSlab_Cycle2_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 상부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_LowerSlab_Cycle1_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 하부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_LowerSlab_Cycle2_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// 하부슬래브 상,하면 각 지간별 이음 개수
	long m_nCountJointMainRebar_OutWall_Cycle1_Old[2][2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_OutWall_Cycle2_Old[2][2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_ArchRib_Cycle1_Old[2][MAX_REBAR_DAN];	// 리브 상하면,1~3단 이음 개수(리브만 특별히 각 단에 대해서 이음을 입력받음)
	long m_nCountJointMainRebar_ArchRib_Cycle2_Old[2][MAX_REBAR_DAN];	// 리브 상하면,1~3단 이음 개수(리브만 특별히 각 단에 대해서 이음을 입력받음)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 전단철근 타입 설정
	CRebarShearDetail m_RebarShearDetail_UpperSlab;		// 상부슬래브
	CRebarShearDetail m_RebarShearDetail_LowerSlab;		// 하부슬래브
	CRebarShearDetail m_RebarShearDetail_OutWall[2];	// 시종점 외측벽체
	CRebarShearDetail m_RebarShearDetail_InWall;		// 중간벽체
	CRebarShearDetail m_RebarShearDetail_FootingLean;	// 직접기초
	CRebarShearDetail m_RebarShearDetail_FootingPile;	// 말뚝기초
	CRebarShearDetail m_RebarShearDetail_ArchRib;			// 리브
	CRebarShearDetail m_RebarShearDetail_Bracket[2];	// 브라켓

	// 날개벽 배근
	void GetXyRebarWingWallVaryPlane(CDPoint *xy, BOOL bStt, BOOL bLeft);
	void GetXyRebarWingWallVarySect(CDPoint *xy, BOOL bStt, BOOL bLeft, long nDan=0);
	double GetWingWallVertRebarCurveRadius(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallHunchWallPlane(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, BOOL bUpper);
	void GetTvArrayRebarWingWallHunchFootingSect(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	double GetHeightWingWallHorRebar(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetDimArrayRebarWingWallUpper(CDPointArray &xyArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetDimArrayRebarWingWallSttSide(CDPointArray &xyArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetDimArrayRebarWingWallMaxSide(CDPointArray &xyArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetDimArrayRebarWingWallMinSide(CDPointArray &xyArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	BOOL IsMergedRebarWingWall(BOOL bStt, BOOL bLeft, int nOut, int nDan, char cFirBlock, char cSecBlock, BOOL bReal=TRUE);
	void GetTvArrayRebarWingWallHunchWallFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	void GetTvArrayRebarWingWallHunchFootingFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft);
	void GetLengthArrayShearFront(CDoubleArray &dArr, char cBlock, BOOL bStt, BOOL bLeft);
	void GetTvArrayRebarWingWallShearAFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut);
	void GetTvArrayRebarWingWallShearBCFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut);
	void GetTvArrayRebarWingWallShearDFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut);
	void GetTvArrayRebarWingWallBlockAMainFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallBlockASupportFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallBlockBMainFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallBlockCMainFront(CTwinVectorArray &tvArrCurve, CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallBlockDMainFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void GetTvArrayRebarWingWallBlockDSupportFront(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	CDPoint GetXyRebarWingWallTailUpperCross(BOOL bStt, BOOL bLeft, int nOut);
	CDPoint GetXyRebarWingWallTailLowerCross(BOOL bStt, BOOL bLeft, int nOut);
	CTwinVector GetTvRebarWingWallUpper(BOOL bStt, BOOL bLeft, int nOut);
	void SetDataDefaultRebarWingWallUpper(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void SetDataDefaultRebarWingWallLower(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void SetDataDefaultRebarWingWallSide(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void SetRebarWingWallSideExt(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void SetConvertWingWallHorRebarDim(BOOL bStt, BOOL bLeft, int nOut, int nDan, BOOL bUpper);
	void GetXyRebarWingWallReinforce(CDPoint *xy, BOOL bStt, BOOL bLeft);		// 흉벽 보강철근 좌표 가져오는함수
	long GetMaxWingWallDan(BOOL bStt, BOOL bLeft, long nOut);
	void SetXyArrConvertRebarVirtualLow(CDPointArray &xyArr, BOOL bStt, BOOL bLeft);
	BOOL IsWindWallSectionExistFoot(BOOL bStt, BOOL bLeft, long nOut, int nDan);

	//날개벽 철근 제원
	int GetWingWallRebarDetail(BOOL bStt, BOOL bLeft, int nBaseRebar);
	void SetDataDefaultRebarJewonWingWall(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonHorA(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonVerA(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonHorB(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonVerC(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonHorD(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonVerD(BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void MakeWingWallRebarJewonHunchWall(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonHunchFoot(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonShearA(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonShearBC(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonShearD(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarJewonReinforce(BOOL bStt, BOOL bLeft);
	void MakeWingWallRebarNumber(BOOL bStt, BOOL bLeft);
//	void SetAlignRebarWingWall(BOOL bStt, BOOL bLeft);

	// 방호벽은 m_Util에 있음
	
		
	// 교명주 받침
	void GetXyRebarMonuFront(CDPoint *xy, BOOL bLeft);
	void GetDimArrayRebarMonuFrontSide(CDPointArray &xyArr);
	void GetDimArrayRebarMonuFrontLower(CDPointArray &xyArr);
	int  GetDimArrayRebarMonuPlaneHor(CDPointArray &xyArr);
	int  GetDimArrayRebarMonuPlaneVer(CDPointArray &xyArr);
	void MakeMonumentRebarJewon();

	// ******************* 여기서부터 아래 표시선 까지는 반드시 SerializeRebar함수에서 파일에 저장함. ***************************
	// CDoubleArray나 CDoubleArray*만 와야 된다.
	// 파일 저장시 복잡한 변수명과 개수로 인한 혼선을 방지하기 위해 하나의 함수에서 일괄적으로 처리하기 때문에..
	
	// CYCLE1~4 ///////////////////////////////////////////////////////////////////
	// rebar info cycle 변수
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 상부 슬래브 상,하면 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 하부 슬래브 상,하면 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];	// 시,종점 외측벽체 내측면,외측면rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainUpperSlabHunch[MAX_REBAR_CYCLE];				// 상부슬래브 헌치철근 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainLowerSlabHunch[MAX_REBAR_CYCLE];				// 하부슬래브 헌치철근 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWallBracket_Cycle1[2];				// 외측벽체 브라켓 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWallBracket_Cycle2[2];				// 외측벽체 브라켓 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainBoxLowerPRF_Cycle1[2];				// 부상방지저판 rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainBoxLowerPRF_Cycle2[2];				// 부상방지저판 rebar info cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_UpperCycle1[2][3];	// 상부단절점부 주철근(좌우,1~3단) - 1cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_UpperCycle2[2][3];	// 상부단절점부 주철근(좌우,1~3단) - 2cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_LowerCycle1[2][3];	// 하부단절점부 주철근(좌우,1~3단) - 1cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_LowerCycle2[2][3];	// 하부단절점부 주철근(좌우,1~3단) - 2cycle

	CRebarInfoCycle m_rebarInfoCycleOutWallHunch[MAX_REBAR_CYCLE][2];	// 외측벽체 헌치 1~4cycle, 시종점, 1~3단
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	
	// 이음 간격
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDistUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 상부슬래브 상하면 1사이클
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDistLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 하부슬래브 상하면 1사이클
	CDoubleArray m_pArrDistOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];	// 시좀점, 좌우측 외측벽체 1사이클 이음 간격
	CDoubleArray m_dArrDistRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 리브 1~4cycle, 상하면

	// 철근 지름
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDiaUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 상부슬래브 상하면 1사이클 철근 지름
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDiaLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 하부슬래브 상하면 1사이클 철근 지름
	CDoubleArray m_pArrDiaOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];		// 시종점, 좌우측 외측벽체 1사이클 철근 지름
	CDoubleArray m_dArrDiaUpperSlabHunch[MAX_REBAR_CYCLE];		// 상부 슬래브 헌치부 철근
	CDoubleArray m_dArrDiaLowerSlabHunch[MAX_REBAR_CYCLE];		// 하부 슬래브 헌치부 철근
	CDoubleArray m_dArrDiaRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];				// 리브 1~4cycle, 상하면

	// 철근 존재 뺄껀지 선택(2cycle에 대해서만 지원하지만 차후 업그레이드를 위해 4cycle 모두 변수 생성함)
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrExistUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrExistLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	CDoubleArray m_pArrExistOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];
	CDoubleArray m_dArrExistRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 사이클,상하,단

	/////////////////////////////////////////////////////////////////// CYCLE1~4 //

	// 철근연결및 이음수 설정
	CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> m_pArrSetRebarJointUpperSlab[2];	// 상부슬래브 상하면 2사이클 철근 연결 및 이음개수
	CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> m_pArrSetRebarJointLowerSlab[2];	// 하부슬래브 상하면 2사이클 철근 연결 및 이음개수
	CSetRebarJoint m_pArrSetRebarJointOutWall_Outter[2];	// 시종점, 외측, 외측벽체 2사이클 철근 연결 및 이음개수
	CSetRebarJoint m_pArrSetRebarJointOutWall_Inner[2][2];	// 시종점, 내측, 위아래 외측벽체  2사이클 철근 연결 및 이음개수

	// 철근배치 옵션
	BOOL m_bDetailInput23DanMainRebar;			// 2,3단 주철근을 2Cycle 1단 입력하듯이 세부적으로 조정해서 입력 할 수 잇게 한다.
	BOOL m_bExtendShearRebarToHunchRebar;		// 상부슬래브 전단철근을 헌치철근까지 연장할지?(기본 FALSE) // ARCBRIDGE-2789이슈에 의하여 기본값 변경 (기존: TRUE)
	BOOL m_bExtendShearRebarBoHunchRebar;		// 하부슬래브 전단철근을 헌치철근까지 연장할지?(기본 FALSE) //추가: ARCBRIDGE-2789 기본값: FALSE
	BOOL m_bPlacingOutWallInnerRebar[2];		// 시 종점외측벽체 내측철근 배치
	BOOL m_bPlacingInWallRebarOfCycle24;		// 중간벽체 철근 배치(cycle 2, 4에 대해서 적용됨)
	BOOL m_bBendingDanBuRebar;					// 단 절점부 철근 구부려서 배치
	BOOL m_bPlacingStirrupAtDanBuRebar;			// 단 절점부 ROUND부에 스트럽 배치
	BOOL m_bPlacingSupportAtHunchRebar;			// 헌치철근위에 배력철근 배치
	BOOL m_bPlacingSupportAtDanBuRebar;			// 단 절점부 ROUND부에 배력철근 배치
	BOOL m_bPlacing23DanByStandardOffset;		// 2,3단 철근을 표준배치간격으로 배치
	long m_nCountWrapMainRebarUpperSlab;		// 주철근을 감싸는 개수
	long m_nCountWrapMainRebarLowerSlab;		// 주철근을 감싸는 개수
	long m_nCountWrapMainRebarForOutWall[2];	// 시종점 외측벽체 전단철근이 주철근 구간 감싸는 개수
	long m_nCountWrapMainRebarForInWall;	// 중간벽체 전단철근이 주철근을 감싸는 개수
	long m_nCountWrapMainRebarForBracket[2];	// 브라켓 전단철근이 주철근 구간 감싸는 개수
	long m_nCountWrapMainRebarForFootingLean;	// 직접기초 전단철근이 주철근 구간 감싸는 개수
	long m_nCountWrapMainRebarForFootingPile;	// 말뚝기초 전단철근이 주철근 구간 감싸는 개수
	long m_nCountWrapMainRebarForArchRib;		// 리브 전단철근이 주철근 구간 감싸는 개수
	long m_nCountWrapMainRebarForDanbuStirrup_XXX;	// 단부 스터럽 철근집계시 주철근을 감싸는 개수

	long m_nTypeDefaultCycle3_OutRebar;			// 주철근 cycle3 권고안 방법(외측철근 : 상부슬래브 상면, 외측철근 외측면)
	long m_nTypeDefaultCycle3_UpperSlabLower;	// 주철근 cycle3 권고안 방법(상 부슬래브 하면)
	long m_nTypeDefaultCycle3_OutWallInner;		// 주철근 cycle3 권고안 방법(외측벽체 내측면)
	long m_nTypeDefaultCycle3_ArchRib;			// 주철근 cycle3 권고안 방법(아치리브)
	long m_nTypeCurveOfWallVertRebar;			// 벽체 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
	long m_nTypeStandardCTCRebarMain;			// 주철근 ctc 기준(0 : 상부슬래브, 1 : 시점벽체)
	BOOL m_bRemoveShearRebarAtSlabYundan;		// 슬래브 연단 전단철근 제거
	long m_nTypeCouplerColumnMainRebar;				// 기둥 주철근 0 : 커플러? 1 : 겹이음. 
	BOOL m_bApplySameSupportRebar;				// 상하행 배력철근 배근 간격 동일적용
	BOOL m_bApplySameMainRebar;					// 상하행 주철근관련 입력 동일 적용

	/// 가각부 배근
	long m_nTypeRebarMainRebarOfExp[2][2];			// 가각 배근 타입(0 : 일반직선, 1 : 곡선)
	long m_nTypeOrgMainRebarOfExp[2][2];				// 가각부 배근시 기준 타입(즉, 철근이 확장되는 기준) 0 : 선형좌우측, 1 : 시종점
	double m_dLenMainRebarOfExp[2][2];				// 가각부 배근시 가각부와 좌우측라인과의 교차점에서 주철근이 뻗어 나가는 거리

	BOOL m_bLineDirRebarOfExp[2][2];			// 가각 철근이 선형방향으로 배치될지?
	long m_nCountSupportRebarOfExpAtLineDirRebar[2][2];	// 가각 철근이 선형방향으로 배치될때 기존 보강철근은 몇개가 배치될지

	// 사보강배치일때 사방향배치가 적용되는 구간 거리
	// 배력근
	double m_dDistSttJijumDirPlacing[2];			// 시점각도방향으로 배치되는 배력철근의 배치구간거리(시점에서부터의 거리)
	double m_dDistEndJijumDirPlacing[2];			// 종점각도방향으로 배치된는 배력철근의 배치구간거리(종점에서부터의 거리)
	
	// 주철근
	double m_dDistLeftDirPlacing;			// 좌측면 각도 방향으로 배치되는 주철근의 배치구간거리(시점에서부터의 거리)
	double m_dDistRightDirPlacing;			// 우측면 각도 방향으로 배치되는 주철근의 배치구간거리(종점에서부터의 거리)
	
	// 철근 단수 설정
	CSafetyTypedPtrArray <CObArray, CSetRebarDan*> m_pArrSetRebarDanUpperSlab[2];	// 상부슬래브 상하면, 2단이상 철근의 철근지름 설정
	CSafetyTypedPtrArray <CObArray, CSetRebarDan*> m_pArrSetRebarDanLowerSlab[2];		// 하부슬래브 2단이상 철근의 철근지름 설정
	CSetRebarDan m_pArrSetRebarDanOutWall[2][2];		// 시종점, 내측 외측벽체 2단이상 철근의 철근지름 설정
	long m_nCountDanMainRebarRib[MAX_REBAR_CYCLE][2];	// 리브 1~4사이클, 상하면 주철근 단 설정

	CDoubleArray m_dArrTypeUpperSlabHunch;						// 상부 슬래브 헌치부 철근 타입(벽체식일때 적용 0 : X자, 1 : V형)
	CDoubleArray m_dArrTypeLowerSlabHunch;						// 하부 슬래브 헌치부 철근 타입(벽체식일때 적용 0 : X자, 1 : V형)

	// 단절점부 철근
	BOOL m_bIsDanbuRebar_Cycle1[2][2];			// 상/하 슬래브 - 시종점(시점상단, 종점상단) - 설치여부.
	BOOL m_bIsDanbuRebar_Cycle2[2][2];			// 상/하 슬래브 - 시종점(시점상단, 종점상단) - 설치여부.
	double m_dDia_DanbuRebar_Cycle1[2][2][3];	// 상/하 슬래브 - 시종점(시점상단, 종점상단) - 지름
	double m_dDia_DanbuRebar_Cycle2[2][2][3];	// 상/하 슬래브 - 시종점(시점상단, 종점상단) - 지름

	// 단절점부 철근 위치
	double m_dDanCorner_UpperSlab[3];			// 상부슬래브 - 1단,2단,3단
	double m_dDanCorner_LowerSlab[3];			// 하부슬래브 - 1단,2단,3단

	// (ARCBRIDGE-3636) 절곡부 구부림 반지름 적용 입력값
	double	m_dDanCorner_UpperVert; 
	double	m_dDanCorner_LowerVert; 
	BOOL	m_bUpperSlab_CornerCurve;
	BOOL	m_bLowerSlab_CornerCurve;

	// 단부 스터럽 철근 직경
	double m_dDiaStirrupAtDanBuRebar[2][2];		// 상/하 슬래브 - 단부 스터럽 철근 직경 (시종점)
	long m_nCountStirrupAtDanBuRebar[2][2];		// 상/하 슬래브 - 단부 스터럽 철근 개수 (시종점)
	long m_nCountStirrupCtcMainRebar[2][2];		// 상/하 슬래브 - 주철근 간격

	long m_nType_DanbuRebar;
	// 사용하지 않는 변수
	long m_nType_DanbuRebar_Cycle1_XXX[2][3];	// 종류 0 : ㄷ, 1 : 갈고리
	long m_nType_DanbuRebar_Cycle2_XXX[2][3];	// 종류 0 : ㄷ, 1 : 갈고리
	long m_nIdxCTC_DanbuRebar_XXX[2][3];		// 2 : 시종점(시점상단, 종점상단) - CTC
	double m_dCTC_DanbuRebar_XXX[2][3];			// index에서  double로 변경 (2005.04.25) - 1cycle, 2cycle 공동 사용
	// 사용하지 않는 변수


	// 브라켓 철근 설치 여부
	BOOL m_bIsBracketRebar[2][2];					// 1,2사이클, 시종점

	// 상부슬래브 둔각부 보강
	BOOL m_bIsObtuseAngle_UpperSlab[2][2];	// 시종점, 좌우측
	double m_dCoverObtuseAngle_UpperSlab_Plane[2][2][2];	// 시종점, 좌우측, 0:종방향,1:횡방향
	double m_dCoverObtuseAngle_UpperSlab[2][2][2];	// 시종점, 좌우측, 상하면피복
	double m_dLenObtuseAngle_UpperSlab[2][2][2];	// 시종점, 좌우측, 상하면 철근 길이
	double m_dLenObtuseAngle_OutWall[2][2];			// 시종점, 좌우측 상면 철근 벽체방향 길이
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_UpperSlab[2][2][2];	// 시종점 좌우측 선형방향 간격(상하)
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Main_UpperSlab[2][2][2];	// 시종점 좌우측 횡방향 간격
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_OutWall[2][2];		// 시종점 좌우측 벽체 선형방향 간격
	double m_dDiaObtuseAngle_UpperSlabLower_XXX;		// 선형방향 하면 보강철근 직경
	double m_dDiaObtuseAngle_UpperSlabLower[2][2];		// 선형방향 하면 보강철근 직경 ( 이변수 안 쓰는거 같음)
	CRebarInfoCycle m_rebarInfoCycle_ObtuseAngle_Upperslab[2][2][2];		// 시종점, 좌우측, 상하면 

	//ARCBRIDGE-3193
	BOOL m_bIsObtuseAngle_LowerSlab[2][2];
	double m_dCoverObtuseAngle_LowerSlab[2][2][2];
	double m_dCoverObtuseAngle_LowerSlab_Plane[2][2][2];
	double m_dLenObtuseAngle_LowerSlab[2][2][2];
	double m_dLenObtuseAngle_LowerOutWall[2][2];
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Main_LowerSlab[2][2][2];
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_LowerSlab[2][2][2];
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_LowerOutWall[2][2];
	CRebarInfoCycle m_rebarInfoCycle_ObtuseAngle_Lowerslab[2][2][2];		
	
	

	// 시종점벽체 예각부
	BOOL m_bIsAcuteAngle_OutWall[2][2];	// 시종점 좌우측 벽체 예각부 설치 여부
	double m_dCoverAcuteAngle_OutWall[2][2][2];	// 벽체 예각부 피복(시종점,좌우측,상하면)
	double m_dLenAcuteangle_OutWall[2][2];	// 벽체 예각부 철근길이(시종점,좌우측)
	CPlaceRebarByBlock m_placeByBlock_AcuteAngle_MainRebar_OutWall[2][2];	// 벽체 예각부 철근 배치 간격
	CPlaceRebarByBlock m_placeByBlock_AcuteAngle_SupportRebar_OutWall[2][2];	// 벽체 예각부 철근 배치 간격
	CRebarInfoCycle m_rebarInfoCycle_AcuteAngle_OutWall[2][2];	// 벽체 예각부 철근
	double m_dRange_AcuteAngle_OutWall[2][2];	// 벽체 예각부 철근 가져올 범위
	/////////////////////////////////////////////////////////////////////////////////////////////////////// 주철근 배치 //

	// 표준단면 철근 배치 관련 변수들 (배력철근, 전단철근)
	// 배력철근
	long m_nTypePlacingSupportRebar_UpperSlab;	// 상부슬래브 배력근 배치 타입(0 : 시점경사방향, 1 : 종점경사방향, 2 : 사보강, 3 : 사보강(구간추가))
	long m_nTypePlacingMainRebar_UpperSlab;		// 상부슬래브 주철근 배치 타입(0 : 선형방향, 1 : 사보강, 2 : 사보강(구간추가)
	long m_nTypePlacingMainRebar_Footing;		// 기초 주철근 배치 타입(0 : 선형방향, 1 : 사보강, 2 : 사보강(구간추가)
	double m_dRangeSupportRebarForSameUpperSlab[2];	// 상부슬래브 형태로 꺽이는 배력근 범위
	CPlaceRebarByBlock m_placeByBlock_SupportRebarAddSameUpperSlab[2];	// 상부슬래브 처럼 꺽을 배력근이 추가될 범위
	double m_dDistAddSupportRebarForSameUpperSlab[2][2];	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	long m_nTypeSideRebarMain_UpperSlabSabogang;		// 사보강 배치시 좌우측 주철근 타입(0 : 직선, 1 : 선형따라)
	BOOL m_bFixOutSideMainRebarBy1Cyle;			// 최외측 주철근 1cycle로 고정(2, 4사이클일 경우 적용) - 벽체 기초, 슬래브 모두 적용

	CPlaceRebarByBlock m_placeByBlock_SupportRebar_UpperSlab[2][MAX_REBAR_DAN];		// 상부슬래브 상하면
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_OutWall[2][2][MAX_REBAR_DAN];	// 외측벽체 시종점, 내외측
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_ArchRib[2][MAX_REBAR_DAN][2];			// 리브 상하면(좌우측)		
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_LowerSlab[2][MAX_REBAR_DAN];		// 하부슬래브 상하면
	double m_dAccuracyArchRibLowerSupportDim;	// 아치리브 하면 배력근 치수 정밀도.

	// 전단철근
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_UpperSlab;		// 상부슬래브 전단철근 
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_LowerSlab;		// 하부슬래브 전단철근 
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_OutWall[2];	// 측벽체 시종점 전단철근
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_Bracket[2];	// 브라켓 시종점 전단철근
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_ArchRib[2];			// 리브 전단철근(좌우측)
	CPlaceRebarByBlock m_placeByBlock_StirrupRebar_ConnectSection[2];		// 접합부 스터럽 철근(좌우측)
	long m_nCountWrapShearRebar_StirrupRebar_ConnectSection[2];				// 접합부 스터럽 철근이 감싸는 전단철근 개수
	long m_nIdxCTC_SupportAndShearRebar;						// 배력,전단철근 표준 배치 간격(상부슬래브, 좌우측벽체공동사용)
	double m_dCTC_SupportRebar[2];		// index에서 double로 변경(2005.04.25)
	double m_dCTC_ShearRebar[2];
	double m_dCTC_SupportRebarOutWall[2];
	double m_dCTC_ShearRebarOutWall[2];
	double m_dCTC_ShearRebarBracket;

	// 보및 기둥 철근 배치 관련 변수들(스터럽 / 기둥 주철근, 띠철근)
	// 시종점 그리고 내측벽체.. 즉 지점개수만큼 있을 수 있다.
	CGirderSectionRebar m_SlabGirderRebar[2][MAX_JIJUM_COUNT_RC]; // 0: Upper 1: Lower

	// 좌우측 종거더
	CGirderSectionRebar m_girderRebarJong[2];
	CGirderSectionRebar m_girderRebarJongGagak[2][2];	// 가각 종거더
	double m_dDistGirderRebarJong[2][2];					// 좌우측 종거더 설치위치([iLEFT][iSTT])

	// 브라켓철근
	double m_dDiaRebarMain_Bracket_Cycle1[2];
	double m_dDiaRebarMain_Bracket_Cycle2[2];
	double m_dDiaRebarSupport_Bracket[2];
	double m_dDiaRebarShear_Bracket_XXX[2];

	// 철근 집계
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarSupport;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarShear;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarEtc;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarGirder;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMonument;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain_User;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarGirder_User;
	// ******************* SerializeRebar 함수 사용하는 변수들 표시선  ***************************





	// 배근시 속도 개선을 위해 자주 쓰이는 좌표나 간격은 미리 구해 놓고 가져다 쓴다. ///////////////////////
	void SettingValueRebarForSlab();		// 상부슬래브 배근도를 위한 값 셋팅
	void InitValueRebarForUpperSlab(BOOL bLowerSlab=FALSE);		// 상부슬래브 배근도를 위한 값 초기화
	void SettingValueRebarForCrossSection(double dStaLeft, double dStaRight);	// 횡단면도 배근도를 위한 값 셋팅
	void InitValueRebarForCrossSection();	// 횡단면도 배근도를 위한 값 초기화

	CTwinVectorArray m_tvArrSupRebarCross[2];	// 횡단면 배력근 상하
	CDoubleArray m_dArrStaSupRebar[2];	// 배력근 스테이션
	double m_dStaLeftCross;
	double m_dStaRightCross;
	CVectorArray m_xyArrMainRebarCross[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN][2];	// 횡단면 배력근
	CVectorArray m_xyArrMainRebarCrossHunch[MAX_REBAR_CYCLE][2];	// 횡단면 배력근

	CTwinVectorArray m_tvArrSupRebarCrossLowerSlab[2];	// 횡단면 배력근 상하
	CDoubleArray m_dArrStaSupRebarLowerSlab[2];	// 배력근 스테이션
	CVectorArray m_xyArrMainRebarCrossLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN][2];	// 횡단면 배력근
	CVectorArray m_xyArrMainRebarCrossHunchLowerSlab[MAX_REBAR_CYCLE][2];	// 횡단면 배력근

	// 평면 주철근, 배력근을 모두 미리 구해놓자.(철근마크할때 또 쓰기 때문에)
	BOOL m_bCaledRebarMain_Plane;
	BOOL m_bCaledRebarSupport_Plane;
	CTwinVectorArrayArray m_tvArr_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrAngLeft_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
						m_tvArrAngRight_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrJoint_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrHidden_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
						m_tvArrHiddenLeft_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrHiddenRight_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrAngLeftHalf_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
						m_tvArrAngRightHalf_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2];
	CTwinVectorArrayArray m_tvArr_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], m_tvArrAngLeft_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], 
						m_tvArrAngRight_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], m_tvArrJoint_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], m_tvArrHidden_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], 
						m_tvArrHiddenLeft_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], m_tvArrHiddenRight_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], m_tvArrAngLeftHalf_RebarMain_PlaneHunch[MAX_REBAR_CYCLE], 
						m_tvArrAngRightHalf_RebarMain_PlaneHunch[MAX_REBAR_CYCLE];
	CTwinVectorArray m_tvArrSttBracket_RebarMain_Plane, m_tvArrEndBracket_RebarMain_Plane;
	CTwinVectorArray m_tvArr_RebarSupport_Plane[MAX_REBAR_DAN][2], m_tvArrAngStt_RebarSupport_Plane[MAX_REBAR_DAN][2], m_tvArrAngEnd_RebarSupport_Plane[MAX_REBAR_DAN][2], 
					m_tvArrRAngStt_RebarSupport_Plane[MAX_REBAR_DAN][2], m_tvArrRAngEnd_RebarSupport_Plane[MAX_REBAR_DAN][2];

	CDoubleArray m_dArrDist_RebarMain_Plane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2][2];
	CDoubleArray m_dArrDist_RebarMain_PlaneHunch[MAX_REBAR_CYCLE][2];

	// 하부 슬래브 평면
	CTwinVectorArrayArray m_tvArr_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrAngLeft_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
		m_tvArrAngRight_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrJoint_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrHidden_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
		m_tvArrHiddenLeft_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrHiddenRight_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], m_tvArrAngLeftHalf_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2], 
		m_tvArrAngRightHalf_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2];
	CTwinVectorArrayArray m_tvArr_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], m_tvArrAngLeft_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], 
		m_tvArrAngRight_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], m_tvArrJoint_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], m_tvArrHidden_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], 
		m_tvArrHiddenLeft_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], m_tvArrHiddenRight_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], m_tvArrAngLeftHalf_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE], 
		m_tvArrAngRightHalf_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE];
	CTwinVectorArray m_tvArrSttPRF_RebarMain_LowerPlane, m_tvArrEndPRF_RebarMain_LowerPlane;
	CTwinVectorArray m_tvArr_RebarSupport_LowerPlane[MAX_REBAR_DAN][2], m_tvArrAngStt_RebarSupport_LowerPlane[MAX_REBAR_DAN][2], m_tvArrAngEnd_RebarSupport_LowerPlane[MAX_REBAR_DAN][2], 
		m_tvArrRAngStt_RebarSupport_LowerPlane[MAX_REBAR_DAN][2], m_tvArrRAngEnd_RebarSupport_LowerPlane[MAX_REBAR_DAN][2];
	
	CDoubleArray m_dArrDist_RebarMain_LowerPlane[MAX_REBAR_CYCLE][MAX_REBAR_DAN][2][2];
	CDoubleArray m_dArrDist_RebarMain_LowerPlaneHunch[MAX_REBAR_CYCLE][2];

	// 가각있는 경우 가각없이 주철근을 배근한다.
	// 그러나 사보강 절반 배치의 경우 가각이 적용되어야 하는 부분도 있으므로 
	// 기존의 가각이 있는 피복평면을 보관하고 있다가 써먹는다.
	CTwinVectorArray m_tvArrPlane1DanOrg;	
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	CRcBridgeRebarUtil m_Util;
	long m_nCountRebarDanbu[2][2][2];	// 단부 철근 개수(상부, 하부)(1, 2cycle)(시종점)

	// (LSD:0001)
	void SetRebarLsdOptionSlab(SRebarEnvOption *pSLsdOption, BOOL bUpperSlab, BOOL bUper, BOOL bMain, long nCycle, long nDan);
	void SetRebarLsdOptionOutWall(SRebarEnvOption *pSLsdOption, BOOL bStt, BOOL bFront, BOOL bMain, long nCycle, long nDan);
	void SetRebarLsdOptionOutWallHunchSupport(SRebarEnvOption *pSLsdOption, BOOL bStt);
	void SetRebarLsdOptionWingWall(SRebarEnvOption *pSLsdOption, BOOL bStt, BOOL bLeft, BOOL bFront, long nPos, BOOL bMain);
	void SetRebarLsdOptionOutWallBracketSupport(SRebarEnvOption *pSLsdOption, BOOL bStt);
	void SetRebarLsdOptionGirderMain(SRebarEnvOption *pSLsdOption, long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder, BOOL bApplyVert, BOOL bUpperSlabGirder, BOOL bUpper, long nDan);

	void SetExposureParameter(SExposureOutputParams &stOutputParams);

	double GetDistCheckShearSlab(long nSpan, long nPos, BOOL bUpperSlab=TRUE, CConcBasic *pConc=NULL);
	double GetDistCheckShearWall(long nJijum, long nPos, CConcBasic *pConc=NULL);
	double GetDistCheckShearPRF(BOOL bStt, CConcBasic *pConc=NULL);
};

#endif // !defined(AFX_RCBRIDGEREBAR_H__25A06976_64C1_4C1B_85E9_286A733F4F5D__INCLUDED_)
