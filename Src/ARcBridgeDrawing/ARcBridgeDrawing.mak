# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeDrawing.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeDrawing - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeDrawing - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeDrawing - Win32 Release" && "$(CFG)" != "ARcBridgeDrawing - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeDrawing.mak" CFG="ARcBridgeDrawing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeDrawing - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeDrawing - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDrawing - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeDrawing.dll"

!ELSE 

ALL : "ARcBridgeDrawingOption - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeDrawing.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDrawingOption - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDrawing.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawing.pch"
	-@erase "$(INTDIR)\ARcBridgeDrawing.res"
	-@erase "$(INTDIR)\ARcBridgeDrawingStd.obj"
	-@erase "$(INTDIR)\DrawingAps.obj"
	-@erase "$(INTDIR)\DrawingBoring.obj"
	-@erase "$(INTDIR)\DrawingCns.obj"
	-@erase "$(INTDIR)\DrawingGeneral.obj"
	-@erase "$(INTDIR)\DrawingOthers.obj"
	-@erase "$(INTDIR)\DrawingRebar.obj"
	-@erase "$(INTDIR)\DrawingShoeArrange.obj"
	-@erase "$(INTDIR)\DrawingSlabStandardOutput.obj"
	-@erase "$(INTDIR)\DrawingUser.obj"
	-@erase "$(INTDIR)\DrawingVerticalPlane.obj"
	-@erase "$(INTDIR)\PageInfo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDrawing.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDrawing.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeDrawing.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDrawing.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDrawing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeData.lib ARcBridgeDBDraw.lib DrawManage.lib ARcBridgeCalculate.lib RCModule.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeDrawing.pdb" /machine:I386 /def:".\ARcBridgeDrawing.def" /out:"../../CommonCompile/Output/ARcBridgeDrawing.dll" /implib:"../../CommonCompile/lib/ARcBridgeDrawing.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDrawing.obj" \
	"$(INTDIR)\ARcBridgeDrawingStd.obj" \
	"$(INTDIR)\DrawingAps.obj" \
	"$(INTDIR)\DrawingBoring.obj" \
	"$(INTDIR)\DrawingCns.obj" \
	"$(INTDIR)\DrawingGeneral.obj" \
	"$(INTDIR)\DrawingOthers.obj" \
	"$(INTDIR)\DrawingRebar.obj" \
	"$(INTDIR)\DrawingShoeArrange.obj" \
	"$(INTDIR)\DrawingSlabStandardOutput.obj" \
	"$(INTDIR)\DrawingUser.obj" \
	"$(INTDIR)\DrawingVerticalPlane.obj" \
	"$(INTDIR)\PageInfo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDrawing.res" \
	"..\..\CommonCompile\lib\ARcBridgeDrawingOption.lib"

"..\..\CommonCompile\Output\ARcBridgeDrawing.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeDrawing - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeDrawingD.dll"

!ELSE 

ALL : "ARcBridgeDrawingOption - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeDrawingD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDrawingOption - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDrawing.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawing.pch"
	-@erase "$(INTDIR)\ARcBridgeDrawing.res"
	-@erase "$(INTDIR)\ARcBridgeDrawingStd.obj"
	-@erase "$(INTDIR)\DrawingAps.obj"
	-@erase "$(INTDIR)\DrawingBoring.obj"
	-@erase "$(INTDIR)\DrawingCns.obj"
	-@erase "$(INTDIR)\DrawingGeneral.obj"
	-@erase "$(INTDIR)\DrawingOthers.obj"
	-@erase "$(INTDIR)\DrawingRebar.obj"
	-@erase "$(INTDIR)\DrawingShoeArrange.obj"
	-@erase "$(INTDIR)\DrawingSlabStandardOutput.obj"
	-@erase "$(INTDIR)\DrawingUser.obj"
	-@erase "$(INTDIR)\DrawingVerticalPlane.obj"
	-@erase "$(INTDIR)\PageInfo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeDrawingD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDrawingD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDrawingD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDrawingD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDrawingD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDrawing.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDrawing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeDataD.lib ARcBridgeDBDrawD.lib DrawManageD.lib ARcBridgeCalculateD.lib RCModuleD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeDrawingD.pdb" /debug /machine:I386 /def:".\ARcBridgeDrawingD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDrawingD.dll" /implib:"../../CommonCompile/libD/ARcBridgeDrawingD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDrawing.obj" \
	"$(INTDIR)\ARcBridgeDrawingStd.obj" \
	"$(INTDIR)\DrawingAps.obj" \
	"$(INTDIR)\DrawingBoring.obj" \
	"$(INTDIR)\DrawingCns.obj" \
	"$(INTDIR)\DrawingGeneral.obj" \
	"$(INTDIR)\DrawingOthers.obj" \
	"$(INTDIR)\DrawingRebar.obj" \
	"$(INTDIR)\DrawingShoeArrange.obj" \
	"$(INTDIR)\DrawingSlabStandardOutput.obj" \
	"$(INTDIR)\DrawingUser.obj" \
	"$(INTDIR)\DrawingVerticalPlane.obj" \
	"$(INTDIR)\PageInfo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDrawing.res" \
	"..\..\CommonCompile\libD\ARcBridgeDrawingOptionD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeDrawingD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeDrawing.dep")
!INCLUDE "ARcBridgeDrawing.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeDrawing.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeDrawing - Win32 Release" || "$(CFG)" == "ARcBridgeDrawing - Win32 Debug"
SOURCE=.\ARcBridgeDrawing.cpp

"$(INTDIR)\ARcBridgeDrawing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\ARcBridgeDrawing.rc

"$(INTDIR)\ARcBridgeDrawing.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeDrawingStd.cpp

"$(INTDIR)\ARcBridgeDrawingStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingAps.cpp

"$(INTDIR)\DrawingAps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingBoring.cpp

"$(INTDIR)\DrawingBoring.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingCns.cpp

"$(INTDIR)\DrawingCns.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingGeneral.cpp

"$(INTDIR)\DrawingGeneral.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingOthers.cpp

"$(INTDIR)\DrawingOthers.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingRebar.cpp

"$(INTDIR)\DrawingRebar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingShoeArrange.cpp

"$(INTDIR)\DrawingShoeArrange.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingSlabStandardOutput.cpp

"$(INTDIR)\DrawingSlabStandardOutput.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingUser.cpp

"$(INTDIR)\DrawingUser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\DrawingVerticalPlane.cpp

"$(INTDIR)\DrawingVerticalPlane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\PageInfo.cpp

"$(INTDIR)\PageInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawing.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeDrawing - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDrawing.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeDrawing - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDrawing.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDrawing - Win32 Release"

"ARcBridgeDrawingOption - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawingOption"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawingOption.mak CFG="ARcBridgeDrawingOption - Win32 Release" 
   cd "..\ARcBridgeDrawing"

"ARcBridgeDrawingOption - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawingOption"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawingOption.mak CFG="ARcBridgeDrawingOption - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeDrawing"

!ELSEIF  "$(CFG)" == "ARcBridgeDrawing - Win32 Debug"

"ARcBridgeDrawingOption - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawingOption"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawingOption.mak CFG="ARcBridgeDrawingOption - Win32 Debug" 
   cd "..\ARcBridgeDrawing"

"ARcBridgeDrawingOption - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeDrawingOption"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDrawingOption.mak CFG="ARcBridgeDrawingOption - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeDrawing"

!ENDIF 


!ENDIF 

