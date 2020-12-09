// OutBridgeResister.cpp: implementation of the COutBridgeResister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeOutBM/arcbridgeoutbm.h"
#include "ARcbridgeOutDesign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutBridgeResister::COutBridgeResister()
{
	m_nIndexTitle = -1;
	m_pStd = NULL;
	m_pCalc = NULL;
	m_nRowSheet = -1;
}

COutBridgeResister::~COutBridgeResister()
{

}

void COutBridgeResister::MakeExcel(CXLControl *pXL, long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar  *pBri  = pData->GetBridgeRebar(nBri);
	CProgressCtrl *pProgress = &m_pStd->m_pDlg->m_ctlProgress;

	CXLFormatText::SetExcelFileName("HgExcelBase.dll");
	CXLFormatText::SetRowPerPage(pData->m_nPageRow);

	if(pBri->m_nSeparBri == 2) return;

	CString sSheetName = _T("");
	long nSheetSu	 = pXL->GetSheetsu();
	long nSheetTotal = pData->GetBridgeSize(TRUE);

	long i=0; for(i=0; i<nSheetTotal-nSheetSu; i++)	pXL->AddSheet();

	long nTotPos   = m_pStd->m_pDlg->m_nPos;
	long nCurStep  = 0;
	long nTotStep  = nSheetTotal;
	long nCurSheet = 1;	
	CString szMsg;	

	//////////////////////////////////////////////////////////////////////////
	CString sNameSheet = _T("");
	sNameSheet.Format("�������");
	// �������
	pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
	szMsg.Format("[ %s ] ������� �����...", pBri->m_strBridgeName);
	m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
	pXL->SetActiveSheet(nCurSheet++);
	m_pStd->SetExcelFormat(pXL, XL_ROWSTT_DESIGN, XL_ROWEND, 25, XFL_TEXTSIZE, TRUE);
	pXL->SetSheetName(sNameSheet);
	CXLFormatText pXLText(pXL);
	pXLText(1);
	MakeExcelResister(&pXLText, nBri);
	MakeExcelResister2(&pXLText, nBri);

	MakeExcelBridge(&pXLText, nBri);

	// ������ �� ���� => �ݵ�� �־�� ��
	CXLFormatText::DeleteAddressMap();
}


void COutBridgeResister::MakeExcelResister(CXLFormatText *pXLText, long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pData->GetBridgeRebar(nBri);
	CRcBridgeRebar *pBri_1;
	if(pBri->m_nSeparBri == 1) pBri_1 = pData->GetBridgeRebar(nBri+1);
	
	pXLText->AddFormatText("$m+11$tb15$c%s �������$n$h", pBri->m_strBridgeName);
	pXLText->AddFormatText("$n$h");

	pXLText->AddTable(24 , 24, 0, 0);
	//////////////////////////////////////////////////////////////////////////
	double dWidthSlab = 0;
	double dLenBri = 0;
	CDPoint vAngle = pBri->GetAngleJijum(0);
	
	long nWidth = 0;
	CString str = _T("");
	CString strTemp = _T("");
	nWidth = 3;
	pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", "�� �� ��");
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str.Format("%s", pData->m_strSpotName);
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 15;
	str = pData->m_strProjectName;
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h");

	nWidth = 3;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = pBri->m_strBridgeName;
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�뿪ȸ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str = pData->m_strEngName;
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("����å����");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h");

	nWidth = 1;
	str = _T("��\n��\n��\n��\n\n��\n\nȾ\n��\n��\n��");
	pXLText->AddMergeFormatText(10, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 24;
	str = _T(" ���� ");
	pXLText->AddMergeFormatText(10, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	*pXLText += 11;
	pXLText->AddFormatText("$h");

	nWidth = 1;
	str = _T("��\n\n��\n\n��\n\n��");
	pXLText->AddMergeFormatText(11, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);

	nWidth = 4;
	str = _T("��������");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	if(pData->m_nDegreeBridge == 0) str = _T("DB-24, DL-24");
	else if(pData->m_nDegreeBridge == 1) str = _T("DB-18, DL-18");
	else if(pData->m_nDegreeBridge == 2) str = _T("DB-13.5, DL-13.5");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("�������");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;

	str.Format("%s ��ޱ�, A =  %.3f", (pBri->GetDegreeEarthQuake() ==0)? "I":"II", pBri->GetCoefficientAccelerateEQ());
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+13");
	nWidth = 4;
	str = _T("���ݰ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str.Format("S = %.2f", pBri->GetCoefficientGround());
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");

	nWidth = 4;
	str = _T("��    ġ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 20;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");
	
	nWidth = 4;
	str = _T("��    ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	if(pBri->m_nSeparBri == 0){
		str.Format("%.2f m", toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1))));
		dWidthSlab = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1)));
	}
	if(pBri->m_nSeparBri == 1){
		str.Format("%s : %.2f m\n%s : %.2f m", pData->m_strLineDirName, toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1))), pData->m_strLineRevDirName, toM(pBri_1->GetWidthSlabAct(pBri_1->m_dStationBridgeStt, CDPoint(0,1))));
		dWidthSlab = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1)))+toM(pBri_1->GetWidthSlabAct(pBri_1->m_dStationBridgeStt, CDPoint(0,1)));
	}
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("��������");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
//	double dLen = 0;
	str = GetStringJigan(nBri);
	long i=0; for(i=0; i<pBri->m_nQtyJigan; i++)
	{
		double dLen = toM(pBri->m_dLengthJigan[i]);
		dLenBri += dLen;
	}
	nWidth = 8;
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");

	nWidth = 4;
	str = _T("��    ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str.Format("%.1f��", vAngle.GetAngleDegree());
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("��������");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str.Format("%.2f x %.2f = %.2f ��", dWidthSlab, dLenBri, dWidthSlab*dLenBri);
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");

	nWidth = 4;
	str = _T("��������");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str = _T("�� �� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 12;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h$m+05");
	nWidth = 8;
	str = _T("�� �� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 12;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");

	nWidth = 4;
	str = _T("��������");
	pXLText->AddMergeFormatText(4, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 8;
	str = _T("�� �� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("��ȹ����");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("������ �� ���ϰ�");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+05");
	nWidth = 2;
	str = _T("��õ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+05");

	nWidth = 2;
	str = _T("����");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+05");

	nWidth = 2;
	str = _T("ö��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+05");

	nWidth = 2;
	str = _T("��Ÿ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h");
}

void COutBridgeResister::MakeExcelResister2(CXLFormatText *pXLText, long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pData->GetBridgeRebar(nBri);
	CRcBridgeRebar *pBri_1;
	if(pBri->m_nSeparBri == 1) pBri_1 = pData->GetBridgeRebar(nBri+1);

	pXLText->AddFormatText("$n$h");
	pXLText->AddTable(25 , 24, 0, 0);

	//////////////////////////////////////////////////////////////////////////
//	CString str = _T("");
	long nWidth = 0;
	nWidth = 1;
	CString str = _T("��\n\n��\n\n��\n\n��");
	pXLText->AddMergeFormatText(6, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);

	nWidth = 3;
	str = _T("��  ��\n��\n��  ��");
	pXLText->AddMergeFormatText(3, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 9;
	str = _T("��   ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("�� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+10");
	nWidth = 3;
	str = _T("��  ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�˻���");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�˻���");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("��  ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+04");
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$n$h$m+01");
	// Ÿ�缺���� �� �⺻����
	nWidth = 3;
	str = _T("Ÿ�缺����\n��\n�⺻����");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("��  ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("�뿪ȸ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("��������");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("��  ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+04");
	nWidth = 6;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 5;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h");

	// ��ΰ���
	nWidth = 1;
	str = _T("��\n��\n��\n��");
	pXLText->AddMergeFormatText(3, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("��  ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 7;
	str = GetStringBridgeType(pBri);
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("�� �� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 10;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");
	nWidth = 3;
	str = _T("������");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 7;
	str = _T("");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 4;
	str = _T("����������ġ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 10;
	str = _T("����������ġ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");
	nWidth = 3;
	str = _T("��������");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	if(pBri->m_nRoadMaterial==0) str.Format("�ƽ���\nT=%.0fmm", pBri->m_dTP);
	else str.Format("LMC����\nT=%.0fmm", pBri->m_dTP);
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("��ũ\n��Ʈ");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str.Format("Fck=%.1f\n%s", pData->GetValueUnitType(pData->m_Fck, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), pData->m_szKgf_CM2);
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("ö��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str.Format("SD %.0f", pData->m_Fy/100);
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("P.S\n����");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("����");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 2;
	str = _T("");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$n$h");

	// �Ϻα���
	nWidth = 1;
	str = _T("��\n\n��\n\n��\n\n��");
	pXLText->AddMergeFormatText(15, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 15;
	str = _T("�� �� �� �� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+%d", nWidth);
	nWidth = 9;
	str = _T("��   ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");
	nWidth = 3;
	str = _T("�� ��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� ��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("��ճ���\n(M)");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 6;
	str = _T("������ħ");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� ��");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("���ʱ���\n(M)");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("��������\n(M)");
	pXLText->AddMergeFormatText(1, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+10");
	nWidth = 3;
	str = _T("�� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	nWidth = 3;
	str = _T("�� ��");
	pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
	pXLText->AddFormatText("$m+0%d", nWidth);
	pXLText->AddFormatText("$n$h$m+01");

	long nFoot = pBri->GetCountFooting();
	if(pBri->m_nSeparBri == 1) nFoot += pBri_1->GetCountFooting();

	long i=0; for(i=0; i<nFoot; i++)
	{
		long nType = 0;
		long nJijum = 0;
		CFootingApp *pFoot;
		if(pBri->m_nSeparBri==1 && i>=pBri->GetCountFooting())
		{
			nJijum = i-pBri->GetCountFooting();
			pFoot = pBri_1->GetFooting(nJijum);
			nType = 2;
		}
		else 
		{
			nJijum = i;
			pFoot = pBri->GetFooting(nJijum);
			nType = (pBri->m_nSeparBri == 1)? 1:0;
		}

		if(pFoot == NULL) continue;

		long j=0; for(j=0; j<8;j++)
		{
			nWidth = 3;
			if(j==0)
			{
				if(nType==0 || nType==1) str = GetStringFootName(pBri, pFoot);
				else if(nType==2) str = GetStringFootName(pBri_1, pFoot);
			}
			else if(j==5)
			{
				if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN) str = "��������";
				else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) str = "MASS ����";
				else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) str = "���ұ���";
			}
			else if(j==6 && (pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE))
			{
				str.Format("$f3%.3f", toM(pFoot->m_exFooting.m_Pile.m_dLength));
			}
			else if(j==7)
			{
				CConcSymbol sym;
				CBoringDataEx *pBorEx = pBri->GetBoringDataExByJijumNumber(nJijum);
				
				CBoringData *pBoring;
				if(pFoot->m_exFooting.m_nType ==EXFOOTING_TYPE_PILE)
				{
					double dEl = toM(pFoot->m_exFooting.m_dEL - pFoot->m_exFooting.m_Pile.m_dLength);
					pBoring = pBorEx->GetBoringByElevation(dEl);
				}
				else
				{
					pBoring = pBri->GetBoringDataJijumBase(nJijum);
				}
							
				str = sym.m_strSoilName[pBoring->m_nNameSoil];
			}
			else str = _T("");
			pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
			pXLText->AddFormatText("$m+0%d", nWidth);
		}
		pXLText->AddFormatText("$n$h$m+01");
	}

	long nRow = 13-nFoot;
	for(i=0; i<nRow; i++)
	{
		long j=0; for(j=0; j<8;j++)
		{
			nWidth = 3;
			str = _T("");
			pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
			pXLText->AddFormatText("$m+0%d", nWidth);
		}
		pXLText->AddFormatText("$n$h$m+01");
	}
	pXLText->AddFormatText("$h");
}

void COutBridgeResister::MakeExcelBridge(CXLFormatText *pXLText, long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pData->GetBridgeRebar(nBri);
	long nCntBri = (pBri->m_nSeparBri == 1)? 2:1;

	long i=0; for(i=0; i<nCntBri; i++)
	{
		if(i!=0) pBri = pData->GetBridgeRebar(nBri+1);

		pBri->SetConcData(TRUE,TRUE,TRUE,TRUE,TRUE);
		m_pCalc = new CARcBridgeCalcStd(pBri);
		m_pCalc->m_pARoadOptionStd = m_pStd->m_pARoadOptionStd;
		m_pCalc->m_pView		   = m_pStd->m_pView;
		m_pCalc->CalculateAll();

		if(nCntBri == 1) pXLText->AddFormatText("$m+11$tb15$c%s$n$h", pBri->m_strBridgeName);
		else if((nCntBri == 2 && i == 0)) pXLText->AddFormatText("$m+11$tb15$c%s(%s)$n$h", pBri->m_strBridgeName, pData->m_strLineDirName);
		else if((nCntBri == 2 && i == 1)) pXLText->AddFormatText("$m+11$tb15$c%s(%s)$n$h", pBri->m_strBridgeName, pData->m_strLineRevDirName);
		pXLText->AddFormatText("$n$h");
		
		pXLText->AddTable(24 , 24, 0, 0);
		//////////////////////////////////////////////////////////////////////////
		long nWidth = 0;
// 		CString str = _T("");
		nWidth = 3;
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", "�� �� ��");
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 14;
		CString str = pData->m_strProjectName;
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 3;
		str = _T("�� �� ��");
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = pBri->m_strBridgeName;
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h");
		
		nWidth = 3;
		str = _T("��������");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 14;
		str = GetStringBridgeType(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 3;
		str = _T("��������");
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringFootTypeAll(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1, "$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h");

		nWidth = 1;
		str = _T("��\n\n��\n\n��");
		pXLText->AddMergeFormatText(9, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 14;
		str = _T("�ܸ鵵");
		pXLText->AddMergeFormatText(9, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 1;
		str = _T("��\n��\n��\n��");
		pXLText->AddMergeFormatText(9, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("��������");
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 6;
		str.Format("W = %.1f %s", pData->GetValueUnitType(pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2), pData->m_szTonf_M2);
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$n$h$m+16");

		nWidth = 3;
		str = _T("�� ä ��\n��    ��");
		pXLText->AddMergeFormatText(3, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("�����߷�");
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str.Format("%.1f %s", pData->GetValueUnitType(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3), pData->m_szTonf_M3);
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$n$h$m+19");
		nWidth = 3;
		str = _T("���θ�����");
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str.Format("�� = %.1f��", pBri->m_dAngleFrictionBackFill);
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$n$h$m+16");

		nWidth = 3;
		str = _T("��а��");
		pXLText->AddMergeFormatText(3, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("��������");
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;

		str.Format("Ka = %.3f", m_pCalc->m_dCoefficientKo);
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$n$h$m+19");
		nWidth = 3;
		str = _T("�ܸ�����");
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str.Format("Ka = %.3f", m_pCalc->m_dCoefficientKo);
		pXLText->AddMergeFormatText(1, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$n$h");

		// ��������
		nWidth = 1;
		str = _T("��\n��\n��\n��");
		pXLText->AddMergeFormatText(3, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 4;
		str = _T("�� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str = _T("PILE ����");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 10;
		str = _T("PILE ������");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");
		nWidth = 4;
		str = _T("-");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("�ִ�����");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("�������");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("�ִ뿬����");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("��뿬����");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");
		nWidth = 4;
		str = _T("-");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str = pData->m_szKgf_CM2;
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 10;
		str.Format("%s/��", pData->m_szTonf);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");
		// ��� ��..
		nWidth = 4;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h");

		// �ܸ����
		nWidth = 1;
		str = _T("��\n��\n��\n��");
		pXLText->AddMergeFormatText(4, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 4;
		str = _T("�� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("�������߾Ӻ�");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("��ü��ܺ�");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("��ü�߾Ӻ�");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("�� �� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// Moment
		nWidth = 4;
		str = _T("Mu, ��Mn");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxMomentSlabCenter(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxMomentWall(pBri, TRUE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxMomentWall(pBri, FALSE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxMomentFoot(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// As
		nWidth = 4;
		str = _T("As");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxAsSlabCenter(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxAsWall(pBri, TRUE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxAsWall(pBri, FALSE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxAsFoot(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// Vert
		nWidth = 4;
		str = _T("Vuf, ��Vn");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxVuSlabCenter(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxVuWall(pBri, TRUE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxVuWall(pBri, FALSE);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = GetStringMaxVuFoot(pBri);
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// As
		nWidth = 4;
		str = _T("As");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 5;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h");

		// ������
		nWidth = 1;
		str = _T("��\n��\n��\n��");
		pXLText->AddMergeFormatText(3, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 4;
		str = _T("�� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("�� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str = _T("�� ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 7;
		str = _T("��  ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");
		// -��ũ��Ʈ
		nWidth = 4;
		str = _T("��ũ��Ʈ");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str.Format("$f3%.3f", GetCalBM(nBri+i, BM_BIG_CONCRETE));
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 7;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// -ö��
		nWidth = 4;
		str = _T("ö    ��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = "Tonf";
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str.Format("$f3%.3f", GetCalBM(nBri+i, BM_BIG_REBAR));
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 7;
		str = _T("NET");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h$m+01");

		// -PILE
		nWidth = 4;
		str = _T("PILE");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 3;
		str = _T("��");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		nWidth = 10;
		str.Format("%.0f", GetCalBM(nBri+i, BM_BIG_PILE));
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+%d", nWidth);
		nWidth = 7;
		str = _T("");
		pXLText->AddMergeFormatText(0, nWidth-1,"$c%s", str);
		pXLText->AddFormatText("$m+0%d", nWidth);
		pXLText->AddFormatText("$n$h");

		delete m_pCalc;
	}
}

CString COutBridgeResister::GetStringJigan(long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pData->GetBridgeRebar(nBri);

// 	double dLen = 0;
	double dLenBri = 0;
	CString str = _T("");
	CString strTemp = _T("");

	CDPoint xyP(0, 0);
	CDPointArray xyArrJigan;

	long i=0; for(i=0; i<pBri->m_nQtyJigan; i++)
	{
		double dLen = toM(pBri->m_dLengthJigan[i]);
		dLenBri += dLen;
		
		long nCntArr = xyArrJigan.GetSize();
		long j=0; for(j=0; j<nCntArr; j++)
		{
			CDPoint xyComp = xyArrJigan.GetAt(j);
			
			if(dLen == xyComp.x)
			{
				xyP.x = dLen;
				xyP.y = xyComp.y+1;

				xyArrJigan.SetAt(j, xyP);
				break;
			}
			else if(j==nCntArr-1)
			{
				xyP.x = dLen;
				xyP.y = 1;

				xyArrJigan.Add(xyP);
			}
		}
		if(nCntArr == 0)
		{
			xyP.x = dLen;
			xyP.y = 1;

			xyArrJigan.Add(xyP);
		}
	}

	long nCntArr = xyArrJigan.GetSize();
	for(i=0; i<nCntArr; i++)
	{
		xyP = xyArrJigan.GetAt(i);
		if(!(i%2) && i!=0) str += "\n+ ";
		else if(i%2 && i!=0) str += " + ";

		strTemp.Format("%.0f @ %.2f", xyP.y, xyP.x);
		str += strTemp;
	}
	
	strTemp.Format(" = %.2f M", dLenBri);
	str += strTemp;

	return str;
}

CString COutBridgeResister::GetStringBridgeType(CRcBridgeRebar *pBri)
{
	CString szBriType = _T("");

	if(pBri->m_bUnderGroundRahmen) szBriType = _T("���� RAHMEN");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_PORTAL) szBriType = _T("���� RAHMEN");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_PI) szBriType = _T("���� RAHMEN");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_V) szBriType = _T("V�� RAHMEN");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP) szBriType = _T("��� ���� RAHMEN");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB) szBriType = _T("�����걳");
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX) szBriType = _T("�ڽ���");

	return szBriType;
}

CString COutBridgeResister::GetStringFootName(CRcBridgeRebar *pBri, CFootingApp *pFoot)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

// 	CFootingApp *pFoot1;
	long nIndexJijum = 0;
	CString szFootName = _T("");

	long i=0; for(i=0; i<pBri->GetCountJijum(); i++)
	{
		CFootingApp *pFoot1 = pBri->GetFootingByJijumNumber(i);
		if(pFoot == pFoot1) nIndexJijum = i;
	}

	// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)
	if(pBri->m_nSeparBri == 0)
	{
		if(nIndexJijum==0) szFootName.Format("�� ��");
		else if(nIndexJijum==pBri->GetCountJijum()-1) szFootName.Format("�� ��");
		else szFootName.Format("���� %d", nIndexJijum+1);
	}
	else if(pBri->m_nSeparBri == 1)
	{
		if(nIndexJijum==0) szFootName.Format("%s\n�� ��", pData->m_strLineDirName);
		else if(nIndexJijum==pBri->GetCountJijum()-1) szFootName.Format("%s����\n�� ��", pData->m_strLineDirName);
		else szFootName.Format("%s����\n���� %d", pData->m_strLineDirName, nIndexJijum+1);
	}
	else if(pBri->m_nSeparBri == 2)
	{
		if(nIndexJijum==0) szFootName.Format("%s\n�� ��", pData->m_strLineRevDirName);
		else if(nIndexJijum==pBri->GetCountJijum()-1) szFootName.Format("%s����\n�� ��", pData->m_strLineRevDirName);
		else szFootName.Format("%s����\n���� %d", pData->m_strLineRevDirName, nIndexJijum+1);
	}

	return szFootName;
}

CString COutBridgeResister::GetStringFootTypeAll(CRcBridgeRebar *pBri)
{
	CString szFootType = _T("");

	long nKind		= 0;
	BOOL bDirect	= FALSE;
	BOOL bMass		= FALSE;
	BOOL bPile		= FALSE;

	long nCnt = pBri->GetCountFooting();
	long i=0; for(i=0; i<nCnt; i++)
	{
		CFootingApp *pFoot = pBri->GetFooting(i);
		if(pFoot)
		{
			if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN) bDirect = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) bMass = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) bPile = TRUE;
		}
	}

	if(bDirect)
	{
		if(nKind == 0) szFootType += "��������";
//		else if(nKind%2) szFootType += ", ��������";
		else szFootType += ", ��������";
		nKind++;
	}
	if(bMass)
	{
		if(nKind == 0) szFootType += "MASS����";
//		else if(nKind%2) szFootType += ", MASS����";
		else szFootType += ", MASS����";
		nKind++;
	}
	if(bPile)
	{
		if(nKind == 0) szFootType += "���ұ���";
//		else if(nKind%2) szFootType += ", ���ұ���";
		else szFootType += ", ���ұ���";
		nKind++;
	}
	
	return szFootType;
}

CString COutBridgeResister::GetStringMaxMomentSlabCenter(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum()-1;
	double Mu = 0;
	double PiMn = 0;
	double MaxMu = 0;
	double MaxPiMn = 0;
	double SFmin = 0;
	double SFtem = 0;

	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataSlab(i, CHECK_POS_CENTER);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);

		Mu = pBeam.GetMu();
		PiMn = pBeam.GetPiMn();
		SFtem = PiMn/Mu;
		
		if(i==0)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
		else if(SFmin>SFtem)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
	}

	if(SFmin > 0) szRet.Format("Mu = %.3f\n< ��Mn = %.3f", MaxMu, MaxPiMn);
	else szRet.Format("Mu = %.3f\n> ��Mn = %.3f", MaxMu, MaxPiMn);

	return szRet;
}

CString COutBridgeResister::GetStringMaxMomentWall(CRcBridgeRebar *pBri, BOOL bUpper)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum();
	double Mu = 0;
	double PiMn = 0;
	double MaxMu = 0;
	double MaxPiMn = 0;
	double SFmin = 0;
	double SFtem = 0;

	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataWall(i, (bUpper)? CHECK_POS_TOP:CHECK_POS_CENTER, (bUpper)? TRUE:FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Mu = pBeam.GetMu();
		PiMn = pBeam.GetPiMn();
		SFtem = PiMn/Mu;
		
		if(i==0)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
		else if(SFmin>SFtem)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
	}
	
	if(SFmin > 0) szRet.Format("Mu = %.3f\n< ��Mn = %.3f", MaxMu, MaxPiMn);
	else szRet.Format("Mu = %.3f\n> ��Mn = %.3f", MaxMu, MaxPiMn);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxMomentFoot(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum();
	double Mu = 0;
	double PiMn = 0;
	double MaxMu = 0;
	double MaxPiMn = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, TRUE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Mu = pBeam.GetMu();
		PiMn = pBeam.GetPiMn();
		SFtem = PiMn/Mu;
		
		if(i==0)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
		else if(SFmin>SFtem)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
	}

	for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Mu = pBeam.GetMu();
		PiMn = pBeam.GetPiMn();
		SFtem = PiMn/Mu;
		
		if(i==0)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
		else if(SFmin>SFtem)
		{
			MaxMu = Mu;
			MaxPiMn = PiMn;
			SFmin = MaxPiMn/MaxMu;
		}
	}
	
	if(SFmin > 0) szRet.Format("Mu = %.3f\n< ��Mn = %.3f", MaxMu, MaxPiMn);
	else szRet.Format("Mu = %.3f\n> ��Mn = %.3f", MaxMu, MaxPiMn);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxAsSlabCenter(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum()-1;
	double AsUse = 0;
	double AsReq = 0;
	double MaxAsUse = 0;
	double MaxAsReq = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataSlab(i, CHECK_POS_CENTER);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		AsUse = pBeam.GetRebarAreaUse();
		AsReq = pBeam.GetRebarAreaRequired();
		SFtem = AsUse/AsReq;
		
		if(i==0)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
		else if(SFmin>SFtem)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
	}
	
	if(SFmin > 0) szRet.Format("req As=%.3f\n< use As=%.3f", MaxAsReq, MaxAsUse);
	else szRet.Format("Mu = %.3f\n> ��Mn = %.3f", MaxAsReq, MaxAsUse);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxAsWall(CRcBridgeRebar *pBri, BOOL bUpper)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum();
	double AsUse = 0;
	double AsReq = 0;
	double MaxAsUse = 0;
	double MaxAsReq = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataWall(i, (bUpper)? CHECK_POS_TOP:CHECK_POS_CENTER, (bUpper)? TRUE:FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		AsUse = pBeam.GetRebarAreaUse();
		AsReq = pBeam.GetRebarAreaRequired();
		SFtem = AsUse/AsReq;
		
		if(i==0)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
		else if(SFmin>SFtem)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
	}
	
	if(SFmin > 0) szRet.Format("req As=%.3f\n< use As=%.3f", MaxAsReq, MaxAsUse);
	else szRet.Format("req As=%.3f\n> use As=%.3f", MaxAsReq, MaxAsUse);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxAsFoot(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum();
	double AsUse = 0;
	double AsReq = 0;
	double MaxAsUse = 0;
	double MaxAsReq = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, TRUE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		AsUse = pBeam.GetRebarAreaUse();
		AsReq = pBeam.GetRebarAreaRequired();
		SFtem = AsUse/AsReq;
		
		if(i==0)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
		else if(SFmin>SFtem)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
	}

	for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		AsUse = pBeam.GetRebarAreaUse();
		AsReq = pBeam.GetRebarAreaRequired();
		SFtem = AsUse/AsReq;
		
		if(i==0)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
		else if(SFmin>SFtem)
		{
			MaxAsUse = AsUse;
			MaxAsReq = AsReq;
			SFmin = MaxAsUse/MaxAsReq;
		}
	}
	
	if(SFmin > 0) szRet.Format("req As=%.3f\n< use As=%.3f", MaxAsReq, MaxAsUse);
	else szRet.Format("req As=%.3f\n> use As=%.3f", MaxAsReq, MaxAsUse);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxVuSlabCenter(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum()-1;
	double Vu = 0;
	double PiVn = 0;
	double MaxVu = 0;
	double MaxPiVn = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataSlab(i, CHECK_POS_CENTER);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Vu = pBeam.GetVu();
		PiVn = pBeam.GetPiVn();
		SFtem = PiVn/Vu;
		
		if(i==0)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
		else if(SFmin>SFtem)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
	}
	
	if(SFmin > 0) szRet.Format("Vuf = %.3f\n< ��Vn = %.3f", MaxVu, MaxPiVn);
	else szRet.Format("Vuf = %.3f\n> ��Vn = %.3f", MaxVu, MaxPiVn);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxVuWall(CRcBridgeRebar *pBri, BOOL bUpper)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum()-1;
	double Vu = 0;
	double PiVn = 0;
	double MaxVu = 0;
	double MaxPiVn = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataWall(i, (bUpper)? CHECK_POS_TOP:CHECK_POS_CENTER, (bUpper)? TRUE:FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Vu = pBeam.GetVu();
		PiVn = pBeam.GetPiVn();
		SFtem = PiVn/Vu;
		
		if(i==0)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
		else if(SFmin>SFtem)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
	}
	
	if(SFmin > 0) szRet.Format("Vuf = %.3f\n< ��Vn = %.3f", MaxVu, MaxPiVn);
	else szRet.Format("Vuf = %.3f\n> ��Vn = %.3f", MaxVu, MaxPiVn);
	
	return szRet;
}

CString COutBridgeResister::GetStringMaxVuFoot(CRcBridgeRebar *pBri)
{
	CString szRet = _T("");
	long nCnt = pBri->GetCountJijum();
	double Vu = 0;
	double PiVn = 0;
	double MaxVu = 0;
	double MaxPiVn = 0;
	double SFmin = 0;
	double SFtem = 0;
	
	long i=0; for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, TRUE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Vu = pBeam.GetVu();
		PiVn = pBeam.GetPiVn();
		SFtem = PiVn/Vu;
		
		if(i==0)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
		else if(SFmin>SFtem)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
	}

	for(i=0; i<nCnt; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(i, FALSE);
		if(pConc == NULL) continue;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		Vu = pBeam.GetVu();
		PiVn = pBeam.GetPiVn();
		SFtem = PiVn/Vu;
		
		if(i==0)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
		else if(SFmin>SFtem)
		{
			MaxVu = Vu;
			MaxPiVn = PiVn;
			SFmin = MaxPiVn/MaxVu;
		}
	}
	
	if(SFmin > 0) szRet.Format("Vuf = %.3f\n< ��Vn = %.3f", MaxVu, MaxPiVn);
	else szRet.Format("Vuf = %.3f\n> ��Vn = %.3f", MaxVu, MaxPiVn);
	
	return szRet;
}

double COutBridgeResister::GetCalBM(long nBri, long nKind)
{
	CARcBridgeDoc *pDoc = dynamic_cast<CARcBridgeDoc *>(m_pStd->m_pDoc);

	pDoc->m_pARcBridgeOutBMStd->MakeBMRcBridgeArray();
	COutBM *pOutBM	= pDoc->m_pARcBridgeOutBMStd->m_pArrOutBMNormal.GetAt(nBri);//new COutBM(nBri);

	pOutBM->CalculateBM();
	pOutBM->MergeBMStd();
	pOutBM->MakeBMStringStd();
	pOutBM->SetInsertDomyunStd();
// #define BM_BIG_CONCRETE		1	// ��ũ��Ʈ
// #define BM_BIG_REBAR			9	// ö�ٰ�������
// #define BM_BIG_PILE			15	// ����
	
	// ���� ��������
	double dTot	= 0;
	CBMData *pBM = NULL;
	// ��ü
	long i = 0; for(i = 0; i < pOutBM->m_pArrBMBridgeStd.GetSize(); i++)
	{
		pBM = pOutBM->m_pArrBMBridgeStd.GetAt(i);
		if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
		{
			if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
			dTot += pBM->GetResult();
		}
	}

	// ������
	long j=0; for(j=0; j<2; j++)
	{
		for(long k=0; k<2; k++)
		{
			for(i = 0; i < pOutBM->m_pArrBMWingStd[j][k].GetSize(); i++)
			{
				pBM = pOutBM->m_pArrBMWingStd[j][k].GetAt(i);
				if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
				{
					if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
					dTot += pBM->GetResult();
				}
			}
		}
	}

	// ���ӽ�����
	for(j=0; j<2; j++)
	{
		for(long k=0; k<2; k++)
		{
			for(i = 0; i < pOutBM->m_pArrBMApsStd[j][k].GetSize(); i++)
			{
				pBM = pOutBM->m_pArrBMApsStd[j][k].GetAt(i);
				if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
				{
					if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
					dTot += pBM->GetResult();
				}
			}
		}
	}

	// ���潽����
	for(long ix=0; ix<2; ix++)
	{
		for(j=0; j<2; j++)
		{
			for(long k=0; k<2; k++)
			{
				for(i = 0; i < pOutBM->m_pArrBMCnsStd[ix][j][k].GetSize(); i++)
				{
					pBM = pOutBM->m_pArrBMCnsStd[ix][j][k].GetAt(i);
					if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
					{
						if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
						dTot += pBM->GetResult();
					}
				}
			}
		}
	}

	// ��ȣ��
	for(long k=0; k<2; k++)
	{
		for(i = 0; i < pOutBM->m_pArrBMBohoStd[k].GetSize(); i++)
		{
			pBM = pOutBM->m_pArrBMBohoStd[k].GetAt(i);
			if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
			{
				if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
				dTot += pBM->GetResult();
			}
		}
	}
	
	// ��ȣ��
	for(long k=0; k<nMaxQtyHdan; k++)
	{
		for(i = 0; i < pOutBM->m_pArrBMGuardWallStd[k].GetSize(); i++)
		{
			pBM = pOutBM->m_pArrBMGuardWallStd[k].GetAt(i);
			if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
			{
				if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
				dTot += pBM->GetResult();
			}
		}
	}

	// ������
	for(i = 0; i < pOutBM->m_pArrBMMonumentStd.GetSize(); i++)
	{
		pBM = pOutBM->m_pArrBMMonumentStd.GetAt(i);
		if(pBM->m_nIdxBigClass == nKind || (nKind==BM_BIG_PILE && pBM->m_nIdxBigClass>=nKind))
		{
			if(nKind == BM_BIG_PILE && pBM->m_sUnit != "��") continue;
			dTot += pBM->GetResult();
		}
	}

	return dTot;
}
