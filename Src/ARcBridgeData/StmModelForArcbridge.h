#pragma once


class CStmModelForArcbridge :
	public CStmModel
{
public:
	CStmModelForArcbridge(void);
	~CStmModelForArcbridge(void);

	// 공통 적용 데이터
	HGINT32 m_nTypePosRebarTie;
	HGDOUBLE m_dUserPosRebarTie;
	HGINT32 m_nTypePosCompressStrut;
	HGDOUBLE m_dUserPosCompressStrut;

	// 우각부 적용데이터
	HGINT32 m_nTypeLoadStressFlow;		//응력흐름도 하중재하방법
	HGINT32 m_nTypeStmModeling;			//모델링 Type

	HGINT32 m_nTypeDistCornerRebar;		//외측경사부의 위치
	HGDOUBLE m_dUserDistCornerRebar;		//외측경사부의 위치 설정값
	HGINT32 m_nTypeDistHunchRebar;		//내측경사부의 위치
	HGDOUBLE m_dUserDistHunchRebar;		//내측경사부의 위치 설정값
	HGINT32 m_ixInnerAngle;		//부재두께비가 큰 경우 기준각도
	HGINT32 m_nTypeStirrupInnerTension;	//내측인장 스트럽부재
	HGDOUBLE m_dUserStirrupInnerTension;	//내측인장 스트럽부재 설정값


	void InitData();
	void Serialize(CArchive &ar);
};

