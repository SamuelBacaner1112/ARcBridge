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
	double	m_dTS						;		// ������ �β�
	BOOL	m_bLevelFrRoadCenterToEdge	;		// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	int		m_nQtyRail					;		// [������ ���� �Է�], ���� ���Ҽ�, ö�������� ���
	double  m_dDisBaseElevation			;		// F.L ������ġ(������� ���� ��ġ) - 20060912�η� ö������ �ƴ� ���� ���� ���� �ϵ��� ���� modified by jaeho.
	double	m_dUnitWeightRail			;		// �˵� �����߷�
	double  m_dDisBaseElevationRL		;		// --R.L ������ġ(ö���� ������� ���� ��ġ)//Ⱦ�� ������ �����Է� �߰����� 20050610
	double  m_dHeightRail				;		// --ö���� ���ϳ���//Ⱦ�� ������ �����Է� �߰����� 20050610
	double  m_dHeightDosangUnderTie				;		// ö���� �������
	double  m_dSlopeDosang				;		// ö���� ���� ����
	BOOL	m_bUnderGroundPaveDetail	;		// ���� ����
	BOOL	m_bApplyLineSlope			;		// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	double	m_dWidthRoadLeft	;		// ���� ��� �Ÿ�
	double	m_dWidthRoadRight	;		// ���� ��� �Ÿ�
	BOOL	m_bViewAllDir;	//�����༱ ��� ǥ��
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
	long m_nTabSlab;	//������ Ⱦ��
	long m_nTabUnderGround;//���߶�౳
	long m_nTabRoad;	//����Ⱦ��, ö��Ⱦ��
	long m_nTabNori;	//������
	long m_nTabPave;	//��������
	long m_nTabRailData;//ö������
	long m_nTabRail;	//������ġ
	long m_nTabDistribute;	// ������
	long m_nColSelect;

	CARcBridgeCalcStd *m_pCalc;

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void DrawInputDomyunViewDimSlabRoad(CDomyun* pDom, BOOL bRoadNotSlab, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp);	//������ Ⱦ��//����Ⱦ��, ö��Ⱦ��
	void DrawInputDomyunViewDimNori(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp);	//������
	void DrawInputDomyunViewDimPave(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1);	//��������
//	void DrawInputDomyunViewDimRail(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1);	//������ġ
	void DrawInputDomyunViewCrossSection(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput);	// �ܸ鵵
	void DrawInputDomyunViewCrossSectionStt(CDomyun* pDom);
	void DrawInputDomyunViewCrossSectionMid(CDomyun* pDom);
	void DrawInputDomyunViewCrossSectionEnd(CDomyun* pDom);
	void DrawInputDomyunViewPlane(CDomyun* pDom);			// ��鵵
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetGridTitle();

	void MakeLinefoBridge(long nBridge);
	void InitTabCtrl();
	
	void SetDataDefault();

	void SetGridTitleSlab();	//������ Ⱦ��
	void SetGridTitleUnderGround();	// ���߶�౳
	void SetGridTitleRoad();	//����Ⱦ��, ö��Ⱦ��
	void SetGridTitleNori();	//������
	void SetGridTitlePave();	//��������
	void SetGridTitleRailData();//ö������
	void SetGridTitleRail();	//������ġ
	void SetGridTitleDistribute();	// ������

	void SetDataInit();
	void SetDataInitSlab();		//������ Ⱦ��
	void SetDataInitUnderGround();	// ���߶�౳
	void SetDataInitRoad();		//����Ⱦ��, ö��Ⱦ��
	void SetDataInitNori();		//������
	void SetDataInitPave();		//��������
	void SetDataInitRailData();	//ö������
	void SetDataInitRail();		//������ġ
	void SetDataInitDistribute();	// ������

	void SetDataSave();
	void SetDataSaveSlab();		//������ Ⱦ��
	void SetDataSaveUnderGround();	// ���߶�౳
	void SetDataSaveRoad();		//����Ⱦ��, ö��Ⱦ��
	void SetDataSaveNori();		//������
	void SetDataSavePave();		//��������
	void SetDataSaveRailData();	//ö������
	void SetDataSaveRail();		//������ġ
	void SetDataSaveDistribute();	// ������

	void GetStation(HGINT32 nPos, HGDOUBLE &dSta, HGDOUBLE &dSta1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASECROSSCONSTRUCT_H__D8AE3217_790F_49BF_A82F_83F670DC4968__INCLUDED_)
