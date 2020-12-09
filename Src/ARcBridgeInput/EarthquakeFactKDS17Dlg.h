#pragma once


// CEarthquakeFactKDS17Dlg ��ȭ �����Դϴ�.

class CEarthquakeFactKDS17Dlg : public CUserDialog
{
	DECLARE_DYNAMIC(CEarthquakeFactKDS17Dlg)

public:
	CEarthquakeFactKDS17Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEarthquakeFactKDS17Dlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EARTHFACT_KDS17 };

public:
	afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CGridCtrlMng m_Grid;
	long m_nOption;

	void SetDataSave();
	void SetGridData();
	void SetDataInit();

};
