// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcAps.h: interface for the CARcBridgeCalcAps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCAPS_H__067ECARcBridgeCalcAps1265_7599_4695_89FB_1616F56E5748__INCLUDED_)
#define AFX_ARCBRIDGECALCAPS_H__067E1265_7599_4695_89FB_1616F56E5748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define APS_CASE_ULT 0
#define APS_CASE_USE 1
#define APS_CASE_USE_JUDGE 2
#define APS_CASE_COUNT	3

class AFX_EXT_CLASS CARcBridgeCalcAps  
{
public:
	CARcBridgeCalcAps( CApsApp* pAps, BOOL bStt);
	virtual ~CARcBridgeCalcAps();

	CApsApp *m_pAps;
	long	m_nSelectApsShearCheck;
	double	m_dLoadFactorDeadLoad[APS_CASE_COUNT][3];	// [ULT/USE][DC/DW/EV]
	double	m_dLoadFactorLiveLoad[APS_CASE_COUNT];

	BOOL m_bLsd;
	CString m_szCombo[APS_CASE_COUNT];
	double  m_dEta[APS_CASE_COUNT]; // ����Ѱ�, �����Ѱ�
	double m_dMu[APS_CASE_COUNT];
	double m_dVu[APS_CASE_COUNT];
	
	BOOL IsCobel();
	double GetVForceCorbel(BOOL bDeadLoad, long nLoadFactor=-1);
	BOOL IsApplyApsAxleWeight(CRcBridgeRebar *pBri);
private:
	void CalculateCobel();
	
	BOOL m_bStt;
	double m_LenAps;		// ���ӽ������� ����
	double m_dSpan;			// ��������
	double m_dWdp;			// ��������(DW)
	double m_dWdw;			// ����(DC)
	double m_dWds;			// �������(EV)
	double m_dDB;			// DB����
	double m_dCo_i;			// ��ݰ��
	double m_dLe;			// ������ ������
	double m_dD;			// 
	double m_dMd[3];		// �������� ���Ʈ  [DC/DW/EV]
	double m_dVd[3];		// �������� ���ܷ�
	double m_dMl;			// Ȱ���� ���Ʈ
	double m_dVl;			// Ȱ���� ���ܷ�
	double m_dVlDist;		// ���ܷ� ������ġ (Ȱ����)
	double m_dPs;			// ������
	double m_VuCobel;		// �ں��� ���ܷ�	

	double m_UW_Ascon;		// ��������߷�
	double m_UW_Conc;		// ��ũ��Ʈ �����߷�
	
	//
	BOOL m_bConcAps;		// ��ũ��Ʈ ����
	
	void WriteCrackCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteDesignAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteRebarCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteDesignCobel(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	
public:
	void SetConcData();
	void CalculateAll();
	void CalculateAps();
	void WriteCalcAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bWriteOne=TRUE);
	
	CARcBridgeCalcStd *m_pStd;
	
	CConcBasic *m_pConc;
	CConcBasic *m_pConcCobel;
	CConcData  *m_pConcCobel_; // �ӽ�..
};

#endif // !defined(AFX_ARCBRIDGECALCAPS_H__067E1265_7599_4695_89FB_1616F56E5748__INCLUDED_)
