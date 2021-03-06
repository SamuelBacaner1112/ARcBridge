#pragma once


// CInputStmFootingStartEnd 대화 상자입니다.

class CInputStmFootingStartEnd : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmFootingStartEnd)

public:
	CInputStmFootingStartEnd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputStmFootingStartEnd();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STM_FOORING_STARTEND };

	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
