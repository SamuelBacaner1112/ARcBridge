// RcBridgeRebarUtil.cpp: implementation of the CRcBridgeRebarUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRcBridgeRebarUtil::CRcBridgeRebarUtil()
{
	m_pARcBridgeDataStd	= NULL;
}

CRcBridgeRebarUtil::~CRcBridgeRebarUtil()
{

}




// rebarinfocycle �迭 �ΰ��� ��ģ��.
// BOOL bInvert : ��ĥ �迭�� ����� ��ĥ��..?
// �� �Լ� ȣ��� �ݵ�� �޸� ���� �� �ٰ�.
void CRcBridgeRebarUtil::AddRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrDest, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrSource, BOOL bInvert)
{
	if(!pArrDest || !pArrSource) return;

	long nSize = pArrSource->GetSize();
	if(bInvert)
	{
		long i = 0; for(i = nSize-1; i >= 0; i--)
		{
			CRebarInfoCycle *pRC	= pArrSource->GetAt(i);
			
			CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;
			*pRCNew	= *pRC;
			InvertRebarInfoCycle(pRCNew);

			pArrDest->Add(pRCNew);
		}
	}
	else
	{
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CRebarInfoCycle *pRC	= pArrSource->GetAt(i);

			CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;
			*pRCNew	= *pRC;
			pArrDest->Add(pRCNew);
		}
	}
}


// rebarinfocycle�� �����´�.
void CRcBridgeRebarUtil::InvertRebarInfoCycle(CRebarInfoCycle *pRC)
{
	if(!pRC) return;

	if(pRC->m_nPosStt == pRC->m_nPosEnd)
	{
		GetSwap(pRC->m_xyStt, pRC->m_xyMid1);
	}
	else
	{
		if(pRC->m_bUseEndExt)
		{
			GetSwap(pRC->m_xyStt, pRC->m_xyEndExt);
			GetSwap(pRC->m_xyMid1, pRC->m_xyMidExt);
			GetSwap(pRC->m_xyMid2, pRC->m_xyEnd);
			GetSwap(pRC->m_nPosStt, pRC->m_nPosEnd);
			GetSwap(pRC->m_xyCen, pRC->m_xyCenExt);
			GetSwap(pRC->m_R_Mid, pRC->m_R_MidExt);
		}
		else
		{
			GetSwap(pRC->m_xyStt, pRC->m_xyEnd);
			GetSwap(pRC->m_xyMid1, pRC->m_xyMid2);
			GetSwap(pRC->m_nPosStt, pRC->m_nPosEnd);
		}
	}
}

// �� ö���� ���ؼ� ������ �մ��� �Ǵ�.
// �Ϲ������� �� rebarinfocycle�� ���ؼ� ������ ã���� ������, ��ν����� ö���� ��� ������ 
// ���󰡱� ������ �Ϲ����� ������� ã�Ⱑ ��ƴ�.
// �׷��� ��ν����� ö�ٸ� �ٸ� ������� ã�´�.
BOOL CRcBridgeRebarUtil::GetJointAtRebarInfoCycle(CRebarInfoCycle *pRC1, CRebarInfoCycle *pRC2, CRebarJoint &joint, BOOL bParabola)
{
	if(!pRC1 || !pRC2) return FALSE;
	if(!pRC1->m_bExist || !pRC2->m_bExist) return FALSE;

	BOOL bJoint		= FALSE;

	CDPointArray xyArrLine1, xyArrLine2;	
	CRebarPlacing rb;
	
	// rebarinfocycle Ư���� ���� ���ؾ� �� ���� �̾Ƽ� ���Ѵ�.
	// 1. �� ö���� ��� ���۰� ���� pos�� ���� ö���϶�.
	if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		if(bParabola)
		{
			// �������� ������ ������ �ִ�. ������
			xyArrLine1.Add(pRC2->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);
			bJoint	= TRUE;
		}
		else
		{
			// ���� �ִ��� �Ǵ�.
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);
			bJoint = rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
		}
	}
	// 2. ù��° ö���� ���۰� ���� ���� �ι�° ö���� �ٸ���
	else if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt != pRC2->m_nPosEnd)
	{
		if(bParabola)
		{
			xyArrLine1.Add(pRC2->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);
			bJoint	= TRUE;
		}
		else
		{
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);

			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.Add(pRC1->m_xyStt);
				xyArrLine1.Add(pRC1->m_xyMid1);

				xyArrLine2.Add(pRC2->m_xyMid2);
				xyArrLine2.Add(pRC2->m_xyEnd);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}
		}
	}
	// 3. ù��° ö���� ���۰� ���� �ٸ��� �ι�° ö���� ������.
	else if(pRC1->m_nPosStt != pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		if(bParabola)
		{
			xyArrLine1.Add(pRC2->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyEnd);
			bJoint	= TRUE;
		}
		else
		{
			// ��, ������ü�� ��ν����갡 ����� �����϶��� ���ܷ� ó����.
			if(pRC1->m_bUseEndExt)
			{
				xyArrLine1.Add(pRC1->m_xyMidExt);
				xyArrLine1.Add(pRC1->m_xyEndExt);
			}
			else
			{
				xyArrLine1.Add(pRC1->m_xyMid2);
				xyArrLine1.Add(pRC1->m_xyEnd);
			}

			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);

			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.RemoveAll();
				if(pRC1->m_bUseEndExt)
				{
					xyArrLine1.Add(pRC1->m_xyMidExt);
					xyArrLine1.Add(pRC1->m_xyEndExt);
				}
				else
				{
					xyArrLine1.Add(pRC1->m_xyMid2);
					xyArrLine1.Add(pRC1->m_xyEnd);
				}

				xyArrLine2.RemoveAll();
				xyArrLine2.Add(pRC2->m_xyStt);
				xyArrLine2.Add(pRC2->m_xyMid1);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}
		}
	}
	// 4. ��ö�ٸ�� ���۰� ���� �ٸ���.
	else
	{
		if(bParabola)
		{
			xyArrLine1.Add(pRC2->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyEnd);
			bJoint	= TRUE;
		}
		else
		{
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);

			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.RemoveAll();
				xyArrLine1.Add(pRC1->m_xyStt);
				xyArrLine1.Add(pRC1->m_xyMid1);

				xyArrLine2.RemoveAll();
				xyArrLine2.Add(pRC2->m_xyMid2);
				xyArrLine2.Add(pRC2->m_xyEnd);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.RemoveAll();
				xyArrLine1.Add(pRC1->m_xyMid2);
				xyArrLine1.Add(pRC1->m_xyEnd);

				xyArrLine2.RemoveAll();
				xyArrLine2.Add(pRC2->m_xyStt);
				xyArrLine2.Add(pRC2->m_xyMid1);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.RemoveAll();
				xyArrLine1.Add(pRC1->m_xyMid2);
				xyArrLine1.Add(pRC1->m_xyEnd);

				xyArrLine2.RemoveAll();
				xyArrLine2.Add(pRC2->m_xyMid2);
				xyArrLine2.Add(pRC2->m_xyEnd);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}

			// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
			if(!bJoint)
			{
				xyArrLine1.RemoveAll();
				xyArrLine1.Add(pRC1->m_xyMidExt);
				xyArrLine1.Add(pRC1->m_xyEndExt);

				xyArrLine2.RemoveAll();
				xyArrLine2.Add(pRC2->m_xyStt);
				xyArrLine2.Add(pRC2->m_xyMid1);
				
				// ���� �ִ��� �Ǵ�.
				bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE, 0.1);
			}
		}
	}


	// �˻簡 �������� ������ �ִ� ��� ������ ������ �����
	if(bJoint)
	{
		// �������ִ� ���� xyArrLine1�� ���� �ȴ�.
		if(xyArrLine1.GetSize() > 1)
		{
			CDPoint xy1	= xyArrLine1.GetAt(0);
			CDPoint xy2	= xyArrLine1.GetAt(1);

			joint.m_xyPos		= (xy2+xy1)/2;
			joint.m_dLength		= ~(xy2-xy1);
			joint.m_xyDir		= (xy2-xy1).Unit();
		}
	}

	return bJoint;
}


// rebarinfocycle �迭���� ���� ������ �����Ѵ�.
void CRcBridgeRebarUtil::GetJointAtRebarInfoCycleArray(CJointArray &arrJoint, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bParabola)
{
	arrJoint.RemoveAll();
	if(!pArrRC) return;

	long nSize	= pArrRC->GetSize();
	for(long rc = 0; rc < nSize-1; rc++)
	{
		CRebarInfoCycle *pRC1	= pArrRC->GetAt(rc);
		CRebarInfoCycle *pRC2	= pArrRC->GetAt(rc+1);
		CRebarJoint joint;
		
		if(GetJointAtRebarInfoCycle(pRC1, pRC2, joint, bParabola))
		{
			// ������ �Ʒ��� �׷����� �Ǵ� ��츦 �Ǵ��ؼ� �������ش�.
			// ù��° ö�� ������ �����̽�����ü �����̰ų� ������ü �����̰ų� ��ν����� �ϸ��϶�...
			if(pRC1->m_nPosEnd == REBARINFO_POS_STTWALL_OUTTER || pRC1->m_nPosEnd == REBARINFO_POS_ENDWALL_INNER 
				|| pRC1->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
			{
				joint.m_bDrawUpper	= FALSE;
			}
			
			arrJoint.Add(joint);
		}
	}
}

void CRcBridgeRebarUtil::AddFromTvArray(CTwinVectorArray &tvArr, CTwinVectorArrayArray *pTvArr, long nIdx)
{
	long nIdxStt	= nIdx == -1 ? 0 : nIdx;
	long nIdxEnd	= nIdx == -1 ? pTvArr->GetSize() : nIdx+1;

	if(nIdx != -1)
	{
		if(pTvArr->GetSize() <= nIdx) return;
	}
	

	long i = 0; for(i = nIdxStt; i < nIdxEnd; i++)
	{
		tvArr.AddFromTvArray(*(pTvArr->GetAt(i)));
	}
}


// �纸��(����) ��ġ �ϰ��
// ���� ö���� ��ȭġ���� ������ ���� ö�ٵ�� �����ش�.
// BOOL bRev : ö���� ��ŷ�� ���ؼ� ������ �ִ� �������� ��Ÿ��.
void CRcBridgeRebarUtil::DivideVertMainRebarBySabogang(CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, BOOL bRev)
{
	long i = 0;
	tvArrLeft.RemoveAll();
	tvArrRight.RemoveAll();

	CTwinVector tvCur, tvNext;
	BOOL bAddLast	= FALSE;
	BOOL bAddNext	= TRUE;
	if(bRev)
	{
		for(i = 0; i < tvArrMain.GetSize()-1; i++)
		{
			tvCur	= tvArrMain.GetAt(i);
			tvNext	= tvArrMain.GetAt(i+1);
			bAddLast	= FALSE;

			if(Compare(tvCur.GetLength(), tvNext.GetLength(), "<", 1))
			{
				tvArrRight.Add(tvCur);
				tvArrMain.RemoveAt(i);
				i--;
				bAddLast	= TRUE;
				bAddNext	= FALSE;
			}
			else if(Compare(tvCur.GetLength(), tvNext.GetLength(), ">", 1))
			{
				tvArrLeft.Add(tvNext);
				tvArrMain.RemoveAt(i+1);
				i--;
				bAddLast	= TRUE;
				bAddNext	= TRUE;
			}
		}


		if(bAddLast && tvArrMain.GetSize() > 0)
		{
			if(bAddNext)
			{
				if(!Compare(tvArrMain.GetAt(0).GetLength(), tvNext.GetLength(), "=", 1) || tvArrMain.GetSize() == 1)
				{
					tvArrRight.Add(tvNext);
					tvArrMain.RemoveAt(tvArrMain.GetSize()-1);
				}
			}
			else
			{
				if(!Compare(tvArrMain.GetAt(0).GetLength(), tvCur.GetLength(), "=", 1) || tvArrMain.GetSize() == 1)
				{
					tvArrLeft.Add(tvCur);
					tvArrMain.RemoveAt(tvArrMain.GetSize()-1);
				}
			}
			
		}
	}
	else
	{
		for(i = 0; i < tvArrMain.GetSize()-1; i++)
		{
			tvCur	= tvArrMain.GetAt(i);
			tvNext	= tvArrMain.GetAt(i+1);
			bAddLast	= FALSE;

			if(Compare(tvCur.GetLength(), tvNext.GetLength(), "<", 1))
			{
 				tvArrLeft.Add(tvCur);
				tvArrMain.RemoveAt(i);
				i--;
				bAddLast	= TRUE;
				bAddNext	= TRUE;
			}
			else if(Compare(tvCur.GetLength(), tvNext.GetLength(), ">", 1))
			{
				tvArrRight.Add(tvNext);
				tvArrMain.RemoveAt(i+1);
				i--;
				bAddLast	= TRUE;
				bAddNext	= FALSE;
			}
		}

		if(bAddLast && tvArrMain.GetSize() > 0)
		{
			if(bAddNext)
			{
				if(!Compare(tvArrMain.GetAt(0).GetLength(), tvNext.GetLength(), "=", 1) || tvArrMain.GetSize() == 1)
				{
					tvArrLeft.Add(tvNext);
					tvArrMain.RemoveAt(tvArrMain.GetSize()-1);
				}
			}
			else
			{
				if(!Compare(tvArrMain.GetAt(0).GetLength(), tvCur.GetLength(), "=", 1) || tvArrMain.GetSize() == 1)
				{
					tvArrRight.Add(tvCur);
					tvArrMain.RemoveAt(tvArrMain.GetSize()-1);
				}
			}
		}
	}
}

// �Ŵ�ö�� �Է°����� ��ǥ ���ϱ�
void CRcBridgeRebarUtil::GetXyDivideLine(CDPointArray &xyArr, CTwinVector tv, double dSttOffset, long nCountMid, double dMidOffset, double dEndOffset)
{
	xyArr.RemoveAll();

	CDPoint xyStt	= tv.m_v1;
	CDPoint xyDir	= tv.GetXyDir();

	xyArr.Add(xyStt);

	if(dSttOffset > 0)
	{
		xyStt += xyDir * dSttOffset;
		xyArr.Add(xyStt);
	}

	long i = 0; for(i = 0; i < nCountMid; i++)
	{
		xyStt += xyDir * dMidOffset;
		xyArr.Add(xyStt);
	}

	if(dEndOffset > 0)
	{
		xyStt += xyDir * dEndOffset;
		xyArr.Add(xyStt);
	}
}

// DWORD CRcBridgeRebarUtil::GetDirectVector(CTwinVector tv, BOOL bRevDir)
// {
// 	CVector vec1	= tv.m_v1;
// 	CVector vec2	= tv.m_v2;
// 
// 	// arc�� ���� �ɷ���
// 	if(Compare(vec1.z, 2.0, "=") || Compare(vec1.z, -2.0, "=") || Compare(vec1.z, -1.0, "=") || Compare(vec1.z, 1.0, "=")) return dwRTArcCW;
// 
// 	CDPoint xy1 = vec1;
// 	CDPoint xy2 = vec2;
// 	double dAngle = (xy2-xy1).Unit().GetAngleDegree();
// 	if(dAngle >= 360) dAngle -= 360;
// 
// 	// ���� �Ǵ��� �ٷ� ����
// 	// ����ȹ��� ���� ����
// 	if(Compare(dAngle, 90.0, "=")) return dwRTTop;
// 	if(Compare(dAngle, 270.0, "=")) return dwRTBottom;
// 
// 	// ���غ� ���� ����
// 	DWORD dir = 0x00000000;
// 	if(!bRevDir)	// ������
// 	{
// 		if(Compare(dAngle, 0.0, "=")) dir = dwRTRight;
// 		else if(Compare(dAngle, 180.0, "=")) dir = dwRTLeft;
// 		else if(dAngle > 0 && dAngle < 90) dir = dwRTRightTop;
// 		else if(dAngle > 90 && dAngle < 180) dir = dwRTLeftTop;
// 		else if(dAngle > 180 && dAngle < 270) dir = dwRTLeftBottom;
// 		else if(dAngle > 270 && dAngle < 360) dir = dwRTRightBottom;
// 	}
// 	else			// ������
// 	{
// 		if(Compare(dAngle, 0.0, "=")) dir = dwRTLeft;
// 		else if(Compare(dAngle, 180.0, "=")) dir = dwRTRight;
// 		else if(dAngle > 0 && dAngle < 90) dir = dwRTLeftTop;
// 		else if(dAngle > 90 && dAngle < 180) dir = dwRTRightTop;
// 		else if(dAngle > 180 && dAngle < 270) dir = dwRTRightBottom;
// 		else if(dAngle > 270 && dAngle < 360) dir = dwRTLeftBottom;
// 	}
// 
// 	return dir;
// }

// ��ν����� �Ϻ� ��±� Ÿ�� ������ ���
void CRcBridgeRebarUtil::SetRebarType()
{
	// 1��
	m_pArrRebarType[1].Add(dwRTRight|dwRTA);

	// 2��
	m_pArrRebarType[2].Add(dwRTBottom|dwRTB);
	m_pArrRebarType[2].Add(dwRTRight|dwRTA);

	// 4��
	m_pArrRebarType[4].Add(dwRTBottom|dwRTB);
	m_pArrRebarType[4].Add(dwRTRight|dwRTA);
	m_pArrRebarType[4].Add(dwRTTop|dwRTC);

	// 6��
	m_pArrRebarType[6].Add(dwRTBottom|dwRTC);
	m_pArrRebarType[6].Add(dwRTArcCW|dwRTB);
	m_pArrRebarType[6].Add(dwRTRight|dwRTA);
	m_pArrRebarType[6].Add(dwRTTop|dwRTD);

	// 6��(��������)
	m_pArrRebarType[6].Add(dwRTBottom|dwRTD);
	m_pArrRebarType[6].Add(dwRTRight|dwRTA);
	m_pArrRebarType[6].Add(dwRTArcCW|dwRTB);
	m_pArrRebarType[6].Add(dwRTTop|dwRTC);
	
	// 7��
	m_pArrRebarType[7].Add(dwRTBottom|dwRTEnableZeroLength|dwRTC);
	m_pArrRebarType[7].Add(dwRTArcCW|dwRTB);
	m_pArrRebarType[7].Add(dwRTRight|dwRTA);
	m_pArrRebarType[7].Add(dwRTArcCW|dwRTB);
	m_pArrRebarType[7].Add(dwRTTop|dwRTEnableZeroLength|dwRTD);

	// 8��
	m_pArrRebarType[8].Add(dwRTRightBottom|dwRTLeftBottom|dwRTBottom|dwRTB);
	m_pArrRebarType[8].Add(dwRTRightBottom|dwRTRightTop|dwRTTop|dwRTA);

	/*
	// 8��
	m_pArrRebarType[8].Add(dwRTRightBottom|dwRTRightTop|dwRTB);
	m_pArrRebarType[8].Add(dwRTTop|dwRTA);
	*/

	// 13��
	m_pArrRebarType[13].Add(dwRTBottom|dwRTA);
	m_pArrRebarType[13].Add(dwRTRight|dwRTB);
	m_pArrRebarType[13].Add(dwRTRightTop|dwRTC);


	// 25��
	m_pArrRebarType[25].Add(dwRTBottom|dwRTA);
	m_pArrRebarType[25].Add(dwRTRightBottom|dwRTB);
	m_pArrRebarType[25].Add(dwRTRight|dwRTC);
	m_pArrRebarType[25].Add(dwRTRightTop|dwRTD);
	m_pArrRebarType[25].Add(dwRTTop|dwRTE);

	// 62��
	m_pArrRebarType[62].Add(dwRTBottom|dwRTB);
	m_pArrRebarType[62].Add(dwRTRightBottom|dwRTA);
	m_pArrRebarType[62].Add(dwRTRightBottom|dwRTRightTop|dwRTC);

	// 68��
	m_pArrRebarType[68].Add(dwRTBottom|dwRTA);
	m_pArrRebarType[68].Add(dwRTRightBottom|dwRTB);
	m_pArrRebarType[68].Add(dwRTRight|dwRTC);
	m_pArrRebarType[68].Add(dwRTTop|dwRTD);

	// 77��
	m_pArrRebarType[77].Add(dwRTBottom|dwRTA);
	m_pArrRebarType[77].Add(dwRTArcCW|dwRTB);
	m_pArrRebarType[77].Add(dwRTRightBottom|dwRTRightTop|dwRTC);
}

// BOOL CRcBridgeRebarUtil::GetRebarType(CRebar *pRB, CTwinVectorArray &tvArr, CTwinVectorArray &retTvArr, CDDWordArray &retDwArr)
// {
// 	CRebarPlacing rb;
// 	// ö�� ����Ÿ �ʱ�ȭ
// 	pRB->InitData();
// 
// 	CDDWordArray pRT;
// 	CDoubleArray dArrZeroLength;		// ���� �������� 0�̾��� ������ �ε����� ����.
// 
// 	CTwinVector tv;
// 	long nFindType = -1;	// ã�� Ÿ��
// 	BOOL bSame = TRUE;
// 	BOOL bFindRevType	= FALSE;
// 	// ���������� �ѹ� �˻��ϰ�..
// 	DWORD	dwVecAttr = 0x00000000;
// 	DWORD	dwAttr = 0x00000000;
// 	BOOL bOKDir = FALSE;
// 
// 	CTwinVectorArray tvArrTmp;
// 	tvArrTmp	= tvArr;
// 
// 	// �տ��� �ڷ� �ѹ� �ڿ��� ������ �ѹ��� �˻���.
// 	long nIdxTv	= 0;
// 
// 	long j = 0; for(j = 0; j < 2; j++)
// 	{
// 		// j == 0�̸� ���͹����� ������ 1�̸� ������
// 		if(j == 1) 
// 		{
// 			rb.ReverseRebar(tvArrTmp);
// 		}
// 
// 		for(long k = 0; k < 2; k++)
// 		{
// 			if(nFindType == -1)
// 			{
// 				// ���͹��� ���������� �ѹ� �˻�.
// 				long i = 0; for(i = 0; i < REBARTYPE_SU; i++)
// 				{
// 					pRT = m_pArrRebarType[i];
// 
// 					// ö�� Ÿ���� ����� �ѹ� �˻�.
// 					if(k == 1) 
// 					{
// 						InvertRebarType(pRT);	// �ι�°�� Ÿ���� ����� �˻�.
// 						bFindRevType	= TRUE;
// 					}
// 					else
// 						bFindRevType	= FALSE;
// 
// 					// ó������ �Ӽ��� ������ ������ �ٸ��� �˻����� ����
// 					//if(pRT.GetSize() != vecArrTmp.GetSize()/2) continue;
// 					if(pRT.GetSize() < tvArrTmp.GetSize()) continue;
// 
// 					bSame	= TRUE;
// 					nIdxTv	= 0;
// 					dArrZeroLength.RemoveAll();
// 
// 					long l = 0;
// 					for(l = 0; l < pRT.GetSize(); l++)
// 					{
// 						dwAttr = (DWORD)pRT.GetAt(l);
// 						if(nIdxTv >= tvArrTmp.GetSize()) break;
// 						tv	= tvArrTmp.GetAt(nIdxTv);
// 						nIdxTv++;
// 
// 						dwVecAttr = GetDirectVector(tv, FALSE);
// 						bOKDir = dwVecAttr & dwAttr;
// 
// 						// ö�������� ��ġ ���� ���� ���.
// 						// ���� ���̰� 0���� �Ǵ��Ѵ�.
// 						if(!bOKDir)
// 						{
// 							// ���� ���̰� 0�� �� �ִٸ� ���� ������ �˻��Ѵ�.
// 							if(dwAttr & dwRTEnableZeroLength)
// 							{
// 								// ���̰� 0�̾��� ���� �ε����� ����.
// 								// ���߿� ���� ���� �����Ҷ� ����ϱ� ����.
// 								dArrZeroLength.Add(l);	
// 
// 								if(l == pRT.GetSize() -1) 
// 								{
// 									bOKDir = TRUE;
// 									continue;
// 								}
// 								else
// 								{
// 									l++;
// 									dwAttr = (DWORD)pRT.GetAt(l);
// 
// 									dwVecAttr = GetDirectVector(tv, FALSE);
// 									bOKDir = dwVecAttr & dwAttr;
// 
// 									if(bOKDir) continue;
// 								}
// 							}
// 							bSame = FALSE;
// 							break;
// 						}
// 					}
// 
// 					// ���´µ�... ���� �˻��� ����Ÿ���� �������� �̰� ��ġ�ϴ°� �ƴϴ�.
// 					for(long t = l; t < pRT.GetSize(); t++)
// 					{
// 						DWORD dw = (DWORD)pRT[t];
// 						if(!(dw & dwRTEnableZeroLength)) bSame = FALSE;
// 					}
// 
// 					// ��� ��ġ ���� ���..
// 					if(bSame)
// 					{
// 						// �����ؾߵ� ���� ������ � ���� �ľ��ؼ� �����ؾ� �� ����Ÿ ����  /////////
// 						// vector ������ ������ ����.
// 						long nSize = 0;
// // 						DWORD dwDir = 0x00000000; 
// 						for(long c = 0; c < pRT.GetSize(); c++)
// 						{
// 							DWORD dwDir = (DWORD)pRT.GetAt(c);
// 							if(!(dwDir & dwRTEnableZeroLength)) nSize++;
// 						}
// 
// 						if(nSize-dArrZeroLength.GetSize() > tvArrTmp.GetSize())
// 						{
// 							bSame = FALSE;
// 							continue;
// 						}
// 						////////////////////////////////////////////////////////////////////////////////
// 							
// 						nFindType = i;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 
// 		// ���ϴ� Ÿ���� ã�� ������ ���̻� ã�� �ʴ´�.
// 		if(bSame) break;
// 	}
// 
// 
// 
// 	// �˻����� �����ϰ� �����Ѵ�.
// 	// ���������� dwRTStaticLength�� �Ȱ��� �ǳʶ�� ����Ÿ�� ������.
// 	// ������� 5������ ���� ����.
// 	if(nFindType > 0)
// 	{
// 		pRT = m_pArrRebarType[nFindType];
// 
// 		// �����ؾߵ� ���� ������ � ���� �ľ�
// 		long nSize = 0;
// //		DWORD dwDir = 0x00000000;
// 		long i = 0; for(i = 0; i < pRT.GetSize(); i++)
// 		{
// 			DWORD dwDir = (DWORD)pRT.GetAt(i);
// 			if(!(dwDir & dwRTStaticLength)) nSize++;
// 		}
// 		
// 		CVector vec1(0, 0, 0);
// 		CVector vec2(0, 0, 0);
// 		CDoubleArray dArrLen;
// 		CDoubleArray dArrR;
// 		CDDWordArray dwPos;
// 
// 		// ���Ϳ��� �������� ��� ����
// 		for(i = 0; i < tvArrTmp.GetSize(); i++)
// 		{
// 			dArrLen.Add(tvArrTmp.GetAt(i).GetLength());
// 			dArrR.Add(fabs(tvArrTmp.GetAt(i).m_v2.z));
// 		}
// 
// 		// ������ ������ ���·� ��ġ �Ȱ�� ������ �ٽ� ������.
// 		if(bFindRevType) 
// 			InvertRebarType(pRT);
// 
// 		// ���Ͽ� ���缭 ���̰� 0�ΰ��� 0�� �߰����ְ�.
// 		for(i = 0; i < dArrZeroLength.GetSize(); i++)
// 		{
// 			dArrLen.InsertAt((int)dArrZeroLength[i], 0);
// 			dArrR.InsertAt((int)dArrZeroLength[i], 0);
// 		}
// 
// 
// 		// ������ ���� ���� ///////////////////
// 		retTvArr = tvArrTmp;
// 		retDwArr= pRT;
// 		///////////////////////////////////////
// 
// 
// 		
// 		// ������ ���̴� ������Ű��
// 		for(i = 0; i < pRT.GetSize(); i++)
// 		{
// 			DWORD dwType = (DWORD)pRT[i];
// 
// 			if(dwType & dwRTStaticLength)
// 			{
// 				dArrLen.RemoveAt(i);
// 				pRT.RemoveAt(i);
// 				i--;
// 			}
// 			else
// 			{
// 				dwPos.Add(dwType);
// 			}
// 		}
// 
// 		// ������ �������� ��� ����� �����ϱ� ���� ���� ������ 
// 		// �ٽ� �����´�.
// 		if(bFindRevType)
// 		{
// 			CDoubleArray dArrTmp;
// 			CDoubleArray dArrRTmp;
// 			CDDWordArray dwPosTmp;
// 
// 			for(i = dArrLen.GetSize()-1 ; i >= 0; i--)
// 				dArrTmp.Add(dArrLen[i]);
// 			dArrLen = dArrTmp;
// 
// 			for(i = dArrR.GetSize()-1; i >= 0; i--)
// 				dArrRTmp.Add(dArrR[i]);
// 			dArrR	= dArrRTmp;
// 
// 			for(i = dwPos.GetSize()-1; i >= 0; i--)
// 				dwPosTmp.Add(dwPos[i]);
// 			dwPos = dwPosTmp;
// 		}
// 
// 		// ���� ���� ����
// 		for(i = 0; i < dArrLen.GetSize(); i++)
// 		{
// 			if(dwPos.GetSize() > i)
// 			{
// 				DWORD dwType = (DWORD)dwPos[i];
// 
// 				if(dwType & dwRTA) 
// 					pRB->m_dLengthMax_A	= dArrLen.GetAt(i);
// 				if(dwType & dwRTB) 
// 					pRB->m_dLengthMax_B	= dArrLen.GetAt(i);
// 				if(dwType & dwRTArcCW)
// 					pRB->m_R	= dArrR.GetAt(i);
// 				if(dwType & dwRTC) 
// 					pRB->m_dLengthMax_C	= dArrLen.GetAt(i);
// 				if(dwType & dwRTD) 
// 					pRB->m_dLengthMax_D	= dArrLen.GetAt(i);
// 				if(dwType & dwRTE) 
// 					pRB->m_E			= dArrLen.GetAt(i);
// 			}
// 		}
// 		
// 		pRB->m_nTypeRebar = nFindType;
// 	}
// 	
// 	return TRUE;
// }




// ö��Ÿ�� ������
// ö�� Ÿ���� ������ �� Ÿ�� �迭�� �ݴ�� �ȴ�.
// ����� �ݴ��̰� ��ǥ������ �������� ���·� �ٲ�.
// void CRcBridgeRebarUtil::InvertRebarType(CDDWordArray &arrWord)
// {
// 	CDDWordArray arrTmp;
// // 	DWORD dw;
// // 	DWORD dwTmp;
// 	long i = 0; for(i = arrWord.GetSize()-1; i >= 0; i--)
// 	{
// 		DWORD dw = (DWORD)arrWord[i];
// 		DWORD  dwTmp = dw;
// 		if(dw & dwRTRight)		
// 		{
// 			dwTmp &= dwRTAllDir-dwRTRight; 
// 			dwTmp |= dwRTRight;
// 		}
// 		if(dw & dwRTRightTop && !(dw & dwRTRightBottom))		
// 		{
// 			dwTmp &= dwRTAllDir-dwRTRightTop;
// 			dwTmp |= dwRTRightBottom;
// 		}
// 		if(dw & dwRTTop && !(dw & dwRTBottom))			
// 		{
// 			dwTmp &= dwRTAllDir-dwRTTop;
// 			dwTmp |= dwRTBottom;
// 		}
// 		if(dw & dwRTLeftTop && !(dw & dwRTLeftBottom))		
// 		{
// 			dwTmp &= dwRTAllDir-dwRTLeftTop;
// 			dwTmp |= dwRTLeftBottom;
// 		}
// 		if(dw & dwRTLeftBottom && !(dw & dwRTLeftTop))	
// 		{
// 			dwTmp &= dwRTAllDir-dwRTLeftBottom;
// 			dwTmp |= dwRTLeftTop;
// 		}
// 		if(dw & dwRTBottom && !(dw & dwRTTop))		
// 		{
// 			dwTmp &= dwRTAllDir-dwRTBottom;
// 			dwTmp |= dwRTTop;
// 		}
// 		if(dw & dwRTRightBottom && !(dw & dwRTRightTop))	
// 		{
// 			dwTmp &= dwRTAllDir-dwRTRightBottom;
// 			dwTmp |= dwRTRightTop;
// 		}
// 
// 		arrTmp.Add(dwTmp);
// 	}
// 
// 	arrWord.RemoveAll();
// 
// 	for(i = 0; i < arrTmp.GetSize(); i++)
// 		arrWord.Add(arrTmp.GetAt(i));
// }

// rc�� ���۰� �� sta�� �Ǵ�
double CRcBridgeRebarUtil::GetStationRebarInfoCycle(CRebarInfoCycle *pRC, BOOL bStt)
{
	if(!pRC) return 0;

	double dStaStt	= 0;
	double dStaEnd	= 0;
	if(pRC->m_bUseTvArrRebar)
	{
		CDRect rect	= pRC->m_tvArrRebar.GetRect();
		dStaStt	= rect.left;
		dStaEnd	= rect.right;
	}
	else
	{
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
			dStaStt	= pRC->m_xyStt.x;
			dStaEnd	= pRC->m_xyMid1.x;
		}
		else
		{
			dStaStt	= pRC->m_xyStt.x;
			dStaEnd	= pRC->m_xyEnd.x;
		}
	}

	return bStt ? dStaStt : dStaEnd;
}


// ö�� ��ǥ �迭�κ��� nCount�� °�� ��ġ�� ���Ѵ�.
// bHor : ���� ö��(y��ǥ ����)���� ���� ö��(x��ǥ ����)����..
double CRcBridgeRebarUtil::GetPosRebarMark(CTwinVectorArray &tvArr, BOOL bHor, int nCount)
{
	double dCoorCurr = 0;
	double dCoorNext = 0;

	int nSize = tvArr.GetSize();
	if(nSize>0 && nCount>=nSize)
		return (bHor ? tvArr.GetAt(nSize-1).m_v1.y : tvArr.GetAt(nSize-1).m_v1.x);

	int i=0; for(i=0; i<nSize-1; i++)
	{
		dCoorCurr = bHor ? tvArr.GetAt(i).m_v1.y : tvArr.GetAt(i).m_v1.x;
		dCoorNext = bHor ? tvArr.GetAt(i+1).m_v1.y : tvArr.GetAt(i+1).m_v1.x;
		if(nCount==i+1)
			break;
	}

	if(nSize == 1) dCoorCurr = dCoorNext = bHor ? tvArr.GetAt(0).m_v1.y : tvArr.GetAt(0).m_v1.x;

	return (dCoorCurr+dCoorNext)/2;
}

double CRcBridgeRebarUtil::GetPosRebarMark(vector<double> &vec, int nCount)
{
	double dCoorCurr = 0;
	double dCoorNext = 0;

	int nSize = vec.size();
	if(nSize>0 && nCount>=nSize)
		return vec.at(nSize-1);

	int i=0; for(i=0; i<nSize-1; i++)
	{
		dCoorCurr = vec.at(i);
		dCoorNext = vec.at(i+1);
		if(nCount==i+1)
			break;
	}

	return (dCoorCurr+dCoorNext)/2;
}

// DimArray�κ��� nCount�� °�� ��ġ�� ���Ѵ�.
// bBet : ö���� ���̰� ���� ����
double CRcBridgeRebarUtil::GetPosRebarMark(CDPointArray &xyArr, int nCount, BOOL bBet)
{
	double dCoorCurr = 0;
	double dCoorNext = 0;

	vector<double> vec;
	CDPoint xyR(0, 0);
	double dDist = 0;
	int i=0; for(i=0; i<xyArr.GetSize(); i++)
	{
		xyR = xyArr.GetAt(i);
		int j=0; for(j=0; j<xyR.x; j++)
		{
			dDist += xyR.y;
			vec.push_back(dDist);
		}
	}

	int nSize = vec.size();
	if(nSize>0 && nCount>=nSize)
		return vec.at(nSize-1);

	for(i=0; i<nSize-1; i++)
	{
		dCoorCurr = vec.at(i);
		dCoorNext = vec.at(i+1);
		if(nCount==i+1)
			break;
	}

	if(bBet)
		return (dCoorCurr+dCoorNext)/2;
	return dCoorCurr;
}

void CRcBridgeRebarUtil::RemoveNotExistRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
{
	if(!pArrRC) return;

	long i = 0; for(i = 0; i < pArrRC->GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		if(!pRC->m_bExist)
		{
			delete pRC;
			pArrRC->RemoveAt(i);
			i--;
		}
	}
}


// Ŀ�÷��� ���� ������ ���Ŀ� �߰�...
void CRcBridgeRebarUtil::SetLengthRebar(CRebar *pRB, long nJointType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	pRB->m_dLengthMax_A = Round(pRB->m_dLengthMax_A,0);
	pRB->m_dLengthMin_A = Round(pRB->m_dLengthMin_A,0);
	pRB->m_dLengthMax_B = Round(pRB->m_dLengthMax_B,0);
	pRB->m_dLengthMin_B = Round(pRB->m_dLengthMin_B,0);
	pRB->m_dLengthMax_C = Round(pRB->m_dLengthMax_C,0);
	pRB->m_dLengthMin_C = Round(pRB->m_dLengthMin_C,0);
	pRB->m_dLengthMax_D = Round(pRB->m_dLengthMax_D,0);
	pRB->m_dLengthMin_D = Round(pRB->m_dLengthMin_D,0);
	pRB->m_E = Round(pRB->m_E,0);

	double dLength = pRB->GetLength(FALSE);
	if(m_pARcBridgeDataStd && dLength>m_pARcBridgeDataStd->m_dLengthRebarMax)
	{
		if(m_pARcBridgeDataStd->IsLsdDesign())
		{
			pRB->m_dJointLength = m_pARcBridgeDataStd->m_pBasicConcInfo->GetValueJoint(pRB->m_dDia, pRB->m_pOptionLsd);
		}
		else
		{
			pRB->m_dJointLength = m_pARcBridgeDataStd->GetValueJoint(pRB->m_dDia,nJointType,toCm(dCover), toCm(dRebarSpace), nBeta, nIp);
		}
		double dInitLen   = m_pARcBridgeDataStd->m_dLengthRebarMax;
// 		double dMaxLength = 0;
		double dLengthRebarMax = m_pARcBridgeDataStd->m_dLengthRebarMax;

		long nJointEa = 0;
		for(nJointEa = 2; nJointEa<21; nJointEa++)		//���� �������� 20���� ���� �ʰ���..
		{
			double dMaxLength = dLengthRebarMax*nJointEa - pRB->m_dJointLength*(nJointEa-1);
			
			if(dLength>dInitLen && dLength<dMaxLength)
				break;
			dInitLen = dMaxLength;
		}
		pRB->m_nJoint = nJointEa-1;
	}
	else
	{
		pRB->m_dJointLength = 0;
		pRB->m_nJoint	    = 0;
	}
	if(pRB->m_dLengthMax_A==pRB->m_dLengthMin_A) pRB->m_dLengthMin_A = 0;
	if(pRB->m_dLengthMax_B==pRB->m_dLengthMin_B) pRB->m_dLengthMin_B = 0;
	if(pRB->m_dLengthMax_C==pRB->m_dLengthMin_C) pRB->m_dLengthMin_C = 0;
	if(pRB->m_dLengthMax_D==pRB->m_dLengthMin_D) pRB->m_dLengthMin_D = 0;
}

// sMark���� ö�� ��ȣ�� ������
long CRcBridgeRebarUtil::GetNumMark(CString sMark)
{
	long nIdxDash	= sMark.Find("-", 0);
	if(nIdxDash > -1)
	{
		sMark.Delete(nIdxDash, sMark.GetLength()-nIdxDash);
	}

	long i = 0; for(i = 0; i < sMark.GetLength(); i++)
	{
		if(sMark.GetAt(i) < '0' || sMark.GetAt(i) > '9')
		{
			sMark.Delete(i);
			i--;
		}
	}

	long nNum	= atol(sMark);

	return nNum;
}

// ��鼱���� �����̼ǰ� dist ��ö��
void CRcBridgeRebarUtil::GetTvArrUpperSlabByDrawLeft_MainRebar(CTwinVectorArray &tvArr, long nDrawLeft)
{
	if(nDrawLeft == 0) return;

	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
	{
		tv	= tvArr.GetAt(i);
		if(tv.m_v1.z == 6 && tv.m_v2.z != nDrawLeft)
		{
			tvArr.RemoveAt(i);
			i--;
		}
	}
}


// rebar info cycle �ΰ��� ��ģ��.
void CRcBridgeRebarUtil::MergeRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nIdx1, long nIdx2)
{
	long nSize = pArrRC->GetSize();
	if(nIdx1 == nIdx2) return;
	if(nIdx1 < 0 || nIdx1 > nSize-1) return;
	if(nIdx2 < 0 || nIdx2 > nSize-1) return;
	if(nIdx1 > nIdx2) GetSwap(nIdx1, nIdx2);

	CRebarInfoCycle *pRC1	= pArrRC->GetAt(nIdx1);
	CRebarInfoCycle *pRC2	= pArrRC->GetAt(nIdx2);

	// �� ö�� ��� ���� ������ �����ؼ� ���������� ������..
	if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		pRC1->m_xyMid1	= pRC2->m_xyMid1;
		pRC1->m_xyMid2	= pRC2->m_xyMid2;
		pRC1->m_xyEnd	= pRC2->m_xyEnd;
	}
	// ù��° ö���� ���۰� ���� �ٸ���, �ι�° ö���� ����
	else if(pRC1->m_nPosStt != pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		pRC1->m_xyEnd = pRC2->m_xyMid1;
	}
	// ù��° ö���� ����, �ι�° ö���� �ٸ�
	else if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt != pRC2->m_nPosEnd)
	{
		pRC1->m_xyMid1	= pRC2->m_xyMid1;
		pRC1->m_xyMid2	= pRC2->m_xyMid2;
		pRC1->m_xyEnd	= pRC2->m_xyEnd;
		pRC1->m_nPosEnd	= pRC2->m_nPosEnd;
		pRC1->m_xyCen	= pRC2->m_xyCen;
		pRC1->m_R_Mid	= pRC2->m_R_Mid;
	}
	// ù��° �ι�° ��� ���۰� ���� �ٸ�(��ν����� �ܰ氣 2,4cycle�� ���)
	else if(pRC1->m_nPosStt != pRC1->m_nPosEnd && pRC2->m_nPosStt != pRC2->m_nPosEnd)
	{
		pRC1->m_xyEnd		= pRC2->m_xyMid1;
		pRC1->m_xyMidExt	= pRC2->m_xyMid2;
		pRC1->m_xyEndExt	= pRC2->m_xyEnd;
		pRC1->m_xyCenExt	= pRC2->m_xyCen;
		pRC1->m_R_MidExt	= pRC2->m_R_Mid;
		pRC1->m_bUseCenExt	= TRUE;
		pRC1->m_bUseEndExt	= TRUE;
		pRC1->m_bUseMidExt	= TRUE;
		pRC1->m_nPosEnd		= pRC2->m_nPosEnd;
	}

	pArrRC->SetAt(nIdx1, pRC1);
	delete pArrRC->GetAt(nIdx2);
	pArrRC->RemoveAt(nIdx2, 1);
}


void CRcBridgeRebarUtil::GetTvRebarGuardWall(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale, BOOL bCover)
{
	if((pGW->IsTypeHDanRoadGuardFence() || (pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof) || pGW->IsTypeHDanRailGuardFence())
		GetTvRebarGuardFence(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof)
	{
		if(pGW->m_bGWhalf)
			GetTvRebarCenterGuard(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
		else
			GetTvRebarCenterGuard2(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	}
	else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
		GetTvRebarCenterGuardHalf(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
	{
		GetTvRebarHandRail(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	}
	else if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
		GetTvRebarSoundProof(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	else if(pGW->IsTypeHDanRoadRetainingWall())
		GetTvRebarRetainingWall(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale);
	else if(pGW->IsTypeHDanRailFootwayBlock())
		GetTvRebarFootwayBlock(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale, bCover);
	else if(pGW->IsTypeHDanRailTotalHole())
		GetTvRebarTotalHole(pGW, tvArr, tvArrRebar, xyPointRebar, dDomScale, bCover);
}

void CRcBridgeRebarUtil::GetTvRebarGuardFence(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	CTwinVector tv;
	CTwinVectorArray tvArrLeft, tvArrTop, tvArrRight, tvArrRight2, tvArrHor;
	tvArrLeft.RemoveAll();
	tvArrTop.RemoveAll();
	tvArrRight.RemoveAll();
	tvArrRight2.RemoveAll();
	tvArrHor.RemoveAll();

	BOOL bRailFence = pGW->IsTypeHDanRailGuardFence();

	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;
	double	dH1 = pGW->m_dH1;
	double	dH2 = pGW->m_dH2;
	double	dH3 = pGW->m_dH3;
	double	dW1 = pGW->m_dW1;
	double	dW2 = pGW->m_dW2;
	double	dW4 = pGW->m_dW4;
	double	dW5 = pGW->m_dW5;
	double	dF1 = pGW->m_dF1;
	double	dF2 = 0;
	double	dF3 = 0;
	double	dL1 = bLeft ? pGW->m_dL1 : -pGW->m_dL1;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	
	double	dTop	= 100;
	CDPoint xyResult(0,0);
	CDPoint vL1(0,0), vL2(0,0), vL3(0,0), vL4(0,0);
	
	// ���߶��
	pGW->m_dCrossF2 = 150;
	pGW->m_dCrossF3 = 200;

	long	nSize	= tvArr.GetSize();
	CDPoint xyFirst =  tvArr.GetAt(1).m_v1;
	CDPoint xyLast = tvArr.GetAt(nSize-1).m_v2;

	CDPoint	vOrigin = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v1;
	if(xyLast.y > xyFirst.y)
		vOrigin.y = xyLast.y;
	double	dEndY	= vOrigin.y - dF1;	

	// Left Rebar_
	long nStt = 1;
	if(dH3!=0)	nStt = 5;
	if(dH2!=0)	nStt = 3;
	if(dH1!=0)	nStt = 1;	

	vL1 = tvArr.GetAt(nStt).m_v1;
	vL2 = tvArr.GetAt(nStt).m_v2;
	Offset(vL1,vL2,dOffset2);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);

	if( (dH1==0&&dH2==0&&!bLeft) || (dH2==0&&dH3==0&&!bLeft) || (dH1==0&&dH3==0&&!bLeft) )
	{
		CDPoint xyResult2(0,0);
		vL1 = tvArr.GetAt(nStt).m_v1;
		vL2 = tvArr.GetAt(nStt).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = CDPoint(0, dEndY);
		vL4 = CDPoint(1, dEndY);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult2);
		// Line 1
		tv.m_v1 = CDPoint(xyResult2.x+dL1, dEndY);
		tv.m_v2 = CDPoint(xyResult2.x, dEndY);
		tv.m_sInfo	= "leftrebar:line1";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
		// Line 2
		tv.m_v1 = tv.m_v2;
		tv.m_v2 = xyResult;	// ������� ��ٰ��� ����
		tv.m_sInfo	= "leftrebar:line2";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
	}
	else
	{
		// Line 1
		tv.m_v1 = CDPoint(xyResult.x+dL1, dEndY);
		tv.m_v2 = CDPoint(xyResult.x, dEndY);
		tv.m_sInfo	= "leftrebar:line1";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
		// Line 2
		tv.m_v1 = CDPoint(xyResult.x, dEndY);
		tv.m_v2 = xyResult;	// ������� ��ٰ��� ����
		tv.m_sInfo	= "leftrebar:line2";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
	}

	// Line 3
	if(dH1>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(1).m_v1;
		vL2 = tvArr.GetAt(1).m_v2;
		Offset(vL1,vL2,dOffset2);
		if(dH2>0)
		{
			vL3 = tvArr.GetAt(3).m_v1;
			vL4 = tvArr.GetAt(3).m_v2;
			Offset(vL3,vL4,dOffset2);
		}
		else if(dH3>0)
		{
			vL3 = tvArr.GetAt(5).m_v1;
			vL4 = tvArr.GetAt(5).m_v2;
			Offset(vL3,vL4,dOffset2);
		}
		else
		{
			vL3 = tvArr.GetAt(7).m_v1;
			vL4 = tvArr.GetAt(7).m_v2;
			Offset(vL3,vL4,dOffset1);
		}
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vL2==vL3 ? vL2 : xyResult;
		tv.m_sInfo	= "leftrebar:line3";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
	}
	// Line 4
	if(dH2>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(3).m_v1;
		vL2 = tvArr.GetAt(3).m_v2;
		Offset(vL1,vL2,dOffset2);
		if(dH3>0)
		{
			vL3 = tvArr.GetAt(5).m_v1;
			vL4 = tvArr.GetAt(5).m_v2;
			Offset(vL3,vL4,dOffset2);
		}
		else
		{
			vL3 = tvArr.GetAt(7).m_v1;
			vL4 = tvArr.GetAt(7).m_v2;
			Offset(vL3,vL4,dOffset1);
		}
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vL2==vL3 ? vL2 : xyResult;
		tv.m_sInfo	= "leftrebar:line4";
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
	}
	// Line 5
	if(dH3>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tv.m_sInfo	= "leftrebar:line5";
		if(!bLeft && pGW->m_nTypeRebar == 1)
		{
			CTwinVector tvLower	= tvArrRebar.GetTvByInfo("leftrebar:line1");
			GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tv.m_v1, tv.GetXyDir(), tv.m_v1);
		}
		tvArrLeft.Add(tv);
		tvArrRebar.Add(tv);
	}



	// Top Rebar_
	// Line 1
	vL3 = tvArr.GetAt(7).m_v1;
	vL4 = tvArr.GetAt(7).m_v2;
	Offset(vL3,vL4,dOffset1+dTop);
	GetXyMatchSegAndSeg(tv.m_v1,tv.m_v2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;
	tv.m_sInfo	= "toprebar:line1";
	tvArrTop.Add(tv);
	tvArrRebar.Add(tv);
	// Line 2
	tv.m_v1 = tv.m_v2;
	vL1 = tvArr.GetAt(7).m_v1;
	vL2 = tvArr.GetAt(7).m_v2;
	Offset(vL1,vL2,dOffset1);
	if(dH3>0)
	{
		vL3 = tvArr.GetAt(9).m_v1;
		vL4 = tvArr.GetAt(9).m_v2;
	}
	else if(dH2>0)
	{
		vL3 = tvArr.GetAt(11).m_v1;
		vL4 = tvArr.GetAt(11).m_v2;
	}
	else
	{
		vL3 = tvArr.GetAt(13).m_v1;
		vL4 = tvArr.GetAt(13).m_v2;
	}
	Offset(vL3,vL4,dOffset1);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tv.m_sInfo	= "toprebar:line2";
	tvArrTop.Add(tv);
	tvArrRebar.Add(tv);
	// Line 3
	tv.m_v1 = tv.m_v2;
	vL1 = tvArr.GetAt(7).m_v1;
	vL2 = tvArr.GetAt(7).m_v2;
	Offset(vL1,vL2,dOffset1+dTop);
	GetXyMatchSegAndSeg(vL3,vL4,vL1,vL2,xyResult);
	tv.m_v2 = xyResult;
	tv.m_sInfo	= "toprebar:line3";
	tvArrTop.Add(tv);
	tvArrRebar.Add(tv);


	// Right Rebar_
	long nEnd = 13;
	if(dH3!=0)	nEnd = 9;
	if(dH2!=0)	nEnd = 11;
	if(dH1!=0)	nEnd = 13;	

	vL1 = tvArr.GetAt(nEnd).m_v1;
	vL2 = tvArr.GetAt(nEnd).m_v2;
	Offset(vL1,vL2,dOffset3);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);

	// ������ �ʰ� �ܺ��� ���� �״�� �����´�.
	CDPoint xyResult2(0,0);
	vL1 = tvArr.GetAt(nEnd).m_v1;
	vL2 = tvArr.GetAt(nEnd).m_v2;
	Offset(vL1,vL2,dOffset2);
	vL3 = CDPoint(0, dEndY);
	vL4 = CDPoint(1, dEndY);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult2);
	// Line 1
	tv.m_v1 = CDPoint(xyResult2.x+dL1, dEndY);
	tv.m_v2 = CDPoint(xyResult2.x, dEndY);
	tv.m_sInfo	= "rightrebar:line1";
	tvArrLeft.Add(tv);
	tvArrRebar.Add(tv);
	// Line 2
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = xyResult;	// ������� ��ٰ��� ����
	tv.m_sInfo	= "rightrebar:line2";
	tvArrLeft.Add(tv);
	tvArrRebar.Add(tv);


	// Line 3
	if(dH1>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(nEnd).m_v1;
		vL2 = tvArr.GetAt(nEnd).m_v2;
		Offset(vL1,vL2,dOffset3);
		if(dH2>0)
		{
			vL3 = tvArr.GetAt(11).m_v1;
			vL4 = tvArr.GetAt(11).m_v2;
			Offset(vL3,vL4,dOffset3);
		}
		else if(dH3>0)
		{
			vL3 = tvArr.GetAt(9).m_v1;
			vL4 = tvArr.GetAt(9).m_v2;
			Offset(vL3,vL4,dOffset3);
		}
		else
		{
			vL3 = tvArr.GetAt(7).m_v1;
			vL4 = tvArr.GetAt(7).m_v2;
			Offset(vL3,vL4,dOffset1);
		}
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vL1==vL4 ? vL1 : xyResult;
		tv.m_sInfo	= "rightrebar:line3";
		tvArrRight.Add(tv);
		tvArrRebar.Add(tv);
	}
	// Line 4
	if(dH2>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(11).m_v1;
		vL2 = tvArr.GetAt(11).m_v2;
		Offset(vL1,vL2,dOffset3);
		if(dH3>0)
		{
			vL3 = tvArr.GetAt(9).m_v1;
			vL4 = tvArr.GetAt(9).m_v2;
			Offset(vL3,vL4,dOffset3);
		}
		else
		{
			vL3 = tvArr.GetAt(7).m_v1;
			vL4 = tvArr.GetAt(7).m_v2;
			Offset(vL3,vL4,dOffset1);
		}
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vL1==vL4 ? vL1 : xyResult;
		tv.m_sInfo	= "rightrebar:line4";
		tvArrRight.Add(tv);
		tvArrRebar.Add(tv);
	}
	// Line 5
	if(dH3>0)
	{
		tv.m_v1 = tv.m_v2;
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dOffset3);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		if(bLeft && pGW->m_nTypeRebar == 1)
		{
			CTwinVector tvLower	= tvArrRebar.GetTvByInfo("rightrebar:line1");
			GetXyMatchLineAndLine(tvLower.m_v1, CDPoint(1, 0), tv.m_v1, tv.GetXyDir(), tv.m_v1);
		}
		tv.m_sInfo	= "rightrebar:line5";
		tvArrRight.Add(tv);
		tvArrRebar.Add(tv);
	}

	// ���� Rebar_
	if((dH1>0 && dH2>0 && dH3>0) && ((dW1>0 && dW2>0) || (dW4>0 && dW5>0)))
	{
		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(13).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);

		// Line 1
		tv.m_v1 = CDPoint(xyResult.x+dL1, dEndY);
		tv.m_v2 = CDPoint(xyResult.x, dEndY);
		tv.m_sInfo	= "����rebar:line1";
		tvArrRight2.Add(tv);
		tvArrRebar.Add(tv);
		// Line 2
		tv.m_v1 = CDPoint(xyResult.x, dEndY);
		tv.m_v2 = xyResult;	// ������� ��ٰ��� ����
		tv.m_sInfo	= "����rebar:line2";
		tvArrRight2.Add(tv);
		tvArrRebar.Add(tv);
		// Line 3
		tv.m_v1 = tv.m_v2;
		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL4 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		pGW->m_dCrossF2 = xyResult.y - vOrigin.y;
		tv.m_sInfo	= "����rebar:line3";
		tvArrRight2.Add(tv);
		tvArrRebar.Add(tv);

		// Line 4
		tv.m_v1 = tv.m_v2;
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
		vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		pGW->m_dCrossF3 = xyResult.y - tv.m_v2.y;
		tv.m_v2 = xyResult;

		if(pGW->m_nTypeRebar == 0)
		{
			double dExtend = 100;
			CDPoint vExtend = (tv.m_v2-tv.m_v1).Unit() * dExtend;
			tv.m_v2 += vExtend;
		}
		else
		{
			if(bLeft)
			{
				CTwinVector tvLeft	= tvArrLeft.GetTvByInfo("leftrebar:line2");
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), tv.m_v1, tv.GetXyDir(), tv.m_v2);
			}
			else
			{
				CTwinVector tvRight	= tvArrLeft.GetTvByInfo("rightrebar:line1");
				GetXyMatchLineAndLine(tvRight.m_v2, CDPoint(0, 1), tv.m_v1, tv.GetXyDir(), tv.m_v2);
			}
		}
		tv.m_sInfo	= "����rebar:line4";
		tvArrRight2.Add(tv);
		tvArrRebar.Add(tv);
	}
	else if((dH3 > 0 && dH1 == 0 && dH2 == 0))
	{
		if(pGW->m_dCrossF2 + pGW->m_dCrossF3 > pGW->m_dH3 - pGW->m_ddc1)
			pGW->m_dCrossF3 =  (pGW->m_dH3 - pGW->m_ddc1) - pGW->m_dCrossF2;			
	}

	// Dot Rebar_ & Shear Rebar_
	if(pGW->m_dF2<0)
		dF2 = pGW->m_dF2 = pGW->m_dCrossF2;
	else
		dF2 = pGW->m_dF2;
	if(pGW->m_dF3<0)
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3;
	else
		dF3 = pGW->m_dF3;

	pGW->m_dS;


	if(pGW->IsTypeHDanRailDetectGravel())	// �ڰ�����,����  F2, F3 ����.
	{
		dF2 = pGW->m_dF2 = pGW->m_dCrossF2 = 0;
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3 = 0;
	}
	else if(bRailFence)
	{
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3 = 0;
	}

	CRebarPlacing rb;

	rb.StretchTvArr(tvArrLeft, 1000, 1000, TRUE);
	rb.StretchTvArr(tvArrRight, 1000, 1000, TRUE);
	long nRebar = tvArrRebar.GetSize();
	double dGap = dDomScale*0.72;
	CDPoint vLineAng = CDPoint(1,0);
	CDPoint xyLeft(0,0), xyRight(0,0);
	
	if(nRebar<=0) return;
	CDPointArray xyArrMatch;

	//������ ��쿡�� ��ܺ��� �ϴ����� ����ö�� ��ġ(���� ö�ٵ� �̶� ��ġ)
	if(bRailFence)
	{
		CDPoint xyTopBase(0,0);
		double dExt = 5000;
		CTwinVector tvMat;

		//xyPointRebar
		xyTopBase.y = tvArrRebar.GetXyTop().y;
		xyTopBase.x = tvArrRebar.GetXyMid().x;

		tvMat.m_v1 = tvMat.m_v2 = xyTopBase;
		tvMat.m_v1.x -= dExt;
		tvMat.m_v2.x += dExt;

		BOOL bExistShearFirst = FALSE;
		
		for (long nIdx =0; nIdx < pGW->m_dE; nIdx++)
		{
			CDPointArray xyArrMatL, xyArrMatR;

			rb.GetXyMatchTvArrAndTv(tvArrLeft, tvMat, xyArrMatL, TRUE);
			rb.GetXyMatchTvArrAndTv(tvArrRight, tvMat, xyArrMatR, TRUE);
			if(xyArrMatL.GetSize() ==0 || xyArrMatR.GetSize() == 0) continue;

			xyLeft	= xyArrMatL.GetAt(0);
			xyRight	= xyArrMatR.GetAt(0);

			//Point Rebar
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);
			
			//Shear Rebar
			long nSc = (long)pGW->m_dSC;
			if(nIdx % nSc ==1 ) //ù �ֻ�� ö���� ���� �ϱ� ������ �״��� ö�ٺ��� ��ġ
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;
				tvArrHor.Add(tv);
				tvArrRebar.Add(tv);
			}

			tvMat.m_v1.y -= pGW->m_dF2;
			tvMat.m_v2.y -= pGW->m_dF2;			
		}		
	}
	else
	{
		long n=0; for(n=0; n<2; n++)
		{
			BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

			vL1 = tvArrRebar.GetAt(0).m_v1;		// Dot Rebar_
			if(n==0)
				vL1.y += dF1+dF2;		// F1+F2
			else
				vL1.y += dF1+dF2+dF3;	// F1+F2+F3

			rb.GetXyMatchTvArrAndLine(tvArrLeft, vL1, vLineAng, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyLeft	= xyArrMatch.GetAt(0);
				bLeftRebar	= TRUE;
			}

			rb.GetXyMatchTvArrAndLine(tvArrRight, vL1, vLineAng, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyRight	= xyArrMatch.GetAt(0);
				bRightRebar	= TRUE;
			}

			if(bLeftRebar && bRightRebar)
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;

				tvArrHor.Add(tv);
				if(!pGW->IsTypeHDanRailDetectGravel())	// �ڰ����̴� F2, F3 ����.
				{
					if((n==0 && dF2>0) || (n==1 && dF3>0))
						tvArrRebar.Add(tv);
				}

				xyLeft.x += dGap;
				xyRight.x -= dGap;

				if(!pGW->IsTypeHDanRailDetectGravel())	// �ڰ����̴� F2, F3 ����.
				{
					if((n==0 && dF2>0) || (n==1 && dF3>0))
					{
						xyPointRebar.Add(xyLeft);
						xyPointRebar.Add(xyRight);
					}
				}
			}
		}

		double dTopY = tvArrTop.GetAt(1).m_v1.y;
		long   nHorRebar = tvArrHor.GetSize();
		if(nHorRebar<1) return;
		double dFY = tvArrHor.GetAt(nHorRebar-1).m_v1.y;
		double dLength = dTopY - dFY;
		long   dE = (long)pGW->m_dE;
		pGW->m_dF = Round(dLength/dE, 0);
		long   nCnt = 0;
		for(n=0; n<dE-1; n++)
		{
			BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

			if(dH1 == 0 && dH2 == 0 && dH3 > 0)
				continue;

			vL1 = tvArrHor.GetAt(nHorRebar-1).m_v1;
			vL1.y += (n+1) * pGW->m_dF;

			rb.GetXyMatchTvArrAndLine(tvArrLeft, vL1, vLineAng, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyLeft	= xyArrMatch.GetAt(0);
				bLeftRebar	= TRUE;
			}

			rb.GetXyMatchTvArrAndLine(tvArrRight, vL1, vLineAng, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyRight	= xyArrMatch.GetAt(0);
				bRightRebar	= TRUE;
			}

			if(bLeftRebar && bRightRebar)
			{
				nCnt++;		// ����ö���� ���ö���� ���� pGW->m_dSC �������� ���δ�.
				BOOL bShearRebar = FALSE;
				long k=0; for(k=1; k<MAX_REBAR_GUGAN; k++)
				{
					long nShear = (long)(k * pGW->m_dSC);
					if(nCnt==nShear)
					{
						bShearRebar = TRUE;
						break;
					}
				}

				if(bShearRebar)
				{
					CTwinVector tv;
					tv.m_v1 = xyLeft;
					tv.m_v2 = xyRight;
					tvArrHor.Add(tv);
					tvArrRebar.Add(tv);
				}

				xyLeft.x += dGap;
				xyRight.x -= dGap;
				xyPointRebar.Add(xyLeft);
				xyPointRebar.Add(xyRight);

				if(n==dE-2)	 // �� ���� ���ö��
				{
					xyLeft = tvArrTop.GetAt(1).m_v1;
					xyRight = tvArrTop.GetAt(1).m_v2;
					xyLeft.x += dGap;
					xyRight.x -= dGap;
					xyLeft.y -= dGap;
					xyRight.y -= dGap;
					xyPointRebar.Add(xyLeft);
					xyPointRebar.Add(xyRight);
				}
			}
		}
	}	
}

void CRcBridgeRebarUtil::GetTvRebarCenterGuard(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	long nSize = tvArr.GetSize();
	// ������ ���۴�
	CDPoint vStt = CDPoint( ( tvArr.GetAt(1).m_v1.x + tvArr.GetAt(nSize-1).m_v1.x ) * 0.5, tvArr.GetAt(1).m_v1.y-pGW->m_dF1);
	CDPoint vEnd = CDPoint( vStt.x , tvArr.GetAt(1).m_v1.y + pGW->m_dF2);

	tvArrRebar.Add(CTwinVector(vStt, vEnd));

	pGW->m_dCrossF2 = tvArr.GetAt(1).m_v2.y - tvArr.GetAt(1).m_v1.y;
	pGW->m_dCrossF3 = 0;
}

void CRcBridgeRebarUtil::GetTvRebarCenterGuard2(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	double	dF1 = pGW->m_dF1;
	double	dF2 = 0;
	double	dF3 = 0;
	double	dL1 = pGW->m_dL1;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	
	CTwinVector tvOffset;
	CTwinVectorArray tvArrLeft, tvArrTop, tvArrRight, tvArrRight2, tvArrHor;
	tvArrLeft.RemoveAll();
	tvArrTop.RemoveAll();
	tvArrRight.RemoveAll();
	tvArrRight2.RemoveAll();
	tvArrHor.RemoveAll();
	
	double dTop = 100; //������
	
	// ���� ö���� ������ ����
	CTwinVector tvLeft, tvRight;	
	CDPoint xyResult(0,0), xyResult1(0,0), xyResult2(0,0);
	
	pGW->m_dCrossF2 = 0;
	pGW->m_dCrossF3 = 0;

	// �����꿡 ���Ե� ö�� 
	long nSize = tvArr.GetSize();
	CDPoint xyLine = tvArr.GetAt(1).m_v1;
	xyLine.y -= dF1;
		
	// Left Rebar_
	long nLeft = 5;
	CDPoint xyPrev1 = tvArr.GetAt(nLeft).m_v1;
	CDPoint xyNext1 = tvArr.GetAt(nLeft).m_v2;
	Offset(xyPrev1,xyNext1,dOffset2);
	
	CDPoint xyPrev2 = tvArr.GetAt(nLeft+2).m_v1;
	CDPoint xyNext2 = tvArr.GetAt(nLeft+2).m_v2;
	Offset(xyPrev2,xyNext2,dOffset1);

	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
	GetXyMatchSegAndLine(xyPrev1,xyNext1,xyLine,CDPoint(1,0),xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;

	// Top
	CDPoint vTopLeft = xyResult1;		// �� �� ��±� ��ġ
	CDPoint vTopLegStt = CDPoint(xyPrev2.x, xyPrev2.y-dTop);
	CDPoint vTopLegEnd = CDPoint(xyNext2.x, xyNext2.y-dTop);
	CDPoint vTopLegLeft(0,0);
	GetXyMatchSegAndSeg(xyPrev1,xyNext1,vTopLegStt,vTopLegEnd,vTopLegLeft);

	tvLeft.m_v1 = xyResult1;	// ���� ö�ٰ��� ������ ����
	tvLeft.m_v2 = xyResult2;
	tvArrLeft.Add(tvOffset);
	tvArrRebar.Add(tvOffset);
	
	tvOffset.m_v1 = xyResult2;
	tvOffset.m_v2 = CDPoint(xyResult2.x-dL1, xyResult2.y);
	tvArrLeft.Add(tvOffset);
	tvArrRebar.Add(tvOffset);

	// Right Rebar_1
	long nRight = 9;
	xyPrev1 = tvArr.GetAt(nRight).m_v1;
	xyNext1 = tvArr.GetAt(nRight).m_v2;
	Offset(xyPrev1,xyNext1,dOffset3);
	
	xyPrev2 = tvArr.GetAt(nRight-2).m_v1;
	xyNext2 = tvArr.GetAt(nRight-2).m_v2;
	Offset(xyPrev2,xyNext2,dOffset1);

	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
	GetXyMatchSegAndLine(xyPrev1,xyNext1,xyLine,CDPoint(1,0),xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;

	// Top
	CDPoint vTopRight = xyResult1;		// �� �� ��±� ��ġ
	vTopLegStt = CDPoint(xyPrev2.x, xyPrev2.y-dTop);
	vTopLegEnd = CDPoint(xyNext2.x, xyNext2.y-dTop);
	CDPoint vTopLegRight(0,0);
	GetXyMatchSegAndSeg(xyPrev1,xyNext1,vTopLegStt,vTopLegEnd,vTopLegRight);

	tvArrTop.Add(CTwinVector(vTopLegLeft, vTopLeft));
	tvArrTop.Add(CTwinVector(vTopLeft, vTopRight));
	tvArrTop.Add(CTwinVector(vTopRight, vTopLegRight));

	tvArrRebar.Add(CTwinVector(vTopLegLeft, vTopLeft));
	tvArrRebar.Add(CTwinVector(vTopLeft, vTopRight));
	tvArrRebar.Add(CTwinVector(vTopRight, vTopLegRight));
	
	tvRight.m_v1 = xyResult1;	// ���� ö�ٰ��� ������ ����
	tvRight.m_v2 = xyResult2;
	tvArrRight.Add(tvOffset);
	tvArrRebar.Add(tvOffset);
	
	tvOffset.m_v1 = xyResult2;
	tvOffset.m_v2 = CDPoint(xyResult2.x+dL1, xyResult2.y);
	tvArrRight.Add(tvOffset);
	tvArrRebar.Add(tvOffset);

	// ���� Rebar_2 
	// Slab �Ʒ� ö�� "L"
	for(long Left=0; Left<2; Left++)
	{
		long nStt = Left ? nSize-1 : 1;
		CDPoint xy3 = Left ? tvArr.GetAt(nStt).m_v2 : tvArr.GetAt(nStt).m_v1;
		xy3.x = Left ? xy3.x - dOffset3 : xy3.x + dOffset2;
		CDPoint xy2 = CDPoint(xy3.x, xyLine.y);
		CDPoint xy1 = Left ? CDPoint(xy2.x + dL1, xy2.y) : CDPoint(xy2.x - dL1, xy2.y);
		
		tvOffset.m_v1 = xy1;
		tvOffset.m_v2 = xy2;
		tvArrRight2.Add(tvOffset);
		tvArrRebar.Add(tvOffset);	// "��"
		
		tvOffset.m_v1 = xy2;
		tvOffset.m_v2 = xy3;
		tvArrRight2.Add(tvOffset);
		tvArrRebar.Add(tvOffset);	// "��"

		xyPrev1 = tvArr.GetAt(nStt).m_v1;
		xyNext1 = tvArr.GetAt(nStt).m_v2;
		Left ? Offset(xyPrev1,xyNext1,dOffset3) : Offset(xyPrev1,xyNext1,dOffset2);
		
		xyPrev2 = Left ? tvArr.GetAt(nStt-2).m_v1 : tvArr.GetAt(nStt+2).m_v1;
		xyNext2 = Left ? tvArr.GetAt(nStt-2).m_v2 : tvArr.GetAt(nStt+2).m_v2;
		Left ? Offset(xyPrev2,xyNext2,dOffset3) : Offset(xyPrev2,xyNext2,dOffset2);

		GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
		tvOffset.m_v1 = Left ? xyNext1 : xyPrev1;
		tvOffset.m_v2 = xyResult1;

		// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
		if(pGW->m_dCrossF2==0 && pGW->m_dCrossF3==0)
		pGW->m_dCrossF2 = tvOffset.GetVerLength();

		tvArrRight2.Add(tvOffset);
		tvArrRebar.Add(tvOffset);		// "��"

		nStt = Left ? nStt-2 : nStt+2;

		xyPrev1 = tvArr.GetAt(nStt).m_v1;
		xyNext1 = tvArr.GetAt(nStt).m_v2;
		Left ? Offset(xyPrev1,xyNext1,dOffset3) : Offset(xyPrev1,xyNext1,dOffset2);
		if(Left)
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvLeft.m_v1,tvLeft.m_v2,xyResult2);
		else
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvRight.m_v1,tvRight.m_v2,xyResult2);
		tvOffset.m_v1 = xyResult1;
		tvOffset.m_v2 = xyResult2;		// "/"

		tvArrRight2.Add(tvOffset);
		tvArrRebar.Add(tvOffset);

		// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
		if(Left)
		GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvRight.m_v1,tvRight.m_v2,xyResult2);
		else
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvLeft.m_v1,tvLeft.m_v2,xyResult2);
		tvOffset.m_v1 = xyResult1;
		tvOffset.m_v2 = xyResult2;		// "/"

		if(pGW->m_dCrossF2!=0 && (pGW->m_dCrossF3==0))
			pGW->m_dCrossF3 = tvOffset.GetVerLength();
	}

	// Dot Rebar_ & Shear Rebar_
	if(pGW->m_dF2<0)
		dF2 = pGW->m_dF2 = pGW->m_dCrossF2;
	else
		dF2 = pGW->m_dF2;
	if(pGW->m_dF3<0)
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3;
	else
		dF3 = pGW->m_dF3;

	long nRebar = tvArrRebar.GetSize();
	long nRebarLeft = tvArrLeft.GetSize();
	long nRebarRight = tvArrRight.GetSize();
	long nRebarRight2 = tvArrRight2.GetSize();

	double dGap = dDomScale*0.72;
	CDPoint vLineAng = CDPoint(1,0);
	CDPoint xyLeft(0,0), xyRight(0,0);
	CDPoint vL1(0,0), vL2(0,0), vL3(0,0), vL4(0,0);
	
	if(nRebar<=0) return;

	long n=0; for(n=0; n<2; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = xyLine;		// ����
		if(n==0)
			vL1.y += dF1+dF2;	// F1+F2
		else
			vL1.y += dF1+dF2+dF3;	// F1+F2+F3

		// Left
		long i=0; for(i=0; i<nRebarRight2; i++)
		{
			vL3 = tvArrRight2.GetAt(i).m_v1;
			vL4 = tvArrRight2.GetAt(i).m_v2;
			bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
			if(bLeftRebar)	break;
		}
		// Right
		for(i=nRebarRight2-1; i>=0; i--)
		{
			vL3 = tvArrRight2.GetAt(i).m_v1;
			vL4 = tvArrRight2.GetAt(i).m_v2;
			bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
			if(bRightRebar)	break;
		}

		if(bLeftRebar && bRightRebar)
		{
			CTwinVector tv;
			tv.m_v1 = xyLeft;
			tv.m_v2 = xyRight;
			tvArrHor.Add(tv);
			tvArrRebar.Add(tv);
						
			xyLeft.x += dGap;
			xyRight.x -= dGap;
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);
		}
	}

	long   nHorRebar = tvArrHor.GetSize();
	if(nHorRebar<1) return;
	double dFY = tvArrHor.GetAt(nHorRebar-1).m_v1.y;
	double dLength = vTopLeft.y - dFY;
	long   dE = (long)(pGW->m_dE);
	pGW->m_dF = Round(dLength/dE, 0);
	long   nCnt = 0;
 	for(n=0; n<dE-1; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = tvArrHor.GetAt(nHorRebar-1).m_v1;
		vL1.y += (n+1) * pGW->m_dF;

		long i=0; for(i=0; i<nRebarLeft; i++)
		{
			vL3 = tvArrLeft.GetAt(i).m_v1;
			vL4 = tvArrLeft.GetAt(i).m_v2;
			bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
			if(bLeftRebar)
				break;
		}
		for(i=0; i<nRebarRight; i++)
		{
			vL3 = tvArrRight.GetAt(i).m_v1;
			vL4 = tvArrRight.GetAt(i).m_v2;
			bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
			if(bRightRebar)
				break;
		}
		
		if(bLeftRebar && bRightRebar)
		{
			nCnt++;		// ����ö���� ���ö���� ���� pGW->m_dSC �������� ���δ�.
			BOOL bShearRebar = FALSE;
			long k=0; for(k=1; k<MAX_REBAR_GUGAN; k++)
			{
				long nShear = (long)(k * pGW->m_dSC);
				if(nCnt==nShear)
				{
					bShearRebar = TRUE;
					break;
				}
			}
			
			if(bShearRebar)
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;
				tvArrHor.Add(tv);
				tvArrRebar.Add(tv);
			}

			xyLeft.x += dGap;
			xyRight.x -= dGap;
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);

			if(n==dE-2)	 // �� ���� ���ö��
			{
				vTopLeft.x += dGap;
				vTopRight.x -= dGap;
				vTopLeft.y -= dGap;
				vTopRight.y -= dGap;
				xyPointRebar.Add(vTopLeft);
				xyPointRebar.Add(vTopRight);
			}
		}
	}
}

void CRcBridgeRebarUtil::GetTvRebarCenterGuardHalf(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;
	
	double	dF1 = pGW->m_dF1;
	double	dF2 = 0;
	double	dF3 = 0;
	double	dL1 = pGW->m_dL1;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	
	CTwinVector tvOffset;
	CTwinVectorArray tvArrLeft, tvArrRight, tvArrRight2, tvArrHor;
	tvArrLeft.RemoveAll();
	tvArrRight.RemoveAll();
	tvArrRight2.RemoveAll();
	tvArrHor.RemoveAll();
	
	// ���� ö���� ������ ����
	CTwinVector tvLeft, tvRight;	
	CDPoint xyResult(0,0), xyResult1(0,0), xyResult2(0,0);
	
	pGW->m_dCrossF2 = 0;
	pGW->m_dCrossF3 = 0;

	// �����꿡 ���Ե� ö�� 
	long nSize = tvArr.GetSize();
	CDPoint xyLine = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v1;
	xyLine.y -= dF1;
		
	// Left Rebar_
	long nLeft = 5;
	CDPoint xyPrev1 = tvArr.GetAt(nLeft).m_v1;
	CDPoint xyNext1 = tvArr.GetAt(nLeft).m_v2;
	Offset(xyPrev1,xyNext1,dOffset2);
	
	CDPoint xyPrev2 = tvArr.GetAt(nLeft+2).m_v1;
	CDPoint xyNext2 = tvArr.GetAt(nLeft+2).m_v2;
	Offset(xyPrev2,xyNext2,dOffset1);

	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
	GetXyMatchSegAndLine(xyPrev1,xyNext1,xyLine,CDPoint(1,0),xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;
	CDPoint vTopLeft = xyResult1;		// �� �� ��±� ��ġ

	tvLeft.m_v1 = xyResult1;	// ���� ö�ٰ��� ������ ����
	tvLeft.m_v2 = xyResult2;
	tvArrLeft.Add(tvOffset);
	tvArrRebar.Add(tvOffset);
	
	if(bLeft)
	{
		tvOffset.m_v1 = xyResult2;
		tvOffset.m_v2 = CDPoint(xyResult2.x+dL1, xyResult2.y);
		//tvOffset.m_v2 = GetXyRotateRadian(tvOffset.m_v1, tvOffset.m_v2, -ToRadian(5));
		tvArrLeft.Add(tvOffset);
		tvArrRebar.Add(tvOffset);
	}

	// Right Rebar_1
	long nRight = 9;
	xyPrev1 = tvArr.GetAt(nRight).m_v1;
	xyNext1 = tvArr.GetAt(nRight).m_v2;
	Offset(xyPrev1,xyNext1,dOffset3);
	
	xyPrev2 = tvArr.GetAt(nRight-2).m_v1;
	xyNext2 = tvArr.GetAt(nRight-2).m_v2;
	Offset(xyPrev2,xyNext2,dOffset1);

	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
	GetXyMatchSegAndLine(xyPrev1,xyNext1,xyLine,CDPoint(1,0),xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;
	CDPoint vTopRight = xyResult1;		// �� �� ��±� ��ġ

	tvRight.m_v1 = xyResult1;	// ���� ö�ٰ��� ������ ����
	tvRight.m_v2 = xyResult2;
	tvArrRight.Add(tvOffset);
	tvArrRebar.Add(tvOffset);
	
	if(!bLeft)
	{
		tvOffset.m_v1 = xyResult2;
		tvOffset.m_v2 = CDPoint(xyResult2.x-dL1, xyResult2.y);
		//tvOffset.m_v2 = GetXyRotateRadian(tvOffset.m_v1, tvOffset.m_v2, ToRadian(5));
		tvArrRight.Add(tvOffset);
		tvArrRebar.Add(tvOffset);
	}

	// ���� Rebar_2
	// Slab �Ʒ� ö�� "L"
	long nStt = bLeft ? nSize-1 : 1;
	CDPoint xy3 = bLeft ? tvArr.GetAt(nStt).m_v2 : tvArr.GetAt(nStt).m_v1;
	xy3.x = bLeft ? xy3.x - dOffset3 : xy3.x + dOffset2;
	CDPoint xy2 = CDPoint(xy3.x, xyLine.y);
	CDPoint xy1 = bLeft ? CDPoint(xy2.x + dL1, xy2.y) : CDPoint(xy2.x - dL1, xy2.y);

	//xy1 = bLeft ? GetXyRotateRadian(xy2, xy1, -ToRadian(5)) : GetXyRotateRadian(xy2, xy1, ToRadian(5));	
	tvOffset.m_v1 = xy1;
	tvOffset.m_v2 = xy2;
	tvArrRight2.Add(tvOffset);
	tvArrRebar.Add(tvOffset);	// "��"
	
	tvOffset.m_v1 = xy2;
	tvOffset.m_v2 = xy3;
	tvArrRight2.Add(tvOffset);
	tvArrRebar.Add(tvOffset);	// "��"

	xyPrev1 = tvArr.GetAt(nStt).m_v1;
	xyNext1 = tvArr.GetAt(nStt).m_v2;
	bLeft ? Offset(xyPrev1,xyNext1,dOffset3) : Offset(xyPrev1,xyNext1,dOffset2);
	
	xyPrev2 = bLeft ? tvArr.GetAt(nStt-2).m_v1 : tvArr.GetAt(nStt+2).m_v1;
	xyNext2 = bLeft ? tvArr.GetAt(nStt-2).m_v2 : tvArr.GetAt(nStt+2).m_v2;
	bLeft ? Offset(xyPrev2,xyNext2,dOffset3) : Offset(xyPrev2,xyNext2,dOffset2);

	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult1);
	tvOffset.m_v1 = bLeft ? xyNext1 : xyPrev1;
	tvOffset.m_v2 = xyResult1;

	// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
	if(pGW->m_dCrossF2==0 && pGW->m_dCrossF3==0)
		pGW->m_dCrossF2 = tvOffset.GetVerLength();

	tvArrRight2.Add(tvOffset);
	tvArrRebar.Add(tvOffset);		// "��"

	nStt = bLeft ? nStt-2 : nStt+2;

	xyPrev1 = tvArr.GetAt(nStt).m_v1;
	xyNext1 = tvArr.GetAt(nStt).m_v2;
	bLeft ? Offset(xyPrev1,xyNext1,dOffset3) : Offset(xyPrev1,xyNext1,dOffset2);
	if(bLeft)
		GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvLeft.m_v1,tvLeft.m_v2,xyResult2);
	else
		GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvRight.m_v1,tvRight.m_v2,xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;		// "/"

	tvArrRight2.Add(tvOffset);
	tvArrRebar.Add(tvOffset);

	// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
	if(bLeft)
		GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvRight.m_v1,tvRight.m_v2,xyResult2);
	else
		GetXyMatchSegAndSeg(xyPrev1,xyNext1,tvLeft.m_v1,tvLeft.m_v2,xyResult2);
	tvOffset.m_v1 = xyResult1;
	tvOffset.m_v2 = xyResult2;		// "/"

	if(pGW->m_dCrossF2!=0 && (pGW->m_dCrossF3==0))
		pGW->m_dCrossF3 = tvOffset.GetVerLength();

	// Dot Rebar_ & Shear Rebar_
	if(pGW->m_dF2<0)
		dF2 = pGW->m_dF2 = pGW->m_dCrossF2;
	else
		dF2 = pGW->m_dF2;
	if(pGW->m_dF3<0)
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3;
	else
		dF3 = pGW->m_dF3;

	long nRebar = tvArrRebar.GetSize();
	long nRebarLeft = tvArrLeft.GetSize();
	long nRebarRight = tvArrRight.GetSize();
	long nRebarRight2 = tvArrRight2.GetSize();

	double dGap = dDomScale*0.72;
	CDPoint vLineAng = CDPoint(1,0);
	CDPoint xyLeft(0,0), xyRight(0,0);
	CDPoint vL1(0,0), vL2(0,0), vL3(0,0), vL4(0,0);
	
	if(nRebar<=0) return;

	long n=0; for(n=0; n<2; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = xyLine;		// ����
		if(n==0)
			vL1.y += dF1+dF2;	// F1+F2
		else
			vL1.y += dF1+dF2+dF3;	// F1+F2+F3

		if(n==0)
		{
			long i=0; for(i=0; i<nRebarRight2; i++)
			{
				vL3 = tvArrRight2.GetAt(i).m_v1;
				vL4 = tvArrRight2.GetAt(i).m_v2;
				bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
				if(bRightRebar)	break;
			}
		}
		else
		{
			if(bLeft)
			{
				long i=0; for(i=0; i<nRebarRight; i++)
				{
					vL3 = tvArrRight.GetAt(i).m_v1;
					vL4 = tvArrRight.GetAt(i).m_v2;
					bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
					if(bRightRebar)	break;
				}
			}
			else
			{
				long i=0; for(i=0; i<nRebarLeft; i++)
				{
					vL3 = tvArrLeft.GetAt(i).m_v1;
					vL4 = tvArrLeft.GetAt(i).m_v2;
					bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
					if(bRightRebar)	break;
				}
			}
		}
		if(bLeft)
		{
			long i=0; for(i=0; i<nRebarLeft; i++)
			{
				vL3 = tvArrLeft.GetAt(i).m_v1;
				vL4 = tvArrLeft.GetAt(i).m_v2;
				bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
				if(bLeftRebar)	break;
			}
		}
		else
		{
			long i=0; for(i=0; i<nRebarRight; i++)
			{
				vL3 = tvArrRight.GetAt(i).m_v1;
				vL4 = tvArrRight.GetAt(i).m_v2;
				bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
				if(bLeftRebar)	break;
			}
		}
		if(bLeftRebar && bRightRebar)
		{
			if(n!=0)
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;
				tvArrHor.Add(tv);
				tvArrRebar.Add(tv);
			}
						
			xyLeft.x += bLeft ? -dGap : dGap;
			xyRight.x -= bLeft ? -dGap : dGap;
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);
		}
	}

	long   nHorRebar = tvArrHor.GetSize();
	if(nHorRebar<1) return;
	double dFY = tvArrHor.GetAt(nHorRebar-1).m_v1.y;
	double dLength = vTopLeft.y - dFY;
	long   dE = (long)pGW->m_dE;
	pGW->m_dF = Round(dLength/dE, 0);
	long   nCnt = 0;
 	for(n=0; n<dE-1; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = tvArrHor.GetAt(nHorRebar-1).m_v1;
		vL1.y += (n+1) * pGW->m_dF;

		long i=0; for(i=0; i<nRebarLeft; i++)
		{
			vL3 = tvArrLeft.GetAt(i).m_v1;
			vL4 = tvArrLeft.GetAt(i).m_v2;
			bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
			if(bLeftRebar)		break;
		}
		for(i=0; i<nRebarRight; i++)
		{
			vL3 = tvArrRight.GetAt(i).m_v1;
			vL4 = tvArrRight.GetAt(i).m_v2;
			bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
			if(bRightRebar)		break;
		}
		
		if(bLeftRebar && bRightRebar)
		{
			nCnt++;		// ����ö���� ���ö���� ���� pGW->m_dSC �������� ���δ�.
			BOOL bShearRebar = FALSE;
			long k=0; for(k=1; k<MAX_REBAR_GUGAN; k++)
			{
				long nShear = (long)(k * pGW->m_dSC);
				if(nCnt==nShear)
				{
					bShearRebar = TRUE;
					break;
				}
			}
			
			if(bShearRebar && n < (long)(dE*0.5))		// ������ ���� ö�� 
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;
				tvArrHor.Add(tv);
				tvArrRebar.Add(tv);
			}

			xyLeft.x += dGap;
			xyRight.x -= dGap;
			if(n<dE-2)
			{
				xyPointRebar.Add(xyLeft);
				xyPointRebar.Add(xyRight);
			}
			if(n==dE-2)
			{
				if(bLeft)
				{
					xyPointRebar.Add(xyRight);
					// ���� ö�� ¥����
					CTwinVector rc;
					rc.m_v2 = tvArrLeft.GetAt(0).m_v2;
					rc.m_v1.x = tvArrLeft.GetAt(0).m_v1.x;
					rc.m_v1.y = xyRight.y;
					tvArrLeft.SetAt(0,rc);
					tvArrRebar.SetAt(0,rc);
				}
				else
				{
					xyPointRebar.Add(xyLeft);
					// ������ ö�� ¥����
					CTwinVector rc;
					rc.m_v2 = tvArrRight.GetAt(0).m_v2;
					rc.m_v1.x = tvArrRight.GetAt(0).m_v1.x;
					rc.m_v1.y = xyLeft.y;
					tvArrRight.SetAt(1,rc);
					tvArrRebar.SetAt(1,rc);
				}

			}
			
			if(n==dE-2)	 // �� ���� ���ö��
			{
				vTopLeft.x += dGap;
				vTopRight.x -= dGap;
				vTopLeft.y -= dGap;
				vTopRight.y -= dGap;
				if(bLeft)
					xyPointRebar.Add(vTopRight);
				else
					xyPointRebar.Add(vTopLeft);

			}
		}
	}
}

void CRcBridgeRebarUtil::GetTvRebarHandRail(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;

	double	dTop  = 50;
	double	dTopY = 0;		
	double	dF1 = pGW->m_dF1;
	double	dL1 = pGW->m_dL1;
	double	dCoverT = pGW->m_ddc1;		// Top �Ǻ�
	double	dCoverL = pGW->m_ddc2;		// Left �Ǻ�
	double	dCoverR = pGW->m_ddc3;		// Right �Ǻ�
	if(bLeft == FALSE)
		swap(dCoverL , dCoverR);

	// TEST ///
	// ���ʿ��� ���������� ����
	CTwinVectorArray tvArrGW;
	tvArrGW.AddFromTvArray(tvArr);
	CRebarPlacing rp;
	rp.RemoveMergeParallelTwinVector(tvArrGW);
	if(bLeft && pGW->m_dDistFrLeft > 0)
	{
		tvArrGW.RemoveAt(0);
	}
	// ����ö�� (����) ����
	// ���� �ϴ� ö�� (��� Ȥ�� ����) : tvArr�� ������ 3���� ���� ����. 
	// ���� ��� ö��  ����
	long nSizeTv = tvArrGW.GetSize();
	if(tvArrGW.GetSize() < 3) return;

// 	double dMinSettle = 100;
	CDPoint xy1(0,0), xy2(0,0), xyR(0,0);
	CDPoint vY(0,1), vX(bLeft ? 1.0 : -1.0, 0);
	CTwinVector tvRebar;
	CTwinVector tvL  = bLeft ? tvArrGW.GetAt(0) : tvArrGW.GetAt(nSizeTv-1);
	CTwinVector tvU  = bLeft ? tvArrGW.GetAt(1) : tvArrGW.GetAt(nSizeTv-2);
	CTwinVector tvR1 = bLeft ? tvArrGW.GetAt(2) : tvArrGW.GetAt(nSizeTv-3);
	CTwinVector tvR2, tvR3;
	double dStandardY = min(tvR1.m_v1.y, tvR1.m_v2.y);
	if(nSizeTv > 3)
	{
		tvR2 = bLeft ? tvArrGW.GetAt(3) : tvArrGW.GetAt(nSizeTv-4);
		dStandardY = min(tvR2.m_v1.y, tvR2.m_v2.y);
	}
	if(nSizeTv > 4)
	{
		tvR3 = bLeft ? tvArrGW.GetAt(4) : tvArrGW.GetAt(nSizeTv-5);
		dStandardY = min(tvR3.m_v1.y, tvR3.m_v2.y);
	}

	CDPoint xyTopCross(0,0), vDirUpper(0,0), xyMidCross(0,0), vDirMid(0,0);
	vDirUpper = (tvR1.m_v2 - tvR1.m_v1).Unit();
	xy1 = tvR1.m_v1;
	xy2 = tvR1.m_v2;
	Offset(xy1, xy2, dCoverR);
	xyR = bLeft ? tvL.m_v2 : tvL.m_v1;
	GetXyMatchSegAndLine(xy1, xy2, xyR + dCoverL * vX - dCoverT * vY, vX, xyTopCross);

	long nIdxRebar = 1;
	//////////////////////////////////////////////////////////////////////////
	// ����ö�� : bLeft�� ��� ����
	xyR = bLeft ? tvL.m_v1 : tvL.m_v2;
	xyR.y = dStandardY;
	CDPoint xyBot = xyR - dF1 * vY + dCoverL * vX;

	tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
	tvRebar.m_v1 = xyBot;
	tvRebar.m_v2 = tvRebar.m_v1 + dL1 * vX;
	tvArrRebar.Add(tvRebar);

	xyR = bLeft ? tvL.m_v2 : tvL.m_v1;
	tvRebar.m_v2 = xyR - dCoverT * vY + dCoverL * vX;
	tvArrRebar.Add(tvRebar);

	tvRebar.m_v1 = tvRebar.m_v2;
	tvRebar.m_v2 = xyTopCross;
	tvArrRebar.Add(tvRebar);

	// ���ö��
	if(tvArrGW.GetSize() <= 3) // �����꿡 ����
	{
		tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
		tvRebar.m_v1 = xyTopCross;
		GetXyMatchLineAndLine(xyBot, vX, xyTopCross, vDirUpper, tvRebar.m_v2);
		tvArrRebar.Add(tvRebar);

		tvRebar.m_v1 = tvRebar.m_v2;
		tvRebar.m_v2 = tvRebar.m_v1 + dL1 * vX;
		tvArrRebar.Add(tvRebar);
	}
	else // ������ġ���� ����
	{
		double dMinSettle = 100;

		vDirMid = bLeft ? (tvR2.m_v1 - tvR2.m_v2).Unit() : (tvR2.m_v2 - tvR2.m_v1).Unit();
		xy1 = tvR2.m_v1;
		xy2 = tvR2.m_v2;
		Offset(xy1, xy2, dCoverR);
		GetXyMatchSegAndLine(xy1, xy2, xyTopCross, vDirUpper, xyMidCross);

		if(nSizeTv > 4 || (tvR2.GetHorLength() > 0 && tvR2.GetVerLength() > 0))
		{
			tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
			tvRebar.m_v1 = xyTopCross;
			tvRebar.m_v2 = xyMidCross - dMinSettle * vY;
			tvArrRebar.Add(tvRebar);
		}
		else if(nSizeTv == 4 && Compare(tvR2.GetHorLength(), 0.0, _T("="))) // 4���� R1�� ���, R2�� ����
		{
			tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
			tvRebar.m_v1 = xyTopCross;
			tvRebar.m_v2 = xyMidCross;
			tvArrRebar.Add(tvRebar);

			xyR = bLeft ? tvR2.m_v2 : tvR2.m_v1;
			xyR.y = dStandardY;
			xyBot = xyR - dF1 * vY - dCoverL * vX;
			tvRebar.m_v1 = xyMidCross;
			tvRebar.m_v2 = xyBot;
			tvArrRebar.Add(tvRebar);

			tvRebar.m_v1 = xyBot;
			tvRebar.m_v2 = xyBot + dL1 * vX;
			tvArrRebar.Add(tvRebar);
		}

		if(nSizeTv > 4)
		{
			CDPoint xyMidCross2(0,0);
			xyR = bLeft ? tvR3.m_v2 : tvR3.m_v1;
			xyR.y = dStandardY;
			xyBot = xyR - dF1 * vY - dCoverL * vX;
			GetXyMatchSegAndLine(xy1, xy2, xyBot, vY, xyMidCross2);

			tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
			tvRebar.m_v1 = xyMidCross + dMinSettle * vDirMid;
			tvRebar.m_v2 = xyMidCross2;
			tvArrRebar.Add(tvRebar);

			tvRebar.m_v1 = xyMidCross2;
			tvRebar.m_v2 = xyBot;
			tvArrRebar.Add(tvRebar);

			tvRebar.m_v1 = xyBot;
			tvRebar.m_v2 = xyBot + dL1 * vX;
			tvArrRebar.Add(tvRebar);
		}
		else if(nSizeTv == 4 && Compare(tvR1.GetHorLength(), 0.0, _T("="))) // 4���� R1�� ����, R2�� ���
		{
			tvRebar.m_sInfo.Format(_T("��ö��%d"),nIdxRebar++);
			xyR = bLeft ? tvL.m_v1 : tvL.m_v2;
			xyR.y = dStandardY;
			xyBot = xyR - dF1 * vY + dCoverL * vX;
			GetXyMatchLineAndLine(xyTopCross, vX, xyMidCross, vDirMid, tvRebar.m_v1);
			GetXyMatchLineAndLine(xyBot, vX, xyMidCross, vDirMid, tvRebar.m_v2);
			tvArrRebar.Add(tvRebar);

			tvRebar.m_v1 = xyBot;
			tvRebar.m_v2 = xyBot + dL1 * vX;
			tvArrRebar.Add(tvRebar);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ����ö�� ��ġ
	// �ϸ鿡�� F2, EA����
	double dF2 = pGW->m_dF2;
	double dF3 = pGW->m_dF3;
	double dEa = pGW->m_dE;
	if(pGW->m_dF2<0)
	{
		if(pGW->m_dCrossF2 != 150)
			pGW->m_dCrossF2 = 150;

		dF2 = pGW->m_dF2 = pGW->m_dCrossF2;
	}
	if(pGW->m_dF3<0)
	{
		if(pGW->m_dH2 == 0 && pGW->m_dH3 == 0)
		{
			dF3 = pGW->m_dF3 = 0;
		}
		else
		{
			if(pGW->m_dCrossF3 != 200)
				pGW->m_dCrossF3 = 200;

			dF3 = pGW->m_dF3 = pGW->m_dCrossF3;
		}
	}
	CDoubleArray dArrPoint;
	double dRebarOffset = REBAR_RADIUS * dDomScale / 50.0;
	
	double dCoverEl		= xyTopCross.y - dRebarOffset;
	double dSumF2El	= xyBot.y + dF1 + dF2;
	double dSumF3El	= xyBot.y + dF1 + dF2 + dF3;

	if(Compare(dSumF2El ,dCoverEl, _T("<"),0.1))
		dArrPoint.Add(xyBot.y + dF1 + dF2);
	if(Compare(dSumF3El ,dCoverEl, _T("<"),0.1))
		dArrPoint.Add(xyBot.y + dF1 + dF2 + dF3);
	if(dEa > 0)
	{
		double dLength = xyTopCross.y - (xyBot.y + dF1 + dF2 + dF3);
		if(Compare(dLength, (double)0, _T(">"), 0.1))
		{
			pGW->m_dF = Round(dLength / dEa, 0);
			for(long ix = 0; ix < dEa - 1; ix++)
			{
				dArrPoint.Add(xyBot.y + dF1 + dF2 + dF3 + pGW->m_dF * (ix + 1));
			}
		}		
	}
	dArrPoint.Add(xyTopCross.y - dRebarOffset);

	nIdxRebar = 1;
	// (ARCBRIDGE-1491) ����ö�� ���� ���� ���ö�� ����
	long nCntShear = static_cast<long>(pGW->m_dSC);
	CTwinVector tvShear; // �ϳ��� �ɷ��� ��ġ
	long nSizePoint = dArrPoint.GetSize();
	for(long ix = 0; ix < nSizePoint; ix++)
	{
		// ����
		xy1 = CDPoint(0, dArrPoint.GetAt(ix));
		GetXyMatchLineAndLine(tvL.m_v1 + (dCoverL + dRebarOffset) * vX, vY, xy1, vX, xy2);
		xyPointRebar.Add(xy2);
		if(nCntShear > 0 && ix % nCntShear == 0 && ix < nSizePoint - 1)
		{
			tvShear.m_v1 = xy2 - dRebarOffset * vX;
		}

		// ����
		nSizeTv = tvArrRebar.GetSize();
		double dDistX = bLeft ? xyBot.x - 10000 : xyBot.x + 10000;
		for(long kk = 0; kk < nSizeTv; kk++)
		{
			CTwinVector tv = tvArrRebar.GetAt(kk);
			if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xy1, vX, xy2))
			{
				if(bLeft && xy2.x > dDistX)
				{
					dDistX = xy2.x - dRebarOffset;
				}
				if(!bLeft && xy2.x < dDistX)
				{
					dDistX = xy2.x + dRebarOffset;
				}
			}
		}
		xy2 = CDPoint(dDistX, xy1.y);
		xyPointRebar.Add(xy2);
		if((nCntShear > 0) && (ix % nCntShear == 0) && (ix < nSizePoint - 1))
		{
			tvShear.m_sInfo.Format(_T("����%d"),nIdxRebar++);
			tvShear.m_v2 = xy2 + dRebarOffset * vX;
			tvArrRebar.Add(tvShear);
		}
	}
	return;



	//////////////////////////////////////////////////////////////////////////
	CTwinVector tvOffset;
	CTwinVectorArray tvArrLeft, tvArrTop, tvArrRight, tvArrRight2, tvArrHor;
	tvArrLeft.RemoveAll();
	tvArrTop.RemoveAll();
	tvArrRight.RemoveAll();
	tvArrRight2.RemoveAll();
	tvArrHor.RemoveAll();
	
	CDPoint vTop1(0,0), vTop2(0,0);
	CDPoint xyResult(0,0);
	BOOL bMatch = FALSE;
	
	pGW->m_dCrossF2 = 0;
	pGW->m_dCrossF3 = 0;

	long nSize = tvArr.GetSize();
	// ������ ���۴�
	double dEndY = bLeft ? tvArr.GetAt(nSize-1).m_v2.y - dF1 - dCoverL 
						 : tvArr.GetAt(1).m_v1.y - dF1 - dCoverR;	

	// Left Rebar_
	long nLeft = 6;
	long i=0; for(i=1; i<nLeft; i++)		// i=0 �� ���� Gap��.
	{
		if(i==1)	// Slab �Ʒ� ö�� "L"
		{
			CDPoint xy3 = tvArr.GetAt(i).m_v1;	
			CDPoint xy2 = bLeft ? CDPoint(xy3.x, dEndY) 
				                : CDPoint(xy3.x, dEndY+(2*dCoverR));
			CDPoint xy1 = bLeft ? CDPoint(xy2.x + dCoverL + dL1, xy2.y)
								: CDPoint(xy2.x + dCoverL - dL1, xy2.y);

			Offset(xy1,xy2,dCoverL);	// "��"
			CDPoint vL1 = xy1;
			CDPoint vL2 = xy2;
			
			xy3 = tvArr.GetAt(i).m_v1;
			xy2 = bLeft ? CDPoint(xy3.x, dEndY)
						: CDPoint(xy3.x, dEndY+(2*dCoverR));
			xy1 = bLeft ? CDPoint(xy2.x + dCoverL + dL1, xy2.y)
						: CDPoint(xy2.x + dCoverL - dL1, xy2.y);
			
			Offset(xy2,xy3,dCoverL);	// "��"
			CDPoint vL3 = xy2;
			CDPoint vL4 = xy3;

			GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
			
			tvOffset.m_v1 = vL1;
			tvOffset.m_v2 = xyResult;	
			
			tvArrLeft.Add(tvOffset);	// "��"	
			tvArrRebar.Add(tvOffset);		
			
			tvOffset.m_v1 = xyResult;
			tvOffset.m_v2 = vL4;
			
			tvArrLeft.Add(tvOffset);	// "��"
			tvArrRebar.Add(tvOffset);	
		}

		CDPoint xyPrev1 = tvArr.GetAt(i).m_v1;
		CDPoint xyNext1 = tvArr.GetAt(i).m_v2;
		
		Offset(xyPrev1,xyNext1,dCoverL);
		
		if(bMatch)
			xyPrev1 = xyResult;
		
		CDPoint xyPrev2 = tvArr.GetAt(i+2).m_v1;
		CDPoint xyNext2 = tvArr.GetAt(i+2).m_v2;
			
		if(i==nLeft-1)
		{
			// �� �� ö�ٰ��� ������ ���Ѵ�. R ���� �ǳʶ�
			bMatch = FALSE;
			Offset(xyPrev2,xyNext2,dCoverT);
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult);
			xyNext1 = xyResult;
			dTopY = xyResult.y;
			
			// Left Rebar���� ����
			xyPrev2.y -= dTop;
			xyNext2.y -= dTop;
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult);
			vTop1 = xyResult;	// Top Rebar_
			vTop2 = xyNext1;
		}
		else
		{
			// R ���� �ǳʶ�
			Offset(xyPrev2,xyNext2,dCoverL);	
			GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult);
			
			if(xyNext1.x != xyNext2.x)		// ���� �϶��� ������ Ʋ����!
				bMatch = TRUE;
			else
				bMatch = FALSE;
		}
		
		tvOffset.m_v1 = xyPrev1;
		if(bMatch)
			tvOffset.m_v2 = xyResult;
		else
			tvOffset.m_v2 = xyNext1;

		// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
		if(!bLeft && i!=nLeft-1)
		{
			double dOrigin = tvArr.GetAt(1).m_v1.y;
			if(pGW->m_dCrossF2==0  && pGW->m_dCrossF3==0)
				pGW->m_dCrossF2 = tvOffset.m_v2.y - dOrigin;
			else if(pGW->m_dCrossF2!=0 && (pGW->m_dCrossF3==0))
				pGW->m_dCrossF3 = tvOffset.m_v2.y - dOrigin - pGW->m_dCrossF2;
		}

		tvArrLeft.Add(tvOffset);	// "��"
		tvArrRebar.Add(tvOffset);	

		i++;	// R ���� �ǳʶ�
	}

	// Top Rebar_
	CDPoint xyPrev1(0,0), xyNext1(0,0);
	CDPoint xyPrev2(0,0), xyNext2(0,0);
	
	tvOffset.m_v1 = vTop1;		// Top 1
	tvOffset.m_v2 = vTop2;
//	tvArrTop.Add(tvOffset);
//	tvArrRebar.Add(tvOffset);

	nLeft++;	// 7
	xyPrev1 = tvArr.GetAt(nLeft).m_v1;
	xyNext1 = tvArr.GetAt(nLeft).m_v2;
	nLeft++;	// 8 : R ����
	nLeft++;	// 9
	xyPrev2 = tvArr.GetAt(nLeft).m_v1;
	xyNext2 = tvArr.GetAt(nLeft).m_v2;
	Offset(xyPrev1,xyNext1,dCoverT);
	Offset(xyPrev2,xyNext2,dCoverR);

	// Right Rebar���� ����
	GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult);
	
	tvOffset.m_v1 = vTop2;		// Top 2
	tvOffset.m_v2 = xyResult;
	CDPoint vRight1 = xyResult;
	// 50
	if(bLeft)
	{
		tvOffset.m_v2 = vRight1;
		tvArrLeft.Add(tvOffset);
		tvArrRebar.Add(tvOffset);
	}
	else
	{
		tvOffset.m_v2 = CDPoint(tvOffset.m_v1.x + dTop, tvOffset.m_v1.y);
		tvArrLeft.Add(tvOffset);
		tvArrRebar.Add(tvOffset);
	}
	
//	tvArrTop.Add(tvOffset);
//	tvArrRebar.Add(tvOffset);

	tvOffset.m_v1 = xyResult;	// Top 3
	CDPoint vTemp = CDPoint(xyResult.x,vTop1.y);
	GetXyMatchSegAndSeg(xyPrev2,xyNext2,vTop1,vTemp,xyResult);		// ������ 100
	tvOffset.m_v2 = xyResult;
	// 50
	if(bLeft)
	{
		tvArrLeft.Add(tvOffset);
		tvArrRebar.Add(tvOffset);
	}

//	tvArrTop.Add(tvOffset);
//	tvArrRebar.Add(tvOffset);		
	
	// Right Rebar_1
	CDPoint vL1(0,0),vL2(0,0),vL3(0,0),vL4(0,0);

	for(i=nLeft; i<nSize; i++)
	{
		if(i==nSize-1)	// Slab �Ʒ� ö�� "L"
		{
			tvOffset.m_v1 = xyResult;	// Slab �ٷ� �� "��"
			xyNext2 = CDPoint(tvArr.GetAt(i).m_v2.x - dCoverR, tvArr.GetAt(i).m_v2.y);
			tvOffset.m_v2 = xyNext2;
			tvArrRight.Add(tvOffset);
			tvArrRebar.Add(tvOffset);	
			
			CDPoint xy3 = tvArr.GetAt(i).m_v2;
			CDPoint xy2 = bLeft ? CDPoint(xy3.x, xy3.y - dF1 + dCoverR) : CDPoint(xy3.x, xy3.y - dF1 - dCoverR);
// 			CDPoint xy1 = bLeft ? CDPoint(xy2.x + dL1 - dCoverR, xy2.y) : CDPoint(xy2.x - dL1 - dCoverR, xy2.y);
			
			Offset(xy3,xy2,dCoverR);		// Slab �Ʒ� "��"
			vL1 = xy3;
			vL2 = xy2;
			
			xy3 = tvArr.GetAt(i).m_v2;
			xy2 = bLeft ? CDPoint(xy3.x, dEndY+(2*dCoverR)) : CDPoint(xy3.x, dEndY);
			CDPoint xy1 = bLeft ? CDPoint(xy2.x + dL1 - dCoverR, xy2.y) : CDPoint(xy2.x - dL1 - dCoverR, xy2.y);
			
			Offset(xy2,xy1,dCoverR);		// "��"
			vL3 = xy2;
			vL4 = xy1;

			GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);	// ����
			
			tvOffset.m_v1 = vL1;
			tvOffset.m_v2 = xyResult;
			tvArrRight.Add(tvOffset);
			tvArrRebar.Add(tvOffset);

			tvOffset.m_v1 = xyResult;
			tvOffset.m_v2 = vL4;
			tvArrRight.Add(tvOffset);
			tvArrRebar.Add(tvOffset);
		}
		else
		{
			xyPrev1 = tvArr.GetAt(i).m_v1;
			xyNext1 = tvArr.GetAt(i).m_v2;
			xyPrev2 = tvArr.GetAt(i+2).m_v1;
			xyNext2 = tvArr.GetAt(i+2).m_v2;
			
			Offset(xyPrev1,xyNext1,dCoverR);
			Offset(xyPrev2,xyNext2,dCoverR);
		
			if(i==nLeft)
			{
				// 50
				if(bLeft)
				{
					tvOffset.m_v2 = CDPoint(vRight1.x - dTop, vRight1.y);
					tvArrRight.Add(tvOffset);
					tvArrRebar.Add(tvOffset);
				}
				else
				{
					tvOffset.m_v1 = vTop1;
					tvOffset.m_v2 = vTop2;
					tvArrRight.Add(tvOffset);
					tvArrRebar.Add(tvOffset);
					
					tvOffset.m_v1 = vTop2;
					tvOffset.m_v2 = vRight1;
					tvArrRight.Add(tvOffset);
					tvArrRebar.Add(tvOffset);
				}
				tvOffset.m_v1 = vRight1;
			}
			else
				tvOffset.m_v1 = xyResult;

			GetXyMatchSegAndSeg(xyPrev1,xyNext1,xyPrev2,xyNext2,xyResult);
			
			if(xyNext1.x == xyNext2.x)		// ���� �϶��� ������ Ʋ����!
				xyResult = xyNext1;

			tvOffset.m_v2 = xyResult;
			
			// Default Hor_ Rebar�� ���� ������ �����Ѵ�.
			if(bLeft)
			{
				double dOrigin = tvArr.GetAt(nSize-1).m_v2.y;
				if(pGW->m_dCrossF2==0  && pGW->m_dCrossF3==0)
					pGW->m_dCrossF3 = tvOffset.m_v2.y - dOrigin;
				else if(pGW->m_dCrossF3!=0 && (pGW->m_dCrossF2==0))
				{
					pGW->m_dCrossF2 = tvOffset.m_v2.y - dOrigin;
					pGW->m_dCrossF3 -= pGW->m_dCrossF2;
				}
			}

			tvArrRight.Add(tvOffset);
			tvArrRebar.Add(tvOffset);	
			
			i++;	// R ���� �ǳʶ�
		}
	}
		
	// ���� Rebar_2
	long nRebar2 = bLeft ? tvArrRight.GetSize() : tvArrLeft.GetSize()-2;
	long nStt = bLeft? 2 : 0;
	for(i=nStt; i<nRebar2; i++)
	{
		CTwinVector tv = bLeft ? tvArrRight.GetAt(i) : tvArrLeft.GetAt(i);
		long nExtendStt = bLeft ? 2 : 3; 
		if(i==nExtendStt)
		{
			long nSize = bLeft ? tvArrLeft.GetSize()-2 : tvArrRight.GetSize();
			long nStt = bLeft ? 0 : 2; 
			for(long j=nStt; j<nSize; j++)
			{
				CDPoint vAngLine = bLeft ? CDPoint( tv.m_v2.x - tv.m_v1.x , tv.m_v2.y - tv.m_v1.y)
										 : CDPoint( tv.m_v2.x - tv.m_v1.x , tv.m_v2.y - tv.m_v1.y);
				xyPrev1 = bLeft ? tvArrLeft.GetAt(j).m_v1 : tvArrRight.GetAt(j).m_v1;
				xyNext1 = bLeft ? tvArrLeft.GetAt(j).m_v2 : tvArrRight.GetAt(j).m_v2;
				BOOL bMeet = GetXyMatchSegAndLine(xyPrev1,xyNext1,tv.m_v1,vAngLine,xyResult);
				if(bMeet)
				{
					// 50
					double dDist = ~(xyResult - vTop2);
					if(dDist>50 && bLeft)
					{
						tvOffset.m_v1 = CDPoint(xyResult.x, xyResult.y+dTop);
						tvOffset.m_v2 = xyResult;
						tvArrRight2.Add(tvOffset);
						tvArrRebar.Add(tvOffset);
					}

					tvOffset.m_v1 = bLeft ? xyResult : tv.m_v1;
					tvOffset.m_v2 = bLeft ? tv.m_v2 : xyResult;
					tvArrRight2.Add(tvOffset);
					tvArrRebar.Add(tvOffset);

					if(dDist>50 && !bLeft)
					{
						tvOffset.m_v1 = xyResult;
						tvOffset.m_v2 = CDPoint(xyResult.x, xyResult.y+dTop);
						tvArrRight2.Add(tvOffset);
						tvArrRebar.Add(tvOffset);
					}
					break;
				}
				if(j==nSize-1)
				{
					xyPrev1 = bLeft ? tvArrLeft.GetAt(j+1).m_v1 : tvArrRight.GetAt(1).m_v1;
					xyNext1 = bLeft ? tvArrLeft.GetAt(j+1).m_v2 : tvArrRight.GetAt(1).m_v2;
					GetXyMatchSegAndLine(xyPrev1,xyNext1,tv.m_v1,vAngLine,xyResult);
					tvOffset.m_v1 = bLeft ? xyResult : tv.m_v1;
					tvOffset.m_v2 = bLeft ? tv.m_v2 : xyResult;
					// ����ö�� ���� ���� �ϱ� ���� 50 ������ �ϳ��� �߰��Ѵ�.
					tvArrRight2.Add(CTwinVector(tvOffset.m_v1, tvOffset.m_v1));
					tvArrRebar.Add(CTwinVector(tvOffset.m_v1, tvOffset.m_v1));
					//
					tvArrRight2.Add(tvOffset);	
					tvArrRebar.Add(tvOffset);
					break;				
				}
			}
		}
		else
		{
			tvArrRight2.Add(tv);
			tvArrRebar.Add(tv);
		}
	}

	// Dot Rebar_ & Shear Rebar_
	if(pGW->m_dF2<0)
		dF2 = pGW->m_dF2 = pGW->m_dCrossF2;
	else
		dF2 = pGW->m_dF2;
	if(pGW->m_dF3<0)
		dF3 = pGW->m_dF3 = pGW->m_dCrossF3;
	else
		dF3 = pGW->m_dF3;

	long nRebar = tvArrRebar.GetSize();
	long nRebarLeft = tvArrLeft.GetSize();
	long nRebarRight = tvArrRight.GetSize();

	double dGap = dDomScale*0.72;
	CDPoint vLineAng = CDPoint(1,0);
	CDPoint xyLeft(0,0), xyRight(0,0);
	
	if(nRebar<=0) return;

	long n=0; for(n=0; n<2; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = tvArrRebar.GetAt(0).m_v1;		// Dot Rebar_
		if(n==0)
			vL1.y += dF1+dF2;	// F1+F2
		else
			vL1.y += dF1+dF2+dF3;	// F1+F2+F3

		for(i=0; i<nRebarLeft; i++)
		{
			vL3 = tvArrLeft.GetAt(i).m_v1;
			vL4 = tvArrLeft.GetAt(i).m_v2;
			bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
			if(bLeftRebar)	break;
		}
		for(i=0; i<nRebarRight; i++)
		{
			vL3 = tvArrRight.GetAt(i).m_v1;
			vL4 = tvArrRight.GetAt(i).m_v2;
			bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
			if(bRightRebar)	break;
		}
		if(bLeftRebar && bRightRebar)
		{
			CTwinVector tv;
			tv.m_v1 = xyLeft;
			tv.m_v2 = xyRight;
			tvArrHor.Add(tv);
			tvArrRebar.Add(tv);
			
			xyLeft.x += dGap;
			xyRight.x -= dGap;
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);
		}
	}

	long   nHorRebar = tvArrHor.GetSize();
	if(nHorRebar<1) return;
	double dFY = tvArrHor.GetAt(nHorRebar-1).m_v1.y;
	double dLength = dTopY - dFY;
	long   dE = (long)pGW->m_dE;
	pGW->m_dF = Round(dLength/dE, 0);
	long   nCnt = 0;
 	for(n=0; n<dE-1; n++)
	{
		BOOL bLeftRebar=FALSE, bRightRebar=FALSE;

		vL1 = tvArrHor.GetAt(nHorRebar-1).m_v1;
		vL1.y += (n+1) * pGW->m_dF;

		for(i=0; i<nRebarLeft; i++)
		{
			vL3 = tvArrLeft.GetAt(i).m_v1;
			vL4 = tvArrLeft.GetAt(i).m_v2;
			bLeftRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyLeft);
			if(bLeftRebar)
				break;
		}
		for(i=0; i<nRebarRight; i++)
		{
			vL3 = tvArrRight.GetAt(i).m_v1;
			vL4 = tvArrRight.GetAt(i).m_v2;
			bRightRebar = GetXyMatchSegAndLine(vL3,vL4,vL1,vLineAng,xyRight);
			if(bRightRebar)
				break;
		}
		
		if(bLeftRebar && bRightRebar)
		{
			nCnt++;		// ����ö���� ���ö���� ���� pGW->m_dSC �������� ���δ�.
			BOOL bShearRebar = FALSE;
			long k=0; for(k=1; k<MAX_REBAR_GUGAN; k++)
			{
				long nShear = (long)(k * pGW->m_dSC);
				if(nCnt==nShear)
				{
					bShearRebar = TRUE;
					break;
				}
			}
			
			if(bShearRebar)
			{
				CTwinVector tv;
				tv.m_v1 = xyLeft;
				tv.m_v2 = xyRight;
				tvArrHor.Add(tv);
				tvArrRebar.Add(tv);
			}

			xyLeft.x += dGap;
			xyRight.x -= dGap;
			xyPointRebar.Add(xyLeft);
			xyPointRebar.Add(xyRight);

			if(n==dE-2)	 // �� ���� ���ö��
			{
				xyLeft = vTop2;
				xyRight = vRight1;
				xyLeft.x += dGap;
				xyRight.x -= dGap;
				xyLeft.y -= dGap;
				xyRight.y -= dGap;
				xyPointRebar.Add(xyLeft);
				xyPointRebar.Add(xyRight);
			}
		}
	}
}

void CRcBridgeRebarUtil::GetTvRebarSoundProof(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;
	
	double	dF1 = pGW->m_dF1;
	double	dL1 = pGW->m_dL1;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�

	CDPoint vTL(0,0), vTR(0,0), vBL(0,0), vBR(0,0);
	CDPoint vL1(0,0), vL2(0,0), vL3(0,0), vL4(0,0);

	vL1 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
	vL2 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset2) : Offset(vL1,vL2,dOffset3);
	vL3 = bLeft ? tvArr.GetAt(7).m_v1 : tvArr.GetAt(7).m_v1;
	vL4 = bLeft ? tvArr.GetAt(7).m_v2 : tvArr.GetAt(7).m_v2;
	Offset(vL3,vL4,dOffset1);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,vTL);
	vL1 = bLeft ? tvArr.GetAt(7).m_v1 : tvArr.GetAt(7).m_v1;
	vL2 = bLeft ? tvArr.GetAt(7).m_v2 : tvArr.GetAt(7).m_v2;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
	vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,vTR);

	double nSize = tvArr.GetSize();
	double dBY = bLeft ? tvArr.GetAt((int)(nSize-1)).m_v2.y - dF1 : tvArr.GetAt(1).m_v1.y - dF1;
	vBL = CDPoint(vTL.x, dBY);
	vBR = CDPoint(vTR.x, dBY);

	CDPoint vBLStt = bLeft ? CDPoint(vBL.x+dL1, vBL.y) : CDPoint(vBL.x-dL1, vBL.y);
	CDPoint vBREnd = bLeft ? CDPoint(vBR.x+dL1, vBR.y) : CDPoint(vBR.x-dL1, vBR.y);
	tvArrRebar.Add(CTwinVector(vBLStt, vBL));
	tvArrRebar.Add(CTwinVector(vBL, vTL));
	tvArrRebar.Add(CTwinVector(vTL, vTR));
	tvArrRebar.Add(CTwinVector(vTR, vBR));
	tvArrRebar.Add(CTwinVector(vBR, vBREnd));

	double dGapX = bLeft ? dDomScale*0.72 : -dDomScale*0.72;
	double dGapY = dDomScale*0.72;
//	double dUpperCover = m_pRcBridgeApp->m_dCoverUpper_UpperSlab[0];
//	xyPointRebar.Add(CDPoint(vBL.x+dGapX, vBL.y+(dF1-dUpperCover)));
//	xyPointRebar.Add(CDPoint(vBR.x-dGapX, vBR.y+(dF1-dUpperCover)));
	xyPointRebar.Add(CDPoint(vTL.x+dGapX, vTL.y-dGapY));
	xyPointRebar.Add(CDPoint(vTR.x-dGapX, vTR.y-dGapY));	
	
	long nCnt = 0;
	double dy = vTL.y;
	long i=0; for(i=0; i<pGW->m_nSGugan; i++)
	{
		for(long j=0; j<pGW->m_nS[i]; j++)
		{
			dy -= pGW->m_dS[i];
			xyPointRebar.Add(CDPoint(vTL.x+dGapX, dy));
			xyPointRebar.Add(CDPoint(vTR.x-dGapX, dy));
			
			nCnt++;
			if(pGW->m_dSC>0 && nCnt % (long)pGW->m_dSC==0)
				tvArrRebar.Add(CTwinVector(CDPoint(vTL.x, dy), CDPoint(vTR.x, dy)));	// ����ö��
		}
	}

	double dx = vTL.x;
	for(i=0; i<pGW->m_nUGugan; i++)
	{
		for(long j=0; j<pGW->m_nU[i]; j++)
		{
			bLeft ? dx += pGW->m_dU[i] : dx -= pGW->m_dU[i];
			xyPointRebar.Add(CDPoint(dx, vTL.y-dGapY));
		}
	}
}

void CRcBridgeRebarUtil::GetTvRebarRetainingWall(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale)
{
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;
	double	dL1 = pGW->m_dL1;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	long	nHGugan = (long)pGW->m_dEH;
	long	nVGugan = (long)pGW->m_dE;
	double	dGapX = bLeft ? dDomScale*0.72 : -dDomScale*0.72;
	double	dGapY = dDomScale*0.72;
	
	pGW->m_dCrossF2 = 0;
	pGW->m_dCrossF3 = 0;

	CTwinVector tv;
	long nSize = tvArr.GetSize();
	CDPoint xyResult(0,0);
	CDPoint vL1,vL2,vL3,vL4;
	CDPoint vP1,vHPoint1,vHPoint2,vVPoint1,vVPoint2;
	
	if(bLeft)
	{
		// Right Rebar_
		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		vVPoint1 = xyResult;

		vL1 = tvArr.GetAt(7).m_v1;
		vL2 = tvArr.GetAt(7).m_v2;
		Offset(vL1,vL2,dOffset1);
		vL3 = tvArr.GetAt(9).m_v1;
		vL4 = tvArr.GetAt(9).m_v2;
		Offset(vL3,vL4,dOffset3);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dOffset3);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY));
		vHPoint1 = xyResult;
		
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,-dL1+dOffset3);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		
		// Left Rebar_
		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;

		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		vP1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y+dGapY));

		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dL1+dOffset2);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
	}
	else
	{
		// Right Rebar_
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dOffset3);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		vVPoint1 = xyResult;

		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dOffset3);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		vP1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y+dGapY));
		
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dL1+dOffset3);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);

		// Left Rebar_
		vL1 = tvArr.GetAt(9).m_v1;
		vL2 = tvArr.GetAt(9).m_v2;
		Offset(vL1,vL2,dOffset3);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;

		vL1 = tvArr.GetAt(7).m_v1;
		vL2 = tvArr.GetAt(7).m_v2;
		Offset(vL1,vL2,dOffset1);
		vL3 = tvArr.GetAt(5).m_v1;
		vL4 = tvArr.GetAt(5).m_v2;
		Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		
		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY));
		vHPoint1 = xyResult;

		vL1 = tvArr.GetAt(5).m_v1;
		vL2 = tvArr.GetAt(5).m_v2;
		Offset(vL1,vL2,-dL1+dOffset2);
		vL3 = tvArr.GetAt(nSize-1).m_v2;
		vL4 = tvArr.GetAt(1).m_v1;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
	}

	// Bottom Top
	vL1 = tvArr.GetAt(nSize-1).m_v2;
	vL2 = tvArr.GetAt(1).m_v1;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(nSize-1).m_v1 : tvArr.GetAt(1).m_v1;
	vL4 = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;
	vHPoint2 = xyResult;

	vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(1).m_v1;
	vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(1).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
	vL3 = bLeft ? tvArr.GetAt(nSize-1).m_v1 : tvArr.GetAt(3).m_v1;
	vL4 = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(3).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;

	vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
	vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
	vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
	vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset2) : Offset(vL3,vL4,dOffset3);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	vVPoint2 = xyResult;
	tvArrRebar.Add(tv);
	bLeft ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
		  : xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY));
	
	vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
	vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset2) : Offset(vL3,vL4,dOffset3);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	bLeft ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
		  : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));
	
	// Bottom
	vL1 = tvArr.GetAt(nSize-1).m_v2;
	vL2 = tvArr.GetAt(1).m_v1;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(1).m_v1;
	vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(1).m_v2;
	Offset(vL3,vL4,dOffset2);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;

	vL1 = tvArr.GetAt(nSize-1).m_v2;
	vL2 = tvArr.GetAt(1).m_v1;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(nSize-1).m_v1 : tvArr.GetAt(9).m_v1;
	vL4 = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(9).m_v2;
	Offset(vL3,vL4,dOffset3);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;

	pGW->m_dF1 = ~(vP1-vVPoint2);

	// H ����ö�ٰ� ���ö��
	double dLength = ~(vHPoint1 - vHPoint2);
	double dStep = pGW->m_dFH = dLength / nHGugan;
	long   nShear = (long)pGW->m_dSC;
	long i=0; for(i=0; i<nHGugan; i++)
	{
		vL1 = tvArr.GetAt(nSize-1).m_v2;
		vL2 = tvArr.GetAt(1).m_v1;
		Offset(vL1,vL2,dOffset1);
		vL3 = bLeft ? tvArr.GetAt(nSize-1).m_v1 : tvArr.GetAt(1).m_v1;
		vL4 = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL3, vL4, dOffset3 + dStep*i) : Offset(vL3, vL4, dOffset2 + dStep*i);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY));
		
		if(i!=0 && i%nShear==0)
			tv.m_v1 = xyResult;
		
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(nSize-1).m_v1 : tvArr.GetAt(1).m_v1;
		vL4 = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL3, vL4, dOffset3 + dStep*i) : Offset(vL3, vL4, dOffset2 + dStep*i);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));

		if(i!=0 && i%nShear==0)
		{
			tv.m_v2 = xyResult;
			tvArrRebar.Add(tv);
		}
	}
	dLength = ~(vVPoint1 - vVPoint2);
	dStep = pGW->m_dF = dLength / nVGugan;
	for(i=0; i<nVGugan; i++)
	{
		vL1 = tvArr.GetAt(7).m_v1;
		vL2 = tvArr.GetAt(7).m_v2;
		Offset(vL1,vL2,dOffset1 + dStep*i);
		vL3 = tvArr.GetAt(5).m_v1;
		vL4 = tvArr.GetAt(5).m_v2;
		Offset(vL3, vL4, dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		bLeft ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
			  : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));

		if(i!=0 && i%nShear==0)
			tv.m_v1 = xyResult;
		
		vL3 = tvArr.GetAt(9).m_v1;
		vL4 = tvArr.GetAt(9).m_v2;
		Offset(vL3, vL4, dOffset3);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		bLeft ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
		      : xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY));

		if(i!=0 && i%nShear==0)
		{
			tv.m_v2 = xyResult;
			tvArrRebar.Add(tv);
		}
	}
}

void CRcBridgeRebarUtil::GetTvRebarFootwayBlock(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale, BOOL bCover)
{
	// ������ ����
	CTwinVectorArray tvCrossArr;
	CTwinVector tvLeft, tvRight, tvLeftCh, tvRightCh, tvLeftH, tvRightH;
	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationBridgeMid();
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvCrossSection(tvCrossArr, sta, vAng, FALSE, TRUE, FALSE);
	if(m_pRcBridgeApp->m_bCantilSlab[0])
	{
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��1"), tvLeft);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��1"), tvRight);
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��2"), tvLeftH);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��2"), tvRightH);
	}
	else
	{
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��0"), tvLeft);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��0"), tvRight);
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��1"), tvLeftH);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��1"), tvRightH);
	}

	// ���� ��ƿ���� �̸�
	if(m_pRcBridgeApp->m_bCantilSlab[0])	
		tvCrossArr.GetTvByInfo(_T("��������������2"), tvLeftCh);
	else
		tvCrossArr.GetTvByInfo(_T("��������������1"), tvLeftCh);
	
	// ���� ��ƿ���� �̸�
	if(m_pRcBridgeApp->m_bCantilSlab[1])	
		tvCrossArr.GetTvByInfo(_T("��������������2"), tvRightCh);
	else
		tvCrossArr.GetTvByInfo(_T("��������������1"), tvRightCh);
	
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;

	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	double	dE1 = pGW->m_dF1;
	double	dE2 = pGW->m_dF2;
	double	dE3 = pGW->m_dF3;
	double	dE4 = pGW->m_dF;
	double	dL1 = pGW->m_dL1;
	double	dGapX = bLeft ? dDomScale*0.72 : -dDomScale*0.72;
	double	dGapY = dDomScale*0.72;
	
	CTwinVector tv;
	long nTotal = tvArr.GetSize();
	long nSize = tvArr.GetSize()-6;
	CDPoint xyResult(0,0);
	CDPoint vL1,vL2,vL3,vL4;
	CDPoint vT1, vT2, vM1, vM2, vB1, vB2;
	
	CDPoint	vOrigin = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v1;

	// ������...
	long nCase = 0;
	double dSideDist = bLeft ? tvLeft.GetLength() : tvRight.GetLength();
	xyResult = bLeft ? tvArr.GetAt(1).m_v1 : tvArr.GetAt(nSize-1).m_v2;
	xyResult.x = bLeft ? xyResult.x+dOffset2 : xyResult.x-dOffset3;
	tv.m_v1 = vL1 = xyResult;
	xyResult = CDPoint(xyResult.x, vOrigin.y-dL1);
	tv.m_v2 = vL2 = xyResult;
	vL3 = bLeft ? tvLeft.m_v1 : tvRight.m_v1;
	vL4 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
	BOOL bCross = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	if(!bCross && dSideDist>dL1)		// ����
	{
		tvArrRebar.Add(tv);
	}
	else
	{
		long nSide = bLeft ? 0 : 1;
		if(m_pRcBridgeApp->m_nSlabChemfer[nSide] == 1)	// ����� ���°� �����̸鼭 �����ϸ� ����� �����
		{	
			nCase = 1;
			CDPoint vUnit = bLeft ? (tvLeftCh.m_v2 - tvLeftCh.m_v1).Unit() : (tvRightCh.m_v2 - tvRightCh.m_v1).Unit();
			tv.m_v2 = CDPoint(tv.m_v1.x + dL1*vUnit.x , tv.m_v1.y + dL1*vUnit.y);
			tvArrRebar.Add(tv);
		}
		else
		{
			// ����(��� �ƴϰ� �������� ����)�� �����ϸ� 45�� ����� �׷��� ������ ����
			vL3 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
			vL4 = bLeft ? tvLeftH.m_v1 : tvRightH.m_v1;
			BOOL bMeet = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);	// � �𼭸��� �������� 
			if(bCross || bMeet)
			{
				nCase = 2;
				CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
				tv.m_v2 = GetXyRotateVector(tv.m_v1, tv.m_v2, ptAng);
			}
			tvArrRebar.Add(tv);
		}
	}
	//

	vL1 = bLeft ? tvArr.GetAt(1).m_v1 : tvArr.GetAt(nSize-1).m_v1;
	vL2 = bLeft ? tvArr.GetAt(1).m_v2 : tvArr.GetAt(nSize-1).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset2) : Offset(vL1,vL2,dOffset3);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;
	vB1 = xyResult;

	vL1 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
	vL2 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset2) : Offset(vL1,vL2,dOffset3);
	vL3 = tvArr.GetAt(7).m_v1;
	vL4 = tvArr.GetAt(7).m_v2;
	Offset(vL3,vL4,dOffset1);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;
	vT1 = xyResult;

	vL1 = tvArr.GetAt(7).m_v1;
	vL2 = tvArr.GetAt(7).m_v2;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
	vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
	bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;
	vT2 = xyResult;

	vL1 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
	vL2 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	vB2 = xyResult;
	
	// ������...
	tv.m_v1 = vL1 = xyResult;
	xyResult = CDPoint(xyResult.x, vOrigin.y-dL1);
	tv.m_v2 = vL2 = xyResult;
	vL3 = bLeft ? tvLeft.m_v1 : tvRight.m_v1;
	vL4 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	//if(!bCross && dSideDist>dL1)		// ����
	if(nCase==0)
	{
		tvArrRebar.Add(tv);
	}
	else
	{
		//long nSide = bLeft ? 0 : 1;
		if(nCase==1)	//if(m_pRcBridgeApp->m_nSlabChemfer[nSide] == 1)	// ����� ���°� �����̸鼭 �����ϸ� ����� �����
		{	
			CDPoint vUnit = bLeft ? (tvLeftCh.m_v2 - tvLeftCh.m_v1).Unit() : (tvRightCh.m_v2 - tvRightCh.m_v1).Unit();
			tv.m_v2 = CDPoint(tv.m_v1.x + dL1*vUnit.x , tv.m_v1.y + dL1*vUnit.y);
			tvArrRebar.Add(tv);
		}
		else
		{
			// ����(��� �ƴϰ� �������� ����)�� �����ϸ� 45�� ����� �׷��� ������ ����
			//vL3 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
			//vL4 = bLeft ? tvLeftH.m_v1 : tvRightH.m_v1;
			//BOOL bMeet = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);	// � �𼭸��� �������� 
			if(nCase==2)	//if(bCross || bMeet)
			{
				CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
				tv.m_v2 = GetXyRotateVector(tv.m_v1, tv.m_v2, ptAng);
			}
			tvArrRebar.Add(tv);
		}
	}

	CDPoint vP1, vP2; // ���� ��±��� ���� ö��
	if(bCover)
	{
		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL4 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		vP1 = xyResult;

		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(nSize-1).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		vP2 = xyResult;

		tv.m_v1 = xyResult;
		tv.m_v2 = CDPoint(xyResult.x, vOrigin.y-dL1);
		tvArrRebar.Add(tv);

		// ���ö��
		double dDist = ~(vP1-vP2);
		double dStep = dDist / dE4;
		long i=0; for(i=0; i<dE4; i++)
		{
			xyResult = CDPoint(vP1.x, vP1.y-(dStep*i));
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
		}
		
		// �����
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
		vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset2) : Offset(vL3,vL4,dOffset3);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		
		// ���ö��
		vM1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY));
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
		vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));
		vM2 = xyResult;
		//
		
		vL1 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
		vL2 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
		bLeft ? Offset(vL1,vL2,-50) : Offset(vL1,vL2,-50);
		vL3 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL4 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));

		vL1 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
		vL2 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
		bLeft ? Offset(vL1,vL2,-50) : Offset(vL1,vL2,-50);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(nSize-1).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);

		tv.m_v1 = xyResult;
		tv.m_v2 = CDPoint(xyResult.x, vOrigin.y-dL1);
		tvArrRebar.Add(tv);
	}
	else	// �����
	{	
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
		vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset2) : Offset(vL3,vL4,dOffset3);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		
		// ���ö��
		vM1 = xyResult;
		xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY));
		vL1 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL2 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(9).m_v1 : tvArr.GetAt(5).m_v1;
		vL4 = bLeft ? tvArr.GetAt(9).m_v2 : tvArr.GetAt(5).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY));
		vM2 = xyResult;
		//
		
		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = bLeft ? tvArr.GetAt(11).m_v1 : tvArr.GetAt(3).m_v1;
		vL4 = bLeft ? tvArr.GetAt(11).m_v2 : tvArr.GetAt(3).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset3) : Offset(vL3,vL4,dOffset2);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = vP1 = xyResult;

		vL1 = bLeft ? tvArr.GetAt(13).m_v1 : tvArr.GetAt(1).m_v1;
		vL2 = bLeft ? tvArr.GetAt(13).m_v2 : tvArr.GetAt(1).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(nSize-1).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vP2 = xyResult;
		tvArrRebar.Add(tv);

		tv.m_v1 = xyResult;
		tv.m_v2 = CDPoint(xyResult.x, vOrigin.y-dL1);
		tvArrRebar.Add(tv);

		// ���ö��
		double dDist = ~(vM2-vP1);
		double dStep = dDist / dE3;
		long i=0; for(i=1; i<dE3; i++)
		{
			xyResult = bLeft ? CDPoint(vM2.x+(dStep*i), vM2.y) : CDPoint(vM2.x-(dStep*i), vM2.y);
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x, xyResult.y-dGapY));
		}
		dDist = ~(vP1-vP2);
		dStep = dDist / dE4;
		for(i=0; i<dE4; i++)
		{
			xyResult = CDPoint(vP1.x, vP1.y-(dStep*i));
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
		}
	}
	// ���ö�� (T ~ M)
	double dDist = ~(vT1-vM1);
	double dStep = dDist / dE1;
	long i=0; for(i=0; i<dE1; i++)
	{
		xyResult = CDPoint(vT1.x, vT1.y - (dStep*i));
		tv.m_v1 = xyResult;
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y));
	
		xyResult = CDPoint(vT2.x, vT2.y - (dStep*i));
		tv.m_v2 = xyResult;
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));

		if(bCover)	// ����ö��
			tvArrRebar.Add(tv);		
	}

	dDist = ~(vM1-vB1);
	dStep = dDist / dE2;
	for(i=0; i<dE2; i++)
	{
		xyResult = CDPoint(vB1.x, vB1.y + (dStep*i));
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y+dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y));
		xyResult = CDPoint(vB2.x, vB2.y + (dStep*i));
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
	}
/*
	// Hunch
	vL1 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
	vL2 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
	CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
	bLeft ? vL2 = GetXyRotateVector(vL1, vL2, ptAng) : vL1 = GetXyRotateVector(vL2, vL1, ptAng);
	//tvArrRebar.Add(CTwinVector(vL1, vL2));
	vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
	vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;

	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	bLeft ? Offset(tv.m_v1,tv.m_v2,dOffset3) : Offset(tv.m_v1,tv.m_v2,-dOffset3);
	tvArrRebar.Add(tv);
*/
}

void CRcBridgeRebarUtil::GetTvRebarTotalHole(CGuardWallRC *pGW, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRebar, CDPointArray &xyPointRebar, double dDomScale, BOOL bCover)
{
	// ������ ����
	CTwinVectorArray tvCrossArr;
	CTwinVector tvLeft, tvRight, tvLeftCh, tvRightCh, tvLeftH, tvRightH;
	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationBridgeMid();
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvCrossSection(tvCrossArr, sta, vAng, FALSE, TRUE, FALSE);
	if(m_pRcBridgeApp->m_bCantilSlab[0])
	{
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��1"), tvLeft);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��1"), tvRight);
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��2"), tvLeftH);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��2"), tvRightH);
	}
	else
	{
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��0"), tvLeft);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��0"), tvRight);
		tvCrossArr.GetTvByInfo(_T("������������ Ⱦ��1"), tvLeftH);
		tvCrossArr.GetTvByInfo(_T("����������� Ⱦ��1"), tvRightH);
	}

	// ���� ��ƿ���� �̸�
	if(m_pRcBridgeApp->m_bCantilSlab[0])	
		tvCrossArr.GetTvByInfo(_T("��������������2"), tvLeftCh);
	else
		tvCrossArr.GetTvByInfo(_T("��������������1"), tvLeftCh);
	
	// ���� ��ƿ���� �̸�
	if(m_pRcBridgeApp->m_bCantilSlab[1])	
		tvCrossArr.GetTvByInfo(_T("��������������2"), tvRightCh);
	else
		tvCrossArr.GetTvByInfo(_T("��������������1"), tvRightCh);
	
	// ���� ��ȣ������ ����������
	BOOL bLeft = pGW->IsLeftGuardFence() ? TRUE : FALSE;

	double	dH1 = pGW->m_dH1;
	double	dH2 = pGW->m_dH2;
	double	dH3 = pGW->m_dH3;
	double	dOffset1 = pGW->m_ddc1;		// Top �Ǻ�
	double	dOffset2 = pGW->m_ddc2;		// Left �Ǻ�
	double	dOffset3 = pGW->m_ddc3;		// Right �Ǻ�
	double	dE1 = pGW->m_dF1;
	double	dE2 = pGW->m_dF2;
	double	dE3 = pGW->m_dF3;
	double	dL1 = pGW->m_dL1;
	double	dGapX = bLeft ? dDomScale*0.72 : -dDomScale*0.72;
	double	dGapY = dDomScale*0.72;
	
	CTwinVector tv;
	long nTotal = tvArr.GetSize();
	long nSize = tvArr.GetSize()-6;
	CDPoint xyResult(0,0);
	CDPoint vL1,vL2,vL3,vL4;
	CDPoint vT1, vT2, vB1, vB2;

	CDPoint	vOrigin = bLeft ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v1;
	long m=0, n=0;
	
	// ������...
	long nCase = 0;
	double dSideDist = bLeft ? tvLeft.GetLength() : tvRight.GetLength();
	xyResult = bLeft ? tvArr.GetAt(1).m_v1 : tvArr.GetAt(nSize-1).m_v2;
	xyResult.x = bLeft ? xyResult.x+dOffset2 : xyResult.x-dOffset3;
	tv.m_v1 = vL1 = xyResult;
	xyResult = CDPoint(xyResult.x, vOrigin.y-dL1);
	tv.m_v2 = vL2 = xyResult;
	vL3 = bLeft ? tvLeft.m_v1 : tvRight.m_v1;
	vL4 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
	BOOL bCross = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	if(!bCross && dSideDist>dL1)		// ����
	{
		tvArrRebar.Add(tv);
	}
	else
	{
		long nSide = bLeft ? 0 : 1;
		if(m_pRcBridgeApp->m_nSlabChemfer[nSide] == 1)	// ����� ���°� �����̸鼭 �����ϸ� ����� �����
		{	
			nCase = 1;
			CDPoint vUnit = bLeft ? (tvLeftCh.m_v2 - tvLeftCh.m_v1).Unit() : (tvRightCh.m_v2 - tvRightCh.m_v1).Unit();
			tv.m_v2 = CDPoint(tv.m_v1.x + dL1*vUnit.x , tv.m_v1.y + dL1*vUnit.y);
			tvArrRebar.Add(tv);
		}
		else
		{
			// ����(��� �ƴϰ� �������� ����)�� �����ϸ� 45�� ����� �׷��� ������ ����
			vL3 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
			vL4 = bLeft ? tvLeftH.m_v1 : tvRightH.m_v1;
			BOOL bMeet = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);	// � �𼭸��� �������� 
			if(bCross || bMeet)
			{
				nCase = 2;
				CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
				tv.m_v2 = GetXyRotateVector(tv.m_v1, tv.m_v2, ptAng);
			}
			tvArrRebar.Add(tv);
		}
	}
	//
		
	vL1 = bLeft ? tvArr.GetAt(1).m_v1 : tvArr.GetAt(nSize-1).m_v1;
	vL2 = bLeft ? tvArr.GetAt(1).m_v2 : tvArr.GetAt(nSize-1).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset2) : Offset(vL1,vL2,dOffset3);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;
	vB1 = xyResult;

	if(dH1>0 && dH2==0 && dH3==0)	m=1, n=13;
	if(dH1==0 && dH2>0 && dH3==0)	m=3, n=11;
	if(dH1==0 && dH2==0 && dH3>0)	m=5, n=9;
	vL1 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
	vL2 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
	bLeft ? Offset(vL1,vL2,dOffset2) : Offset(vL1,vL2,dOffset3);
	vL3 = tvArr.GetAt(7).m_v1;
	vL4 = tvArr.GetAt(7).m_v2;
	Offset(vL3,vL4,dOffset1);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;
	vT1 = xyResult;

	vL1 = tvArr.GetAt(7).m_v1;
	vL2 = tvArr.GetAt(7).m_v2;
	Offset(vL1,vL2,dOffset1);
	vL3 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
	vL4 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
	bLeft ? Offset(vL3,vL4,-dOffset2) : Offset(vL3,vL4,-dOffset3);
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	tv.m_v1 = xyResult;
	vT2 = xyResult;

	vL1 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
	vL2 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
	bLeft ? Offset(vL1,vL2,-dOffset2) : Offset(vL1,vL2,-dOffset3);
	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	tvArrRebar.Add(tv);
	vB2 = xyResult;
	
	// ������...
	tv.m_v1 = vL1 = xyResult;
	xyResult = CDPoint(xyResult.x, vOrigin.y-dL1);
	tv.m_v2 = vL2 = xyResult;
	vL3 = bLeft ? tvLeft.m_v1 : tvRight.m_v1;
	vL4 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	//if(!bCross && dSideDist>dL1)		// ����
	if(nCase==0)
	{
		tvArrRebar.Add(tv);
	}
	else
	{
		//long nSide = bLeft ? 0 : 1;
		if(nCase==1)	//if(m_pRcBridgeApp->m_nSlabChemfer[nSide] == 1)	// ����� ���°� �����̸鼭 �����ϸ� ����� �����
		{	
			CDPoint vUnit = bLeft ? (tvLeftCh.m_v2 - tvLeftCh.m_v1).Unit() : (tvRightCh.m_v2 - tvRightCh.m_v1).Unit();
			tv.m_v2 = CDPoint(tv.m_v1.x + dL1*vUnit.x , tv.m_v1.y + dL1*vUnit.y);
			tvArrRebar.Add(tv);
		}
		else
		{
			// ����(��� �ƴϰ� �������� ����)�� �����ϸ� 45�� ����� �׷��� ������ ����
			//vL3 = bLeft ? tvLeft.m_v2 : tvRight.m_v2;
			//vL4 = bLeft ? tvLeftH.m_v1 : tvRightH.m_v1;
			//BOOL bMeet = GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);	// � �𼭸��� �������� 
			if(nCase==2)	//if(bCross || bMeet)
			{
				CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
				tv.m_v2 = GetXyRotateVector(tv.m_v1, tv.m_v2, ptAng);
			}
			tvArrRebar.Add(tv);
		}
	}

	CDPoint vP1, vP2; // ���� ��±��� ���� ö��
	if(bCover)
	{
		if(dH1>0 && dH2==0 && dH3==0)	m=13, n=1;
		if(dH1==0 && dH2>0 && dH3==0)	m=11, n=3;
		if(dH1==0 && dH2==0 && dH3>0)	m=9,  n=5;
		vL1 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
		vL2 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;
		vP1 = xyResult;

		vL1 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
		vL2 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(nSize-1).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		vP2 = xyResult;

		tv.m_v1 = xyResult;
		tv.m_v2 = CDPoint(xyResult.x, vOrigin.y-dL1);
		tvArrRebar.Add(tv);

		// ���ö��
		double dDist = ~(vP1-vP2);
		double dStep = dDist / dE3;
		long i=0; for(i=0; i<dE3; i++)
		{
			xyResult = CDPoint(vP1.x, vP1.y-(dStep*i));
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
		}
	}
	else	// �����
	{	
		if(dH1>0 && dH2==0 && dH3==0)	m=1, n=13;
		if(dH1==0 && dH2>0 && dH3==0)	m=3, n=11;
		if(dH1==0 && dH2==0 && dH3>0)	m=5, n=9;
		vL1 = tvArr.GetAt(7).m_v1;
		vL2 = tvArr.GetAt(7).m_v2;
		Offset(vL1,vL2,dOffset1);
		vL3 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
		vL4 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
		bLeft ? Offset(vL3,vL4,dOffset2) : Offset(vL3,vL4,dOffset3);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v1 = xyResult;

		if(dH1>0 && dH2==0 && dH3==0)	m=13, n=1;
		if(dH1==0 && dH2>0 && dH3==0)	m=11, n=3;
		if(dH1==0 && dH2==0 && dH3>0)	m=9,  n=5;
		vL1 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
		vL2 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(7).m_v1;
		vL4 = tvArr.GetAt(7).m_v2;
		Offset(vL3,vL4,dOffset1);
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = xyResult;
		tvArrRebar.Add(tv);
		tv.m_v1 = vP1 = xyResult;

		vL1 = bLeft ? tvArr.GetAt(m).m_v1 : tvArr.GetAt(n).m_v1;
		vL2 = bLeft ? tvArr.GetAt(m).m_v2 : tvArr.GetAt(n).m_v2;
		bLeft ? Offset(vL1,vL2,dOffset3) : Offset(vL1,vL2,dOffset2);
		vL3 = tvArr.GetAt(1).m_v1;
		vL4 = tvArr.GetAt(nSize-1).m_v2;
		GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
		tv.m_v2 = vP2 = xyResult;
		tvArrRebar.Add(tv);

		tv.m_v1 = xyResult;
		tv.m_v2 = CDPoint(xyResult.x, vOrigin.y-dL1);
		tvArrRebar.Add(tv);
		
		// ���ö��
		double dDist = ~(vT2-vP1);
		double dStep = dDist / dE2;
		long i=0; for(i=1; i<dE2; i++)
		{
			xyResult = bLeft ? CDPoint(vT2.x+(dStep*i), vT2.y) : CDPoint(vT2.x-(dStep*i), vT2.y);
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x, xyResult.y-dGapY));
		}
		dDist = ~(vP1-vP2);
		dStep = dDist / dE3;
		for(i=0; i<dE3; i++)
		{
			xyResult = CDPoint(vP1.x, vP1.y-(dStep*i));
			i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
				 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
			/*if(i==dE3-1)
			{
				xyResult = CDPoint(vP1.x, vP1.y-(dStep*dE3));
				xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY));
			}*/
		}
	}
	// ���ö�� (T ~ B)
	double dDist = ~(vT1-vB1);
	double dStep = dDist / dE1;
	long i=0; for(i=0; i<dE1; i++)
	{
		xyResult = CDPoint(vT1.x, vT1.y - (dStep*i));
		tv.m_v1 = xyResult;
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y-dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y));
		if(i==dE1-1)
		{
			xyResult = CDPoint(vT1.x, vT1.y - (dStep*dE1));
			xyPointRebar.Add(CDPoint(xyResult.x+dGapX, xyResult.y+dGapY));
		}
		xyResult = CDPoint(vT2.x, vT2.y - (dStep*i));
		tv.m_v2 = xyResult;
		i==0 ? xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y-dGapY))
			 : xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y));
		if(i==dE1-1)
		{
			xyResult = CDPoint(vT2.x, vT2.y - (dStep*dE1));
			xyPointRebar.Add(CDPoint(xyResult.x-dGapX, xyResult.y+dGapY));
		}	
	}
/*
	// Hunch
	vL1 = bLeft ? tvArr.GetAt(nTotal-6).m_v1 : tvArr.GetAt(nTotal-4).m_v1;
	vL2 = bLeft ? tvArr.GetAt(nTotal-6).m_v2 : tvArr.GetAt(nTotal-4).m_v2;
	CDPoint ptAng = bLeft ? ToDPointFrDegree(45) : ToDPointFrDegree(-45);
	bLeft ? vL2 = GetXyRotateVector(vL1, vL2, ptAng) : vL1 = GetXyRotateVector(vL2, vL1, ptAng);
	//tvArrRebar.Add(CTwinVector(vL1, vL2));
	vL3 = bLeft ? tvArr.GetAt(5).m_v1 : tvArr.GetAt(9).m_v1;
	vL4 = bLeft ? tvArr.GetAt(5).m_v2 : tvArr.GetAt(9).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v1 = xyResult;

	vL3 = tvArr.GetAt(1).m_v1;
	vL4 = tvArr.GetAt(nSize-1).m_v2;
	GetXyMatchSegAndSeg(vL1,vL2,vL3,vL4,xyResult);
	tv.m_v2 = xyResult;
	bLeft ? Offset(tv.m_v1,tv.m_v2,dOffset3) : Offset(tv.m_v1,tv.m_v2,-dOffset3);
	tvArrRebar.Add(tv);
*/
}

void CRcBridgeRebarUtil::MakeGuardFenceRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	double	dH1 = pGW->m_dH1;
	double	dH2 = pGW->m_dH2;
	double	dH3 = pGW->m_dH3;
	double	dW1 = pGW->m_dW1;
	double	dW2 = pGW->m_dW2;
	double	dW3 = pGW->m_dW3;
	double	dW4 = pGW->m_dW4;
	double	dW5 = pGW->m_dW5;
	
	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	// ��ȣ�� rebar���� = 17(except Dot & Shear Rebar_) 
	//long nRebar = 17;
	//if( (dH1==0&&dH2==0&&dH3>0) || (dH1>0&&dH2==0&&dH3==0) || (dH1==0&&dH2>0&&dH3==0) )
	//	nRebar = 9;
	// 2006.11.20 added by jaeho
	//if( dH1 > 0 && dH3 > 0)
	//	nRebar	= 11;
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	//long nShearRebar = tvArrRebar.GetSize() - nRebar;
	
	//32076 ���� Ÿ���̸鼭 (�� ��ȣ���� H�� ���̰� �����ϸ鼭 ������ ��쿡 ö�� ����� �ݴ�� ������ ����..) ù ö���� ������ �ֱ� ���� ����
	//CR1�� ���Ҷ� �ѹ��� ���δ�.
	BOOL bRightH = FALSE;
	if((dH1!=0 && dH2!=0 &&!bLeft ) || (dH2!=0 && dH3!=0 &&!bLeft) || (dH1!=0 && dH3!=0 &&!bLeft ))
		bRightH = TRUE;
	//}

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}

	long n = 0;
	double dCover = 0, dSpace = 0;
	
	// Left
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		pRB->m_sDescription = "����ö��1";
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("C1") : _T("CL1");
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();		// 0
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();		// 1
			if(dH1!=0)	pRB->m_dLengthMax_A += tvArrRebar[n++].GetLength();	// 2
			if(dH2!=0)	pRB->m_dLengthMax_A += tvArrRebar[n++].GetLength();	// 3
			if(dH3!=0)	pRB->m_dLengthMax_A += tvArrRebar[n++].GetLength();	// 4	

			if(pGW->m_nTypeRebar == 1)
			{
				pRB->m_nTypeRebar	= 97;
				GetSwap(pRB->m_dLengthMax_A, pRB->m_dLengthMax_B);
				pRB->m_dLengthMax_C	= tvArrRebar.GetAt(6).GetLength();
				pRB->m_dLengthMax_D	= 50;
			}
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("C1") : _T("CR1");
			if( (dW1==0&&dW2==0&&dW3>0&&dW4>0&&dW5>0) || (dW1>0&&dW2>0&&dW3>0&&dW4==0&&dW5==0) )	// ������
			{
				pRB->m_nTypeRebar = 45;
				pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();	// 0
				pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 1 + 2
				pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 3
				pRB->m_dLengthMax_D = tvArrRebar[n++].GetLength();	// 4
				if(pGW->m_nTypeRebar == 1)
				{
					pRB->m_nTypeRebar	= 2;
					pRB->m_dLengthMax_A	= pRB->m_dLengthMax_D;
					pRB->m_dLengthMax_B	= 50;
					pRB->m_dLengthMax_C	= 0;
					pRB->m_dLengthMax_D	= 0;
				}
			}
			// 32076
			else if(bRightH)
			{
				long nx = 0;
				CTwinVectorArray tvArrTempRight;
				CTwinVector tvTempRight;

				for(long ix =0; ix < tvArrRebar.GetSize(); ix++)
				{
					tvTempRight = tvArrRebar.GetAt(ix);
// 					CString strTemp = _T("");

					CString strTemp = tvTempRight.m_sInfo;

					if(strTemp.Find(_T("leftrebar")) != -1)
					{
						tvArrTempRight.Add(tvTempRight);
					}
				}

				if(dH1>0 && dH2>0 && dH3>0)
				{
					pRB->m_nTypeRebar = 45;
					pRB->m_dLengthMax_A = tvArrTempRight[nx++].GetLength();	// 7
					pRB->m_dLengthMax_B = tvArrTempRight[nx++].GetLength()+tvArrTempRight[n++].GetLength(); // 8 + 9
					pRB->m_dLengthMax_C = tvArrTempRight[nx++].GetLength();	// 10
					pRB->m_dLengthMax_D = tvArrTempRight[nx++].GetLength();	// 11	

					if(pGW->m_nTypeRebar == 1)
					{
						pRB->m_nTypeRebar	= 2;
						pRB->m_dLengthMax_A	= pRB->m_dLengthMax_D;
						pRB->m_dLengthMax_B	= 50;
						pRB->m_dLengthMax_C	= 0;
						pRB->m_dLengthMax_D	= 0;
					}
				}
				else if( (dH1==0&&dH2==0&&dH3>0) || (dH1>0&&dH2==0&&dH3==0) || (dH1==0&&dH2>0&&dH3==0) )
				{
					pRB->m_nTypeRebar = pGW->IsTypeHDanRailDetectGravel() ? 8 : 2;
					pRB->m_dLengthMax_B = tvArrTempRight[nx++].GetLength();	// 7
					pRB->m_dLengthMax_A = tvArrTempRight[nx++].GetLength()+tvArrTempRight[nx++].GetLength(); // 8 + 9			
				}
				else if(dH1 > 0 && dH3 > 0)
				{
					pRB->m_nTypeRebar	= 13;
					pRB->m_dLengthMax_A	= tvArrTempRight[nx++].GetLength();
					pRB->m_dLengthMax_B	= tvArrTempRight[nx++].GetLength() + tvArrTempRight[nx++].GetLength();
					pRB->m_dLengthMax_C	= tvArrTempRight[nx++].GetLength();
				}
				n += nx;
			}
			//}
			else
			{
				pRB->m_nTypeRebar = 2;
				pRB->m_dLengthMax_B	= tvArrRebar[n++].GetLength();	// 0
				pRB->m_dLengthMax_A	= tvArrRebar[n++].GetLength() + tvArrRebar[n++].GetLength();	// 1 + 2

			}
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}


	// Top
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_sMark = bLeft ? _T("CL2") : _T("CR2");
		pRB->m_sMark = bDuplicate ? _T("C2") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��2";
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pGW->m_nTypeRebar == 1)
		{
			pRB->m_nEa	= 0;
		}
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 4;
		pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 4
		pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();	// 5
		pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 6


		//n++;	// �� ��ĭ �ǳʶ���? (2006.10.9) �ǳʶٸ鼭 ���谡 ���� �ʾƼ� ����..
		// �ٸ� ������ ��ȣ���� ������ ��ġ������ Ȯ������ �� ����.
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = pGW->m_ddc1;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// Right
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_3;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		pRB->m_sDescription = "����ö��3";
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("C3") : _T("CL3");
			if(dH1>0 && dH2>0 && dH3>0)
			{
				pRB->m_nTypeRebar = 45;
				pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();	// 7
				pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 8 + 9
				pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 10
				pRB->m_dLengthMax_D = tvArrRebar[n++].GetLength();	// 11	

				if(pGW->m_nTypeRebar == 1)
				{
					pRB->m_nTypeRebar	= 2;
					pRB->m_dLengthMax_A	= pRB->m_dLengthMax_D;
					pRB->m_dLengthMax_B	= 50;
					pRB->m_dLengthMax_C	= 0;
					pRB->m_dLengthMax_D	= 0;
				}
			}
			else if( (dH1==0&&dH2==0&&dH3>0) || (dH1>0&&dH2==0&&dH3==0) || (dH1==0&&dH2>0&&dH3==0) )
			{
				pRB->m_nTypeRebar = pGW->IsTypeHDanRailDetectGravel() ? 8 : 2;
				pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 7
				pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 8 + 9			
			}
			else if(dH1 > 0 && dH3 > 0)
			{
				pRB->m_nTypeRebar	= 13;
				pRB->m_dLengthMax_A	= tvArrRebar[n++].GetLength();
				pRB->m_dLengthMax_B	= tvArrRebar[n++].GetLength() + tvArrRebar[n++].GetLength();
				pRB->m_dLengthMax_C	= tvArrRebar[n++].GetLength();
			}
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("C3") : _T("CR3");

			//32076
			if(bRightH)
			{
				long nx = 0;
				CTwinVectorArray tvArrTempLeft;
				CTwinVector tvTempLeft;

				for(long ix =0; ix < tvArrRebar.GetSize(); ix++)
				{
					tvTempLeft = tvArrRebar.GetAt(ix);
// 					CString strTemp = _T("");

					CString strTemp = tvTempLeft.m_sInfo;

					if(strTemp.Find(_T("rightrebar")) != -1)
					{
						tvArrTempLeft.Add(tvTempLeft);
					}
				}

				pRB->m_nTypeRebar = 2;
				pRB->m_dLengthMax_B = tvArrTempLeft[nx++].GetLength();		// 0
				pRB->m_dLengthMax_A = tvArrTempLeft[nx++].GetLength();		// 1
				if(dH1!=0)	pRB->m_dLengthMax_A += tvArrTempLeft[nx++].GetLength();	// 2
				if(dH2!=0)	pRB->m_dLengthMax_A += tvArrTempLeft[nx++].GetLength();	// 3
				if(dH3!=0)	pRB->m_dLengthMax_A += tvArrTempLeft[nx++].GetLength();	// 4	

				if(pGW->m_nTypeRebar == 1)
				{
					pRB->m_nTypeRebar	= 97;
					GetSwap(pRB->m_dLengthMax_A, pRB->m_dLengthMax_B);
					pRB->m_dLengthMax_C	= tvArrRebar.GetAt(6).GetLength();
					pRB->m_dLengthMax_D	= 50;
				}

				n += nx;
			}
			else
			{
				pRB->m_nTypeRebar = 2;
				pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 7
				if(dH1>0 && dH2>0 && dH3>0)
					pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength()
					+tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength();	// 8 + 9 + 10 + 11
				else if( (dH1==0&&dH2==0&&dH3>0) || (dH1>0&&dH2==0&&dH3==0) || (dH1==0&&dH2>0&&dH3==0) )
					pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength();	// 8 + 9
				else 
					pRB->m_dLengthMax_A	= tvArrRebar[n++].GetLength() + tvArrRebar[n++].GetLength() + tvArrRebar[n++].GetLength();	// 8 + 9  + 10

				if(pGW->m_nTypeRebar == 1)
				{
					pRB->m_nTypeRebar	= 97;
					GetSwap(pRB->m_dLengthMax_A, pRB->m_dLengthMax_B);
					pRB->m_dLengthMax_C	= tvArrRebar.GetAt(6).GetLength();
					pRB->m_dLengthMax_D	= 50;
				}
			}		
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// Right2
	if((dH1>0 && dH2>0 && dH3>0) && ((dW1>0 && dW2>0) || (dW4>0 && dW5>0)))
	{
		pRB = pGW->m_pArrRebar[GW_REBAR_VERT_4];
		if(pRB->m_bLock == FALSE)
		{
			if(pPrevGW != NULL)
				pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_4];

			pRB->InitData();
			pRB->m_nIdxNumber = GW_REBAR_VERT_4;
			pRB->m_sMark = bLeft ? _T("CL4") : _T("CR4");
			pRB->m_sMark = bDuplicate ? _T("C4") : pRB->m_sMark;
			pRB->m_sDescription = "����ö��4";
			pRB->m_dDia = pGW->m_dVR;
			pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_nTypeRebar = 13;
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength();
			pRB->m_dLengthMax_C = tvArrRebar.GetSize() > n ? tvArrRebar[n++].GetLength() : 0;
			if(pGW->m_nTypeRebar == 1)
			{
				pRB->m_nTypeRebar	= 45;
				pRB->m_dLengthMax_D	= 50;
			}
			
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
			dSpace = pGW->m_dCTC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
		}
	}

	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("CL5") : _T("CR5");
		pRB->m_sMark = bDuplicate ? _T("C5") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;

		SetRebarLsdOptionGuardWall(pRB->m_pOptionLsd, pGW);
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö��
	CRebarPlacing rb;
	CDPointArray xyArrMatch;
	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	CDPointArray xyArrShearRebar[4];
	GetXyRebarCR6789OfGueardFence(pGW, xyArrShearRebar[0], xyArrShearRebar[1], xyArrShearRebar[2], xyArrShearRebar[3], tvArrRebar);
	CString sMark(_T("")), sDescription(_T(""));

	long nIdx = 0;
	for(long sr = 0; sr < 4; sr++)
	{
		CDPointArray *pXyArr	= &xyArrShearRebar[sr];
		
		pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1+sr];
		if(pRB->m_bLock == FALSE)
		{
			if(pPrevGW != NULL)
				pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1+sr];
			pRB->InitData();

			if(pXyArr->GetSize() == 0) continue;
			pRB->m_nEa = (long)((nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC)) * pXyArr->GetSize());

			/////////////////////////////////////////////// DrawRebarMarkGuardFence()�� ���� ����
			if(bDuplicate)
				sMark.Format(_T("C%d"),6+nIdx);
			else
			{
				if(bLeft)
					sMark.Format(_T("CL%d"),6+nIdx);
				else
					sMark.Format(_T("CR%d"),6+nIdx);
			}
			sDescription.Format(_T("����ö��%d"),sr+1);
			nIdx++;
			//////////////////////////////////////////

			pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
			pRB->m_sMark = sMark;
			pRB->m_sDescription = sDescription;
			pRB->m_dDia = pGW->m_dSR;
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_nTypeRebar	= 18;
			for(long x = 0; x < pXyArr->GetSize(); x++)
			{
				rb.GetXyMatchTvArrAndLine(tvArrRebar, pXyArr->GetAt(x), CDPoint(1, 0), xyArrMatch);
				if(xyArrMatch.GetSize() < 2) continue;
				xyArrMatch.Sort(TRUE);

				double dDist	= ~(xyArrMatch.GetAt(0) - xyArrMatch.GetAt(xyArrMatch.GetSize()-1));
				if(x == 0)
				{
					pRB->m_dLengthMax_B = dDist;
					pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
				}
				else
				{
					pRB->m_dLengthMax_B	= max(dDist, pRB->m_dLengthMax_B);
					pRB->m_dLengthMin_B	= min(dDist, pRB->m_dLengthMin_B);
				}
			}
			pRB->m_dLengthMax_C	= pRB->m_dLengthMax_B;
			pRB->m_dLengthMin_C	= pRB->m_dLengthMin_B;

			pRB->m_dLengthMax_A = pGW->m_dCTC + pGW->m_dVR + pGW->m_dSR;
			pRB->m_dLengthMin_A = 0;

		
			pRB->m_Fy		= pGW->m_dFy;
			pRB->m_dRateAdd	= MAKE_RATE_UTIL;
			dCover = 0;
			dSpace = pGW->m_dCTC * pGW->m_dEC;
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, dCover, dSpace);	
		}
	}
	
	/*
	
	long nShearSu=0;
	long nGugan=0, nPrevGugan=-1;
	long nSize = tvArrRebar.GetSize();
	for(i=nSize-nShearRebar; i<nSize; i++)
	{		
		// ����
		CTwinVector tv = tvArrRebar.GetAt(i);
		double dOriginY = tvArrRebar.GetAt(1).m_v1.y + pGW->m_dF2;
		if(tv.m_v1.y < dOriginY + pGW->m_dCrossF2-0.5)
			nGugan = 0;
		else if(tv.m_v1.y >= (dOriginY+pGW->m_dCrossF2-0.5) && tv.m_v1.y <= (dOriginY+pGW->m_dCrossF2+pGW->m_dCrossF3+0.5))
			nGugan = 1;
		else
			nGugan = 2;
		if( (dH1==0&&dH2==0&&dH3>0) || (dH1>0&&dH2==0&&dH3==0) || (dH1==0&&dH2>0&&dH3==0) )
			nGugan = 0;
	
		BOOL bMerge	= TRUE;
		if(nGugan != nPrevGugan)
		{
			bMerge = FALSE;
			if(nPrevGugan != -1)
				nShearSu++;
		}

		pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearSu];
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearSu];
		
		if(!bMerge)
		{
			pRB->InitData();
			pRB->m_nEa = (nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
		}
		else
		{
			pRB->m_nEa += (nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
		}
		if(i==nSize-nShearRebar || i>nSize-nShearRebar && nPrevGugan!=nGugan)
		{	
			if(bDuplicate)
				sMark.Format(_T("C%d"),6+nShearSu);
			else
			{
				if(bLeft)
					sMark.Format(_T("CL%d"),6+nShearSu);
				else
					sMark.Format(_T("CR%d"),6+nShearSu);
			}
			
			sDescription.Format(_T("����ö��%d"),nShearSu+1);

			
			
			pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
			pRB->m_sMark = sMark;
			pRB->m_sDescription = sDescription;
			pRB->m_dDia = pGW->m_dSR;
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_nTypeRebar = 18;
			pRB->m_dLengthMax_A = tvArrRebar[i].GetLength();
			pRB->m_dLengthMax_B = pGW->m_dCTC + pGW->m_dVR + pGW->m_dSR;
			pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
			pRB->m_dLengthMax_C = pRB->m_dLengthMax_B;
			pRB->m_Fy = pGW->m_dFy;//m_pARcBridgeDataStd->m_Fy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = 0;
			dSpace = pGW->m_dCTC * pGW->m_dEC;
		}
		else if(i>nSize-nShearRebar && nPrevGugan==nGugan)
		{
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}
			
			pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
			double dDist = tvArrRebar[i].GetLength();
			if(pRB->m_dLengthMax_B > dDist)
			{
				if(pRB->m_dLengthMin_B > dDist)
					pRB->m_dLengthMin_B = dDist;
			}
			else
			{
				if(pRB->m_dLengthMin_B > pRB->m_dLengthMax_B)
					pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
				pRB->m_dLengthMax_B = dDist;
			}
		}
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
		nPrevGugan = nGugan;
	}
	*/
}

void CRcBridgeRebarUtil::SetRebarLsdOptionGuardWall(SRebarEnvOption *pSLsdOption, CGuardWallRC *pGW)
{
	CBasicConcInfo *pBasicConcInfo = m_pARcBridgeDataStd->m_pBasicConcInfo;
	if(!pBasicConcInfo || pBasicConcInfo->IsDesignLsd() == FALSE)
		return;

	*pSLsdOption = *pBasicConcInfo->GetLsdRebarOpt();

// 	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];

	double dFy = pGW->m_dFy;
	double dFck = pGW->m_dFck;

	pSLsdOption->dFy = GetValueUnitNmm(dFy, UNIT_CGS_KGF_CM2);
 	pSLsdOption->dFck = GetValueUnitNmm(dFck, UNIT_CGS_KGF_CM2);
 	pSLsdOption->nRebarType = eStraitRebar;
 	pSLsdOption->dCover = MAX(pGW->m_ddc2, pGW->m_ddc3);
 	pSLsdOption->nHorDia	= (ERebarDia)m_pARcBridgeDataStd->m_pBasicConcInfo->GetIndexDia(pGW->m_dVR);
 	pSLsdOption->ePosType = eRebarPosTension;
 	pSLsdOption->dCheckCTC	= pGW->m_dF;
 	pSLsdOption->dHorCTC	= pGW->m_dCTC;
	pSLsdOption->dK = m_pARcBridgeDataStd->m_pBasicConcInfo->GetLsdRebarOpt()->dK_RebarPos[iINNER];
}

void CRcBridgeRebarUtil::MakeCenterGuardRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	
	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	if(pRB->m_bLock == FALSE)
	{
	//	CRebar* pRBPrev = NULL;
	//	if(pPrevGW != NULL)
	//		pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
	//	if(pPrevGW != NULL)
	//	{
	//		pRBPrev->m_nEa += pRB->m_nEa;
	//		pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
	//	}
		pRB->m_sMark = bDuplicate ? _T("A1") : _T("A1");
		pRB->m_sDescription = "����ö��1";
		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = tvArrRebar[0].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		double dCover = 0;
		double dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}


void CRcBridgeRebarUtil::MakeCenterGuard2RebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	// ��ȣ�� rebar���� = 15(except Dot & Shear Rebar_) 
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	long nShearRebar = tvArrRebar.GetSize() - 15;
	if(nShearRebar<0)	return;

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}

	long nIdx = 1;
	CString sIdx(_T(""));
	char ch[20];
	double dCover = 0, dSpace = 0;
	
	// Left
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		_ITOT_S(nIdx++, ch, sizeof(ch), 10);
		sIdx.Format(_T("A%s"), ch); 
		pRB->m_sMark = sIdx;
		pRB->m_sDescription = "����ö��1";
		pRB->m_nTypeRebar = 2;
		pRB->m_dLengthMax_A = pGW->m_dL1;
		pRB->m_dLengthMax_B = tvArrRebar[0].GetLength();

		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
	
	// Top
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_sDescription = "����ö��2";
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		_ITOT_S(nIdx++, ch, sizeof(ch), 10);
		sIdx.Format(_T("A%s"), ch); 
		pRB->m_sMark = sIdx;
		pRB->m_nTypeRebar = 4;
		pRB->m_dLengthMax_B = tvArrRebar[2].GetLength();	// 4
		pRB->m_dLengthMax_A = tvArrRebar[3].GetLength();	// 5
		pRB->m_dLengthMax_C = tvArrRebar[4].GetLength();	// 6
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = pGW->m_ddc1;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// Right
	if(tvArrRebar[0].GetLength() == tvArrRebar[5].GetLength())
	{
		if(pRB->m_bLock == FALSE)
		{
			pRB->m_nEa += pRB->m_nEa;
		}
	}
	else
	{
		pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
		if(pRB->m_bLock == FALSE)
		{
			if(pPrevGW != NULL)
				pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];

			pRB->InitData();
			pRB->m_nIdxNumber = GW_REBAR_VERT_3;
			pRB->m_dDia = pGW->m_dVR;
			pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			_ITOT_S(nIdx++, ch, sizeof(ch), 10);
			sIdx.Format(_T("A%s"), ch); 
			pRB->m_sMark = sIdx;
			pRB->m_sDescription = "����ö��3";
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_A = pGW->m_dL1;
			pRB->m_dLengthMax_B = tvArrRebar[5].GetLength();

			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = pGW->m_ddc3;
			dSpace = pGW->m_dCTC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
		}
	}

	// Right2
	for(long nLeft=0; nLeft<2; nLeft++)
	{
		pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3+nLeft];
		if(pRB->m_bLock == FALSE)
		{
			if(pPrevGW != NULL)
				pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3+nLeft];

			pRB->InitData();
			pRB->m_nIdxNumber = GW_REBAR_VERT_3+nLeft;

			_ITOT_S(nIdx++, ch, sizeof(ch), 10);
			sIdx.Format(_T("A%s"), ch); 
			pRB->m_sMark = sIdx;
			pRB->m_sDescription = nIdx>3 ? "����ö��3" : "����ö��2";
			pRB->m_dDia = pGW->m_dVR;
			pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_nTypeRebar = 13;
			pRB->m_dLengthMax_A = pGW->m_dL1;

			double dLengthMax_B=0, dLengthMax_C=0;
			if(nLeft==0)
			{
				pRB->m_dLengthMax_B = tvArrRebar[8].GetLength()+tvArrRebar[9].GetLength();
				pRB->m_dLengthMax_C = tvArrRebar[10].GetLength();

				dLengthMax_B = tvArrRebar[12].GetLength()+tvArrRebar[13].GetLength();
				dLengthMax_C = tvArrRebar[14].GetLength();			
			}
			else
			{
				pRB->m_dLengthMax_B = tvArrRebar[12].GetLength()+tvArrRebar[13].GetLength();
				pRB->m_dLengthMax_C = tvArrRebar[14].GetLength();
			}
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = nLeft ? pGW->m_ddc3 : pGW->m_ddc2;
			dSpace = pGW->m_dCTC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	

			// �ణ�� ������ �ִ�.
			if(fabs(pRB->m_dLengthMax_B-dLengthMax_B)<0.2 && fabs(pRB->m_dLengthMax_C-dLengthMax_C)<0.2)
			{
				pRB->m_nEa += pRB->m_nEa;
				break;
			}
		}
	}
	
	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;

		_ITOT_S(nIdx++, ch, sizeof(ch), 10);
		sIdx.Format(_T("A%s"), ch); 
		pRB->m_sMark = sIdx;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö��
	CString sMark(_T("")), sDescription(_T(""));
	long nShearSu=0;
	long nGugan=0, nPrevGugan=0;
	long nSize = tvArrRebar.GetSize();
	for(i=nSize-nShearRebar; i<nSize; i++)
	{		
		// ����
		CTwinVector tv = tvArrRebar.GetAt(i);
		double dOriginY = tvArrRebar.GetAt(1).m_v1.y + pGW->m_dF1;
		if(tv.m_v1.y < dOriginY + pGW->m_dCrossF2-0.5)
			nGugan = 0;
		else if(tv.m_v1.y >= (dOriginY+pGW->m_dCrossF2-0.5) && tv.m_v1.y <= (dOriginY+pGW->m_dCrossF2+pGW->m_dCrossF3+0.5))
			nGugan = 1;
		else
			nGugan = 2;

		if(i==nSize-nShearRebar || i>nSize-nShearRebar && nPrevGugan!=nGugan)
		{
			sDescription.Format(_T("����ö��%d"),nShearSu+1);

			pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearSu];
			if(pRB->m_bLock) continue;

			if(pPrevGW != NULL)
				pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearSu];

			nShearSu++;

			pRB->InitData();
			pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
			_ITOT_S(nIdx++, ch, sizeof(ch), 10);
			sIdx.Format(_T("A%s"), ch); 
			pRB->m_sMark = sIdx;
			pRB->m_sDescription = sDescription;
			pRB->m_dDia = pGW->m_dSR;
			pRB->m_nEa = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_nTypeRebar = 16;
			pRB->m_dLengthMax_A = pGW->m_dCTC + pGW->m_dVR + pGW->m_dSR;
			pRB->m_dLengthMax_B = tvArrRebar[i].GetLength();
			pRB->m_dLengthMax_C = 100;//pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = 0;
			dSpace = pGW->m_dCTC * pGW->m_dEC;
		}
		else if(i>nSize-nShearRebar && nPrevGugan==nGugan)
		{
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
			double dDist = tvArrRebar[i].GetLength();
			if(pRB->m_dLengthMax_B > dDist)
			{
				if(pRB->m_dLengthMin_B > dDist)
					pRB->m_dLengthMin_B = dDist;
			}
			else
			{
				if(pRB->m_dLengthMin_B > pRB->m_dLengthMax_B)
					pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
				pRB->m_dLengthMax_B = dDist;
			}
		}
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	

		nPrevGugan = nGugan;
	}
}


void CRcBridgeRebarUtil::MakeCenterGuardHalfRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	// ��ȣ�� rebar���� = 7(except Dot & Shear Rebar_) 
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	long nShearRebar = tvArrRebar.GetSize() - 7;
	if(nShearRebar<0)	return;
	
	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}

	double dCover = 0, dSpace = 0;
	// Left
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("B1") : _T("BL1");
			pRB->m_sDescription = "����ö��1";
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_A = pGW->m_dL1;
			pRB->m_dLengthMax_B = tvArrRebar[0].GetLength();
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("B2") : _T("BR2");
			pRB->m_sDescription = "����ö��2";
			pRB->m_nTypeRebar = 1;
			pRB->m_dLengthMax_A = tvArrRebar[2].GetLength();
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	
	// Right
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("B2") : _T("BL2");
			pRB->m_sDescription = "����ö��2";
			pRB->m_nTypeRebar = 1;
			pRB->m_dLengthMax_A = tvArrRebar[2].GetLength();
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("B1") : _T("BR1");
			pRB->m_sDescription = "����ö��1";
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_A = pGW->m_dL1;
			pRB->m_dLengthMax_B = tvArrRebar[0].GetLength();			
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// Right2
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_3;
		pRB->m_sMark = bLeft ? _T("BL3") : _T("BR3");
		pRB->m_sMark = bDuplicate ? _T("B3") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��3";
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 13;
		pRB->m_dLengthMax_A = pGW->m_dL1;
		pRB->m_dLengthMax_B = tvArrRebar[4].GetLength()+tvArrRebar[5].GetLength();
		pRB->m_dLengthMax_C = tvArrRebar[6].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}


	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("BL4") : _T("BR4");
		pRB->m_sMark = bDuplicate ? _T("B4") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö�� : �������� �ִ�� �ּҸ� ��󳻸� ��
	CString sMark = bLeft ? _T("BL5") : _T("BR5");
	if(bDuplicate)	sMark = _T("B5");
	
	pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
		pRB->m_sMark = sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_dDia = pGW->m_dSR;

		pRB->m_nTypeRebar = 16;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = pGW->m_dCTC * pGW->m_dEC;

		pRB->m_nEa  = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
		if(pPrevGW != NULL)
			pRBPrev->m_nEa += pRB->m_nEa;
	
		for(i=0; i<nShearRebar; i++)
		{
			if(i==0)			
			{	
				pRB->m_dLengthMax_B = ~(tvArrRebar[7+nShearRebar-1-i].m_v2-tvArrRebar[7+nShearRebar-1-i].m_v1);
				pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
			}
			else
			{
				double dDist = ~(tvArrRebar[7+nShearRebar-1-i].m_v2-tvArrRebar[7+nShearRebar-1-i].m_v1);
				if(pRB->m_dLengthMax_B > dDist)
				{
					if(pRB->m_dLengthMin_B > dDist)
						pRB->m_dLengthMin_B = dDist;
				}
				else
				{
					if(pRB->m_dLengthMin_B > pRB->m_dLengthMax_B)
						pRB->m_dLengthMin_B = pRB->m_dLengthMax_B;
					pRB->m_dLengthMax_B = dDist;
				}
			}
		}
		pRB->m_dLengthMax_A = pGW->m_dCTC + pGW->m_dVR + pGW->m_dSR;
		pRB->m_dLengthMax_C = 100;//pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}


void CRcBridgeRebarUtil::MakeHandRailRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	long nNum = 1;
	CString sMark = _T(""), sNum =_T("");
	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);
	xyPointRebar.RemoveSameValue(0.001);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	// ��ȣ�� rebar���� = 18(except Dot & Shear Rebar_) 
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	long nSize = tvArrRebar.GetSize();
	long nSizeVert = (long)(nGuardFenceLength / pGW->m_dCTC);

	//////////////////////////////////////////////////////////////////////////
	for(long i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		pRB->InitData();
	}

	double dCover = 0, dSpace = 0;
	CTwinVector tv1,tv2,tv3;
	// ���� ����ö�� 1 : ���� ö��
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = nSizeVert;
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		sNum.Format(_T("%d"), nNum);
		if(bDuplicate)		sMark =  _T("E") + sNum;
		else				sMark = (bLeft ? _T("EL") : _T("ER")) + sNum;
		nNum++;
		
		tv1 = tvArrRebar.GetAt(0);
		tv2 = tvArrRebar.GetAt(1);
		tv3 = tvArrRebar.GetAt(2);
		pRB->m_sMark = sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_nTypeRebar = 4;
		pRB->m_dLengthMax_A = tv1.GetLength();
		pRB->m_dLengthMax_B = tv2.GetLength();
		pRB->m_dLengthMax_C = tv3.GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ���� 1
	CTwinVectorArray tvArrVert;
	for(long ix = 0; ix < nSize; ix++)
	{
		CTwinVector tv = tvArrRebar.GetAt(ix);
		if(tv.m_sInfo.Find(_T("��ö��2")) != -1)
		{
			tvArrVert.Add(tv);
		}
	}

	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		if(tvArrVert.GetSize() > 0)
		{
			tv1 = tvArrVert.GetAt(0);

			pRB->m_nIdxNumber = GW_REBAR_VERT_2;
			pRB->m_dDia = pGW->m_dVR;
			pRB->m_nEa  = nSizeVert;
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}

			sNum.Format(_T("%d"), nNum);
			if(bDuplicate)		sMark =  _T("E") + sNum;
			else				sMark = (bLeft ? _T("EL") : _T("ER")) + sNum;
			nNum++;

			pRB->m_sMark = sMark;
			pRB->m_sDescription = "����ö��2";
			pRB->m_nTypeRebar = 1;
			pRB->m_dLengthMax_A = tv1.GetLength();
			if(tvArrVert.GetSize() > 1)
			{
				tv2 = tvArrVert.GetAt(1);
				pRB->m_nTypeRebar = tv1.IsVertical() ? 2 : 8;
				pRB->m_dLengthMax_B = tv2.GetLength();
				if(tvArrVert.GetSize() == 3)
				{
					tv3 = tvArrVert.GetAt(2);
					pRB->m_nTypeRebar = 47;
					pRB->m_dLengthMax_C = tv3.GetLength();
				}
			}
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
			dSpace = pGW->m_dCTC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
		}
	}
	

	// ���� 2
	tvArrVert.RemoveAll();
	for(long ix = 0; ix < nSize; ix++)
	{
		CTwinVector tv = tvArrRebar.GetAt(ix);
		if(tv.m_sInfo.Find(_T("��ö��3")) != -1)
		{
			tvArrVert.Add(tv);
		}
	}
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];

		pRB->InitData();

		if(tvArrVert.GetSize() > 0)
		{
			tv1 = tvArrVert.GetAt(0);

			sNum.Format(_T("%d"), nNum);
			if(bDuplicate)		sMark =  _T("E") + sNum;
			else				sMark = (bLeft ? _T("EL") : _T("ER")) + sNum;
			nNum++;

			pRB->m_nIdxNumber = GW_REBAR_VERT_3;
			pRB->m_sMark = sMark;
			pRB->m_sDescription = "����ö��3";
			pRB->m_dDia = pGW->m_dVR;
			pRB->m_nEa  = nSizeVert;
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}
			pRB->m_nTypeRebar = 1;
			pRB->m_dLengthMax_A = tv1.GetLength();
			if(tvArrVert.GetSize() > 1)
			{
				tv2 = tvArrVert.GetAt(1);
				pRB->m_nTypeRebar = tv1.IsVertical() ? 2 : 8;
				pRB->m_dLengthMax_B = tv2.GetLength();
				if(tvArrVert.GetSize() == 3)
				{
					tv3 = tvArrVert.GetAt(2);
					pRB->m_nTypeRebar = tv1.IsLevel() ? 33 : 47;
					pRB->m_dLengthMax_C = tv3.GetLength();
				}
			}
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
			dSpace = pGW->m_dCTC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
		}
	}

	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		sNum.Format(_T("%d"), nNum);
		if(bDuplicate)		sMark =  _T("E") + sNum;
		else				sMark = (bLeft ? _T("EL") : _T("ER")) + sNum;
		nNum++;

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dF;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	//RB ����ö��
	CString sDescription(_T(""));
	sDescription.Format(_T("����ö��"));

	//����ö�� �ִ� B���� �� �ּ� ����
	double dMaxLen(0), dCurrLen(0), dMinLen(0);
	CTwinVectorArray tvArrShear;
	for (long nRebar = 0; nRebar < tvArrRebar.GetSize(); nRebar++)
	{
		CTwinVector tv = tvArrRebar.GetAt(nRebar);
		if(tv.m_sInfo.Find(_T("����")) == -1) continue;

		tvArrShear.Add(tv);
	}
	for (long nShear = 0; nShear < tvArrShear.GetSize(); nShear++)
	{
		CTwinVector tv = tvArrShear.GetAt(nShear);
		dCurrLen = tv.GetLength();
		if(nShear == 0)
		{
			dMaxLen = dMinLen = dCurrLen;
			continue;
		}
		dMaxLen = max(dCurrLen, dMaxLen);
		dMinLen = min(dCurrLen, dMinLen);
	}

	pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1];

		pRB->InitData();

		if(tvArrShear.GetSize() > 0)
		{
			sNum.Format(_T("%d"), nNum);
			if(bDuplicate)		sMark =  _T("E") + sNum;
			else				sMark = (bLeft ? _T("EL") : _T("ER")) + sNum;
			nNum++;

			pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
			pRB->m_sMark = sMark;
			pRB->m_sDescription = sDescription;
			pRB->m_dDia = pGW->m_dSR;
			pRB->m_nEa  = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC)) * tvArrShear.GetSize();
			if(pPrevGW != NULL)
			{
				pRBPrev->m_nEa += pRB->m_nEa;
				pRB->m_nEa = 0;
			}
			pRB->m_nTypeRebar = 16;
			pRB->m_dLengthMax_A = pGW->m_dCTC + pGW->m_dVR + pGW->m_dSR;
			pRB->m_dLengthMax_B = dMaxLen;
			pRB->m_dLengthMin_B = dMinLen;
			pRB->m_dLengthMax_C = 100;
			pRB->m_Fy = pGW->m_dFy;
			pRB->m_dRateAdd = MAKE_RATE_UTIL;
			dCover = 0;
			dSpace = pGW->m_dCTC * pGW->m_dEC;
			SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
		}
	}
}

void CRcBridgeRebarUtil::MakeSoundProofRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}

	double dCover = 0, dSpace = 0;
	// ����ö��
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("GL1") : _T("GR1");
		pRB->m_sMark = bDuplicate ? _T("G1") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_nTypeRebar = 95;
		pRB->m_dLengthMax_A = tvArrRebar[2].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[1].GetLength();
		pRB->m_dLengthMax_C = pGW->m_dL1;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}


	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("GL2") : _T("GR2");
		pRB->m_sMark = bDuplicate ? _T("G2") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö�� : �������� �ִ�� �ּҸ� ��󳻸� ��
	long nSize = tvArrRebar.GetSize();
	
	pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
		pRB->m_sMark = bLeft ? _T("GL3") : _T("GR3");
		pRB->m_sMark = bDuplicate ? _T("G3") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_dDia = pGW->m_dSR;
		pRB->m_nTypeRebar = 3;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = pGW->m_dCTC * pGW->m_dEC;
	
		long nShearStt = 5;

		for(i=nShearStt; i<nSize; i++)
		{
			pRB->m_nEa  = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
			if(i==nShearStt)			
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}
				pRB->m_dLengthMax_A = tvArrRebar[i].GetLength();
				pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
			}
			else
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}

				double dDist = tvArrRebar[i].GetLength();
				if(pRB->m_dLengthMax_A > dDist)
				{
					if(pRB->m_dLengthMin_A > dDist)
						pRB->m_dLengthMin_A = dDist;
				}
				else
				{
					if(pRB->m_dLengthMin_A > pRB->m_dLengthMax_A)
						pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
					pRB->m_dLengthMax_A = dDist;
				}
			}
		}
		//pRB->m_dLengthMax_B = pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
		pRB->m_dLengthMax_B = 100;	// 2006.10.11 �׳� 100���� ����
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}

void CRcBridgeRebarUtil::MakeRetainingWallRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;
		pRB->InitData();
	}

	double dCover = 0, dSpace = 0;
	// ����ö�� 1
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("JL1") : _T("JR1");
		pRB->m_sMark = bDuplicate ? _T("J1") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_nTypeRebar = 4;
		pRB->m_dLengthMax_A = tvArrRebar[0].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[1].GetLength();
		pRB->m_dLengthMax_C = tvArrRebar[2].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 2
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		pRBPrev = NULL;
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("JL2") : _T("JR2");
		pRB->m_sMark = bDuplicate ? _T("J2") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��2";
		pRB->m_nTypeRebar = 2;
		pRB->m_dLengthMax_A = tvArrRebar[3].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[4].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 3
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
	if(pRB->m_bLock == FALSE)
	{
		pRBPrev = NULL;
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_3;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("JL3") : _T("JR3");
		pRB->m_sMark = bDuplicate ? _T("J3") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��3";
		pRB->m_nTypeRebar = 2;
		pRB->m_dLengthMax_B = tvArrRebar[5].GetLength();
		pRB->m_dLengthMax_A = tvArrRebar[6].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 4
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_4];
	if(pRB->m_bLock == FALSE)
	{
		pRBPrev = NULL;
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_4];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_4;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("JL4") : _T("JR4");
		pRB->m_sMark = bDuplicate ? _T("J4") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��4";
		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = tvArrRebar[7].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("JL5") : _T("JR5");
		pRB->m_sMark = bDuplicate ? _T("J5") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö�� : �������� �ִ�� �ּҸ� ��󳻸� ��
	long nSize = tvArrRebar.GetSize();
	
	CString sMark = bLeft ? _T("JL6") : _T("JR6");
	if(bDuplicate)	sMark = _T("J6");
	
	pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
		pRB->m_sMark = sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_dDia = pGW->m_dSR;

		pRB->m_nTypeRebar = 3;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = pGW->m_dCTC * pGW->m_dEC;
		
		long nShearStt = 8;
		for(i=nShearStt; i<nSize; i++)
		{
			pRB->m_nEa = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
			if(i==nShearStt)			
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}
				pRB->m_dLengthMax_A = tvArrRebar[i].GetLength();
				pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
			}
			else
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}

				double dDist = tvArrRebar[i].GetLength();
				if(pRB->m_dLengthMax_A > dDist)
				{
					if(pRB->m_dLengthMin_A > dDist)
						pRB->m_dLengthMin_A = dDist;
				}
				else
				{
					if(pRB->m_dLengthMin_A > pRB->m_dLengthMax_A)
						pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
					pRB->m_dLengthMax_A = dDist;
				}
			}
		}
		pRB->m_dLengthMax_B = 100;//pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}


void CRcBridgeRebarUtil::MakeFootwayBlockRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}	

	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	double dGuardFenceLeft = nGuardFenceLength * 0.1;
	double dGuardFenceRight = nGuardFenceLength * 0.1;
	double dGuardFenceSide = dGuardFenceLeft + dGuardFenceRight;
	double dGuardFenceMid = nGuardFenceLength - dGuardFenceSide;

	double dCover = 0, dSpace = 0;
	// ����ö�� 1
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("KL1") : _T("KR1");
		pRB->m_sMark = bDuplicate ? _T("K1") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_nTypeRebar   = 82;
		pRB->m_dLengthMax_A = tvArrRebar[2].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[1].GetLength();
		pRB->m_dLengthMax_C = tvArrRebar[3].GetLength();
		pRB->m_dLengthMax_D = tvArrRebar[0].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 2
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(dGuardFenceSide / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("KL2") : _T("KR2");
		pRB->m_sMark = bDuplicate ? _T("K2") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��2(�ܺ�)");
		pRB->m_nTypeRebar   = 2;
		pRB->m_dLengthMax_A = tvArrRebar[5].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[6].GetLength()+tvArrRebar[7].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 1
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("KL3") : _T("KR3");
		pRB->m_sMark = bDuplicate ? _T("K3") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1(�ܺ�)");
		pRB->m_dDia = bLsd? m_pRcBridgeApp->GetValueFy(ePartUpperSlab) : pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = dGuardFenceSide;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// �߾Ӻ� ���
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();
	
	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, 0, TRUE);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev, 0, TRUE);
	}

	// ����ö�� 2
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_1];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(dGuardFenceMid / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("KL4") : _T("KR4");
		pRB->m_sMark = bDuplicate ? _T("K4") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��2(�߾Ӻ�)");
		pRB->m_nTypeRebar   = 1;
		pRB->m_dLengthMax_A = tvArrRebar[5].GetLength()+tvArrRebar[6].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 3
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_2;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(dGuardFenceMid / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("KL5") : _T("KR5");
		pRB->m_sMark = bDuplicate ? _T("K5") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��3(�߾Ӻ�)");
		pRB->m_nTypeRebar   = 2;
		pRB->m_dLengthMax_A = tvArrRebar[7].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[8].GetLength()+tvArrRebar[9].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 1
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_3];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_3];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_3;
		pRB->m_sMark = bLeft ? _T("KL6") : _T("KR6");
		pRB->m_sMark = bDuplicate ? _T("K6") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1(�߾Ӻ�)");
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = dGuardFenceMid;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 1
	CString sMark = bLeft ? _T("KL7") : _T("KR7");
	if(bDuplicate)	sMark = _T("K7");
	
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_4];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_4];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_4;
		pRB->m_sMark = sMark;
		pRB->m_sDescription = _T("����ö��1(�߾Ӻ�)");
		pRB->m_dDia = pGW->m_dSR;
		pRB->m_nEa  = (long)(dGuardFenceMid / (pGW->m_dCTC*pGW->m_dEC));
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 3;
		pRB->m_dLengthMax_A = tvArrRebar[10].GetLength();
		pRB->m_dLengthMax_B = 100;//pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = pGW->m_dCTC * pGW->m_dEC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}

void CRcBridgeRebarUtil::MakeRailGuardFenceRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}
	
	// ��ȣ�� rebar���� = 17(except Dot & Shear Rebar_) 
	long nRebar = 10;
	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	long nShearRebar = tvArrRebar.GetSize() - nRebar;
	if(nShearRebar<0)	return;

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}

	long n = 0;
	
	double dCover = 0, dSpace = 0;
	// Left
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_3];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		pRB->m_sDescription = "����ö��1";
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("L1") : _T("LL1");
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();		// 0
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength();		// 1 + 2		
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("L1") : _T("LR1");
			pRB->m_nTypeRebar = 33;
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 0
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 1 + 2
			pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 3
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// Top
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_sMark = bLeft ? _T("LL2") : _T("LR2");
		pRB->m_sMark = bDuplicate ? _T("L2") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��2";
		pRB->m_dDia = pGW->m_dVR;
		nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 33;
		pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 5
		pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength();	// 6
		pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 7
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = pGW->m_ddc1;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	
	// Right
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_3];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_3;
		pRB->m_dDia = pGW->m_dVR;
		nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		pRB->m_sDescription = "����ö��3";
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		if(bLeft)
		{
			pRB->m_sMark = bDuplicate ? _T("L3") : _T("LL3");
			pRB->m_nTypeRebar = 33;
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 7
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 8 + 9
			pRB->m_dLengthMax_C = tvArrRebar[n++].GetLength();	// 10	
		}
		else
		{
			pRB->m_sMark = bDuplicate ? _T("L3") : _T("LR3");
			pRB->m_nTypeRebar = 2;
			pRB->m_dLengthMax_B = tvArrRebar[n++].GetLength();	// 7
			pRB->m_dLengthMax_A = tvArrRebar[n++].GetLength()+tvArrRebar[n++].GetLength(); // 8 + 9
		
		}
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö��
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("LL4") : _T("LR4");
		pRB->m_sMark = bDuplicate ? _T("L4") : pRB->m_sMark;
		pRB->m_sDescription = "����ö��1";
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = nGuardFenceLength;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// ����ö�� : �������� �ִ�� �ּҸ� ��󳻸� ��
	long nSize = tvArrRebar.GetSize();
	long nShearStt = nRebar;
	
	CString sMark = bLeft ? _T("LL5") : _T("LR5");
	if(bDuplicate)	sMark = _T("L5");
	
	pRB = pGW->m_pArrRebar[GW_REBAR_SHEAR_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_SHEAR_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_SHEAR_1;
		pRB->m_sMark = sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_dDia = pGW->m_dSR;
		pRB->m_nTypeRebar = 3;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = pGW->m_dCTC * pGW->m_dEC;
	
		for(i=nShearStt; i<nSize; i++)
		{
			pRB->m_nEa  = (long)(nGuardFenceLength / (pGW->m_dCTC*pGW->m_dEC));
			if(i==nShearStt)			
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}
				pRB->m_dLengthMax_A = tvArrRebar[i].GetLength();
				pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
			}
			else
			{
				if(pPrevGW != NULL)
				{
					pRBPrev->m_nEa += pRB->m_nEa;
					pRB->m_nEa = 0;
				}
				double dDist = tvArrRebar[i].GetLength();
				if(pRB->m_dLengthMax_A > dDist)
				{
					if(pRB->m_dLengthMin_A > dDist)
						pRB->m_dLengthMin_A = dDist;
				}
				else
				{
					if(pRB->m_dLengthMin_A > pRB->m_dLengthMax_A)
						pRB->m_dLengthMin_A = pRB->m_dLengthMax_A;
					pRB->m_dLengthMax_A = dDist;
				}
			}
		}
		pRB->m_dLengthMax_B = 100;//pGW->m_dSR <= 16 ? 6*pGW->m_dSR : 12*pGW->m_dSR;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}

void CRcBridgeRebarUtil::MakeRailTotalHoleRebarJewon(CGuardWallRC *pGW, CGuardWallRC *pPrevGW, BOOL bDuplicate)
{
	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	// ���� �ߺ��� ��ȣ��
	CTwinVectorArray tvArrPrev, tvArrRebarPrev;
	CDPointArray xyPointRebarPrev;
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();

	CDPoint vAng(0,1);
	double sta = m_pRcBridgeApp->GetStationOnJijum(0,0);
	CJijum* pJL = m_pRcBridgeApp->GetJijum(0);
	if(m_pRcBridgeApp->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev);
	}

	long i=0; for(i=0; i<pGW->m_pArrRebar.GetSize(); i++)
	{
		CRebar* pRB = pGW->m_pArrRebar[i];
		if(pRB->m_bLock) continue;

		pRB->InitData();
	}	

	BOOL bLeft = pGW->IsLeftGuardFence();
	long nGuardFenceLength = m_pRcBridgeApp->GetGuardWallTotalLength(pGW);
	double dGuardFenceLeft = nGuardFenceLength * 0.1;
	double dGuardFenceRight = nGuardFenceLength * 0.1;
	double dGuardFenceSide = dGuardFenceLeft + dGuardFenceRight;
	double dGuardFenceMid = nGuardFenceLength - dGuardFenceSide;

	double dCover = 0, dSpace = 0;
	// ����ö�� 1
	CRebar* pRB = pGW->m_pArrRebar[GW_REBAR_VERT_1];
	CRebar* pRBPrev = NULL;
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_1];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(nGuardFenceLength / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("NL1") : _T("NR1");
		pRB->m_sMark = bDuplicate ? _T("N1") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1");
		pRB->m_nTypeRebar   = 82;
		pRB->m_dLengthMax_A = tvArrRebar[2].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[1].GetLength();
		pRB->m_dLengthMax_C = tvArrRebar[3].GetLength();
		pRB->m_dLengthMax_D = tvArrRebar[0].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc2 : pGW->m_ddc3;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 2
	pRB = pGW->m_pArrRebar[GW_REBAR_VERT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_VERT_2];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_VERT_2;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(dGuardFenceSide / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("NL2") : _T("NR2");
		pRB->m_sMark = bDuplicate ? _T("N2") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��2(�ܺ�)");
		pRB->m_nTypeRebar   = 2;
		pRB->m_dLengthMax_A = tvArrRebar[5].GetLength();
		pRB->m_dLengthMax_B = tvArrRebar[6].GetLength()+tvArrRebar[7].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 1
	pRB = pGW->m_pArrRebar[GW_REBAR_HORI_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_HORI_1];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_HORI_1;
		pRB->m_sMark = bLeft ? _T("NL3") : _T("NR3");
		pRB->m_sMark = bDuplicate ? _T("N3") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1(�ܺ�)");
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = dGuardFenceSide;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);	
	}

	// �߾Ӻ� ���
	tvArr.RemoveAll();	
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();
	tvArrPrev.RemoveAll();	
	tvArrRebarPrev.RemoveAll();	
	xyPointRebarPrev.RemoveAll();
	
	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, 0, TRUE);

	if(pPrevGW != NULL)
	{
		m_pRcBridgeApp->GetTvGuardWall(pPrevGW, tvArrPrev, sta, vAng);
		GetTvRebarGuardWall(pPrevGW, tvArrPrev, tvArrRebarPrev, xyPointRebarPrev, 0, TRUE);
	}

	// ����ö�� 2
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_1];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_1];
	
		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_1;
		pRB->m_dDia = pGW->m_dVR;
		pRB->m_nEa  = (long)(dGuardFenceMid / pGW->m_dCTC);
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;		// �ߺ��Ǵ� ���� ö�� ���ǥ�� ���� ���� �ʵ��� ������ 0 �� �Ѵ�.
		}

		pRB->m_sMark = bLeft ? _T("NL4") : _T("NR4");
		pRB->m_sMark = bDuplicate ? _T("N4") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��2(�߾Ӻ�)");
		pRB->m_nTypeRebar   = 1;
		pRB->m_dLengthMax_A = tvArrRebar[5].GetLength()+tvArrRebar[6].GetLength();
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = bLeft ? pGW->m_ddc3 : pGW->m_ddc2;
		dSpace = pGW->m_dCTC;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}

	// ����ö�� 1
	pRB = pGW->m_pArrRebar[GW_REBAR_EXT_2];
	if(pRB->m_bLock == FALSE)
	{
		if(pPrevGW != NULL)
			pRBPrev = pPrevGW->m_pArrRebar[GW_REBAR_EXT_2];

		pRB->InitData();
		pRB->m_nIdxNumber = GW_REBAR_EXT_2;
		pRB->m_sMark = bLeft ? _T("NL5") : _T("NR5");
		pRB->m_sMark = bDuplicate ? _T("N5") : pRB->m_sMark;
		pRB->m_sDescription = _T("����ö��1(�߾Ӻ�)");
		pRB->m_dDia = pGW->m_dHR;
		pRB->m_nEa  = xyPointRebar.GetSize();
		if(pPrevGW != NULL)
		{
			pRBPrev->m_nEa += pRB->m_nEa;
			pRB->m_nEa = 0;
		}

		pRB->m_nTypeRebar = 1;
		pRB->m_dLengthMax_A = dGuardFenceMid;
		pRB->m_Fy = pGW->m_dFy;
		pRB->m_dRateAdd = MAKE_RATE_UTIL;
		dCover = 0;
		dSpace = 0;
		SetLengthRebar(pRB,JOINT_TENS_NORMAL_B,dCover,dSpace);
	}
}

void CRcBridgeRebarUtil::SetDataDefaultRebarGuardWall(CGuardWallRC* pGW, double sta, CDPoint vAng)
{
	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;

	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();
	xyPointRebar.RemoveAll();

	pGW->m_dFck = 270;
	pGW->m_dFy  = 4000;
	pGW->m_dAggregate = 19;

	pGW->m_dHR  = 13;		// ����ö�� ����
	pGW->m_dVR  = 16;		// ����ö�� ����
	pGW->m_dSR  = 13;		// ����ö�� ����
	pGW->m_dSC  = 2;		// ����ö�� ����
	pGW->m_dEC  = 2;		// ����ö���� ��ö�� ������
	pGW->m_ddc1 = 50.0;		// ���� �Ǻ�
	pGW->m_ddc2 = 50.0;		// ���� �Ǻ�
	pGW->m_ddc3 = 50.0;		// ������ �Ǻ�
	pGW->m_dCTC = 250.0;	// ��ö�� ����
	pGW->m_dL1  = 200.0;	// Slab�� ���Ե� ��
	pGW->m_dF1  = 190;		// Slab�� ���Ե� ö�� ���� 1
	pGW->m_dF2  = -1;
	pGW->m_dF3  = -1;
	pGW->m_dFH  = 0;
	pGW->m_dEH  = 0;

	if(pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel())
	{
		pGW->m_dE   = pGW->IsTypeHDanRoadGuardFence() ? 6 : 3;		// ����ö�� ����
		pGW->m_dSC  = pGW->IsTypeHDanRoadGuardFence() ? 2 : 1;		// ����ö�� ����
		if(pGW->m_bSoundProof)
		{
			pGW->m_dSC	   = 1;		// ����ö�� ����
			pGW->m_nUGugan = 1;
			pGW->m_nSGugan = 1;
			long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
			{
				pGW->m_nU[i] = 0;
				pGW->m_nS[i] = 1;
				pGW->m_dU[i] = 100;
				pGW->m_dS[i] = 200;
			}
		}
	}
	else if(pGW->IsTypeHDanRoadCentralWallFull())
	{
		if(pGW->m_bGWhalf)
		{
			pGW->m_dVR  = 25;
			pGW->m_dCTC = 400.0;
			pGW->m_dF1  = 125;
			pGW->m_dF2  = 125;
		}
		else
		{
			pGW->m_dE   = 6;		// ����ö�� ����
			if(pGW->m_bSoundProof)
			{
				pGW->m_dSC	   = 1;		// ����ö�� ����
				pGW->m_nUGugan = 1;
				pGW->m_nSGugan = 1;
				long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
				{
					pGW->m_nU[i] = 0;
					pGW->m_nS[i] = 1;
					pGW->m_dU[i] = 100;
					pGW->m_dS[i] = 200;
				}
			}
		}
	}
	else if(pGW->IsTypeHDanRoadCentralWallHalf())
	{
		pGW->m_dE   = 5;		// ����ö�� ����
		if(pGW->m_bSoundProof)
		{
			pGW->m_dSC	   = 1;		// ����ö�� ����
			pGW->m_nUGugan = 1;
			pGW->m_nSGugan = 1;
			long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
			{
				pGW->m_nU[i] = 0;
				pGW->m_nS[i] = 1;
				pGW->m_dU[i] = 100;
				pGW->m_dS[i] = 200;
			}
		}
	}
	else if(pGW->IsTypeHDanRoadHandRail())
	{
		pGW->m_dE   = 2;		// ����ö�� ����
		if(pGW->m_bSoundProof)
		{
			pGW->m_dSC	   = 1;		// ����ö�� ����
			pGW->m_nUGugan = 1;
			pGW->m_nSGugan = 1;
			long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
			{
				pGW->m_nU[i] = 0;
				pGW->m_nS[i] = 1;
				pGW->m_dU[i] = 100;
				pGW->m_dS[i] = 200;
			}
		}
	}
	else if(pGW->IsTypeHDanRoadSoundProof())
	{
		pGW->m_dSC	   = 1;		// ����ö�� ����
		pGW->m_nUGugan = 1;
		pGW->m_nSGugan = 1;
		long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
		{
			pGW->m_nU[i] = 0;
			pGW->m_nS[i] = 1;
			pGW->m_dU[i] = 100;
			pGW->m_dS[i] = 200;
		}
	}
	else if(pGW->IsTypeHDanRoadRetainingWall())
	{
		pGW->m_dE   = 8;		// V ����ö�� ����
		pGW->m_dEH  = 6;		// H ����ö�� ����
		pGW->m_dL1  = 300.0;	// Slab�� ���Ե� ��
	}
	else if(pGW->IsTypeHDanRailGuardFence())
	{
		pGW->m_dE   = 3;		// ����ö�� ����
		pGW->m_dF2  = 250;		// ����ö�� ����
	}
	m_pRcBridgeApp->GetTvGuardWall(pGW, tvArr, sta, vAng);
	if(pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel())
		GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);
	else if(pGW->IsTypeHDanRoadCentralWallFull())
	{
		if(pGW->m_bGWhalf)
			GetTvRebarCenterGuard(pGW, tvArr, tvArrRebar, xyPointRebar);	
		else
			GetTvRebarCenterGuard2(pGW, tvArr, tvArrRebar, xyPointRebar);	
	}
	else if(pGW->IsTypeHDanRoadCentralWallHalf())
		GetTvRebarCenterGuardHalf(pGW, tvArr, tvArrRebar, xyPointRebar);
	else if(pGW->IsTypeHDanRoadHandRail())
	{
		GetTvRebarHandRail(pGW, tvArr, tvArrRebar, xyPointRebar, -1);
	}
	else if(pGW->IsTypeHDanRoadRetainingWall())
		GetTvRebarRetainingWall(pGW, tvArr, tvArrRebar, xyPointRebar);	

	if(pGW->IsTypeHDanRailFootwayBlock())
	{
		pGW->m_dF1 = 2;		// �������� ���� ��� ö�� ����
		pGW->m_dF2 = 2;		// �������� �Ʒ��� ��� ö�� ����
		pGW->m_dF3 = 4;		// ������� ��� ö�� ����
		pGW->m_dF  = 2;		// ������� ������ ��� ö�� ����
	}
	if(pGW->IsTypeHDanRailTotalHole())
	{
		pGW->m_dF1 = 1;		// �������� ���� ��� ö�� ����
		pGW->m_dF2 = 4;		// �������� �Ʒ��� ��� ö�� ����
		pGW->m_dF3 = 2;		// ������� ��� ö�� ����
	}
}


void CRcBridgeRebarUtil::RemoveMeanLessTvArr(CTwinVectorArrayArray *pTvArrArr)
{
	if(!pTvArrArr) return;
	
	long i = 0; for(i = 0; i < pTvArrArr->GetSize(); i++)
	{
		CTwinVectorArray *pTvArr	= pTvArrArr->GetAt(i);
		if(pTvArr->GetLength() == 0)
		{
			delete pTvArr;
			pTvArrArr->RemoveAt(i);
			i--;
		}
		
	}
}


// ���� ���� �ڸ��⿡ ���� Ʈ������
// BOOL bFindByLineFirstLine=FALSE : ù���� ���� Seg�� �˻����� ����, line�� �˻���
void CRcBridgeRebarUtil::TrimCutEndOfGagak(CTwinVectorArrayArray *pTvArrArr, CTwinVectorArray &tvArrPlane, BOOL bStt, BOOL bLeft, BOOL bFindByLineFirstLine)
{
	if(!pTvArrArr) return;
// 	CTwinVectorArray *pTvArr	= NULL;
		
	long i = 0; for(i = 0; i < pTvArrArr->GetSize(); i++)
	{
		CTwinVectorArray *pTvArr	= pTvArrArr->GetAt(i);
		TrimCutEndOfGagak(*pTvArr, tvArrPlane, bStt, bLeft, bFindByLineFirstLine ? i==0 : FALSE);
	}
}

// BOOL bFindByLineFirstLine=FALSE : ù���� ���� Seg�� �˻����� ����, line�� �˻���
void CRcBridgeRebarUtil::TrimCutEndOfGagak(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrPlane, BOOL bStt, BOOL bLeft, BOOL bFindByLineFirstLine, BOOL bHunch)
{
	CString sInfo	= _T("");
	if(bStt)
	{
		sInfo	= bLeft ? "����(��������������)��Ʈ" : "����(��������������)��Ʈ";
	}
	else
	{
		sInfo	= bLeft ? "����(��������������)��Ʈ" : "����(��������������)��Ʈ";
	}

	CTwinVector tvCut;
	if(!tvArrPlane.GetTvByInfo(sInfo, tvCut)) return;
	
	CRebarPlacing rb;
	CDPointArray xyArrMatch;
	if(bFindByLineFirstLine)
		rb.GetXyMatchTvArrAndLine(tvArr, tvCut.m_v1, tvCut.GetXyDir(), xyArrMatch);
	else
		rb.GetXyMatchTvArrAndSeg(tvArr, tvCut.m_v1, tvCut.m_v2, xyArrMatch, FALSE);
	if(xyArrMatch.GetSize() == 0) return;

	CDPoint xyMatch(0, 0);
	long j = 0; for(j = 0; j < tvArr.GetSize(); j++)
	{
		CTwinVector *pTv	= &tvArr.GetAt(j);

		if(pTv->m_v1.z == 0)
		{
			if(j == 0 && bFindByLineFirstLine)
			{
				if(GetXyMatchLineAndLine(tvCut.m_v1, tvCut.GetXyDir(), pTv->m_v1, pTv->GetXyDir(), xyMatch))
				{
					pTv->m_v1	= xyMatch;
				}
				else
				{
					if(!bHunch)
					{
						tvArr.RemoveAt(j);
						j--;
					}
				}
			}
			else
			{
				if(GetXyMatchSegAndSeg(tvCut.m_v1, tvCut.m_v2, pTv->m_v1, pTv->m_v2, xyMatch))
				{
					pTv->m_v1	= xyMatch;
				}
				else
				{
					if(!bHunch)
					{
						tvArr.RemoveAt(j);
						j--;
					}
				}
			}
		}
		else if(pTv->m_v1.z == -1 || pTv->m_v2.z == 1)
		{
			// ��ũ�� ������ �� ������ �Է��� �ߴٸ� �Է��� �� �� �� ����
			break;
		}
	}
}

// ��ȣ�� ����ö�� 6,7,8,9�� ö�� ��ġ
void CRcBridgeRebarUtil::GetXyRebarCR6789OfGueardFence(CGuardWallRC *pGW, CDPointArray &xyArr6, CDPointArray &xyArr7, CDPointArray &xyArr8, CDPointArray &xyArr9, CTwinVectorArray &tvArrRebar)
{
	xyArr6.RemoveAll();
	xyArr7.RemoveAll();
	xyArr8.RemoveAll();
	xyArr9.RemoveAll();

		double	dH1 = pGW->m_dH1;
	double	dH2 = pGW->m_dH2;
	double	dH3 = pGW->m_dH3;
	double	dW1 = pGW->m_dW1;
	double	dW2 = pGW->m_dW2;
	double	dW4 = pGW->m_dW4;
	double	dW5 = pGW->m_dW5;
	
	// �� ö�� ����
	long nSize = tvArrRebar.GetSize();
	long nSideRebar = 2;
	if(dH1>0)	nSideRebar++;
	if(dH2>0)	nSideRebar++;
	if(dH3>0)	nSideRebar++;
	long nTopRebar = 3;
	long nRebar2 = 0;
	if((dH1>0 && dH2>0 && dH3>0) && ((dW1>0 && dW2>0) || (dW4>0 && dW5>0)))
		nRebar2 = 4;
	long nHorRebar = nSize - nSideRebar - nSideRebar - nTopRebar - nRebar2;

	CDPoint vUnit(0,0);
	double dLength = 0;	
	BOOL bLeft = pGW->IsLeftGuardFence();

	// SR_6,7,8,9...
	long nMarkNo = 6;
	char ch[5];
	CDPointArray xyArr;
	
	CTwinVector tv;
	long nGugan=0, nPrevGugan=0;
	long nSttShearNo = nSize-nHorRebar;
	CDPoint xy(0, 0);
	long i = 0; for(i = nSttShearNo; i < nSize; i++)
	{
		if(bLeft)
		{
			tv = tvArrRebar.GetAt(i);
			vUnit = (tv.m_v2 - tv.m_v1).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v1 + (vUnit * dLength);
		}
		else
		{
			tv = tvArrRebar.GetAt(i);
			vUnit = (tv.m_v1 - tv.m_v2).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v2+(vUnit*dLength);
		}
		
		// ����
		double dOriginY = tvArrRebar.GetAt(0).m_v1.y + pGW->m_dF2;
		if(tv.m_v1.y < dOriginY + pGW->m_dCrossF2-0.5)
			nGugan = 0;
		else if(tv.m_v1.y >= (dOriginY+pGW->m_dCrossF2-0.5) && tv.m_v1.y <= (dOriginY+pGW->m_dCrossF2+pGW->m_dCrossF3+0.5))
			nGugan = 1;
		else
			nGugan = 2;
		if(dH1==0 && dH2==0)
			nGugan = 0;
		
		if(i>nSttShearNo && nPrevGugan!=nGugan)
			_ITOT_S(nMarkNo++, ch, sizeof(ch), 10);
		
		CString str(_T(""));
		if(bLeft)
			str.Format(_T("CL%s"), ch); 
		else
			str.Format(_T("CR%s"), ch); 

		// ������ ���� ��ũ �׸���
		if(i==nSttShearNo || nPrevGugan==nGugan)
		{
			xyArr.Add(xy);
		}
		else
		{			
			long k=0; for(k=xyArr.GetSize()-1; k>=0; k--)
			{
				double dx = xyArr.GetAt(k).x;
				double dy = xyArr.GetAt(k).y;
				switch(nMarkNo)
				{
				case 6: xyArr6.Add(CDPoint(dx, dy)); break;
				case 7: xyArr7.Add(CDPoint(dx, dy)); break;
				case 8: xyArr8.Add(CDPoint(dx, dy)); break;
				case 9: xyArr9.Add(CDPoint(dx, dy)); break;
				}
			}
			
			xyArr.RemoveAll();
			xyArr.Add(xy);
		}
		if(i==nSize-1)
		{
			_ITOT_S(nMarkNo++, ch, sizeof(ch), 10);
			long k=0; for(k=xyArr.GetSize()-1; k>=0; k--)
			{
				double dx = xyArr.GetAt(k).x;
				double dy = xyArr.GetAt(k).y;
				switch(nMarkNo)
				{
				case 6: xyArr6.Add(CDPoint(dx, dy)); break;
				case 7: xyArr7.Add(CDPoint(dx, dy)); break;
				case 8: xyArr8.Add(CDPoint(dx, dy)); break;
				case 9: xyArr9.Add(CDPoint(dx, dy)); break;
				}
			}
			xyArr.RemoveAll();
		}
		nPrevGugan = nGugan;
	}
}

void CRcBridgeRebarUtil::SyncRebarAddRateOpt()
{
	// ö�� ������ ���� �ɼ�
	m_nTypeAddRate		= m_pRcBridgeApp->m_nTypeAddRate;	// 0 : ���溰, 1 : ��ö��,��Ÿö��
	m_dAddRateLower		= m_pRcBridgeApp->m_dAddRateLower;	// ���� ������
	m_dAddRateUpper		= m_pRcBridgeApp->m_dAddRateUpper;	// ���� ������
	m_dAddRateDiaOrg	= m_pRcBridgeApp->m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����
}
