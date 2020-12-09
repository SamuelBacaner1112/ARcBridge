#pragma once


// CDlgCorner ��ȭ �����Դϴ�.

class AFX_EXT_CLASS CDlgCorner : public CDialog
{
	DECLARE_DYNAMIC(CDlgCorner)

public:
	CDlgCorner(CARcBridgeInputStd* pIn, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCorner();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DANCORNER };
	double	m_dUpSlab_1Dan;
	double	m_dUpSlab_2Dan;
	double	m_dUpSlab_3Dan;
	double	m_dLoSlab_1Dan;
	double	m_dLoSlab_2Dan;
	double	m_dLoSlab_3Dan;	

	// (ARCBRIDGE-3636)
	double m_dUpSlab_CornerVert;
	double m_dLoSlab_CornerVert;
	BOOL	  m_bUpSlab_CornerCurve;
	BOOL	  m_bLoSlab_CornerCurve;

private:
	CARcBridgeInputStd *m_pIn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDefault();
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnClickOption();

	DECLARE_MESSAGE_MAP()

public:
	void SetInitData();
};
