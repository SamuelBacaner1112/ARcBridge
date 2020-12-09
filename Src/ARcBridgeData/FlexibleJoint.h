// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// FlexibleJoint.h: interface for the CFlexibleJoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEXIBLEJOINT_H__A8B862DB_A5AB_45DA_9639_B3B68D8F71EA__INCLUDED_)
#define AFX_FLEXIBLEJOINT_H__A8B862DB_A5AB_45DA_9639_B3B68D8F71EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 신축이음
class AFX_EXT_CLASS CFlexibleJoint : public CObject  
{
public:
	long m_nFlag;

// 단면 제원
	double m_dWidth;
	double m_dHeight;

// 수량 산출
	CString m_sName;
	double m_dLenMargin;
	double m_nCountMargin;
	CRebar m_rb[2];

// 철근
	double m_dLenHorRebar[MAX_REBAR_CYCLE][MAX_REBAR_DAN];	// 하면 철근의 상부 수평 길이

	void Serialize(CArchive &ar);
	CFlexibleJoint& operator=(const CFlexibleJoint& Obj);
	CTwinVectorArray m_tvArrSection;
public:
	
	CFlexibleJoint();
	virtual ~CFlexibleJoint();

};

#endif // !defined(AFX_FLEXIBLEJOINT_H__A8B862DB_A5AB_45DA_9639_B3B68D8F71EA__INCLUDED_)
