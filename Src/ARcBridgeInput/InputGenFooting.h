// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENFOOTING_H__C9E44072_FAC9_4667_81B1_77D96EC96254__INCLUDED_)
#define AFX_INPUTGENFOOTING_H__C9E44072_FAC9_4667_81B1_77D96EC96254__INCLUDED_

#include "DialogEHPLink.h"	// Added by ClassView
#include "BaseDesignFactDlg.h"	// Added by ClassView
#include "InputGenHyunTa.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenFooting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenFooting dialog
#include "BaseFactDlg.h"	// Added by ClassView
#include "DialogDom.h"	// Added by ClassView
class CInputGenFooting : public CInputDialogBase
{
// Construction
public:
	void SetEpInnerFill(CExPileApp *pPile);
	CInputGenFooting(CWnd* pParent = NULL);   // standard constructor
	~CInputGenFooting();

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenFooting)
	enum { IDD = IDD_GEN_FOOTING };
	CButton	m_checkSameDist;
	CButton	m_btnSame;
	CButton	m_btnFinite;
	CButton	m_btnJewon;
	CButton m_btnFce;
	CButton	m_btnHyunta;

	CXTPComboBox	m_comboKindPile;
	CXTPComboBox	m_comboKindPile2;
	CXTPComboBox	m_comboVertSu;
	CXTPComboBox	m_comboFooting;
	CXTPComboBox	m_comboBridgeNo;
	CButton	m_btnExamine;
	CGridCtrlMng	m_Grid;
	CGridCtrlMng	m_GridPos;
	CXTPTabCtrl	m_TabPos;
	CXTPTabCtrl	m_Tab;
	BOOL	m_bApplyPileConstruct;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenFooting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenFooting)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeComboFooting();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboVertSu();
	afx_msg void OnButtonExamine();
	afx_msg void OnSelchangeComboKindPile();
	afx_msg void OnSelchangeComboKindPile2();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFinite();
	afx_msg void OnButtonJewon();
	afx_msg void OnButtonSame();
	afx_msg void OnCheckSamedist();
	afx_msg void OnCheckPileKind();
	afx_msg void OnCheckConcStress();
	afx_msg void OnCheckDist();
	afx_msg void OnCellButtonClick(NMHDR* pNotifyStruct, LRESULT* result);
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedPos(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedDataPos(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

	CDPoint m_xyButton;
	CDPoint m_xyButtonFactor;
private:
	CBaseDesignFactDlg m_dlgBaseShapeFact;	// 기초형상계수
	CBaseFactDlg	   m_dlgBaseFact;		// 기초근입계수

	void SetSizeTabAndGrid();
	void SetGridDataBaseShapeFactor();
	void SetGridDataFootingGround();
	void SetGridDataModeling();
	void SetGridDataBaseFactor();
//	void MoveControl();
	void SetGridDataSelect();
	void SetGridDataPileProperty();
	void InitTabCtrl();
	void SetGridDataCross(long nIdxOrg);
	void SetGridDataVert();
	void SetGridDataPHC();
	void SetGridDataPileJewon();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void DrawPlanView(CDomyun *pDomP);
	void DrawSectionView(CDomyun *pDomP);
	void DrawSectionBoxTypeView(CDomyun *pDomP);
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabPosCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	void AddCellFootingGround(HGINT32 &row, HGINT32 &col);		// 기초지반
	void AddCellCalcSupportForce(HGINT32 &row, HGINT32 &col);	// 지지력 산정

	CStringArray m_sArrTabPosName;
	long m_nTabPos;
	long m_nTabPosVert;
	CDoubleArray m_dArrTabPosCross;

	CStringArray m_sArrTabName;
	long m_nTab;
	long m_nTabPileJewon;
	long m_nTabPileProperty;
	long m_nTabPHC;
	long m_nTabFootingGround;
	long m_nTabSelect;
	long m_nTabModeling;
	long m_nTabFact;		// 기초근입계수 탭
	long m_nTabShapeFact;	// 기초형상계수 탭
	
	CDoubleArray m_dArrID;		// 이동시킬 컨트롤 아이디 모음
	BOOL m_bOutPileControl;		// 현재 파일전용 컨트롤들이 다이얼로그 밖에 나가 있는지?

	BOOL m_bMove;
	BOOL m_bCheckDist;
	BOOL m_bSPTDefault;

	CFootingApp* GetFooting(long nIdxFooting=-1);
	CExPileApp* GetPile(long nIdxFooting=-1);
	CWallApp* GetInWall(long nIdxFooting=-1);

public:
	CDialogEHPLink m_dlgEHPLink;
	void SetFocusItem(CString szTitle);
	BOOL IsFocusItem(CString &szTitle);
	void SetNValueByBoring(CFootingApp *pFooting);
	CButton m_checkDist;
	void OnButtonFce();
	afx_msg void OnBnClickedButtonHyunta();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENFOOTING_H__C9E44072_FAC9_4667_81B1_77D96EC96254__INCLUDED_)
