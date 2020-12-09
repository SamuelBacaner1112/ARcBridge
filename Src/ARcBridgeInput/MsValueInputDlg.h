#pragma once


// CMsValueInputDlg 대화 상자입니다.

class CMsValueInputDlg : public CUserDialog
{
	DECLARE_DYNAMIC(CMsValueInputDlg)

public:
	CMsValueInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMsValueInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MSVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
