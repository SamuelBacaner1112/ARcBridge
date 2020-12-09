#pragma once

#define OUTSIDE_REBAR		0
#define SLABINSIDE_REBAR	1
#define WALLINSIDE_REBAR	2
#define HUNCH				3
#define REBARTYPE_COUNT	4
// CDialogSTMFootingMain 대화 상자입니다.

class CDialogSTMArcCornerMain : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMArcCornerMain)

public:
	CDialogSTMArcCornerMain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSTMArcCornerMain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STM_ARCCORNER_MAIN };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
	BOOL m_bUpper;
	BOOL m_bChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
