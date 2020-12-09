// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASELINEINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_)
#define AFX_INPUTBASEPLINEINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseLineInfo.h : header file
//

#include "InputDialogBase.h"

/////////////////////////////////////////////////////////////////////////////
// CInputBaseLineInfo dialog

class CLineInfoApp;
class CDialogMakeLineInfo;

class CInputBaseLineInfo : public CInputDialogBase
{
// Construction
public:
	BOOL IsCheckMoveOtherDlg();
	CInputBaseLineInfo(CWnd* pParent = NULL);   // standard constructor
	~CInputBaseLineInfo();

// Dialog Data
	//{{AFX_DATA(CInputBaseLineInfo)
	enum { IDD = IDD_BASE3_LINEINFO };
	CButton	m_btInsert;
	CStatic		m_stcLineName;
	CEdit		m_edtLineName;
	CButton		m_rdVertical;
	CButton		m_rdPlane;
	CButton	m_btPlane;
	CButton	m_btSlope;
	CButton	m_btVertical;
	CButton	m_btSymLR;
	CButton	m_btEQLR;
	CButton	m_btAdd;
	CButton	m_btDelete;
	CButton	m_btLineOpen;
	CEdit		m_edtFilePath;
	CXTPTabCtrl	m_Tab;
	CGridCtrlMng	m_Grid;
	int			m_nLineImport;
	CString	m_strFilePath;
	CString	m_strLineName;
	CString	m_strStationStt;
	CString	m_strHeightIp;
	CComboBox m_ctlLineSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseLineInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void KillFocusDlg();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseLineInfo)
	afx_msg void OnRadioLineImport();
	afx_msg void OnRadioLineInput();
	afx_msg void OnButtonLineOpen();
	afx_msg void OnButtonLineSave();
	afx_msg void OnButtonAddRow();
	afx_msg void OnButtonDeleteRow();
	afx_msg void OnButtonEqLR();
	afx_msg void OnButtonSymLR();
	afx_msg void OnButtonPlane();
	afx_msg void OnButtonVertical();
	afx_msg void OnButtonSlope();
	afx_msg void OnButtonNext();
	afx_msg void OnKillfocusEditLinename();
	afx_msg void OnKillfocusEditStationStt();
	afx_msg void OnKillfocusEditIpHeight();
	afx_msg void OnButtonAddLine();
	afx_msg void OnButtonDelLine();
	afx_msg void OnSelchangeComboLineselect();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellInsertedRow(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	void CheckInput();
//	CFTextOutDlg* m_pLineOutDlg;
	double m_dVScaleDraw;

	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void DrawLinePlane(CDomyun* pDom);
	void DrawVertGroundEL(CDomyun* pDom);
	void DrawVertPlanEL(CDomyun* pDom);
	void DrawSlope(CDomyun* pDom, BOOL bLeft);
	void DrawCursor(CDomyun* pDom, int nRow);
	void OnPreInitDialog(); 
	void SetDataSave();
	void SetDataSaveLinePlane();
	void SetDataSaveVertGroundEL();
	void SetDataSaveVertPlanEL();
	void SetDataSaveSlope();
	void SetGridTitle();
	void SetGridTitleLinePlane();
	void SetGridTitleVertGroundEL();
	void SetGridTitleVertPlanEL();
	void SetGridTitleSlope();
	void SetDataInit();
	void SetDataInitLinePlane();
	void SetDataInitVertGroundEL();
	void SetDataInitVertPlanEL();
	void SetDataInitSlope();
	void SetDataDefault();
	void SetEnableWindow();
	void InitTabCtrl();
	void SetLineInfoFile(CString strFilePath);
	void AddRowLinePlane();
	void AddRowVertGroundEL();
	void AddRowVertPlanEL();
	void AddRowSlope();
	void DeleteRowLinePlane();
	void DeleteRowVertGroundEL();
	void DeleteRowVertPlanEL();
	void DeleteRowSlope();
	void OnLineinfoAdd();
	void OutputViewOnAView(CStringArray &Arr, CString szTitle);

	CLineInfo* GetCurLineInfo();
	void SetLineName(long nCur);
	void ResetLine();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASELINEINFO_H__EA9747BB_708F_4A0F_A26C_E1A236A67D5D__INCLUDED_)
