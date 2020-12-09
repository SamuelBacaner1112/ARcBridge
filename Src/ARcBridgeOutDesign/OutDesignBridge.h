// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutDesignBridge.h: interface for the COutDesignBridge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_)
#define AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Src/ARcBridgeData/ARcBridgeData.h"

class CRcBridgeRebar;
class CARcBridgeCalcStd;
class COutDesignBridge  
{
public:
	void MakeExcel(CXLControl *pXL, long nBri);
	COutDesignBridge();
	virtual ~COutDesignBridge();

	long m_nRowSheet;
	long m_nIndexTitle;

	CARcBridgeOutDesignStd *m_pStd;
	CARcBridgeCalcStd	*m_pCalc;
	
public:
	void MakeExcelTitle(CXLControl *pXL, CRcBridgeRebar *pBri);					//ǥ��
	void MakeExcelCalcFlow(CXLControl *pXL, CRcBridgeRebar *pBri);				//�����帧��
	void MakeExcelCalcIndex(CXLControl *pXL, CRcBridgeRebar *pBri);				//����
	void MakeExcelCondition(CXLControl *pXL, CRcBridgeRebar *pBri);				//��������
	void MakeExcelLsdExposure(CXLControl *pXL, CRcBridgeRebar *pBri);				//������
	void MakeExcelSection(CXLControl *pXL, CRcBridgeRebar *pBri);				//�ܸ鰡��
	void MakeExcelCalcLoad(CXLControl *pXL, CRcBridgeRebar *pBri);				//���߻���
	void MakeExcelCalcLoad_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);				//���߻���
	void MakeExcelLoadDiagram(CXLControl *pXL, CRcBridgeRebar *pBri);			//��������
	void MakeExcelModeling(CXLControl *pXL, CRcBridgeRebar *pBri);				//�𵨸��ؼ�
	void MakeExcelCombine(CXLControl *pXL, CRcBridgeRebar *pBri);				//��������
	void MakeExcelForceSummarySheet(CXLControl *pXL, CRcBridgeRebar *pBri);		//�ܸ�� ����
	void MakeExcelForceSummarySheet_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);		//�ܸ�� ����
	void MakeExcelSectionDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//�ܸ����
	void MakeExcelUseableDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//��뼺����
	void MakeExcelSluggishAndFatigue(CXLControl *pXL, CRcBridgeRebar *pBri);	//ó�� �� �Ƿΰ���
	void MakeExcelCornerAndSupportBar(CXLControl *pXL, CRcBridgeRebar *pBri);	//�찢�� �� ���ö�� ����
	void MakeExcelColumnAndGirder(CXLControl *pXL, CRcBridgeRebar *pBri);		//��,��� ����
	void MakeExcelArchRib(CXLControl *pXL, CRcBridgeRebar *pBri);				//Arch ���� ����
	void MakeExcelFootingSafeCheck(CXLControl *pXL, CRcBridgeRebar *pBri);		//���ʾ�������
	void MakeExcelFootingDesign(CXLControl *pXL, CRcBridgeRebar *pBri);			//���ʼ���
	void MakeExcelWingWall(CXLControl *pXL, CRcBridgeRebar *pBri);				//���ʼ���
	void MakeExcelApproachSlab(CXLControl *pXL, CRcBridgeRebar *pBri);			//���ӽ����꼳��
	void MakeExcelExpJoint(CXLControl *pXL, CRcBridgeRebar *pBri);				//�������� ����
	void MakeExcelCantilever(CXLControl *pXL, CRcBridgeRebar *pBri, HGBOOL bLeft);	// ĵƿ���� ����
	void MakeExcelPile(CXLControl *pXL, CRcBridgeRebar *pBri, long nIndex);		//���Ұ���
//	void MakeExcelSubsidence(CXLControl *pXL, CRcBridgeRebar *pBri);			//�ε�ħ�ϰ���
	void MakeExcelCalcResultSummary(CXLControl *pXL, CRcBridgeRebar *pBri);		//�ؼ���� ���
	void MakeExcelCalcShoeReaction(CXLControl *pXL, CRcBridgeRebar *pBri);		//������ħ �ݷ»���
	void MakeExcelSTM(CXLControl *pXL, CRcBridgeRebar *pBri, HGINT32 nItem, HGINT32 iKey, HGINT32 nIndexMiddle);			//STM

	void MakeExcelCombine_Lsd(CXLControl *pXL, CRcBridgeRebar *pBri);				//��������

	void WriteExcelEarthQuakeLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long &nIndexMiddle);
	void MakeExcelSTM_Condition(CXLFormatText *pXLText, CRcBridgeRebar *pBri, CStmModel*pModel, long nColPos, long nIndexTitle, HGINT32 nItem, HGINT32 iKey);

	//�찢��
	void MakeExcelSTM_ArcCorner_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_ArcCorner_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	//STM �찢�� �ܸ��� �������� ���� �Լ�
	void GetTvArrSTM_Corner_Section(CTwinVectorArray &tvArrCorner, CTwinVectorArray &tvArrDimTopBot , CTwinVectorArray &tvArrDimLeftRight, CRcBridgeRebar *pBri, HGINT32 nPos );

	//�귡Ŷ
	void MakeExcelSTM_Bracket_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_Bracket_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);

	//���� StartEnd
	void MakeExcelSTM_Footing_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
	void MakeExcelSTM_Footing_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
//	void MakeExcelSTM_Footing_Combine(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexTitle, long nIndexMiddle);

	//���� Mid
	void MakeExcelSTM_FootingMid_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey);
//	void MakeExcelSTM_FootingMid_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexTitle, long nIndexMiddle, HGINT32 iKey);

private:
	void MakeTableBoringData(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeDiagramBMD_SFD(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeDiagramBMD_SFD_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableMainRebarSummaryCrack(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableMainRebarSummaryCrackLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	// 0: �������, 1: �������, 2: �������(�ִ����), 3: ��������
	void MakeTableLoadComBo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType, BOOL bIncreaseCombo=FALSE);
	void MakeTableMainRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableShearRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableShearRebarSummaryLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	//
	void MakeExcelCalcShoeReaction_2DFrame(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeReaction_3DPlate(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeReaction_Apply(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeExcelCalcShoeHoriForce(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableSectionForce(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableSectionForceEQ(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableSectionForceEQ_BOX(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bMoment, ETypeEnvComboLsd eEnvTypeLsd=eEnvLimit);
	void MakeTableLoadForFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	CString GetStringSectionForceTableAtWall(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd, BOOL b3D, BOOL b3DLoad);
	CString GetStringSectionForceTableAtWallEQ(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd);
	CString GetStringSectionForceTableAtWallEQ_BOX(CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, BOOL bMoment, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd);

	void GetKeySttEnd(HGINT32 nItem, HGINT32 &iKeyStt, HGINT32 &iKeyEnd);

	void MakeExcelConditionUseMaterial(CXLFormatText *pXLText, long &nIndexSmall, long &nIndexTiny, long &nTypeNumber, long nT_1, long nT_2);
	void MakeExcelConditionUseMaterialLsd(CXLFormatText *pXLText, CRcBridgeRebar *pBri);
};

#endif // !defined(AFX_OUTDESIGNBRIDGE_H__8365FEFA_E7CB_4FBF_8019_5063DD41FFF0__INCLUDED_)