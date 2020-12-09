#pragma once


// CInputStmArcCorner ��ȭ �����Դϴ�.

class CInputStmArcCorner : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmArcCorner)

public:
	CInputStmArcCorner(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputStmArcCorner();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STM_ARC_CORNER };

	BOOL IsValid();
	HGBOOL IsStmUpperArcCorner(HGBOOL iKey);
	HGBOOL IsStmOpenArcCorner(HGBOOL iKey);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	void OnPreInitDialog();

	DECLARE_MESSAGE_MAP()

	virtual afx_msg void OnBnClickedBtnCheckmain();
	afx_msg void OnBnClickedBtnCheckshear();

	virtual void InitModelCombo(HGINT32 iDepth=0);
	virtual void MakeStmTrussData(HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly=FALSE);
	virtual void MakeStmStressFlowData(CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey);
	BOOL InitStmModel(CStmModel *pModel);
public:
};
