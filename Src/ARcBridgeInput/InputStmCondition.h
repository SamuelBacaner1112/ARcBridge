#pragma once

// CInputStmCondition 대화 상자입니다.

class CInputStmCondition : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputStmCondition)

public:
	CInputStmCondition(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputStmCondition();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STM_CONDITION };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	void KillFocusDlg();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataDefaultCondition(HGINT32 ix);
	void SetDataDefaultStmCheck(HGINT32 ix);
	void SetDataSave();
	void SetDataInit();
	void SetGridData();
	void SetGridDataCondition();
	void SetGridDataStmCheck();
	void SetGridDataLsdStmCheck();

	void AddCellTension(long nStmType, long &row, long &col);		// 구조물 강도
	void AddCellStmCheckCorner(long &row, long &col, long nCase, CString strTitle, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bUpper);		// 우각부
	void AddCellStmCheckBracket(long &row, long &col, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bStt);						// 브라켓
	void AddCellStmCheckFooting(long &row, long &col, CString strTitle, HGBOOL bStt);										// 기초 - 시종점
	void AddCellStmCheckFootingMid(long &row, long &col);												// 기초 - 중간지점

	CString GetStringValueBracket(CARcBridgeCalcStd *pCalc, HGBOOL bStt);
	CString GetStringValueCorner(HGBOOL bStt, HGBOOL bClose, HGBOOL bUpper, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	CString GetStringValueFoont(CARcBridgeCalcStd *pCalc, HGINT32 nJijum, HGINT32 iCase, HGINT32 &ixCombo, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);

	void InitComboCtrl();
	void SynMaterialUsd(BOOL bDefault);

	DECLARE_MESSAGE_MAP()

	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);

	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonSameAll();

protected:
	CLsdGridCtrl m_Grid;
	CTabCtrl m_ctlTab;
	CXTPComboBox	m_cbBridgeNo;

	HGINT32 m_nTab;
	HGINT32 m_nTabCondition;
	HGINT32 m_nTabStmCheck;

	HGDOUBLE m_dEtaBracket;
};
