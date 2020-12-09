// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ExpJointRef.h: interface for the CExpJointRef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPJOINTREF_H__59A14AE3_A97A_439E_97E1_FC10B9D0E95E__INCLUDED_)
#define AFX_EXPJOINTREF_H__59A14AE3_A97A_439E_97E1_FC10B9D0E95E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const static char* ExpJointRef[] =
{	
//  단위 : mm, Kg, m2, m3
//   모델명		최대허용신축량	블럭아웃폭 블럭아웃높이 유간(최소)   유간(최대)
	"UCF-50S"		,"50"		,"300"		,"300"		,"25"		,"75"		,
	"UCF-75S"		,"75"		,"310"		,"300"		,"42.5"		,"117.5"	,
	"UCF-80S"		,"80"		,"310"		,"300"		,"45"		,"125"		,
	"UCF-100S"		,"100"		,"315"		,"300"		,"55"		,"155"		,
	"UCF-120S"		,"120"		,"315"		,"300"		,"65"		,"185"		,
	"UCF-150S"		,"150"		,"325"		,"325"		,"80"		,"230"		,
	"UCF-200S"		,"200"		,"340"		,"325"		,"110"		,"310"		,
	"UCF-250S"		,"250"		,"360"		,"325"		,"140"		,"390"		,
	"UCF-300S"		,"300"		,"380"		,"350"		,"165"		,"465"		,
	"UCF-350S"		,"350"		,"390"		,"350"		,"195"		,"545"		,
	"UCF-400S"		,"400"		,"405"		,"350"		,"220"		,"620"		,
	"UCF-450S"		,"450"		,"415"		,"350"		,"250"		,"700"		,
	"UCF-500S"		,"500"		,"425"		,"375"		,"280"		,"780"		,
	"UCF-550S"		,"550"		,"440"		,"375"		,"305"		,"855"		,
	"UCF-600S"		,"600"		,"455"		,"375"		,"335"		,"935"		,
	"MC-2G"			,"20"		,"250"		,"200"		,"50"		,"70"		,
	"MC-4G"			,"40"		,"250"		,"200"		,"80"		,"120"		,
	"MC-6G"			,"60"		,"250"		,"200"		,"80"		,"140"		,
	"URJ50"			,"50"		,"200"		,"200"		,"25"		,"75"		,
	"URJ80"			,"80"		,"250"		,"250"		,"45"		,"125"		,
	"URJ100"		,"100"		,"250"		,"250"		,"45"		,"145"		,
	"URJ160"		,"160"		,"400"		,"330"		,"90"		,"250"		
};


class AFX_EXT_CLASS CExpJointRef : public CObject  
{
public:
	CExpJointRef();
	virtual ~CExpJointRef();

	void Serialize(CArchive& ar);
	CExpJointRef& operator=(const CExpJointRef& Obj);

public:
	CString m_szName;
	double m_dMaxAllowExpL;
	double m_dBlockW;
	double m_dBlockH;
	double m_dUganMin;
	double m_dUganMax;
};

#endif // !defined(AFX_EXPJOINTREF_H__59A14AE3_A97A_439E_97E1_FC10B9D0E95E__INCLUDED_)


class AFX_EXT_CLASS CExpJointRefArr : public CObject  
{
public:
	CExpJointRefArr();
	virtual ~CExpJointRefArr();
protected:
	CTypedPtrArrayEx < CObArray, CExpJointRef* > m_arrExpJointRef;

// Implementation
public:

	void	Serialize(CArchive& ar);
	void	SetRefDefault();

	long	GetSize()	{ return m_arrExpJointRef.GetSize();	}
	CExpJointRef* GetExpJointRef(CString szName);
	CExpJointRef* GetExpJointRef(long nIndex) { return m_arrExpJointRef[nIndex]; }
//	long	GetIndexByExpJointName(CString szName);
};
