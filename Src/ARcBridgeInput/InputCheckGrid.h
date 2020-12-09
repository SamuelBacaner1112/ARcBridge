// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTCHECKGRID_H__0283AA8A_43A4_4D39_9917_B676EDD738E2__INCLUDED_)
#define AFX_INPUTCHECKGRID_H__0283AA8A_43A4_4D39_9917_B676EDD738E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputCheckGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputCheckGrid dialog
class CARcBridgeCalcStd;
class CStmDesignStd;

class CInputCheckGrid : public CXTResizeDialog
{
// Construction
public:
	CInputCheckGrid(CWnd* pParent = NULL);   // standard constructor
	CInputCheckGrid(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck=0, CWnd* pParent = NULL);
	virtual ~CInputCheckGrid();
	CRcBridgeRebar *m_pBri;

	long m_nInputIDD;
	long m_nTypeCheck;

	CARcBridgeCalcStd *m_pCalc;

	CXTPTabCtrl	m_ctlTab;
	CGridCtrlMng	m_Grid; 
	BOOL	m_bOK;

	void Check();
	void ShowAndCheck(CWnd *pWndParent);
	void SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck);
	void SetStmModel(CStmModel *pStmModel) {m_pStmModel = pStmModel;}

// Dialog Data
	//{{AFX_DATA(CInputCheckGrid)
	enum { IDD = IDD_DIALOG_CHECK_GRID };
	CButton	m_btnOk;
	CButton	m_btnCancel;
	BOOL	m_bUseUserOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputCheckGrid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStmModel *m_pStmModel;

	// Generated message map functions
	//{{AFX_MSG(CInputCheckGrid)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckUserPlateOutput();
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	void CalculateBridge();
	void SetDataSave();
	void SetDataSave_3DOutput();
	void SetDataSave_3DOutputWingWall();
	double GetDesignValue(long nRow,long nCol);
	void MakeMatrixDesign(BOOL bFrame=TRUE, BOOL bEQ=TRUE, BOOL bShell=TRUE);
	void MakeMatrixDesign_Lsd(BOOL bFrame=TRUE, BOOL bShell=TRUE);
	void ShowDlg();
	void SetGridTitle();
	void SetDataInit();

	void SetGridTitle_Analysis();
	void SetGridTitle_Analysis_WingWall();

	void SetDataInit_Analysis();
	void SetDataInit_Analysis_Design();
	void SetDataInit_Analysis_Frame();
	void SetDataInit_Analysis_Shell();
	void SetDataInit_Analysis_WingWall();
	void SetDataInit_Analysis_Design_Lsd();
	void SetDataInit_Analysis_Frame_Lsd();
	void SetDataInit_Analysis_Shell_Lsd();

	void SetGridTitle_FootingSafty();
	void SetDataInit_FootingSafty();
	void SetDataInit_FootingSafty_Overtruning();
	void SetDataInit_FootingSafty_Sliding();
	void SetDataInit_FootingSafty_Support();
	void SetDataInit_FootingSafty_RigidBody();

	void SetGridTitle_FootingSafty_Lsd();
	void SetDataInit_FootingSafty_Overtruning_Lsd();
	void SetDataInit_FootingSafty_Sliding_Lsd();
	void SetDataInit_FootingSafty_Support_Lsd();
	void SetDataInit_FootingSafty_Subsidence_Lsd();

	void SetGridTitle_FootingPile();
	void SetDataInit_FootingPile();
	void SetDataInit_FootingPile_Support();
	void SetDataInit_FootingPile_Bending();
	void SetDataInit_FootingPile_Shear();
	void SetDataInit_FootingPile_Disp();
	void SetDataInit_FootingPile_Hyunta();

	void SetGridTitle_FootingPile_Lsd();
	void SetDataInit_FootingPile_Support_Lsd();
	void SetDataInit_FootingPile_AxialForce_Lsd();
	void SetDataInit_FootingPile_Fa_Lsd();
	void SetDataInit_FootingPile_ShaerStress_Lsd();
	
	void SetGridTitle_MainRebar();
	void SetDataInit_MainRebar();
	void SetDataInit_MainRebar_Stress();
	void SetDataInit_MainRebar_Crack();
	
	void SetGridTitle_MainRebar_Lsd();
	void SetDataInit_MainRebar_Stress_Lsd();
	void SetTextMatrix_MainRebar_Stress(CConcBasic *pConc, long nRowPos, const CString &strPos, const CString &strPos1, const CString &strEnv);
	void SetDataInit_MainRebar_Use_Lsd();
	void SetTextMatrix_MainRebar_Use(CConcBasic *pConc, HGINT32 ixRow, const CString &strPos, const CString &strPos1);
	
	
	void SetGridTitle_Corner();
	void SetDataInit_Corner();

	void SetGridTitle_Section();
	void SetDataInit_Section();
	void SetDataInit_Section_Shear();
	void SetDataInit_Section_Support();

	void SetGridTitle_Section_Lsd();
	void SetDataInit_Section_Shear_Lsd();
	void SetTextMatrix_Section_Shear(CConcBasic *pConc, HGINT32 ixRow, const CString &strPos, const CString &strPos1, const CString &strEnv);
	void SetDataInit_Section_Support_Lsd();

	void SetGridTitle_WingWall();
	void SetDataInit_WingWall();
	void SetDataInit_WingWall_Stress();
	void SetDataInit_WingWall_Crack();
	void SetDataInit_WingWall_Shear();
	void SetDataInit_WingWall_Support();

	void SetGridTitle_WingWall_Lsd();
	void SetDataInit_WingWall_Stress_Lsd();
	void SetDataInit_WingWall_Use_Lsd();
	void SetDataInit_WingWall_Support_Lsd();
	void SetDataInit_WingWall_Shear_Lsd();

	void SetGridTitle_Column();
	void SetDataInit_Column();

	void SetGridTitle_Column_Lsd();
	void SetDataInit_Column_Lsd();

	void SetDataInit_STM();
	void SetDataInit_STM_RebarTie();
	void SetDataInit_STM_CompressionStrut();
	void SetDataInit_STM_NodalZone();
	void SetDataInit_STM_MinRebar();

	// 한계상태
	void SetDataInit_STM_RebarTie_Lsd();
	void SetDataInit_STM_CompressionStrut_Lsd();
	void SetDataInit_STM_NodalZone_Lsd();

	void SetGridTitle_DesignVariable_ExposureRating();
	void SetDataInit_DesignVariable_ExposureRating();

	void SetGridTitle_RebarAps();
	void SetDataInit_RebarAps();
	void SetDataInit_RebarAps_Stress_Lsd();
	void SetDataInit_RebarAps_Use_Lsd();
	void SetDataInit_RebarAps_Support_Lsd();
	void SetDataInit_RebarAps_Shear_Lsd();
	
	void SetDataInit_Stress_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol);
	void SetDataInit_Use_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol);
	void SetDataInit_Shear_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol);
	void SetDataInit_Support_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol);

	HGBOOL GetFootInfo_Lsd(HGINT32 ixJijum, HGINT32 ixPos, CString &strPos, CString &strPos1, HGBOOL &bFront, HGBOOL &bUpper, HGINT32 &ixCheckAdd);
	HGINT32 GetCountFootPos(HGINT32 ixJijum);

	void InitTab();

	CString GetStrPileCase(HGINT32 ixCase) const;
	CDesignPile *GetCurDesignPile() const;
	//
	CMatrix m_MatShell;
	CMatrix m_MatFrame;
	CMatrix m_MatEQ;

	long m_nTab_Pile_Disp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCHECKGRID_H__0283AA8A_43A4_4D39_9917_B676EDD738E2__INCLUDED_)
