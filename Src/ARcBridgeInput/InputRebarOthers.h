// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBAROTHERS_H__2BF2CD42_A7DE_42D9_8C03_CB034E956968__INCLUDED_)
#define AFX_INPUTREBAROTHERS_H__2BF2CD42_A7DE_42D9_8C03_CB034E956968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarOthers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarOthers dialog

class CInputRebarOthers : public CInputDialogBase
{
// Construction
public:
	
	void SetDataSaveNonRebarConcrete();
	void SetDataInitNonRebarConcrete();
	void SetGridTitleNonRebarConcrete();
	CInputRebarOthers(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputRebarOthers)
	enum { IDD = IDD_REBAR_OTHERS };
	CButton			m_btnApply;
	CStatic				m_sUpperGugan;
	CStatic				m_sSideGugan;
	CStatic				m_sGWGugan;
	CXTPComboBox		m_cbBridgeNo;
	CXTPComboBox		m_cbGWGugan;
	CXTPComboBox		m_cbUpperGugan;
	CXTPComboBox		m_cbSideGugan;
	CXTPTabCtrl			m_Tab;
	CGridCtrlMng		m_Grid;
	BOOL				m_bPosition;
	int 				m_nGWGugan;
	int					m_nUpperGugan;
	int					m_nSideGugan;
	double				m_dFck;
	double				m_dFy;
	CString				m_sFck;
	CString				m_sFy;
	BOOL				m_bGWhalf;
	double	m_dAggregate;
	//}}AFX_DATA

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarOthers)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInputRebarOthers)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeComboGWGugan();
	afx_msg void OnSelchangeComboUpperGugan();
	afx_msg void OnSelchangeComboSideGugan();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCheckPosition();
	afx_msg void OnChangeEditFy();
	afx_msg void OnChangeEditFck();
	afx_msg void OnCheckGwhalf();
	afx_msg void OnKillfocusEditFck();
	afx_msg void OnKillfocusEditFy();
	afx_msg void OnKillfocusEditAgg();
	afx_msg void OnChangeEditAgg();
	//}}AFX_MSG
	virtual afx_msg void OnButtonApply();
	DECLARE_MESSAGE_MAP()
private:
	long m_nTabMonument;
	long m_nTabDetectGravel;
	long m_nTabHoleFootway;
	long m_nTabHole;
	long m_nTabRailGuardFence;
	long m_nTabFootwayBlock;
	long m_nTabHandRail;
	long m_nTabRetainingWall;
	long m_nTabGuardHalf;
	long m_nTabSoundProofType;
	long m_nTabSoundProof;
	long m_nTabCenterGuard;
	long m_nTabGuardFence;
	long m_nTabNonRebar;
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetFocusCell();
	CString GetNameOfTab();					// 현재 탭의 이름 가져오기
	CString GetNameOfTab(long nTab);		// n번째 탭의 이름 가져오기
//	long GetItemNo(CString strItem);		// 해당 탭 이름의 Index
	void SetGuganCombo(BOOL bShow);			// 방음벽 일때 3개 콤보 설정
	long GetGugan();						// 방음벽 구간
	void SetDataDefaultGuganCombo();		// 3개 콤보 초기화
	
	void SetDataDefault();
	void SetDataDefaultMonument();			// 교명주

	void SetGridTitle();
	void SetGridTitleGuardFenceAndCentralWall(CString strTab);		// 방호벽, 중분대, 중분대1/2, 난간기초
	void SetGridTitleCentralWall();		// 중분대 일체형
	void SetGridTitleSoundProof();		// 방음벽 기초
	void SetGridTitleRetainingWall();	// 흙막이벽
	void SetGridTitleRailGuardFence();	// 연석
	void SetGridTitleFootwayBlock();	// 연석+보도
	void SetGridTitleHole();			// 통신구
	void SetGridTitleHoleFootway();		// 통신구+보도
	void SetGridTitleMonument();		// 교명주

	void SetDataInit();
	void SetDataInitGuardFenceAndCentralWall(CString strTab);		// 방호벽, 중분대, 중분대1/2, 난간기초
	void SetDataInitCentralWall();		// 중분대 일체형
	void SetDataInitSoundProof(CString strTab);						// 방음벽 기초
	void SetDataInitRetainingWall();	// 흙막이벽
	void SetDataInitRailGuardFence();	// 연석
	void SetDataInitFootwayBlock();		// 연석+보도
	void SetDataInitHole();				// 통신구
	void SetDataInitHoleFootway();		// 통신구+보도
	void SetDataInitMonument();			// 교명주

	void SetDataSave();
	void SetDataSaveGuardFenceAndCentralWall(CString strTab);		// 방호벽, 중분대, 중분대1/2, 난간기초
	void SetDataSaveCentralWall();		// 중분대 일체형
	void SetDataSaveSoundProof(CString strTab);						// 방음벽 기초
	void SetDataSaveRetainingWall();	// 흙막이벽
	void SetDataSaveRailGuardFence();	// 연석
	void SetDataSaveFootwayBlock();		// 연석+보도
	void SetDataSaveHole();				// 통신구
	void SetDataSaveHoleFootway();		// 통신구+보도
	void SetDataSaveMonument();			// 교명주

	void   DrawInputDomyunView(BOOL bZoomAll = TRUE, BOOL bCover = FALSE);
	CDRect DrawGuardFence(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// 방호벽
	CDRect DrawCenterGuard(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// 중분대
	CDRect DrawCenterGuardHalf(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// 중분대1/2
	CDRect DrawSoundProof(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// 방음벽 기초
	CDRect DrawRetainingWall(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// 흙막이벽
	CDRect DrawHandRail(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);			// 난간기초
	CDRect DrawRailGuardFence(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// 연석
	CDRect DrawFootwayBlock(CDomyun* pDom, long nHDan, BOOL bInput = TRUE, BOOL bCover = FALSE);	// 연석+보도
	CDRect DrawTotalHole(CDomyun* pDom, long nHDan, BOOL bInput = TRUE, BOOL bCover = FALSE);		// 통신구, 통신구+보도
	void   DrawMonument(CDomyun* pDom);		// 교명주
	void   DrawCrossPosition(CDomyun* pDom, long nHDan, CDRect rtMain);			// 횡단위치도
	CDRect DrawCrossGuardWall(CDomyun *pDom, long nHDan);

	double  GetNumberInString(CString str);		// 문자열에서 숫자만 분리
	CString GetComment(CString str);			// 도움말
	void	SetControl();						// fck, fy 단위 설정
	CString GetStringDia(double dDia);			// fy에 따른 직경
	void	SetFckAndFy();						// fck, fy, Title 설정
	long	GetHDan();							// 방호벽의 구간값

	 void SetDlgFckFyAg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBAROTHERS_H__2BF2CD42_A7DE_42D9_8C03_CB034E956968__INCLUDED_)
