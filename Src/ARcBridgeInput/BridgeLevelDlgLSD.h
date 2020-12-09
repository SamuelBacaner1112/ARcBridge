#pragma once


// CBridgeLevelDlgLSD 대화 상자입니다.

class CBridgeLevelDlgLSD : public CUserDialog
{
	DECLARE_DYNAMIC(CBridgeLevelDlgLSD)

public:
	CBridgeLevelDlgLSD(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBridgeLevelDlgLSD();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BRIDGELEVEL_LSD };
	int		m_nType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
