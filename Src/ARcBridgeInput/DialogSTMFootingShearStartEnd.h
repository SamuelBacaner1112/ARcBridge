#pragma once


// CDialogSTMFootingShearStartEnd ��ȭ �����Դϴ�.

class CDialogSTMFootingShearStartEnd : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMFootingShearStartEnd)

public:
	CDialogSTMFootingShearStartEnd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMFootingShearStartEnd();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_FOOTING_SHEAR_STARTEND };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
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

	void SetFootLoation();

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
