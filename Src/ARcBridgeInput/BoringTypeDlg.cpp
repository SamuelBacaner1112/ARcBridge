// BoringTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "BoringTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoringTypeDlg dialog
#define	SOILCOUNT		37
CString strSoilName[SOILCOUNT] = 
	{
		"실트", "점토", "모래 몰탈", "자갈질 모래 콘크리트", "실트질 모래", "실트질 자갈", "점토질 모래",
		"점토질 자갈", "풍화암", "연암", "경암", "퇴적토", "표토", "매립토", "취약암", "풍화토", 
		"이탄토", "자갈", "호박돌", "붕적토", "사질토", "극경암", "보통암", "충적토", "전석",
		"실트 섞인 점토(CL)", "점토 섞인 실트(ML)", "점토 섞인 모래(SC)", "실트 섞인 모래(SM)", "점토 섞인 자갈(GC)",
		"모래(SP)", "실트 섞인 자갈(GM)", "모래와 자갈 섞인 호박돌", "풍화 잔류토(RS)",
		"풍화암(WR)", "연암(SR)", "경암(HR)"
	};

CBoringTypeDlg::CBoringTypeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBoringTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoringTypeDlg)
	m_nBoringType = 0;
	//}}AFX_DATA_INIT
}


void CBoringTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoringTypeDlg)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Radio(pDX, IDC_RADIO1, m_nBoringType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoringTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CBoringTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoringTypeDlg message handlers
BOOL CBoringTypeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

		
	long n=0; for(n=0; n<SOILCOUNT; n++)
	{
		if(m_ReturnValue==strSoilName[n]) m_nBoringType = n;
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBoringTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_ReturnValue = strSoilName[m_nBoringType];
	CUserDialog::OnOK();
}
