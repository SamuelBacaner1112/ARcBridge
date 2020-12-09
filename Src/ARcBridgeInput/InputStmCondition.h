#pragma once

// CInputStmCondition ��ȭ �����Դϴ�.

class CInputStmCondition : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputStmCondition)

public:
	CInputStmCondition(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputStmCondition();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STM_CONDITION };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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

	void AddCellTension(long nStmType, long &row, long &col);		// ������ ����
	void AddCellStmCheckCorner(long &row, long &col, long nCase, CString strTitle, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bUpper);		// �찢��
	void AddCellStmCheckBracket(long &row, long &col, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bStt);						// �����
	void AddCellStmCheckFooting(long &row, long &col, CString strTitle, HGBOOL bStt);										// ���� - ������
	void AddCellStmCheckFootingMid(long &row, long &col);												// ���� - �߰�����

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
