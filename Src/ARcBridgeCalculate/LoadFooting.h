// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// LoadFooting.h: interface for the CLoadFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_)
#define AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VER_LOAD_LIVE		0
#define VER_LOAD_FOOTW		1
#define VER_LOAD_PAVE		2
#define VER_LOAD_BACKFILL	3
#define VER_LOAD_DEAD		4
#define VER_LOAD_COUNT		5

class CFootingApp;

class AFX_EXT_CLASS CLoadFooting : public CObject
{
public:
	CLoadFooting(CARcBridgeCalcStd *pStd, CRcBridgeRebar *pBri, long nJijum, long nIdex, BOOL bUseLoad, ETypeEnvComboLsd eEnvType=eEnvService);	// 0: �ձ��ִ�, 1: �ޱ��ִ�, 2: ����ִ�, 3: ������	
	virtual ~CLoadFooting();

	CARcBridgeCalcStd *m_pStd;
	CRcBridgeRebar *m_pBri;
	
	CFootingApp *m_pFooting;
	
	CString m_sTitle;
	CString m_sTitleSub;
	CString m_sCombo;
	long m_nCombo;					// ����� �������� Index
	long m_nJijum;
	long m_nIndex;					// 0: ����ִ�, 1: �θ��Ʈ �ִ�, 2: �����Ʈ �ִ�, 3: ������
	BOOL m_bUseLoad;
	BOOL m_bEarthQuake;
	BOOL m_bApplyHoriEarQ;			// ���Ž� ������ ���� ����
	BOOL m_bPile;					// ���ұ��ʸ� TRUE, �������ʸ� FALSE

	ETypeEnvComboLsd m_eEnvComboType;

	double m_dHightBackFill;		// ��ä�� ����
	double m_dFactorDeadLoad;		// �������� ���
	double m_dFactorSoilHor;		// ������� ���
	double m_dFactorPave;			// �������� ���
	double m_dFactorLiveLoad;		// Ȱ���� ���
	// ������, ������
	// 0: ��� Ȱ����, 1: ��������, 2: ����, 3: ������
	double m_dVerLoad[VER_LOAD_COUNT];
	double m_dHorLoad[VER_LOAD_COUNT];

	double m_dQmaxMass;				// ������ ���信 MASS�ݷ��� ���� ��ų��
	// Xi, Yi : ���������� �ۿ��� ������ �Ÿ�
	double m_dXiVerLoad[VER_LOAD_COUNT];
	double m_dYiHorLoad[VER_LOAD_COUNT];
	double m_dMr[VER_LOAD_COUNT];
	double m_dMo[VER_LOAD_COUNT];
	double m_dA;						// ��������� ���

	double m_dVerSmallTotal;		// �Ұ�
	double m_dHorSmallTotal;		// �Ұ�
	double m_dMrSmallTotal;
	double m_dMoSmallTotal;

	double m_dHightTosa;			// ������� ����

	// ��� ��������
	double m_dUpperLoadV;			// ������� ������
	double m_dUpperLoadH;			// ������� ������
	double m_dUpperLoadM;			// ������� ���Ʈ

	double m_dXiUpperLoadV;
	double m_dYiUpperLoadH;

	double m_dMrUpperV;
	double m_dMoUpperV;
	double m_dMrUpperH;
	double m_dMoUpperH;
	double m_dMrUpperM;
	double m_dMoUpperM;

	// ���������� BRIDGE_TYPE_TRAP2�϶� ��� ������ �ϳ��� �߰� �ȴ�.
	BOOL m_bTwoUpperLoad;
	double m_dUpperLoadV_Add;
	double m_dUpperLoadH_Add;
	double m_dUpperLoadM_Add;
	long m_nCombo_Add;
	
	double m_dXiUpperLoadV_Add;
	double m_dYiUpperLoadH_Add;

	double m_dMrUpperV_Add;
	double m_dMoUpperV_Add;
	double m_dMrUpperH_Add;
	double m_dMoUpperH_Add;
	double m_dMrUpperM_Add;
	double m_dMoUpperM_Add;

	// �Ѱ�
	double m_dVerTotal;
	double m_dHorTotal;
	double m_dMoTotal;
	double m_dMrTotal;

	double m_dDistCenttoDosim;
	double m_dPileLoadV;			// ���ϱ��� ���� �ۿ��
	double m_dPileLoadH;
	double m_dPileLoadM;
	double m_dPileLoadMCen;			// ���� �߽ɿ����� �ۿ� ���Ʈ

	// ���� �ݷ�
	CDoubleArray m_dArrPileFoce;
	CDoubleArray m_dArrDistPile;	// ���������� ���ϱ����� �Ÿ�
	CStringArray m_szAddPileNo;
	CStringArray m_szAddPileFoceString;
	CDoubleArray m_dAddPileFoce;

	double m_dE;					// ���
	double m_dLx;					// �ٴڸ� �ݷ��� �ۿ���
	double m_dQmax;					// ���ʹٴڸ鿡���� �ִ����ݹݷ�
	double m_dQmin;					// ���ʹٴڸ鿡���� �ּ����ݹݷ�
	double m_dQa;					// ���ʹٴڸ鿡���� �ִ����������
	double m_dHu;					// ��� �����
	BOOL m_bQTriangle;				// TRUE: �ﰢ�� ����, FALSE: ��ٸ��� ����
	BOOL m_bQmaxFront;				// TRUE: Qmax�� �ձ���, FALSE: Qmax�� �ޱ���
	BOOL m_bOkSupportForce;			// �����¿� ���� ������
	BOOL m_bOkSliding;				// Ȱ���� ���� ������
	BOOL m_bOkOverturning;			// ������ ���� ������

	double m_ScF;			// ���� ���߿� ���� �ܸ��
	double m_ScB;
	double m_McF;
	double m_McF2;
	double m_McF3;
	double m_McB;
	double m_SqF;			// ���� �ݷ¿� ���� �ܸ��
	double m_SqB;
	double m_MqF;
	double m_MqF2;
	double m_MqF3;
	double m_MqB;
	double m_SsB;			// ��ä�� ��翡 ���� �ܸ��
	double m_MsB;
	double m_SpB;			// ��ä�� ���忡 ���� �ܸ��
	double m_MpB;
	double m_SlB;			// ���Ȱ���߿� ���� �ܸ��
	double m_MlB;			

	// �ձ�,�ޱ��� �ۿ�����(���� �ܸ��)
	double m_dDistMf;
	double m_dDistMb;
	double m_dDistMf2;
	double m_dDistMf3;
	double m_dDistSf;				// ���������κ����� �ձ� ���ܸ� �Ÿ�
	double m_dDistSb;				// ���������κ����� �ޱ� ���ܸ� �Ÿ�
	double m_dHgtSf;				// �ձ� ���ܰ��� ����;
	BOOL m_bOutBoundFootFront;
	BOOL m_bOutBoundFootBack;

	double m_dQmF;
	double m_dQmF2;
	double m_dQmF3;
	double m_dQmB;
	double m_dQvF;
	double m_dQvB;
	double m_dVuFront;
	double m_dVuBack;
	double m_dMuFront;
	double m_dMuFront2;
	double m_dMuFront3;
	double m_dMuBack;

	// �������
	double m_dQaEquation;
	double m_dA_Equation;
	
	void CalculateAll();
	void MakeTableOtherLoad(CXLFormatText *XLText);
	void MakeTableOtherLoadLsd(CXLFormatText *XLText);
	void MakeTableLoadAll(CXLFormatText *XLText, long nIndexTiny, BOOL bTable=TRUE);
	void WriteCheckOverturning(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteCheckSliding(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteCheckSupportForce(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteSectionDesign(CXLFormatText *XLText, BOOL bUse, BOOL bFirst);
	void WriteSectionDesignFront(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack(CXLFormatText * XLText, long nIndexSub, BOOL bUse);
	void WriteSectionDesignFront_Lsd(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack_Lsd(CXLFormatText * XLText, long nIndexSub, BOOL bUse);
	void WriteSectionDesign_Pile(CXLFormatText *XLText, BOOL bUse, BOOL bFirst);
	void WriteSectionDesign_PileText(CXLFormatText * XLText, long nIndexSub, HGBOOL bFront);
	void WriteSectionDesignFront_Pile(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack_Pile(CXLFormatText *XLText, BOOL bUse);

	void WriteForceCenter_Pile(CXLFormatText *XLText);
	void CalculateBaseLoad();
	void CalculateApplyLoad();
	void CalulatePileCentroidLoad();
	void CalulateQaMax();
	void CalculateSupportForce();
	void CalculateSlidingCheck();
	void CalculateOverturningCheck();
	// nType = 0: ��ü, 1: �ձ�, 2: �ޱ� 3: �ձ�������ġ, 4: �ޱ�������ġ
	void GetPileForcArr(long nType = 0, BOOL bUnitM = TRUE);				// ���� �ݷ��� �����´�.
	void CalculateSectionDesign();
	void GetDistFrontTrap2AndArch(double &dDist, double &dHeight);

	void MakeTableFootCenter(CXLFormatText *XLText);
	void DrawSupportForce(CDomyun *pDomP, long nType, BOOL bSection);
	CString GetStringVertLoad(HGINT32 iIndexLoad, HGBOOL bMark=TRUE);
private:
	void WriteCalcQubyHansen(CXLFormatText *XLText, BOOL bFirst);
	void WriteCalcQubyTerzaghi(CXLFormatText *XLText, BOOL bFirst);
	void WriteCalcQubyMeyerhof(CXLFormatText *XLText, BOOL bFirst);
	double GetPileFootingCentroid(long nJijum);	// ���� ������ ����
	double GetDistSlopeBack();  // ��� �����϶� ���������� ���� �ޱ� �������� ������ �Ÿ�
};

#endif // !defined(AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_)
