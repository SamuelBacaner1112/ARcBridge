// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcFooting.h: interface for the CARcBridgeCalcFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCFOOTING_H__D0593717_C74D_42A9_AC65_A776F01EBFC9__INCLUDED_)
#define AFX_ARCBRIDGECALCFOOTING_H__D0593717_C74D_42A9_AC65_A776F01EBFC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadFooting.h"	// Added by ClassView
class CLoadFooting;

class AFX_EXT_CLASS CARcBridgeCalcFooting  
{
public:
	CARcBridgeCalcFooting();
	virtual ~CARcBridgeCalcFooting();
	
	CARcBridgeCalcStd *m_pStd;

	CTypedPtrArray <CObArray, CLoadFooting*> m_pListLoadUlt[MAX_JIJUM_COUNT_RC];	// ������ �ۿ����߿� ���� �迭(�������);
	CTypedPtrArray <CObArray, CLoadFooting*> m_pListLoadUse[MAX_JIJUM_COUNT_RC];	// ������ �ۿ����߿� ���� �迭(�������);
	CTypedPtrArray <CObArray, CLoadFooting*> m_pListLoadSafty[MAX_JIJUM_COUNT_RC];	// ������ �ۿ����߿� ���� �迭(���������);
	CTypedPtrArray <CObArray, CLoadFooting*> m_pListLoadLsd[MAX_JIJUM_COUNT_RC][eCountEnvTypeLsd];

	std::vector<CLsdFootingDesign*> m_pListLsdFootingSafe;
	
	// 0: �ձ� ����� 1: �ޱ� ����� 2: �ձ�2
	double m_dATensionUpperMUse[MAX_JIJUM_COUNT_RC][4];				// ������� ������� ���Ʈ
	double m_dATensionLowerMUse[MAX_JIJUM_COUNT_RC][4];				// ������� �ϸ����� ���Ʈ
	double m_dATensionUpperMUlt[MAX_JIJUM_COUNT_RC][4];				// ������� ������� ���Ʈ
	double m_dATensionLowerMUlt[MAX_JIJUM_COUNT_RC][4];				// ������� �ϸ����� ���Ʈ
	double m_dATensionUpperVUlt[MAX_JIJUM_COUNT_RC][4];				// ������� ������� ���ܷ�
	double m_dATensionLowerVUlt[MAX_JIJUM_COUNT_RC][4];				// ������� �ϸ����� ���ܷ�

	double m_dATensionUpperMLsd[MAX_JIJUM_COUNT_RC][eCountEnvTypeLsd][4];				// LSD ������� ���Ʈ
	double m_dATensionLowerMLsd[MAX_JIJUM_COUNT_RC][eCountEnvTypeLsd][4];				// LSD �ϸ����� ���Ʈ
	double m_dATensionUpperVLsd[MAX_JIJUM_COUNT_RC][eCountEnvTypeLsd][4];				// LSD ������� ���ܷ�
	double m_dATensionLowerVLsd[MAX_JIJUM_COUNT_RC][eCountEnvTypeLsd][4];				// LSD �ϸ����� ���ܷ�

	// ������ ��꿡 MASS���� ���Խ�
	double m_dB[MAX_MASS_DAN*2];
	double m_dH[MAX_MASS_DAN*2];
	double m_dA[MAX_MASS_DAN*2];
	double m_dDist[MAX_MASS_DAN*2];
	double m_dVer[MAX_MASS_DAN*2];
	double m_dHor[MAX_MASS_DAN*2];
	double m_dMoment[MAX_MASS_DAN*2];

	long GetCountLoadFootingLsd(long nJijum, ETypeEnvComboLsd eEnvCombo);
	CLoadFooting* GetLoadFootingLsd(long nJijum, ETypeEnvComboLsd eEnvCombo, long ixCheck);
	CLsdFootingDesign* GetLsdFootingDesign(long nJijum);

	void CalculateAll();
	void CalculateAll_Lsd();
	void CalculateAll_Usd();
	void WriteCalcFootingSafeCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcFootingSupportCheck(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake);
	void WriteCalcFootingSupportCheck_Lsd(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake);
	void WriteCalcFootingBuoyancyCheck(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle);
	void WriteCalcFootingPile(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle);
	void WriteCalcFootingDesign(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcFootingDesignLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	BOOL GetCheckValueOverturning(long nJijum, double *dMr, double *dMo, BOOL bNormal);
	BOOL GetCheckValueSliding(long nJijum, double *dHu, double *dH, BOOL bNormal);
	BOOL GetCheckValueSupport(long nJijum, double *dQa, double *dQmax, BOOL bNormal, ELSDLoadGroupKind eKindLsd=eLSD_COMBINATION_LIMIT);
	void GetCheckValuePileForce(long nJijum, double *dRa, double *dPmax, BOOL bNormal);
	void GetCheckValuePileBending(long nJijum, double *dFsa, double *dFmax, BOOL bNormal, BOOL bTens=TRUE);
	void GetCheckValuePileShear(long nJijum, double *dTsa, double *dTmax, BOOL bNormal);
	void GetCheckValuePileDisplacement(long nJijum, double *dDisa, double *dDismax, BOOL bNormal);
	void GetCheckValuePileForceMinus(long nJijum, double *dPa, double *dPmin, BOOL bNormal);
	void GetCheckValueHyuntaPileAxialForce(long nJijum, double *dPiPn, double *dAxial, BOOL bNormal);

	void CalculateSection_Usd();
	void CalculateSection_Lsd();
	double GetDistMaxPile(long nIdxOrg);

	void MakeTableLoadSum(CXLFormatText *XLText, long nIndex, BOOL bUseLoad, BOOL bSafety, long nType=0, BOOL bAdd=FALSE, long nLsdEnv = eEnvLimit);	// nType 0: ��������, 1: ���ұ���
	void MakeTableLoadForFooting_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bAdd, BOOL bService1, BOOL bService2);
	void MakeTableLoadCombo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, BOOL bSafeCheck);
	void MakeTableLoadCombo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, const std::vector<ELSDLoadCaseKind> &vecLoadCaseKind, BOOL bSafeCheck);
	void MakeTableLoadFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, BOOL bSafeCheck);
	void MakeTableLoadSubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle);
	void WriteSectionDesign_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle);
	void WriteSectionForceBySubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n);
	void WriteDesignReaction_Direct(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n);
	void WriteSectionDesign_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle);
	void MakeTableLoad_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront);
	void WriteDesignReaction_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n);

	HGINT32 GetCountLoadFooting(CRcBridgeRebar *pBri, BOOL bSafeCheck);

	// �η� �� ������ (BOX��)
	void CalculateSupportCheck_Usd(BOOL bEarthquake);
	void CalculateSupportCheck_Lsd(BOOL bEarthquake);
	void CalculateBuoyancyCheck();

	CStringArray m_sArrSelfWeight[3];
	CStringArray m_sArrSupport;
	double m_dVerTotal;
	double m_dMrTotal;
	double m_dMoTotal;
	double m_dQaEquation;
	double m_dQMax[eCountEnvTypeLsd-1];				// ����, �ش�1, �ش�2, ��� 
	double m_dLoadFactor[eCountEnvTypeLsd-1][5];		// DC, DW, EV, Ȱ����, ���μ���

	CDoubleArray m_dArrDispX[4]; // ���,���,����(2)
	CDoubleArray m_dArrDispZ[4];
	CDoubleArray m_dArrDispR[4];

	double m_dSelfWeight;	// ��ü����
	double m_dPaveWeight;	// �������� DW
	double m_dMassWeight;   // Mass����
	double m_dAdditionalDC;	// DC �߰�����
	double m_dAdditionalDW;	// DW �߰�����
	double m_dLiveLoad;
	double m_dBuoyance;
	double m_dSideFriction;
	double m_dProtectionRiseFooting;
	double m_dSF_Buoyance;
	double m_dInteriorWaterWeight;

private:
	void CalculateLoad_Lsd();
	void CalculateLoad_Usd();
	void CalculateFootingAndPile_Usd();
	void CalculateFootingAndPile_Lsd();
	void CalculateFootingAndPile_BoxType_Usd();
	void CalculateFootingAndPile_BoxType_Lsd();
	void DrawFootingSection(CDomyun *pDomP,  CRcBridgeRebar *pBri, long nJijum);
	void DrawFootingSection_Type_Trap2(CDomyun *pDomP,  CRcBridgeRebar *pBri, BOOL bStt);		// BridgeType�� BRIDGE_TYPE_TRAP2�϶� ���� �׷��ش�.
	void MakeTableCenterPile(CXLFormatText *XLText, long nJijum);
	void MakeTableCenterPileLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void MakeTableSectionForceSum(CXLFormatText *XLText, long nIndex);
	void MakeTableSectionForceSum_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteSupportCheckFooting(CXLFormatText *XLText, long nIndexSub, long nJijum, BOOL bFront);
	void WriteCheckFootingRigidBody(CXLFormatText *XLText, long nIndex, long nJijum);
	long GetCountSectMASS(long nJijum);
	void CalculateForceMASS(long nJijum);
	// nIndCol - 0:B(m), 1:H(m), 2:A(m2), 3:�Ÿ�(m), 4:V(ton), 5:H(ton), 6:M(ton.m)
	double GetValueForceMASS(long nJijum, long nIndSect, long nIndCol, BOOL bTot=FALSE);
	double GetQmaxMASS(long nJijum);
	void MakeTableForceMASS(CXLFormatText *XLText, long nJijum);
	// ������౳ ���Ұ���
	void MakeTablePileForceAndDisp_Usd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileReactionCheck_Usd(CXLFormatText *pXLText, long nPos);
	void WriteHyuntaPileCheckPM_Usd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileStressCheckPHC_Usd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileStressCheck_Usd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileShearStressCheck_Usd(CXLFormatText *pXLText, long nPos);

	void MakeTablePileForceAndDisp_Lsd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileReactionCheck_Lsd(CXLFormatText *pXLText, long nPos);
	void WriteHyuntaPileCheckPM_Lsd(CXLFormatText *pXLText, long nPos, long nTypeCase);
	void MakeTablePileStressCheckPHC_Lsd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileStressCheck_Lsd(CXLFormatText *pXLText, long nPos);
	void MakeTablePileShearStressCheck_Lsd(CXLFormatText *pXLText, long nPos);
	void WritePileCheckMain_Lsd(CXLFormatText *pXLText, long nPos);		// �Ѱ���� ���Һ�ü����
	void WritePileOutputSafeCheck_AxialForce_Lsd(CXLFormatText *pXLText, long nPos);
	void WritePileOutputSafeCheck_Fa_Lsd(CXLFormatText *pXLText, long nPos);
	void WritePileOutputSafeCheck_ShaerStress_Lsd(CXLFormatText *pXLText, long nPos);

	void CheckFootingSafeLSD();
	void SetLsdFooting(long nJijum);
	HGBOOL IsUsedLoadCase(CLsdManager *pMng, ELSDCombKind eComboKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case, long nCombo);

	HGDOUBLE GetCenterOfFigure(CRcBridgeRebar *pBri, HGINT32 ixJijum);

	void WriteCalcSupport_Usd(CXLFormatText *XLText, long nJijum, long &nIndexSmall, BOOL bEarthquake);
	void WriteCalcSupport_Lsd(CXLFormatText *XLText, long nJijum, long &nIndexSmall, BOOL bEarthquake);

	HGDOUBLE GetValueQ2(CLoadFooting *pData) const;
	HGDOUBLE GetValueQ1(CLoadFooting *pData) const;
	HGDOUBLE GetLengthBox();
	//
	double m_dPr;
	double m_dPf;
	double m_dPm;
	double m_dLive1;
	double m_dLive2;
};
#endif // !defined(AFX_ARCBRIDGECALCFOOTING_H__D0593717_C74D_42A9_AC65_A776F01EBFC9__INCLUDED_)
