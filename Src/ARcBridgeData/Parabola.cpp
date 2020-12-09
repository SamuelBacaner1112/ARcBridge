// Parabola.cpp: implementation of the CParabola class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parabola.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParabola::CParabola()
{
	m_nFlag				= 0;
	m_xyCrown			= CDPoint(0, 0);
	m_dDistTotal			= 0;
	m_dHeightTotal		= 0;
	m_dDistStt			= 0;
	m_dDistEnd			= 0;
	m_dFactorM			= 0;
	m_dLenArchLine		= 0;
	m_dOffsetFromUpper	= 0;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�
}

CParabola::~CParabola()
{

}

CParabola& CParabola::operator=(const CParabola& Obj)
{
	m_xyCrown			= Obj.m_xyCrown;
	m_dDistTotal			= Obj.m_dDistTotal;
	m_dHeightTotal		= Obj.m_dHeightTotal;
	m_dDistStt			= Obj.m_dDistStt;
	m_dDistEnd			= Obj.m_dDistEnd;
	m_dFactorM			= Obj.m_dFactorM;
	m_dLenArchLine		= Obj.m_dLenArchLine;
	m_dOffsetFromUpper	= Obj.m_dOffsetFromUpper;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�

	return *this;
}

void CParabola::Serialize(CArchive& ar)
{
	m_nFlag = 0;
	if(ar.IsStoring())
	{
		ar << m_nFlag;
		ar << m_xyCrown;
		ar << m_dDistTotal;
		ar << m_dHeightTotal;
		ar << m_dDistStt;
		ar << m_dDistEnd;
		ar << m_dFactorM;
		ar << m_dLenArchLine;
		ar << m_dOffsetFromUpper;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�	
	}
	else
	{
		ar >> m_nFlag;
		ar >> m_xyCrown;
		ar >> m_dDistTotal;
		ar >> m_dHeightTotal;
		ar >> m_dDistStt;
		ar >> m_dDistEnd;
		ar >> m_dFactorM;
		ar >> m_dLenArchLine;
		ar >> m_dOffsetFromUpper;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�	
	}
}

// ������ offset
void CParabola::GetOffsetParabola(CParabola &pb, double dOffset)
{
	pb	= *this;

	CDPoint xyStt		= GetXyByDist(-pb.m_dDistTotal/2);
	CDPoint xyEnd		= GetXyByDist(pb.m_dDistTotal/2);
	CDPoint xyDirStt	= GetXyDirPerByDist(-pb.m_dDistTotal/2);
	CDPoint xyDirEnd	= GetXyDirPerByDist(pb.m_dDistTotal/2);

	xyStt	= xyStt + xyDirStt * dOffset;
	xyEnd	= xyEnd + xyDirEnd * dOffset;


	pb.m_xyCrown.y += dOffset;
	pb.m_dDistTotal	= xyEnd.x - xyStt.x;
	pb.m_dHeightTotal = (pb.m_xyCrown.y + pb.m_dOffsetFromUpper) - xyStt.y; 

	xyStt	= GetXyByDist(m_dDistStt);
	xyDirStt	= GetXyDirPerByDist(m_dDistStt);
	xyEnd	= GetXyByDist(m_dDistEnd);
	xyDirEnd	= GetXyDirPerByDist(m_dDistEnd);
	xyStt	= xyStt + xyDirStt * dOffset;
	xyEnd	= xyEnd + xyDirEnd * dOffset;


	pb.m_dDistStt	= pb.GetDistByXPos(xyStt.x);
	pb.m_dDistEnd	= pb.GetDistByXPos(xyEnd.x);
}

void CParabola::Init()
{
	m_nFlag				= 0;
	m_xyCrown			= CDPoint(0, 0);
	m_dDistTotal			= 0;
	m_dHeightTotal		= 0;
	m_dDistStt			= 0;
	m_dDistEnd			= 0;
	m_dFactorM			= 0;
	m_dLenArchLine		= 0;
	m_dOffsetFromUpper	= 0;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�
}

// CDPoint CParabola::GetXyCen()
// {
// 	CDPoint xyCen	= m_xyCrown;
// 	xyCen.y -= (m_dOffsetFromUpper + m_dHeightTotal);
// 
// 	return xyCen;
// }

// ũ���� dDist��ġ�� ��ǥ
CDPoint CParabola::GetXyByDist(double dDist)
{
	return GetXyInArchByCatenary(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDist);
}

// ��ǥ���� vAng�� ��� ���� �Ǵ� ������ ���� ��
CDPoint CParabola::GetXyMatchXyAndAngle(CDPoint xy, CDPoint vAng, BOOL bLeft)
{
	CDPoint xyMatch(0, 0);
	if(!GetXyMatchByCatenaryXyAndAngle(m_dDistTotal, m_dHeightTotal, m_dFactorM, CDPoint(m_xyCrown.x, m_xyCrown.y-m_dOffsetFromUpper), xy, vAng, xyMatch, 0, 1.5))
		xyMatch	= xy;

	if(!bLeft)
	{
		xyMatch.x = xyMatch.x + (m_xyCrown.x - xyMatch.x) * 2;
	}

	return xyMatch;
}

// �ش� dDist���� ������ ����
CDPoint CParabola::GetXyDirPerByDist(double dDist, BOOL bRev)
{
	CDPoint xyDir	= GetXyDirTanByDist(dDist);
	return bRev ? xyDir.RotateInv90() : xyDir.Rotate90();
}

// �������� �� ��ġ
CDPoint CParabola::GetXyPerByDistAndOffset(double dDist, double dOffset)
{
	CDPoint xyDir	= GetXyDirPerByDist(dDist);
	CDPoint xy		= GetXyByDist(dDist);

	xy = xy + xyDir * dOffset;

	return xy;
}

// ���� ����
CDPoint CParabola::GetXyDirTanByDist(double dDist)
{
	CDPoint xyDir	= GetAngleInArchByCatenary(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDist);

	return xyDir;
}

double CParabola::GetLenByDistToDist(double dDistStt, double dDistEnd)
{
	double dLen	= 0;
	if(dDistEnd < dDistStt)
	{
		dLen	= GetLengthInArchByCatenary(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDistStt*-1, dDistEnd*-1);
	}
	else
	{
		dLen	= GetLengthInArchByCatenary(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDistStt, dDistEnd);
	}

	return dLen;
}

double CParabola::GetDistByDistToLen(double dDist, double dLen)
{
	double dDistEnd	= dDist;
	if(dLen < 0)
	{
		dDistEnd	= GetDistInArchByCatenaryLength(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDist*-1, dLen*-1);
		dDistEnd *= -1;
	}
	else
	{
		dDistEnd	= GetDistInArchByCatenaryLength(m_dDistTotal, m_dHeightTotal, m_dFactorM, m_xyCrown, dDist, dLen);
	}

	return dDistEnd;
}

double CParabola::GetDistByXPos(double dX)
{
	return dX - m_xyCrown.x;
}

// �β���� ������� �Ի�
void CParabola::SetCalcRate()
{
	m_dRatefl		= m_dHeightTotal / m_dDistTotal;
}
