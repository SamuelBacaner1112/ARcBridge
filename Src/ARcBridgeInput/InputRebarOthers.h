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
	CString GetNameOfTab();					// ���� ���� �̸� ��������
	CString GetNameOfTab(long nTab);		// n��° ���� �̸� ��������
//	long GetItemNo(CString strItem);		// �ش� �� �̸��� Index
	void SetGuganCombo(BOOL bShow);			// ������ �϶� 3�� �޺� ����
	long GetGugan();						// ������ ����
	void SetDataDefaultGuganCombo();		// 3�� �޺� �ʱ�ȭ
	
	void SetDataDefault();
	void SetDataDefaultMonument();			// ������

	void SetGridTitle();
	void SetGridTitleGuardFenceAndCentralWall(CString strTab);		// ��ȣ��, �ߺд�, �ߺд�1/2, ��������
	void SetGridTitleCentralWall();		// �ߺд� ��ü��
	void SetGridTitleSoundProof();		// ������ ����
	void SetGridTitleRetainingWall();	// �븷�̺�
	void SetGridTitleRailGuardFence();	// ����
	void SetGridTitleFootwayBlock();	// ����+����
	void SetGridTitleHole();			// ��ű�
	void SetGridTitleHoleFootway();		// ��ű�+����
	void SetGridTitleMonument();		// ������

	void SetDataInit();
	void SetDataInitGuardFenceAndCentralWall(CString strTab);		// ��ȣ��, �ߺд�, �ߺд�1/2, ��������
	void SetDataInitCentralWall();		// �ߺд� ��ü��
	void SetDataInitSoundProof(CString strTab);						// ������ ����
	void SetDataInitRetainingWall();	// �븷�̺�
	void SetDataInitRailGuardFence();	// ����
	void SetDataInitFootwayBlock();		// ����+����
	void SetDataInitHole();				// ��ű�
	void SetDataInitHoleFootway();		// ��ű�+����
	void SetDataInitMonument();			// ������

	void SetDataSave();
	void SetDataSaveGuardFenceAndCentralWall(CString strTab);		// ��ȣ��, �ߺд�, �ߺд�1/2, ��������
	void SetDataSaveCentralWall();		// �ߺд� ��ü��
	void SetDataSaveSoundProof(CString strTab);						// ������ ����
	void SetDataSaveRetainingWall();	// �븷�̺�
	void SetDataSaveRailGuardFence();	// ����
	void SetDataSaveFootwayBlock();		// ����+����
	void SetDataSaveHole();				// ��ű�
	void SetDataSaveHoleFootway();		// ��ű�+����
	void SetDataSaveMonument();			// ������

	void   DrawInputDomyunView(BOOL bZoomAll = TRUE, BOOL bCover = FALSE);
	CDRect DrawGuardFence(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// ��ȣ��
	CDRect DrawCenterGuard(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// �ߺд�
	CDRect DrawCenterGuardHalf(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// �ߺд�1/2
	CDRect DrawSoundProof(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);		// ������ ����
	CDRect DrawRetainingWall(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// �븷�̺�
	CDRect DrawHandRail(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);			// ��������
	CDRect DrawRailGuardFence(CDomyun* pDom, long nHDan, BOOL bInput = TRUE);	// ����
	CDRect DrawFootwayBlock(CDomyun* pDom, long nHDan, BOOL bInput = TRUE, BOOL bCover = FALSE);	// ����+����
	CDRect DrawTotalHole(CDomyun* pDom, long nHDan, BOOL bInput = TRUE, BOOL bCover = FALSE);		// ��ű�, ��ű�+����
	void   DrawMonument(CDomyun* pDom);		// ������
	void   DrawCrossPosition(CDomyun* pDom, long nHDan, CDRect rtMain);			// Ⱦ����ġ��
	CDRect DrawCrossGuardWall(CDomyun *pDom, long nHDan);

	double  GetNumberInString(CString str);		// ���ڿ����� ���ڸ� �и�
	CString GetComment(CString str);			// ����
	void	SetControl();						// fck, fy ���� ����
	CString GetStringDia(double dDia);			// fy�� ���� ����
	void	SetFckAndFy();						// fck, fy, Title ����
	long	GetHDan();							// ��ȣ���� ������

	 void SetDlgFckFyAg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBAROTHERS_H__2BF2CD42_A7DE_42D9_8C03_CB034E956968__INCLUDED_)
