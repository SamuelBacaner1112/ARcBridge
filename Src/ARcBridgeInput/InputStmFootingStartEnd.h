#pragma once


// CInputStmFootingStartEnd ��ȭ �����Դϴ�.

class CInputStmFootingStartEnd : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmFootingStartEnd)

public:
	CInputStmFootingStartEnd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputStmFootingStartEnd();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STM_FOORING_STARTEND };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	void OnPreInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnCheckmain();
	afx_msg void OnBnClickedBtnCheckshear();
	afx_msg void OnCbnSelchangeComboStmcomboCheck();

	virtual void InitModelCombo(HGINT32 iDepth=0);
	virtual void MakeStmTrussData(HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly=FALSE);
	virtual void MakeStmStressFlowData(CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey);
	virtual BOOL InitStmModel(CStmModel *pModel);
};
