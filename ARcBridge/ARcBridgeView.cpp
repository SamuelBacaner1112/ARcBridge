// ARcBridgeView.cpp : implementation of the CARcBridgeView class
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "DlgOcc3D.h"

#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "WmfSizeDlg.h"
#include "../Src/ARcBridgeData/ARcBridgeData.h"
#include "../Src/ARcBridgeInput/ARcBridgeInput.h"
#include "../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../Src/ARcBridgeDrawing/ARcBridgeDrawing.h"
#include "../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "../Src/ARcBridgeOutDesign/ARcBridgeOutDesign.h"
#include "../Src/ARcBridgeOutBM/ARcBridgeOutBM.h"

#include "ExportToAStrutTie.h"
#include "DialogExportStxOption.h"

#include "spromeps.h" // RainBow Lock Driver

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AUTOSAVE_TIMER 300
/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView

IMPLEMENT_DYNCREATE(CARcBridgeView, CDomyunGLView)

BEGIN_MESSAGE_MAP(CARcBridgeView, CDomyunGLView)
	//{{AFX_MSG_MAP(CARcBridgeView)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_ZOOM_DYNAMIC, OnZoomDynamic)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_PREV, OnZoomPrev)
	ON_COMMAND(ID_ZOOM_WINDOW, OnZoomWindow)
	ON_COMMAND(ID_REALTIME_ZOOM, OnRealtimeZoom)
	ON_COMMAND(ID_REALTIME_MOVE, OnRealtimeMove)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_DRAWING_OPTION, OnDrawingOption)
	ON_COMMAND(ID_DRAWING_EXPORT, OnDrawingExport)
	ON_COMMAND(ID_STRUCT_OUT, OnStructOut)
	ON_COMMAND(ID_GEN_OUT, OnGenOut)
	ON_COMMAND(ID_COMMON_OUT, OnCommonOut)
	ON_COMMAND(ID_EARTH_OUT, OnEarthOut)
	ON_COMMAND(ID_CURRENT_PAGE, OnDrawCurrentPage)
	ON_COMMAND(ID_PREV_PAGE, OnDrawPrevPage)
	ON_COMMAND(ID_NEXT_PAGE, OnDrawNextPage)
	ON_COMMAND(ID_PREV_CATEGORY, OnDrawPrevCategory)
	ON_COMMAND(ID_NEXT_CATEGORY, OnDrawNextCategory)
	ON_COMMAND(ID_FIRST_CATEGORY, OnDrawFirstCategory)
	ON_COMMAND(ID_LAST_CATEGORY, OnDrawLastCategory)
	ON_COMMAND(ID_INIT_DRAWING, OnDrawPageInit)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SELECT_OBJECT, OnSelectObject)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MEASURE_DIST, OnMeasureDist)
	ON_COMMAND(ID_OBJECT_INFO, OnObjectInfo)
	ON_COMMAND(ID_SET_LAYER, OnSetLayer)
	ON_COMMAND(ID_SET_LINE, OnSetLine)
	ON_COMMAND(ID_SET_FONT, OnSetFont)
	ON_COMMAND(ID_SET_DIM, OnSetDim)
	ON_COMMAND(ID_CALC_STATION, OnCalcStation)
	ON_COMMAND(ID_CALC_LINEINFO, OnCalcLineInfo)
	ON_COMMAND(ID_SNAP_END, OnSnapEnd)
	ON_UPDATE_COMMAND_UI(ID_SNAP_END, OnUpdateSnapEnd)
	ON_COMMAND(ID_SNAP_MIDDLE, OnSnapMiddle)
	ON_UPDATE_COMMAND_UI(ID_SNAP_MIDDLE, OnUpdateSnapMiddle)
	ON_COMMAND(ID_SNAP_INTERSECTION, OnSnapIntersection)
	ON_UPDATE_COMMAND_UI(ID_SNAP_INTERSECTION, OnUpdateSnapIntersection)
	ON_COMMAND(ID_SNAP_CENTER, OnSnapCenter)
	ON_UPDATE_COMMAND_UI(ID_SNAP_CENTER, OnUpdateSnapCenter)
	ON_COMMAND(ID_SNAP_PERPEN, OnSnapPerpen)
	ON_UPDATE_COMMAND_UI(ID_SNAP_PERPEN, OnUpdateSnapPerpen)
	ON_COMMAND(ID_SNAP_NODE, OnSnapNode)
	ON_UPDATE_COMMAND_UI(ID_SNAP_NODE, OnUpdateSnapNode)
	ON_COMMAND(ID_SNAP_NEAR, OnSnapNear)
	ON_UPDATE_COMMAND_UI(ID_SNAP_NEAR, OnUpdateSnapNear)
	ON_COMMAND(ID_ALIGN_LEFT, OnAlignLeft)
	ON_COMMAND(ID_ALIGN_CENTER_HOR, OnAlignCenterHor)
	ON_COMMAND(ID_ALIGN_RIGHT, OnAlignRight)
	ON_COMMAND(ID_ALIGN_TOP, OnAlignTop)
	ON_COMMAND(ID_ALIGN_CENTER_VER, OnAlignCenterVer)
	ON_COMMAND(ID_ALIGN_BOTTOM, OnAlignBottom)
	ON_COMMAND(ID_ALIGN_SPACE_ACROSS, OnAlignSpaceAcross)
	ON_COMMAND(ID_ALIGN_SPACE_DOWN, OnAlignSpaceDown)
	ON_COMMAND(ID_ALIGN_ATTACH_HOR, OnAlignAttachHor)
	ON_COMMAND(ID_ALIGN_ATTACH_VER, OnAlignAttachVer)
	ON_COMMAND(ID_OBJECT_ALIGN, OnObjectAlign)
	ON_COMMAND(ID_ADD_NOTE, OnAddNote)
	ON_UPDATE_COMMAND_UI(ID_ADD_NOTE, OnUpdateAddNote)
	ON_WM_CHAR()
	ON_UPDATE_COMMAND_UI(ID_CALC_STATION, OnUpdateCalcStation)
	ON_UPDATE_COMMAND_UI(ID_CALC_LINEINFO, OnUpdateCalcLineInfo)
	ON_COMMAND(ID_EXPORT_LINE, OnExportLine)
	ON_COMMAND(ID_CATEGORY_ORDER, OnCategoryOrder)
	ON_COMMAND(ID_ALIGN_UNDO, OnAlignUndo)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_UNDO, OnUpdateAlignUndo)
	ON_COMMAND(ID_ALIGN_REDO, OnAlignRedo)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_REDO, OnUpdateAlignRedo)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_BLOCK_CUT, OnBlockCut)
	ON_COMMAND(ID_BLOCK_PASTE, OnBlockPaste)
	ON_COMMAND(ID_BLOCK_CANCEL, OnBlockCancel)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_CANCEL, OnUpdateBlockCancel)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_PASTE, OnUpdateBlockPaste)
	ON_COMMAND(ID_BLOCK_COPY, OnBlockCopy)
	ON_COMMAND(ID_BLOCK_INIT_CUR, OnBlockInitCur)
	ON_COMMAND(ID_BLOCK_INIT_ALL, OnBlockInitAll)
	ON_COMMAND(ID_EXPORT_BRIDGEINFO, OnExportBridgeinfo)
	ON_COMMAND(ID_TOOL_HGEXPORT, OnToolHgexport)
	ON_COMMAND(ID_INPUT_END, OnDrawCurrentPage)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HGEXPORT, OnUpdateToolHgexport)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_DRAW_CHANGE_PAGE, OnDrawChangePage)
	ON_COMMAND(ID_FILE_PRINT, CDomyunGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CDomyunGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDomyunGLView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EXPORT_DXF_CUR, ID_ORDER_APPLYSAME, OnUpdateAll)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SELECT_OBJECT, ID_OBJECT_ALIGN, OnUpdateAlign)			
	ON_COMMAND(ID_INPUT_END, OnDrawCurrentPage)
	ON_XTP_EXECUTE_RANGE(IDC_TEDIT_PAGE, IDC_TCOMBO_CATEGORY, OnToolbarControl)
	ON_COMMAND_RANGE(ID_ALIGN_UNDO_DUMMY, ID_ALIGN_UNDO_DUMMY+50, OnAlignUndoDummy)
	ON_COMMAND_RANGE(ID_ALIGN_REDO_DUMMY, ID_ALIGN_REDO_DUMMY+50, OnAlignRedoDummy)		
	ON_MESSAGE(WM_USER_APPLYDOMYUN, ApplyDom)
	ON_COMMAND(ID_EXPORT_ASTRUTTIE, &CARcBridgeView::OnExportAStrutTie)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_ASTRUTTIE, &CARcBridgeView::OnUpdateExportAStrutTie)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView construction/destruction
UINT CARcBridgeView::m_nIDEventLock = 0;
CARcBridgeView::CARcBridgeView() : CDomyunGLView(FALSE)
{
	m_bUCSIcon = FALSE;
	m_bModeCursor = FALSE;
	m_bMoveCursor = FALSE;
	m_bTransZoomMove = FALSE;
	m_bEnableCrossCursor = FALSE;
	m_bUseToolTip = TRUE;
	m_bViewVectorInfo	= FALSE;	// tvArr그릴때 정보를 그릴지?
	m_nLeft				= 0;
	m_pEdit	= NULL;
	m_ArrCur = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_ActiveSizeCurUD = AfxGetApp()->LoadCursor(IDC_ACTIVESIZE_UD);
	m_ActiveSizeCurLR = AfxGetApp()->LoadCursor(IDC_ACTIVESIZE_LR);
	m_ActiveSizeCurUDLR = AfxGetApp()->LoadCursor(IDC_ACTIVESIZE_UDLR);
	
	m_bPush	= FALSE;
	m_bPainted = FALSE;	

//	m_pDlgOccView	= NULL;
//	m_pDlgOccViewRendering = NULL;
}

CARcBridgeView::~CARcBridgeView()
{
	if(m_pEdit)
		delete m_pEdit;

	//if(m_pDlgOccView)
	//	delete m_pDlgOccView;
	//	m_pDlgOccView = NULL;

	//if(m_pDlgOccViewRendering)
	//	delete m_pDlgOccViewRendering;
	//	m_pDlgOccViewRendering = NULL;

	CARcBridgeDoc* pDoc = GetDocument();
	if(pDoc)
		pDoc->m_pARcBridgeInputStd->m_FemDraw.ClearData();
}

BOOL CARcBridgeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.style &= ~WS_BORDER;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView drawing

void CARcBridgeView::OnDraw(CDC* pDC)
{
	if(IsViewOutBM())	return;
	if(IsView3D()) return;

	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CARcBridgeDoc* pDoc = GetDocument();

	// 도면 위치일 경우에 GL 모드 무조건 제거 - 쉰
//-	if(pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD()==ID_INPUT_END && IsGL()) 
//-		SetGL(FALSE);

	ASSERT_VALID(pDoc);
	// 
	if(!IsGL()) 
	{
		CDomyunGLView::OnDraw(pDC);
	}
	else
	{		
		if(!IsOwnerMouseMode())
			SetOwnerMouseMode(TRUE);
			
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);

		SetRanderContextState(TRUE);

		pDoc->m_pARcBridgeInputStd->m_FemDraw.Render(pDC);	
				
		SwapBuffers(m_hDC);				
		glFinish();
		SetRanderContextState(FALSE);		
		
		if(m_bOwnerMouse && /*!GetDocument()->m_bMoveCursor &&*/ GetFocus()==this && !m_bPrint)
		{
			DrawMouseLine(TRUE);
			DrawMouseRect(TRUE);
		}
	}		


}

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView printing

BOOL CARcBridgeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CARcBridgeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CARcBridgeDoc * pDoc = GetDocument();
	pMFrm->m_bPrintPreview	= TRUE;

	SetLineThickStruct(pDoc->m_pARoadOptionStd->m_LineThick);	
	CDomyunGLView::OnBeginPrinting(pDC, pInfo);
}

void CARcBridgeView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{	
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	pMFrm->m_bPrintPreview	= FALSE;
	CDomyunGLView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void CARcBridgeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CDomyunGLView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView diagnostics

#ifdef _DEBUG
void CARcBridgeView::AssertValid() const
{
	CDomyunGLView::AssertValid();
}

void CARcBridgeView::Dump(CDumpContext& dc) const
{
	CDomyunGLView::Dump(dc);
}

CARcBridgeDoc* CARcBridgeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CARcBridgeDoc)));
	return (CARcBridgeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeView message handlers

void CARcBridgeView::OnInitialUpdate() 
{
	CDomyunGLView::OnInitialUpdate();

	SetDomyun(m_pBasicDom);

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	CXTPMenuBar* pMenuBar = pFrm->m_pCommandBars->GetMenuBar();

	pIn->m_FemDraw.SetGLView(this);
	pIn->InsertInputMenu(pMenuBar);
	pFrm->m_pView = this;

	SetOwnerMouseMode(TRUE);

	////////////////////////////////////////////////////////////////////////////	
	// 제일 처음 실행일 경우 설정창을 보여줌
	CEnvRegist env;
	if(!env.LoadEnvInt(CEnvRegist::SET_PRGOPT))
		pFrm->SendMessage(WM_COMMAND, ID_SET_CREATION);

	SetAutoSave(env.LoadEnvInt(CEnvRegist::AUTO_SAVE));
	SetCrossAngle(env.LoadEnvDouble(CEnvRegist::CROSSCURSOR_ANGLE));	
	CRect rect;
	long nLen = 0;
	GetClientRect(&rect);
	double dLenRatio = env.LoadEnvDouble(CEnvRegist::CROSSCURSOR_LENGTH);
	if(dLenRatio==0)
		nLen = 30;
	else
	{
		if(dLenRatio!=100)
			nLen = (long)(rect.Width()/2*dLenRatio/100);
	}
	SetCrossLength(nLen);

	CString strPath = env.LoadEnvStr(CEnvRegist::FONT_DIR) + _T("\\");	
	if(strPath==_T("\\"))
		strPath = env.LoadEnvStr(CEnvRegist::PRG_DIR) + _T("\\FONTS\\");
	SetFontDir(strPath);
	SetUseShx(env.LoadEnvInt(CEnvRegist::USE_SHXFONT));		
	m_bUseToolTip = env.LoadEnvInt(CEnvRegist::USE_MOVETOOLTIP);
	COutMsgDlg::m_bStatsMessage = TRUE;
	////////////////////////////////////////////////////////////////////////////

	pDoc->m_pARcBridgeDrawingStd->m_pWnd = this;
	pDoc->m_pARcBridgeOutDesignStd->m_pView = this;
	pDoc->m_pARcBridgeOutBMStd->m_pView = this;
	pDoc->m_pARcBridgeDrawingStd->SetDomyun(this);
	pDoc->m_pShareWithADManagerStd->m_pView	= this;

	// 도면 정보 초기화
	CARcBridgeDrawingOptionStd* pStd = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDBStandard::SetUseHCadBlock(!(pStd->m_dwDrawCommon & COMMON_NOHCAD));
	CHgBaseDrawStd::SetUseHCadBlock(!(pStd->m_dwDrawCommon & COMMON_NOHCAD));	
	////////////////////////////////////////////////////////////////////////////
	// 도각정보 재조정
	pDoc->m_pARcBridgeDrawingStd->SetInitDogak(pStd->m_strDgkName, pStd->m_strDomNumHead);
	CDrawDogak *pDgk = pDoc->m_pARcBridgeDrawingStd->GetDogak();
	if(pDgk->m_szAttdefText.GetSize()!=pStd->m_strDgkAttrPrompt.GetSize())
	{
		pStd->m_strDgkAttrPrompt.RemoveAll();
		pStd->m_strDgkAttrText.RemoveAll();
	}
	if(pDgk->m_bHCadDgk && pStd->m_strDgkAttrPrompt.GetSize()!=0)
	{
		for(long j=0; j<pStd->m_strDgkAttrPrompt.GetSize(); j++)
			pDgk->m_szAttdefText.SetAt(j, pStd->m_strDgkAttrText[j]);
	}

	GetDomyun()->ClearEtt(TRUE);

	
	ZoomAll();
	if(IsGL())	ZoomPlusNegaExt(FALSE,0.3);		
	
	if(pFrm->m_wndShortcutBar.GetSafeHwnd()==0)
		pFrm->CreateShortcutBar();
	pFrm->m_paneShortcutStart.InitMRUList();
	
	if(pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD())
	{
		if(pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD()!=ID_INPUT_END)
			pDoc->m_pARcBridgeInputStd->GoCurrentDlg();	 				
		else		
			OnDrawCurrentPage();		
	}		

	pFrm->SendMessage(WM_UPDATE_STATUSBAR);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	pFrm->OnUnitComboChange();
	pDoc->SetModifiedFlag(FALSE);		
	Sleep(0);

	SetRanderContextState(TRUE);
	GetDocument()->m_pARcBridgeInputStd->m_FemDraw.InitializeOpenGL(m_hDC);
	SetRanderContextState(FALSE);
}

void CARcBridgeView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;

	CXTPDockingPane* pPane = pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);

	CString str;
	CDPoint xy = ClientToExt(point);

	str.Format("%14.6lf, %14.6lf",xy.y,xy.x);
	pFrm->m_wndStatusBar.SetPaneText(0, str);
	
	pDraw->OnMouseMove(nFlags, point);
	if(m_bMoveCursor || pDraw->IsInsertNoteMode())
		CView::OnMouseMove(nFlags, point);
	else
		CDomyunGLView::OnMouseMove(nFlags, point);



	CDrawTool* pTool = FindTool(c_drawShape);	
	CInputDialogBase* pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
	if(pDlg && pTool->m_drawShape==CDrawTool::normal)
		SetTimer(77, 10, NULL);

	if(pDoc && pPane->GetTitle()==_T("입력") && pTool->m_drawShape == CDrawTool::normal)
	{
		if(pDoc->m_pARcBridgeInputStd)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pARcBridgeInputStd->m_strME.xy = xy;
			pDoc->m_pARcBridgeInputStd->m_strME.bViewMouseEvent = TRUE;
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_MOUSEMOVE);			
		}
	}
	if(IsGL() && m_ViewMode == NORMAL && !pDoc->m_pARcBridgeInputStd->m_FemDraw.GetFlag(FEMVIEW_LOADCASE) && (nFlags & MK_LBUTTON))
	{
		SetRanderContextState(TRUE);
		if(pDoc->m_pARcBridgeInputStd->m_FemDraw.MousePickingForce(point))
			Invalidate(FALSE);
		SetRanderContextState(FALSE);
	}
}

void CARcBridgeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bModeCursor)				
		GetDocument()->m_pARcBridgeDrawingStd->OnRButtonDown(nFlags, point);
	
	CDomyunGLView::OnRButtonDown(nFlags, point);
}

void CARcBridgeView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bTransZoomMove = m_bContextMenu;
	
	CDomyunGLView::OnRButtonUp(nFlags, point);
}

void CARcBridgeView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(!m_bTransZoomMove) return;
	CDrawTool* pTool = FindTool(c_drawShape);	
	if(pTool->m_drawShape != CDrawTool::normal) return;
	
	if(TransZoomMove()) return;	

	CMenu menuZoom, menuOrder, *pContextMenuZoom, *pContextMenuOrder;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();


	menuZoom.LoadMenu(IDR_POPUP_ZOOM);
	pContextMenuZoom = menuZoom.GetSubMenu(0);

	menuOrder.LoadMenu(IDR_POPUP_ORDER);
	pContextMenuOrder = menuOrder.GetSubMenu(0);

#ifdef _DEBUG
#else
	pContextMenuZoom->DeleteMenu(ID_BLOCK_PASTE, MF_BYCOMMAND);
	pContextMenuZoom->DeleteMenu(ID_BLOCK_CANCEL, MF_BYCOMMAND);
	pContextMenuZoom->DeleteMenu(ID_BLOCK_INIT_CUR, MF_BYCOMMAND);
	pContextMenuZoom->DeleteMenu(ID_BLOCK_INIT_ALL, MF_BYCOMMAND);

	pContextMenuOrder->DeleteMenu(ID_BLOCK_CUT, MF_BYCOMMAND);
	pContextMenuOrder->DeleteMenu(ID_BLOCK_COPY, MF_BYCOMMAND);
#endif

	if(m_bModeCursor)	
		pFrm->m_pCommandBars->TrackPopupMenu(pContextMenuOrder, TPM_LEFTALIGN,  point.x, point.y, this);
	else
		pFrm->m_pCommandBars->TrackPopupMenu(pContextMenuZoom, TPM_LEFTALIGN,  point.x, point.y, this);
}

void CARcBridgeView::OnZoomAll() 
{
	ZoomAll(); 
	if(IsGL())	ZoomPlusNegaExt(FALSE,0.3);	
}

void CARcBridgeView::OnZoomDynamic() 
{
	SetZoomRect(3);
}

void CARcBridgeView::OnZoomIn() 
{	
	ZoomPlusNegaExt(TRUE);
}

void CARcBridgeView::OnZoomOut() 
{	
	ZoomPlusNegaExt(FALSE);
}

void CARcBridgeView::OnZoomPrev() 
{
	ZoomPrew();
}

void CARcBridgeView::OnZoomWindow() 
{
	SetZoomRect(1);
}

void CARcBridgeView::OnRealtimeZoom() 
{
	SetZoomRect(4);
}

void CARcBridgeView::OnRealtimeMove() 
{
	SetZoomRect(2);
}

void CARcBridgeView::OnExportLine() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CLineInfoApp *pLineApp = pDoc->m_pARcBridgeDataStd->GetLineApp(); 
	CString strName = pDoc->GetTitle();
	if(strName.Find('.')>=0)
	{
		int pos = strName.Find('.');
		strName = strName.Left(pos);
	}

	CFileDialog dlg(FALSE, _T("*.lin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("ALine 파일 (*.lin)|*.lin||"), this);
	dlg.m_ofn.lpstrTitle = _T("선형 파일 내보내기");

	if(dlg.DoModal()==IDOK)
	{
		CFile file;
		if(file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&file, CArchive::store);
		pLineApp->Serialize(ar);
		ar.Close();
		file.Close();

		// 파일로 저장한 뒤에 ALine을 바로 실행 시켜서 불러온다. //////////////////////
		CString szLineExe = ((CARcBridgeApp*)AfxGetApp())->GetFixedDirectory() + "\\ALine.exe " + "\"" + dlg.GetPathName() + "\"";				

		PROCESS_INFORMATION pi;
		// Set up the start up info struct.
		STARTUPINFO si;
		::ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;		
		// Use this if you want to show the child.
		si.wShowWindow = SW_SHOW;
		BOOL bResult = ::CreateProcess(NULL, szLineExe.GetBuffer(0), NULL, NULL, TRUE,
						CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if(bResult && pi.hProcess)
		{		
			m_WaitForALineDlg.m_bCancel = FALSE;
			if(!m_WaitForALineDlg.GetSafeHwnd()) m_WaitForALineDlg.Create(CWaitForSubProgram::IDD);
			m_WaitForALineDlg.CenterWindow();
			m_WaitForALineDlg.ShowWindow(SW_SHOW);

			DWORD dwExitCode;
			::GetExitCodeProcess(pi.hProcess, &dwExitCode);
			while(dwExitCode == STILL_ACTIVE)
			{
				MSG msg;	
				while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
				{
					AfxGetThread()->PumpMessage();							
				}	
				::GetExitCodeProcess(pi.hProcess, &dwExitCode);
				if(m_WaitForALineDlg.m_bCancel) break;
			}
			if(!m_WaitForALineDlg.m_bCancel) 
			{
				m_WaitForALineDlg.DestroyWindow();
				CString szMsg;
				szMsg.Format("ALine에서 수정된 %s 파일을 현재 파일에 반영하시겠습니까 ?", dlg.GetPathName());
				if(AfxMessageBox(szMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					CFile f;
					if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;					
					CArchive ar(&f,CArchive::load);
					pLineApp->Serialize(ar);
					ar.Close();
					f.Close();

					CString csErrorMsg(_T(""));
					if(!pLineApp->SetDataIp(csErrorMsg))
					{
						AfxMessageBox(csErrorMsg,MB_OK | MB_ICONEXCLAMATION);
					}
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////////
	}
}

void CARcBridgeView::SetAutoSave(BOOL bStart)
{
	UINT nElapse = 300000;	// 1000 * 60 * 5(5분)
	if(bStart)
		SetTimer(AUTOSAVE_TIMER, nElapse, NULL);
	else
		KillTimer(AUTOSAVE_TIMER);
}

void CARcBridgeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();	
	CXTPDockingPane* pPane = pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);
	if(pPane->GetTitle()!=_T("입력")) return;

	CDrawTool* pTool = FindTool(c_drawShape);	
	CARcBridgeDoc *pDoc = GetDocument();
	if(pDoc)
	{
		if(pDoc->m_pARcBridgeInputStd && pTool->m_drawShape == CDrawTool::normal)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pARcBridgeInputStd->m_strME.xy = xy;
			pDoc->m_pARcBridgeInputStd->m_strME.bViewMouseEvent = TRUE;
			CInputDialogBase* pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_LBUTTONDBLCLK);			
		}
	}
	CDomyunGLView::OnLButtonDblClk(nFlags, point);
}

void CARcBridgeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();	

	CXTPDockingPane* pPane = pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);

	if(pDraw->IsInsertNoteMode() && pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD()==ID_INPUT_END)
		pDraw->OnLButtonDown(nFlags, point);		
	if(m_bMoveCursor && GetDrawShape()==CDrawTool::normal)
		pDraw->OnLButtonDown(nFlags, point);
	
	if(m_bMoveCursor && m_bUseToolTip) m_wndToolTip.Activate(FALSE);

		// 화면 클릭시 자동연결	
	CDrawTool* pTool = FindTool(c_drawShape);	
	CInputDialogBase *pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();

	if(pDoc && pPane->GetTitle()==_T("입력"))
	{
		if(pDoc->m_pARcBridgeInputStd && pTool->m_drawShape == CDrawTool::normal)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pARcBridgeInputStd->m_strME.xy = xy;
			pDoc->m_pARcBridgeInputStd->m_strME.bViewMouseEvent = TRUE;
			CInputDialogBase* pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_LBUTTONDOWN);
		}
	}
	if(IsGL() && m_ViewMode == NORMAL && !pDoc->m_pARcBridgeInputStd->m_FemDraw.GetFlag(FEMVIEW_LOADCASE) && (nFlags & MK_LBUTTON))
	{
		SetRanderContextState(TRUE);
		if(pDoc->m_pARcBridgeInputStd->m_FemDraw.MousePickingForce(point))
			Invalidate(FALSE);
		SetRanderContextState(FALSE);
	}

	if(pDlg && pTool->m_drawShape == CDrawTool::normal)
	{
		POINT pt;
		GetCursorPos(&pt);						
		ScreenToClient(&pt);			
		CRect rect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);	
		m_bStopFind = FALSE;
		CObArray obArr;
		if(Intersects(&obArr,rect,INTER_TEXT))
		{
			CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);
			for(long i = 0; i < obArr.GetSize(); i++)
			{
				pSelObj	= (CDrawObject*)obArr.GetAt(i);
				if(pSelObj->GetEttKind() == ETT_TEXT)
					break;
			}
			if(pSelObj->GetEttKind()==ETT_TEXT)
			{						
				CObText *pText = (CObText *)pSelObj;	
				CString szText = pText->m_TextString;
				if(pDlg->IsFocusItem(szText))// && !m_bHasFocus)					
				{					
					CDC* pDC = GetDC();
					CDRect rectText = GetDomyun()->GetTextRect(pText, szText);						
					
					rectText.left += pText->m_SttPoint.x;
					rectText.top  += pText->m_SttPoint.y;
					rectText.right += pText->m_SttPoint.x;
					rectText.bottom += pText->m_SttPoint.y;

					CRect rect = ExtToClient(rectText);	
					rect.NormalizeRect();
					rect.DeflateRect(-2,-2);

					DrawMouseLine(FALSE);
					DrawMouseRect(FALSE);
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));												
					rect.DeflateRect (1, 1);
					pDC->Draw3dRect (rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_3DLIGHT));
					// 마우스 그림
					if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
					{	
						DrawMouseLine(TRUE);
						DrawMouseRect(TRUE);		
					}		
					ReleaseDC(pDC);					
					m_bPush = TRUE;
				}			
			}
		}		
	}	
	CDomyunGLView::OnLButtonDown(nFlags, point);
}

void CARcBridgeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();	
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CXTPDockingPane* pPane = pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);

	if(m_bMoveCursor)
	{
		pDraw->OnLButtonUp(nFlags, point);	
		OnSetCursor(this,0,0);
	}

	// 화면 클릭시 자동연결	
	CDrawTool* pTool = FindTool(c_drawShape);	
	CInputDialogBase *pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
	if(m_bPush && pDlg && pTool->m_drawShape == CDrawTool::normal)
	{
		POINT pt;
		GetCursorPos(&pt);						
		ScreenToClient(&pt);			
		CRect rect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);	
		m_bStopFind = FALSE;
		CObArray obArr;
		if(Intersects(&obArr,rect,INTER_TEXT))
		{
			CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);
			for(long i = 0; i < obArr.GetSize(); i++)
			{
				pSelObj	= (CDrawObject*)obArr.GetAt(i);
				if(pSelObj->GetEttKind() == ETT_TEXT)
					break;
			}
			if(pSelObj->GetEttKind()==ETT_TEXT)
			{				
				CObText *pText = (CObText *)pSelObj;					
				CString szText = pText->m_TextString;
				if(pDlg->IsFocusItem(szText))// && !m_bHasFocus)					
				{					
					CDC* pDC = GetDC();
					CDRect rectText = GetDomyun()->GetTextRect(pText, szText);						
					
					rectText.left += pText->m_SttPoint.x;
					rectText.top  += pText->m_SttPoint.y;
					rectText.right += pText->m_SttPoint.x;
					rectText.bottom += pText->m_SttPoint.y;

					CRect rect = ExtToClient(rectText);	
					rect.NormalizeRect();
					rect.DeflateRect(-2,-2);

					DrawMouseLine(FALSE);
					DrawMouseRect(FALSE);
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));												
					rect.DeflateRect (1, 1);
					pDC->Draw3dRect (rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
					// 마우스 그림
					if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
					{	
						DrawMouseLine(TRUE);
						DrawMouseRect(TRUE);		
					}		
					pDlg->SetFocusItem(szText);
					ReleaseDC(pDC);						
				}			
			}
		}			
		m_bPush = FALSE;
	}	

	if(pDoc && pPane->GetTitle()==_T("입력"))
	{
		if(pDoc->m_pARcBridgeInputStd && pTool->m_drawShape == CDrawTool::normal)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pARcBridgeInputStd->m_strME.xy = xy;
			pDoc->m_pARcBridgeInputStd->m_strME.bViewMouseEvent = TRUE;
			CInputDialogBase* pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_LBUTTONUP);			
		}
	}
	
	CDomyunGLView::OnLButtonUp(nFlags, point);
}

void CARcBridgeView::OnSize(UINT nType, int cx, int cy) 
{
	CDomyunGLView::OnSize(nType, cx, cy);

	ZoomAll();
	if(IsGL())	ZoomPlusNegaExt(FALSE,0.3);		

	CARcBridgeDoc *pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd)
	{
		if(pBMStd->m_pOutBMDialog)
		{
			if(pBMStd->m_pOutBMDialog->GetSafeHwnd() == 0)
				pBMStd->m_pOutBMDialog->Create(IDD_DIALOG_OUTBM, this);

			CXTPClientRect rc(this);
			pBMStd->m_pOutBMDialog->MoveWindow(rc);
		}
	}

	/*
	if(m_pDlgOccView)
	{
		CRect rect;
		GetClientRect(&rect);
		m_pDlgOccView->MoveWindow(rect);
	}
	*/
}

void CARcBridgeView::OnDrawingOption() 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	COptionSheet OptDlg(_T("ARcBridge 도면 환경 설정"), this, pOpt->m_nCurPropPage, pStd->m_nTypeProcess == TYPE_PROCESS_SLAB_STANDARD_OUTPUT);
	OptDlg.m_pDoc = pDoc;

	if(OptDlg.DoModal()==IDOK)
	{
		OnDrawCurrentPage();
	}
}

void CARcBridgeView::OnDrawingExport() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CString strDir = ((CARcBridgeApp*)AfxGetApp())->GetFixedDirectory();
	pDraw->ShowShopDxfDlg(strDir);
}

void CARcBridgeView::OnStructOut() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	COutDesignDlg *pDlg = (COutDesignDlg*)pDoc->m_pARcBridgeOutDesignStd->m_pDlg;

	long nCurBri = pDoc->m_pARcBridgeInputStd->m_nCurrentBridge;

	if(pDlg)
	{
		pDoc->m_pARcBridgeOutDesignStd->m_nCurBridge = nCurBri;
		pDlg->DoModal();
	}
}

// 일반수량 출력
void CARcBridgeView::OnGenOut() 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL)
		return;

	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(3);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	CDialogMenuOutBM *pDlg	= &pFrm->m_paneShortcutBM.m_dlgMenuOutBM;
	pDlg->RunOnButtonBmByStruct();

	// 수량 종류 선택
	pDlg->SetSelectBMTab(0);
}

// 공통수량 출력
void CARcBridgeView::OnCommonOut() 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL)
		return;

	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(3);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	CDialogMenuOutBM *pDlg	= &pFrm->m_paneShortcutBM.m_dlgMenuOutBM;
	pDlg->RunOnButtonBmByStruct();

	// 수량 종류 선택
	pDlg->SetSelectBMTab(1);
}

// 토공수량 출력
void CARcBridgeView::OnEarthOut() 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL)
		return;

	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(3);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	CDialogMenuOutBM *pDlg	= &pFrm->m_paneShortcutBM.m_dlgMenuOutBM;
	pDlg->RunOnButtonBmByStruct();

	// 수량 종류 선택
	pDlg->SetSelectBMTab(2);
}

void CARcBridgeView::OnDrawChangePage() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	
	if(!pDoc->m_bFileOpen)
		return;

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
	CDrawPageMng* pDrawMng = pDraw->GetDrawPageMng();
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);
	
	pDrawMng->SetPageInit();
	if(pDraw->m_nCurPage<1)
		pDraw->m_nCurPage = 1;
	if(pDraw->m_nCurPage>pDraw->GetCountTotalPage())
		pDraw->m_nCurPage = pDraw->GetCountTotalPage();

	COutMsgDlg msgDlg(this, pDrawMng);
	
	SetGL(FALSE);
	msgDlg.Run();

	pFrm->SendMessage(WM_UPDATE_DRAWING_TITLE, pDraw->m_nCurPage);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);
}

void CARcBridgeView::OnDrawCurrentPage() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// INSERT_ATESTCODE
	AfxGetMainWnd()->SendMessage(WM_ATT_MAINMSG,eMsgAppSetCAME_IN_DOMYUNPOS);

	if(pDoc->m_pARcBridgeDataStd->GetBridgeSize()==0)
		return;


	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	if(pDraw->m_nCurPage<1)
		pDraw->m_nCurPage = 1;
	if(pDraw->m_nCurPage>pDraw->GetCountTotalPage())
		pDraw->m_nCurPage = pDraw->GetCountTotalPage();

	pDraw->SetModified(pDraw->GetCurPage(), FALSE);	
	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	SetGL(FALSE);
	msgDlg.Run();

	pFrm->SendMessage(WM_UPDATE_DRAWING_TITLE, pDraw->m_nCurPage);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawPrevPage() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);		
	if(pMove)
		pMove->ClearSelect();

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pDraw->m_nCurPage = pDraw->GetOrgPage(nSubPage-2);
	if(pDraw->m_nCurPage>pDraw->GetCountTotalPage())
		pDraw->m_nCurPage = pDraw->GetCountTotalPage();

	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);

	nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawNextPage() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);		
	if(pMove)
		pMove->ClearSelect();

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pDraw->m_nCurPage = pDraw->GetOrgPage(nSubPage);
	if(pDraw->m_nCurPage>pDraw->GetCountTotalPage())
		pDraw->m_nCurPage = pDraw->GetCountTotalPage();

	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);	

	nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawPrevCategory() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);
	
	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	long nBri = pDraw->GetBridgeNumber(nSubPage);
	long nCategory = pDraw->GetCategoryNumber(nSubPage);
	long nCountCate = pDraw->GetCountCategory();
	if(nCategory==0)
	{
		if(nBri>0)
		{
			nBri--;
			nCategory = nCountCate;
		}
		else
			return;
	}

	CString str = pDraw->GetTitleCategory(--nCategory);
	int nCountCategoryPage = pDraw->GetCountCategoryPage(str);
	if(nCountCategoryPage==0)
		str = pDraw->GetTitleCategory(--nCategory);

	nSubPage = pDraw->GetSubPageSttCategory(nBri, str);
	pDraw->m_nCurPage = pDraw->GetOrgPage(nSubPage);
	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawNextCategory() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pData = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	long nBri = pDraw->GetBridgeNumber(nSubPage);
	long nCategory = pDraw->GetCategoryNumber(nSubPage);
	long nCountBri = pData->GetBridgeSize();
	long nCountCate = pDraw->GetCountCategory();
	if(nCategory==nCountCate-1)
	{
		if(nBri<nCountBri-1)
		{
			nBri++;
			nCategory = -1;
		}
		else
			return;
	}

	CString str = pDraw->GetTitleCategory(++nCategory);
	int nCountCategoryPage = pDraw->GetCountCategoryPage(str);
	if(nCountCategoryPage==0)
		str = pDraw->GetTitleCategory(++nCategory);

	nSubPage = pDraw->GetSubPageSttCategory(nBri, str);
	pDraw->m_nCurPage = pDraw->GetOrgPage(nSubPage);
	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawFirstCategory() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	pDraw->m_nCurPage = pDraw->GetPageFirstCategory(pDraw->GetSubPage(pDraw->m_nCurPage)+1);

	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawLastCategory() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	pDraw->m_nCurPage = pDraw->GetPageLastCategory(pDraw->GetSubPage(pDraw->m_nCurPage)+1);

	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	SetGL(FALSE);
	msgDlg.Run();
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
}

void CARcBridgeView::OnDrawPageInit() 
{
	if(AfxMessageBox(_T("현재 도면의 배치 정보를 삭제하고 초기화합니다. 계속하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	CARcBridgeDoc *pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// 교량별 도면 옵션 생성
	pOpt->MakeDrawingOption();

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

	pDraw->InitMovePointInfo();
	OnDrawCurrentPage();
}

BOOL CARcBridgeView::PreTranslateMessage(MSG* pMsg) 
{
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();
	CARcBridgeDoc* pDoc = GetDocument();

	if(IsView3D())
	{
		if(pMsg->message == WM_MOUSEWHEEL)
		{
			/*
			if(m_pDlgOccView)
				return m_pDlgOccView->m_pOccView->PreTranslateMessage(pMsg);
				*/

			return TRUE;
		}
	}
	m_wndToolTip.RelayEvent(pMsg);

	// 엔터를 치면 치트키입력을 한다.
	// 단 디버그 모드일 때만 발동을 한다.
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==13)
	{
		InputCheatKey();
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==27)
	{
		m_wndToolTip.Activate(FALSE);
		if(GetDrawShape()!=CDrawTool::normal || m_bMoveCursor)
		{							
			if(m_bMoveCursor)
				OnSelectObject();
			m_bModeCursor = FALSE;
			CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
			COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->GetCurPage());
			if(pMove)
				pMove->ClearSelect();
		}
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_F11)
	{
		SetUseShx(IsUseShp() ? FALSE : TRUE);
		if(m_bFastTextDraw && !IsUseShp())
			OnDrawCurrentPage();
		else
			RedrawAll();
	}

	// PageUp, PageDown시 입력창 스케일 조정 //////////////////////////////
	if(pDoc->m_pARcBridgeInputStd && pFrm->m_nCurShortcutID == ID_SHORTCUT_INPUT)
	{
		CInputDialogBase* pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();
		if(pMsg->message==WM_KEYDOWN && pMsg->wParam==UK_PAGE_UP)
		{
			if(pDoc->m_pARcBridgeInputStd->IsScaleInputDialog())
			{
				pDoc->m_pARcBridgeInputStd->IncreaseScaleInputDialog(GetAsyncKeyState(VK_CONTROL));
				if(pDlg->GetSafeHwnd())
					pDlg->DrawInputDomyunView(FALSE);
			}
		}
		if(pMsg->message==WM_KEYDOWN && pMsg->wParam==UK_PAGE_DOWN)
		{
			if(pDoc->m_pARcBridgeInputStd->IsScaleInputDialog())
			{
				pDoc->m_pARcBridgeInputStd->DecreaseScaleInputDialog(GetAsyncKeyState(VK_CONTROL));
				if(pDlg->GetSafeHwnd())
					pDlg->DrawInputDomyunView(FALSE);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	
	return CDomyunGLView::PreTranslateMessage(pMsg);
}

void CARcBridgeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	if(nChar==VK_ESCAPE && !m_bEnableCrossCursor)
	{
		m_bModeCursor = FALSE;
		if(m_bMoveCursor)
			OnSelectObject();
	}

	if(nChar==VK_ESCAPE && GetDomyun()->m_SelectedObjMap.GetCount()) 
	{
		m_bModeCursor = FALSE;
		GetDomyun()->m_SelectedObjMap.RemoveAll();
	}

	switch(nChar)
	{
	case VK_F8 :
		{
			SetHVMode(!IsHVMode());
			long nPage = pStd->m_nCurPage;
			COutMovePoint* pMove = pStd->GetMovePoint(nPage);
			if(pMove!=NULL)
				pStd->GetMovePoint(nPage)->SetVHMode(m_bOnlyVHDraw);			

			if(pDoc->m_bFileOpen && pStd->GetMovePoint(nPage)!=NULL)
			{
				pFrm->m_wndStatusBar.SetPaneStyle(DIR_TEXT, pDoc->m_bFileOpen ? SBPS_NORMAL : SBPS_DISABLED);
				if(pStd->GetMovePoint(nPage)->IsVHMode())
					pFrm->m_wndStatusBar.SetPaneText(DIR_TEXT,_T("직각"));
				else
					pFrm->m_wndStatusBar.SetPaneText(DIR_TEXT,_T("자유"));
			}
			return;
		}
	}
		
	pStd->OnKeyDown(nChar, nFlags, nFlags);	
	
	CDomyunGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CARcBridgeView::OnSelectObject() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	if(m_bMoveCursor)
	{
		if(m_bUseToolTip)
		{
			m_wndToolTip.UpdateTipText(_T(""), this);
			m_wndToolTip.Activate(FALSE);
		}
		m_bOwnerMouse = m_bOldOwnerMouse;		
		m_bMoveCursor = FALSE;
		SetViewModeCtrl(NORMAL);
		CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
		COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->GetCurPage());		
		if(pMove)
			pMove->ClearSelect();
		ShowTaskButton(FALSE);
	}
	else 
	{
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);

		m_bMoveCursor = TRUE;
		if(m_bUseToolTip) m_wndToolTip.Activate(TRUE);
		m_bOldOwnerMouse = m_bOwnerMouse;
		m_bOwnerMouse = FALSE;
		ShowTaskButton();
	}
}

int CARcBridgeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDomyunGLView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndToolTip.Create(this);	
	m_wndToolTip.Activate(FALSE);
	m_wndToolTip.AddTool(this, _T(""));			 

	UINT nElapse = 120000;//1000 * 60 * 2(2분)

	nElapse = 6000009;
	long nChk = atoi(AfxGetApp()->GetProfileString(_T("Program Option"), _T("CheckVersion")));
	if(nChk==0) SetTimer(100, nElapse, NULL);

	return 0;
}

BOOL CARcBridgeView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(nHitTest==6 || nHitTest==7)
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}

	if(pDraw->IsInsertNoteMode())
	{
		Sleep(0);
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);
		if(pWnd==this) 
		{
			pDraw->OnSetCursor(pWnd, nHitTest, message);			
		}		
		return TRUE;
	}
	if(m_bMoveCursor && GetDrawShape()==CDrawTool::normal)
	{
		Sleep(0);
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);
		if(pWnd==this) 
		{
			if(!pDraw->OnSetCursor(pWnd, nHitTest, message))
			{				
				m_bModeCursor = FALSE;				
				m_wndToolTip.Activate (FALSE);
				SetCursor(m_ArrCur);
			}
			else
			{
				m_bModeCursor = TRUE;
				if(m_bUseToolTip && !pDraw->GetMovePoint(pDraw->m_nCurPage)->IsSelectRect())
				{												
					m_wndToolTip.Activate (TRUE);
					m_wndToolTip.UpdateTipText(pDraw->GetToolTipInfo(), this);															
				}			
				return TRUE;
			}			
		}	
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}
	
	CDrawTool* pTool = FindTool(c_drawShape);	
	if(pTool->m_drawShape == CDrawTool::normal)
	{
		if(pDoc->m_pARcBridgeInputStd->GetCurrentDialog() && pDoc->m_pARcBridgeInputStd->GetCurrentDialog()->GetIDD()==IDD_GEN_JONG_SECTION)
		{
			CInputGenJongSection *pDlg = (CInputGenJongSection *)pDoc->m_pARcBridgeInputStd->GetCurrentDialog();						
			POINT pt;
			GetCursorPos(&pt);		
			ScreenToClient(&pt);
			pDoc->m_pARcBridgeInputStd->m_strME.xy = ClientToExt(pt);
			long nMove	= pDlg->OnHitTest();
			if(nMove>-1) // 0 : 상하, 1 : 좌우, 2 : 상하좌우
			{
				SetCursor(nMove == 0 ? m_ActiveSizeCurUD : (nMove == 1 ? m_ActiveSizeCurLR : m_ActiveSizeCurUDLR));
				return TRUE;
			}
		}
	}
	
	return CDomyunGLView::OnSetCursor(pWnd, nHitTest, message);
}

void CARcBridgeView::OnUpdateAll(CCmdUI* pCmdUI) 
{
/*	if(!((CARcBridgeApp*)AfxGetApp())->m_bLockOK) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}*/
	
	CARcBridgeDoc* pDoc = GetDocument();

	pCmdUI->Enable(pDoc->m_bFileOpen);

	AfxGetMainWnd()->SendMessage(WM_UPDATE_STATUSBAR);
}

void CARcBridgeView::OnObjectInfo() 
{
	SetZoomRect(SELOBJECT);
}

void CARcBridgeView::OnMeasureDist() 
{
	SetZoomRect(6);
}

// 재정의
void CARcBridgeView::OnPostDrawList()
{
	CARcBridgeDoc *pDoc = GetDocument();
	//pDoc->m_pARcBridgeInputStd->OnDrawColorIndex(this);		
}

void CARcBridgeView::OnSetLayer() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CARoadOptionSheet pSht("도면 형식", pDoc->m_pARoadOptionStd);
	pSht.SetActivePage(0);
	if(pSht.DoModal()==IDOK)
	{
		GetDomyun()->ResetLayer();
		pDraw->SetModifiedAll();
	}
}

void CARcBridgeView::OnSetLine() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CARoadOptionSheet pSht("도면 형식", pDoc->m_pARoadOptionStd);
	pSht.SetActivePage(3);
	if(pSht.DoModal()==IDOK)
		pDraw->SetModifiedAll();
}

void CARcBridgeView::OnSetFont() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CARoadOptionSheet pSht("도면 형식", pDoc->m_pARoadOptionStd);
	pSht.SetActivePage(2);
	if(pSht.DoModal()==IDOK)
		pDraw->SetModifiedAll();
}

void CARcBridgeView::OnSetDim() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	CARoadOptionSheet pSht("도면 형식", pDoc->m_pARoadOptionStd);
	pSht.SetActivePage(1);
	if(pSht.DoModal()==IDOK)
		pDraw->SetModifiedAll();
}

void CARcBridgeView::OnCalcStation() 
{
	if(!m_dlgCalcStation.GetSafeHwnd())
		m_dlgCalcStation.Create(this, GetDocument()->m_pARcBridgeDataStd->GetLineApp());

	m_dlgCalcStation.ShowWindow(SW_SHOW);
}

void CARcBridgeView::OnUpdateCalcStation(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_pARcBridgeDataStd->IsValidLine()) 
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
}

void CARcBridgeView::OnCalcLineInfo() 
{
	if(!m_dlgCalcLineInfo.GetSafeHwnd())
		m_dlgCalcLineInfo.Create(this, GetDocument()->m_pARcBridgeDataStd->GetLineApp());

	m_dlgCalcLineInfo.ShowWindow(SW_SHOW);
}

void CARcBridgeView::OnUpdateCalcLineInfo(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_pARcBridgeDataStd->IsValidLine()) 
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
}

void CARcBridgeView::OnSnapEnd() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_END;
	nPre ^= OSNAP_END;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapEnd(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_END)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapMiddle() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_MID;
	nPre ^= OSNAP_MID;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapMiddle(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_MID)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapIntersection() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_CROSS;
	nPre ^= OSNAP_CROSS;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapIntersection(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_CROSS)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapCenter() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_CEN;
	nPre ^= OSNAP_CEN;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapCenter(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_CEN)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapPerpen() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_PER;
	nPre ^= OSNAP_PER;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapPerpen(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_PER)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapNode() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_NODE;
	nPre ^= OSNAP_NODE;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapNode(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_NODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnSnapNear() 
{
	long nPre = GetOsnapMode();
	nPre &= OSNAP_NEAR;
	nPre ^= OSNAP_NEAR;
	SetOsnapMode(nPre);
}

void CARcBridgeView::OnUpdateSnapNear(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	long nPre = GetOsnapMode();
	if(nPre & OSNAP_NEAR)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CARcBridgeView::OnAlignLeft() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignLeft();		
}

void CARcBridgeView::OnAlignCenterHor() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignCenter();	
}

void CARcBridgeView::OnAlignRight() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignRight();		
}

void CARcBridgeView::OnAlignTop() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignTop();		
}

void CARcBridgeView::OnAlignCenterVer() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignVCenter();	
}

void CARcBridgeView::OnAlignBottom() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignBottom();		
}

void CARcBridgeView::OnAlignSpaceAcross() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignHorz();	
}

void CARcBridgeView::OnAlignSpaceDown() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAlignVer();	
}

void CARcBridgeView::OnAlignAttachHor() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAttachHor();	
}

void CARcBridgeView::OnAlignAttachVer() 
{
	GetDocument()->m_pARcBridgeDrawingStd->OnAttachVer();	
}

void CARcBridgeView::OnObjectAlign() 
{
	CAlignSelectDlg dlgSel(GetDocument()->m_pARcBridgeDrawingStd);
	dlgSel.DoModal();	
}

void CARcBridgeView::OnUpdateAlign(CCmdUI* pCmdUI)
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(pDoc==NULL || pDraw==NULL) return;

	long nPage = pDraw->m_nCurPage;

	if(pDraw->GetMovePoint(nPage)==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(pCmdUI->m_nID==ID_SELECT_OBJECT || pCmdUI->m_nID==ID_OBJECT_ALIGN)
		return;

	if(pDraw->GetMovePoint(nPage)->m_SelectedMPE.GetSize()<2)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CARcBridgeView::OnAddNote() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD()!=ID_INPUT_END)
		return;
	if(!pDraw->IsInsertNoteMode())
		pDraw->SetInsertNoteMode(TRUE);
	else
		pDraw->SetInsertNoteMode(FALSE);	
}

void CARcBridgeView::OnUpdateAddNote(CCmdUI* pCmdUI) 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(pDoc->m_pARcBridgeInputStd->GetSaveMenuIDD()==ID_INPUT_END)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

	if(pDraw->IsInsertNoteMode())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CARcBridgeView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();		
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(nChar==27 && m_bMoveCursor) 
	{		
		m_bModeCursor = FALSE;
		OnSelectObject();
	}

	if(nChar==27 && pDraw->IsInsertNoteMode()) 
	{
		pDraw->SetInsertNoteMode(FALSE);
	}

	if(nChar==27 && GetDomyun()->m_SelectedObjMap.GetCount()) 
	{
		m_bModeCursor = FALSE;
		GetDomyun()->m_SelectedObjMap.RemoveAll();
	}
	
	OnSetCursor(this, 0, 0);
		
	if(IsGL())
	{	
		char axis = (char)nChar;
		if(axis == 27) return; // ESC
		char newAxis = (char)tolower((int)axis);
	//	BOOL bShift = (axis != newAxis) ? TRUE : FALSE;
		SetRanderContextState(TRUE);

		switch (newAxis)   
		{
			case 'x': m_dHoriRotate = 0.0;
					  m_dVertRotate = 0.0;
					  break;
			case 'y': m_dHoriRotate = 90.0;
					  m_dVertRotate = 0.0;
					  break;
			case 'z': m_dHoriRotate = 90.0;
					  m_dVertRotate = -90.0;
					  break;
			case 'd': m_dHoriRotate = 63.0;
					  m_dVertRotate = -20.0;
					  break;
			case 'X': m_dHoriRotate = 180.0;
					  m_dVertRotate = 0.0;
					  break;
			case 'Y': m_dHoriRotate = -90.0;
					  m_dVertRotate = 0.0;
					  break;
			case 'Z': m_dHoriRotate = 90.0;
					  m_dVertRotate = 90.0;
					  break;
		}

		IsEyes() ?	SetClippingVolume() : Rotate(0,0);

		SetRanderContextState(FALSE);
		ZoomAll ();
		ZoomPlusNegaExt(FALSE,0.3);	
	}

	// 임차장님이 주신거 ////////////
	m_DrawToolZoom.OnCancel(this);
	CString	sChar;
	sChar.Format(_T("%c"),nChar);	// ASCII -> Char
	sChar.MakeLower();
	if( sChar == _T("w") )
		OnZoomWindow();
	else if( sChar == _T("d") )
		OnZoomDynamic();
	else if( sChar == _T("i") )
		OnZoomIn();
	else if( sChar == _T("a") || sChar == _T("e") )
		OnZoomAll();
	else if( sChar == _T("m") )
		OnZoomMove();
	else if( sChar == _T("o") )
		OnZoomOut();
	else if( sChar == _T("p") )
		OnZoomPrevious();
	else if( sChar == _T("r") )
		OnZoomRealtime();
	////////////////////////////

	CDomyunView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CARcBridgeView::OnToolbarControl(UINT nID, NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControls* pControls = tagNMCONTROL->pControl->GetControls();

	CXTPControlEdit* pPage = (CXTPControlEdit*)pControls->GetAt(0);
	CXTPControlComboBox* pBridge = (CXTPControlComboBox*)pControls->GetAt(1);
	CXTPControlComboBox* pCategory = (CXTPControlComboBox*)pControls->GetAt(2);

	CARcBridgeDoc* pDoc = GetDocument();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	CString str = _T("");
	int nSubPage = 0;
	int nBri = 0;
	int nCategory = 0;
	if(nID==IDC_TEDIT_PAGE)
	{
		nSubPage = atoi(pPage->GetEditText());
		if(nSubPage<1)
		{
			nSubPage = 1;
			pPage->SetEditText(_T("1"));
		}
		if(nSubPage>pDraw->GetCountTotalPage())
		{
			nSubPage = pDraw->GetCountTotalPage();
			str.Format(_T("%d"), nSubPage);
			pPage->SetEditText(str);
		}
		nBri = pDraw->GetBridgeNumber(nSubPage);
		pBridge->SetCurSel(nBri);
		nCategory = pDraw->GetCategoryNumber(nSubPage);
		pCategory->SetCurSel(nCategory);
	}
	else
	{
		nBri = pBridge->GetCurSel();
		nCategory = pCategory->GetCurSel();
		str = pDraw->GetTitleCategory(nCategory);
		nSubPage = pDraw->GetSubPageSttCategory(nBri, str) + 1;
		str.Format(_T("%d"), nSubPage);
		pPage->SetEditText(str);
	}

	pDraw->m_nCurPage = pDraw->GetOrgPage(nSubPage-1);

	nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);

	OnDrawChangePage();

	return TRUE;
}

void CARcBridgeView::OnCategoryOrder() 
{
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(pDraw->GetDrawPageMng()->ResetPageSort()==IDOK)
	{
		GetBasicDomyun()->ClearEtt(TRUE);
		SetDomyun(GetBasicDomyun());
		RedrawAll();

		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
		pFrm->m_paneShortcutDrawing.InitTree();
	}
}

void CARcBridgeView::OnAlignUndoDummy(WPARAM nID)
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	long nSelID = nID - ID_ALIGN_UNDO_DUMMY;

	int i=0; for(i=pMove->m_UnDoMPE.GetSize()-1; i>=nSelID; i--)
		pMove->OnUnDo(i);
}

void CARcBridgeView::OnAlignRedoDummy(WPARAM nID)
{	
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	long nSelID = nID - ID_ALIGN_REDO_DUMMY;

	int i=0; for(i=pMove->m_ReDoMPE.GetSize()-1; i>=nSelID; i--)
		pMove->OnReDo(i);
}

void CARcBridgeView::OnAlignUndo() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	OnAlignUndoDummy(ID_ALIGN_UNDO_DUMMY+pMove->m_UnDoMPE.GetSize()-1);	
}

void CARcBridgeView::OnUpdateAlignUndo(CCmdUI* pCmdUI) 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	if(pMove!=NULL)
	{
		if(pMove->m_UnDoMPE.GetSize())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

// 치트키 입력을 받는다.
// 엔터를 칠때까지
void CARcBridgeView::InputCheatKey()
{
	CARcBridgeDoc *pDoc = GetDocument();

	static BOOL bInputingCheatKey	= FALSE;
	const long nCountCheatKey = 7;
	CString sCheatKey[nCountCheatKey]	= {"show info", "hide info", "left", "center", "right", "export bri", "show hdan"};
	if(bInputingCheatKey)
	{
		m_pEdit->UpdateData(TRUE);
		CString sInput	= _T("");
		m_pEdit->GetWindowText(sInput);
		long nIdxCheat	= -1;
		for(long i = 0; i < nCountCheatKey; i++)
		{
			if(sInput == sCheatKey[i])
			{
				nIdxCheat	= i;
				break;
			}
		}


		switch(nIdxCheat)
		{
		case 0: m_bViewVectorInfo	= TRUE; break;
		case 1: m_bViewVectorInfo	= FALSE; break;
		case 2: m_nLeft				= -1; break;
		case 3: m_nLeft				= 0; break;
		case 4: m_nLeft				= 1; break;
		case 5: pDoc->ExportBridgeInfo(); break;
		case 6: pDoc->ShowDlgHDanLength(); break;
		default: break;
		}

		bInputingCheatKey	= FALSE;
		m_pEdit->ShowWindow(FALSE);
		m_pEdit->SetSel(0, -1);

		// 치트키 적용 //////
		CARcBridgeDoc* pDoc = GetDocument();
		pDoc->m_pARcBridgeInputStd->m_bViewVectorInfo	= m_bViewVectorInfo;
		pDoc->m_pARcBridgeInputStd->m_nLeft				= m_nLeft;
		CARcBridgeDBStandard::m_bViewVectorInfo	= m_bViewVectorInfo;
		/////////////////////

	}
	else
	{
		if(!m_pEdit)
		{
			m_pEdit	= new CEdit;
			m_pEdit->Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 100, 16), this, 0);
		}
		m_pEdit->ShowWindow(TRUE);
		m_pEdit->SetFocus();
		bInputingCheatKey	= TRUE;
	}
}
void CARcBridgeView::OnAlignRedo() 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	OnAlignRedoDummy(ID_ALIGN_REDO_DUMMY+pMove->m_ReDoMPE.GetSize()-1);
}

void CARcBridgeView::OnUpdateAlignRedo(CCmdUI* pCmdUI) 
{
	CARcBridgeDoc *pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	COutMovePoint* pMove = pDraw->GetMovePoint(pDraw->m_nCurPage);

	if(pMove!=NULL)
	{
		if(pMove->m_ReDoMPE.GetSize())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CARcBridgeView::OnTimer(UINT nIDEvent) 
{
	CARcBridgeApp *pApp = (CARcBridgeApp*)AfxGetApp();
	CARcBridgeDoc* pDoc = GetDocument();
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	if(nIDEvent == 	AUTOSAVE_TIMER)
	{
		CString szDir = pDoc->GetPathName();
		if(pDoc->m_bFileOpen && szDir != _T(""))
			szDir.Format("%s.bak", szDir);
		else
			szDir.Format("%s\\AutoSave.%s", pApp->GetFixedDirectory(), "arb");
		CFile f;
		if(f.Open(szDir,CFile::modeCreate|CFile::modeWrite)!=FALSE)
		{
			CArchive ar(&f,CArchive::store);
			pDoc->Serialize(ar);
			ar.Close();
			f.Close();
		}
	}
	
	if(nIDEvent == 100)
	{	
		CString szRunTime;
		aUpdateCheck("", atoi(AfxGetApp()->GetProfileString("Program Option", "CheckVersion","")), szRunTime);
		if(!szRunTime.IsEmpty())	
			AfxGetApp()->WriteProfileString("Settings", "LastRunDate",szRunTime);
	}
	else if(nIDEvent == 77)
	{
		CDrawTool* pTool = FindTool(c_drawShape);	
		CInputDialogBase *pDlg = pDoc->m_pARcBridgeInputStd->GetCurrentDialog();		

		// 도면출력중일때는 여기 들어오면 안됨.
		if(pDraw->GetDxfDlg() && pDraw->GetDxfDlg()->GetSafeHwnd() && pDraw->GetDxfDlg()->IsWindowVisible())
			return;
		// 도면 출력 중일때
		CMainFrame* pFrm = (CMainFrame*)GetParentOwner();
		if(!pFrm || pFrm->m_nCurShortcutID == ID_SHORTCUT_DRAWING)
			return;
		
		// 수량출력 중일때도 들어오지 않게
		if(IsViewOutBM() == TRUE)
			return;

		// View가 활성화되지 않았다면 들어오지 않게
		if((CView*)pFrm->GetActiveView() != (CView*)this)
			return;
		
		if(pDlg && pTool->m_drawShape == CDrawTool::normal)
		{
			POINT pt;
			GetCursorPos(&pt);						
			ScreenToClient(&pt);			
			CRect rect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);	
			m_bStopFind = FALSE;
			CObArray obArr;
			if(Intersects(&obArr,rect,INTER_TEXT))
			{
				CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);
				for(long i = 0; i < obArr.GetSize(); i++)
				{
					pSelObj	= (CDrawObject*)obArr.GetAt(i);
					if(pSelObj->GetEttKind() == ETT_TEXT)
						break;
				}
				if(pSelObj->GetEttKind()==ETT_TEXT)
				{				
					CObText *pText = (CObText *)pSelObj;
					CString szText = pText->m_TextString;
					if(pDlg->IsFocusItem(szText))// && !m_bHasFocus)					
					{					
						if (m_bPainted)							
							return;					
						else 
						{
							m_bPainted = TRUE;	
							CDC* pDC = GetDC();
							CDRect rectText = GetDomyun()->GetTextRect(pText, szText);						
							
							rectText.left += pText->m_SttPoint.x;
							rectText.top  += pText->m_SttPoint.y;
							rectText.right += pText->m_SttPoint.x;
							rectText.bottom += pText->m_SttPoint.y;

							CRect rect = ExtToClient(rectText);	
							rect.NormalizeRect();
							rect.DeflateRect(-2,-2);

							DrawMouseLine(FALSE);
							DrawMouseRect(FALSE);						

							if(m_bPush)
							{
								pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));												
								rect.DeflateRect (1, 1);
								pDC->Draw3dRect (rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_3DLIGHT));
							}
							else
							{
								pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));												
								rect.DeflateRect (1, 1);
								pDC->Draw3dRect (rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
							}
							// 마우스 그림
							if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
							{	
								DrawMouseLine(TRUE);
								DrawMouseRect(TRUE);		
							}		
							ReleaseDC(pDC);		
						}		
					}
					else
					{
						KillTimer(77);
						if (m_bPainted == TRUE) 
						{											
							Invalidate();							
							m_bActivate = FALSE; //깜박이 제어
							m_bPainted = FALSE;
						}							
						return;
					}				 
				}
				else
				{
					KillTimer(77);
					if (m_bPainted == TRUE) 
					{
						Invalidate();					
						m_bPainted = FALSE;
					}							
					return;
				}
			}
		}				
	}

	CDomyunGLView::OnTimer(nIDEvent);
}

void CARcBridgeView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDomyunGLView::CalcWindowRect(lpClientRect, nAdjustType);
}

void CARcBridgeView::OnMove(int x, int y) 
{
	CDomyunGLView::OnMove(x, y);
	
	// TODO: Add your message handler code here
}

void CARcBridgeView::OnSetFocus(CWnd* pOldWnd) 
{
	CDomyunGLView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
}

LRESULT CARcBridgeView::ApplyDom(WPARAM wParam, LPARAM lParam)
{
	CARcBridgeDoc* pDoc = GetDocument();

	return 1;
}

void CARcBridgeView::OnBlockCut() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockCut();
}

void CARcBridgeView::OnBlockPaste() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockPaste();
}

void CARcBridgeView::OnBlockCancel() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockCancel();
}

void CARcBridgeView::OnUpdateBlockCancel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	long nCuttingBlock	= GetDocument()->m_pARcBridgeDrawingStd->m_nCuttingBlock;
	pCmdUI->Enable(nCuttingBlock == 0 || nCuttingBlock == 1);	
}

void CARcBridgeView::OnUpdateBlockPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	long nCuttingBlock	= GetDocument()->m_pARcBridgeDrawingStd->m_nCuttingBlock;
	pCmdUI->Enable(nCuttingBlock == 0 || nCuttingBlock == 1);	
}

void CARcBridgeView::OnBlockCopy() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockCopy();	
}

void CARcBridgeView::OnBlockInitCur() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockInitCurPage();		
}

void CARcBridgeView::OnBlockInitAll() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->m_pARcBridgeDrawingStd->OnBlockInitAllPage();		
}
// 통합 교량 정보 파일 내보내기
void CARcBridgeView::OnExportBridgeinfo() 
{
	// TODO: Add your command handler code here
	CARcBridgeDoc* pDoc = GetDocument();
	pDoc->ExportBridgeInfo();
}

void CARcBridgeView::OnToolHgexport() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CARcBridgeDoc* pDoc = GetDocument();
	if(pMFrm && pDoc)
	{
		if(pMFrm->m_pHgExportMain->IsRun())
			pMFrm->m_pHgExportMain->Stop();
		else
		{
			pMFrm->m_pHgExportMain->Run(pDoc->m_pARoadOptionStd);
		}
	}
}

void CARcBridgeView::OnUpdateToolHgexport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	if(pMFrm)
	{
		pCmdUI->SetCheck(pMFrm->m_pHgExportMain->IsRun());
	}
}

BOOL CARcBridgeView::IsView3D()
{
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();
	if(pFrm==NULL)	return FALSE;
	return pFrm->m_nCurShortcutID == ID_SHORTCUT_MODEL;
}

// 현재 뷰가 수량산출용이어야 하는지 판단
BOOL CARcBridgeView::IsViewOutBM()
{
	CMainFrame* pFrm = (CMainFrame*)GetParentOwner();	
	return pFrm->m_nCurShortcutID == ID_SHORTCUT_BM;
}


// shortcutbar에 따른 뷰 설정
void CARcBridgeView::SettingViewByShortcutBar()
{
	CARcBridgeDoc *pDoc = GetDocument();
	if(pDoc==NULL)	return;

	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	if(pBMStd->m_pOutBMDialog->GetSafeHwnd() == 0)
	{
		pBMStd->m_pOutBMDialog->Create(IDD_DIALOG_OUTBM, this);
	}


	if(IsViewOutBM())
	{
		GetDomyun()->ClearEtt(TRUE, FALSE);
		pBMStd->m_pOutBMDialog->ShowWindow(TRUE);
		pBMStd->m_pOutBMDialog->SetFocus();
	}
	else
		pBMStd->m_pOutBMDialog->ShowWindow(FALSE);




	/*
	if(!m_pDlgOccView)
	{
		CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
		m_pDlgOccView	= new CDlgOccView(IDD_DIALOG_HGOCCVIEW, this, szPath);
		m_pDlgOccView->Create(IDD_DIALOG_HGOCCVIEW, this);
	}
	if(!m_pDlgOccViewRendering)
	{
		m_pDlgOccViewRendering = new CDlgOcc3D;
		m_pDlgOccViewRendering->Create(IDD_DIALOG_OCCVIEW_SELECT, m_pDlgOccView);
	}

	if(IsView3D())
	{
		m_pDlgOccView->m_pOccView->m_pManage->RemoveAllData();
		m_pDlgOccView->m_pOccView->m_3DRender.RemoveAllDataCallList();

		CMainFrame* pFrm = (CMainFrame*)GetParentOwner();
		if(pFrm==NULL)	return;
		//m_ubARcBridgeData.m_pARcBridgeDataStd	= pDoc->m_pARcBridgeDataStd;
		m_pDlgOccView->m_pOccView->m_pTreeCtrl	= &pFrm->m_paneShortcutModel.m_wndTreeModel.m_wndTreeObject;
		m_pDlgOccView->m_pOccView->m_pGrid		= &pFrm->m_paneShortcutModel.m_wndTreeModel.m_wndPropGrid;
		m_pDlgOccView->m_pOccView->m_pMenuTreeCtrl	= &pFrm->m_paneShortcutModel.m_wndTreeModel;
		//m_pDlgOccView->m_pDraw3D	= &m_ubARcBridgeData;
		m_pDlgOccView->ShowWindow(SW_SHOW);
		/////////////////////////////////////////////////////////////////////////////////////////////////
		
		m_pDlgOccViewRendering->m_pARcBridgeDataStd	= pDoc->m_pARcBridgeDataStd;
		//m_pDlgOccViewRendering->m_pUbiARcBridgeData	= &m_ubARcBridgeData;
		m_pDlgOccViewRendering->m_pDlgOccView	= m_pDlgOccView;
		if(m_pDlgOccViewRendering->GetSafeHwnd()==NULL)
				m_pDlgOccViewRendering->Create(IDD_DIALOG_OCCVIEW_SELECT, m_pDlgOccView);
		m_pDlgOccViewRendering->ShowWindow(SW_SHOW);

	}
	else
	{
		m_pDlgOccView->ShowWindow(SW_HIDE);
		if(m_pDlgOccViewRendering->GetSafeHwnd()!=NULL)
		{
			m_pDlgOccViewRendering->m_pARcBridgeDataStd	= NULL;
			m_pDlgOccViewRendering->ShowWindow(SW_HIDE);
		}
	}
	*/
}

void CARcBridgeView::OnExportAStrutTie()
{
	CARcBridgeDoc *pDoc = GetDocument();
	if(pDoc == NULL) return;

	CARcBridgeDataStd *pDataStd = pDoc->m_pARcBridgeDataStd;

	if(pDataStd->IsLockRcBridgeStmModule() == FALSE)
	{
		AfxMessageBox(_T("현재 버전에서는 지원하지 않는 기능입니다."));
		return;
	}

	CDialogExportStxOption dlg(pDataStd);
	dlg.DoModal();
}

void CARcBridgeView::OnUpdateExportAStrutTie( CCmdUI* pCmdUI )
{
	CARcBridgeDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->m_bFileOpen);
}