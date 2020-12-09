#pragma once


// CDialogSTMSetting ��ȭ �����Դϴ�.

class CDialogSTMSetting : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMSetting)

public:
	CDialogSTMSetting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMSetting();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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

	HGINT32 m_nOriTypeLoadStressFlow;		//�����帧�� �������Ϲ��
	HGINT32 m_nOriTypeStmModeling;			//�𵨸� Type
	HGINT32 m_nOriTypeDistCornerRebar;		//���������� ��ġ
	HGDOUBLE m_dOriUserDistCornerRebar;		//���������� ��ġ ������
	HGINT32 m_nOriTypeDistHunchRebar;		//���������� ��ġ
	HGDOUBLE m_dOriUserDistHunchRebar;		//���������� ��ġ ������
	HGINT32 m_nOriInnerAngle;		//����β��� ū ��� ���ذ���
	HGINT32 m_nOriTypeStirrupInnerTension;	//�������� ��Ʈ������
	HGDOUBLE m_dOriUserStirrupInnerTension;	//�������� ��Ʈ������ ������

	CGridCtrlMng m_Grid;

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
};
