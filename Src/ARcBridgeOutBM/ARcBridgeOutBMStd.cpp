// ARcBridgeOutBMStd.cpp: implementation of the CARcBridgeOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeOutBM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeOutBMStd::CARcBridgeOutBMStd(CARcBridgeDataStd *pARcBridgeDataStd)
{
	m_pARcBridgeDataStd	= pARcBridgeDataStd;
	m_pBaseBM = new CHgBaseBMStd;

	m_pOutBMDialog = new COutBMDialog;
	m_pOutBMDialog->m_pStd = this;
	m_pOutBMDialog->m_pBaseBM = m_pBaseBM;

	m_nTypeBM	= 0;	// 0 : �Ϲݻ������, 1 : �����������(��������и�). 2 : ���������������(�����������)
	// ������ ���� ���� �ɼ�(��� ��������...)
	m_pViewBM		= new CViewBM;
	m_pViewBMCommon	= new CViewBM;
	m_pViewBMTogong	= new CViewBM;
	m_bCalNormalBM	= TRUE;			// �Ϲ����� ���� ����
	m_bCalRebarBM	= TRUE;			// ö������ ���� ����
	
	// ��� ���� ���� �ɼ�(������ �� �߿� ����� ���븸 ����)
	m_pViewBMSheet	= new CViewBM;

	// ���� ���� �ɼ�(���� �����̳� ��½� ����ؾ� �� ���׵�)
	m_bConvertToForm	= TRUE;		// ������½� ������ ��ȯ����...
	m_bConvertToRef		= FALSE;	// ������½� �� �������·� ��ȯ����...
	m_bRefCellBMSum		= FALSE;		// ����ǥ �� �� ����
	m_bDrawElEarth		= TRUE;		// �𵵿� �����ݼ� ǥ��
	m_nTypeRebarBM_XXX		= 0;		// ö������ ���(0:������������, 1:��ü�����ӽ�����γ����� ����)
	m_bSeparateExcludeRebarMonument	= FALSE;	// ��üö������� ������ ö�ټ��� ����
	
	// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
	m_nTypePost			= 0;		// ���ٸ� Ÿ��(������, �Ϲݿ�)
	m_nTypePostBaseLine	= 0;		// ���ٸ� ���ؼ�(0 : ���ݼ�, 1 : ����)
	m_dThickPostUpper	= 1200;		// ������ ���ٸ� ��� �β�
	m_dThickPostSide	= 1200;		// ������ ���ٸ� �¿��� �β�

	m_pDlgOptionBMSheet	= new CDlgOptionBMSheet("�������� �ɼ�");
	m_pDlgOptionBMSheet->m_pStd	= this;
	//////////////////////////////////////////////////////////////////

	// ��� ���� ���� �ɼ� ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	// ��ȣ�� ��� �� ///////////
	// ��ȣ�� ���� ġ��(������)
	m_dBLBaseMaxHeight[0][0]	= 0.4;
	m_dBLBaseMinHeight[0][0]	= 0.234;
	m_dBLBaseMaxWidth[0][0]		= 0.4;
	m_dBLBaseMinWidth[0][0]		= 0.289;

	m_dBLBaseMaxHeight[0][1]	= 0.4;
	m_dBLBaseMinHeight[0][1]	= 0.225;
	m_dBLBaseMaxWidth[0][1]		= 0.4;
	m_dBLBaseMinWidth[0][1]		= 0.303;

	// ��ȣ�� ���� ġ��(��õ��)
	m_dBLBaseMaxHeight[1][0]	= 0.4;
	m_dBLBaseMinHeight[1][0]	= 0.234;
	m_dBLBaseMaxWidth[1][0]		= 0.4;
	m_dBLBaseMinWidth[1][0]		= 0.290;

	m_dBLBaseMaxHeight[1][1]	= 0.4;
	m_dBLBaseMinHeight[1][1]	= 0.225;
	m_dBLBaseMaxWidth[1][1]		= 0.4;
	m_dBLBaseMinWidth[1][1]		= 0.303;

	// ��ũ��Ʈ ����
	m_nTypeConc[0]				= 0;		// ���ʿ� : 3��
	m_nTypeConc[1]				= 1;		// ��ä��� : 5��

	// ���ʹ� ��ᷮ
	// ��ȣ�� ������ũ��Ʈ
	m_dMaterialConc[0]			= 0.151;	// 1.5	
	m_dMaterialConc[1]			= 0.152;	// 1.8
	// ��ȣ�� ���ʰ�Ǫ��
	m_dMaterialForm[0]			= 0.634;	// 1.5
	m_dMaterialForm[1]			= 0.625;	// 1.8

	m_nFlag	= 0;
}

CARcBridgeOutBMStd::~CARcBridgeOutBMStd()
{
	AhTPADelete(&m_pArrOutBMTogong, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMNormal, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMCommon, (COutBM*)0);
	AhTPADelete(&m_pArrBri, (CRcBridgeRebar*)0);

	delete m_pBaseBM;
	delete m_pOutBMDialog;
	delete m_pViewBM;
	delete m_pViewBMCommon;
	delete m_pViewBMTogong;
	delete m_pViewBMSheet;

	delete m_pDlgOptionBMSheet;
}

// ���� ����� �ʿ��� ��ŭ�� �迭�� �����.(�������� �ϳ��� ����)
void CARcBridgeOutBMStd::MakeBMRcBridgeArray()
{
	AhTPADelete(&m_pArrOutBMTogong,	(COutBMTogong*)0);
	AhTPADelete(&m_pArrOutBMNormal, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMCommon, (COutBM*)0);
	AhTPADelete(&m_pArrBri, (CRcBridgeRebar*)0);

	long i = 0; for(i = 0; i < m_pARcBridgeDataStd->GetBridgeSize(); i++)
	{
		// �Ϲݼ���
		COutBM *pOutBM	= new COutBM(i);
		pOutBM->m_pStd = this;
		m_pArrOutBMNormal.Add(pOutBM);

		// �������
		pOutBM = new COutBM(i);
		pOutBM->m_pStd = this;
		m_pArrOutBMCommon.Add(pOutBM);

		// �������
		COutBMTogong *pOutBMTogong	= new COutBMTogong(i);
		pOutBMTogong->m_pStd	= this;
		m_pArrOutBMTogong.Add(pOutBMTogong);

		// ��������� ����
		CRcBridgeRebar *pBri = new CRcBridgeRebar;
		pBri->m_bBridgeForBM = TRUE;
		m_pArrBri.Add(pBri);
	}
}

void CARcBridgeOutBMStd::Serialize(CArchive &ar)
{
	// 1 : �����ɼ��� FALSE�� ����Ǽ� �ɼ��� �ȳ����� ���� ��ħ.�ϴ� 1��������Ÿ�� TRUE�� �о����.
	// 2 : m_bRefCellBMSum;		// ����ǥ �� �� ����
	// 3 : m_bSeparateExcludeRebarMonument;	// ��üö������� ������ ö�ټ��� ����
	// 4 : m_pViewBMCommon ��������ɼ� �߰�.
	m_nFlag	= 4;

	if(ar.IsStoring())
	{
		ar << m_nFlag;
		// ���� �ɼ� //////////////////////////////////////////////////////
		ar << m_nTypeBM;		// 0 : �Ϲݻ������, 1 : �����������.

		// ������ ���� ���� �ɼ�(��� ��������...)
		m_pViewBM->Serialize(ar);				// �Ϲݼ����ɼ�(������ ���� �ɼ�)
		m_pViewBMCommon->Serialize(ar);			// ��������ɼ�(������ ���� �ɼ�)
		m_pViewBMTogong->Serialize(ar);			// ��������ɼ�(������ ���� �ɼ�)
		ar << m_bCalNormalBM;			// �Ϲ����� ���� ����
		ar << m_bCalRebarBM;			// ö������ ���� ����
		
		// ��� ���� ���� �ɼ�(������ �� �߿� ����� ���븸 ����)
		m_pViewBMSheet->Serialize(ar);	// �����ɼ�(��Ʈ�� ��� �ɼ�)

		// ���� ���� �ɼ�(���� �����̳� ��½� ����ؾ� �� ���׵�)
		ar << m_bConvertToForm;	// ������½� ������ ��ȯ����...
		ar << m_bConvertToRef;		// ������½� �� �������·� ��ȯ����...
		ar << m_bRefCellBMSum;		// ����ǥ �� �� ����
		ar << m_bDrawElEarth;		// �𵵿� �����ݼ� ǥ��
		ar << m_nTypeRebarBM_XXX;		// ö������ ���(0:������������, 1:��ü�����ӽ�����γ����� ����)
		ar << m_bSeparateExcludeRebarMonument;	// ��üö������� ������ ö�ټ��� ����
		
		// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
		ar << m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�)
		ar << m_nTypePostBaseLine;	// ���ٸ� ���ؼ�(0 : �ڵ�, 1 : ���ݼ�, 2 : ����)
		ar << m_dThickPostUpper;	// ������ ���ٸ� ��� �β�
		ar << m_dThickPostSide;		// ������ ���ٸ� �¿��� �β�
		//////////////////////////////////////////////////////////////////

		// ��� ���� ���� �ɼ� ///////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
	}
	else
	{
		ar >> m_nFlag;
		// ���� �ɼ� //////////////////////////////////////////////////////
		ar >> m_nTypeBM;		// 0 : �Ϲݻ������, 1 : �����������.

		// ������ ���� ���� �ɼ�(��� ��������...)
		m_pViewBM->Serialize(ar);			// �Ϲݼ����ɼ�(������ ���� �ɼ�)
		if(m_nFlag > 3)
			m_pViewBMCommon->Serialize(ar);	// ��������ɼ�(������ ���� �ɼ�)
		m_pViewBMTogong->Serialize(ar);		// ��������ɼ�(������ ���� �ɼ�)
		if(m_nFlag < 1)
		{
			long i = 0; for(i = 0; i < m_pViewBM->m_pArrViewBM.GetSize(); i++)
				m_pViewBM->SetBMState(i, TRUE);
			for(i = 0; i < m_pViewBMTogong->m_pArrViewBM.GetSize(); i++)
				m_pViewBMTogong->SetBMState(i, TRUE);
		}
		ar >> m_bCalNormalBM;			// �Ϲ����� ���� ����
		ar >> m_bCalRebarBM;			// ö������ ���� ����
		
		// ��� ���� ���� �ɼ�(������ �� �߿� ����� ���븸 ����)
		m_pViewBMSheet->Serialize(ar);	// �����ɼ�(��Ʈ�� ��� �ɼ�)

		// �Ϲ� ���� ���� �ɼ�(���� �����̳� ��½� ����ؾ� �� ���׵�)
		ar >> m_bConvertToForm;		// ������½� ������ ��ȯ����...
		ar >> m_bConvertToRef;		// ������½� �� �������·� ��ȯ����...
		if(m_nFlag > 1)
			ar >> m_bRefCellBMSum;		// ����ǥ �� �� ����
		ar >> m_bDrawElEarth;		// �𵵿� �����ݼ� ǥ��
		ar >> m_nTypeRebarBM_XXX;		// ö������ ���(0:������������, 1:��ü�����ӽ�����γ����� ����)
		if(m_nFlag > 2)
			ar >> m_bSeparateExcludeRebarMonument;	// ��üö������� ������ ö�ټ��� ����
		
		// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
		ar >> m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�)
		ar >> m_nTypePostBaseLine;	// ���ٸ� ���ؼ�(0 : �ڵ�, 1 : ���ݼ�, 2 : ����)
		ar >> m_dThickPostUpper;	// ������ ���ٸ� ��� �β�
		ar >> m_dThickPostSide;		// ������ ���ٸ� �¿��� �β�
		//////////////////////////////////////////////////////////////////

		// ��� ���� ���� �ɼ� ///////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
	}
}

// tvArr�� tvArrAreaShape�� ��ġ
long CARcBridgeOutBMStd::ConvertTvArrToTvArrAreaShape(CTwinVectorArray &tvArr, CDoubleArray &dArrAngleSu, CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nBaseNum, BOOL bMerge)
{
	if(!pArrAreaConc) return nBaseNum;
	if(!bMerge)
		AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	long nSize	= dArrAngleSu.GetSize();
	long nIdx	= 0;
	for(long i = 0; i < nSize; i++)
	{
		long nCountAngle = (long)dArrAngleSu.GetAt(i);

		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape;
		pArea->m_nNum	= nBaseNum++;
		for(long j = 0; j < nCountAngle; j++)
		{
			pArea->AddTwinVector(tvArr.GetAt(nIdx++));
		}
		pArea->Calculate();
		pArrAreaConc->Add(pArea);
	}

	return nBaseNum;
}

// ������ ��Ǫ�� Ÿ�� ��������
// void CARcBridgeOutBMStd::SetFormByPos(CBMData *pBM, long nPos)
// {
// }

void CARcBridgeOutBMStd::CheckCalBM()
{
	if(!m_pARcBridgeDataStd) return;

	long nTypeBM	= m_pARcBridgeDataStd->m_nTypeBMApply;

	for(HGINT32 ix = 0; ix < m_pArrOutBMNormal.GetSize(); ix++)
	{
		// �Ϲݼ���
		COutBM *pOutBMNor = m_pArrOutBMNormal.GetAt(ix);
		if(pOutBMNor == NULL)
			continue;

		pOutBMNor->m_dwCalBM = NORMAL_BM;

		// ��������� �и����� ���� ��쿡�� ��������� �����ϵ��� �߰��Ѵ�. 
		if(m_pARcBridgeDataStd->m_nTypeBMApply != 1)
			pOutBMNor->m_dwCalBM |= COMMON_BM;


		///////////////////////////////////////////////////////////////////////////////////////////
		// �������
		COutBM *pOutBMCom = m_pArrOutBMCommon.GetAt(ix);
		if(pOutBMCom == NULL)
			continue;

		// ������� �и� �ɼ��� ��쿡�� ����ǵ��� �Ѵ�. 
		pOutBMCom->m_dwCalBM = (nTypeBM == 1) ? COMMON_BM : 0x00000000;
	}
}

/**
	@brief
		tvArrStructure�� tv�߿��� ����� ������ �з�, ������ tvArrArrReturn�� ����.\n
		CHgRcStructureGenStd���� ����.
*/
void CARcBridgeOutBMStd::GroupConnectedTvArr(CTwinVectorArrayArray &tvArrArrReturn, CTwinVectorArray &tvArrStructure, BOOL bCheckMeanless)
{
	AhTPADelete(&tvArrArrReturn, (CTwinVectorArray*)0);
	if(tvArrStructure.GetSize() == 0)	return;
	
	CTwinVectorArray tvArrBase;
	tvArrBase	= tvArrStructure;
	
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrBase, 0);
	tvArrBase.Sort();
	if(bCheckMeanless)
		rb.RemoveMeanlessTwinVector(tvArrBase);
	tvArrBase.Sort();
	
	CTwinVectorArray *pTvArrTemp	=  NULL;
	long i = 0;
	for(i = 0; i < tvArrBase.GetSize(); i++)
	{
		CTwinVector *pTv	= tvArrBase.GetPAt(i);
		if(!pTvArrTemp)
		{
			pTvArrTemp	=  new CTwinVectorArray;
			pTvArrTemp->Add(*pTv);
			tvArrBase.RemoveAt(i);
		}
		else
		{
			CTwinVector tvLast;
			pTvArrTemp->GetLast(tvLast);
			if(ComparePt(CDPoint(tvLast.m_v2), CDPoint(pTv->m_v1), "="))
			{
				pTvArrTemp->Add(*pTv);
				tvArrBase.RemoveAt(i);
			}
			else
			{
				tvArrArrReturn.Add(pTvArrTemp);
				pTvArrTemp	= NULL;
				tvArrBase.Sort();
				//i++;
			}
		}
		i--;
	}
	if(pTvArrTemp)
		tvArrArrReturn.Add(pTvArrTemp);
}

CRcBridgeRebar* CARcBridgeOutBMStd::GetBridge( long nBri )
{
	CRcBridgeRebar *pBri = NULL;
	long nSize = m_pArrBri.GetSize();
	if(nBri > -1  && nBri < nSize)
		pBri = m_pArrBri.GetAt(nBri);

	return pBri;
}
