#include "stdafx.h"
#include "ARcBridgeCalculate.h"

CARcBridgeCalcStm::CARcBridgeCalcStm(CARcBridgeCalcStd* pStd)
{
	m_pStd	= pStd;
	m_pCalcStmBracket	= new CCalcStmBracket(this, TRUE);
	m_pCalcStmCorner	= new CCalcStmCorner(this);
	m_pCalcStmFoot		= new CCalcStmFoot(this, TRUE);
	m_pCalcStmMidFoot	= new CCalcStmMidFoot(this);

	m_bSimpleModeling = TRUE;
}


CARcBridgeCalcStm::~CARcBridgeCalcStm(void)
{
	delete m_pCalcStmBracket;
	delete m_pCalcStmCorner;
 	delete m_pCalcStmFoot;
 	delete m_pCalcStmMidFoot;
}

void CARcBridgeCalcStm::SetFootRebar( CStmModel *pStmModel, HGINT32 nJijum )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(!pBri) return;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(pFoot == NULL) return;

	CStmDataDefine *pDefine = pStmModel->GetStmDataDefine();
	if(pDefine == NULL) return;

	CConcBasic *pConc = NULL;
	SStmDefineTieType *pTie = NULL;
	HGINT32 iTie(0);

	//��ö��
	for(HGINT32 nUpper=iUPPER; nUpper<2; ++nUpper)
	{
		HGBOOL bUpper = (nUpper == iUPPER);

		// �ձ��� �ϸ� �ޱ��� ���
		pConc = pBri->GetConcDataFooting(nJijum, bUpper? FALSE : TRUE);
		if(pConc == NULL) continue;

		iTie = pDefine->GetIndexTieTypeByName(bUpper? STM_REBAR_STR_UPPER_REBAR : STM_REBAR_STR_LOWER_REBAR);
		pTie = pDefine->GetTieType(iTie);

		if(pTie)
		{
			ClearVectorPtr(pTie->main_rebar);
			for(HGINT32 nDan=0; nDan<MAX_REBAR_DAN; ++nDan)
			{
				HGDOUBLE dCover = pConc->m_RbT1C_dc[nDan];
				if(pConc->m_RbT1C_DIA[nDan] == 0)
					continue;

				SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;
				pMainRebar->cover			= dCover;

				for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
				{
					HGDOUBLE dDia = (nCycle == 0)? pConc->m_RbT1C_DIA[nDan] : pConc->m_RbT2C_DIA[nDan];
					if(Compare(dDia, 0.0, _T("="), 0.01)) continue;

					HGDOUBLE dCnt = (nCycle == 0)? pConc->m_RbT1C_B[nDan] : pConc->m_RbT2C_B[nDan];
					
					pMainRebar->dia[nCycle]		= dDia;
					pMainRebar->count[nCycle]	= dCnt;
				}
				pTie->main_rebar.push_back(pMainRebar);
			}
		}
	}

	//��������ö��
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_VERT_SHEAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie && pConc)
	{
		ClearVectorPtr(pTie->shear_rebar);

		SStmTieShearRebar *pShearRebar = new SStmTieShearRebar;
		pShearRebar->dia		= pConc->m_SvDia;
		pShearRebar->count		= pConc->m_SvEa;
		pShearRebar->ctc		= pFoot->GetCTCRebarShear(); // pConc->m_SvCtc;//	(ARCBRIDGE-3238) �ձ��� ª�Ƽ� ����ö�� ������ ���谡 �ȵǴ� ��찡 �ִ�. �ձ����� �������� ���� �Է°����� �����´�.
		pTie->type_shear		= eStmShearVert;
		pTie->shear_rebar.push_back(pShearRebar);
	}

	//��������ö��
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_HORI_SHEAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie)
	{
		pDefine->EraseTieType(iTie);
	}
}

void CARcBridgeCalcStm::WriteExcelSTM_Bracket_CalcLoad( CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri, CXLFormatText *XLText, long nColPos, HGINT32 iKey )
{
	long nIndexSmall = 0;
	long nSizeRow = 0;

	HGBOOL bLSD = pData->IsLsdDesign();

	std::map<HGINT32,CStmObjNode*> mpNode = pStm->GetNodes();
	std::map<HGINT32,CStmObjNode*>::iterator posNode;
	for(posNode = mpNode.begin(); posNode != mpNode.end(); ++posNode)
	{
		if(!posNode->second->IsEnable()) continue;

		if(pStm->IsExistingLoadNode(posNode->second->GetId()))
			nSizeRow++;
	}

	//////////////////////////////////////////////////////////////////////////
	// �귡Ŷ����
	XLText->GoPosition(NO_MOVE, nColPos);
	XLText->AddFormatText("$h(%d) �귡Ŷ���� (SW)$n",++nIndexSmall);
	XLText->AddFormatText("$h$r: ������ ������ ��յβ��� �����Ͽ� ��������� �����Ͽ� ����, ������� ������ 20%%�� �����Ѵ�.$n");
	long nCountCol = 7; // ����,�д���,��յβ�,������,�����߷�,��������,��������
	long nWidCol[7] = {3, 3, 3, 3, 3, 4, 4};
	XLText->GoPosition(NO_MOVE, nColPos + 1);
	XLText->AddTable(nSizeRow, 22, 1, 1);
	
	CString strUnit = bLSD ? _T("(N)") : _T("(kN)");
	CString sTitle[7] = { _T("����"), _T("�д���"), _T("��յβ�"), _T("������"), _T("�����߷�"), _T("��������") + strUnit, _T("��������") + strUnit };
	for(long ix = 0; ix < nCountCol; ix++)
	{
		XLText->AddMergeFormatText(0, nWidCol[ix]-1,_T("$c%s"), sTitle[ix]);
		XLText->AddFormatText(_T("$m+0%d"), nWidCol[ix]);
	}
	XLText->AddText(_T("$n$h"));

	CVector vPos(0,0,0);
	double dUW = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	bool bStt = iKey == STM_MODEL_BRACKET_STT ? TRUE : FALSE;
	double dTB = toM(bStt ? pBri->m_dBHWS : pBri->m_dBHWE);
	double dAverageThick = bStt==TRUE ? ((pBri->m_dBHS - pBri->m_dBHHS) + pBri->m_dBHHS / 2) : ((pBri->m_dBHE - pBri->m_dBHHE) + pBri->m_dBHHE / 2);
	long nDec = bLSD ? 0 : 3;

	CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(_T("SW"));
	if (pLoadStatic)
	{
		long nCountLoad = pLoadStatic->GetCountLoadNode();
		for(long ix = 0; ix < nCountLoad; ix ++)
		{
			CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(ix);
			CVector vLoad = pLoadNode->GetLoad();
			CStmObjNode *pNode = pStm->GetNode(pLoadNode->GetIdNode());
			CString str = _T("");
			
			for(long kk = 0; kk < nCountCol; kk++)
			{
				if(kk == 0)
					str = pNode->GetName();
				else if(kk == 1) // �д���
					str = GetStrDouble(GetValueUnitChange(dTB, UNIT_CGS_M, pData->m_szM), nDec, bLSD, TRUE);
				else if(kk == 2) // ��յβ�
					str = GetStrDouble(bLSD ? dAverageThick : toM(dAverageThick), nDec, bLSD, TRUE);
				else if(kk == 3) // ��
					str = GetStrDouble(pStm->GetThickModel(), nDec, bLSD, TRUE);		// Set�� �� ������ȯ�� �ؼ� �Ѱ��ְ� ����.
				else if(kk == 4) // �����߷�
					str = GetStrDouble(dUW, bLSD ? 6 : 3 , bLSD, TRUE);
				else if(kk == 5) // ��������
					str = GetStrDouble(vLoad.z, nDec, bLSD, TRUE);
				else if(kk == 6) // �������� 
					str = GetStrDouble(vLoad.x, nDec, bLSD, TRUE);

				XLText->AddMergeFormatText(0, nWidCol[kk]-1,_T("$c%s"), str);
				XLText->AddFormatText(_T("$m+0%d"), nWidCol[kk]);
			}
			XLText->AddText(_T("$n$h"));
		}
	}
	

	///////////��������, Ȱ����
	for(long nLoad = 0; nLoad < 2; nLoad++)
	{
		CString strLoad = (nLoad == 0 ? _T("D") : _T("L"));

		nCountCol = 3;
		long nWidCol[4] = {3, 4, 4};
		XLText->AddText(_T("$n$h"));
		XLText->GoPosition(NO_MOVE, nColPos);
		if(nLoad == 0)
		{
			XLText->AddFormatText("$h(%d) �������� (D)$n",++nIndexSmall);
			XLText->AddFormatText("$h$r: ���������� ���ӽ����� ����-��ħ�� ��������, ������� ���������� 20%%�� �����Ѵ�.$n");
		}
		else
		{
			XLText->AddFormatText("$h(%d) Ȱ���� (L)$n",++nIndexSmall);
			XLText->AddFormatText("$h$r: Ȱ������ ���ӽ����� ����-��ħ�� ��������, ������� ���������� 20%%�� �����Ѵ�.$n");
		}
		XLText->GoPosition(NO_MOVE, nColPos + 1);
		XLText->AddTable(nSizeRow, 10, 1, 1);

		CString sTitle1[3] = { _T("����"), _T("��������") + strUnit, _T("��������") + strUnit };
		for(long ix = 0; ix < nCountCol; ix++)
		{
			XLText->AddMergeFormatText(0, nWidCol[ix]-1,_T("$c%s"), sTitle1[ix]);
			XLText->AddFormatText(_T("$m+0%d"), nWidCol[ix]);
		}
		XLText->AddText(_T("$n$h"));

		CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(strLoad);
		if (pLoadStatic)
		{
			long nCountNode = pLoadStatic->GetCountLoadNode();
			for(long ix = 0; ix < nCountNode; ix++)
			{
				CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(ix);
				CVector vLoad = pLoadNode->GetLoad();
				CStmObjNode *pNode = pStm->GetNode(pLoadNode->GetIdNode());

				CString str = _T("");
				for(long kk = 0; kk < nCountCol; kk++)
				{
					if(kk == 0)
						str = pNode->GetName();
					else if(kk == 1) // ��������
						str = GetStrDouble(vLoad.z, nDec, bLSD, TRUE);
					else if(kk == 2) // ��������
						str = GetStrDouble(vLoad.x, nDec, bLSD, TRUE);

					XLText->AddMergeFormatText(0, nWidCol[kk]-1,_T("$c%s"), str);
					XLText->AddFormatText(_T("$m+0%d"), nWidCol[kk]);
				}
			}
			XLText->AddText(_T("$n$h"));
		}	
	}
}

void CARcBridgeCalcStm::WriteExcelSTM_Footing_CalcLoad( CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri, CXLFormatText *XLText, long nColPos, HGINT32 iKey )
{
	CStmDataConfig *pStmData = pStm->GetStmDataConfig();
	if(pStmData == NULL) return;

	CLoadFooting *pLoad;
	long nIndexSmall = 1;
	long nIndex = 0;
	long nCntJijum = pBri->GetCountJijum();

	HGBOOL bLsd = pStm->IsLsdDesign();
	if (iKey == STM_MODEL_FOOT_STT_MAXP || iKey == STM_MODEL_FOOT_STT_MAXM || iKey == STM_MODEL_FOOT_STT_MINM || iKey == STM_MODEL_FOOT_STT_EARTH || 
		 (STM_MODEL_FOOT_STT_MAX_AXIAL <= iKey && iKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2 ))		// ����
		nIndex = 0;
	else if (iKey == STM_MODEL_FOOT_END_MAXP || iKey == STM_MODEL_FOOT_END_MAXM || iKey == STM_MODEL_FOOT_END_MINM || iKey == STM_MODEL_FOOT_END_EARTH ||
		 (STM_MODEL_FOOT_END_MAX_AXIAL <= iKey && iKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 ) )		// ����
		nIndex = nCntJijum - 1;
	else
	{
		nIndex = pData->IsLsdDesign() ? (iKey - STM_MODEL_FOOT_MID_MAX_AXIAL) : (iKey - STM_MODEL_MIDFOOT_HEAD);
		nIndex /= bLsd? (5 * 3) : 4;
		nIndex += 1;
	}

	XLText->AddFormatText("$e$h(%d) ��� ��������$n", nIndexSmall++);
	m_pStd->m_pFooting->MakeTableLoadSum(XLText, nIndex, FALSE, FALSE, 0, FALSE, pBri->GetLsdEnvType(iKey));

	std::map<CString,CVector> VecLoad;
	GetTotalLoadNodeConditionStatics(pStm, VecLoad, TRUE);

	long nCountCol = 4; // NO, ������(V),�����(H), ���
	long nCountLoad = VecLoad.size();
	long nWidCol[4] = {3, 3, 3, 3};
	XLText->GoPosition(NO_MOVE, nColPos + 1);
	XLText->AddTable(nCountLoad, 12-1, 1, 1);

	CString sTitle[4] = { _T("NO"), _T("������(V)"), _T("�����(H)"), _T("���")};
	for(long ix = 0; ix < nCountCol; ix++)
	{
		XLText->AddMergeFormatText(0, nWidCol[ix]-1,_T("$c%s"), sTitle[ix]);
		XLText->AddFormatText(_T("$m+0%d"), nWidCol[ix]);
	}
	XLText->AddText(_T("$n$h"));

	std::map<CString, CVector>::iterator miter;
	for (miter = VecLoad.begin(); miter != VecLoad.end(); ++miter)
	{
		CString str = _T("");
		for(long jx = 0; jx < nCountCol; ++jx)
		{
			if(jx == 0)	//No
				str.Format(_T("%s"), miter->first);
			else if(jx == 1) // ������(V)
				str.Format(_T("$f3%.3f"), miter->second.z);
			else if(jx == 2) // �����(H)
				str.Format(_T("$f3%.3f"), miter->second.x);
			else if(jx == 3) // ���
				str.Format(_T("%s"), _T(""));

			XLText->AddMergeFormatText(0, nWidCol[jx]-1,_T("$c%s"), str);
			XLText->AddFormatText(_T("$m+0%d"), nWidCol[jx]);
		}
		XLText->AddText(_T("$n$h"));
	}

	XLText->AddText(_T("$n$h"));
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$e$h(%d) ��Ÿ �ۿ�����$n", nIndexSmall++);

	// ������ ��Ÿ �ۿ�����
	long iCase = bLsd ? (iKey - STM_MODEL_MIDFOOT_HEAD) % 4 : (iKey - STM_MODEL_FOOT_MID_MAX_AXIAL) % 5;
	long nLsdEnvType = pBri->GetLsdEnvType(iKey);
	if (iKey == STM_MODEL_FOOT_STT_EARTH || iKey == STM_MODEL_FOOT_END_EARTH || iCase == 3)
	{
		pLoad = bLsd ? m_pStd->m_pFooting->m_pListLoadLsd[nIndex][nLsdEnvType].GetAt(3) : m_pStd->m_pFooting->m_pListLoadUlt[nIndex].GetAt(3);
		pLoad->MakeTableOtherLoad(XLText);
	}	
	else // ���� ��Ÿ �ۿ�����
	{
		pLoad = bLsd ? m_pStd->m_pFooting->m_pListLoadLsd[nIndex][nLsdEnvType].GetAt(0) : m_pStd->m_pFooting->m_pListLoadUlt[nIndex].GetAt(0);
//			pLoad = m_pStd->m_pFooting->m_pListLoadUlt[nIndex].GetAt(0);
		pLoad->MakeTableOtherLoad(XLText);
	}
	*XLText += 1;

	std::map<CString,CVector> VecLoad1;
	GetTotalLoadNodeConditionStatics(pStm, VecLoad1);

	nCountCol = 4; // NO, ������(V),�����(H), ���
	nCountLoad = VecLoad1.size();
	long nWidCol1[4] = {3, 3, 3, 3};
	XLText->GoPosition(NO_MOVE, nColPos + 1);
	XLText->AddTable(nCountLoad, 12-1, 1, 1);

	CString sTitle1[4] = { _T("NO"), _T("������(V)"), _T("�����(H)"), _T("���")};
	for(long ix = 0; ix < nCountCol; ix++)
	{
		XLText->AddMergeFormatText(0, nWidCol1[ix]-1,_T("$c%s"), sTitle1[ix]);
		XLText->AddFormatText(_T("$m+0%d"), nWidCol1[ix]);
	}
	XLText->AddText(_T("$n$h"));

	std::map<CString, CVector>::iterator miter1;
	for (miter1 = VecLoad1.begin(); miter1 != VecLoad1.end(); ++miter1)
	{
		CString str = _T("");
		for(long jx = 0; jx < nCountCol; ++jx)
		{
			if(jx == 0)	//No
				str.Format(_T("%s"), miter1->first);
			else if(jx == 1) // ������(V)
				str.Format(_T("$f3%.3f"), miter1->second.z);
			else if(jx == 2) // �����(H)
				str.Format(_T("$f3%.3f"), miter1->second.x);
			else if(jx == 3) // ���
				str.Format(_T("%s"), _T(""));

			XLText->AddMergeFormatText(0, nWidCol1[jx]-1,_T("$c%s"), str);
			XLText->AddFormatText(_T("$m+0%d"), nWidCol1[jx]);
		}
		XLText->AddText(_T("$n$h"));
	}

	//�����ϴܹݷ��ۿ�
	if(pStmData->m_ixTypeFootingLoad == 1)
	{
		CString strUnit = pData->IsLsdDesign() ? _T("(N)") : _T("");

		long nJijum = 0;
		if(nIndex == 0 || nIndex == nCntJijum-1)
			nJijum = nIndex;
		else if(m_pCalcStmMidFoot) 
			nJijum = m_pCalcStmMidFoot->GetJijum();

		CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
		if(pFoot == NULL) return;

		CExPileApp *pPile = &(pFoot->m_exFooting.m_Pile);
		if(pPile == NULL) return;

		CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(_T("REACTION"));
		if(pLoadStatic)
		{
			long nCountLoad = pLoadStatic->GetCountLoadNode();
			CString strName = pLoadStatic->GetName();
			CString strComment = pLoadStatic->GetComment();

			XLText->AddFormatText("$n$s+%02d", nCountLoad + 2);
			XLText->GoPosition(NO_MOVE, nColPos + 1);
			long nCountCol = 5; // ����, �����ݷ�, ����ݷ�, ��������, ��������
			long nWidCol = 3;

			XLText->GoPosition(NO_MOVE, nColPos + 1);
			XLText->AddTable(nCountLoad, (nWidCol*nCountCol)-1, 1, 1);

			CString sTitle1[6] = { _T("����"), _T("�����ݷ�") + strUnit, _T("����ݷ�") + strUnit, _T("��������") + strUnit, _T("��������") + strUnit};
			for(long ix = 0; ix < nCountCol; ix++)
			{
				XLText->AddMergeFormatText(0, nWidCol-1,_T("$c%s"), sTitle1[ix]);
				XLText->AddFormatText(_T("$m+0%d"), nWidCol);
			}
			XLText->AddText(_T("$n$h"));

			for(long ix = 0; ix < nCountLoad; ix ++)
			{
				CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(ix);
				CVector vLoad = pLoadNode->GetLoad();
				CStmObjNode *pNode = pStm->GetNode(pLoadNode->GetIdNode());
				CString str = _T("");
				
				long nPileSide	= pPile ? pPile->GetPileSu(ix, TRUE) : 0;
				double dReactionV = vLoad.z / nPileSide * pFoot->GetLength() / 1000.;
				double dReactionH = vLoad.x / nPileSide * pFoot->GetLength() / 1000.;

				for(long kk = 0; kk < nCountCol; kk++)
				{
					if(kk == 0)
						str = pNode->GetName();
					else if(kk == 1)
						str = GetStrDouble(dReactionV, 0, bLsd, TRUE);
					else if(kk == 2)
						str = GetStrDouble(dReactionH, 0, bLsd, TRUE);
					else if(kk == 3)
						str = GetStrDouble(vLoad.z, 0, bLsd, TRUE);
					else if(kk == 4)
						str = GetStrDouble(vLoad.x, 0, bLsd, TRUE);

					XLText->AddMergeFormatText(0, nWidCol-1,_T("$c%s"), str);
					XLText->AddFormatText(_T("$m+0%d"), nWidCol);
				}
				XLText->AddText(_T("$n$h"));
			}
			XLText->AddText(_T("$n$h"));
		}
	}
}

void CARcBridgeCalcStm::GetTotalLoadNodeConditionStatics(CStmModel *pStm, std::map<CString,CVector> &mVecLoad, HGBOOL bTopLoad)
{
	mVecLoad.clear();
	for(long ix = 0; ix < pStm->GetCountLoadConditionStatic(); ix ++)
	{
		CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(ix);
		if (!pLoadStatic) continue;

		if (bTopLoad && pLoadStatic->GetName() != _T("W"))
			continue;
		else if (!bTopLoad && pLoadStatic->GetName() == _T("W"))
			continue;

		long nConuntNode = pLoadStatic->GetCountLoadNode();
		for (long jx = 0; jx < nConuntNode; ++jx)
		{
			CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(jx);
			if (!pLoadNode) continue;

			CVector vLoad = pLoadNode->GetLoad();
			HGINT32 id = pLoadNode->GetIdNode();

			CStmObjNode *pNode = pStm->GetNode(id);
			if(pNode == NULL) continue;

			CString sName = pNode->GetName();
			std::map<CString, CVector>::iterator miter;
			for (miter = mVecLoad.begin(); miter != mVecLoad.end(); ++miter)
			{
				if (miter->first == sName)
				{
					vLoad.x += miter->second.x;
					vLoad.z += miter->second.z;
				}
			}

			std::map<CString, CVector>::iterator iter = mVecLoad.find(sName);
			if( iter == mVecLoad.end() )
				mVecLoad.insert(std::make_pair(sName, vLoad));
			else
				iter->second = vLoad;
		}
	}
}

void CARcBridgeCalcStm::WriteExcelSTM_ArcCorner_CalcLoad( CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri, CXLFormatText *XLText, long nColPos, HGINT32 iKey )
{
	long nIndexSmall = 0;
	long nSizeRow = 0;

// 	long nIndex = 0;
// 	if (iKey == STM_MODEL_CORNER_STT_UPPER_PLUS || iKey == STM_MODEL_CORNER_STT_LOWER_PLUS|| iKey == STM_MODEL_CORNER_STT_UPPER_MINUS || iKey == STM_MODEL_CORNER_STT_LOWER_MINUS)
// 		nIndex = 0;
// 	else
// 		nIndex = pBri->GetCountJijum()-1;

//	double dThickOutWall = pBri->GetThickWall(nIndex);

	CStmCorner *pStmCorner = new CStmCorner;
	
	pStmCorner->SetStmModel(pStm);
	m_pCalcStmCorner->SetPosCorner(iKey);
	m_pCalcStmCorner->SetCornerTemplate(pStmCorner);
	m_pCalcStmCorner->SetCornerLoad(pStmCorner);


	std::map<HGINT32,CStmObjNode*> mpNode = pStm->GetNodes();
	std::map<HGINT32,CStmObjNode*>::iterator posNode;
	for(posNode = mpNode.begin(); posNode != mpNode.end(); ++posNode)
	{
		if(!posNode->second->IsEnable()) continue;

		if(pStm->IsExistingLoadNode(posNode->second->GetId()))
			nSizeRow++;
	}

//	XLText->AddFormatText("$e$h%d)$r�������� (D)$n",++nIndexSmall);

	XLText->AddFormatText("$e$h(%d) Moment Load(M, 1m��)", ++nIndexSmall);

	long nCountCol = 3;
	long nWidCol[3] = {3, 4, 4};
	XLText->AddText(_T("$n$h"));
	XLText->GoPosition(NO_MOVE, nColPos);

	XLText->GoPosition(NO_MOVE, nColPos + 1);
	XLText->AddTable(nSizeRow, 10, 1, 1);

	HGBOOL bLsd = pStm->IsLsdDesign();
	CString sTitle[3] = { _T("����"), _T("��������(kN)"), _T("��������(kN)") };
	if( bLsd ) { 
		sTitle[1] = _T("��������(N)"); sTitle[2] = _T("��������(N)");
	}
	for(long ix = 0; ix < nCountCol; ix++)
	{
		XLText->AddMergeFormatText(0, nWidCol[ix]-1,_T("$c%s"), sTitle[ix]);
		XLText->AddFormatText(_T("$m+0%d"), nWidCol[ix]);
	}
	XLText->AddText(_T("$n$h"));

	CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(_T("M"));
	if (pLoadStatic)
	{
		CDoubleArray dArrVert, dArrHori;
		long nCountNode = pLoadStatic->GetCountLoadNode();
		
		for(long ix = 0; ix < nCountNode; ix++)
		{
			CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(ix);
			CVector vLoad = pLoadNode->GetLoad();
			CStmObjNode *pNode = pStm->GetNode(pLoadNode->GetIdNode());
			CString str = _T("");
			dArrVert.Add(vLoad.z);
			dArrHori.Add(vLoad.x);
			for(long kk = 0; kk < nCountCol; kk++)
			{
				if(kk == 0)
					str = pNode->GetName();
				else if(kk == 1) // ��������
				{
					double dValue = pBri->m_bCheckCorner ? (vLoad.z==0 ? vLoad.z : GetLoadMax(pStm, _T("M"), vLoad.z>0 ? 1 : -1)) : vLoad.z;
					str = pData->GetStrDouble(dValue, UNIT_CGS_TONF);
				}
				else if(kk == 2) // ��������
				{
					double dValue = pBri->m_bCheckCorner ? (vLoad.x==0 ? vLoad.x : GetLoadMax(pStm, _T("M"), vLoad.x>0 ? 1 : -1)) : vLoad.x;
					str = pData->GetStrDouble(dValue, UNIT_CGS_TONF);
				}

				XLText->AddMergeFormatText(0, nWidCol[kk]-1,_T("$c%s"), str);
				XLText->AddFormatText(_T("$m+0%d"), nWidCol[kk]);
			}
			XLText->AddText(_T("$n$h"));
		}

		XLText->GoPosition(NO_MOVE, nColPos);

		for (HGINT32 ix=0; ix<2; ++ix)
		{
			HGDOUBLE dMoment = pStmCorner->m_dMoment;
			HGDOUBLE dLoadFrame = pStmCorner->GetLoadFrame(ix==0 ? TRUE : FALSE);
			HGDOUBLE dLength = fabs(dMoment / dLoadFrame);

			CString str = ix==0 ? _T("Ls") : _T("Lw");
			XLText->AddText(_T("$n$h"));
			XLText->AddCellRowLine(nColPos + 2, nColPos + 2, 2);
			XLText->AddCellRowLine(nColPos + 4, nColPos + 6, 2);

			XLText->AddFormatText(_T("$c$md1T$r$c$md1=$r$cMu$r$c$md1=$r$c$me2%s$m+03$c$md1=$r$c$me3$md1%s$m+04$c$md1N"), pData->GetStrDouble(dMoment, UNIT_CGS_TONF),
				pData->GetStrDouble(dMoment/dLength, UNIT_CGS_TONF));
			XLText->AddFormatText(_T("$n$h$m+02$c%s$m+02$c$me2%s"), str, pData->GetStrDouble(dLength, UNIT_CGS_M));
		}
		
		if( pBri->m_bCheckCorner )
			XLText->AddFormatText(_T("$n$h���� �ִ����� %s N �� ��� T = C �� ����"), pData->GetStrDouble(MAX(dArrVert.Max(), dArrHori.Max()), UNIT_CGS_TONF));
		else
			XLText->AddFormatText(_T("$n$h���� ��� T = C �� ����"));
	}	

	delete pStmCorner;
}

void CARcBridgeCalcStm::SetConcData( HGBOOL bSlab/*=FALSE*/, HGBOOL bWall/*=FALSE*/, HGBOOL bFoot/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(!pBri) return;

	// ���⼭ �ð��� �ʹ� ���� �ɸ���. ConcData�� �������� �ٽ� ��������.
	// ���� ö���� �����ϰ� �ٷ� �̸��� �Ѿ���� �ٲ� ö���� �ݿ��� �ȵɼ��� �ְ�����
	// 2D Frame �ؼ��� ���ִٸ� ö�ٺ��� ���Ŀ� ������䰡 ���״� �Ƹ��� �ݿ��� �ɰ��̴�.

	// ö�� �����Ѱ͵��� �ݿ��� �ȵȴ�. �������� ��� ����������.
//	if(bSlab && pBri->m_pArrConcSlab.GetSize() == 0)
	{
		pBri->SetConcDataSlab();
	}
//	if(bWall && pBri->m_pArrConcWall.GetSize() == 0)
	{
		pBri->SetConcDataWall();
	}
//	if(bFoot && pBri->m_pArrConcFoot.GetSize() == 0)
	{
		pBri->SetConcDataFooting(FALSE);
		pBri->SetConcDataFooting(TRUE);
	}
}

HGDOUBLE CARcBridgeCalcStm::GetLoadMax(CStmModel *pStm, CString strNameLoadConditionStatic, long nSw)
{
	HGDOUBLE dMax(0);
	CStmObjLoadConditionStatic *pLoadStatic = pStm->GetLoadConditionStatic(strNameLoadConditionStatic);
	if (pLoadStatic)
	{
		long nCountNode = pLoadStatic->GetCountLoadNode();

		for(long ix = 0; ix < nCountNode; ix++)
		{
			CStmObjLoadNode *pLoadNode = pLoadStatic->GetLoadNode(ix);
			CVector vLoad = pLoadNode->GetLoad();

			dMax = (fabs(vLoad.x) > fabs(vLoad.z)) ? fabs(vLoad.x) : fabs(vLoad.z);
		}
	}

	return dMax * nSw; 
}