// OutBMTogong.cpp: implementation of the COutBMTogong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutBMTogong::COutBMTogong(long nBri)
{
	m_nBri	= nBri;

	// �� ��¿� ���� �ɼǵ� /////////////////////////////////////////
	m_dScaleDim_VertSection		= 100;	// ���ܸ鵵 ������
	m_dHeightText_VertSection	= 7;	// ���ܸ鵵 �ؽ�Ʈ ����
	////////////////////////////////////////////////////////////////////
}

COutBMTogong::~COutBMTogong()
{
	ClearBM();
}

// ������ ��� ������.
// �����ѵ� ������ �ٷ� ��.
void COutBMTogong::CalculateBM()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	ClearBM();

	if(pBri->IsRcSlab()) return;

	CRcBridgeRebar *pBriOrg	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	m_pStd->m_pARcBridgeDataStd->MakeRcBridgeForBM(pBriOrg, m_pStd->GetBridge(m_nBri));

	// ���� ����
	MakeBMTyopagi();			// ���ı�
	MakeBMWaterOut();			// ��Ǫ��
	MakeBMBohoTyopagi();		// ��ȣ�� ���ı�
	MakeBMBackFill();			// ��ä��
	MakeBMReFill();			// �Ǹ޿��
	MakeBMReFillBoho();		// ��ȣ�� �Ǹ޿��
	MakeBMValuableSoil();		// ������
	MakeBMValuableSoilBoho();	// ��ȣ�� ������
	MakeBMDeductSoil();		// ������
	MakeBMApsungto();			// �ռ���
	
	// ������ ���� ViewBM�� ���
	long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		if(pBri->IsFootingJijum(nJ) == FALSE) continue;
		SetViewBM(&m_pArrBMTogongStd[nJ]);
		ApplyOptionBMStd(&m_pArrBMTogongStd[nJ]);
	}
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		SetViewBM(&m_pArrBMBohoStd[stt]);
		// ����ѵ� ������ ������ ViewBM�ɼ��� ���� ��Ų��.
		ApplyOptionBMStd(&m_pArrBMBohoStd[stt]);
	}
}

// �׳� ������ ������ ���ؼ� �ʿ信 ���� �̸����� ������ �ش�.
void COutBMTogong::MergeBMStd()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// ���ʴ�� ����. /////////////////////
	CHgBaseBMStd baseBM;
	DWORD exceptionFlag	= 0x00000000;
	long stt(0), jijum(0);


	// ���� ���� ���� ������ ������.(����ٰ� ��¿�) - ���������� ������ ���ؼ��� ������.
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogong[jijum], &m_pArrBMTogongStd[jijum], FALSE, exceptionFlag);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBoho[stt], &m_pArrBMBohoStd[stt], FALSE, exceptionFlag);
	}
	
	// ������ ����ǥ��
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogongForPartSum[jijum], &m_pArrBMTogong[jijum], TRUE, exceptionFlag);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBohoForPartSum[stt], &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// ��������ǥ��(���+��ȣ��) - �Ѱ賻�� ������ ���� ������
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogongSum, &m_pArrBMTogong[jijum], TRUE, exceptionFlag);
	}

	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBohoSum, &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}
	
	// ��������ǥ ��
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMTogongSum, TRUE, exceptionFlag);
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMBohoSum, TRUE, exceptionFlag);
}

// ���� ��¿� ���ڿ� ����
void COutBMTogong::MakeBMStringStd()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CHgBaseBMStd baseBM;
	long stt(0), jijum(0);

	// ������ ����ٰſ� ����
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MakeBMString(&m_pArrBMStringTogong[jijum], &m_pArrBMTogong[jijum], TRUE);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MakeBMString(&m_pArrBMStringBoho[stt], &m_pArrBMBoho[stt], TRUE);
	}

	// ������ ����ǥ(�������, ��ȣ������)
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MakeBMSumString(&m_pArrBMStringTogongSum, &m_pArrBMTogongForPartSum[jijum], TRUE, jijum==0);		// ��ü
	}
	baseBM.MakeBMSumString(&m_pArrBMStringTogongSum, &m_pArrBMTogongSum, TRUE, FALSE);		// ��ü

	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoForPartSum[stt], TRUE, stt==0);		// ��ȣ��
	}
	baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoSum, TRUE, FALSE);		// ��ȣ��

	// ��������ǥ��(���+��ȣ��)
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMTogongSum, TRUE, TRUE);		// ��ü
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMBohoSum, TRUE, FALSE);		
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSum, TRUE, FALSE);	// ��
}

// ���� ����
void COutBMTogong::ClearBM(BOOL bRemainStdBM)
{
	long jijum(0), stt(0);

	if(bRemainStdBM == FALSE)
	{
		// ������(�����ȵ� ���� ����)
		for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
		{
			AhTPADelete(&m_pArrBMTogongStd[jijum],	(CBMData*)0);			// �������(���� �ȵȰ�)
		}
		for(stt = 0; stt < 2; stt++)
		{
			AhTPADelete(&m_pArrBMBohoStd[stt],		(CBMData*)0);			// ��ȣ������(���� �ȵȰ�)
		}
	}

	// ��¿�(������ ����)
	for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		AhTPADelete(&m_pArrBMTogong[jijum],		(CBMData*)0);			// �������(���� �ȵȰ�) - ����ٰ� ���
		AhTPADelete(&m_pArrBMStringTogong[jijum],	(CBMString*)0);			// �������(���� �ȵȰ�) - ����ٰ� ���
	}
	
	for(stt = 0; stt < 2; stt++)
	{
		AhTPADelete(&m_pArrBMBoho[stt],			(CBMData*)0);			// ��ȣ������(���� �ȵȰ�) - ����ٰ� ���
		AhTPADelete(&m_pArrBMStringBoho[stt],	(CBMString*)0);			// ��ȣ������(���� �ȵȰ�) - ����ٰ� ���
	}

	// ����������ǥ�� ��µ� ����
	for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		AhTPADelete(&m_pArrBMTogongForPartSum[jijum],	(CBMData*)0);
	}

	for(stt = 0; stt < 2; stt++)
	{
		AhTPADelete(&m_pArrBMBohoForPartSum[stt],	(CBMData*)0);;	
	}

	AhTPADelete(&m_pArrBMTogongSum,			(CBMData*)0);				// ������� - ������ ����ǥ ��� 
	AhTPADelete(&m_pArrBMStringTogongSum,	(CBMString*)0);
		
	AhTPADelete(&m_pArrBMBohoSum,			(CBMData*)0);				// ��ȣ������ - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMStringBohoSum,		(CBMString*)0);

	AhTPADelete(&m_pArrBMStringAllSum,		(CBMString*)0);				// ����(�������+��ȣ��) - ������ ����ǥ ��� 
	AhTPADelete(&m_pArrBMSum,				(CBMData*)0);				// ��������(���� �ȵȰ�) - ������ ����ǥ ���

	AhTPADelete(&m_pArrBMStringSepBridgeSum,		(CBMSumString*)0);
	AhTPADelete(&m_pArrBMSepBridgeSum,				(CBMData*)0);	
}

// ������ ViewBM�� �����.
void COutBMTogong::SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		m_pStd->m_pViewBMTogong->AddBM(pArrBM->GetAt(i)->m_sBigClass);
	}
}

// ������ ViewBM�� ������Ѽ� �ʿ� ���°Ŵ� ������.
void COutBMTogong::ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		if(!m_pStd->m_pViewBMTogong->IsCheckView(pArrBM->GetAt(i)->m_sBigClass))
		{
			delete pArrBM->GetAt(i);
			pArrBM->RemoveAt(i);
			i--;
		}
	}
}

// ������ ������������ ��������� ���� �ɼ� ����
// ��, ���Ǽ����� ��� BMData�� �ɼ��� �켱���� ����
void COutBMTogong::ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		CBMData *pBM	= pArrBM->GetAt(i);
		if(pBM->m_sForm == "" || pBM->m_bIsPrintByRefForm)
		{
			pBM->m_bIsPrintByRefCell	= m_pStd->m_bConvertToRef;
			pBM->m_bIsPrintByRefForm	= m_pStd->m_bConvertToForm;
		}
		
		pArrBM->SetAt(i, pBM);
	}
}

// ������ �ɼ��� ������.
void COutBMTogong::ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	ApplyViewBM(pArrBM);
	ApplyRefCellBM(pArrBM);
}


// ���� �۾����� bri ����
CRcBridgeRebar* COutBMTogong::GetRcBridgeRebar(BOOL bOrg)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return NULL;
	CRcBridgeRebar *pBri	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);

	return bOrg ? pBri : m_pStd->GetBridge(m_nBri);
}

// ������ �̸��� �ɼǿ� ���� �ٲ� ��츦 ����ؼ� �Լ��� ó����.
CString COutBMTogong::GetStringBigClass(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxBigClass == BM_BIG_TUPAGI)					return "���ı�";
	if(nIdxBigClass == BM_BIG_TUPAGI_WATER)		return pBri->m_bApplyYongSu? "������ı�" : "�������ı�";
	if(nIdxBigClass == BM_BIG_BACKFILL)				return "��ä��";
	if(nIdxBigClass == BM_BIG_REFILL)					return "�Ǹ޿��";
	if(nIdxBigClass == BM_BIG_VALUABLE_SOIL)		return "������";
	if(nIdxBigClass == BM_BIG_DEDUCT_SOIL)		return "������";
	if(nIdxBigClass == BM_BIG_APSUNGTO)			return "�ռ���";
		
	return _T("");
}

// ������ �̸��� �ɼǿ� ���� �ٲ� ��츦 ����ؼ� �Լ��� ó����.
CString COutBMTogong::GetStringMidClass(long nIdxMidClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxMidClass == BM_MID_TUPAGI_TOSA)		return "���";
	if(nIdxMidClass == BM_MID_TUPAGI_PUWHAAM)	return "ǳȭ��";
	if(nIdxMidClass == BM_MID_TUPAGI_YUNAM)		return "����";
	if(nIdxMidClass == BM_MID_TUPAGI_GYUNGAM)	return "���";


	return _T("");
}

CString COutBMTogong::GetStringSmallClass(long nIdxSmallClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxSmallClass == BM_SMALL_DEDUCT_SOIL_NOSANG)			return "���";
	if(nIdxSmallClass == BM_SMALL_DEDUCT_SOIL_NOCHE)			return "��ü";

	return _T("");
}

// �Ѱ� ���� ���� ����
long COutBMTogong::GetLevelTotal(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;

	if(nIdxBigClass == BM_BIG_TUPAGI)			return 4;
	if(nIdxBigClass == BM_BIG_TUPAGI_WATER)		return 4;
	if(nIdxBigClass == BM_BIG_BACKFILL)			return 0;
	if(nIdxBigClass == BM_BIG_REFILL)			return 0;
	if(nIdxBigClass == BM_BIG_VALUABLE_SOIL)	return 0;
	if(nIdxBigClass == BM_BIG_DEDUCT_SOIL)		return 2;
	if(nIdxBigClass == BM_BIG_APSUNGTO)			return 0;
	
	
	
	return 0;
}

CString COutBMTogong::GetStringItemClass(long nIdxItemClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");
	
	if(nIdxItemClass == BM_ITEM_BACKFILL)						return "��ä��";
	if(nIdxItemClass == BM_ITEM_BACKFILL_APS)					return "���ӽ����� ����";
	if(nIdxItemClass == BM_ITEM_BACKFILL_WING)					return "������ ����";
	if(nIdxItemClass == BM_ITEM_BACKFILL_BRACKET)				return "����� ����";
	if(nIdxItemClass == BM_ITEM_REFILL_TYPAGI)					return "���ı�";
	if(nIdxItemClass == BM_ITEM_REFILL_MAIN)					return "��ü����";
	if(nIdxItemClass == BM_ITEM_REFILL_BACKFILL)				return "��ä�����";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_MAINS)					return "��ü(����)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_MAINE)					return "��ü(����)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGSL)				return "������(��������)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGSR)				return "������(��������)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGEL)				return "������(��������)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGER)				return "������(��������)";

	return _T("");
}

// ����ٰ��� ���� �߰��� ���� �Ǵ� �𵵸� �߰���Ų��.
void COutBMTogong::SetInsertDomyunStd()
{
	SetInsertDomyunBackFill();
	SetInsertDomyunApsungto();
}

// ���ͺ��� ������ �Ǵ� ������ ���������� ���������ش�.
void COutBMTogong::RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dYBase, double dFirstMeter, double dSecondMeter, long nIdxStt, long nIdxEnd)
{
	if(!pArrBM) return;
	long nSize		= pArrBM->GetSize();
	if(dFirstMeter < 0 && dSecondMeter > 0) return;
	if(dFirstMeter > 0 && dSecondMeter < 0) return;
	if(dSecondMeter == 0) return;

	BOOL bRev		= dFirstMeter < 0 && dSecondMeter < 0;
	long nIdxMeter	= 0;
	double dYCur	= 0;
	double dYOld	= 0;
	if(nIdxStt == -1) nIdxStt	= 0;
	if(nIdxEnd == -1) nIdxEnd	= nSize-1;
	if(nIdxEnd >= nSize) nIdxEnd	= nSize-1;

	long i = 0; for(i = nIdxStt; i <= nIdxEnd; i++)
	{
		CBMData *pBM	= pArrBM->GetAt(i);
		if(!IsPrintMeter(pBM)) continue;

		dYCur		= dYBase;
		dYOld		= dYBase;
		nIdxMeter	= 0;

		while(1)
		{
			dYCur += nIdxMeter==0 ? dFirstMeter : dSecondMeter;

			if((bRev && Compare(dYCur, frM(pBM->m_dV[EndMeter]), "<=")) || (!bRev && Compare(dYCur, frM(pBM->m_dV[EndMeter]), ">=")))
			{
				pBM->m_sMeterClass.Format("%s(%.0f~%.0fm)", pBM->m_sSmallClass, toM(fabs(dYOld-dYBase)), toM(fabs(dYCur-dYBase)));
				pBM->m_nIdxMeterClass	= nIdxMeter;
				break;
			}
			nIdxMeter++;

			dYOld	= dYCur;
		}
	}
}

// �������� ����ϴ� ������ �ڸ��� ��� ���������� ����� ��� �ȴ�.
BOOL COutBMTogong::IsPrintMeter(CBMData *pBM)
{
	if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI) return TRUE;
	if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI_WATER) return TRUE;

	return FALSE;
}

// �������� ������ ù��° �ڸ� ���̿� �� ���� �ڸ� ���� ������.
// CFootingApp *pFooting : ���ı� ���� �������� ���� ���Ҷ��� �ʿ���
void COutBMTogong::GetInfoMeter(long nIdxBigClass, double &dFirstMeter, double &dSecondMeter, CFootingApp *pFooting)
{
	if(!pFooting) return;

	if(nIdxBigClass == BM_BIG_TUPAGI)
	{
		dFirstMeter		= -pFooting->m_dDepthDiggingFirst;
		dSecondMeter	= -pFooting->m_dDepthDiggingNext;
	}
}

// ��� ����ٰ� ù������ ��
// ���ı� (�ܸ鵵, ���鵵)(�� ���ʺ�)
long COutBMTogong::DrawFirstPageTogong(CHgBaseBMStd *pBaseBMStd, long jijum)
{
	long nRow	= 5;
	if(!pBaseBMStd) return nRow;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return nRow;

	// ���鵵 ������� ��� �׷��� ��.
	// (ARCBRIDGE-3549) BOX���� ���ı�� ���������� ���������� ���Ѵ�.
	pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2, TRUE);

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawVertSection *pDrawVertSection		= DBStd.m_pDrawVertSection;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	
	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg			= 400;	// ���� ����ũ��
	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;

	// ������ ������ ���� ///////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	pDrawVertSection->DrawPartByJijum(&domTmp, jijum, TRUE);
	if(bDrawRubble)
		pDrawVertSection->DrawExFootingBoxType(&domTmp, TRUE);
	pDrawVertSection->DrawTogongLine(&domTmp, pBri, jijum, TRUE, FALSE, FALSE, FALSE);
	pDrawVertSection->DimTogongLine(&domTmp, pBri, jijum, TRUE, FALSE, FALSE);
	/////////////////////////////

	// ���ı� �� �׸��� ///////////////////////////////////////////////////////////
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
	if(!pFooting)
	{
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
		return nRow;
	}

	if(!pFooting->m_bIs) 
	{
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
		return nRow;
	}

	CDomyun domVertSection;
	domVertSection.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domVertSection, dWidthOrg, &domTmp);
	///////////////////////////////////
	
	// ����� �׸�
	pDrawVertSection->DrawPartByJijum(&domVertSection, jijum, TRUE);
	if(bDrawRubble)
		pDrawVertSection->DrawExFootingBoxType(&domVertSection, TRUE);
	pDrawVertSection->DrawTogongLine(&domVertSection, pBri, jijum, TRUE, FALSE, FALSE, FALSE);
	pDrawVertSection->DimTogongLine(&domVertSection, pBri, jijum, TRUE, FALSE, FALSE, TRUE);

	//BOX���϶��� �ܸ� ��ü�� �׷���
	if(pBri->IsBoxType())
	{
		DBStd.m_pDrawVertSection->DrawVertSection(&domVertSection, TRUE);
	}

	// �𵵸� WidthOrg ������� ����
	domVertSection.RedrawByScale(dScale, TRUE);

	// ������ ����
	domVertSection.SetCalcExtRect();
	CDRect rect	= domVertSection.GetExtRect();

	CString sTitle	= _T("");
	if(jijum == 0)
		sTitle	= "������ ���ı� �ܸ�";
	else if(jijum == pBri->GetCountJijum()-1)
		sTitle	= "������ ���ı� �ܸ�";
	else
		sTitle.Format("�߰�����%d ���ı� �ܸ�", jijum);
	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &domVertSection, dArrTmp, rect.Width(), rect.Height(), sTitle) + 5;

	// ���ı� �� ���鵵

	// ���� ����  ////////////////
	CDomyun domTmp2;
	domTmp2.SetCWnd(m_pStd->m_pView);

	// ����� �׸�
	//pDrawCrossSection->DrawCrossAllAtJijum(&domTmp2, jijum, FALSE, FALSE, FALSE, FALSE);

	// ����� �׸�
	pDrawCrossSection->DrawTogongLine(&domTmp2, jijum, TRUE, FALSE);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	pDrawCrossSection->DimTogongLine(&domTmp2, jijum, FALSE, TRUE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);
	// ���� ����  ////////////////

	CDomyun domFront;
	domFront.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domFront, dWidthOrg, &domTmp2);
	///////////////////////////////////

	// ����� �׸�
	//pDrawCrossSection->DrawCrossAllAtJijum(&domFront, jijum, FALSE, FALSE, FALSE, FALSE);
	// pDrawCrossSection->DrawCrossAllAtJijum�� �׸��� ���� ���õǾ� �ִ� ���� Ǯ�� �����Ƿ�
	// ���ʿ� Ȯ����ʸ� ���� �׸���
	CExFootingApp *pExFooting	= &pFooting->m_exFooting;
	DBStd.m_pDrawFooting->DrawLeftSide(&domFront, pFooting, FALSE, 0, TRUE, TRUE);
	DBStd.m_pDrawExFooting->DrawLeftSide(&domFront, pExFooting, 0);
	
	if(pBri->IsBoxType())
	{
		DBStd.m_pDrawCrossSection->DrawCrossAllAtJijum(&domFront, 0, TRUE, FALSE, TRUE, TRUE, bDrawRubble);
	}

	// ����� �׸�
	pDrawCrossSection->DrawTogongLine(&domFront, jijum, TRUE, FALSE, FALSE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	pDrawCrossSection->DimTogongLine(&domFront, jijum, FALSE, TRUE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);

	// �𵵸� WidthOrg ������� ����
	domFront.RedrawByScale(dScale, TRUE);
	
	// ������ ����
	domFront.SetCalcExtRect();
	rect	= domFront.GetExtRect();

	sTitle	= _T("");
	if(jijum == 0)
		sTitle	= "������ ���ı� ����";
	else if(jijum == pBri->GetCountJijum()-1)
		sTitle	= "������ ���ı� ����";
	else
		sTitle.Format("�߰�����%d ���ı� ����", jijum);

	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &domFront, dArrTmp, rect.Width(), rect.Height(), sTitle) + 3;


	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	return nRow;
}

// ��ȣ�� ���� ù������ ��
void COutBMTogong::DrawFirstPageBoho(CHgBaseBMStd *pBaseBMStd, BOOL bStt)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	
	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg			= 600;	// ���� ����ũ��
	double dHeightOrg			= 800;	// ���� ����ũ��

	// ���ı� �� �׸��� ///////////////////////////////////////////////////////////
	long nRow	= 3;
	
	CDomyun Dom;
	Dom.SetCWnd(m_pStd->m_pView);

	// ���� ��ȣ�� dxf ����.
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	
	// ���뺸ȣ���� �������� �Ǿ� ������ ��õ������ �����ϰ� ����.
	CString strDxfFileName		= nTypeBoho == 2  ? "���뺸ȣ��(������).dxf" : "���뺸ȣ��(��õ��).dxf";
	CString strDxfName			= _T("");
	CString sBoho				= _T("");
	
	strDxfName.Format("%s\\dxf\\%s", szPath, strDxfFileName);
	//sBoho.Format("���뺸ȣ��(%s)", nTypeBoho == 2 ? "������" : "��õ��");
	
	if(!Dom.DxfIn(strDxfName)) AfxMessageBox("���뺸ȣ�� �������� ����!");

	// ������ ����
	Dom.SetCalcExtRect();
	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &Dom, dArrTmp, dWidthOrg, dHeightOrg, sBoho);
}

// ��ȣ�� ���ı�(������)
void COutBMTogong::MakeBMBohoTyopagi()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;

	CRebarPlacing rb;

	// ���
	const double dTyopagiDepth	= 1000;		// ���뺸ȣ�� ���ı� ����
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> pArrAreaConc;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt		= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];

		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
	
		double dLenWall			= toM(pBri->GetTvJijum(nJ, 0).GetLength());
		double dLenBohoFooting	= toM(2*ConstPi*GetLengthBohoHor(bStt)*15*2/360) + dLenWall;

		// ��ȣ�� ���ʿ� �ش��ϴ� footingApp�� �����ؼ� ���ı� ������ ���� /////////////////////
		// ���� ������ ���� �ϸ鸸 ������ �ǹǷ� ���� Ÿ���� FOOTING_TYPE_BASIC_BOTH�� �׳� �Ѵ�.
		double dElEarth		= 0;
		pLine->GetElevationErathOnStation(pBri->GetStationOnJijum(nJ, 0), dElEarth);

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		CFootingApp footing;
		footing.m_nType		= FOOTING_TYPE_BASIC_BOTH;
		footing.m_dFH		= 400;
		footing.m_dEL		= dElEarth - dTyopagiDepth + footing.m_dFH;
		footing.m_dSta		= pFooting->m_dSta;
		footing.m_dFWR		= 200;
		footing.m_dFWL		= 200;
		footing.m_dMR		= 0;
		footing.m_dML		= 0;
		footing.m_dDistOffsetDigging	= 1000;
		footing.m_xyBottomCenter.x	= footing.m_dSta;
		footing.m_xyBottomCenter.y	= footing.m_dEL - footing.GetHeight();
		footing.m_dEL_DiggingFirst	= dElEarth;
		footing.m_dDiggingSlopeTosa	= pFooting->m_dDiggingSlopeTosa;
		footing.m_dDiggingSlopeGyungam	= pFooting->m_dDiggingSlopeGyungam;
		footing.m_dDiggingSlopePungwhaam	= pFooting->m_dDiggingSlopePungwhaam;
		footing.m_dDiggingSlopeYunam	= pFooting->m_dDiggingSlopeYunam;
		
//		CBoringDataEx *pExBoring	= pBri->GetBoringDataExByStation(footing.m_dSta);

		CBoringDataEx *pExBoring	= pBri->GetBoringDataEx(nJ);
		if(!pExBoring) continue;

		CTwinVectorArray tvArr;
		pBri->GetTvTogongLine(tvArr, &footing, pExBoring, TRUE, FALSE, TRUE, FALSE);

		// �������� ������.
		CTwinVectorArray tvArrResult;
		CDoubleArray dArrAngleSu;
		long nIdx	= 0;

		rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
		long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
		{
			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(j+1);
			for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
				pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

			pArea->Calculate();
			pArrAreaConc.Add(pArea);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		
		// ���� ����
		long nSizeBM	= pArrBM->GetSize();
		long nSize		= pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pArrAreaConc.GetAt(i);

			double dWidthUpper	= toM(pArea->m_dWidthUpper);
			double dWidthLower	= toM(pArea->m_dWidthLower);
			double dHeight		= toM(pArea->m_dHeight);
			
			// ���� Ÿ�Կ� ���� �̸�
			long nMidClass	= 0;
			CTwinVector tvLeft	= pArea->GetTvLeft();
				
			if(tvLeft.m_sInfo.Find("���", 0) != -1) nMidClass = BM_MID_TUPAGI_TOSA;
			else if(tvLeft.m_sInfo.Find("ǳȭ��", 0) != -1) nMidClass = BM_MID_TUPAGI_PUWHAAM;
			else if(tvLeft.m_sInfo.Find("����", 0) != -1) nMidClass = BM_MID_TUPAGI_YUNAM;
			else if(tvLeft.m_sInfo.Find("���", 0) != -1) nMidClass = BM_MID_TUPAGI_GYUNGAM;
			else nMidClass	= BM_MID_TUPAGI_TOSA;
			
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_TUPAGI;		
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= nMidClass;			
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;		
			pBM->m_sUnit				= "��";
			pBM->m_dV[SttMeter]			= 0;
			pBM->m_dV[EndMeter]			= 4;
			pBM->m_nShape				= pArea->m_nTypeShape;
			pBM->m_dV[WidthUpper]		= Round(dWidthUpper,3);
			pBM->m_dV[Length]			= Round(dLenBohoFooting,3);
			pBM->m_dV[WidthLower]		= Round(dWidthLower,3);
			pBM->m_dV[Height]			= Round(dHeight,3);
			pBM->CalculateResult();
			pBM->m_sVCN[Result]			= "��ȣ�����ı�";
			pArrBM->Add(pBM);
		}

		// ������ ���������� �����ؼ� �־��ش�
		// ��ȣ���� �����̴�.(0 ~ 4m)
		RemakeByHeightBMList(pArrBM, 0, 4000, 1000, nSizeBM-1, pArrBM->GetSize()-1);
		AhTPADelete(&pArrAreaConc,	(CTwinVectorAreaShape*)0);
	}

	AhTPADelete(&pArrAreaConc,	(CTwinVectorAreaShape*)0);
}

// ��ȣ�� ���� ����
// ������ο��� ���� 600��ŭ �� �� �� ������ ������
double COutBMTogong::GetLengthBohoHor(BOOL bStt)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	
	// ��ȣ���� �⺻������ ������ �������� ������, ���ٸ� �����ʵ� ��
	// �����ʵ� ���ٸ� 0 ����
	BOOL bLeft	= pBri->IsWingWall(bStt, TRUE);
	if(!bLeft && !pBri->IsWingWall(bStt, FALSE)) return 0;

	CTwinVectorArray tvArrBoho;
	pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);

	return tvArrBoho.GetHorLength();
}

// ���ı�
void COutBMTogong::MakeBMTyopagi()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> pArrAreaConc[2];

	CRebarPlacing rb;
	CHgBaseBMStd baseBM;

	pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2, TRUE);
	
	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		CTwinVectorArray tvArrVert, tvArrCross;	// ���ܰ� Ⱦ�ܸ��� ��������� ��� ���ؾ� ��.
		pBri->GetTvTogongLine(tvArrVert, jijum, FALSE, TRUE, TRUE, FALSE);
		pBri->GetTvVertSection_Std(0, TRUE, TRUE, FALSE, 2);
		pBri->GetTvTogongLine(tvArrCross, jijum, TRUE, TRUE, TRUE, FALSE, pBri->m_nDeductNearTupagiBM);
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);

		// 32223
		//���谡 ��� 0�� �ƴ϶�� False ��� 0�̶�� TRUE
		double dYBase		= 0;
		dYBase	= tvArrVert.GetXyTop().y; //�̰����� �ű�

		BOOL bGooBae = FALSE;
		CTwinVectorArray tvArrResultGooBae;

		if(pFooting->m_dDiggingSlopeTosa == 0 && pFooting->m_dDiggingSlopePungwhaam == 0 && 
			pFooting->m_dDiggingSlopeYunam == 0 && pFooting->m_dDiggingSlopeGyungam == 0)
		{
			bGooBae = TRUE;
		}
		//}
		
		// �������� ������. ////////////////////////////////////////////////////////////////////////
		for(long vert = 0; vert < 2; vert++)
		{
			CTwinVectorArray tvArrResult;
			CDoubleArray dArrAngleSu;
			CTwinVectorArray tvArr;
			long nIdx	= 0;
			tvArr	= vert==0 ? tvArrVert : tvArrCross;

			//32223 ��� ���谡 0�̰� ����EL�� ������ ��쿡�� �����
			if(bGooBae && pFooting->m_dEL_DiggingWater > 1)
			{
				rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 0.0, FALSE, FALSE);

				CalcZeroGooBaeExcavation(tvArrResult, tvArrResultGooBae, dYBase, pFooting->m_dEL_DiggingWater ); 
			}
			else
			{
				CTwinVectorArray tvArrTosa, tvArrPung, tvArrYun, tvArrKyung, tvArrEtc;

				for (long nIdx =0; nIdx < tvArr.GetSize(); nIdx++)
				{
					CTwinVector tvTu = tvArr.GetAt(nIdx);
					if(tvTu.m_sInfo.Find(_T("���")))
						tvArrTosa.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("ǳȭ��")))
						tvArrPung.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("����")))
						tvArrYun.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("���")))
						tvArrKyung.Add(tvTu);
					else
						tvArrEtc.Add(tvTu);
				}

				long nCntSize = 0;
				if(tvArrTosa.GetSize() > 0)
					nCntSize++;
				if(tvArrPung.GetSize() > 0)
					nCntSize++;
				if(tvArrYun.GetSize() > 0)
					nCntSize++;
				if(tvArrKyung.GetSize() > 0)
					nCntSize++;
				if(tvArrEtc.GetSize() > 0)
					nCntSize++;

				BOOL bMeanLessSameType = nCntSize > 1? FALSE : TRUE;
				rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 0.0, FALSE, bMeanLessSameType);
			}

			long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(j+1);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
				{
					if(bGooBae )
						pArea->AddTwinVector(tvArrResultGooBae.GetAt(nIdx++));
					else
						pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
				}

				pArea->Calculate();
				pArrAreaConc[vert].Add(pArea);
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		// ���ͺ��� ������ ///////
		//double dYBase		= 0;// 32223
		double dFirstMeter	= 0;
		double dSeconeMeter	= 0;
		GetInfoMeter(BM_BIG_TUPAGI, dFirstMeter, dSeconeMeter, pFooting);
		//dYBase	= tvArrVert.GetXyTop().y; // 32223
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[0], dYBase, dFirstMeter, dSeconeMeter);
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[1], dYBase, dFirstMeter, dSeconeMeter);

		// ���� ���ı� ������ �ѹ� �� �ڸ�
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[0], dYBase, pFooting->m_dEL_DiggingWater - dYBase, -tvArrVert.GetVerLength());
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[1], dYBase, pFooting->m_dEL_DiggingWater - dYBase, -tvArrVert.GetVerLength());
		///////////////////////////
		
		// ���ܰ� Ⱦ���� ��¥�� ��������� ������ ���ƾ� ����� �ǹǷ� Ȥ�ö� �ٸ��� ������ ������.
		if(pArrAreaConc[0].GetSize() != pArrAreaConc[1].GetSize()) continue;

		// ���� ����
		long nSizeBM	= pArrBM->GetSize();
		long nSize		= pArrAreaConc[0].GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea[2]	= {pArrAreaConc[0].GetAt(i), pArrAreaConc[1].GetAt(i)};
			
			double dWidthUpper[2]	= {toM(pArea[0]->m_dWidthUpper), toM(pArea[1]->m_dWidthUpper)};
			double dWidthLower[2]	= {toM(pArea[0]->m_dWidthLower), toM(pArea[1]->m_dWidthLower)};
			double dHeight			= toM(pArea[0]->m_dHeight);
						
			// ���� Ÿ�Կ� ���� �̸�
			long nMidClass	= 0;
			CTwinVector tvLeft	= pArea[0]->GetTvLeft();

			BOOL bWaterTyopagi	= Compare(tvLeft.GetXyTop().y, pFooting->m_dEL_DiggingWater, "<=");
			
			if(tvLeft.m_sInfo.Find("���", 0) != -1) nMidClass = BM_MID_TUPAGI_TOSA;
			else if(tvLeft.m_sInfo.Find("ǳȭ��", 0) != -1) nMidClass = BM_MID_TUPAGI_PUWHAAM;
			else if(tvLeft.m_sInfo.Find("����", 0) != -1) nMidClass = BM_MID_TUPAGI_YUNAM;
			else if(tvLeft.m_sInfo.Find("���", 0) != -1) nMidClass = BM_MID_TUPAGI_GYUNGAM;
			else nMidClass	= BM_MID_TUPAGI_TOSA;
		
			
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= bWaterTyopagi ? BM_BIG_TUPAGI_WATER : BM_BIG_TUPAGI;		
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= nMidClass;			
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			if(bWaterTyopagi)
				pBM->m_sMidClass	= " " + pBM->m_sMidClass + " ";
			pBM->m_nTypeResult			= RESULT_VOLUMN;		
			pBM->m_sUnit				= "��";
			pBM->m_dV[SttMeter]			= Round(toM(tvLeft.GetXyTop().y),3);
			pBM->m_dV[EndMeter]			= Round(toM(tvLeft.GetXyBottom().y),3);
			pBM->m_nShape				= SHAPE_TRAPEZOID;
			// ��ܸ� ��չ����� ����(������ ������ ����)
			// ��Ȯ�� �Ϸ��� ���԰��� �̿��ؾ� �ϳ� �����ϰ�, ����翡�� ������ ������� �����Ƿ�
			// �� �� �´��� ��ܸ� ��չ��� �����.
			pBM->m_sForm.Format("(((%.3f x %.3f) + (%.3f x %.3f)) / 2) x %.3f", dWidthUpper[0], dWidthUpper[1], dWidthLower[0], dWidthLower[1], dHeight);
			pBM->SetResult((((dWidthUpper[0]*dWidthUpper[1])+(dWidthLower[0]*dWidthLower[1]))/2)*dHeight);
			pBM->m_sVCN[Result]			= "���ı�";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);
		}

		// ������ ���������� �����ؼ� �־��ش�
		RemakeByHeightBMList(pArrBM, dYBase, dFirstMeter, dSeconeMeter, nSizeBM-1, pArrBM->GetSize()-1);

		AhTPADelete(&pArrAreaConc[0],	(CTwinVectorAreaShape*)0);
		AhTPADelete(&pArrAreaConc[1],	(CTwinVectorAreaShape*)0);
	}

	AhTPADelete(&pArrAreaConc[0],	(CTwinVectorAreaShape*)0);
	AhTPADelete(&pArrAreaConc[1],	(CTwinVectorAreaShape*)0);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 1);
}

/**
	@brief
		��Ǫ�� ���� �߰�
*/
void COutBMTogong::MakeBMWaterOut()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];


		double dWaterTupagiTosa	= 0;
		double dWaterTupaiOfPungwhaAm	= 0;
		double dWaterTupagOfYunAmGyungAm	= 0;

		// ���� ���ı� �������� ��Ǫ�� ������ ������.
		for(HGINT32 ix = 0; ix < pArrBM->GetSize(); ++ix)
		{
			CBMData *pBM	= pArrBM->GetAt(ix);
			if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI_WATER)
			{
				if(pBM->m_nIdxMidClass == BM_MID_TUPAGI_TOSA)
					dWaterTupagiTosa += pBM->GetResult();
				else if(pBM->m_nIdxMidClass == BM_MID_TUPAGI_PUWHAAM)
					dWaterTupaiOfPungwhaAm += pBM->GetResult();
				else 
					dWaterTupagOfYunAmGyungAm += pBM->GetResult();
			}
		}

		CMakeBMStd makeBMStd;
		makeBMStd.InitBMWaterOut(BM_BIG_WATEROUT);
		makeBMStd.MakeBMWaterOut(pArrBM, dWaterTupagiTosa, dWaterTupaiOfPungwhaAm, dWaterTupagOfYunAmGyungAm, pBri->m_dTimeConcForm);
	}
	
}

// ��ä��
// ��ä�� ���ѵ�
// ������, ���ӽ�����, ����� �����ؾ� ��

// nForUse : 0 : ���� ��ä��, 1 : �Ǹ޿��� ��ä�� ����, 2 : ��������� ��ä�� ����, 3 : ��ü������� ��ä�� ����
// 1 (�Ǹ޿���) : ��ä�� �Ϻο��� ���꼱(���ı���)����
// 2 (���������) : ��� ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)
// 3 (��ü�������) : ��ü ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)
// 4 (��ä����������) : ���� ��ä�򼱿��� ����κ� ����.
// ����������� ���Ҷ��� ������������ �Ÿ��� ���ϰ�, �������� ���� ���� �ʴ´�.
// �ֳ��ϸ�, ������� ���κο��� ���� ��������� �������Ҵ��� ���� ���̱� ������.
void COutBMTogong::MakeBMBackFill(long nForUse)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);
	CRebarPlacing rb;

	long nIdxStt[2] = {0, 0};
	long nIdxEnd[2] = {0, 0};
	CDPoint vAng = pBri->GetAngleJijum(0);
	long stt = 0; 
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(!pBri->IsOutWall(bStt)) continue;	// ������ü�� ������ ��ä�� ����

		//Box Type�϶��� �������� ��� ���.
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;	 
		if(pBri->IsBoxType()) nJ = 0;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		// ��ä�� ���� ����
		// ����ϻ����� ������ �����̹Ƿ� �׳� ��ٸ��÷� ���ϸ� ��
		double dX	= bStt ? pBri->GetStationOnJijum(0, 0, TRUE)-pBri->m_dWS : pBri->GetStationOnJijum(pBri->m_nQtyJigan, 0, TRUE)+pBri->m_dWE;
		// ������� �������κ��� ����
		if(nForUse==2 || nForUse == 3 )
		{
			dX	= bStt ? pBri->GetStationOnJijum(0, 0, TRUE) : pBri->GetStationOnJijum(pBri->m_nQtyJigan, 0, TRUE);
		}

		CTwinVector tvBackFill		= pBri->GetTvBackFill(bStt, nForUse);
		double dHoriBackFill		= tvBackFill.GetHorLength();
		double dDistOffsetBackFill	= fabs(tvBackFill.m_v1.x - dX);
		
		//ARCBRIDGE-3080 ������ �����ϱ�� ��
		//double dLenBackFill			= pBri->GetWidthSlabAct(bStt ? pBri->m_dStationBridgeStt : pBri->m_dStationBridgeStt + pBri->m_dLengthBridge, vAng, 0, FALSE, FALSE);		
		CTwinVector tvSlab;
		pBri->GetTvWingWallPlane(bStt);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܿ���", tvSlab);
		double dLenBackFill = tvSlab.GetLength();

		//todo
		CFootingApp* pFooting = NULL;
		//�ڽ����� ��� �������ʰ� �������� ��Ȱ���� �Ѵ�.
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		else 
			pFooting	= &pBri->m_footingBox;
		double dELBackFill = tvBackFill.GetXyTop().y;



		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_BACKFILL;		
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= -1;
		pBM->m_sMidClass			= _T("");
		if(pBri->IsBoxType())
		{
			pBM->m_nIdxPartClass		= bStt ? 0 : 1;
			pBM->m_sPartClass	    	= bStt ? "����" : "����";
		}
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxItemClass		= BM_ITEM_BACKFILL;
		pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;		
		pBM->m_sUnit				= "��";
		pBM->m_nShape				= SHAPE_TRAPEZOID;
		pBM->m_dV[WidthUpper]		= Round(toM(tvBackFill.GetHorLength()+dDistOffsetBackFill),3);
		pBM->m_dV[WidthLower]		= Round(toM(dDistOffsetBackFill),3);
		pBM->m_dV[Height]			= Round(toM(tvBackFill.GetVerLength()),3);
		pBM->m_dV[Length]			= Round(toM(dLenBackFill),3);

		pBM->CalculateResult();
		pArrBM->Add(pBM);

		if(!pBri->IsBoxType())
		{
			nIdxStt[stt]	= pArrBM->GetSize();
			nIdxEnd[stt]	= nIdxStt[stt];
		}
		else
		{
			nIdxStt[0]	= pArrBM->GetSize();
			nIdxEnd[0]	= nIdxStt[0];
		}
		
	}
	
	// ����������� ���Ҷ��� ������������ �Ÿ��� ���ϰ�, �������� ���� ���� �ʴ´�.
	// �ֳ��ϸ�, ������� ���κο��� ���� ��������� �������Ҵ��� ���� ���̱� ������.
	if(nForUse == 2 || nForUse == 3) 
	{
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
		return;
	}


	// 1. ������ ����(COutBM�� �ִ� ������ ��ũ��Ʈ �� ���ϴ� �Լ����� ���������� ������ �̾ƿͼ� ������)
	// ���������� ���� ���� �������� ��ũ��Ʈ ���� ������
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->MakeBMConcreteWing(nForUse+1);

	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(pBri->IsBoxType() && !bStt) continue;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		// ������ ������ �°� ������
		nIdxEnd[stt]	= pArrBM->GetSize()-1;
		
		if(nIdxStt[stt] > -1 && nIdxEnd[stt] > nIdxStt[stt])
		{
			long i = 0; for(i = nIdxStt[stt]; i <= nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_BACKFILL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= -1;
				pBM->m_sMidClass			= _T("");
				pBM->m_nIdxSmallClass = 0;

				pBM->m_nIdxSmallClass	= -1;
				pBM->m_sSmallClass		= _T("");
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxItemClass	= BM_ITEM_BACKFILL_WING;

				pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
				pBM->CalculateResult();
			}
		}
	}

	// ����ϰ� ���ӽ������ �Ǹ޿�� �� ������ ���� �������� �ʴ´�.
	// ��Ȯ�ϰ� �������� �׶� ������
	if(1)//nForUse == 0)
	{
		// 2. ����� ����
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			if(pBri->IsBoxType()) nJ = 0;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tvBackFill		= pBri->GetTvBackFill(bStt, nForUse);
				
				CTwinVector tvBracket;
				pBri->GetTvWingWallPlane(bStt, TRUE);
				
				if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����", tvBracket))
				{
					CTwinVectorArray tvArrBracket;
					pBri->GetTvVertSectionBracket(tvArrBracket, bStt);
					CDRect rect	= tvArrBracket.GetRect();
					rect.top	= tvBackFill.GetXyTop().y;
					rect.bottom	= tvBackFill.GetXyBottom().y;
					rect.left -= 10;
					rect.right += 10;
					
					CTwinVectorArray tvArrRect;
					CTwinVectorArray tvArrResult;
					tvArrRect.SetFromRect(rect);
					rb.SubTractTvArrAndTvArr(tvArrBracket, tvArrRect, tvArrResult);

					if(tvArrResult.GetSize() == 0) continue;

					CTwinVectorAreaShape area;
					area.m_tvArr	= tvArrResult;
					area.Calculate();

					CBMData *pBM	= new CBMData;
					pBM->m_nIdxBigClass			= BM_BIG_BACKFILL;		
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= -1;
					pBM->m_sMidClass			= _T("");
					pBM->m_nIdxSmallClass = 1;
					if(pBri->IsBoxType())
					{
						pBM->m_nIdxPartClass		= bStt ? 0 : 1;
						pBM->m_sPartClass	    	= bStt ? "����" : "����";
					}
					pBM->m_nIdxItemClass		= BM_ITEM_BACKFILL_BRACKET;
					pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;		
					pBM->m_sUnit				= "��";
					pBM->m_nShape				= area.m_nTypeShape;
					pBM->m_dV[Width]			= Round(toM(area.m_dWidth),3);
					pBM->m_dV[WidthLower]		= Round(toM(area.m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(area.m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(area.m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(area.m_dWidthDiffRight),3);
					pBM->m_dV[Height]			= Round(toM(area.m_dHeight),3);
					pBM->m_dV[HeightLeft]		= Round(toM(area.m_dHeightLeft),3);
					pBM->m_dV[HeightRight]		= Round(toM(area.m_dHeightRight),3);
					pBM->m_dV[Length]			= Round(toM(tvBracket.GetLength()),3);
					pBM->m_bIsUseForDeduct		= TRUE;
					pBM->CalculateResult();
					pArrBM->Add(pBM);
				}
			}

			if(!pBri->IsBoxType())
			{
				nIdxStt[stt]	= pArrBM->GetSize();
				nIdxEnd[stt]	= nIdxStt[stt];
			}
			else
			{
				nIdxStt[0]	= pArrBM->GetSize();
				nIdxEnd[0]	= nIdxStt[stt];
			}
		}

		// 3. ���ӽ����� ����
		// ���������� ���� ���� �������� ��ũ��Ʈ ���� ������
		COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
		//pOutBM->MakeRcBridgeForBM();
		// ������ �̹� �����ѻ����̹Ƿ� �ٸ����·� ������ �ϱ� ���ؼ���
		// ������ �ϰ� �ٲ�� �Ѵ�
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);

		pBri->SyncBridgeAps(0, FALSE);
		CTwinVector tvBackFill[2]	= {pBri->GetTvBackFill(TRUE, nForUse), pBri->GetTvBackFill(FALSE, nForUse)};
		pOutBM->MakeBMConcreteAps(TRUE, tvBackFill[0], tvBackFill[1]);
		for(stt = 0; stt < 2; stt++) // ���ӽ����� ��ũ��Ʈ ���ϴ� �Լ��� ������ ��� ���ϹǷ� ���⼭�� �ѹ��� ȣ�� �ϸ� ��
		{
			BOOL bStt	= stt == iSTT;
			if(pBri->IsBoxType() && !bStt) continue;
			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			// ������ ������ó�� ��Ȯ�ϰ� ������������� �߸��� �κ��� �˻��ؾ� ������, 
			// �ϴ��� �׳� ���ӽ����� ��ũ��Ʈ��� �����ϰ� ����.(���� �׷��� �������� ������)
			// 1. ������ ����(COutBM�� �ִ� ������ ��ũ��Ʈ �� ���ϴ� �Լ����� ���������� ������ �̾ƿͼ� ������)
	
			// ������ ������ �°� ������
			nIdxEnd[stt]	= pArrBM->GetSize()-1;
			long i = 0; for(i = nIdxStt[stt]; i <= nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_BACKFILL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= -1;
				pBM->m_sMidClass			= _T("");
				pBM->m_nIdxSmallClass	= -1;
				pBM->m_sSmallClass		= _T("");
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxItemClass	= BM_ITEM_BACKFILL_APS;
				pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
				if(pBM->m_nShape != -1)
					pBM->CalculateResult();
			}
		}
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
}

// ��ä��� �� ����
// 1. ������ ����
void COutBMTogong::SetInsertDomyunBackFill()
{
	// �������� ��� �����ؼ��� ���� ������ ����Ѵ�.
	// ��ճ��� ���� �ʿ� ����, ������ EL�� ��Ȯ�ϰ� �ϱ� ���ؼ���.
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();	
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CHgBaseDrawStd baseDraw;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	double dWidthOrg	= 350;
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->CalAreaConcWing(pBri, 5);	// ��ä�� ������ ������ ���� ����.

	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;
	// ��ä�򿡼� ������ ������ �׸�
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		double dY	= 0;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		if(!pBri->IsWingWall(bStt, TRUE) && !pBri->IsWingWall(bStt, FALSE)) continue;
		pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2);

		// ������ ������ /////////////
		CDomyun domTmp;
		domTmp.SetCWnd(m_pStd->m_pView);
		
		
		double dCutXPos = 0;
		//�ܸ� Box�ڸ� ��ǥ
		if(pBri->IsBoxType())
			dCutXPos = bStt ? pBri->GetStationOnJijum(0) + 3000 : pBri->GetStationOnJijum(pBri->m_nQtyJigan) - 3000;

		// ���ܸ� �׸�
	
		// ��ü
		DBStd.m_pDrawVertSection->DrawPartByJijum(&domTmp, nJ, FALSE);
		// ��ä��
		DBStd.m_pDrawVertSection->DrawTogongLine(&domTmp, pBri, nJ, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE);
		DBStd.m_pDrawVertSection->DimTogongLine(&domTmp, pBri, nJ, FALSE, TRUE, FALSE, TRUE);


		CDRect rect = domTmp.GetExtRect();
		//�ڽ����ϰ�� ��ä���� ���� �������� �� ���ԵǹǷ� ������ �߶�����
		if(pBri->IsBoxType())
		{
			if(bStt) rect.right = dCutXPos;
			else	rect.left = dCutXPos;
			domTmp.TrimByRect(rect, FALSE);
		}

		// ������ ������ /////////////
		
		// ��ũ��Ʈ ����� ���� �������� �������°� ���������� ���̴°� �߰�
		CDomyun domWing;
		domWing.SetCWnd(m_pStd->m_pView);

		// ���� ���� ////////////////////
		double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
		baseBMStd.SetDomyunValue(&domWing, dWidthOrg, &domTmp);
		///////////////////////////////////

		// ��ä�� �� �׸�
		CDomyun domBack(&domWing);

		//�ܸ�
		if(pBri->IsBoxType())
		{
			DBStd.m_pDrawVertSection->DrawVertSection(&domBack, TRUE);
			if(pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE)
				DBStd.m_pDrawVertSection->DrawExFootingBoxType(&domTmp, TRUE);
		}
		// ��ü
		DBStd.m_pDrawVertSection->DrawPartByJijum(&domBack, nJ, FALSE);
		// ��ä��
		DBStd.m_pDrawVertSection->DrawTogongLine(&domBack, pBri, nJ, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE);
		DBStd.m_pDrawVertSection->DimTogongLine(&domBack, pBri, nJ, FALSE, TRUE, FALSE, TRUE);

		rect	= domBack.GetExtRect();
		//�ڽ����ϰ�� ��ä���� ���� �������� �� ���ԵǹǷ� ������ �߶�����
		if(pBri->IsBoxType())
		{
			if(bStt) rect.right = dCutXPos;
			else	rect.left = dCutXPos;;
			domBack.TrimByRect(rect, FALSE);
			domBack.CutLightning(dCutXPos, rect.bottom, dCutXPos, rect.top);
		}
		// �𵵸� WidthOrg ������� ����
		domBack.RedrawByScale(dScale);
		domBack.SetCalcExtRect();

		rect	= domBack.GetExtRect();
		// ������ ��ġ�� �ű�
		domBack.Move(CDPoint(-rect.left + dWidthOrg/2, dY));
		domBack.SetCalcExtRect();
		
		domWing << domBack;
		pBri->GetTvVertSection_Std(0, TRUE, TRUE, TRUE, 2);

		dY -= (rect.Height() + 30);
		// �¿��� ������ ���� ���� �׸�
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;
			if(pWing->m_pArrAreaConc.GetSize() == 0) continue;
			
			CDomyun dom(&domWing);

			// ������ ���� (��ä��� �����Ǵ� ������)
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.

			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);

			// ������
			CTwinVectorArray tvArrGen;
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrGen, FALSE, FALSE, TRUE, TRUE);
			baseDraw.DrawTvArray(&dom, tvArrGen, FALSE, FALSE);

			if(pWing->m_nAttachPos == 0)
				DBStd.m_pDrawVertSection->DrawPartByJijum(&dom, nJ, FALSE);
			///////////////�ڽ����ϋ� ������� ���ʸ�----------------------
			DBStd.m_pDrawVertSection->DrawTogongLine(&dom, pBri, nJ, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE);
			baseBMStd.DrawAreaSectionNumber(&dom, &pWing->m_pArrAreaConc, 1, TRUE);
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0));
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, bStt ? 2 : 0, TRUE, CDPoint(0, 0), FALSE);
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, 3, TRUE, CDPoint(0, 0));

			// ���� 1,2�� ���ؼ��� ���� ġ�� ����(������ L2�� �ִ� ��쿡��)
			if(pWing->m_dL2 > 0)
			{

			}
	
			dom.RedrawByScale(dScale);

			dom.SetCalcExtRect();
			CDRect rectWing	= dom.GetExtRect();

			// ������ ��ġ�� �ű�
			dom.Move(CDPoint(-rectWing.left + (bLeft ? 10 : dWidthOrg+40), dY));
			dom.SetCalcExtRect();

			domWing << dom;
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE, 2);
		}

		// ��ä����� ������ ����
		long nIdx	= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nIdx = 0;
		long i = 0; for(i = 0; i < m_pArrBMStringTogong[nIdx].GetSize(); i++)
		{
			CBMString *pBMString			= m_pArrBMStringTogong[nIdx].GetAt(i);

			if(!pBri->IsBoxType())
			{
				if(pBMString->m_sProcessKind1.Find("��ä��", 0) != -1)
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
						break;
				}
			}
			//Box���϶��� �� �ΰ��� ��(��ä�������, ������)
			else
			{
				if(bStt && pBMString->m_sForm == "(����)")
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}

				if(!bStt && pBMString->m_sForm == "(����)")
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
		}	
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
}

// �Ǹ޿�� = ���ı� - (���ݼ������� ��ü + ���ݼ������� ��ä��)
void COutBMTogong::MakeBMReFill()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);

	long nIdxStt[MAX_JIJUM_COUNT_RC];
	long nIdxEnd[MAX_JIJUM_COUNT_RC];

	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// ���ı� ������ ////////////
		double dTypagi = 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM = pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "���ı�" || pBM->m_sBigClass == "�������ı�" || pBM->m_sBigClass == "������ı�")
				dTypagi	+= pBM->GetResult();
		}

		if(dTypagi > 0)
		{
			CBMData *pBM			= new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_TYPAGI;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;		
			pBM->m_sUnit			= "��";
			pBM->m_sNoteHead		= "�Ǹ޿�� = ���ı� - (���ݼ� ������ ��ü �� ��ä��)";
			pBM->m_sForm.Format("%.3f", dTypagi);
			pBM->m_sFormExcel.Format("#���ı�:%.3f#", dTypagi);
			pBM->SetResult(dTypagi);
			pBM->m_sVCN[Result]		= "�Ǹ޿��";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);
		}

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}
	
	// 1. ���ݼ�(���ı� ��� ����)������ ��ü ����(�� �������� �� �ִ�)
	// ���������� ���� ���� ��ü�� ��ũ��Ʈ ���� ������
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->MakeBMConcreteMain(TRUE);
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];
		
		// �Ǹ޿�� ������ ������ ����
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("�Ǹ޿��", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}
		if(!bReFill) continue;

		// ������ ������ �°� ������
		nIdxEnd[jijum] = pArrBM->GetSize();
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_MAIN;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "�Ǹ޿��";
			pBM->m_bIsPrintByRefForm	= TRUE;
		}	

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}
	
	pOutBM->MakeBMConcreteWing(6);	// ���ı� ���� ������ ��ü ����
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];
		
		// �Ǹ޿�� ������ ������ ����
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("�Ǹ޿��", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}
		if(!bReFill) continue;

		// ������ ������ �°� ������
		nIdxEnd[jijum] = pArrBM->GetSize();
		if(!bReFill) 
		{
			long j = 0; for(j = nIdxStt[jijum]; j < nIdxEnd[jijum]; j++)
			{
				delete pArrBM->GetAt(nIdxStt[jijum]);
				pArrBM->RemoveAt(nIdxStt[jijum]);
			}
			continue;
		}
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_MAIN;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "�Ǹ޿��";
			pBM->m_bIsPrintByRefForm	= TRUE;
		}	

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}

	// 2. ���ݼ�(���ı� ��� ����)������ ��ä�� ����
	// ���������� ���� ���� ��ä�� ���� ������

	// MakeBMBackFill(1); ������ GetTvVertSection_Std�� ȣ���ϹǷ� ������ ����Ÿ�� �ٲ�� ������.
	// ������� �������״ٰ� �ٽ� GetTvVertSection_Std�� ȣ������.
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
	MakeBMBackFill(1);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// �Ǹ޿�� ������ ������ ����
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("�Ǹ޿��", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}

		// ������ ������ �°� ������
		nIdxEnd[jijum]	= pArrBM->GetSize();
		if(!bReFill) 
		{
			long j = 0; for(j = nIdxStt[jijum]; j < nIdxEnd[jijum]; j++)
			{
				delete pArrBM->GetAt(nIdxStt[jijum]);
				pArrBM->RemoveAt(nIdxStt[jijum]);
			}
			continue;
		}
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_BACKFILL;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->m_sVCN[Result]		= "�Ǹ޿��";
			if(pBM->m_nShape != -1)
			{
				pBM->CalculateResult();
				pBM->m_bIsPrintByRefForm	= TRUE;
			}
			else
				pBM->m_bIsPrintByRefForm	= FALSE;
		}	
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
}

// ������
// ���ı� - �Ǹ޿��
void COutBMTogong::MakeBMValuableSoil()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// ���ı�� �Ǹ޿�� ������
		double dTypagi	= 0;
		double dReFill	= 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "���ı�" || pBM->m_sBigClass == "�������ı�" || pBM->m_sBigClass == "������ı�")
				dTypagi	+= pBM->GetResult();
			else if(pBM->m_sBigClass == "�Ǹ޿��")
				dReFill	+= pBM->GetResult();
		}

		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_VALUABLE_SOIL;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nTypeResult		= RESULT_VOLUMN;		
		pBM->m_sUnit			= "��";
		pBM->m_sNoteHead		= "������ = ���ı� - �Ǹ޿��";
		pBM->m_sForm.Format("%.3f - %.3f", dTypagi, dReFill);
		pBM->m_sFormExcel.Format("#���ı�:%.3f# - #�Ǹ޿��:%.3f#", dTypagi, dReFill);
		pBM->SetResult(dTypagi - dReFill);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pArrBM->Add(pBM);
	}
}

// ������
// ���, ��ü
// ���� ��ü�� �ش��ϴ� �κ��� ��ä�� ������ ���ϸ� �ȴ�.
// ����̳� ��ü�� ���ݺ��� �Ʒ��ʿ� �ִٸ� ����̳� ��ü�� ����
void COutBMTogong::MakeBMDeductSoil()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long nIdxStt[2] = {0, 0};
	long nIdxEnd[2] = {0, 0};

	for(long bm = 0; bm < 2; bm++)
	{
		long stt = 0; for(stt =0; stt < 2; stt++)
		{
			BOOL bStt		= stt == iSTT;
			long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		
			if(pBri->IsBoxType()) 
				if(!bStt) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			if(!pBri->IsBoxType())
			{
				nIdxStt[stt]	= pArrBM->GetSize();
				nIdxEnd[stt]	= nIdxStt[stt];
			}
			else
			{
				nIdxStt[0]	= pArrBM->GetSize();
				nIdxEnd[0]	= nIdxStt[0];
			}
		}

		// ���������� ���� ���� ��ä�� ���� ������
		MakeBMBackFill(bm==0 ? 2 : 3);

		// ������ ���� ������������ ����
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt		= stt == iSTT;
			if(pBri->IsBoxType() && !bStt) continue;

			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];
			// ��� / ��ü
			// ���/��ü�ο� �ش��ϴ� ��ä�� ���� �����ͼ� �������� ����
			
			// ������ ������ �°� ������
			nIdxEnd[stt]	= pArrBM->GetSize();
			long i = 0; for(i = nIdxStt[stt]; i < nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_DEDUCT_SOIL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
 				pBM->m_nIdxMidClass		= -1;
 				pBM->m_sMidClass		= _T("");
				pBM->m_nIdxSmallClass	= bm == 0 ? BM_SMALL_DEDUCT_SOIL_NOSANG : BM_SMALL_DEDUCT_SOIL_NOCHE;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass	= -1;
				pBM->m_sItemClass		= _T("");
				if(pBri->IsBoxType())
				{
					pBM->m_nIdxItemClass	= -1;
					pBM->m_sItemClass		= _T("");
				}
				pBM->m_sVCN[Result]		= "������";
			}	
		}
	}
}

// �ռ��� ����
void COutBMTogong::MakeBMApsungto()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long nIdxItemClass	= 0;
	// ��ü 
	// �¿��� �� ���� �Է� �ް� ������, ��ü�� ������ �������� �ռ��並���� �ϰ�
	// ������ ������ ������ �������� �����Ѵ�.
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		//BOX���϶��� �������� ��� �־��ش�.
		BOOL bStt	= stt == iSTT;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		if(!pBri->IsOutWall(bStt)) continue;
		if(pBri->IsBoxType()) nJ = 0;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		double dWidth		= pBri->GetWidthSlabAct(pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE), CDPoint(0, 1), 0);

		// ���� ���� ����
		CalConcAreaSungto(bStt);
		long nSize	= pBri->m_pArrAreaConc.GetSize();

		// ���� ������(��ü)
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);
			
			nIdxItemClass	= bStt ? BM_ITEM_APSUNGTO_MAINS : BM_ITEM_APSUNGTO_MAINE;
			CBMData *pBM			= new CBMData;
			pBM->m_nNumbering		= pArea->m_nNum;
			pBM->m_nIdxBigClass		= BM_BIG_APSUNGTO;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= nIdxItemClass;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;
			pBM->m_sUnit			= "��";
			pBM->m_nShape			= pArea->m_nTypeShape;
			pBM->m_dV[WidthUpper]	= Round(toM(pArea->m_dWidthUpper),3);
			pBM->m_dV[WidthLower]	= Round(toM(pArea->m_dWidthLower),3);
			pBM->m_dV[Height]		= Round(toM(pArea->m_dHeight),3);
			pBM->m_dV[Length]		= Round(toM(dWidth),3);
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "�ռ���";			
			pArrBM->Add(pBM);
	
			// ��ü ����
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				nIdxItemClass	= bStt ? (bLeft ? BM_ITEM_APSUNGTO_WINGSL : BM_ITEM_APSUNGTO_WINGSR) : (bLeft ? BM_ITEM_APSUNGTO_WINGEL : BM_ITEM_APSUNGTO_WINGER);
				double dAng		= pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
				dAng			= bLeft ? 180 - dAng : dAng;

				CBMData *pBMSide			= new CBMData(pBM);
				pBMSide->m_nIdxItemClass	= nIdxItemClass;
				pBMSide->m_sItemClass		= GetStringItemClass(pBMSide->m_nIdxItemClass);
				pBMSide->m_dV[Angle]		= dAng;
				pBMSide->m_dwSide			= SIDE_ROTATE;
				pBMSide->CalculateResult();
				pBMSide->m_sVCN[Result]			= "�ռ���";
				if(!Compare(pBMSide->m_dV[Result], 0.0, _T(">")))
				{
					delete pBMSide;
					pBMSide	= NULL;
					continue;
				}
				pArrBM->Add(pBMSide);
			}
		}
	}
}

// ��ũ��Ʈ ���� ���ϱ�
void COutBMTogong::CalConcAreaSungto(BOOL bStt)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CRebarPlacing rb;
	CTwinVectorArray tvArr;
	CTwinVector tv;
	
	// �ռ���
	AhTPADelete(&pBri->m_pArrAreaConc, (CTwinVectorAreaShape*)0);

	pBri->GetTvSungtoLine(tvArr, bStt, TRUE, FALSE);
	if(tvArr.GetSize() == 0)
	{
		pBri->GetTvSungtoLine(tvArr, bStt, FALSE, FALSE);
		if(tvArr.GetSize() == 0) return;
	}

	// ���� ��������� ��ٰ������� ����
	long nSize	= tvArr.GetSize();
	tv.m_v1	= tvArr.GetAt(nSize-1).m_v2;
	tv.m_v2	= CVector(tvArr.GetAt(0).m_v1.x, tvArr.GetAt(nSize-1).m_v2.y, 0);
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tvArr.GetAt(0).m_v1;
	tvArr.Add(tv);

	// �� �ٰ����� ������ ����
	long nNum	= 1;
	long nIdx	= 0;
	CTwinVectorArray tvArrResult;
	CDoubleArray dArrAngleSu;
	rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
	long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
		for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
			pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

		pArea->Calculate();
		pBri->m_pArrAreaConc.Add(pArea);
	}
}

// �ռ��� ��
void COutBMTogong::SetInsertDomyunApsungto()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	double dWidthOrg	= 350;

	// ��ä�򿡼� ������ ������ �׸�
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(!pBri->IsOutWall(bStt)) continue;

		// �ռ��� ���� ����
		CalConcAreaSungto(bStt);

		// ������ ������ //////////////
		CDomyun domTmp;
		domTmp.SetCWnd(m_pStd->m_pView);
		// ������ ���� (��ä��� �����Ǵ� ������)
		baseBMStd.DrawAreaSectionNumber(&domTmp, &pBri->m_pArrAreaConc);
		baseBMStd.DimAreaSection(&domTmp, &pBri->m_pArrAreaConc, 0, TRUE, CDPoint(0, 0), FALSE);
		baseBMStd.DimAreaSection(&domTmp, &pBri->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0), FALSE);
		// ������ ������ //////////////

		// ��ũ��Ʈ ����� ���� �������� �������°� ���������� ���̴°� �߰�
		CDomyun domTot;
		domTot.SetCWnd(m_pStd->m_pView);

		// ���� ���� ////////////////////
		double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
		baseBMStd.SetDomyunValue(&domTot, dWidthOrg, &domTmp);
		///////////////////////////////////

		CDomyun dom(&domTot);

		// ������ ���� (��ä��� �����Ǵ� ������)
		baseBMStd.DrawAreaSectionNumber(&dom, &pBri->m_pArrAreaConc);
		baseBMStd.DimAreaSection(&dom, &pBri->m_pArrAreaConc, 0, TRUE, CDPoint(0, 0), FALSE);
		baseBMStd.DimAreaSection(&dom, &pBri->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0), FALSE);

		// �𵵸� 250, 400 ������� ����
		dom.RedrawByScale(dScale);
		dom.SetCalcExtRect();
		domTot << dom;

		// �ռ��� ������ ����
		long nIdx	= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nIdx = 0;
		long i = 0; for(i = 0; i < m_pArrBMStringTogong[nIdx].GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringTogong[nIdx].GetAt(i);

			if(!pBri->IsBoxType())
			{
				if(pBMString->m_sProcessKind1.Find("�ռ���", 0) != -1)
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
			//Box���϶��� �� �ΰ��� ��(�ռ��� ��, ���� ��)
			else
			{
				if(bStt && pBMString->m_sForm == "(��ü(����))")
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
				if(!bStt && pBMString->m_sForm == "(��ü(����))")
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
		}	
	}
}

// ��ȣ�� �Ǹ޿��
// ���ı� - ����������(��ȣ�� + ��ȣ������)
void COutBMTogong::MakeBMReFillBoho()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];

		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

		// ������ : 0���� ��õ�� : 1����..
		long nBlockType		= pBri->m_nTypeRcBridgeBlock == 2 ? 0 : 1;
		// �Ҵ� ����� 1.8�϶� 1.8������ ���� �������� 1.5������ ���
		long nGubeType		= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? 1 : 0;
		
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);
		
		// �ʿ��� ����
		long nCountWing	= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;

		CTwinVector tvJigan	= pBri->GetTvJijum(nJ, 0);
		double dAngle		= pBri->m_dAngleRcBridgeBlock;
		double dAngleBoho	= 33.6901;	// ��ȣ���� ���ʿ� �������� ��..(�̰� ���� ����ؾߵ�..)
		double dAngleBohoRad	= dAngleBoho*ConstPi/180;
		double dLenBoho		= toM(tvJigan.GetLength());
		double dMaxH		= m_pStd->m_dBLBaseMaxHeight[nBlockType][nGubeType];
		double dMinH		= m_pStd->m_dBLBaseMinHeight[nBlockType][nGubeType];
		double dMaxW		= m_pStd->m_dBLBaseMaxWidth[nBlockType][nGubeType];
		double dMinW		= m_pStd->m_dBLBaseMinWidth[nBlockType][nGubeType];
		double dTopagi		= 0;
		double dTotalLength	= 0;	// ��ȣ�� �ѱ���
		double dBohoWidth	= 0;	// ��ȣ�� �ʺ�
		double dOutLength	= 0;	// ��ȣ�� �ܰ� ����
		double dT			= 0;	// ��ȣ��	�β�(��ä�� ��ũ��Ʈ ����)
		double dSungtoLength	= 0;	// ������� ����(���ݿ� �ĺ����� ���� ��ȣ�� ����)
		double dLastGube	= pWing->m_dSlopeNoriNext;	// ��ȣ�� ������ ����
		double dBohoLengthInEl	= 0;	// ���� �Ĺ��� ��ȣ�� ����
		double dBohoH		= 0;		// ���ʿ� ������ ��ȣ������ ���� ����..
		double dLenBohoHor	= GetLengthBohoHor(bStt);
		double dLenBohoFooting	= toM(2*ConstPi*dLenBohoHor*dAngle*nCountWing/360) + dLenBoho;
		
		long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "���ı�")
				dTopagi += pBM->GetResult();
		}

		for(i = 0; i < tvArrBoho.GetSize(); i++)
		{
			dTotalLength += toM(tvArrBoho.GetAt(i).GetLength());
			dBohoWidth += toM(tvArrBoho.GetAt(i).GetHorLength());
		}

		dOutLength		= dLenBoho + 2 * ConstPi * dBohoWidth * (dAngle * nCountWing / 360);
		dT				= sqrt(pow((dMaxW-dMinW),2) + pow((dMaxH-dMinH),2));
		dBohoLengthInEl	= sqrt(pow(0.6,2)+pow((0.6*dLastGube),2));
		dSungtoLength	= dTotalLength - dBohoLengthInEl;
		dBohoH			= dT/tan(dAngleBohoRad);

		if(dTopagi > 0)
		{
			CBMData *pBM			= new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_TYPAGI;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;		
			pBM->m_sUnit			= "��";
			pBM->m_sNoteHead		= "�Ǹ޿�� = ���ı� - ����������";
			pBM->m_sForm.Format("%.3f", dTopagi);
			pBM->m_sFormExcel.Format("#��ȣ�����ı�:%.3f#", dTopagi);
			pBM->SetResult(dTopagi);
			pBM->m_sVCN[Result]			= "��ȣ���Ǹ޿��";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);

			// ���ʼ��� ����
			double dH = fabs(m_pStd->m_dBLBaseMaxHeight[nBlockType][nGubeType]-m_pStd->m_dBLBaseMinHeight[nBlockType][nGubeType]);
			CBMData *pBM1 = new CBMData;
			pBM1->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM1->m_sBigClass			= GetStringBigClass(pBM1->m_nIdxBigClass);
			pBM1->m_nLevelTotal			= GetLevelTotal(pBM1->m_nIdxBigClass);
			pBM1->m_nIdxItemClass		= BM_ITEM_REFILL_MAIN;
			pBM1->m_sItemClass			= GetStringItemClass(pBM1->m_nIdxItemClass);
			pBM1->m_sUnit				= "��";
			pBM1->m_nTypeResult			= RESULT_VOLUMN;		
			pBM1->m_sForm.Format("(((%.3f + %.3f) / 2 x %.3f) + (%.3f x %.3f)) x %.3f", 
				dMaxW, dMinW, dH, dMaxW, dMinH, dLenBohoFooting);
			pBM1->SetResult((((dMaxW + dMinW) / 2 * dH) + (dMaxW * dMinH)) * dLenBohoFooting);
			pBM1->m_bIsPrintByRefForm	= TRUE;
			pBM1->m_bIsUseForDeduct		= TRUE;
			pBM1->m_sNoteHead			= "��ȣ�� ���ʰ���";
			pBM1->m_sVCN[Result]		= "��ȣ���Ǹ޿��";
			pArrBM->Add(pBM1);

			// ��ȣ�� ���� ����
			CBMData *pBM2 = new CBMData;
			pBM2->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM2->m_sBigClass			= GetStringBigClass(pBM2->m_nIdxBigClass);
			pBM2->m_nLevelTotal			= GetLevelTotal(pBM2->m_nIdxBigClass);
			pBM2->m_nIdxItemClass		= BM_ITEM_REFILL_MAIN;
			pBM2->m_sItemClass			= GetStringItemClass(pBM2->m_nIdxItemClass);
			pBM2->m_sUnit				= "��";
			pBM2->m_nTypeResult			= RESULT_VOLUMN;
			pBM2->m_sForm.Format("(%.3f x %.3f x %.3f) + (((�� x %.3f�� x %.3f / 360) - (�� x %.3f�� x %.3f / 360)) x %.3f) + (%.3f x %.3f / 2 x %.3f)",
				dT, dBohoLengthInEl, dLenBoho, dTotalLength, dAngle*2, dSungtoLength, dAngle*2, dT, dT, dBohoH, dOutLength);
			pBM2->SetResult((dT * dBohoLengthInEl * dLenBoho) + (((ConstPi * pow(dTotalLength,2) * dAngle*2 / 360) - (ConstPi * pow(dSungtoLength,2) * dAngle*2 / 360)) * dT) + (dT * dBohoH / 2 * dOutLength));
			pBM2->m_bIsUseForDeduct		= TRUE;
			pBM2->m_bIsPrintByRefForm	= TRUE;
			pBM2->m_sNoteHead			= "��ȣ�� ����";
			pBM2->m_sVCN[Result]		= "��ȣ���Ǹ޿��";
			pArrBM->Add(pBM2);		
		}
	}
}

// ��ȣ�� ������
void COutBMTogong::MakeBMValuableSoilBoho()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT;
		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];
		
		// ���ı�� �Ǹ޿�� ������
		double dTypagi	= 0;
		double dReFill	= 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "���ı�")
				dTypagi	+= pBM->GetResult();
			else if(pBM->m_sBigClass == "�Ǹ޿��")
				dReFill	+= pBM->GetResult();
		}

		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_VALUABLE_SOIL;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nTypeResult		= RESULT_VOLUMN;		
		pBM->m_sUnit			= "��";
		pBM->m_sNoteHead		= "������ = ���ı� - �Ǹ޿��";
		pBM->m_sForm.Format("%.3f - %.3f", dTypagi, dReFill);
		pBM->m_sFormExcel.Format("#��ȣ�����ı�:%.3f# - #��ȣ���Ǹ޿��:%.3f#", dTypagi, dReFill);
		pBM->SetResult(dTypagi - dReFill);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pArrBM->Add(pBM);
	}
}


// void COutBMTogong::MakeBMWaterTupagi(CTypedPtrArray <CObArray, CBMData*> *pArr, BOOL bWaterTupagi, double dELWaterTupagi, long nIdxStt, long nIdxEnd)
// {
// 	if(!bWaterTupagi) return;
// 
// 	long i = 0; for(i = nIdxStt; i <= nIdxEnd; i++)
// 	{
// 		CBMData *pBM	= pArr->GetAt(i);
// 		if(pBM->m_nIdxBigClass != BM_BIG_TUPAGI) continue;
// 
// 	
// 		// ������ �������ı⿡ ������ ���ԵǴ� ���
// 		if(pBM->m_dV[SttMeter] <= dELWaterTupagi)
// 		{
// 			pBM->m_nIdxBigClass		= BM_BIG_TUPAGI_WATER;
// 			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
// 			pBM->m_sMidClass		= " " + pBM->m_sMidClass + " ";
// 		}
// 		// ������ �������ı⿡ �ɸ��� ���. �߶� �Ʒ����� �������ı� �������� �����.
// 		else if(pBM->m_dV[SttMeter] > dELWaterTupagi && pBM->m_dV[EndMeter] < dELWaterTupagi)
// 		{
// 			double dHOld	= pBM->m_dV[SttMeter] - pBM->m_dV[EndMeter];
// 			double dHNew	= pBM->m_dV[SttMeter] - dELWaterTupagi;
// 			double dRate	= dHNew / dHOld;
// 			double dDiffNew	= pBM->m_dV[WidthDiffLeft] * dRate;
// 			double dWLNew	= pBM->m_dV[WidthLower] * dRate;
// 			
// 			CBMData *pNew	= new CBMData(pBM);
// 
// 			// ������ (��) ����
// 			pBM->m_dV[WidthLower]		= dWLNew;
// 			pBM->m_dV[WidthDiffLeft]	= dDiffNew;
// 			pBM->m_dV[EndMeter]			= dELWaterTupagi;
// 			pBM->m_dV[Height]			= pBM->m_dV[SttMeter] - pBM->m_dV[EndMeter];
// 			pBM->CalculateResult();
// 			pBM->m_dV[Result]			= pBM->GetResult();
// 
// 			// ���ο��(�Ʒ�) �߰�
// 			pNew->m_nIdxBigClass		= BM_BIG_TUPAGI_WATER;
// 			pNew->m_sBigClass			= GetStringBigClass(pNew->m_nIdxBigClass);
// 			pNew->m_sMidClass			= " " + pNew->m_sMidClass + " ";
// 			pNew->m_dV[WidthUpper]		= pBM->m_dV[WidthLower];
// 			pNew->m_dV[WidthDiffLeft]	= pNew->m_dV[WidthDiffLeft] - dDiffNew;
// 			pNew->m_dV[WidthDiffRight]	= pNew->m_dV[WidthLower] - pNew->m_dV[WidthUpper];
// 			pNew->m_dV[Height]			= pNew->m_dV[SttMeter] - pNew->m_dV[EndMeter];
// 			pNew->CalculateResult();
// 			pNew->m_dV[Result]			= pNew->GetResult();
// 			pArr->Add(pNew);
// 		}
// 	}
// }

//32223 ���ı� ���� ���谡 0�� ��쿡 ���ӵǴ� ���� ������� ���ı⸦ �������ֱ� ���� �Լ�
//tvArr = tvArrVert�� �ʿ� tvArrResultGooBae: ���� �Ǵ� ������ ���� ���ı� dYBase: ���� ���ı� EL  dWaterEL: ���� ���ı� EL
// ���� ���ıⰡ �����ϰ� �ٸ� ����� ���ı� �̸�, ���谡 ��� 0�� ��쿡�� �Ʒ��� ���� �����
// ���ϵǴ� tvArrResultGooBae���� SeperatorTwinVectorArea���� ���� tvArr�� �������ı� EL�� �������� ������ �ٽ� ������
void COutBMTogong::CalcZeroGooBaeExcavation(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrResultGooBae, double dYBase, double dWaterEL)
{
	tvArrResultGooBae.RemoveAll();

	CTwinVector tvBottom, tvLeft, tvRight, tvTop, tvTemp;
	CTwinVector tvFixBottom, tvFixLeft, tvFixRight, tvFixTop;
	CTwinVectorArray tvArrTemp;
	CTwinVectorArray tvArrPolyGon , tvArrPolyGonPrev;

	CRebarPlacing rb;

	long nIdx4 = 0;
	BOOL bFixHeight = FALSE;

	tvArrResultGooBae.AddFromTvArray(tvArr);

	for (long ix =tvArr.GetSize()-1; ix >= 1; ix--)
	{
		tvTemp = tvArr.GetAt(ix);

		if(nIdx4 < 4)
		{
			tvArrPolyGon.Add(tvTemp);
			
			if(nIdx4 == 3)
			{
				long nIdxBottom = 0, nIdxPrev =0, nIdxNext = 0, nIdxTop = 0;
				nIdx4 = 0;

				nIdxBottom	= rb.GetIdxLeftBottomOfTvArr(tvArrPolyGon);

				nIdxPrev	= rb.GetIdxSideOfTvArr(tvArrPolyGon, nIdxBottom, TRUE);
				nIdxNext	= rb.GetIdxSideOfTvArr(tvArrPolyGon, nIdxBottom, FALSE);

				for(long nx=0; nx < 4 ; nx++)
				{
					if(nx == nIdxBottom || nx == nIdxNext || nx == nIdxPrev)
						continue;

					nIdxTop = nx;
				}

				//�ϴ��� ������� ���Ѵ�
				tvBottom = tvArrPolyGon.GetAt(nIdxBottom);
				tvLeft = tvArrPolyGon.GetAt(nIdxNext);
				tvRight = tvArrPolyGon.GetAt(nIdxPrev);
				tvTop = tvArrPolyGon.GetAt(nIdxTop);

				//���ϸ��� ���̰� �����Ͱ� ��� ���� ������ ����
				if(Compare(tvBottom.m_v1.y, dWaterEL , _T("<"),0.001) && Compare(tvTop.m_v1.y, dWaterEL , _T(">"),0.001))
				{
					tvFixBottom.m_v1.x = tvBottom.m_v1.x;
					tvFixBottom.m_v1.y = dWaterEL;
					tvFixBottom.m_v2.x  = tvBottom.m_v2.x;
					tvFixBottom.m_v2.y  = dWaterEL;
					tvFixBottom.m_sInfo = tvBottom.m_sInfo;

					tvFixLeft.m_v1 = tvLeft.m_v1;
					tvFixLeft.m_v2.x = tvLeft.m_v2.x;
					tvFixLeft.m_v2.y = dWaterEL;
					tvFixLeft.m_sInfo = tvLeft.m_sInfo;

					tvFixRight.m_v2 = tvRight.m_v2;
					tvFixRight.m_v1.y = dWaterEL;
					tvFixRight.m_v1.x = tvRight.m_v1.x;
					tvFixRight.m_sInfo = tvRight.m_sInfo;

					tvFixTop = tvTop;
					tvFixTop.m_sInfo = tvTop.m_sInfo;

					bFixHeight = TRUE;

					tvArrPolyGonPrev.Add(tvFixLeft);
					tvArrPolyGonPrev.Add(tvFixTop);
					tvArrPolyGonPrev.Add(tvFixRight);
					tvArrPolyGonPrev.Add(tvBottom);

					//��ģ���� ���� �־��־�� �� �������
					CTwinVectorArray tvArrPush;

					tvArrPush.Add(tvFixBottom);
					tvArrPush.Add(tvFixRight);
					tvArrPush.Add(tvFixTop);
					tvArrPush.Add(tvFixLeft);

					long nIdxPush = 0;
					for (long jx = ix; jx < ix+4; jx++)
					{
						CTwinVector tvPush = tvArrPush.GetAt(nIdxPush);
						tvArrResultGooBae.SetAt(jx, tvPush);

						nIdxPush++;
					}
					tvArrPush.RemoveAll();
				}
				//������ ���� ������ ��쿡 �ٷ� ������ ���� ��쿡�� ����� ���̰� �޶����� ������ ������ �ʿ���
				else if(bFixHeight && tvArrPolyGonPrev.GetSize() > 0)
				{
					tvFixBottom = tvFixBottom;
					tvFixBottom.m_sInfo = tvBottom.m_sInfo;

					tvFixLeft.m_v2 = tvLeft.m_v2;
					tvFixLeft.m_v1.x = tvLeft.m_v1.x;
					tvFixLeft.m_v1.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixLeft.m_sInfo = tvLeft.m_sInfo;
					
					tvFixRight.m_v1 = tvRight.m_v1;
					tvFixRight.m_v2.y =  tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixRight.m_v2.x = tvRight.m_v2.x;
					tvFixRight.m_sInfo = tvRight.m_sInfo;

					tvFixTop.m_v1.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixTop.m_v2.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixTop.m_sInfo = tvTop.m_sInfo;

					bFixHeight = FALSE;
					tvArrPolyGonPrev.RemoveAll();

					//���⵵ ���� �־�� �ϴµ� ��� ���� ����...
					CTwinVectorArray tvArrPush;

					tvArrPush.Add(tvFixBottom);
					tvArrPush.Add(tvFixRight);
					tvArrPush.Add(tvFixTop);
					tvArrPush.Add(tvFixLeft);

					long nIdxPush = 0;
					for (long jx = ix; jx < ix+4; jx++)
					{
						CTwinVector tvPush = tvArrPush.GetAt(nIdxPush);
						tvArrResultGooBae.SetAt(jx, tvPush);

						nIdxPush++;
					}
					tvArrPush.RemoveAll();
				}

				tvArrPolyGon.RemoveAll();
			} // nIdx > 3
			else
				nIdx4++;
		}

	}
}