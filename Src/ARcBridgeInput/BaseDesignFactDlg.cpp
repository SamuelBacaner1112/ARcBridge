// BaseDesignFactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "BaseDesignFactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseDesignFactDlg dialog


CBaseDesignFactDlg::CBaseDesignFactDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBaseDesignFactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseDesignFactDlg)
	m_szTitle = _T("");
	m_szFact = _T("");
	m_dLB = 0.0;
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_dLB = 0;
	m_pFooting = NULL;
}

CBaseDesignFactDlg::CBaseDesignFactDlg(CFootingApp *pFooting, CWnd* pParent /*=NULL*/)
	: CUserDialog(CBaseDesignFactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseDesignFactDlg)
	m_szTitle = _T("");
	m_szFact = _T("");
	m_dLB = 0.0;
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_dLB = 0;
	m_pFooting = pFooting;
}


void CBaseDesignFactDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseDesignFactDlg)
	DDX_Text(pDX, IDC_STATIC_FACTTITLE, m_szTitle);
	DDX_Text(pDX, IDC_EDIT_FACTVALUE, m_szFact);
	DDX_Text(pDX, IDC_EDIT_RD, m_dLB);
	DDV_MinMaxDouble(pDX, m_dLB, 1., 4.0);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
}


BEGIN_MESSAGE_MAP(CBaseDesignFactDlg, CDialog)
	//{{AFX_MSG_MAP(CBaseDesignFactDlg)
	ON_BN_CLICKED(IDC_BUTTON_REDRAW, OnButtonRedraw)
	ON_BN_CLICKED(ID_BUTTON_RECOMMAND, OnButtonRecommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDesignFactDlg message handlers

void CBaseDesignFactDlg::OnOK() 
{
	UpdateData();
	m_ReturnValue = m_szFact;
	
	CUserDialog::OnOK();
}

BOOL CBaseDesignFactDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	if(!m_pFooting) 
	{
		OnCancel();
		return TRUE;
	}
	
	if(m_dLB==0)
	{		
		m_dLB = m_pFooting->GetLB();
		UpdateData(FALSE);
	}

	m_szFact = m_ReturnValue;

	switch(m_nType)
	{
	case 0:	m_szTitle = "수평변위에 대한 형상계수 (X방향) :";	break;
	case 1:	m_szTitle = "수평변위에 대한 형상계수 (Y방향) :";	break;
	case 2:	m_szTitle = "수직변위에 대한 형상계수 (Z방향) :";	break;
	case 3:	m_szTitle = "회전 로킹에 대한 형상계수 (X축) :";	break;
	case 4:	m_szTitle = "회전 로킹에 대한 형상계수 (Y축) :";	break;
	case 5:	m_szTitle = "회전 비틀림에 대한 형상계수 (Z축) :";	break;
	}
	UpdateData(FALSE);
	CString szDxfFile = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory") + "\\DXF";
	m_DomView.GetDomyun()->ClearEtt(TRUE, FALSE);
	m_DomView.GetDomyun()->FixEtt(FALSE);
	m_DomView.GetDomyun()->DxfIn(szDxfFile + "\\Table2.dxf");
	m_DomView.GetDomyun()->FixEtt(TRUE);
	m_DomView.ZoomAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBaseDesignFactDlg::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->hwnd == m_DomView.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE) ||
	   (pMsg->hwnd == m_DomView.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN))
	{		
		CPoint point;
		POINTS pt = MAKEPOINTS(pMsg->lParam);
		point.x = pt.x;
		point.y = pt.y;
		UINT nFlags = pMsg->wParam;
		CDPoint ptPos = m_DomView.ClientToExt(point);
		OnLButtonDownDom(nFlags, ptPos);		
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CBaseDesignFactDlg::OnLButtonDownDom(UINT nFlags, CDPoint ptPos) 
{	
	if(!(nFlags & MK_LBUTTON)) return;

	CDomyun *pDom = m_DomView.GetDomyun();	
	if(ptPos.x < 0) ptPos.x = 0;
	pDom->ClearEtt(FALSE, FALSE);	

	CDPoint ptFindPos;	
	BOOL bFind = FALSE;
	
	CPtrList *pList = pDom->GetObjectList(ETT_POLYLINE);	
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		CObPolyLine *pPoly = (CObPolyLine *)pList->GetNext(pos);
	
		if(m_nType+1 != pPoly->m_nColor) continue;
		if(ptPos.x > pPoly->m_vLineArr[pPoly->m_vLineArr.GetUpperBound()].x)
			ptPos.x = pPoly->m_vLineArr[pPoly->m_vLineArr.GetUpperBound()].x;
		long n=0; for(n=0; n<pPoly->m_vLineArr.GetSize()-1; n++)
		{			
			CDPoint ptStt = pPoly->m_vLineArr[n];
			CDPoint ptEnd = pPoly->m_vLineArr[n+1];
			CDPoint vVec = (ptEnd - ptStt).Unit();
			CDPoint xyResult;
			if(GetXyMatchSegAndLine(ptStt, ptEnd, ptPos, CVector(0,1), xyResult))
			{	
				bFind = TRUE;
				ptFindPos = xyResult;				
				break;
			}
		}
	}

	if(bFind)
	{
		pDom->SetLineColor(7);
		pDom->Circle(ptFindPos.x,0,30,TRUE);
		pDom->Circle(ptFindPos,30,TRUE);
		pDom->SetLineColor(6);			
		pDom->LineTo(ptFindPos.x, 0, ptFindPos.x, 2000);				
		pDom->LineTo(0, ptFindPos.y, 3000, ptFindPos.y);				
		m_DomView.RedrawAll();	

		m_szFact.Format("%.3f", 1 + ptFindPos.y/10000);
		m_dLB = Round(1 + ptFindPos.x / 1000,3);
		UpdateData(FALSE);
	}	
}

void CBaseDesignFactDlg::OnButtonRedraw() 
{
	UpdateData();	
	OnLButtonDownDom(MK_LBUTTON,CDPoint((m_dLB-1)*1000, atof(m_szFact)*1000));
}

void CBaseDesignFactDlg::OnButtonRecommand() 
{
	m_dLB = m_pFooting->GetLB();
	UpdateData(FALSE);	
	OnLButtonDownDom(MK_LBUTTON,CDPoint((m_dLB-1)*1000, atof(m_szFact)*1000));
}
