// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDrawingOptionData.h: interface for the CARcBridgeDrawingOptionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_)
#define AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeDrawingOptionData : public CObject  
{
public:
	void DefaultDrawingOptionCns();
	CARcBridgeDrawingOptionData();
	virtual ~CARcBridgeDrawingOptionData();


	// ����鵵
	CString m_szDxfPlanName;
	DWORD m_dwDrawVP;
	double m_dScaleVPBase;			// �⺻ ������
	double m_dScaleVPCross;			// Ⱦ�ܸ鵵 ������
	double m_dScaleVPLocation;
	BOOL   m_bDrawVPCoordinateXY;

	// �Ϲݵ�
	DWORD m_dwDrawGen;				// �Ϲݵ�
	BOOL   m_bDrawGenWingWall[4][3];// ����������, [4]���� ��.��, ���� ��.��, [3]-���鵵,�ܸ鵵(����),�ܸ鵵(����)
	BOOL   m_bDrawGenCoordinateXY;
	BOOL   m_bDimTableForPilePlane;	// ��鵵 ���� ���� ���̺� ó��

	double m_dScaleGenBase;			// �⺻ ������
	double m_dScaleGenDetailA;		// �� "A" ������(��ȣ����)
	double m_dScaleGenNotch;		// NOTCH �� ������
	double m_dScaleGenOrder;		// �ð� ������ ������
	double m_dScaleGenWingWall;		// ������ ������
	double m_dScaleGenDetailB;		// �� "B" ������
	double m_dScaleGenConcJoint;	// �ð����� �� ������
	double m_dScaleFXJoint;			// �������� �� ������
	double m_dScaleGenCrack;		// �տ����߿� �� ������

	// ������
	DWORD  m_dwDrawRebar;
	BOOL   m_bDrawRebarWingWall[4][4];		// ����������, [4]���� ��.��, ���� ��.��, [4]-���鵵,�ܸ鵵(����),�ܸ鵵(����), �亮������
	long   m_nTypeMainRebarAssm;			// ��ö�� ������ ǥ�� ���
	double m_dScaleRebarBase;				// �⺻ ������
	double m_dScaleRebarDetail;				// �� ������		-----> ����... 
	double m_dScaleRebarWingWall;			// ������ ������
	double m_dScaleBMD;						// BMD ������
	double m_dScaleSFD;						// SFD ������
	double m_dScaleAFD;						// ��µ� ������
	double m_dScaleRebarDetailArchRibSpringging;	// ��ġ���� ��������� �� ������
	double m_dScaleRebarDetailColumnSection;	// ��� �ܸ� �� ������
	double m_dScaleRebarDetailGirderSection;	// �Ŵ� �ܸ� �� ������
	double m_dScaleRebarDetailSlabAcute;		// ������ �а��� �� ������
	double m_dScaleRebarDetailWallObtuse;		// ��ü ������ �� ������
	double m_dScaleRebarDetailFootingAcute;		// ���� �а��� �� ������
	double m_dScaleRebarDetailPRF;				// �λ�������� �� ������
	BOOL   m_bSeparateRebarBMWing;			// ������ ö�ټ��� �и� ���
	BOOL   m_bSeparateRebarBMColumn;		// ����� ö�ټ��� �и� ���
	BOOL   m_bSeparateRebarBMOther;			// ��Ÿö�� ö�ټ��� �и� ���(��ȣ��, �����ֹ�ħ)
	BOOL   m_bSeparateOutputRebarBMMain;	// ��ü ö�����ǥ �и� ���(������ ������� 2����� �þ)
	long   m_nTypeOutRebarTable;			// ö�����ǥ ��¹��(0:��ü,1:��ö��,��Ÿö�ٱ���,2:������)
	BOOL m_bRebarTableSepByFy;			// ö�����ǥ fy���� �и��ؼ� ǥ��
	double m_dSpaceBetweenRebarDetail;		// �󼼵��� ����
	long   m_nCountRowEachTable;			// ö�����ǥ ���̺�� �ִ� row��
	BOOL   m_bMainRebarNumberingInStandardSection;		// ǥ�شܸ鵵�� ��ö�� �ѹ��� ǥ������?
	BOOL   m_bLengthInfoInMainRebarNumbering;			// ��ö�� ������ ö�� ��ȣ�� ���� ���� ǥ������?
	BOOL   m_bExcludeDoupleMainRebarCrossSection;	// Ⱦ�� ������ ��½� ��ġ�� ��ö�� ǥ�� ����(�� ġ���� �״�� ǥ��)
	BOOL   m_bSepFrontAndBackWallRebar;	// ��ü ������ ���� ����� �и� �ؼ� ǥ��
	BOOL   m_bSepUpperAndLowerFootingRebar;	// ���� ������ ��� �ϸ��� �и��ؼ� ǥ��
	BOOL   m_bSepUpperAndLowerSlabRebar;	// ��ν����� ������ ��� �ϸ��� �и��ؼ� ǥ��
	BOOL   m_bSepUpperSlab2DanAll;	// ��ν����� ������ 2��ö�� ��� ǥ��
	long   m_nDirMarkUpperSlabMainRebar;	// ��ν����� ��ö�� ������ ö�� ���ü� ����(0 : �������� ����, 1 : �������� ����, 2 : �ڵ�)
	BOOL	m_bDrawHunchRebarOnly;	// ��ġ ö�� ������ ���� �׸�.
	long	m_nBasePosPlaneSupportRebarDim;		// ������ ��鱸���� ����ö�� ġ�� ������ġ (0: ������ �¿� ����, 1: �߽ɼ�����ġ)

	// ���� ������ ������
	DWORD  m_dwDrawRebarAps;
	double m_dScaleRebarAps;	// ���ӽ����� ������ ������
	double m_dScaleDetailAps;	// �󼼵� ������
	double m_dSpaceBetweenRebarDetailAps;	// �󼼵��� ����
	BOOL m_bRebarTableSepByFyAps;	// ö�����ǥ fy���� �и��ؼ� ǥ��(���ӽ�����)

	// ���� ������ ������
	DWORD  m_dwDrawRebarCns;
	long   m_nTypeCnsRebar;		// ���潽���� 1, 2 ǥ�����(0 : 1,2�� �Բ�ǥ��, 1 : �и��ؼ� ǥ��)
	double m_dScaleRebarCns;	// ���潽���� ������ ������
	double m_dSpaceBetweenRebarDetailCns;	// �󼼵��� ����

	// ����Ÿ�� ����
	DWORD  m_dwDrawHyunTa;			// �Ϲݵ�
	double m_dScaleHyunTaFront;		// ����Ÿ�� ���� ���鵵 ������
	double m_dScaleHyunTaSection;	// ����Ÿ�� ���� �ܸ鵵 ������
	double m_dScaleHyunTaDetailHold;	// ö�ٿ��� ���� ö�� ������
	double m_dScaleHyunTaDetailSpac;	// SPACER �� ������
	long   m_nTypeHyunTaCut;		// ���� ���� ǥ�� ���( 0 : ��������, 1 : ���� ������, 2 : �ѱ�������)
	long	m_nTypeHyunTaVol;		//���� ���� ǥ�� ��� (0: ����1���� ���� 1. ���� ��ü ����) // ARCBRIDGE-2474

	// ���� �ֻ�
	DWORD m_dwDrawBoring;
	double m_dScaleBoring;

	// ��Ÿ ����
	DWORD  m_dwDrawOthers;
	double m_dScaleRebarOthers;					// ��ȣ�� �⺻ ������
	double m_dScaleDetailGuardWall;				// ��ȣ�� �� ������
	double m_dScaleDetailMonument;				// ������ �� ������
	double m_dSpaceBetweenRebarDetailOthers;	// ��ȣ�� �󼼵��� ����

	// RC �����걳 ǥ�ص�
	DWORD  m_dwDrawRcStd;
	double m_dScaleRcStd1Base;	// ����1 �⺻ ������
	double m_dScaleRcStd2Base;	// ����2 �⺻ ������
	double m_dScaleRcStd3Base;	// ����3 �⺻ ������

	
	void Serialize(CArchive& ar);

	void DefaultDrawingOptionVerticalPlane();
	void DefaultDrawingOptionGeneral();
	void DefaultDrawingOptionRebar();
	void DefaultDrawingOptionAps();
	void DefaultDrawingOptionOthers();
	void DefaultDrawingOptionShoeArrange();
	void DefaultDrawingOptionBoring();
	void DefaultDrawingOptionHyunTa();
	void DefaultDrawingOptionSlabStandardOutput();
};

#endif // !defined(AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_)
