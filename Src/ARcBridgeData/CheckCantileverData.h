#pragma once

//////////////////////////////////////////////////////////////////////////
// 작용하중 데이터 클래스
//////////////////////////////////////////////////////////////////////////
const int BANGUM					= 0;
const int NANGAN					= 1;

const int LEFT_CANTILEVER_FLOOR			= 0;	// 좌측캔틸레버(외측가로보가 있을 경우-브라켓비접촉부)
const int RIGHT_CANTILEVER_FLOOR		= 1;	// 우측캔틸레버(외측가로보가 있을 경우-브라켓비접촉부)

const int USERLOAD_SIZE				= 2;

// 방음벽, 난간
struct _USER_LOAD
{
	HGBOOL m_bExist;
	double m_dWeight;
	double m_dHeight;
	double m_dDist;
	BOOL m_bDw;		// LSD 하중재하고려사항 - 방음벽(좌우)
};

struct _BODO	// 보도자중
{
	BOOL m_bExist;
	double m_dWeightStt;
	double m_dWeightEnd;
	double m_dHeight;
};	

class AFX_EXT_CLASS CWorkingLoad
{
public:
	CWorkingLoad(void);
	~CWorkingLoad(void);

	void Serialize(CArchive &ar);
	CWorkingLoad& operator=(const CWorkingLoad &Obj);

private:
	_USER_LOAD m_UserLoad[USERLOAD_SIZE];		// [방음벽,난간]
	_BODO m_BodoLoad;	

	// 군중하중
	BOOL m_bWalkLoad;
	long m_nWorkLoadType;				// 주거더 해석시 군중하중 재하 방식	- 0: 고정하중 타입 , 1: 이동하중 타입

	// 	double	m_dCo_H;
	// 
	double	m_dHoriF;
	double	m_dVerF;
	long	m_nHorType;					// 바닥판 설계 고려사항 -> "하중재하 고려사항" : 수평력 도로타입 0:일반도로, 1:도심도로, 2:사용자입력

public:
 	_USER_LOAD &GetUserLoad(long nType);
 	_BODO &GetBodoWeight();
	BOOL &GetWalkLoad();
	long &GetWorkLoadType();
	double &GetHoriF();
	double &GetVertF();
	long &GetHoriType();
};

//한계상태 충격하중계수	//ARoad-4553
const static double _dImpactLoad[] = {0.7, 0.25, 0.15};


class AFX_EXT_CLASS CCheckCantileverData
{
public:
	CCheckCantileverData(void);
	~CCheckCantileverData(void);

	void Serialize(CArchive& ar);
	CCheckCantileverData& operator=(const CCheckCantileverData &Obj);

public:
	CWorkingLoad *GetWorkingLoadData(HGBOOL bLeft);
	
	//////////////////////////////////////////////////////////////////////////
	// 설계선택사항
	//////////////////////////////////////////////////////////////////////////
	double	m_dCenter_R[3];					//설계반경 [좌][우][중앙]
	double	m_dCo_V;						//설계속도

	BOOL m_bCentMugun;										// 중분대 무근콘크리트
	BOOL m_bMugun;			// 무근콘크리트
	BOOL m_bFloorTank;
	BOOL m_bFloorTrailer;

	double	m_dWindStrength;		// 풍하중 강도
	BOOL m_bWind;		// m_bWind 로 통일 ( 철도교 / 도로교 구분X )
	BOOL m_bFloowWind;	// 바닥판 풍하중 적용

	// ARoad는 CBaseDesignCtl 클래스로 나누어져있음.
	long	m_nSlabCentDesignMethod;					// 바닥판 중앙부 철근 계산방법

	long	m_nCent_HeightType;		// 작용위치 타입 ( 0:HL하중, 1:LS하중, 2:EL하중, 3:사용자하중)

	// 콘크리트 피복 두께
	double	m_dCoverUp;
	double	m_dCoverDn;
	double	m_dCoverSd;

private:
	CWorkingLoad m_WorkingLoadData[2];		// [좌,우] - 작용하중
};