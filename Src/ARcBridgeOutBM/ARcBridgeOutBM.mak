# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeOutBM.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeOutBM - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeOutBM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeOutBM - Win32 Release" && "$(CFG)" != "ARcBridgeOutBM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeOutBM.mak" CFG="ARcBridgeOutBM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeOutBM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeOutBM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeOutBM - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeOutBM.dll"

!ELSE 

ALL : "ARcBridgeDrawing - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeOutBM.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDrawing - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeOutBM.obj"
	-@erase "$(INTDIR)\ARcBridgeOutBM.pch"
	-@erase "$(INTDIR)\ARcBridgeOutBM.res"
	-@erase "$(INTDIR)\ARcBridgeOutBMStd.obj"
	-@erase "$(INTDIR)\DlgOptionBMPage.obj"
	-@erase "$(INTDIR)\DlgOptionBMSheet.obj"
	-@erase "$(INTDIR)\DlgSelectBMPage.obj"
	-@erase "$(INTDIR)\DlgViewBMPage.obj"
	-@erase "$(INTDIR)\OutBM.obj"
	-@erase "$(INTDIR)\OutBMDlg.obj"
	-@erase "$(INTDIR)\OutBMTogong.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeOutBM.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeOutBM.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeOutBM.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutBM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeOutBM.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeOutBM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ACommon.lib HgBaseConc.lib HgGhostConc.lib ARoadConfig.lib ARcBridgeData.lib ARcBridgeDBDraw.lib ARcBridgeCalculate.lib HgGhostConc.lib XLReport.lib HgBaseBM.lib ARcBridgeInput.lib RCModule.lib BearingInfo.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeOutBM.pdb" /machine:I386 /def:".\ARcBridgeOutBM.def" /out:"../../CommonCompile/Output/ARcBridgeOutBM.dll" /implib:"../../CommonCompile/lib/ARcBridgeOutBM.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeOutBM.obj" \
	"$(INTDIR)\ARcBridgeOutBMStd.obj" \
	"$(INTDIR)\DlgOptionBMPage.obj" \
	"$(INTDIR)\DlgOptionBMSheet.obj" \
	"$(INTDIR)\DlgSelectBMPage.obj" \
	"$(INTDIR)\DlgViewBMPage.obj" \
	"$(INTDIR)\OutBM.obj" \
	"$(INTDIR)\OutBMDlg.obj" \
	"$(INTDIR)\OutBMTogong.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeOutBM.res" \
	"..\..\CommonCompile\lib\ARcBridgeDrawing.lib"

"..\..\CommonCompile\Output\ARcBridgeOutBM.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeOutBM - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeOutBMD.dll"

!ELSE 

ALL : "ARcBridgeDrawing - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeOutBMD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDrawing - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeOutBM.obj"
	-@erase "$(INTDIR)\ARcBridgeOutBM.pch"
	-@erase "$(INTDIR)\ARcBridgeOutBM.res"
	-@erase "$(INTDIR)\ARcBridgeOutBMStd.obj"
	-@erase "$(INTDIR)\DlgOptionBMPage.obj"
	-@erase "$(INTDIR)\DlgOptionBMSheet.obj"
	-@erase "$(INTDIR)\DlgSelectBMPage.obj"
	-@erase "$(INTDIR)\DlgViewBMPage.obj"
	-@erase "$(INTDIR)\OutBM.obj"
	-@erase "$(INTDIR)\OutBMDlg.obj"
	-@erase "$(INTDIR)\OutBMTogong.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeOutBMD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeOutBMD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeOutBMD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeOutBMD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeOutBMD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutBM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeOutBM.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeOutBM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ACommonD.lib HgBaseConcD.lib HgGhostConcD.lib ARoadConfigD.lib ARcBridgeDataD.lib ARcBridgeDBDrawD.lib ARcBridgeCalculateD.lib HgGhostConcD.lib XLReportD.lib HgBaseBMD.lib ARcBridgeInputD.lib RCModuleD.lib BearingInfoD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeOutBMD.pdb" /debug /machine:I386 /def:".\ARcBridgeOutBMD.def" /out:"../../CommonCompile/OutputD/ARcBridgeOutBMD.dll" /implib:"../../CommonCompile/libD/ARcBridgeOutBMD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeOutBM.obj" \
	"$(INTDIR)\ARcBridgeOutBMStd.obj" \
	"$(INTDIR)\DlgOptionBMPage.obj" \
	"$(INTDIR)\DlgOptionBMSheet.obj" \
	"$(INTDIR)\DlgSelectBMPage.obj" \
	"$(INTDIR)\DlgViewBMPage.obj" \
	"$(INTDIR)\OutBM.obj" \
	"$(INTDIR)\OutBMDlg.obj" \
	"$(INTDIR)\OutBMTogong.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeOutBM.res" \
	"..\..\CommonCompile\libD\ARcBridgeDrawingD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeOutBMD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeOutBM.dep")
!INCLUDE "ARcBridgeOutBM.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeOutBM.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeOutBM - Win32 Release" || "$(CFG)" == "ARcBridgeOutBM - Win32 Debug"
SOURCE=.\ARcBridgeOutBM.cpp

"$(INTDIR)\ARcBridgeOutBM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\ARcBridgeOutBM.rc

"$(INTDIR)\ARcBridgeOutBM.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeOutBMStd.cpp

"$(INTDIR)\ARcBridgeOutBMStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\DlgOptionBMPage.cpp

"$(INTDIR)\DlgOptionBMPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\DlgOptionBMSheet.cpp

"$(INTDIR)\DlgOptionBMSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\DlgSelectBMPage.cpp

"$(INTDIR)\DlgSelectBMPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\DlgViewBMPage.cpp

"$(INTDIR)\DlgViewBMPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\OutBM.cpp

"$(INTDIR)\OutBM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\OutBMDlg.cpp

"$(INTDIR)\OutBMDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\OutBMTogong.cpp

"$(INTDIR)\OutBMTogong.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutBM.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeOutBM - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutBM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeOutBM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeOutBM - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutBM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeOutBM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeOutBM - Win32 Release"

"ARcBridgeDrawing - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawing"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawing.mak CFG="ARcBridgeDrawing - Win32 Release" 
   cd "..\ARcBridgeOutBM"

"ARcBridgeDrawing - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawing"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawing.mak CFG="ARcBridgeDrawing - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeOutBM"

!ELSEIF  "$(CFG)" == "ARcBridgeOutBM - Win32 Debug"

"ARcBridgeDrawing - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawing"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawing.mak CFG="ARcBridgeDrawing - Win32 Debug" 
   cd "..\ARcBridgeOutBM"

"ARcBridgeDrawing - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawing"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawing.mak CFG="ARcBridgeDrawing - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeOutBM"

!ENDIF 


!ENDIF 

