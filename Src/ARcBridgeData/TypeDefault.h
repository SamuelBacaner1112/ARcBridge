// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// TypeDefault.h: interface for the CTypeDefault class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPEDEFAULT_H__4336D273_34A5_4288_A74A_F2EAE5C744AF__INCLUDED_)
#define AFX_TYPEDEFAULT_H__4336D273_34A5_4288_A74A_F2EAE5C744AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 권고안 종류를 각 다이얼로그 각 교량마다 설정한다.
class CTypeDefault : public CObject  
{
public:
	long m_nFlag;
	long m_nIDDialog;			// 입력창 id
	DWORD m_dwDefaulted;		// 권고안 된 상태

	
public:
	CTypeDefault& operator=(const CTypeDefault& Obj);
	void Serialize(CArchive &ar);
	BOOL IsDefaulted(DWORD dwType);
	CTypeDefault();
	virtual ~CTypeDefault();

};

#endif // !defined(AFX_TYPEDEFAULT_H__4336D273_34A5_4288_A74A_F2EAE5C744AF__INCLUDED_)
