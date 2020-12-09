# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridge.dsp
!IF "$(CFG)" == ""
CFG=ARcBridge - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridge - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridge - Win32 Release" && "$(CFG)" != "ARcBridge - Win32 Debug" && "$(CFG)" != "ARcBridge - Win32 Release_Nong"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridge.mak" CFG="ARcBridge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridge - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ARcBridge - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ARcBridge - Win32 Release_Nong" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "../commonCompile/lib/Out" "..\CommonCompile\Output\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc"

!ELSE 

ALL : "ARcBridgeInput - Win32 Release" "ARcBridgeOutDesign - Win32 Release" "../commonCompile/lib/Out" "..\CommonCompile\Output\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeOutDesign - Win32 ReleaseCLEAN" "ARcBridgeInput - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridge.obj"
	-@erase "$(INTDIR)\ARcBridge.pch"
	-@erase "$(INTDIR)\ARcBridge.res"
	-@erase "$(INTDIR)\ARcBridge.sbr"
	-@erase "$(INTDIR)\ARcBridgeDoc.obj"
	-@erase "$(INTDIR)\ARcBridgeDoc.sbr"
	-@erase "$(INTDIR)\ARcBridgeView.obj"
	-@erase "$(INTDIR)\ARcBridgeView.sbr"
	-@erase "$(INTDIR)\CalcLineInfoDlg.obj"
	-@erase "$(INTDIR)\CalcLineInfoDlg.sbr"
	-@erase "$(INTDIR)\CalcStationDlg.obj"
	-@erase "$(INTDIR)\CalcStationDlg.sbr"
	-@erase "$(INTDIR)\DlgOcc3D.obj"
	-@erase "$(INTDIR)\DlgOcc3D.sbr"
	-@erase "$(INTDIR)\LockDataInfo.obj"
	-@erase "$(INTDIR)\LockDataInfo.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\SaveCompleteDlg.obj"
	-@erase "$(INTDIR)\SaveCompleteDlg.sbr"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.obj"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.sbr"
	-@erase "$(INTDIR)\ShortcutPaneInput.obj"
	-@erase "$(INTDIR)\ShortcutPaneInput.sbr"
	-@erase "$(INTDIR)\ShortcutPaneStart.obj"
	-@erase "$(INTDIR)\ShortcutPaneStart.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\UbiARcBridgeData.obj"
	-@erase "$(INTDIR)\UbiARcBridgeData.sbr"
	-@erase "$(INTDIR)\UpdateModifyDlg.obj"
	-@erase "$(INTDIR)\UpdateModifyDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WaitForSubProgram.obj"
	-@erase "$(INTDIR)\WaitForSubProgram.sbr"
	-@erase "$(INTDIR)\WmfSizeDlg.obj"
	-@erase "$(INTDIR)\WmfSizeDlg.sbr"
	-@erase "$(OUTDIR)\ARcBridge.bsc"
	-@erase "..\CommonCompile\Output\ARcBridge.exe"
	-@erase "../commonCompile/lib/Out"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WNT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridge.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridge.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ARcBridge.sbr" \
	"$(INTDIR)\ARcBridgeDoc.sbr" \
	"$(INTDIR)\ARcBridgeView.sbr" \
	"$(INTDIR)\CalcLineInfoDlg.sbr" \
	"$(INTDIR)\CalcStationDlg.sbr" \
	"$(INTDIR)\DlgOcc3D.sbr" \
	"$(INTDIR)\LockDataInfo.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\SaveCompleteDlg.sbr" \
	"$(INTDIR)\ShortcutPaneDrawing.sbr" \
	"$(INTDIR)\ShortcutPaneInput.sbr" \
	"$(INTDIR)\ShortcutPaneStart.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UbiARcBridgeData.sbr" \
	"$(INTDIR)\UpdateModifyDlg.sbr" \
	"$(INTDIR)\WaitForSubProgram.sbr" \
	"$(INTDIR)\WmfSizeDlg.sbr"

"$(OUTDIR)\ARcBridge.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=CommonBridge.lib opengl32.lib glu32.lib ARcBridgeDrawingOption.lib ARcBridgeInput.lib DrawManage.lib XTP8610Lib.lib ARcBridgeData.lib ARoadConfig.lib ARcBridgeDrawing.lib ARcBridgeOutBM.lib ARcBridgeOutDesign.lib AException.lib ARcBridgeDBDraw.lib shlwapi.lib HgBaseBM.lib ShareWithADManager.lib RCModule.lib BearingInfo.lib ARcBridgeCalculate.lib UbGround.lib HGFrame.lib HGRender.lib TKBrep.lib TKBool.lib TKernel.lib TKFeat.lib TKFillet.lib TKG3d.lib TKGeomAlgo.lib TKGeomBase.lib TKHLR.lib TKMath.lib TKOffset.lib TKPrim.lib TKTopAlgo.lib TKG2d.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ARcBridge.pdb" /machine:I386 /out:"../CommonCompile/Output/ARcBridge.exe" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../CommonCompile/lib" /libpath:"$(HANGIL_ENGINE)/bin" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridge.obj" \
	"$(INTDIR)\ARcBridgeDoc.obj" \
	"$(INTDIR)\ARcBridgeView.obj" \
	"$(INTDIR)\CalcLineInfoDlg.obj" \
	"$(INTDIR)\CalcStationDlg.obj" \
	"$(INTDIR)\DlgOcc3D.obj" \
	"$(INTDIR)\LockDataInfo.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SaveCompleteDlg.obj" \
	"$(INTDIR)\ShortcutPaneDrawing.obj" \
	"$(INTDIR)\ShortcutPaneInput.obj" \
	"$(INTDIR)\ShortcutPaneStart.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UbiARcBridgeData.obj" \
	"$(INTDIR)\UpdateModifyDlg.obj" \
	"$(INTDIR)\WaitForSubProgram.obj" \
	"$(INTDIR)\WmfSizeDlg.obj" \
	"$(INTDIR)\ARcBridge.res" \
	"..\CommonCompile\lib\ARcBridgeOutDesign.lib" \
	"..\CommonCompile\lib\ARcBridgeInput.lib"

"..\CommonCompile\Output\ARcBridge.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "../commonCompile/libD/OutD" "..\CommonCompile\OutputD\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc"

!ELSE 

ALL : "ARcBridgeInput - Win32 Debug" "ARcBridgeOutDesign - Win32 Debug" "../commonCompile/libD/OutD" "..\CommonCompile\OutputD\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeOutDesign - Win32 DebugCLEAN" "ARcBridgeInput - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridge.obj"
	-@erase "$(INTDIR)\ARcBridge.pch"
	-@erase "$(INTDIR)\ARcBridge.res"
	-@erase "$(INTDIR)\ARcBridge.sbr"
	-@erase "$(INTDIR)\ARcBridgeDoc.obj"
	-@erase "$(INTDIR)\ARcBridgeDoc.sbr"
	-@erase "$(INTDIR)\ARcBridgeView.obj"
	-@erase "$(INTDIR)\ARcBridgeView.sbr"
	-@erase "$(INTDIR)\CalcLineInfoDlg.obj"
	-@erase "$(INTDIR)\CalcLineInfoDlg.sbr"
	-@erase "$(INTDIR)\CalcStationDlg.obj"
	-@erase "$(INTDIR)\CalcStationDlg.sbr"
	-@erase "$(INTDIR)\DlgOcc3D.obj"
	-@erase "$(INTDIR)\DlgOcc3D.sbr"
	-@erase "$(INTDIR)\LockDataInfo.obj"
	-@erase "$(INTDIR)\LockDataInfo.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\SaveCompleteDlg.obj"
	-@erase "$(INTDIR)\SaveCompleteDlg.sbr"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.obj"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.sbr"
	-@erase "$(INTDIR)\ShortcutPaneInput.obj"
	-@erase "$(INTDIR)\ShortcutPaneInput.sbr"
	-@erase "$(INTDIR)\ShortcutPaneStart.obj"
	-@erase "$(INTDIR)\ShortcutPaneStart.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\UbiARcBridgeData.obj"
	-@erase "$(INTDIR)\UbiARcBridgeData.sbr"
	-@erase "$(INTDIR)\UpdateModifyDlg.obj"
	-@erase "$(INTDIR)\UpdateModifyDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WaitForSubProgram.obj"
	-@erase "$(INTDIR)\WaitForSubProgram.sbr"
	-@erase "$(INTDIR)\WmfSizeDlg.obj"
	-@erase "$(INTDIR)\WmfSizeDlg.sbr"
	-@erase "$(OUTDIR)\ARcBridge.bsc"
	-@erase "$(OUTDIR)\ARcBridge.pdb"
	-@erase "..\CommonCompile\OutputD\ARcBridge.exe"
	-@erase "..\CommonCompile\OutputD\ARcBridge.ilk"
	-@erase "../commonCompile/libD/OutD"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WNT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridge.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridge.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ARcBridge.sbr" \
	"$(INTDIR)\ARcBridgeDoc.sbr" \
	"$(INTDIR)\ARcBridgeView.sbr" \
	"$(INTDIR)\CalcLineInfoDlg.sbr" \
	"$(INTDIR)\CalcStationDlg.sbr" \
	"$(INTDIR)\DlgOcc3D.sbr" \
	"$(INTDIR)\LockDataInfo.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\SaveCompleteDlg.sbr" \
	"$(INTDIR)\ShortcutPaneDrawing.sbr" \
	"$(INTDIR)\ShortcutPaneInput.sbr" \
	"$(INTDIR)\ShortcutPaneStart.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UbiARcBridgeData.sbr" \
	"$(INTDIR)\UpdateModifyDlg.sbr" \
	"$(INTDIR)\WaitForSubProgram.sbr" \
	"$(INTDIR)\WmfSizeDlg.sbr"

"$(OUTDIR)\ARcBridge.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=CommonBridgeD.lib opengl32.lib glu32.lib ARcBridgeDrawingOptionD.lib ARcBridgeInputD.lib DrawManageD.lib XTP8610Libd.lib ARcBridgeDataD.lib ARoadConfigD.lib ARcBridgeDrawingD.lib ARcBridgeOutBMD.lib ARcBridgeOutDesignD.lib AExceptionD.lib ARcBridgeDBDrawD.lib shlwapi.lib HgBaseBMD.lib ShareWithADManagerD.lib RCModuleD.lib BearingInfoD.lib ARcBridgeCalculateD.lib UbGroundD.lib HGFrameD.lib HGRenderD.lib TKBrep.lib TKBool.lib TKernel.lib TKFeat.lib TKFillet.lib TKG3d.lib TKGeomAlgo.lib TKGeomBase.lib TKHLR.lib TKMath.lib TKOffset.lib TKPrim.lib TKTopAlgo.lib TKG2d.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ARcBridge.pdb" /debug /machine:I386 /out:"../CommonCompile/OutputD/ARcBridge.exe" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../CommonCompile/libD" /libpath:"$(HANGIL_ENGINE)/bin" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridge.obj" \
	"$(INTDIR)\ARcBridgeDoc.obj" \
	"$(INTDIR)\ARcBridgeView.obj" \
	"$(INTDIR)\CalcLineInfoDlg.obj" \
	"$(INTDIR)\CalcStationDlg.obj" \
	"$(INTDIR)\DlgOcc3D.obj" \
	"$(INTDIR)\LockDataInfo.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SaveCompleteDlg.obj" \
	"$(INTDIR)\ShortcutPaneDrawing.obj" \
	"$(INTDIR)\ShortcutPaneInput.obj" \
	"$(INTDIR)\ShortcutPaneStart.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UbiARcBridgeData.obj" \
	"$(INTDIR)\UpdateModifyDlg.obj" \
	"$(INTDIR)\WaitForSubProgram.obj" \
	"$(INTDIR)\WmfSizeDlg.obj" \
	"$(INTDIR)\ARcBridge.res" \
	"..\CommonCompile\libD\ARcBridgeOutDesignD.lib" \
	"..\CommonCompile\libD\ARcBridgeInputD.lib"

"..\CommonCompile\OutputD\ARcBridge.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Release_Nong"

OUTDIR=.\ARcBridge___Win32_Release_Nong
INTDIR=.\ARcBridge___Win32_Release_Nong
# Begin Custom Macros
OutDir=.\ARcBridge___Win32_Release_Nong
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "../commonCompile/lib/Out" "..\CommonCompile\Output\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc" "../commonCompile/OutputD/ARcBridgeCopy.out"

!ELSE 

ALL : "../commonCompile/lib/Out" "..\CommonCompile\Output\ARcBridge.exe" "$(OUTDIR)\ARcBridge.bsc" "../commonCompile/OutputD/ARcBridgeCopy.out"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridge.obj"
	-@erase "$(INTDIR)\ARcBridge.pch"
	-@erase "$(INTDIR)\ARcBridge.res"
	-@erase "$(INTDIR)\ARcBridge.sbr"
	-@erase "$(INTDIR)\ARcBridgeDoc.obj"
	-@erase "$(INTDIR)\ARcBridgeDoc.sbr"
	-@erase "$(INTDIR)\ARcBridgeView.obj"
	-@erase "$(INTDIR)\ARcBridgeView.sbr"
	-@erase "$(INTDIR)\CalcLineInfoDlg.obj"
	-@erase "$(INTDIR)\CalcLineInfoDlg.sbr"
	-@erase "$(INTDIR)\CalcStationDlg.obj"
	-@erase "$(INTDIR)\CalcStationDlg.sbr"
	-@erase "$(INTDIR)\DlgOcc3D.obj"
	-@erase "$(INTDIR)\DlgOcc3D.sbr"
	-@erase "$(INTDIR)\LockDataInfo.obj"
	-@erase "$(INTDIR)\LockDataInfo.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\SaveCompleteDlg.obj"
	-@erase "$(INTDIR)\SaveCompleteDlg.sbr"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.obj"
	-@erase "$(INTDIR)\ShortcutPaneDrawing.sbr"
	-@erase "$(INTDIR)\ShortcutPaneInput.obj"
	-@erase "$(INTDIR)\ShortcutPaneInput.sbr"
	-@erase "$(INTDIR)\ShortcutPaneStart.obj"
	-@erase "$(INTDIR)\ShortcutPaneStart.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\UbiARcBridgeData.obj"
	-@erase "$(INTDIR)\UbiARcBridgeData.sbr"
	-@erase "$(INTDIR)\UpdateModifyDlg.obj"
	-@erase "$(INTDIR)\UpdateModifyDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WaitForSubProgram.obj"
	-@erase "$(INTDIR)\WaitForSubProgram.sbr"
	-@erase "$(INTDIR)\WmfSizeDlg.obj"
	-@erase "$(INTDIR)\WmfSizeDlg.sbr"
	-@erase "$(OUTDIR)\ARcBridge.bsc"
	-@erase "$(OUTDIR)\ARcBridge.pdb"
	-@erase "..\CommonCompile\Output\ARcBridge.exe"
	-@erase "..\CommonCompile\Output\ARcBridge.ilk"
	-@erase "../commonCompile/lib/Out"
	-@erase "../commonCompile/OutputD/ARcBridgeCopy.out"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WNT" /D "_NONG_GI_BAN_" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridge.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridge.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ARcBridge.sbr" \
	"$(INTDIR)\ARcBridgeDoc.sbr" \
	"$(INTDIR)\ARcBridgeView.sbr" \
	"$(INTDIR)\CalcLineInfoDlg.sbr" \
	"$(INTDIR)\CalcStationDlg.sbr" \
	"$(INTDIR)\DlgOcc3D.sbr" \
	"$(INTDIR)\LockDataInfo.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\SaveCompleteDlg.sbr" \
	"$(INTDIR)\ShortcutPaneDrawing.sbr" \
	"$(INTDIR)\ShortcutPaneInput.sbr" \
	"$(INTDIR)\ShortcutPaneStart.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UbiARcBridgeData.sbr" \
	"$(INTDIR)\UpdateModifyDlg.sbr" \
	"$(INTDIR)\WaitForSubProgram.sbr" \
	"$(INTDIR)\WmfSizeDlg.sbr"

"$(OUTDIR)\ARcBridge.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=CommonBridge.lib opengl32.lib glu32.lib ARcBridgeDrawingOption.lib ARcBridgeInput.lib DrawManage.lib XTP8610Lib.lib ARcBridgeData.lib ARoadConfig.lib ARcBridgeDrawing.lib ARcBridgeOutBM.lib ARcBridgeOutDesign.lib AException.lib ARcBridgeDBDraw.lib shlwapi.lib HgBaseBM.lib ShareWithADManager.lib RCModule.lib BearingInfo.lib ARcBridgeCalculate.lib UbGround.lib HGFrame.lib HGRender.lib TKBrep.lib TKBool.lib TKernel.lib TKFeat.lib TKFillet.lib TKG3d.lib TKGeomAlgo.lib TKGeomBase.lib TKHLR.lib TKMath.lib TKOffset.lib TKPrim.lib TKTopAlgo.lib TKG2d.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ARcBridge.pdb" /debug /machine:I386 /out:"../CommonCompile/Output/ARcBridge.exe" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridge.obj" \
	"$(INTDIR)\ARcBridgeDoc.obj" \
	"$(INTDIR)\ARcBridgeView.obj" \
	"$(INTDIR)\CalcLineInfoDlg.obj" \
	"$(INTDIR)\CalcStationDlg.obj" \
	"$(INTDIR)\DlgOcc3D.obj" \
	"$(INTDIR)\LockDataInfo.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SaveCompleteDlg.obj" \
	"$(INTDIR)\ShortcutPaneDrawing.obj" \
	"$(INTDIR)\ShortcutPaneInput.obj" \
	"$(INTDIR)\ShortcutPaneStart.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UbiARcBridgeData.obj" \
	"$(INTDIR)\UpdateModifyDlg.obj" \
	"$(INTDIR)\WaitForSubProgram.obj" \
	"$(INTDIR)\WmfSizeDlg.obj" \
	"$(INTDIR)\ARcBridge.res"

"..\CommonCompile\Output\ARcBridge.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=\Hangil_Project\ARcBridge\CommonCompile\Output\ARcBridge.exe
SOURCE="$(InputPath)"

"..\commonCompile\OutputD\ARcBridgeCopy.out" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy ..\Toolkit\XTP8610Lib.dll ..\CommonCompile\Output\XTP8610Lib.dll > ..\commonCompile\OutputD\ARcBridgeCopy.out
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridge.dep")
!INCLUDE "ARcBridge.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridge.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridge - Win32 Release" || "$(CFG)" == "ARcBridge - Win32 Debug" || "$(CFG)" == "ARcBridge - Win32 Release_Nong"
SOURCE=.\ARcBridge.cpp

"$(INTDIR)\ARcBridge.obj"	"$(INTDIR)\ARcBridge.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ARcBridge.rc

"$(INTDIR)\ARcBridge.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeDoc.cpp

"$(INTDIR)\ARcBridgeDoc.obj"	"$(INTDIR)\ARcBridgeDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ARcBridgeView.cpp

"$(INTDIR)\ARcBridgeView.obj"	"$(INTDIR)\ARcBridgeView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\CalcLineInfoDlg.cpp

"$(INTDIR)\CalcLineInfoDlg.obj"	"$(INTDIR)\CalcLineInfoDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\CalcStationDlg.cpp

"$(INTDIR)\CalcStationDlg.obj"	"$(INTDIR)\CalcStationDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\DlgOcc3D.cpp

"$(INTDIR)\DlgOcc3D.obj"	"$(INTDIR)\DlgOcc3D.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\LockDataInfo.cpp

"$(INTDIR)\LockDataInfo.obj"	"$(INTDIR)\LockDataInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\SaveCompleteDlg.cpp

"$(INTDIR)\SaveCompleteDlg.obj"	"$(INTDIR)\SaveCompleteDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ShortcutPaneDrawing.cpp

"$(INTDIR)\ShortcutPaneDrawing.obj"	"$(INTDIR)\ShortcutPaneDrawing.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ShortcutPaneInput.cpp

"$(INTDIR)\ShortcutPaneInput.obj"	"$(INTDIR)\ShortcutPaneInput.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ShortcutPaneStart.cpp

"$(INTDIR)\ShortcutPaneStart.obj"	"$(INTDIR)\ShortcutPaneStart.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\Splash.cpp

"$(INTDIR)\Splash.obj"	"$(INTDIR)\Splash.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WNT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ARcBridge.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WNT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ARcBridge.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Release_Nong"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WNT" /D "_NONG_GI_BAN_" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ARcBridge.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ARcBridge.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\UbiARcBridgeData.cpp

"$(INTDIR)\UbiARcBridgeData.obj"	"$(INTDIR)\UbiARcBridgeData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\UpdateModifyDlg.cpp

"$(INTDIR)\UpdateModifyDlg.obj"	"$(INTDIR)\UpdateModifyDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\WaitForSubProgram.cpp

"$(INTDIR)\WaitForSubProgram.obj"	"$(INTDIR)\WaitForSubProgram.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\WmfSizeDlg.cpp

"$(INTDIR)\WmfSizeDlg.obj"	"$(INTDIR)\WmfSizeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridge.pch" ".\ARcBridge.h"


SOURCE=.\ARcBridge.h

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

WkspDir=.
InputPath=.\ARcBridge.h

"..\commonCompile\lib\Out" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	echo Run WriteBuildDate.exe For Date Version....... 
	$(WkspDir)\WriteBuildDate.exe                     $(WkspDir)\  
	copy %HANGIL_ENGINE%\bin\DLL\HgExport.dll ..\commonCompile\Output\HgExport.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShareBridgeInfo.dll ..\commonCompile\Output\ShareBridgeInfo.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShareWithADManager.dll ..\commonCompile\Output\ShareWithADManager.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShGrid.dll ..\commonCompile\Output\ShGrid.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShControlBar.dll ..\commonCompile\Output\ShControlBar.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\Domyun.dll ..\commonCompile\Output\Domyun.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShImageMng.dll ..\commonCompile\Output\ShImageMng.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\LineApp.dll ..\commonCompile\Output\LineApp.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\DrawManage.dll ..\commonCompile\Output\DrawManage.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AException.dll ..\commonCompile\Output\AException.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ARoadConfig.dll ..\commonCompile\Output\ARoadConfig.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\XLReport.dll ..\commonCompile\Output\XLReport.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\XTP8610Lib.dll ..\commonCompile\Output\XTP8610Lib.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGGhostConc.dll ..\commonCompile\Output\HGGhostConc.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseConc.dll ..\commonCompile\Output\HGBaseConc.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseDesign.dll ..\commonCompile\Output\HGBaseDesign.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseDraw.dll ..\commonCompile\Output\HGBaseDraw.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AFem.dll ..\commonCompile\Output\AFem.dll  > ..\commonCompile\lib\out 
	copy ..\HgExcelBase.dll ..\commonCompile\Output\HgExcelBase.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\src\HgBaseDraw\HCAD_Symbol.dxf ..\HCAD_Symbol.dxf  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ALine.exe ..\commonCompile\Output\ALine.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ABorder.exe ..\commonCompile\Output\ABorder.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ADwgMaker.exe ..\commonCompile\Output\ADwgMaker.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ADwgMaker2004.exe ..\commonCompile\Output\ADwgMaker2004.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AEdit.exe ..\commonCompile\Output\AEdit.exe  > ..\commonCompile\lib\out 
	copy ..\AUpdate.exe ..\commonCompile\Output\AUpdate.exe > ..\commonCompile\lib\out 
	copy ..\UpdateSub.exe ..\commonCompile\Output\UpdateSub.exe > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\Sx32w.dll ..\commonCompile\Output\Sx32w.dll 
	copy %HANGIL_MODULE%\bin\DLL\MeshGen.dll ..\commonCompile\Output\MeshGen.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_MODULE%\bin\DLL\RCModule.dll ..\commonCompile\Output\RCModule.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_MODULE%\bin\DLL\HgBaseBM.dll ..\commonCompile\Output\HgBaseBM.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\CommonBridge.dll ..\commonCompile\Output\CommonBridge.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\BearingInfo.dll ..\commonCompile\Output\BearingInfo.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGDBControl.dll ..\commonCompile\Output\HGDBControl.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%3D\bin\DLL\*.dll ..\commonCompile\Output > ..\commonCompile\lib\out 
	copy %HANGIL_FRAME%\bin\DLL\HgOccView.dll ..\commonCompile\Output\HgOccView.dll  > ..\commonCompile\lib\out 
	copy %UBICON_ENGINE_TOTAL%\Ubicon_Engine\bin\release\*.dll ..\commonCompile\Output\*.dll  > ..\commonCompile\lib\out 
	copy %UBICON_ENGINE_TOTAL%\Ubicon_Occ\bin\release\*.dll ..\commonCompile\Output\*.dll  > ..\commonCompile\lib\out 
<< 
	

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

WkspDir=.
InputPath=.\ARcBridge.h

"..\commonCompile\libD\OutD" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	echo Run WriteBuildDate.exe For Date Version....... 
	$(WkspDir)\WriteBuildDate.exe                      $(WkspDir)\  
	copy ..\ARcBridge\res\work.ico ..\commonCompile\OutputD\work.ico > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HgExportD.dll ..\commonCompile\OutputD\HgExportD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ShareBridgeInfoD.dll ..\commonCompile\OutputD\ShareBridgeInfoD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ShareWithADManagerD.dll ..\commonCompile\OutputD\ShareWithADManagerD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ShGridD.dll ..\commonCompile\OutputD\ShGridD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ShControlBarD.dll ..\commonCompile\OutputD\ShControlBarD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\DomyunD.dll ..\commonCompile\OutputD\DomyunD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ShImageMngD.dll ..\commonCompile\OutputD\ShImageMngD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\LineAppD.dll ..\commonCompile\OutputD\LineAppD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\DrawManageD.dll ..\commonCompile\OutputD\DrawManageD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\AExceptionD.dll ..\commonCompile\OutputD\AExceptionD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\ARoadConfigD.dll ..\commonCompile\OutputD\ARoadConfigD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\XLReportD.dll ..\commonCompile\OutputD\XLReportD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\XTP8610LibD.dll ..\commonCompile\OutputD\XTP8610LibD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HGGhostConcD.dll ..\commonCompile\OutputD\HGGhostConcD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HGBaseConcD.dll ..\commonCompile\OutputD\HGBaseConcD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HGBaseDesignD.dll ..\commonCompile\OutputD\HGBaseDesignD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HGBaseDrawD.dll ..\commonCompile\OutputD\HGBaseDrawD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\AFemD.dll ..\commonCompile\OutputD\AFemD.dll  > ..\commonCompile\libD\outD 
	copy ..\HgExcelBase.dll ..\commonCompile\OutputD\HgExcelBase.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\src\HgBaseDraw\HCAD_Symbol.dxf ..\HCAD_Symbol.dxf  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\Sx32w.dll ..\commonCompile\OutputD\Sx32w.dll 
	copy %HANGIL_MODULE%\bin\DLLD\MeshGenD.dll ..\commonCompile\OutputD\MeshGenD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_MODULE%\bin\DLLD\RCModuleD.dll ..\commonCompile\OutputD\RCModuleD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_MODULE%\bin\DLLD\HgBaseBMD.dll ..\commonCompile\OutputD\HgBaseBMD.dll  > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\CommonBridgeD.dll ..\commonCompile\OutputD\CommonBridgeD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\BearingInfoD.dll ..\commonCompile\OutputD\BearingInfoD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%\bin\DLLD\HGDBControlD.dll ..\commonCompile\OutputD\HGDBControlD.dll > ..\commonCompile\libD\outD 
	copy %HANGIL_ENGINE%3D\bin\DLLD\*.dll ..\commonCompile\OutputD > ..\commonCompile\libD\outD 
	copy %HANGIL_FRAME%\bin\DLLD\HgOccViewD.dll ..\commonCompile\OutputD\HgOccViewD.dll  > ..\commonCompile\libD\outD 
	copy %UBICON_ENGINE_TOTAL%\Ubicon_Engine\bin\debug\*.dll ..\commonCompile\OutputD\*.dll  > ..\commonCompile\libD\outD 
	copy %UBICON_ENGINE_TOTAL%\Ubicon_Occ\bin\debug\*.dll ..\commonCompile\OutputD\*.dll  > ..\commonCompile\libD\outD 
<< 
	

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Release_Nong"

InputPath=.\ARcBridge.h

"..\commonCompile\lib\Out" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy %HANGIL_ENGINE%\bin\DLL\ShareBridgeInfo.dll ..\commonCompile\Output\ShareBridgeInfo.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShareWithADManager.dll ..\commonCompile\Output\ShareWithADManager.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShGrid.dll ..\commonCompile\Output\ShGrid.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShControlBar.dll ..\commonCompile\Output\ShControlBar.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\Domyun.dll ..\commonCompile\Output\Domyun.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ShImageMng.dll ..\commonCompile\Output\ShImageMng.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\LineApp.dll ..\commonCompile\Output\LineApp.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\DrawManage.dll ..\commonCompile\Output\DrawManage.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AException.dll ..\commonCompile\Output\AException.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ARoadConfig.dll ..\commonCompile\Output\ARoadConfig.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\XLReport.dll ..\commonCompile\Output\XLReport.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\XTP8610Lib.dll ..\commonCompile\Output\XTP8610Lib.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGGhostConc.dll ..\commonCompile\Output\HGGhostConc.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseConc.dll ..\commonCompile\Output\HGBaseConc.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseDesign.dll ..\commonCompile\Output\HGBaseDesign.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\HGBaseDraw.dll ..\commonCompile\Output\HGBaseDraw.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AFem.dll ..\commonCompile\Output\AFem.dll  > ..\commonCompile\lib\out 
	copy ..\HgExcelBase.dll ..\commonCompile\Output\HgExcelBase.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\src\HgBaseDraw\HCAD_Symbol.dxf ..\HCAD_Symbol.dxf  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ALine.exe ..\commonCompile\Output\ALine.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ABorder.exe ..\commonCompile\Output\ABorder.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ADwgMaker.exe ..\commonCompile\Output\ADwgMaker.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\ADwgMaker2004.exe ..\commonCompile\Output\ADwgMaker2004.exe  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\AEdit.exe ..\commonCompile\Output\AEdit.exe  > ..\commonCompile\lib\out 
	copy ..\AUpdate.exe ..\commonCompile\Output\AUpdate.exe > ..\commonCompile\lib\out 
	copy ..\UpdateSub.exe ..\commonCompile\Output\UpdateSub.exe > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\Sx32w.dll ..\commonCompile\Output\Sx32w.dll 
	copy %HANGIL_MODULE%\bin\DLL\RCModule.dll ..\commonCompile\Output\RCModule.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_MODULE%\bin\DLL\HgBaseBM.dll ..\commonCompile\Output\HgBaseBM.dll  > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\CommonBridge.dll ..\commonCompile\Output\CommonBridge.dll > ..\commonCompile\lib\out 
	copy %HANGIL_ENGINE%\bin\DLL\BearingInfo.dll ..\commonCompile\Output\BearingInfo.dll > ..\commonCompile\lib\out 
<< 
	

!ENDIF 

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

"ARcBridgeOutDesign - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutDesign"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutDesign.mak CFG="ARcBridgeOutDesign - Win32 Release" 
   cd "..\..\ARcBridge"

"ARcBridgeOutDesign - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutDesign"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutDesign.mak CFG="ARcBridgeOutDesign - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\ARcBridge"

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

"ARcBridgeOutDesign - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutDesign"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutDesign.mak CFG="ARcBridgeOutDesign - Win32 Debug" 
   cd "..\..\ARcBridge"

"ARcBridgeOutDesign - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutDesign"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutDesign.mak CFG="ARcBridgeOutDesign - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\ARcBridge"

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Release_Nong"

!ENDIF 

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

"ARcBridgeInput - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Release" 
   cd "..\..\ARcBridge"

"ARcBridgeInput - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\ARcBridge"

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

"ARcBridgeInput - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Debug" 
   cd "..\..\ARcBridge"

"ARcBridgeInput - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\ARcBridge"

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Release_Nong"

!ENDIF 


!ENDIF 

