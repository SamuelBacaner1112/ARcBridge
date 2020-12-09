// DimParabola.cpp: implementation of the CDimParabola class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/Parabola.h"
#include "ARcBridgeDBDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDimParabola::CDimParabola(CDomyun *pDom, CParabola *pPb):CDomyun(pDom)
{
	m_pMainDom		= pDom;
	m_pPb			= pPb;
	m_dLastXDis		= 0;
	m_xyLastDotPos	= CDPoint(0, 0);
	m_bFirst		= TRUE;
}

CDimParabola::CDimParabola()
{

}

CDimParabola::~CDimParabola()
{

}

void CDimParabola::DimMoveToParabola(double xDis, long nDan, CString sText1, CString sText2)
{
	if(!m_pPb) return;

	double dExo		= Always(m_DIMEXO);
	double dExe		= Always(m_DIMEXE);
	double dVLen	= Always(m_DIMVLEN);
	double dLevDis	= Always(m_DIMLEVELDIS) * nDan;
	if(!IsUpperDim())
	{
		dExo	= -dExo;
		dExe	= -dExe;
		dVLen	= -dVLen;
		dLevDis	= -dLevDis;
	}


	// 직선 긋기
	CDPoint xyLower	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis);
	CDPoint xyMid	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen);
	CDPoint xyUpper	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen + dExe);
	
	LineTo(xyLower, xyUpper);

	// 도트 표시
	double dAng	= m_pPb->GetXyDirTanByDist(xDis).GetAngleDegree();
	BlockDot(xyMid, dAng, TRUE);

	// 텍스트가 있다면
	if(sText1 != "")
	{
		double textAngle = GetTextAngle();	
		long oldHor = GetTextAlignHorz();

		CString csMax	= sText1.GetLength() > sText2.GetLength() ? sText1 : sText2;
		double dAngMid	= m_pPb->GetXyDirPerByDist(xDis).RotateInv90().GetAngleDegree();
		SetTextAlignHorz(TA_RIGHT);
		SetTextAngle(dAngMid+textAngle);	

		double dTvp	= Always(m_DIMTVP);
		CDPoint xyText	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen + dTvp);
		TextOut(xyText, sText1);	

		if(sText2 != "")
		{
			xyText	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen - dTvp - GetTextHeight());
			TextOut(xyText, sText2);
		}

		SetTextAngle(textAngle);	
		SetTextAlignHorz(oldHor);
	}

	m_dLastXDis		= xDis;
	m_xyLastDotPos	= xyMid;
}

void CDimParabola::DimLineToParabola(double dist, long nDan, CString sText1, CString sText2, BOOL bOver)
{
	if(!m_pPb) return;
	if(dist == 0) return;
	double xDis	= m_pPb->GetDistByDistToLen(m_dLastXDis, dist);

	double dExo		= Always(m_DIMEXO);
	double dExe		= Always(m_DIMEXE);
	double dVLen	= Always(m_DIMVLEN);
	double dLevDis	= Always(m_DIMLEVELDIS) * nDan;
	if(!IsUpperDim())
	{
		dExo	= -dExo;
		dExe	= -dExe;
		dVLen	= -dVLen;
		dLevDis	= -dLevDis;
	}

	// 처음 LineTo가 아니라면 도트를 또 그려줌
	if(!m_bFirst)
	{
		CDPoint xyMid	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen);
		double dAng	= m_pPb->GetXyDirPerByDist(m_dLastXDis).RotateInv90().GetAngleDegree();

		BlockDot(m_xyLastDotPos, dAng, TRUE);
	}


	// 직선 긋기
	CDPoint xyLower	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis);
	CDPoint xyMid	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen);
	CDPoint xyUpper	= m_pPb->GetXyPerByDistAndOffset(xDis, dExo + dLevDis + dVLen + dExe);
	
	LineTo(xyLower, xyUpper);

	// 도트 표시
	double dAng	= m_pPb->GetXyDirTanByDist(xDis).GetAngleDegree();
	BlockDot(xyMid, dAng, FALSE);


	// 포물선 그리기
	CHgBaseDrawStd baseDraw;
	m_pStd->DrawParabola(this, m_pPb, m_dLastXDis, xDis, dExo + dLevDis + dVLen);

	// 텍스트 기입
	double textAngle = GetTextAngle();	
	long oldHor = GetTextAlignHorz();

	CString csMax	= sText1.GetLength() > sText2.GetLength() ? sText1 : sText2;
	double dLenText = GetTextWidth(csMax);
	double dXDisMid	= bOver ? xDis : (m_dLastXDis + xDis)/2;
	double dAngMid	= m_pPb->GetXyDirPerByDist(dXDisMid).RotateInv90().GetAngleDegree();
	SetTextAngle(dAngMid+textAngle);	
	if(bOver)
	{
		SetTextAlignHorz(TA_LEFT);
		LineTo(xyMid, xyMid + (ToDPointFrDegree(dAngMid) * dLenText));
	}

	double dTvp	= Always(m_DIMTVP);
	CDPoint xyText	= m_pPb->GetXyPerByDistAndOffset(dXDisMid, dExo + dLevDis + dVLen + dTvp);
	TextOut(xyText, sText1);

	if(sText2 != "")
	{
		xyText	= m_pPb->GetXyPerByDistAndOffset(dXDisMid, dExo + dLevDis + dVLen - dTvp - GetTextHeight());
		TextOut(xyText, sText2);
	}

	SetTextAngle(textAngle);	
	SetTextAlignHorz(oldHor);


	m_dLastXDis		= xDis;
	m_xyLastDotPos	= xyMid;
	m_bFirst		= FALSE;
}

// 위로 쓰는 치수인지?
BOOL CDimParabola::IsUpperDim()
{
	BOOL bUpper	= m_CurDirection == DIM_BOTTOM || m_CurDirection == DIM_RIGHT ? FALSE : TRUE;

	return bUpper;
}

void CDimParabola::BlockDot(CDPoint xy, double dAng, BOOL bStart)
{	
	double x	= xy.x;
	double y	= xy.y;
	if(m_bBlock)
	{
		CString BlockName = "DOT0";
		CDomyun *pDom = new CDomyun(this);	
		if(m_nDimDotValue==0)
		{
			pDom->Circle(0,0,Always(m_ArrowHeadSize/2),TRUE);
		}
		else if(m_nDimDotValue==1)
		{
			// 화살표를 반대방향으로 그리기
			double Ang = bStart ? ConstPi : 0;			
			Arrow(x,y,Ang+dAng);			
		}
		else
		{
			pDom->Circle(0,0,Always(m_ArrowHeadSize/2),FALSE);
		}

		if(m_pMainDom)
		{
			long nID = 0;
			while(m_pMainDom->IsExistBlock(BlockName) || IsExistBlock(BlockName))
			{
				nID ++;
				BlockName.Format("DOT%d",nID);
			}
		}
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(x,y));		
	}
	else				
	{
		if(m_nDimDotValue==0)
		{
			Circle(x,y,Always(m_ArrowHeadSize/2),TRUE);	
		}
		else if(m_nDimDotValue==1)
		{
			// 화살표를 반대방향으로 그리기
			double Ang = bStart ? ConstPi : 0;				
			Arrow(x,y,Ang+dAng);		
		}
		else
		{
			Circle(x,y,Always(m_ArrowHeadSize/2),FALSE);
		}
	}
}

// void CDimParabola::SetParabola(CDomyun *pDomP, CParabola *pPb)
// {
// 	m_pMainDom	= pDomP;
// 	m_pPb	= pPb;
// }
