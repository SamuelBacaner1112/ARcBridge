// BaseFactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "BaseFactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseFactDlg dialog


CBaseFactDlg::CBaseFactDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBaseFactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseFactDlg)
	m_szFact = _T("");
	m_szTitle = _T("");
	m_dRD = 0.0;
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_dRD = 0;
	m_pFooting = NULL;
}

CBaseFactDlg::CBaseFactDlg(CFootingApp *pFooting, CWnd* pParent /*=NULL*/)
	: CUserDialog(CBaseFactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseFactDlg)
	m_szFact = _T("");
	m_szTitle = _T("");
	m_dRD = 0.0;
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_dRD = 0;
	m_pFooting = pFooting;
}

void CBaseFactDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseFactDlg)
	DDX_Text(pDX, IDC_EDIT_FACTVALUE, m_szFact);
	DDX_Text(pDX, IDC_STATIC_FACTTITLE, m_szTitle);
	DDX_Text(pDX, IDC_EDIT_RD, m_dRD);
	DDV_MinMaxDouble(pDX, m_dRD, 0., 2.5);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
}


BEGIN_MESSAGE_MAP(CBaseFactDlg, CDialog)
	//{{AFX_MSG_MAP(CBaseFactDlg)
	ON_WM_LBUTTONDOWN()	
	ON_BN_CLICKED(IDC_BUTTON_REDRAW, OnButtonRedraw)
	ON_BN_CLICKED(ID_BUTTON_RECOMMAND, OnButtonRecommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseFactDlg message handlers

BOOL CBaseFactDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(!m_pFooting) 
	{
		OnCancel();
	}

	if(m_dRD==0)
	{
		m_dRD = m_pFooting->GetRD(m_nType);
		UpdateData(FALSE);
	}

	m_szFact = m_ReturnValue;
	switch(m_nType)
	{
	case 0:	m_szTitle = "수직변위에 대한 근입계수 :";	break;
	case 1:	m_szTitle = "수평변위에 대한 근입계수 :";	break;
	case 2:	m_szTitle = "비틀림에 대한 근입계수 :";	break;
	case 3:	m_szTitle = "회전에 대한 근입계수(X축) :";	break;
	case 4:	m_szTitle = "회전에 대한 근입계수(Y축) :";	break;
	}
	UpdateData(FALSE);
	CString szDxfFile = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory") + "\\DXF";
	m_DomView.GetDomyun()->ClearEtt(TRUE, FALSE);
	m_DomView.GetDomyun()->FixEtt(FALSE);
	m_DomView.GetDomyun()->DxfIn(szDxfFile + "\\Table1.dxf");
	m_DomView.GetDomyun()->FixEtt(TRUE);
	m_DomView.ZoomAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBaseFactDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

void CBaseFactDlg::OnLButtonDownDom(UINT nFlags, CDPoint ptPos) 
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
		long nType = m_nType==4 ? m_nType-1 : m_nType;
		if(nType+1 != pPoly->m_nColor) continue;
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
		pDom->LineTo(ptFindPos.x, 0, ptFindPos.x, 4000);				
		pDom->LineTo(0, ptFindPos.y, 2500, ptFindPos.y);					
		m_DomView.RedrawAll();	

		if(m_nType==0 || m_nType==1)
			m_szFact.Format("%.3f", 1 + ptFindPos.y/2000);
		else		
			m_szFact.Format("%.3f", 1 + ptFindPos.y/500);
		m_dRD = Round(ptFindPos.x / 1000,3);
		UpdateData(FALSE);
	}	
}

void CBaseFactDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CBaseFactDlg::OnButtonRedraw() 
{
	UpdateData();
	OnLButtonDownDom(MK_LBUTTON,CDPoint(m_dRD*1000, atof(m_szFact)*1000));
}

void CBaseFactDlg::OnOK() 
{
	UpdateData();
	m_ReturnValue = m_szFact;
	
	CDialog::OnOK();
}

void CBaseFactDlg::OnButtonRecommand() 
{
	m_dRD = m_pFooting->GetRD(m_nType);
	UpdateData(FALSE);
	OnLButtonDownDom(MK_LBUTTON,CDPoint(m_dRD*1000, atof(m_szFact)*1000));
}
