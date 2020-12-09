#pragma once


// CDialogBMSelectKind ��ȭ �����Դϴ�.

class CDialogBMSelectKind : public CDialog
{
	DECLARE_DYNAMIC(CDialogBMSelectKind)

public:
	void SetDataSave();
	CViewBM *m_pViewBM;
	void SetViewBM(CViewBM *pViewBM);
	void InitListCtrl();
	CDialogBMSelectKind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogBMSelectKind();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BM_SELECTKIND };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnRevSelect;
	CButton	m_btnNonSelect;
	CButton	m_btnAllSelect;
	CListCtrl	m_listView;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonAllSelect();
	afx_msg void OnButtonNonSelect();
	afx_msg void OnButtonRevSelect();

	DECLARE_MESSAGE_MAP()
};
