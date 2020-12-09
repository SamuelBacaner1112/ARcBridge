#pragma once


// CDialogBMOption 대화 상자입니다.

class CRcBridgeApp;
class CDialogBMOption : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CDialogBMOption)

public:
	CString GetStringArray(CStringArray &sArr, long nIdx);
	long m_nCurPier;
	long m_nCurAbut;
	void InitComboCtrl();
	void SetOutBMStd(CARcBridgeOutBMStd *pStd);
	CARcBridgeOutBMStd* m_pStd;
	long m_nBri;
	void SetGridDataOut();		// 출력 옵션
	void SetGridDataNormal();	// 일반수량 옵셥
	void SetGridDataForm();		// 거푸집수량 옵션
	long m_nTabOut;
	long m_nTabNormal;
	long m_nTabForm;


	long m_nTab;
	void InitTabCtrl();
	void SetDataSave();
	void SetDataInit();
	void SetGridData();
	void MakeSameBM(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void SetDataDefaultOut();

	CDialogBMOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogBMOption();

	CButton	m_btnSame;
	CButton	m_btnRecommand;
	CComboBox	m_comboBridge;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CXTPTabCtrl	m_ctlTab;
	CGridCtrlMng	m_Grid;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BM_OPTION };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnButtonRecommand();
	afx_msg void OnButtonSame();
	//}}AFX_MSG
	virtual afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	virtual afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

	void EnableControl();
	void SyncAggregateAndSlump(CRcBridgeApp *pBri);
};
