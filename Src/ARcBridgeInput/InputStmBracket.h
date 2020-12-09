#pragma once


// CInputStmBracket ��ȭ �����Դϴ�.

class CInputStmBracket : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmBracket)

public:
	CInputStmBracket(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputStmBracket();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STM_BRACKET };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	void OnPreInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnCheckmain();
	afx_msg void OnBnClickedBtnCheckshear();

	virtual void InitModelCombo(HGINT32 iDepth=0);
	BOOL InitStmModel(CStmModel *pModel);
	
	virtual void MakeStmTrussData(HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly=FALSE);
	virtual void MakeStmStressFlowData(CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey);
};
