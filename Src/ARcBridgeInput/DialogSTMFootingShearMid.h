#pragma once


// CDialogSTMFootingShearMid 대화 상자입니다.

class CDialogSTMFootingShearMid : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMFootingShearMid)

public:
	CDialogSTMFootingShearMid(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSTMFootingShearMid();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STM_FOOTING_SHEAR_MID };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_nCurIdx;
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

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	HGINT32 m_nStt;
	long m_nCtcShear;

	HGDOUBLE m_dDiaRebarOri;
	HGINT32 m_nTypeOri;
	HGDOUBLE m_dCTCOri;
	HGINT32 m_nCountWrapOri;
};
