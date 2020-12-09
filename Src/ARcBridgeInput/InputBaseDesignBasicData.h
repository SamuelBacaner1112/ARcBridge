// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASEDESIGNBASICDATA_H__5360C2D4_6BBB_42B1_9D45_0A65B4279E16__INCLUDED_)
#define AFX_INPUTBASEDESIGNBASICDATA_H__5360C2D4_6BBB_42B1_9D45_0A65B4279E16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseDesignBasicData.h : header file
//

#include "InputDialogBase.h"

#include "EarthquakeDlg.h"
#include "BridgeLevelDlg.h"
#include "BridgeLevelDlgLSD.h"
#include "EarthquakeGradeDlg.h"
#include "EarthquakeGradeKDS17Dlg.h"
#include "EarthquakeFactDlg.h"
#include "EarthquakeFactKDS17Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// InputBaseDesignBasicData dialog

#define DEFAULT_BASEDESIGNBASEDATA_FACTOR		0x00000001
#define DEFAULT_BASEDESIGNBASEDATA_SETTLE		0x00000002
#define DEFAULT_BASEDESIGNBASEDATA_JOINT		0x00000004
#define DEFAULT_BASEDESIGNBASEDATA_WEIGHT		0x00000008
#define DEFAULT_BASEDESIGNBASEDATA_UWEIGHT		0x00000010
#define DEFAULT_BASEDESIGNBASEDATA_REBARFACTOR	0x00000020
#define DEFAULT_BASEDESIGNBASEDATA_RAILLOAD		0x00000040

class CLsdGridCtrl;

class CInputBaseDesignBasicData : public CInputDialogBase
{
// Construction
public:
	void SetDataInitRailLoadUser();
	void SetDataSaveRailLoadUser();
	BOOL m_bBecomeSmallGrid;
	void ShowControlRailLoad();
	CInputBaseDesignBasicData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseDesignBasicData)
	enum { IDD = IDD_BASE2_DESIGN_BASICDATA };
	CButton	m_btnSave;
	CButton	m_btnLoad;
	CEdit	m_editRailDistWheelMid;
	CEdit	m_editRailDistWheel;
	CEdit	m_editRailLoadWheel;
	CEdit	m_editDistributionLoadBack;
	CEdit	m_editDistributionLoadFront;
	CStatic	m_ctlStatic_Tank5;
	CStatic	m_ctlStatic_Tank4;
	CStatic	m_ctlStatic_Trailer6;
	CStatic	m_ctlStatic_Trailer7;
	CEdit	m_ctlEdit_Tank_WidSide;
	CEdit	m_ctlEdit_Tank_WidInner;
	CStatic	m_ctlStatic_Trailer5;
	CStatic	m_ctlStatic_Trailer4;
	CStatic	m_ctlStatic_Trailer3;
	CStatic	m_ctlStatic_Trailer2;
	CStatic	m_ctlStatic_Trailer1;
	CStatic	m_ctlStatic_Tank3;
	CEdit	m_ctlEdit_Trailer_WidTot;
	CEdit	m_ctlEdit_Trailer_WidSide;
	CEdit	m_ctlEdit_Trailer_WidInner;
	CEdit	m_ctlEdit_Trailer_P3;
	CEdit	m_ctlEdit_Trailer_P2;
	CEdit	m_ctlEdit_Trailer_P1;
	CEdit	m_ctlEdit_Trailer_L4;
	CEdit	m_ctlEdit_Trailer_L3;
	CEdit	m_ctlEdit_Trailer_L2;
	CEdit	m_ctlEdit_Trailer_L1;
	CEdit	m_ctlEdit_Tank_WidTot;
	CEdit	m_ctlEdit_Tank_Weight;
	CEdit	m_ctlEdit_Tank_Len;
	CStatic	m_ctlBM_Trailer;
	CStatic	m_ctlBM_Tank;
	CButton	m_ctlGB_Trailer;
	CButton	m_ctlGB_Tank;
	CStatic	m_ctlStatic_Tank2;
	CStatic	m_ctlStatic_Tank1;
	CButton	m_btnRemoveLoad;
	CButton	m_btnAddLoad;
	CXTPComboBox	m_ComboRailCar;
	CLsdGridCtrl	m_Grid;
	CXTPTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseDesignBasicData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void SetGridData();

// Implementation
protected:
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);

	// Generated message map functions
	//{{AFX_MSG(CInputBaseDesignBasicData)
	afx_msg void OnSelchangeComboRailCar();
	afx_msg void OnButtonAddLoad();
	afx_msg void OnButtonRemoveLoad();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void DrawRailLoadInput(CDomyun *pDomP);
	
	void SetGridDataRebarFactor();
	void SetGridDataRebarFactorUsd();
	void SetGridDataUWeight();
	void SetGridDataWeight();
	void SetGridDataJoint();
	void SetGridDataSettle();
	void SetGridDataFactor();
	void SetGridDataRailLoad();	

	// userdialog
	CEarthquakeDlg		m_EarhtquakeDlg;
	CBridgeLevelDlg		m_BridgeLevelDlg;
	CBridgeLevelDlgLSD	m_BridgeLevelDlgLsd;
	CEarthquakeGradeDlg m_EarthquakeGradeDlg;
	CEarthquakeGradeKDS17Dlg m_EarthquakeGradeKDS17Dlg;
	CEarthquakeFactDlg	m_EarthquakeFactDlg;
	CEarthquakeFactKDS17Dlg m_EarthquakeFactKDS17Dlg;

	// #ARCBRIDGE-3529 재료저항계수 및 기초부저항계수 문헌표기시 필요
	CLsdManager* m_pLsdManager;

	double m_dRebarSpace;
	double m_dCover;
	long m_nTab;
	BOOL bTabTankLoad;
	CStringArray m_sArrTabName;
	// 각 탭의 인덱스
	long m_nTabFactor;
	long m_nTabJoint;
	long m_nTabSettle;
	long m_nTabWeight;
	long m_nTabUWeight;
	long m_nTabRebarFactor;
	long m_nTabRailLoad;
	long m_nTabTankLoad;
	long m_nTabResistFactor;
	long m_nTabFootResistFactor;

	void SetDataSave();
	void SetDataInit();
	void SetDataDefault(); 
	void SetDataDefaultFactor(); 
	void SetDataDefaultFactor_Deduct(); 
	void SetDataDefaultJoint(); 
	void SetDataDefaultSettle(); 
	void SetDataDefaultWeight(); 
	void SetDataDefaultUWeight(); 
	void SetDataDefaultRebarFactor();
	void SetDataDefaultRailLoad(); 
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitTabCtrl();

	void SetDataSaveTankLoad();
	void SetEditDataTankLoad();
	void SetDataDefaultTankLoad();
	void ShowControlTankLoad(BOOL bTank);

	// (LSD:0003)
	BOOL IsCheckNotLSDType();
	BOOL OnChangeMethod();

	void SetDataDefaultMatResistFactor();
	void SetDataDefaultBaseResistFactor();

	void SetGridDataResistFactor();
	void SetGridDataFootResistFactor();	
	void SetGridDataRebarFactorLsd();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASEDESIGNBASICDATA_H__5360C2D4_6BBB_42B1_9D45_0A65B4279E16__INCLUDED_)
