#pragma once


// CDialogFce 대화 상자입니다.

class AFX_EXT_CLASS CDialogFce : public CDialog
{
	DECLARE_DYNAMIC(CDialogFce)

public:
	CDialogFce(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogFce();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
