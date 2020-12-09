// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SLABSHAPEDLG_H__A581EFE5_AC84_46A9_8CE7_BEE527EEED74__INCLUDED_)
#define AFX_SLABSHAPEDLG_H__A581EFE5_AC84_46A9_8CE7_BEE527EEED74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlabShapeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSlabShapeDlg dialog

class CSlabShapeDlg : public CDialog
{
// Construction
public:
	CSlabShapeDlg(CARcBridgeInputStd* pIn, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSlabShapeDlg)
	enum { IDD = IDD_DIALOG_SLAB_SHAPE };
	CXTHyperLink	m_hyperHelp;
	CButton	m_btDefault;
	CButton	m_btOK;
	CDomyunView m_DomView;
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA

private:
	CARcBridgeInputStd* m_pIn;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlabShapeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSlabShapeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLABSHAPEDLG_H__A581EFE5_AC84_46A9_8CE7_BEE527EEED74__INCLUDED_)
