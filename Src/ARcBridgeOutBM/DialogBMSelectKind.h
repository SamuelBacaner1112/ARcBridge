#pragma once


// CDialogBMSelectKind 대화 상자입니다.

class CDialogBMSelectKind : public CDialog
{
	DECLARE_DYNAMIC(CDialogBMSelectKind)

public:
	void SetDataSave();
	CViewBM *m_pViewBM;
	void SetViewBM(CViewBM *pViewBM);
	void InitListCtrl();
	CDialogBMSelectKind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogBMSelectKind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BM_SELECTKIND };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnRevSelect;
	CButton	m_btnNonSelect;
	CButton	m_btnAllSelect;
	CListCtrl	m_listView;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonAllSelect();
	afx_msg void OnButtonNonSelect();
	afx_msg void OnButtonRevSelect();

	DECLARE_MESSAGE_MAP()
};
