// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDrawingOptionStd.h: interface for the CARcBridgeDrawingOptionStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_)
#define AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLR_BACK	 RGB(253, 250, 255)

// ���� �Ϲ�
#define COMMON_TABLESHADE	0x00000001
#define COMMON_TITLEBOX		0x00000002
#define COMMON_NOHCAD		0x00000004
#define COMMON_DISPLAY_ANG_LEN_BASE	0x00000008	// ��Ÿ� ǥ�� ���� ǥ��

// ����鵵
#define VP_PLANE			0x00000001
#define VP_CROSS			0x00000002
#define VP_LONGI			0x00000004
#define VP_LOCATION			0x00000008
#define VP_CANT				0x00000010
#define VP_LONGI_SLOPE		0x00000020
#define VP_LEGEND			0x00000040
#define VP_NOTE				0x00000080
// �Ϲݵ�
#define GEN_PLANE			0x00000001
#define GEN_CROSS			0x00000002
#define GEN_LONGI			0x00000004
#define GEN_DETAIL_A		0x00000008
#define GEN_NOTCH			0x00000010
#define GEN_ORDER			0x00000020
#define GEN_FRONT			0x00000040
#define GEN_FOOT_PLANE		0x00000080
#define GEN_WING_BL			0x00000100
#define GEN_WING_ER			0x00000200
#define GEN_WING_BR			0x00000400
#define GEN_WING_EL			0x00000800
#define GEN_DETAIL_B		0x00001000
#define GEN_NOTE			0x00002000
#define GEN_CONCJOINT		0x00004000
#define GEN_WALL_PLANE		0x00008000
#define GEN_DETAIL_FXJOINT	0x00010000
#define GEN_DETAIL_CRACK	0x00020000
// ������
#define REBAR_STD_SECT		0x00000001
#define REBAR_MAIN_ASSM		0x00000002
#define REBAR_BMD			0x00000004
#define REBAR_SFD			0x00000008
#define REBAR_UPPER_SLAB	0x00000010
#define REBAR_BEGIN_WALL	0x00000020
#define REBAR_END_WALL		0x00000040
#define REBAR_INWALL_FRONT	0x00000080
#define REBAR_INWALL_SECT	0x00000100
#define REBAR_FOOTING_PLANE	0x00000200
#define REBAR_COLUMN_PLANE	0x00000400
#define REBAR_GIRDER_SECT	0x00000800
#define REBAR_DETAIL		0x00001000
#define REBAR_WING_BL		0x00002000
#define REBAR_WING_ER		0x00004000
#define REBAR_WING_BR		0x00008000
#define REBAR_WING_EL		0x00010000
#define REBAR_NUMMB_STD_SECT		0x00020000	// ǥ�شܸ鵵�� ��ö�� �ѹ��� ǥ��
#define REBAR_DETAIL_CHAIR_BLOCK	0x00040000	// chair block �󼼵� 
#define REBAR_CROSSSECTION	0x00080000	// Ⱦ�ܸ� ������
#define REBAR_AFD			0x00100000	// ��ġ���� ��µ�
#define REBAR_DETAIL_COUPLER	0x00200000	// Ŀ�÷� �󼼵�
#define REBAR_LOWER_SLAB	0x00400000
#define REBAR_DETAIL_PRF	0x00800000	//�λ�������� �󼼵�

// ���� ������
#define REBAR_APS_SIDE		0x00000001	// �ܸ鵵
#define REBAR_APS_FRONT		0x00000002	// ���鵵
#define REBAR_APS_PLANE		0x00000004	// ��鵵
#define REBAR_APS_DETAIL_RB	0x00000008	// ö�ٻ�
#define REBAR_APS_DETAIL_A	0x00000010	// ��A
#define REBAR_APS_TABLE_M	0x00000020	// ���ǥ
#define REBAR_APS_TABLE_RB	0x00000040	// ö�� ���ǥ
#define REBAR_APS_NOTE		0x00000080	// �⺻��Ʈ

//���潽����
#define REBAR_CNS_SIDE		0x00000001	// �ܸ鵵
#define REBAR_CNS_FRONT		0x00000002	// ���鵵
#define REBAR_CNS_PLANE		0x00000004	// ��鵵
#define REBAR_CNS_DETAIL_RB	0x00000008	// ö�ٻ�
#define REBAR_CNS_DETAIL_A	0x00000010	// ��A
#define REBAR_CNS_TABLE_M	0x00000020	// ���ǥ
#define REBAR_CNS_TABLE_RB	0x00000040	// ö�� ���ǥ
#define REBAR_CNS_NOTE		0x00000080	// �⺻��Ʈ


// ��Ÿ ���� - ��ȣ��
// ��Ÿ ���� - ����Ÿ�� ����
#define HYUNTA_FRONT		0x00000001
#define HYUNTA_SECT_A		0x00000002
#define HYUNTA_SECT_B		0x00000004
#define HYUNTA_SECT_C		0x00000008
#define HYUNTA_DETAIL_HOLD	0x00000010
#define HYUNTA_DETAIL_SPAC	0x00000020
#define HYUNTA_NOTE			0x00000040
// ���� ��ħ ��ġ��
// ���� �ֻ�
#define BORING_VERSECT		0x00000001
#define BORING_FORCE		0x00000002
#define BORING_LEGEND		0x00000004
#define BORING_NOTE			0x00000008

#define RCSTD1_DESIGN_CONDITION	0x00000001	// ����1_��������
#define RCSTD1_STANDARD_OUTLINE	0x00000002	// ����1_ǥ�شܸ鵵 �ܰ�
#define RCSTD1_CROSS			0x00000004	// ����1_Ⱦ�ܸ鵵
#define RCSTD1_PLAN				0x00000008	// ����1_��鵵
#define RCSTD1_NOTE				0x00000010	// ����1_��Ʈ
#define RCSTD1_COUNT			5
#define RCSTD2_STANDARD			0x00000020	// ����2_ǥ�شܸ鵵
#define RCSTD2_PLAN				0x00000040	// ����2_��鵵
#define RCSTD2_BMD				0x00000080	// ����2_BMD
#define RCSTD2_SFD				0x00000100	// ����2_SFD
#define RCSTD2_MAINASSM_1		0x00000200	// ����2_��ö��������1Cycle
#define RCSTD2_MAINASSM_2		0x00000400	// ����2_��ö��������2Cycle
#define RCSTD2_OBTUS_DETAIL		0x00000800	// ����2_�а��� ������
#define RCSTD2_COUNT			7
#define RCSTD3_GIRDER_DETAIL	0x00001000	// ����3_�Ŵ���
#define RCSTD3_GUARDWALL_DETAIL	0x00002000	// ����3_��ȣ����
#define RCSTD3_NOTE				0x00004000	// ����3_��Ʈ
#define RCSTD3_SPACER			0x00008000	// ����3_����������
#define RCSTD3_DESIGN_METHOD	0x00010000	// ����3_������
#define RCSTD3_COUNT			5

class CARcBridgeDrawingOptionData;

class AFX_EXT_CLASS CARcBridgeDrawingOptionStd : public CObject  
{
public:
	CARcBridgeDrawingOptionStd();
	virtual ~CARcBridgeDrawingOptionStd();

	void Serialize(CArchive& ar);
	void DefaultDrawingOptionCommon();
	void MakeDrawingOption(long nCountOpt=-1);
	CARcBridgeDrawingOptionData* GetDrawingOption(long nBri);
	void SyncApsDowelRebarSeperateRebarOption(long nBir = -1);

	CDocument* m_pDoc;
	long m_nCurPropPage;

	CTypedPtrArray <CObArray, CARcBridgeDrawingOptionData*> m_pArrDrawingOption;

	// ���� �Ϲ�
		// In Tree
	DWORD m_dwDrawCommon;
	long m_nTypeEL;		// Elevation ǥ�� ��� 0 : EL. 15.000, 1 : EL.:1.5000, 2 : EL.=15.000
	long m_nSelectFckDomText;	// ��ũ��Ʈ ���� ���� ǥ�� ���(0 : ��ũ��Ʈ ����, 1 : ���� ���� �ִ�ġ��)
	long   m_nTypeTitleSeparatorBridge;	// �и��� ���� ������ Ÿ��(0:�����, 1:������+�����, 2:������+����/�ݴ�����+�����)
	long   m_nTypeTitleBridge;			// ��ü�� ���� ������ Ÿ��(0:�����, 1:������+�����)
	long  m_nTypeRebarTable;			// 0 : �⺻Ÿ��, 1 : ����ȭŸ��
	BOOL m_bAddRawRebarTable;	// �̰��� ö�� ���ǥ �߰�

		// In Grid
	CString	m_strDgkName;
	CString	m_strXRefPath;
	CString	m_strDomNumHead;
	CStringArray m_strDgkAttrPrompt;
	CStringArray m_strDgkAttrText;
	double m_dWidthRebarTableRect; // ö�����ǥ ����ȭ Ÿ�Կ��� ö�ٻ� Rect�ϳ��� ��
	double m_dHeightRebarTableRect; // ö�����ǥ ����ȭ Ÿ�Կ��� ö�ٻ� Rect�ϳ��� ����
};

#endif // !defined(AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_)
