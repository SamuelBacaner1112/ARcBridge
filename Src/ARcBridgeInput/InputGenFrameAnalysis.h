// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENFRAMEANALYSIS_H__8DD3179B_0F4C_40CF_ACF1_5B34D64C629E__INCLUDED_)
#define AFX_INPUTGENFRAMEANALYSIS_H__8DD3179B_0F4C_40CF_ACF1_5B34D64C629E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenFrameAnalysis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenFrameAnalysis dialog

class CSapJoint;
class CSapElement;
class CUnitLoadExt;

class CInputGenFrameAnalysis : public CInputGenAnalysisBase
{
// Construction 
public:
	void InitComboCtrlToolName();
	BOOL IsValid();
	CInputGenFrameAnalysis(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputGenFrameAnalysis)
	enum { IDD = IDD_GEN_FRAME_ANLYSIS };
	CEdit	m_ctrlEditUserDispMax;
	CButton	m_ctrlCheckUserDispMax;
	CButton	m_btnViewSetting;
	CXTPComboBox	m_cmbToolName;
	CStatic	m_ctlToolName;
	CSliderCtrl	m_ctlDispSlider;
	CSliderCtrl	m_ctlSlider;
	CTabCtrl	m_ctlTab;
	BOOL	m_bCheckAllBridge;
	BOOL	m_bCheckAutoClose;
	int		m_nRadioForce;
	BOOL	m_bKorCode;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenFrameAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenFrameAnalysis)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnRadioV22();
	afx_msg void OnRadioTortion();
	afx_msg void OnRadioAxial();
	afx_msg void OnRadioDisp();
	afx_msg void OnRadioM22();
	afx_msg void OnRadioM33();
	afx_msg void OnRadioReaction();
	afx_msg void OnButtonRunSap2000();
	afx_msg void OnButtonLoadSap();
	afx_msg void OnButtonMakeSap();
	afx_msg void OnCheckElement(); 
	afx_msg void OnCheckJoint();
	afx_msg void OnReleasedcaptureSliderScale(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioV33();
	afx_msg void OnButtonViewCalc();
	afx_msg void OnSelchangeTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderScale2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelEndOkBackClr();
	afx_msg void OnSelchangeComboToolname();
	afx_msg void OnCheckUserDispmax();
	afx_msg void OnChangeEditDispmax();
	//}}AFX_MSG
	afx_msg LRESULT OnCalcOk(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	void InitTabControl();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetLoadComboCombo();
	void SetLoadCaseCombo();	

	void SetDataInit();
	void SetControlUserDisp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENFRAMEANALYSIS_H__8DD3179B_0F4C_40CF_ACF1_5B34D64C629E__INCLUDED_)
