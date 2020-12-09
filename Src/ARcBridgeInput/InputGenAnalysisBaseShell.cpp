// InputGenAnalysisBaseShell.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "arcbridgeinput.h"

// CInputGenAnalysisBaseShell 대화 상자입니다.
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CInputGenAnalysisBaseShell, CInputGenAnalysisBase)

CInputGenAnalysisBaseShell::CInputGenAnalysisBaseShell(UINT nID, CWnd* pParent /*=NULL*/)
	: CInputGenAnalysisBase(nID, pParent)
{
	m_nForceType = FORCE_F11;
	m_nForceTypeFrame = ELE_MOMENT2;
}

CInputGenAnalysisBaseShell::~CInputGenAnalysisBaseShell()
{
}

void CInputGenAnalysisBaseShell::DoDataExchange(CDataExchange* pDX)
{
	CInputGenAnalysisBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputGenAnalysisBaseShell, CInputGenAnalysisBase)
	ON_BN_CLICKED(IDC_RADIO_FXX, OnRadioFxx)
	ON_BN_CLICKED(IDC_RADIO_FYY, OnRadioFyy)
	ON_BN_CLICKED(IDC_RADIO_FXY, OnRadioFxy)
	ON_BN_CLICKED(IDC_RADIO_FMAX,OnRadioFmax)
	ON_BN_CLICKED(IDC_RADIO_FMIN, OnRadioFmin) 
	ON_BN_CLICKED(IDC_RADIO_MXX,OnRadioMxx) 
	ON_BN_CLICKED(IDC_RADIO_MYY,OnRadioMyy) 
	ON_BN_CLICKED(IDC_RADIO_MXY,OnRadioMxy) 
	ON_BN_CLICKED(IDC_RADIO_MMAX,OnRadioMmax) 
	ON_BN_CLICKED(IDC_RADIO_MMIN,OnRadioMmin) 
	ON_BN_CLICKED(IDC_RADIO_VXX,OnRadioVxx) 
	ON_BN_CLICKED(IDC_RADIO_VYY,OnRadioVyy)
	ON_BN_CLICKED(IDC_RADIO_V22, OnRadioV22)
	ON_BN_CLICKED(IDC_RADIO_V33, OnRadioV33)
	ON_BN_CLICKED(IDC_RADIO_M22, OnRadioM22)
	ON_BN_CLICKED(IDC_RADIO_M33, OnRadioM33)
	ON_BN_CLICKED(IDC_RADIO_AXIAL, OnRadioAxial)
	ON_BN_CLICKED(IDC_RADIO_TORTION, OnRadioTortion)
END_MESSAGE_MAP()


// CInputGenAnalysisBaseShell 메시지 처리기입니다.
void CInputGenAnalysisBaseShell::OnRadioFxx() 
{	
	m_nForceType = FORCE_F11;	
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioFyy() 
{
	m_nForceType = FORCE_F22;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioFxy() 
{
	m_nForceType = FORCE_F12;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioFmax() 
{
	m_nForceType = FORCE_MAX;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioFmin() 
{
	m_nForceType = FORCE_MIN;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioMxx() 
{
	m_nForceType = MOMENT_M11;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioMyy() 
{
	m_nForceType = MOMENT_M22;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioMxy() 
{
	m_nForceType = MOMENT_M12;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioMmax() 
{
	m_nForceType = MOMENT_MAX;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioMmin() 
{
	m_nForceType = MOMENT_MIN;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioVxx() 
{
	m_nForceType = VER_13;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioVyy() 
{
	m_nForceType = VER_23;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioV22() 
{	
	m_nForceTypeFrame = ELE_SHEAR_2;
	DrawInputDomyunView(FALSE);
}

void CInputGenAnalysisBaseShell::OnRadioV33() 
{
	m_nForceTypeFrame = ELE_SHEAR_3;
	DrawInputDomyunView(FALSE);	
}

void CInputGenAnalysisBaseShell::OnRadioM22() 
{
	m_nForceTypeFrame = ELE_MOMENT2;
	DrawInputDomyunView(FALSE);	
}

void CInputGenAnalysisBaseShell::OnRadioM33() 
{
	m_nForceTypeFrame = ELE_MOMENT3;
	DrawInputDomyunView(FALSE);	
}

void CInputGenAnalysisBaseShell::OnRadioAxial() 
{
	m_nForceTypeFrame = ELE_AXFORCE;
	DrawInputDomyunView(FALSE);	
}

void CInputGenAnalysisBaseShell::OnRadioTortion() 
{
	m_nForceTypeFrame = ELE_AXTORSN;
	DrawInputDomyunView(FALSE);	
}