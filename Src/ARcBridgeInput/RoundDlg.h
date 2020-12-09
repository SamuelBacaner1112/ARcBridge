// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_ROUNDDLG_H__7D364C8A_F368_41FF_A634_563390B52DF1__INCLUDED_)
#define AFX_ROUNDDLG_H__7D364C8A_F368_41FF_A634_563390B52DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoundDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRoundDlg dialog

class CRoundDlg : public CDialog
{
// Construction
public:
	CRoundDlg(CWnd* pParent = NULL);   // standard constructor
	CRoundDlg(CColumnApp *pColumn, CARcBridgeDataStd *pStd, CInputDialogBase* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoundDlg)
	enum { IDD = IDD_DIALOG_ROUND };
	CXTPComboBox	m_comboRoundOut;
	CXTPComboBox	m_comboRoundIn;
	CButton	m_btnOk;
	CStatic	m_staticRound;
	CButton	m_btnRecommand;
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoundDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoundDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangeComboRound();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboRoundIn();
	afx_msg void OnSelchangeComboRoundOut();
	afx_msg void OnButtonRecommand();
	//}}AFX_MSG
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
public:
	void SetDataInit();
	void SetGridData();
	void SetDataSave();
	CColumnApp *m_pColumn;
	CARcBridgeDataStd* m_pStd;
private:
	void SetControl();
	void InitCombo();
	CInputDialogBase *m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUNDDLG_H__7D364C8A_F368_41FF_A634_563390B52DF1__INCLUDED_)
