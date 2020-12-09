// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeOutBMStd.h: interface for the CARcBridgeOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_)
#define AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���� ���� ����

#define COMMON_BM	0x00001		// �������
#define NORMAL_BM	0x00002		// �Ϲݼ���
#define REBAR_BM	0x00004		// ö�ټ���

#define TYPE_BM_NORMAL	0		// �Ϲݼ��� ���� ����
#define TYPE_BM_ACTUAL	1		// �������� ���� ����

#define TYPE_POST_BRIDGE	0	// ���ٸ� Ÿ�� (������)
#define TYPE_POST_NORMAL	1	// ���ٸ� Ÿ�� (�Ϲݿ�)

// ��Ǫ�� Ÿ�� ����
#define FORM_POS_NORMAL			0	// �Ϲ���ũ��Ʈ
#define FORM_POS_FOOTING		1	// ����
#define FORM_POS_WALL_FRONT		2	// ��ü����
#define FORM_POS_WALL_BACK		3	// ��ü���
#define FORM_POS_WINGSL_FRONT	4	// �������������� ����
#define FORM_POS_WINGSL_BACK	5	// �������������� ���
#define FORM_POS_WINGSR_FRONT	6	// �������������� ����
#define FORM_POS_WINGSR_BACK	7	// �������������� ���
#define FORM_POS_WINGEL_FRONT	8	// �������������� ����
#define FORM_POS_WINGEL_BACK	9	// �������������� ���
#define FORM_POS_WINGER_FRONT	10	// �������������� ����
#define FORM_POS_WINGER_BACK	11	// �������������� ���
#define FORM_POS_APS			12	// ���ӽ�����
#define FORM_POS_MASS			13	// mass
#define FORM_POS_LEAN			14	// lean


class CARcBridgeDataStd;
class CARoadOptionStd;
class COutBMDialog;
class COutBM;
class COutBMTogong;
class CDlgOptionBMSheet;
class CBMData;
class CTwinVectorAreaShape;
class CRcBridgeRebar;
class CViewBM;

class AFX_EXT_CLASS CARcBridgeOutBMStd  
{
public:
	CTypedPtrArray <CObArray, CRcBridgeRebar*> m_pArrBri;
	void CheckCalBM();
//	void SetFormByPos(CBMData *pBM, long nPos);
	void MakeBMRcBridgeArray();
	CRcBridgeRebar* GetBridge(long nBri);

	CDocument* m_pDoc;
	CView* m_pView;
	CARoadOptionStd* m_pARoadOptionStd;
	CARcBridgeDataStd* m_pARcBridgeDataStd;

	// ���� ���⼭ dlg
	COutBMDialog *m_pOutBMDialog;
	CHgBaseBMStd *m_pBaseBM;

	// �Ϲݼ���
	CTypedPtrArray <CObArray, COutBM*> m_pArrOutBMNormal;	// �Ϲݼ���
	CTypedPtrArray <CObArray, COutBM*> m_pArrOutBMCommon;	// �������
	CTypedPtrArray <CObArray, COutBMTogong*> m_pArrOutBMTogong;

	// ���� �ɼ� //////////////////////////////////////////////////////
	// ���������� ��ü���� �ɼǿ� ���õ� ����
	long m_nTypeBM;	// ��������
	
	// ������ ���� ���� �ɼ�(��� ��������...)
	CViewBM* m_pViewBM;				// �Ϲݼ����ɼ�(������ ���� �ɼ�)
	CViewBM* m_pViewBMCommon;		// ��������ɼ�(������ ���� �ɼ�)
	CViewBM* m_pViewBMTogong;		// ��������ɼ�(������ ���� �ɼ�)

	BOOL m_bCalNormalBM;		// �Ϲ����� ���� ������ �� ������?
	BOOL m_bCalRebarBM;			// ö������ ���� ������ �� ������?
	
	// ��� ���� ���� �ɼ�(������ �� �߿� ����� ���븸 ����)
	CViewBM* m_pViewBMSheet;	// �����ɼ�(��Ʈ�� ��� �ɼ�)

	// ���� ���� �ɼ�(���� �����̳� ��½� ����ؾ� �� ���׵�)
	BOOL m_bConvertToForm;		// ������½� ������ ��ȯ����...
	BOOL m_bConvertToRef;		// ������½� �� �������·� ��ȯ����...
	BOOL m_bRefCellBMSum;		// ����ǥ �� �� ����
	BOOL m_bDrawElEarth;		// �𵵿� �����ݼ� ǥ��
	long m_nTypeRebarBM_XXX;		// ö������ ���(0:������������, 1:��ü�����ӽ�����γ����� ����)
	BOOL m_bSeparateExcludeRebarMonument;	// ��üö������� ������ ö�ټ��� ����

	// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
	long m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�)	(��������)
	long m_nTypePostBaseLine;	// ���ٸ� ���ؼ�(0 : ���ݼ�, 2 : ����) (��������)
	double m_dThickPostUpper;	// ������ ���ٸ� ��� �β� (��������)
	double m_dThickPostSide;	// ������ ���ٸ� �¿��� �β� (��������)

	CDlgOptionBMSheet* m_pDlgOptionBMSheet;
	//////////////////////////////////////////////////////////////////

	// ��ȣ�� ����� /////////////////////////////////////
	// ��ȣ�� ���� ġ��(������) 
	double m_dBLBaseMaxHeight[2][2];
	double m_dBLBaseMinHeight[2][2];
	double m_dBLBaseMaxWidth[2][2];
	double m_dBLBaseMinWidth[2][2];

	// ��ũ��Ʈ ����
	long m_nTypeConc[2];
	
	// ���ʹ� ��ᷮ
	// ��ȣ�� ������ũ��Ʈ
	double m_dMaterialConc[2];
	
	// ��ȣ�� ���ʰ�Ǫ��
	double m_dMaterialForm[2];
	////////////////////////////////////////////////////////

	void GroupConnectedTvArr(CTwinVectorArrayArray &tvArrArrReturn, CTwinVectorArray &tvArrStructure, BOOL bCheckMeanless=TRUE);
	long ConvertTvArrToTvArrAreaShape(CTwinVectorArray &tvArr, CDoubleArray &dArrAngleSu, CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nBaseNum, BOOL bMerge);
	CARcBridgeOutBMStd(CARcBridgeDataStd *pARcBridgeDataStd=NULL);
	virtual ~CARcBridgeOutBMStd();
	long m_nFlag;
	void Serialize(CArchive &ar);

private:
	
};

#endif // !defined(AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_)
