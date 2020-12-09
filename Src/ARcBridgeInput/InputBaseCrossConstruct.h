// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASECROSSCONSTRUCT_H__D8AE3217_790F_49BF_A82F_83F670DC4968__INCLUDED_)
#define AFX_INPUTBASECROSSCONSTRUCT_H__D8AE3217_790F_49BF_A82F_83F670DC4968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseCrossConstruct.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossConstruct dialog
class CARcBridgeCalcStd;
class CInputBaseCrossConstruct : public CInputDialogBase
{
// Construction
public:
	BOOL CheckHDanZero();
	BOOL IsOKMoveOtherDlg();
	BOOL CheckVariableSection();
	CInputBaseCrossConstruct(CWnd* pParent = NULL);   // standard constructor
	~CInputBaseCrossConstruct();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseCrossConstruct)
	enum { IDD = IDD_BASE9_CROSS_CONSTRUCT };
	CXTPTabCtrl	m_ctlTab;
	CGridCtrlMng	m_Grid;
	double	m_dTS						;		// 슬래브 두께
	BOOL	m_bLevelFrRoadCenterToEdge	;		// --도로중심간격 레벨 적용//횡단 슬래브 제원입력 추가사항 20050610
	int		m_nQtyRail					;		// [슬래브 제원 입력], 레일 개소수, 철도교에서 사용
	double  m_dDisBaseElevation			;		// F.L 기준위치(도상높이 기준 위치) - 20060912부로 철도교가 아닐 때도 적용 가능 하도록 수정 modified by jaeho.
	double	m_dUnitWeightRail			;		// 궤도 단위중량
	double  m_dDisBaseElevationRL		;		// --R.L 기준위치(철도교 도상높이 기준 위치)//횡단 슬래브 제원입력 추가사항 20050610
	double  m_dHeightRail				;		// --철도교 레일높이//횡단 슬래브 제원입력 추가사항 20050610
	double  m_dHeightDosangUnderTie				;		// 철도교 도상높이
	double  m_dSlopeDosang				;		// 철도교 도상 기울기
	BOOL	m_bUnderGroundPaveDetail	;		// 세부 포장
	BOOL	m_bApplyLineSlope			;		// --횡경사 선형 편경사 적용//횡단 슬래브 제원입력 추가사항 20050610
	double	m_dWidthRoadLeft	;		// 좌측 노면 거리
	double	m_dWidthRoadRight	;		// 우측 노면 거리
	BOOL	m_bViewAllDir;	//상하행선 모두 표현
	CButton	m_btGuardWall;
	CButton	m_btSlabShape;
	CButton	m_btDelete;
	CButton	m_btAdd;
	CXTPComboBox	m_cbBridgeNo;
	CString	m_szUnderElevationEnd;
	CString	m_szUnderElevationStt;
	BOOL	m_bViewHatch;
	double	m_dDistRoadCenterLevelLeft;
	double	m_dDistRoadCenterLevelRight;
	HGBOOL m_bShowSectionEnd;
	HGBOOL m_bShowPlane;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseCrossConstruct)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseCrossConstruct)
	afx_msg void OnKillfocusEditTS();
	afx_msg void OnSetfocusEditTP();
	afx_msg void OnSetfocusEditTS();
	afx_msg void OnCheckLevelRoadCenter();
	afx_msg void OnKillfocusHdaninDisBaseElevation();
	afx_msg void OnCheckApplyLineSlope();
	afx_msg void OnButtonGuardWall();
	afx_msg void OnButtonGuardWallUnder();
	afx_msg void OnButtonSlabShape();
	afx_msg void OnCheckViewCounterDir();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonInsertCol();
	afx_msg void OnButtonDeleteCol();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckViewHatch();
	afx_msg void OnKillfocusEditRoadcenterLevelLeft();
	afx_msg void OnKillfocusEditRoadcenterLevelRight();
	afx_msg void OnBnClickedCheckViewEndCross();
	afx_msg void OnBnClickedCheckViewPlane();
	//}}AFX_MSG
	afx_msg void OnSelchangeTab(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnSelchangingTab(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	long m_nTabSlab;	//슬래브 횡단
	long m_nTabUnderGround;//지중라멘교
	long m_nTabRoad;	//도로횡단, 철도횡단
	long m_nTabNori;	//성토사면
	long m_nTabPave;	//포장제원
	long m_nTabRailData;//철도제원
	long m_nTabRail;	//레일위치
	long m_nTabDistribute;	// 분포폭
	long m_nColSelect;

	CARcBridgeCalcStd *m_pCalc;

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void DrawInputDomyunViewDimSlabRoad(CDomyun* pDom, BOOL bRoadNotSlab, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp);	//슬래브 횡단//도로횡단, 철도횡단
	void DrawInputDomyunViewDimNori(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp);	//성토사면
	void DrawInputDomyunViewDimPave(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1);	//포장제원
//	void DrawInputDomyunViewDimRail(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1);	//레일위치
	void DrawInputDomyunViewCrossSection(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput);	// 단면도
	void DrawInputDomyunViewCrossSectionStt(CDomyun* pDom);
	void DrawInputDomyunViewCrossSectionMid(CDomyun* pDom);
	void DrawInputDomyunViewCrossSectionEnd(CDomyun* pDom);
	void DrawInputDomyunViewPlane(CDomyun* pDom);			// 평면도
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetGridTitle();

	void MakeLinefoBridge(long nBridge);
	void InitTabCtrl();
	
	void SetDataDefault();

	void SetGridTitleSlab();	//슬래브 횡단
	void SetGridTitleUnderGround();	// 지중라멘교
	void SetGridTitleRoad();	//도로횡단, 철도횡단
	void SetGridTitleNori();	//성토사면
	void SetGridTitlePave();	//포장제원
	void SetGridTitleRailData();//철도제원
	void SetGridTitleRail();	//레일위치
	void SetGridTitleDistribute();	// 분포폭

	void SetDataInit();
	void SetDataInitSlab();		//슬래브 횡단
	void SetDataInitUnderGround();	// 지중라멘교
	void SetDataInitRoad();		//도로횡단, 철도횡단
	void SetDataInitNori();		//성토사면
	void SetDataInitPave();		//포장제원
	void SetDataInitRailData();	//철도제원
	void SetDataInitRail();		//레일위치
	void SetDataInitDistribute();	// 분포폭

	void SetDataSave();
	void SetDataSaveSlab();		//슬래브 횡단
	void SetDataSaveUnderGround();	// 지중라멘교
	void SetDataSaveRoad();		//도로횡단, 철도횡단
	void SetDataSaveNori();		//성토사면
	void SetDataSavePave();		//포장제원
	void SetDataSaveRailData();	//철도제원
	void SetDataSaveRail();		//레일위치
	void SetDataSaveDistribute();	// 분포폭

	void GetStation(HGINT32 nPos, HGDOUBLE &dSta, HGDOUBLE &dSta1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASECROSSCONSTRUCT_H__D8AE3217_790F_49BF_A82F_83F670DC4968__INCLUDED_)
