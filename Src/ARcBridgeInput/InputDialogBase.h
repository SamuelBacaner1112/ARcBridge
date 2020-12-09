// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTDIALOGBASE_H__D2B2291F_897F_4D36_B534_EE53558C8043__INCLUDED_)
#define AFX_INPUTDIALOGBASE_H__D2B2291F_897F_4D36_B534_EE53558C8043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLR_BACK	 RGB(253, 250, 255)
#define CLR_READONLY RGB(255, 242, 200)

// InputDialogBase.h : header file
//

#include "resource.h"
class CRcBridgeData;

/////////////////////////////////////////////////////////////////////////////
// CInputDialogBase dialog

class CARcBridgeInputStd;

class AFX_EXT_CLASS CInputDialogBase : public CXTResizeDialog
{
// Construction
public:
	BOOL IsRedrawInputApply();
	
	double m_dScaleDefault;
	double m_dScaleOffset;
	BOOL IsClickDefaultButton(long nBri);
	BOOL m_bClickDefaultButton;	// 권고안을 할때 버튼을 눌러서 들어왔는지 그냥 들어온건지 체크함.
	BOOL m_bClickApplyButton;	// 적용 버튼을 눌렀는지?
//	void MoveControlGroup(CDoubleArray &arrID, CPoint xyMove, long nSpeed, long nDelay, double dBoundRate);
	CDomyunGLView* m_pView;
	CARcBridgeInputStd* m_pIn;
	CInputDialogBase(UINT nID, CWnd* pParent = NULL);   // standard constructor

	virtual BOOL IsCheckMoveOtherDlg() { return TRUE; }	// IsOKMOveOtherDlg의 영향을 받는 입력창인지?
	virtual void KillFocusDlg() { }	// 현재 입력창에서 벗어 나는 경우
	virtual BOOL IsOKMoveOtherDlg() { return TRUE; }	// 다른 입력창으로 이동해도 되는지?
	virtual void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	virtual void OnPreInitDialog() {}
	virtual void SetDataSave() {}
	virtual void SetDataInit() {}
	virtual void SetDataDefault() {}
	virtual void SetGridData() {}
	//virtual void SetGridTitle() {}
	virtual BOOL IsValid() { return TRUE; }
	virtual void SetFocusItem(CString szTitle);
	virtual BOOL IsFocusItem(CString &szTitle);
	virtual void OnCursorChange(long nRow, long nCol);
	virtual void SetUnlock() {}

// Dialog Data
	//{{AFX_DATA(CInputDialogBase)
	CButton	m_btnPrev;
	CButton	m_btnNext;
	CButton	m_btnApply;
	CButton	m_btnDefault;
	CButton	m_btnExamine;
	CXTHyperLink	m_hyperHelp;
	//}}AFX_DATA

	BOOL IsFirst(long nBri);
	BOOL IsDefaulted(long nBri, DWORD dwDefault);
	BOOL IsGL() { return m_bGL; }
	BOOL IsUsedFlag() { return m_bUsedFlag; }
	UINT GetIDD() const	{ return m_nIDD; }
	UINT GetMenuID() const { return m_nMenuID; }
	CDomyunGLView *GetView() const { return m_pView; }
	void SetParentStd(CARcBridgeInputStd* pIn) { m_pIn = pIn; }
	void SetMenuID(UINT nID) { m_nMenuID = nID; }
	void SetUsedFlag(BOOL bFlag) { m_bUsedFlag = bFlag; }
	void SetPaneTitle();
	void SetHyperHelp(CString strPath);
	
	long m_nTab;
	void SetTabCurSel(CTabCtrl *pTab);
	virtual void SetControl() {}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDialogBase)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_nIDD;
	UINT m_nMenuID;
	BOOL m_bUsedFlag;
	BOOL m_bGL;
	BOOL m_bUbicon;
	long m_nIDGrid;				// 포커스 아이템 체크시 사용 될 그리드 
	long m_nRowFocusItemTitle;	// 포커스 아이템 제목이 있는 열

	BOOL m_bDesignLsd;

	// Generated message map functions
	//{{AFX_MSG(CInputDialogBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual afx_msg void OnPaint();
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual afx_msg void OnButtonApply();
	virtual afx_msg void OnButtonNext();
	virtual afx_msg void OnButtonPrev();
	virtual afx_msg void OnButtonDefault();
	virtual afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	virtual afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	virtual afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsPrintPreview();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDIALOGBASE_H__D2B2291F_897F_4D36_B534_EE53558C8043__INCLUDED_)
