// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENDESIGNVARIABLE_H__104FBCDB_A040_47B8_83E7_948DA9F1CEEB__INCLUDED_)
#define AFX_INPUTGENDESIGNVARIABLE_H__104FBCDB_A040_47B8_83E7_948DA9F1CEEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenDesignVariable.h : header file
//

#include "DialogCrackEnv.h"
#include "PostBaseLineDlg.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CInputGenDesignVariable dialog

class CInputCheckGrid;
class CInputGenDesignVariable : public CInputDialogBase
{
// Construction
public:
	void KillFocusDlg();
	void SyncCover(CRcBridgeRebar *pBri);
	CInputGenDesignVariable(CWnd* pParent = NULL);   // standard constructor
	~CInputGenDesignVariable();

// Dialog Data
	//{{AFX_DATA(CInputGenDesignVariable)
	enum { IDD = IDD_GEN_DESIGN_VARIABLE };
	CButton	m_btnSave;
	CButton	m_btnLoad;
	CButton	m_btnSameAll;
	CButton	m_btnMirror;
	CButton	m_checkQuickSearch;
	CXTPComboBox	m_cbBridgeNo;
	CLsdGridCtrl	m_Grid;
	
	CXTPTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenDesignVariable)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInputGenDesignVariable)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCheckQuickSearch();
	afx_msg void OnButtonApplyMirror();
	afx_msg void OnButtonApplySameAll();
	afx_msg void OnButtonBasedataSave();
	afx_msg void OnButtonBasedataLoad();
	afx_msg void OnButtonExamine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void DrawInputDomyunView(BOOL bZoomAll=FALSE);

	void InitComboCtrl();
	void InitTabCtrl();
	void OnPreInitDialog();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

	void SetGridData();
	void SetGridDataCover();
	void SetGridDataEnv();
	void SetGridDataCondition();
	
	//
	void MakeSameCover(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameEnv(CRcBridgeRebar* pBri1, CRcBridgeRebar* pBri2);
	void MakeSameCondition(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	
	void MakeSameLsdSupportRebar(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	//

	CRcBridgeRebar* GetRcBridgeRebarForCover();
	
	long m_nTabCover;
	long m_nTabCondition;

	long m_nTabEnv;
	long m_nTabExposureRating;		// 노출등급
	long m_nTabDesignRating;		// 설계등급
	long m_nTabRebarFactor;			// 철근정착, 이음보정계수
	long m_nTabRebarSettle;			// 철근 정착장
	long m_nTabRebarJoint;			// 철근 이음장
	long m_nTabLsdSupportRebar;		// 수평철근량 검토

	long m_nMergeRowSupportBarSlab;
	long m_nMergeRowSupportBarWall;
	long m_nMergeRowSupportBarFoot;
	HGBOOL m_bMaterialDetail;		// 재료 물성치 상세입력

	CStringArray m_sArrTabName;
	CDialogCrackEnv m_DlgCrackEnv;
	CPostBaseLineDlg m_PostBaseLineDlg;
	CRcBridgeRebar *m_pBriForCover;
	
	CEarthquakeDlg		m_EarhtquakeDlg;
	CEarthquakeGradeDlg m_EarthquakeGradeDlg;
	CEarthquakeGradeKDS17Dlg m_EarthquakeGradeKDS17Dlg;
	CEarthquakeFactDlg	m_EarthquakeFactDlg;
	CEarthquakeFactKDS17Dlg m_EarthquakeFactKDS17Dlg;

	CInputCheckGrid *m_pDlgCheck;

	// (LSD:0001)
	void SetGridDataExposureRating();
	void SetGridDataDesignRating();
	void SetGridDataRebarFactor();
	void SetGridDataRebarSettle();
	void SetGridDataRebarJoint();
	void SeteGridDataLsdSupportRebar();

	void SyncMaterial();
	void AddCellByBridgePart(HGINT32 &iRow, HGINT32 &iCol, const CString &strCategory, HGDOUBLE dArr[]);
	void AddCellFckConc(HGINT32 &iRow, HGINT32 &iCol);
	void AddCellValidDepthBaseGround(HGINT32 &iRow, HGINT32 &iCol);
	void AddCellWallSpringEQ(HGINT32 &iRow, HGINT32 &iCol);
	void AddCellModelingEQ(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine);
	void AddCellFootForce(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENDESIGNVARIABLE_H__104FBCDB_A040_47B8_83E7_948DA9F1CEEB__INCLUDED_)
