// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARJEWON_H__A7AC0069_3707_4391_95F4_CB4D1D4AB8D4__INCLUDED_)
#define AFX_INPUTREBARJEWON_H__A7AC0069_3707_4391_95F4_CB4D1D4AB8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarJewon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarJewon dialog

#define DEFAULT_REBARJEWON_MAIN		0x00000001
#define DEFAULT_REBARJEWON_ETC		0x00000002
#define DEFAULT_REBARJEWON_COLUMN	0x00000004
#define DEFAULT_REBARJEWON_WINGSL	0x00000008
#define DEFAULT_REBARJEWON_WINGSR	0x00000010
#define DEFAULT_REBARJEWON_WINGEL	0x00000020
#define DEFAULT_REBARJEWON_WINGER	0x00000040
#define DEFAULT_REBARJEWON_APS		0x00000080
#define DEFAULT_REBARJEWON_HYUNTA	0x00000100
#define DEFAULT_REBARJEWON_ETC2		0x00000200


class CInputRebarJewon : public CInputDialogBase
{
// Construction
public:
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar;	// 현재 출력된 rebar배열
	CRebarTableDlg m_RebarTableDlg;
	CInputRebarJewon(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputRebarJewon)
	enum { IDD = IDD_REBAR_JEWON };
	CButton	m_btnAddRate;
	CButton	m_btnRecommandAll;
	CXTPComboBox	m_comboBridgeNo;
	CXTPTabCtrl		m_Tab;
	CGridCtrlMng		m_Grid;
	CButton	m_checkViewlsd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarJewon)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarJewon)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRecommandAll();
	afx_msg void OnButtonAddrate();
	afx_msg void OnButtonAddRebar();
	afx_msg void OnButtonRemoveRebar();
	afx_msg void OnBnClickedCheckViewLsdinfo();
	afx_msg void OnButtonDetJoint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void DrawRebarDetail(CDomyun *pDom, CDRect rect, double dScale, long nPosDetail=2);
	CDRect DrawMonument(CDomyun *pDom);
	CDRect DrawGuardWall(CDomyun *pDom, BOOL bInput = TRUE);
	CDRect DrawHyunTa(CDomyun *pDomP);
	CDRect DrawWingWall(CDomyun *pDom, BOOL bStt, BOOL bLeft, int nOut);
	CDRect DrawRebarMain_VertSection(CDomyun *pDomP, long nCycle, CString sMark);
	CDRect DrawRebarSupport_VertSection(CDomyun *pDomP, CString sMark);
	CDRect DrawRebarGirder_VertSection(CDomyun *pDomP, long nIdxJijum, CString sMark);
	CDRect DrawRebarBracket_VertSection(CDomyun *pDomP, BOOL bStt, CString sMark);
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitleFirst();
	void SetGridData();
	void SetGridJewonInit(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nType, long nGW=0, BOOL bDisableEditJewon=FALSE, CString strType="", CDoubleArray* pArr=NULL, BOOL bEditMark=FALSE);
	void SetGridJewonData(CRebar *pRB, long nTab, long nGW=0, BOOL bInit=FALSE, BOOL bDisableEditJewon=FALSE, CString strTypeTmp="", CDoubleArray* pArr=NULL, BOOL bEditMark=FALSE);
	void SetDataInit();
	void SetControl();
	int GetTabCurOriginSel();
//	int GetCountRebarGuardWall();
	int GetHDanByRow(int nRow);
	CGuardWallRC* GetGuardWallByRow(int nRow);
	void SetDefaultRebarJewonETC2(long bri);
	void SetGridJewonDataETC();
	BOOL IsDuplicate(long n, long& nHDan, long nType);
	CString GetGuganTitle(long nGW, long nSum, long nType, CDoubleArray* pArr);
	double GetNumberInString(CString str);		// 문자열에서 숫자만 분리

	BOOL m_bViewLsd;
	CStringArray m_sArrDia;
	void SetStringDia();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARJEWON_H__A7AC0069_3707_4391_95F4_CB4D1D4AB8D4__INCLUDED_)
