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
// CCheckCantileverDlg 대화 상자입니다.

class CARcBridgeCalcStd;
class CCheckCantileverDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckCantileverDlg)

public:
	CCheckCantileverDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CCheckCantileverDlg(CRcBridgeRebar *pBri, long nCurrentBridge, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckCantileverDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CHECK_CANTILEVER };

	CButton	m_btnOk;
	CButton	m_btnCancel;
	CButton	m_btnRecommand;
	CButton	m_btnExamine;

	CARcBridgeCalcStd *m_pCalc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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

	void SetGridDataSelectLoadLsd();		// 설계하중선택
	void SetGridData_CombineLoad();		// 하중조합선택
	void SetGridData_Limit();			// 극한한계상태
	void SetGridData_Extremel();		// 극단상황한계상태
	void SetGridData_UseLimit();		// 사용한계상태
	void SetGridData_Eta();				// 하중수정계수
	void SetGridData_Design();			// 설계선택사항
	void SetGridData_WorkingLoad();		// 작용하중선택
	void SetGridData_LsdCombo(long nTypeLsdCombo);

public:
	CXTPTabCtrl m_Tab;

	long m_nTabSelLoad;			// 설계하중선택
	long m_nTabLoadCombine;		// 하중조합선택
	long m_nTabLimit;				// 극한한계상태
	long m_nTabExtremel;			// 극단상황한계상태
	long m_nTabUseLimit;			// 사용한계상태
	long m_nTabEta;				// 사용한계상태
	long m_nTabDesign;				// 설계선택사항
	long m_nTabLoad;				// 작용하중선택
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUAKEGRADEDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_)
