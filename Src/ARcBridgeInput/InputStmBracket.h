#pragma once


// CInputStmBracket 대화 상자입니다.

class CInputStmBracket : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmBracket)

public:
	CInputStmBracket(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputStmBracket();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STM_BRACKET };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void OnPreInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnCheckmain();
	afx_msg void OnBnClickedBtnCheckshear();

	virtual void InitModelCombo(HGINT32 iDepth=0);
	BOOL InitStmModel(CStmModel *pModel);
	
	virtual void MakeStmTrussData(HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly=FALSE);
	virtual void MakeStmStressFlowData(CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey);
};
