// OptionItem.cpp: implementation of the COptionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgeoutdesign.h"
#include "OptionItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionItem::COptionItem()
{
	m_strArrItem.RemoveAll();
	m_dArrItem.RemoveAll();
	m_dArrEnable.RemoveAll();
}

COptionItem::~COptionItem()
{

}

void COptionItem::AddItem(CString strItem, BOOL bInitCheck, BOOL bEnable)
{
	m_strArrItem.Add(strItem);
	m_dArrItem.Add(bInitCheck ? 1 : 0);
	m_dArrEnable.Add(bEnable ? 1 : 0);
}

CString COptionItem::GetItem(long nIndex)
{
	return m_strArrItem.GetAt(nIndex);

}

BOOL COptionItem::IsCheck(CString strItem)
{
	long nIndex = -1;
	long nSize = m_strArrItem.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CString str = m_strArrItem.GetAt(i);
		if(strItem == str)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex>-1) 
		return (m_dArrItem.GetAt(nIndex) == 0) ? FALSE : TRUE;
	else
		return FALSE;

}


BOOL COptionItem::IsCheck(long nIndex)
{
	return (m_dArrItem.GetAt(nIndex)==0) ? FALSE : TRUE;
}


void COptionItem::SetCheck(long nIndex, BOOL bCheck)
{
	DWORD dwCheck = bCheck ? 1 : 0;
	m_dArrItem.SetAt(nIndex, dwCheck);

}

long COptionItem::GetOptionSize()
{
	return m_strArrItem.GetSize();

}

void COptionItem::RemoveAllData()
{
	m_strArrItem.RemoveAll();
	m_dArrItem.RemoveAll();
	m_dArrEnable.RemoveAll();
}

// long COptionItem::GetCheckedOptionSize()
// {
// 	long nRetV = 0;
// 	long nSize = m_dArrItem.GetSize();
// 	long i=0; for(i=0; i<nSize; i++)
// 	{
// 		if(m_dArrItem.GetAt(i)==1) 
// 			nRetV++;
// 	}
// 
// 	return nRetV;
// 
// }

BOOL COptionItem::IsEnable(long nIndex)
{
	return (BOOL)m_dArrEnable.GetAt(nIndex);
}