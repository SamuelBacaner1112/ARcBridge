#pragma once

#define OUTSIDE_REBAR		0
#define SLABINSIDE_REBAR	1
#define WALLINSIDE_REBAR	2
#define HUNCH				3
#define REBARTYPE_COUNT	4
// CDialogSTMFootingMain ��ȭ �����Դϴ�.

class CDialogSTMArcCornerMain : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMArcCornerMain)

public:
	CDialogSTMArcCornerMain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMArcCornerMain();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_ARCCORNER_MAIN };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
	BOOL m_bUpper;
	BOOL m_bChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetDataInit();
	void SetDataSave();
	void SetGridData();

	void SetDataRebarDia();

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	HGDOUBLE m_arrDia[4][6];
	HGDOUBLE m_arrDiaOrg[4][6];
};
