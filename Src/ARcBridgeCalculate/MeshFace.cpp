// MeshFace.cpp: implementation of the CMeshFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshFace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshFace::CMeshFace()
{
	m_nIndexFace = -1;
	long i=0; for(i=0; i<4; i++)
	{
		m_vPoint[i] = CVector(0,0,0);
		m_nIndexJoint[i] = -1;
	}
	m_nTypeHDan		= -1;
	m_nIndexHDan	= -1;
	m_nIndexPos		= -1;
	m_nIndexPosStd	= -1;
	m_nSpanSlab		= -1;
	m_bSpanCheckStt	= FALSE;
	m_bSpanCheckEnd	= FALSE;
}

CMeshFace::~CMeshFace()
{

}
