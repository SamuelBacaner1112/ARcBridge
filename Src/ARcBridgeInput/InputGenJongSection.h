// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENJONGSECTION_H__FBD3A0BD_3C53_490B_B049_C8F3373C9954__INCLUDED_)
#define AFX_INPUTGENJONGSECTION_H__FBD3A0BD_3C53_490B_B049_C8F3373C9954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenJongSection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenJongSection dialog
#define DEFAULT_GENJONGSECTION_STT		0x00000001
#define DEFAULT_GENJONGSECTION_END		0x00000002
#define DEFAULT_GENJONGSECTION_INWALL	0x00000004
#define DEFAULT_GENJONGSECTION_ARCHRIB		0x00000008
#define DEFAULT_GENJONGSECTION_PRF			0x00000010
#define DEFAULT_GENJONGSECTION_LOWERSlAB		0x00000020

class CARcBridgeOutBMStd;
class AFX_EXT_CLASS CInputGenJongSection : public CInputDialogBase
{
// Construction
public:
	void SetGridDataRib();
	double m_dAngleByHorOfInWall[100];	// 경사교 중간 벽체의 각도 입력을 받을때 편의 상 수평에 대한 각도록 입력 받을 수 있도록 하기 위해 임시로 만든 변수
	
	// 사방향으로도 입력 받을 수 있게 함.(경사교에 대해서만 예외적임) ////
	double m_dAngleByUpperSlabOfInWall_Angle[100];	// 경사교 중간 벽체의 각도 입력을 받을때 편의 상 수평에 대한 각도록 입력 받을 수 있도록 하기 위해 임시로 만든 변수
	double m_dAngleByHorOfInWall_Angle[100];	// 경사교 중간 벽체의 각도 입력을 받을때 편의 상 수평에 대한 각도록 입력 받을 수 있도록 하기 위해 임시로 만든 변수
	////////////////////////////////////////////////////////////////////////

	void SyncWall(CWallApp *pWall, long nJijum, BOOL bApplyAngle);
	void SyncBridge(long nLeft, BOOL bApplyAngle, BOOL bApplyAngleAps);
	void KillFocusDlg();
	void ApplySameShapeSttAndEnd();
	long GetIdxCurInWall();
	long GetIdxCurOutWall();
	CInputGenJongSection(CWnd* pParent = NULL);   // standard constructor
	~CInputGenJongSection();

	long OnHitTest();
// Dialog Data
	//{{AFX_DATA(CInputGenJongSection)
	enum { IDD = IDD_GEN_JONG_SECTION };
	CButton	m_checkViewCross;
	CButton	m_btnSameInWall;
	CButton	m_btnSameSttEnd;
	CButton	m_checkViewEarth;
	CXTPComboBox	m_comboBridgeNo;
	CButton	m_btnExamine;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	BOOL	m_bCheckStation;
	CString	m_szHyungHaEL;
	CString	m_szHyungHaStation;

	CComboBox m_comboBoring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenJongSection)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenJongSection)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonExamine();
	afx_msg void OnCheckViewEarth();
	afx_msg void OnButtonSamesttend();
	afx_msg void OnButtonSameinwall();
	afx_msg void OnCheckViewCross();
	afx_msg void OnCheckStation();
	afx_msg void OnKillfocusEditHyunghaStation();
	afx_msg void OnSelchangeComboBoring();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);	
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
private:
	void SetTextHyungHaElevation();
	BOOL m_bCheckViewEarth;
	BOOL m_bCheckViewCross;
	void ApplySameShapeInWall(long nIdxInWallStd);
	long m_nLeft;
	void SetGridDataInWall(long nIdxWall);
	void SetGridDataEndWall();
	void SetGridDataSttWall();
	void SetGridDataPRF();	
	void SetGridDataLowerSlab();
	void SetGridDataInWallBox();
	void SetGridDataOutWallBox();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTabWallStt;
	long m_nTabWallEnd;
	long m_nTabInWall[MAX_JIJUM_COUNT_RC];
	long m_nTabRib;
	long m_nTabPRF;
	long m_nTabOutWall;
	long m_nTabInWallBox;
	long m_nTabLowerSlab;
	long m_nTab;
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	void SetDataInitBoring(HGBOOL bUpdateUI=FALSE);
	CFootingApp *GetCurrentFooting();
	
	CStringArray m_sArrTabName;

	BOOL m_bMove;
	CTwinVectorArray m_tvArrBridge;	// 여기서 체크할 브리지 정보 저장
	CTwinVectorArray m_tvArrFootingStt;
	CTwinVectorArray m_tvArrFootingEnd;
	CTwinVectorArray m_tvArrExFootingStt;
	CTwinVectorArray m_tvArrExFootingEnd;

	CSafetyTypedPtrArray <CObArray, CTwinVectorArray*> m_ptvArrInWall; // 여기서 체크할 내부 벽체정보 저장
	CSafetyTypedPtrArray <CObArray, CTwinVectorArray*> m_ptvArrFooting; // 여기서 체크할 기초정보 저장
	CSafetyTypedPtrArray <CObArray, CTwinVectorArray*> m_ptvArrExFooting; // 여기서 체크할 기초정보 저장

	CDlgProgress* m_pProgress;
public:
	double m_dBoringEL;
	afx_msg void OnEnKillfocusEditEl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENJONGSECTION_H__FBD3A0BD_3C53_490B_B049_C8F3373C9954__INCLUDED_)
