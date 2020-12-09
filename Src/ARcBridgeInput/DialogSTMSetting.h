#pragma once


// CDialogSTMSetting 대화 상자입니다.

class CDialogSTMSetting : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMSetting)

public:
	CDialogSTMSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSTMSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetDataInit();
	void SetDataSave();
	void SetGridData();

	BOOL IsChangedData();
	void SameAllConfigData();

	BOOL IsFooting();

	BOOL IsCornerCloseType();
	BOOL IsCornerOpenType();

	void SetOriStmCornerData();

	DECLARE_MESSAGE_MAP()

protected:
	CStmDataConfig m_DataModeling_Ori;

	HGINT32 m_nOriTypePosRebarTie;
	HGDOUBLE m_dOriUserPosRebarTie;
	HGINT32 m_nTOriypePosCompressStrut;
	HGDOUBLE m_dOriUserPosCompressStrut;

	HGINT32 m_nOriTypeLoadStressFlow;		//응력흐름도 하중재하방법
	HGINT32 m_nOriTypeStmModeling;			//모델링 Type
	HGINT32 m_nOriTypeDistCornerRebar;		//외측경사부의 위치
	HGDOUBLE m_dOriUserDistCornerRebar;		//외측경사부의 위치 설정값
	HGINT32 m_nOriTypeDistHunchRebar;		//내측경사부의 위치
	HGDOUBLE m_dOriUserDistHunchRebar;		//내측경사부의 위치 설정값
	HGINT32 m_nOriInnerAngle;		//부재두께비가 큰 경우 기준각도
	HGINT32 m_nOriTypeStirrupInnerTension;	//내측인장 스트럽부재
	HGDOUBLE m_dOriUserStirrupInnerTension;	//내측인장 스트럽부재 설정값

	CGridCtrlMng m_Grid;

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
};
