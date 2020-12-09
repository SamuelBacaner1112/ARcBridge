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
#define GAP_JOINT			30		// ��ö�ٰ� �������� ����

// ö�ٸ�ũ ������ ���� ���� ���� ///////////////////////////////
#define MARK_UPPERSLAB_UPPER	0	// ��ν����� ��� ö�� ��ũ
#define MARK_UPPERSLAB_LOWER	1	// ��ν����� �ϸ� ö�� ��ũ
#define MARK_UPPERSLAB_HUNCH	2	// ��ν����� ��ġ�� ö�� ��ũ
#define MARK_STTWALL_INNER		3	// ��������ü ���� ö�� ��ũ
#define MARK_STTWALL_OUTTER		4	// ��������ü ���� ö�� ��ũ
#define MARK_ENDWALL_INNER		5	// ��������ü ���� ö�� ��ũ
#define MARK_ENDWALL_OUTTER		6	// ��������ü ���� ö�� ��ũ
#define MARK_OUTWALL_HUNCH		7	// ������ü ��ġ ö�� ��ũ
#define MARK_OBTUSE_UPPER		8	// ��ν����� �а��� ���
#define MARK_OBTUSE_LOWER		9	// ��ν����� �а��� �ϸ�
#define MARK_OBTUSE_CROSS		10	// ��ν����� �а��� (Ⱦ)
#define MARK_ACUTE_OUTWALL		11	// ������ü ������ ��ö��
#define MARK_ARCHRIB_UPPER			12	// ���� ���
#define MARK_ARCHRIB_LOWER			13	// ���� �ϸ�
#define MARK_LOWERSLAB_UPPER	14	// �Ϻν����� ��� ö�� ��ũ
#define MARK_LOWERSLAB_LOWER	15	// �Ϻν����� �ϸ� ö�� ��ũ
#define MARK_LOWERSLAB_HUNCH	16	// �Ϻν����� ��ġ�� ö�� ��ũ
#define MARK_OBTUSE_UPPER_LOWSLAB      17	// �Ϻν����� �а��� ���
#define MARK_OBTUSE_LOWER_LOWSLAB		18	// �Ϻν����� �а��� �ϸ�


/////////////////////////////////////////////////////////////////

// ö�� ����

// ���ö�� ////////////
// ��ν�����
#define US_REBAR_S_U1		0	// ��ν����� ���ö�� ��� 1��.
#define US_REBAR_S_U2		1	// ��ν����� ���ö�� ��� 2��.
#define US_REBAR_S_U3		2	// ��ν����� ���ö�� ��� 3��.
#define US_REBAR_S_L1		3	// ��ν����� ���ö�� �ϸ� 1��.
#define US_REBAR_S_L2		4	// ��ν����� ���ö�� �ϸ� 2��.
#define US_REBAR_S_L3		5	// ��ν����� ���ö�� �ϸ� 3��.

// ������ ��ü
#define SW_REBAR_S_O1		6	// ��������ü ���ö�� ���� 1��.
#define SW_REBAR_S_O2		7	// ��������ü ���ö�� ���� 2��.
#define SW_REBAR_S_O3		8	// ��������ü ���ö�� ���� 3��.
#define SW_REBAR_S_I1		9	// ��������ü ���ö�� ���� 1��.
#define SW_REBAR_S_I2		10	// ��������ü ���ö�� ���� 2��.
#define SW_REBAR_S_I3		11	// ��������ü ���ö�� ���� 3��.

// ������ ��ü
#define EW_REBAR_S_O1		12	// ��������ü ���ö�� ���� 1��.
#define EW_REBAR_S_O2		13	// ��������ü ���ö�� ���� 2��.
#define EW_REBAR_S_O3		14	// ��������ü ���ö�� ���� 3��.
#define EW_REBAR_S_I1		15	// ��������ü ���ö�� ���� 1��.
#define EW_REBAR_S_I2		16	// ��������ü ���ö�� ���� 2��.
#define EW_REBAR_S_I3		17	// ��������ü ���ö�� ���� 3��.

// ��ü ��ġ��
#define SW_REBAR_SH			18	// ������ü ��ġ�� ���ö��
#define EW_REBAR_SH			19	// ������ü ��ġ�� ���ö��

// ������ü �ϴ� ������ ��±�
#define SW_REBAR_AC_L		20	// ������ü ���� ������ ���ö��
#define SW_REBAR_AC_R		21	// ������ü ���� ������ ���ö��
#define EW_REBAR_AC_L		22	// ������ü ���� ������ ���ö��
#define EW_REBAR_AC_R		23	// ������ü ���� ������ ���ö��

// ��ν����� �а��� ��±�
#define US_REBAR_OB_SL		24	// ��ν����� �������� �а��� ���ö��
#define US_REBAR_OB_SR		25	// ��ν����� �������� �а��� ���ö��
#define US_REBAR_OB_EL		26	// ��ν����� �������� �а��� ���ö��
#define US_REBAR_OB_ER		27	// ��ν����� �������� �а��� ���ö��

#define US_REBAR_OB_SLL		28	// ��ν����� �������� �а��� �ϸ� ���ö��
#define US_REBAR_OB_SRL		29	// ��ν����� �������� �а��� �ϸ� ���ö��
#define US_REBAR_OB_ELL		30	// ��ν����� �������� �а��� �ϸ� ���ö��
#define US_REBAR_OB_ERL		31	// ��ν����� �������� �а��� �ϸ� ���ö��

// ��ġ����
#define AR_REBAR_S_U1		32	// ��ġ���� ��� 1�� ���ö��
#define AR_REBAR_S_U2		33	// ��ġ���� ��� 2�� ���ö��
#define AR_REBAR_S_U3		34	// ��ġ���� ��� 3�� ���ö��

#define AR_REBAR_S_L1		35	// ��ġ���� �ϸ� 1�� ���ö��
#define AR_REBAR_S_L2		36	// ��ġ���� �ϸ� 2�� ���ö��
#define AR_REBAR_S_L3		37	// ��ġ���� �ϸ� 3�� ���ö��

// �Ϻν�����
#define LS_REBAR_S_U1		38	// �Ϻν����� ���ö�� ��� 1��.
#define LS_REBAR_S_U2		39	// �Ϻν����� ���ö�� ��� 2��.
#define LS_REBAR_S_U3		40	// �Ϻν����� ���ö�� ��� 3��.
#define LS_REBAR_S_L1		41	// �Ϻν����� ���ö�� �ϸ� 1��.
#define LS_REBAR_S_L2		42	// �Ϻν����� ���ö�� �ϸ� 2��.
#define LS_REBAR_S_L3		43	// �Ϻν����� ���ö�� �ϸ� 3��.

// �Ϻν����� �а��� ��±�
#define LS_REBAR_OB_SL		44	// �Ϻν����� �������� �а��� ���ö��
#define LS_REBAR_OB_SR		45	// �Ϻν����� �������� �а��� ���ö��
#define LS_REBAR_OB_EL		46	// �Ϻν����� �������� �а��� ���ö��
#define LS_REBAR_OB_ER		47	// �Ϻν����� �������� �а��� ���ö��

#define LS_REBAR_OB_SLL		48	// �Ϻν����� �������� �а��� �ϸ� ���ö��
#define LS_REBAR_OB_SRL		49	// �Ϻν����� �������� �а��� �ϸ� ���ö��
#define LS_REBAR_OB_ELL		50	// �Ϻν����� �������� �а��� �ϸ� ���ö��
#define LS_REBAR_OB_ERL		51	// �Ϻν����� �������� �а��� �ϸ� ���ö��

#define SUP_REBAR_SU		52	// ���ö�� ����

// ����ö�� ////////////
#define US_REBAR_V			0	// ��ν����� ����ö��
#define SW_REBAR_V			1	// ��������ü ����ö��
#define EW_REBAR_V			2	// ��������ü ����ö��
#define SD_REBAR_V1			3	// ���� �ܺ� ���ͷ�1
#define SD_REBAR_V2			4	// ���� �ܺ� ���ͷ�2
#define ED_REBAR_V1			5	// ���� �ܺ� ���ͷ�1
#define ED_REBAR_V2			6	// ���� �ܺ� ���ͷ�2
#define AR_REBAR_V			7	// ��ġ���� ����ö��
#define CO_REBAR_VL			8	// ���պ� ���ͷ�ö�� ����
#define CO_REBAR_VR			9	// ���պ� ���ͷ�ö�� ����
#define LS_REBAR_V			10	// �Ϻν����� ����ö��
#define SH_REBAR_SU			11	// ����ö�� ����


// ��Ÿö�� ///////////
// �Ŵ� 
#define G_REBAR_M_U1		0	// �Ŵ� ö�� ��� 1��(��ö��)
#define G_REBAR_M_U2		1	// �Ŵ� ö�� ��� 2��(��ö��)
#define G_REBAR_M_U3		2	// �Ŵ� ö�� ��� 3��(��ö��)
#define G_REBAR_M_L1		3	// �Ŵ� ö�� �ϸ� 1��(��ö��)
#define G_REBAR_M_L2		4	// �Ŵ� ö�� �ϸ� 2��(��ö��)
#define G_REBAR_M_L3		5	// �Ŵ� ö�� �ϸ� 3��(��ö��)
#define G_REBAR_HORV		6	// �Ŵ� ö�� ����(��������ö��)
#define G_REBAR_HORV_I		7	// �Ŵ� ö�� ����(��������ö�� ����)
#define G_REBAR_ST			8	// �Ŵ� ���ͷ� ö��(��������ö��)
#define G_REBAR_SUP			9	// �Ŵ� ����ö��(��ö�� 2�ܺ��� �Ʒ��� ��ġ�� ö��)

// �����
#define BR_REBAR_M			10	// ����� ��ö��
#define BR_REBAR_S			11	// ����� ��±�
#define BR_REBAR_V			12	// ����� ����ö��
#define ETC_REBAR_SU		13	// ��Ÿö�� ����

//�λ��������
#define BF_REBAR_M			14	// �λ�������� ��ö��
#define BF_REBAR_S			15	// �λ�������� ��±�
#define BF_REBAR_V			16	// �λ�������� ����ö��

// ������ ��ħ
#define MO_REBAR_M1			0	// ������ ��ħ ��ö�� 1
#define MO_REBAR_M2			1	// ������ ��ħ ��ö�� 2
#define MO_REBAR_S_U		2	// ������ ��ħ ��� ��±�
#define MO_REBAR_S_S		3	// ������ ��ħ ���� ��±�
#define MO_REBAR_S_L		4	// ������ ��ħ �ϴ� ��±�
#define MO_REBAR_COUNT		5	// ������ ��ħ ö�� ����

#define MIN_REBAR_SPACE 100

typedef CSimpleTypedArray <CRebarJoint> CJointArray;

// ���� ö�� ��ġ Ŭ����
class AFX_EXT_CLASS CRcBridgeRebar : public CRcBridgeApp
{
public:
	void SerializeEnv(CArchive &ar);
	void SetDataDefaultEnv();
private:
	// �� �Լ��� �Ϲ� ���� �ϳ��� �����ϰų� �д� �Լ��ε�.. ///////
	// ���� �ϳ��ϳ� ��� ���ؼ� �����ϱ� ������ �ӵ������� �ɸ���.
	// �׷��� SerializeRebar������ ����ϱ⸦ �ǰ��Ѵ�.
	// bLoad�� flag�� ���� �о�� ���� �Ǵ�..
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
	BOOL IsRebarMainOneLineUpperSlab(BOOL bLeft, BOOL bStt);	/**< ��ν����� ��ö�� ù��°�� ������ �������� ö���� '��'�ڰ� �Ǿ�� �ϴ���? */
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

	// 2cycle �ǰ��
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

	// ��ö�� ��ġ ���� �Լ� ----------------------------------------
	// rebar info cycle ���� �Լ�
	// ��ö�� �����Ҷ��� �ݵ�� MakeRebarInfoCycleStd�� ���ؼ� �����ؾ� ö�� ��ȣ�� ����� ���´�.
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
	void MakeRebarInfoCycle_UpperSlab(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// ��� ������ RebarInfoCycle ����(cycle1,3).
	void MakeRebarInfoCycle_OutWall(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// ������ü RebarInfoCycle ����(cycle1,3).
	void MakeRebarInfoCycle_UpperSlabHunch(long nCycle, BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_UpperDanbu(BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_LowerDanbu(BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_LowerSlab(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// �Ϻ� ������ RebarInfoCycle ����(cycle1,3).
	void MakeRebarInfoCycle_LowerSlabHunch(long nCycle, BOOL bInput, long nVertDir=0);
	void MakeRebarInfoCycle_OutWallBoxType(long nCycle, long nDan, BOOL bInput, long nVertDir=0);		// ������ ������ü RebarInfoCycle ����(cycle1,3).
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
	BOOL m_bMakedRebarInfoCycleByVert;	// ���� ��ö���� ��Ÿ��� ����� ������ ���Ÿ��� ����� ������
	BOOL m_bMakedRebarInfoCycleByVertUpperSlab;		// ��ν����� ��ö���� ���Ÿ�����, ��Ÿ�����
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

	// ���� ���� �������� �Լ�
	long GetCountJointMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner);
	long GetCountJointMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan);
	long GetCountJointMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan);

	// ö�� ��ġ �ܼ� ���� ���� �Լ�
	long GetDanMainRebarOutWall(long nCycle, BOOL bStt, BOOL bInner);
	long GetDanMainRebarUpperSlab(long nCycle, BOOL bUpper, long nIdx);
	long GetDanMainRebarLowerSlab(long nCycle, BOOL bUpper, long nIdx);
	
	// ���� ���� �������� �Լ�
	double GetDistMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx, BOOL bAccumulation);
	double GetDistMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, BOOL bAccumulation, long nVertDir=0);
	double GetDistMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx, BOOL bAccumulation, long nVertDir=0);

	// ��ö�� ���� �������� �Լ�
	double GetDiaMainRebarOutWall(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nIdx);
	double GetDiaMainRebarUpperSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	double GetDiaMainRebarUpperSlabHunch(long nCycle, long nJijum);
	double GetDiaMainRebarLowerSlab(long nCycle, long nDan, BOOL bUpper, long nJigan, long nIdx);
	double GetDiaMainRebarLowerSlabHunch(long nCycle, long nJijum);
	
	// ������ ����
	double GetDiaLsdExposure_Main(EPartOfBridge ePos, BOOL bUpperFront); //�ּ��Ǻ��β�(����)
	double GetDiaLsdExposure_Shear(EPartOfBridge ePos);

	//��ö�� ���� �����ϴ� �Լ�
	BOOL SetDiaMainRebarUpperSlabHunch(long nCycle, long nJijum, double dDia);
	BOOL SetDiaMainRebarLowerSlabHunch(long nCycle, long nJijum, double dDia);

	// ö���� ����Ǿ����� �Ǵ��ϴ� �Լ�
	BOOL IsJointMainRebarUpperSlab_Lower_Cycle2(double dSta);
	BOOL IsJointMainRebarUpperSlab_Lower_Cycle2(long nJijum);
	BOOL IsJointMainRebarUpperSlab_Upper_Cycle2(long nJigan);

	BOOL IsJointMainRebarLowerSlab_Upper_Cycle2(double dSta);
	BOOL IsJointMainRebarLowerSlab_Upper_Cycle2(long nJijum);
	BOOL IsJointMainRebarLowerSlab_Lower_Cycle2(long nJigan);

	// ��ö�� ���� ����ö�� �������� �Լ�
	CTwinVector GetTvOffset_OutWall(long nDan, BOOL bStt, BOOL bInner, BOOL bVertDir);
	CTwinVector GetTvOffset_UpperSlab(long nDan, BOOL bUpper, double dSttSta=-1, double dEndSta=-1, BOOL bVertDir=FALSE, BOOL bApplyFlexibleJoint=TRUE);
	CTwinVector GetTvOffset_LowerSlab(long nDan, BOOL bUpper, double dSttSta=-1, double dEndSta=-1, BOOL bVertDir=FALSE);

	// RebarInfoCycle �������� �Լ�
	CRebarInfoCycle *GetRebarInfoCycleUpperSlab(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleUpperSlabCheckExist(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleLowerSlab(long nCycle, long nDan, BOOL bUpper, long nIdx);
	CRebarInfoCycle *GetRebarInfoCycleLowerSlabCheckExist(long nCycle, long nDan, BOOL bUpper, long nIdx);

	// rebarinfocycle �迭�� �ش� ���ڷ� ��ũ��Ʈ���� �������ش�.
	void SetStringMarkToRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nType, long nCycle, BOOL bInput, BOOL bInit=TRUE, long nDan=0);
	void SetStringDescriptionRebarInfoCycle_UpperSlab(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_ArchRib(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_UpperSlabHunch(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle);
	void SetStringDescriptionRebarInfoCycle_OutWall(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bStt, BOOL bInner, long nDan, long nCycle);
	void SetStringDescriptionRebarInfoCycle_LowerSlab(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle, BOOL bUpper, long nDan);
	void SetStringDescriptionRebarInfoCycle_LowerSlabHunch(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nCycle);

	

	// ���� ���� �����ϴ� �Լ�(���⼭�� pArrJoint�� new�� ����ؼ� �Ҵ��ϹǷ� 
	void MakeJointMainRebarOutWall(CJointArray &arrJoint, long nCycle, long nDan, BOOL bStt, BOOL bInner);
	void MakeJointMainRebarUpperSlab(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper);
	void MakeJointMainRebarLowerSlab(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper);
	void MakeJointMainRebarRib(CJointArray &arrJoint, long nCycle, long nDan, BOOL bUpper, CParabola *pPb=NULL);

	// ��Ÿ �뵵 �Լ�
	double GetLenRebarBeforeEdge_UpperSlab_Cycle13(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nVertDir);
	double GetLenRebarAtEdge(double dDia, BOOL bInFooting=FALSE, long nVertDir=0, BOOL bUpperSlab=FALSE, BOOL bBending=FALSE);
	double GetLenRebarBeforeEdge_LowerSlab_Cycle13(long nCycle, long nDan, BOOL bStt, BOOL bInner, long nVertDir);
	CTwinVector GetTvCornerBaseLine(HGBOOL bStt, HGBOOL bUpper);
	// ��ö�� ��ġ ���� �Լ� ----------------------------------------

	// ��ö�� ��ġ ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	// ö�� ��ġ �ɼ� ����
	BOOL m_bRoundAtEdgeRebar;		// ��ġ�� ö���� ���� ��������?

	// ö�� ��ġ ���� index
	long m_nIdxCTC_MainRebar;
	double m_dCTC_MainRebar;	// index��Ŀ��� double������ ���� index�� ������� ����(2005.04.25)

	// ����, ���౸���� ���ϰ������� ��ġ
	BOOL m_bSameOffsetPlaceTAndCBlock;	
	
	// ���� ����
	long m_nCountJointMainRebar_UpperSlab_Cycle1[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// ��ν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_UpperSlab_Cycle2[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// ��ν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_LowerSlab_Cycle1[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// �Ϻν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_LowerSlab_Cycle2[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// �Ϻν����� ��,�ϸ� �� ������ ���� ����

	long m_nCountJointMainRebar_OutWall_Cycle1[2][2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_OutWall_Cycle2[2][2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_ArchRib_Cycle1[2][MAX_REBAR_DAN];	// ���� ���ϸ�,1~3�� ���� ����(���길 Ư���� �� �ܿ� ���ؼ� ������ �Է¹���)
	long m_nCountJointMainRebar_ArchRib_Cycle2[2][MAX_REBAR_DAN];	// ���� ���ϸ�,1~3�� ���� ����(���길 Ư���� �� �ܿ� ���ؼ� ������ �Է¹���)

	// ���� ����(������ ������) - ���� ���� ����� ����� �κи� �ǰ���� �ؾ� �ϱ� ������(Seralize�� ���� ����) //////
	long m_nCountJointMainRebar_UpperSlab_Cycle1_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// ��ν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_UpperSlab_Cycle2_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// ��ν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_LowerSlab_Cycle1_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// �Ϻν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_LowerSlab_Cycle2_Old[MAX_JIJUM_COUNT_RC][2][MAX_REBAR_DAN];	// �Ϻν����� ��,�ϸ� �� ������ ���� ����
	long m_nCountJointMainRebar_OutWall_Cycle1_Old[2][2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_OutWall_Cycle2_Old[2][2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_ArchRib_Cycle1_Old[2][MAX_REBAR_DAN];	// ���� ���ϸ�,1~3�� ���� ����(���길 Ư���� �� �ܿ� ���ؼ� ������ �Է¹���)
	long m_nCountJointMainRebar_ArchRib_Cycle2_Old[2][MAX_REBAR_DAN];	// ���� ���ϸ�,1~3�� ���� ����(���길 Ư���� �� �ܿ� ���ؼ� ������ �Է¹���)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ����ö�� Ÿ�� ����
	CRebarShearDetail m_RebarShearDetail_UpperSlab;		// ��ν�����
	CRebarShearDetail m_RebarShearDetail_LowerSlab;		// �Ϻν�����
	CRebarShearDetail m_RebarShearDetail_OutWall[2];	// ������ ������ü
	CRebarShearDetail m_RebarShearDetail_InWall;		// �߰���ü
	CRebarShearDetail m_RebarShearDetail_FootingLean;	// ��������
	CRebarShearDetail m_RebarShearDetail_FootingPile;	// ���ұ���
	CRebarShearDetail m_RebarShearDetail_ArchRib;			// ����
	CRebarShearDetail m_RebarShearDetail_Bracket[2];	// �����

	// ������ ���
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
	void GetXyRebarWingWallReinforce(CDPoint *xy, BOOL bStt, BOOL bLeft);		// �亮 ����ö�� ��ǥ ���������Լ�
	long GetMaxWingWallDan(BOOL bStt, BOOL bLeft, long nOut);
	void SetXyArrConvertRebarVirtualLow(CDPointArray &xyArr, BOOL bStt, BOOL bLeft);
	BOOL IsWindWallSectionExistFoot(BOOL bStt, BOOL bLeft, long nOut, int nDan);

	//������ ö�� ����
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

	// ��ȣ���� m_Util�� ����
	
		
	// ������ ��ħ
	void GetXyRebarMonuFront(CDPoint *xy, BOOL bLeft);
	void GetDimArrayRebarMonuFrontSide(CDPointArray &xyArr);
	void GetDimArrayRebarMonuFrontLower(CDPointArray &xyArr);
	int  GetDimArrayRebarMonuPlaneHor(CDPointArray &xyArr);
	int  GetDimArrayRebarMonuPlaneVer(CDPointArray &xyArr);
	void MakeMonumentRebarJewon();

	// ******************* ���⼭���� �Ʒ� ǥ�ü� ������ �ݵ�� SerializeRebar�Լ����� ���Ͽ� ������. ***************************
	// CDoubleArray�� CDoubleArray*�� �;� �ȴ�.
	// ���� ����� ������ ������� ������ ���� ȥ���� �����ϱ� ���� �ϳ��� �Լ����� �ϰ������� ó���ϱ� ������..
	
	// CYCLE1~4 ///////////////////////////////////////////////////////////////////
	// rebar info cycle ����
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// ��� ������ ��,�ϸ� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �Ϻ� ������ ��,�ϸ� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];	// ��,���� ������ü ������,������rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainUpperSlabHunch[MAX_REBAR_CYCLE];				// ��ν����� ��ġö�� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainLowerSlabHunch[MAX_REBAR_CYCLE];				// �Ϻν����� ��ġö�� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWallBracket_Cycle1[2];				// ������ü ����� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainOutWallBracket_Cycle2[2];				// ������ü ����� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainBoxLowerPRF_Cycle1[2];				// �λ�������� rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainBoxLowerPRF_Cycle2[2];				// �λ�������� rebar info cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_UpperCycle1[2][3];	// ��δ������� ��ö��(�¿�,1~3��) - 1cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_UpperCycle2[2][3];	// ��δ������� ��ö��(�¿�,1~3��) - 2cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_LowerCycle1[2][3];	// �Ϻδ������� ��ö��(�¿�,1~3��) - 1cycle
	CRebarInfoCycle m_rebarInfoCycleDanbu_LowerCycle2[2][3];	// �Ϻδ������� ��ö��(�¿�,1~3��) - 2cycle

	CRebarInfoCycle m_rebarInfoCycleOutWallHunch[MAX_REBAR_CYCLE][2];	// ������ü ��ġ 1~4cycle, ������, 1~3��
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMainRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	
	// ���� ����
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDistUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// ��ν����� ���ϸ� 1����Ŭ
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDistLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �Ϻν����� ���ϸ� 1����Ŭ
	CDoubleArray m_pArrDistOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];	// ������, �¿��� ������ü 1����Ŭ ���� ����
	CDoubleArray m_dArrDistRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// ���� 1~4cycle, ���ϸ�

	// ö�� ����
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDiaUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// ��ν����� ���ϸ� 1����Ŭ ö�� ����
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrDiaLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �Ϻν����� ���ϸ� 1����Ŭ ö�� ����
	CDoubleArray m_pArrDiaOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];		// ������, �¿��� ������ü 1����Ŭ ö�� ����
	CDoubleArray m_dArrDiaUpperSlabHunch[MAX_REBAR_CYCLE];		// ��� ������ ��ġ�� ö��
	CDoubleArray m_dArrDiaLowerSlabHunch[MAX_REBAR_CYCLE];		// �Ϻ� ������ ��ġ�� ö��
	CDoubleArray m_dArrDiaRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];				// ���� 1~4cycle, ���ϸ�

	// ö�� ���� ������ ����(2cycle�� ���ؼ��� ���������� ���� ���׷��̵带 ���� 4cycle ��� ���� ������)
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrExistUpperSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	CSafetyTypedPtrArray <CObArray, CDoubleArray*> m_pArrExistLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];
	CDoubleArray m_pArrExistOutWall[MAX_REBAR_CYCLE][2][2][MAX_REBAR_DAN];
	CDoubleArray m_dArrExistRib[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// ����Ŭ,����,��

	/////////////////////////////////////////////////////////////////// CYCLE1~4 //

	// ö�ٿ���� ������ ����
	CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> m_pArrSetRebarJointUpperSlab[2];	// ��ν����� ���ϸ� 2����Ŭ ö�� ���� �� ��������
	CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> m_pArrSetRebarJointLowerSlab[2];	// �Ϻν����� ���ϸ� 2����Ŭ ö�� ���� �� ��������
	CSetRebarJoint m_pArrSetRebarJointOutWall_Outter[2];	// ������, ����, ������ü 2����Ŭ ö�� ���� �� ��������
	CSetRebarJoint m_pArrSetRebarJointOutWall_Inner[2][2];	// ������, ����, ���Ʒ� ������ü  2����Ŭ ö�� ���� �� ��������

	// ö�ٹ�ġ �ɼ�
	BOOL m_bDetailInput23DanMainRebar;			// 2,3�� ��ö���� 2Cycle 1�� �Է��ϵ��� ���������� �����ؼ� �Է� �� �� �հ� �Ѵ�.
	BOOL m_bExtendShearRebarToHunchRebar;		// ��ν����� ����ö���� ��ġö�ٱ��� ��������?(�⺻ FALSE) // ARCBRIDGE-2789�̽��� ���Ͽ� �⺻�� ���� (����: TRUE)
	BOOL m_bExtendShearRebarBoHunchRebar;		// �Ϻν����� ����ö���� ��ġö�ٱ��� ��������?(�⺻ FALSE) //�߰�: ARCBRIDGE-2789 �⺻��: FALSE
	BOOL m_bPlacingOutWallInnerRebar[2];		// �� ����������ü ����ö�� ��ġ
	BOOL m_bPlacingInWallRebarOfCycle24;		// �߰���ü ö�� ��ġ(cycle 2, 4�� ���ؼ� �����)
	BOOL m_bBendingDanBuRebar;					// �� ������ ö�� ���η��� ��ġ
	BOOL m_bPlacingStirrupAtDanBuRebar;			// �� ������ ROUND�ο� ��Ʈ�� ��ġ
	BOOL m_bPlacingSupportAtHunchRebar;			// ��ġö������ ���ö�� ��ġ
	BOOL m_bPlacingSupportAtDanBuRebar;			// �� ������ ROUND�ο� ���ö�� ��ġ
	BOOL m_bPlacing23DanByStandardOffset;		// 2,3�� ö���� ǥ�ع�ġ�������� ��ġ
	long m_nCountWrapMainRebarUpperSlab;		// ��ö���� ���δ� ����
	long m_nCountWrapMainRebarLowerSlab;		// ��ö���� ���δ� ����
	long m_nCountWrapMainRebarForOutWall[2];	// ������ ������ü ����ö���� ��ö�� ���� ���δ� ����
	long m_nCountWrapMainRebarForInWall;	// �߰���ü ����ö���� ��ö���� ���δ� ����
	long m_nCountWrapMainRebarForBracket[2];	// ����� ����ö���� ��ö�� ���� ���δ� ����
	long m_nCountWrapMainRebarForFootingLean;	// �������� ����ö���� ��ö�� ���� ���δ� ����
	long m_nCountWrapMainRebarForFootingPile;	// ���ұ��� ����ö���� ��ö�� ���� ���δ� ����
	long m_nCountWrapMainRebarForArchRib;		// ���� ����ö���� ��ö�� ���� ���δ� ����
	long m_nCountWrapMainRebarForDanbuStirrup_XXX;	// �ܺ� ���ͷ� ö������� ��ö���� ���δ� ����

	long m_nTypeDefaultCycle3_OutRebar;			// ��ö�� cycle3 �ǰ�� ���(����ö�� : ��ν����� ���, ����ö�� ������)
	long m_nTypeDefaultCycle3_UpperSlabLower;	// ��ö�� cycle3 �ǰ�� ���(�� �ν����� �ϸ�)
	long m_nTypeDefaultCycle3_OutWallInner;		// ��ö�� cycle3 �ǰ�� ���(������ü ������)
	long m_nTypeDefaultCycle3_ArchRib;			// ��ö�� cycle3 �ǰ�� ���(��ġ����)
	long m_nTypeCurveOfWallVertRebar;			// ��ü ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
	long m_nTypeStandardCTCRebarMain;			// ��ö�� ctc ����(0 : ��ν�����, 1 : ������ü)
	BOOL m_bRemoveShearRebarAtSlabYundan;		// ������ ���� ����ö�� ����
	long m_nTypeCouplerColumnMainRebar;				// ��� ��ö�� 0 : Ŀ�÷�? 1 : ������. 
	BOOL m_bApplySameSupportRebar;				// ������ ���ö�� ��� ���� ��������
	BOOL m_bApplySameMainRebar;					// ������ ��ö�ٰ��� �Է� ���� ����

	/// ������ ���
	long m_nTypeRebarMainRebarOfExp[2][2];			// ���� ��� Ÿ��(0 : �Ϲ�����, 1 : �)
	long m_nTypeOrgMainRebarOfExp[2][2];				// ������ ��ٽ� ���� Ÿ��(��, ö���� Ȯ��Ǵ� ����) 0 : �����¿���, 1 : ������
	double m_dLenMainRebarOfExp[2][2];				// ������ ��ٽ� �����ο� �¿������ΰ��� ���������� ��ö���� ���� ������ �Ÿ�

	BOOL m_bLineDirRebarOfExp[2][2];			// ���� ö���� ������������ ��ġ����?
	long m_nCountSupportRebarOfExpAtLineDirRebar[2][2];	// ���� ö���� ������������ ��ġ�ɶ� ���� ����ö���� ��� ��ġ����

	// �纸����ġ�϶� ������ġ�� ����Ǵ� ���� �Ÿ�
	// ��±�
	double m_dDistSttJijumDirPlacing[2];			// ���������������� ��ġ�Ǵ� ���ö���� ��ġ�����Ÿ�(�������������� �Ÿ�)
	double m_dDistEndJijumDirPlacing[2];			// ���������������� ��ġ�ȴ� ���ö���� ��ġ�����Ÿ�(�������������� �Ÿ�)
	
	// ��ö��
	double m_dDistLeftDirPlacing;			// ������ ���� �������� ��ġ�Ǵ� ��ö���� ��ġ�����Ÿ�(�������������� �Ÿ�)
	double m_dDistRightDirPlacing;			// ������ ���� �������� ��ġ�Ǵ� ��ö���� ��ġ�����Ÿ�(�������������� �Ÿ�)
	
	// ö�� �ܼ� ����
	CSafetyTypedPtrArray <CObArray, CSetRebarDan*> m_pArrSetRebarDanUpperSlab[2];	// ��ν����� ���ϸ�, 2���̻� ö���� ö������ ����
	CSafetyTypedPtrArray <CObArray, CSetRebarDan*> m_pArrSetRebarDanLowerSlab[2];		// �Ϻν����� 2���̻� ö���� ö������ ����
	CSetRebarDan m_pArrSetRebarDanOutWall[2][2];		// ������, ���� ������ü 2���̻� ö���� ö������ ����
	long m_nCountDanMainRebarRib[MAX_REBAR_CYCLE][2];	// ���� 1~4����Ŭ, ���ϸ� ��ö�� �� ����

	CDoubleArray m_dArrTypeUpperSlabHunch;						// ��� ������ ��ġ�� ö�� Ÿ��(��ü���϶� ���� 0 : X��, 1 : V��)
	CDoubleArray m_dArrTypeLowerSlabHunch;						// �Ϻ� ������ ��ġ�� ö�� Ÿ��(��ü���϶� ���� 0 : X��, 1 : V��)

	// �������� ö��
	BOOL m_bIsDanbuRebar_Cycle1[2][2];			// ��/�� ������ - ������(�������, �������) - ��ġ����.
	BOOL m_bIsDanbuRebar_Cycle2[2][2];			// ��/�� ������ - ������(�������, �������) - ��ġ����.
	double m_dDia_DanbuRebar_Cycle1[2][2][3];	// ��/�� ������ - ������(�������, �������) - ����
	double m_dDia_DanbuRebar_Cycle2[2][2][3];	// ��/�� ������ - ������(�������, �������) - ����

	// �������� ö�� ��ġ
	double m_dDanCorner_UpperSlab[3];			// ��ν����� - 1��,2��,3��
	double m_dDanCorner_LowerSlab[3];			// �Ϻν����� - 1��,2��,3��

	// (ARCBRIDGE-3636) ����� ���θ� ������ ���� �Է°�
	double	m_dDanCorner_UpperVert; 
	double	m_dDanCorner_LowerVert; 
	BOOL	m_bUpperSlab_CornerCurve;
	BOOL	m_bLowerSlab_CornerCurve;

	// �ܺ� ���ͷ� ö�� ����
	double m_dDiaStirrupAtDanBuRebar[2][2];		// ��/�� ������ - �ܺ� ���ͷ� ö�� ���� (������)
	long m_nCountStirrupAtDanBuRebar[2][2];		// ��/�� ������ - �ܺ� ���ͷ� ö�� ���� (������)
	long m_nCountStirrupCtcMainRebar[2][2];		// ��/�� ������ - ��ö�� ����

	long m_nType_DanbuRebar;
	// ������� �ʴ� ����
	long m_nType_DanbuRebar_Cycle1_XXX[2][3];	// ���� 0 : ��, 1 : ����
	long m_nType_DanbuRebar_Cycle2_XXX[2][3];	// ���� 0 : ��, 1 : ����
	long m_nIdxCTC_DanbuRebar_XXX[2][3];		// 2 : ������(�������, �������) - CTC
	double m_dCTC_DanbuRebar_XXX[2][3];			// index����  double�� ���� (2005.04.25) - 1cycle, 2cycle ���� ���
	// ������� �ʴ� ����


	// ����� ö�� ��ġ ����
	BOOL m_bIsBracketRebar[2][2];					// 1,2����Ŭ, ������

	// ��ν����� �а��� ����
	BOOL m_bIsObtuseAngle_UpperSlab[2][2];	// ������, �¿���
	double m_dCoverObtuseAngle_UpperSlab_Plane[2][2][2];	// ������, �¿���, 0:������,1:Ⱦ����
	double m_dCoverObtuseAngle_UpperSlab[2][2][2];	// ������, �¿���, ���ϸ��Ǻ�
	double m_dLenObtuseAngle_UpperSlab[2][2][2];	// ������, �¿���, ���ϸ� ö�� ����
	double m_dLenObtuseAngle_OutWall[2][2];			// ������, �¿��� ��� ö�� ��ü���� ����
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_UpperSlab[2][2][2];	// ������ �¿��� �������� ����(����)
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Main_UpperSlab[2][2][2];	// ������ �¿��� Ⱦ���� ����
	CPlaceRebarByBlock m_placeByBlock_ObtuseAngle_Support_OutWall[2][2];		// ������ �¿��� ��ü �������� ����
	double m_dDiaObtuseAngle_UpperSlabLower_XXX;		// �������� �ϸ� ����ö�� ����
	double m_dDiaObtuseAngle_UpperSlabLower[2][2];		// �������� �ϸ� ����ö�� ���� ( �̺��� �� ���°� ����)
	CRebarInfoCycle m_rebarInfoCycle_ObtuseAngle_Upperslab[2][2][2];		// ������, �¿���, ���ϸ� 

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
	
	

	// ��������ü ������
	BOOL m_bIsAcuteAngle_OutWall[2][2];	// ������ �¿��� ��ü ������ ��ġ ����
	double m_dCoverAcuteAngle_OutWall[2][2][2];	// ��ü ������ �Ǻ�(������,�¿���,���ϸ�)
	double m_dLenAcuteangle_OutWall[2][2];	// ��ü ������ ö�ٱ���(������,�¿���)
	CPlaceRebarByBlock m_placeByBlock_AcuteAngle_MainRebar_OutWall[2][2];	// ��ü ������ ö�� ��ġ ����
	CPlaceRebarByBlock m_placeByBlock_AcuteAngle_SupportRebar_OutWall[2][2];	// ��ü ������ ö�� ��ġ ����
	CRebarInfoCycle m_rebarInfoCycle_AcuteAngle_OutWall[2][2];	// ��ü ������ ö��
	double m_dRange_AcuteAngle_OutWall[2][2];	// ��ü ������ ö�� ������ ����
	/////////////////////////////////////////////////////////////////////////////////////////////////////// ��ö�� ��ġ //

	// ǥ�شܸ� ö�� ��ġ ���� ������ (���ö��, ����ö��)
	// ���ö��
	long m_nTypePlacingSupportRebar_UpperSlab;	// ��ν����� ��±� ��ġ Ÿ��(0 : ����������, 1 : ����������, 2 : �纸��, 3 : �纸��(�����߰�))
	long m_nTypePlacingMainRebar_UpperSlab;		// ��ν����� ��ö�� ��ġ Ÿ��(0 : ��������, 1 : �纸��, 2 : �纸��(�����߰�)
	long m_nTypePlacingMainRebar_Footing;		// ���� ��ö�� ��ġ Ÿ��(0 : ��������, 1 : �纸��, 2 : �纸��(�����߰�)
	double m_dRangeSupportRebarForSameUpperSlab[2];	// ��ν����� ���·� ���̴� ��±� ����
	CPlaceRebarByBlock m_placeByBlock_SupportRebarAddSameUpperSlab[2];	// ��ν����� ó�� ���� ��±��� �߰��� ����
	double m_dDistAddSupportRebarForSameUpperSlab[2][2];	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	long m_nTypeSideRebarMain_UpperSlabSabogang;		// �纸�� ��ġ�� �¿��� ��ö�� Ÿ��(0 : ����, 1 : ��������)
	BOOL m_bFixOutSideMainRebarBy1Cyle;			// �ֿ��� ��ö�� 1cycle�� ����(2, 4����Ŭ�� ��� ����) - ��ü ����, ������ ��� ����

	CPlaceRebarByBlock m_placeByBlock_SupportRebar_UpperSlab[2][MAX_REBAR_DAN];		// ��ν����� ���ϸ�
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_OutWall[2][2][MAX_REBAR_DAN];	// ������ü ������, ������
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_ArchRib[2][MAX_REBAR_DAN][2];			// ���� ���ϸ�(�¿���)		
	CPlaceRebarByBlock m_placeByBlock_SupportRebar_LowerSlab[2][MAX_REBAR_DAN];		// �Ϻν����� ���ϸ�
	double m_dAccuracyArchRibLowerSupportDim;	// ��ġ���� �ϸ� ��±� ġ�� ���е�.

	// ����ö��
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_UpperSlab;		// ��ν����� ����ö�� 
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_LowerSlab;		// �Ϻν����� ����ö�� 
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_OutWall[2];	// ����ü ������ ����ö��
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_Bracket[2];	// ����� ������ ����ö��
	CPlaceRebarByBlock m_placeByBlock_ShearRebar_ArchRib[2];			// ���� ����ö��(�¿���)
	CPlaceRebarByBlock m_placeByBlock_StirrupRebar_ConnectSection[2];		// ���պ� ���ͷ� ö��(�¿���)
	long m_nCountWrapShearRebar_StirrupRebar_ConnectSection[2];				// ���պ� ���ͷ� ö���� ���δ� ����ö�� ����
	long m_nIdxCTC_SupportAndShearRebar;						// ���,����ö�� ǥ�� ��ġ ����(��ν�����, �¿�����ü�������)
	double m_dCTC_SupportRebar[2];		// index���� double�� ����(2005.04.25)
	double m_dCTC_ShearRebar[2];
	double m_dCTC_SupportRebarOutWall[2];
	double m_dCTC_ShearRebarOutWall[2];
	double m_dCTC_ShearRebarBracket;

	// ���� ��� ö�� ��ġ ���� ������(���ͷ� / ��� ��ö��, ��ö��)
	// ������ �׸��� ������ü.. �� ����������ŭ ���� �� �ִ�.
	CGirderSectionRebar m_SlabGirderRebar[2][MAX_JIJUM_COUNT_RC]; // 0: Upper 1: Lower

	// �¿��� ���Ŵ�
	CGirderSectionRebar m_girderRebarJong[2];
	CGirderSectionRebar m_girderRebarJongGagak[2][2];	// ���� ���Ŵ�
	double m_dDistGirderRebarJong[2][2];					// �¿��� ���Ŵ� ��ġ��ġ([iLEFT][iSTT])

	// �����ö��
	double m_dDiaRebarMain_Bracket_Cycle1[2];
	double m_dDiaRebarMain_Bracket_Cycle2[2];
	double m_dDiaRebarSupport_Bracket[2];
	double m_dDiaRebarShear_Bracket_XXX[2];

	// ö�� ����
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarSupport;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarShear;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarEtc;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarGirder;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMonument;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain_User;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarGirder_User;
	// ******************* SerializeRebar �Լ� ����ϴ� ������ ǥ�ü�  ***************************





	// ��ٽ� �ӵ� ������ ���� ���� ���̴� ��ǥ�� ������ �̸� ���� ���� ������ ����. ///////////////////////
	void SettingValueRebarForSlab();		// ��ν����� ��ٵ��� ���� �� ����
	void InitValueRebarForUpperSlab(BOOL bLowerSlab=FALSE);		// ��ν����� ��ٵ��� ���� �� �ʱ�ȭ
	void SettingValueRebarForCrossSection(double dStaLeft, double dStaRight);	// Ⱦ�ܸ鵵 ��ٵ��� ���� �� ����
	void InitValueRebarForCrossSection();	// Ⱦ�ܸ鵵 ��ٵ��� ���� �� �ʱ�ȭ

	CTwinVectorArray m_tvArrSupRebarCross[2];	// Ⱦ�ܸ� ��±� ����
	CDoubleArray m_dArrStaSupRebar[2];	// ��±� �����̼�
	double m_dStaLeftCross;
	double m_dStaRightCross;
	CVectorArray m_xyArrMainRebarCross[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN][2];	// Ⱦ�ܸ� ��±�
	CVectorArray m_xyArrMainRebarCrossHunch[MAX_REBAR_CYCLE][2];	// Ⱦ�ܸ� ��±�

	CTwinVectorArray m_tvArrSupRebarCrossLowerSlab[2];	// Ⱦ�ܸ� ��±� ����
	CDoubleArray m_dArrStaSupRebarLowerSlab[2];	// ��±� �����̼�
	CVectorArray m_xyArrMainRebarCrossLowerSlab[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN][2];	// Ⱦ�ܸ� ��±�
	CVectorArray m_xyArrMainRebarCrossHunchLowerSlab[MAX_REBAR_CYCLE][2];	// Ⱦ�ܸ� ��±�

	// ��� ��ö��, ��±��� ��� �̸� ���س���.(ö�ٸ�ũ�Ҷ� �� ���� ������)
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

	// �Ϻ� ������ ���
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

	// �����ִ� ��� �������� ��ö���� ����Ѵ�.
	// �׷��� �纸�� ���� ��ġ�� ��� ������ ����Ǿ�� �ϴ� �κе� �����Ƿ� 
	// ������ ������ �ִ� �Ǻ������ �����ϰ� �ִٰ� ��Դ´�.
	CTwinVectorArray m_tvArrPlane1DanOrg;	
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	CRcBridgeRebarUtil m_Util;
	long m_nCountRebarDanbu[2][2][2];	// �ܺ� ö�� ����(���, �Ϻ�)(1, 2cycle)(������)

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
