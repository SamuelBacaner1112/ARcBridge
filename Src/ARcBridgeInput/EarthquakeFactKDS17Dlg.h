#pragma once


// CEarthquakeFactKDS17Dlg 대화 상자입니다.

class CEarthquakeFactKDS17Dlg : public CUserDialog
{
	DECLARE_DYNAMIC(CEarthquakeFactKDS17Dlg)

public:
	CEarthquakeFactKDS17Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEarthquakeFactKDS17Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EARTHFACT_KDS17 };

public:
	afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
