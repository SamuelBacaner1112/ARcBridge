// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#include "afxcmn.h"
#if !defined(AFX_CHECKCANTILEVERDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_)
#define AFX_CHECKCANTILEVERDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCheckCantileverDlg ��ȭ �����Դϴ�.

class CARcBridgeCalcStd;
class CCheckCantileverDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckCantileverDlg)

public:
	CCheckCantileverDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CCheckCantileverDlg(CRcBridgeRebar *pBri, long nCurrentBridge, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckCantileverDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CHECK_CANTILEVER };

	CButton	m_btnOk;
	CButton	m_btnCancel;
	CButton	m_btnRecommand;
	CButton	m_btnExamine;

	CARcBridgeCalcStd *m_pCalc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnRecommond();
	afx_msg void OnExamine();
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCellChangedData(NMHDR *pNMHDR, LRESULT*);
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	CLsdGridCtrl	m_Grid;
	CRcBridgeRebar* m_pBri;
	long m_nCurrentBridge;

	void SetDataDefault(HGBOOL bAll=FALSE);
	void InitTabCtrl();
	void SetDataSave();
	void SetDataInit();
	void SetGridData();
	
	void SetCalcStdData();

	void SetGridDataSelectLoadLsd();		// �������߼���
	void SetGridData_CombineLoad();		// �������ռ���
	void SetGridData_Limit();			// �����Ѱ����
	void SetGridData_Extremel();		// �شܻ�Ȳ�Ѱ����
	void SetGridData_UseLimit();		// ����Ѱ����
	void SetGridData_Eta();				// ���߼������
	void SetGridData_Design();			// ���輱�û���
	void SetGridData_WorkingLoad();		// �ۿ����߼���
	void SetGridData_LsdCombo(long nTypeLsdCombo);

public:
	CXTPTabCtrl m_Tab;

	long m_nTabSelLoad;			// �������߼���
	long m_nTabLoadCombine;		// �������ռ���
	long m_nTabLimit;				// �����Ѱ����
	long m_nTabExtremel;			// �شܻ�Ȳ�Ѱ����
	long m_nTabUseLimit;			// ����Ѱ����
	long m_nTabEta;				// ����Ѱ����
	long m_nTabDesign;				// ���輱�û���
	long m_nTabLoad;				// �ۿ����߼���
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUAKEGRADEDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_)
