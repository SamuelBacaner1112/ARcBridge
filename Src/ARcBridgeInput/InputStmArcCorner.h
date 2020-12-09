#pragma once


// CInputStmArcCorner 대화 상자입니다.

class CInputStmArcCorner : public CInputStmModelBase
{
	DECLARE_DYNAMIC(CInputStmArcCorner)

public:
	CInputStmArcCorner(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputStmArcCorner();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STM_ARC_CORNER };

	BOOL IsValid();
	HGBOOL IsStmUpperArcCorner(HGBOOL iKey);
	HGBOOL IsStmOpenArcCorner(HGBOOL iKey);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
