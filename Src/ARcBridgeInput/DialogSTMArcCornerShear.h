#pragma once


// CDialogSTMArcCornerShear ��ȭ �����Դϴ�.

class CDialogSTMArcCornerShear : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMArcCornerShear)

public:
	CDialogSTMArcCornerShear(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMArcCornerShear();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_ARCCORNER_SHEAR };

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

	void SetDanRebarLocation();
	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	HGINT32 m_nUpper;
	HGINT32 m_nStt;
	HGDOUBLE m_dDiaCycle1Ori[3];
	HGDOUBLE m_dDiaCycle2Ori[3];
	HGINT32 m_nCountStirrupAtDanBuRebarOri;
	HGDOUBLE m_dDiaStirrupAtDanBuRebarOri;
};
