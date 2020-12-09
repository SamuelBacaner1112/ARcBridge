#pragma once


// CEarthquakeGradeKDS17Dlg 대화 상자입니다.

class CEarthquakeGradeKDS17Dlg : public CUserDialog
{
public:
	CEarthquakeGradeKDS17Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEarthquakeGradeKDS17Dlg();

	int		m_nType;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EARTHQUACKGRADE_KDS17 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
