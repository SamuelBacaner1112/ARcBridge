// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGEXPANSIONJOINTREF_H__543E3B43_E210_4CA4_BDF7_D72DD1EE97A6__INCLUDED_)
#define AFX_DIALOGEXPANSIONJOINTREF_H__543E3B43_E210_4CA4_BDF7_D72DD1EE97A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogExpansionJointRef.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogExpansionJointRef dialog
class CExpJointRefArr;
class AFX_EXT_CLASS CDialogExpansionJointRef : public CXTResizeDialog
{
// Construction
public:
	CDialogExpansionJointRef(CExpJointRefArr *pExpJointRefArr, CExpansionJointData* pExpData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogExpansionJointRef)
	enum { IDD = IDD_DIALOG_EXPJOINT_REF };
	CButton	m_btnDefault;
	CButton	m_btnDel;
	CButton	m_btnAdd;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CGridCtrlMng m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogExpansionJointRef)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogExpansionJointRef)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnButtonUserAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

public:
//	CString		m_sPathBitmap;

private:
	CExpJointRefArr		*m_pExpJointRefArr;
	CExpansionJointData	*m_pExpData;

private:
	void MarkIndexSelect();
	void SetGridData();
	void SetExpJointRef();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEXPANSIONJOINTREF_H__543E3B43_E210_4CA4_BDF7_D72DD1EE97A6__INCLUDED_)
