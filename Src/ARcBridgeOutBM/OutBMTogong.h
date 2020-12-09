// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutBMTogong.h: interface for the COutBMTogong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_)
#define AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BM_BIG_TUPAGI			0	// ���ı�
#define BM_BIG_TUPAGI_WATER		1	// ���� ���ı�
#define BM_BIG_WATEROUT			2	// ��Ǫ��
#define BM_BIG_BACKFILL			3	// ��ä��
#define BM_BIG_REFILL			4	// ��޿��
#define BM_BIG_VALUABLE_SOIL	5	// ������
#define BM_BIG_DEDUCT_SOIL		6	// ������
#define BM_BIG_APSUNGTO			7	// �ռ���

#define BM_MID_TUPAGI_TOSA		0	// ���ı�(���)
#define BM_MID_TUPAGI_PUWHAAM	1	// ���ı�(ǳȭ��)
#define BM_MID_TUPAGI_YUNAM		2	// ���ı�(����)
#define BM_MID_TUPAGI_GYUNGAM	3	// ���ı�(���)

#define BM_SMALL_DEDUCT_SOIL_NOSANG	0	// ������(���)
#define BM_SMALL_DEDUCT_SOIL_NOCHE	1	// ������(��ü)

#define BM_ITEM_BACKFILL			0	// ��ä��
#define BM_ITEM_BACKFILL_APS		1	// ��ä�� ���ӽ����� ����
#define BM_ITEM_BACKFILL_WING		2	// ��ä�� ������ ����
#define BM_ITEM_BACKFILL_BRACKET	3	// ��ä�� ����� ����
#define BM_ITEM_REFILL_TYPAGI		4	// �Ǹ޿�� ���ı�
#define BM_ITEM_REFILL_MAIN			5	// �Ǹ޿�� ���ݼ� ������ ��ü ����
#define BM_ITEM_REFILL_BACKFILL		6	// �Ǹ޿�� ���ݼ� ������ ��ä�� ����
#define BM_ITEM_APSUNGTO_MAINS		7	// �ռ��� ��ü(����)
#define BM_ITEM_APSUNGTO_WINGSL		8	// �ռ��� ������(��������)
#define BM_ITEM_APSUNGTO_WINGSR		9	// �ռ��� ������(��������)
#define BM_ITEM_APSUNGTO_MAINE		10	// �ռ��� ��ü(����)
#define BM_ITEM_APSUNGTO_WINGEL		11	// �ռ��� ������(��������)
#define BM_ITEM_APSUNGTO_WINGER		12	// �ռ��� ������(��������)


class CBMData;
class CBMString;
class CBMSumString;
class CRcBridgeRebar;
class CFootingApp;
#define MAX_JIJUM_COUNT_RC 21
class AFX_EXT_CLASS COutBMTogong : public COutBMBase  
{
public:
	void CalConcAreaSungto(BOOL bStt);	// ���� ���� ���ϱ�
	// �� ��¿� ���� �ɼǵ� /////////////////////////////////////////
	double m_dScaleDim_VertSection;	// ���ܸ鵵 ������
	double m_dHeightText_VertSection;	// ���ܸ鵵 �ؽ�Ʈ ����
	////////////////////////////////////////////////////////////////////

	void DrawFirstPageBoho(CHgBaseBMStd *pBaseBMStd, BOOL bStt);
	long DrawFirstPageTogong(CHgBaseBMStd *pBaseBMStd, long jijum);
	void CalculateBM();
	void ClearBM(BOOL bRemainStdBM=FALSE);
	void MakeBMStringStd();
	void MergeBMStd();
	void SetInsertDomyunStd();
	COutBMTogong(long nBri);
	virtual ~COutBMTogong();

	// ����
	// ������(�����ȵ� ���� ����)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongStd[MAX_JIJUM_COUNT_RC];			// �������(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoStd[2];				// ��ȣ������(���� �ȵȰ�)

	// ��¿�(������ ����)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogong[MAX_JIJUM_COUNT_RC];				// �������(���� �ȵȰ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringTogong[MAX_JIJUM_COUNT_RC];		// �������(���� �ȵȰ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBoho[2];				// ��ȣ������(���� �ȵȰ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBoho[2];		// ��ȣ������(���� �ȵȰ�) - ����ٰ� ���
	

	// ����������ǥ�� ��µ� ����
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongForPartSum[MAX_JIJUM_COUNT_RC];
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoForPartSum[2];	


	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongSum;					// ������� - ������ ����ǥ ��� 
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringTogongSum;
		
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoSum;					// ��ȣ������ - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBohoSum;

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAllSum;			// ����(�������+��ȣ��) - ������ ����ǥ ��� 
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSum;						// ��������(���� �ȵȰ�) - ������ ����ǥ ���

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringSepBridgeSum;	// ������ �и� ���� ����ǥ ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSepBridgeSum;

	CARcBridgeOutBMStd* m_pStd;

	double GetLengthBohoHor(BOOL bStt);
private:
	void MakeBMValuableSoilBoho();
	void MakeBMReFillBoho();
	void SetInsertDomyunApsungto();
	void MakeBMApsungto();
	void MakeBMDeductSoil();
	void MakeBMValuableSoil();
	void MakeBMReFill();
	void SetInsertDomyunBackFill();
	void MakeBMBackFill(long nForUse=4);
	void MakeBMTyopagi();
	void MakeBMWaterOut();
	void MakeBMBohoTyopagi();
	void GetInfoMeter(long nIdxBigClass, double &dFirstMeter, double &dSecondMeter, CFootingApp *pFooting);
	BOOL IsPrintMeter(CBMData *pBM);
	void RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dYBase, double dFirstMeter, double dSecondMeter, long nIdxStt=-1, long nIdxEnd=-1);
//	void MakeBMWaterTupagi(CTypedPtrArray <CObArray, CBMData*> *pArr, BOOL bWaterTupagi, double dELWaterTupagi, long nIdxStt, long nIdxEnd);

	CString GetStringBigClass(long nIdxBigClass);
	CString GetStringMidClass(long nIdxMidClass);
	CString GetStringSmallClass(long nIdxSmallClass);
	CString GetStringItemClass(long nIdxItemClass);
	long GetLevelTotal(long nIdxBigClass);
	
	CRcBridgeRebar* GetRcBridgeRebar(BOOL bOrg = TRUE);
	
	void SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM);

	long m_nBri;

	//32223 ���ı� ���� ���谡 0�� ��쿡 ���ӵǴ� ���� ������� ���ı⸦ �������ֱ� ���� �Լ�
	void CalcZeroGooBaeExcavation(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrResultGooBae, double dYBase, double dWaterEL);
};

#endif // !defined(AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_)
