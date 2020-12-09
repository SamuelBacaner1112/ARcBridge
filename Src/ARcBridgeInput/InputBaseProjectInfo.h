// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASEPROJECTINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_)
#define AFX_INPUTBASEPROJECTINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseProjectInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseProjectInfo dialog

class CInputBaseProjectInfo : public CInputDialogBase
{
// Construction
public:
	CInputBaseProjectInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseProjectInfo)
	enum { IDD = IDD_BASE1_PROJECTINFO };
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseProjectInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseProjectInfo)
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog(); 
	void SetDataSave();
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASEPROJECTINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_)
