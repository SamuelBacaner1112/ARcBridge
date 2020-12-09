# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeDrawingOption.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeDrawingOption - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeDrawingOption - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeDrawingOption - Win32 Release" && "$(CFG)" != "ARcBridgeDrawingOption - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeDrawingOption.mak" CFG="ARcBridgeDrawingOption - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeDrawingOption - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeDrawingOption - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeDrawingOption.dll"

!ELSE 

ALL : "ARcBridgeInput - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeDrawingOption.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeInput - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.pch"
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.res"
	-@erase "$(INTDIR)\ARcBridgeDrawingOptionData.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawingOptionStd.obj"
	-@erase "$(INTDIR)\DrawingOptionAps.obj"
	-@erase "$(INTDIR)\DrawingOptionBoring.obj"
	-@erase "$(INTDIR)\DrawingOptionCns.obj"
	-@erase "$(INTDIR)\DrawingOptionCommon.obj"
	-@erase "$(INTDIR)\DrawingOptionGeneral.obj"
	-@erase "$(INTDIR)\DrawingOptionHyunTa.obj"
	-@erase "$(INTDIR)\DrawingOptionOthers.obj"
	-@erase "$(INTDIR)\DrawingOptionRebar.obj"
	-@erase "$(INTDIR)\DrawingOptionShoeArrange.obj"
	-@erase "$(INTDIR)\DrawingOptionSlabStandardOutput.obj"
	-@erase "$(INTDIR)\DrawingOptionVerticalPlane.obj"
	-@erase "$(INTDIR)\OptionBase.obj"
	-@erase "$(INTDIR)\OptionSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDrawingOption.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDrawingOption.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeDrawingOption.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawingOption.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDrawingOption.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDrawingOption.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARoadConfig.lib ARcBridgeData.lib ARcBridgeDBDraw.lib DrawManage.lib HgGhostConc.lib RCModule.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeDrawingOption.pdb" /machine:I386 /def:".\ARcBridgeDrawingOption.def" /out:"../../CommonCompile/Output/ARcBridgeDrawingOption.dll" /implib:"../../CommonCompile/lib/ARcBridgeDrawingOption.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDrawingOption.obj" \
	"$(INTDIR)\ARcBridgeDrawingOptionData.obj" \
	"$(INTDIR)\ARcBridgeDrawingOptionStd.obj" \
	"$(INTDIR)\DrawingOptionAps.obj" \
	"$(INTDIR)\DrawingOptionBoring.obj" \
	"$(INTDIR)\DrawingOptionCns.obj" \
	"$(INTDIR)\DrawingOptionCommon.obj" \
	"$(INTDIR)\DrawingOptionGeneral.obj" \
	"$(INTDIR)\DrawingOptionHyunTa.obj" \
	"$(INTDIR)\DrawingOptionOthers.obj" \
	"$(INTDIR)\DrawingOptionRebar.obj" \
	"$(INTDIR)\DrawingOptionShoeArrange.obj" \
	"$(INTDIR)\DrawingOptionSlabStandardOutput.obj" \
	"$(INTDIR)\DrawingOptionVerticalPlane.obj" \
	"$(INTDIR)\OptionBase.obj" \
	"$(INTDIR)\OptionSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDrawingOption.res" \
	"..\..\CommonCompile\lib\ARcBridgeInput.lib"

"..\..\CommonCompile\Output\ARcBridgeDrawingOption.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeDrawingOptionD.dll"

!ELSE 

ALL : "ARcBridgeInput - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeDrawingOptionD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeInput - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.pch"
	-@erase "$(INTDIR)\ARcBridgeDrawingOption.res"
	-@erase "$(INTDIR)\ARcBridgeDrawingOptionData.obj"
	-@erase "$(INTDIR)\ARcBridgeDrawingOptionStd.obj"
	-@erase "$(INTDIR)\DrawingOptionAps.obj"
	-@erase "$(INTDIR)\DrawingOptionBoring.obj"
	-@erase "$(INTDIR)\DrawingOptionCns.obj"
	-@erase "$(INTDIR)\DrawingOptionCommon.obj"
	-@erase "$(INTDIR)\DrawingOptionGeneral.obj"
	-@erase "$(INTDIR)\DrawingOptionHyunTa.obj"
	-@erase "$(INTDIR)\DrawingOptionOthers.obj"
	-@erase "$(INTDIR)\DrawingOptionRebar.obj"
	-@erase "$(INTDIR)\DrawingOptionShoeArrange.obj"
	-@erase "$(INTDIR)\DrawingOptionSlabStandardOutput.obj"
	-@erase "$(INTDIR)\DrawingOptionVerticalPlane.obj"
	-@erase "$(INTDIR)\OptionBase.obj"
	-@erase "$(INTDIR)\OptionSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeDrawingOptionD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDrawingOptionD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDrawingOptionD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDrawingOptionD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDrawingOptionD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawingOption.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDrawingOption.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDrawingOption.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARoadConfigD.lib ARcBridgeDataD.lib ARcBridgeDBDrawD.lib DrawManageD.lib HgGhostConcD.lib RCModuleD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeDrawingOptionD.pdb" /debug /machine:I386 /def:".\ARcBridgeDrawingOptionD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDrawingOptionD.dll" /implib:"../../CommonCompile/libD/ARcBridgeDrawingOptionD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDrawingOption.obj" \
	"$(INTDIR)\ARcBridgeDrawingOptionData.obj" \
	"$(INTDIR)\ARcBridgeDrawingOptionStd.obj" \
	"$(INTDIR)\DrawingOptionAps.obj" \
	"$(INTDIR)\DrawingOptionBoring.obj" \
	"$(INTDIR)\DrawingOptionCns.obj" \
	"$(INTDIR)\DrawingOptionCommon.obj" \
	"$(INTDIR)\DrawingOptionGeneral.obj" \
	"$(INTDIR)\DrawingOptionHyunTa.obj" \
	"$(INTDIR)\DrawingOptionOthers.obj" \
	"$(INTDIR)\DrawingOptionRebar.obj" \
	"$(INTDIR)\DrawingOptionShoeArrange.obj" \
	"$(INTDIR)\DrawingOptionSlabStandardOutput.obj" \
	"$(INTDIR)\DrawingOptionVerticalPlane.obj" \
	"$(INTDIR)\OptionBase.obj" \
	"$(INTDIR)\OptionSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDrawingOption.res" \
	"..\..\CommonCompile\libD\ARcBridgeInputD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeDrawingOptionD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeDrawingOption.dep")
!INCLUDE "ARcBridgeDrawingOption.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeDrawingOption.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeDrawingOption - Win32 Release" || "$(CFG)" == "ARcBridgeDrawingOption - Win32 Debug"
SOURCE=.\ARcBridgeDrawingOption.cpp

"$(INTDIR)\ARcBridgeDrawingOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\ARcBridgeDrawingOption.rc

"$(INTDIR)\ARcBridgeDrawingOption.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeDrawingOptionData.cpp

"$(INTDIR)\ARcBridgeDrawingOptionData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\ARcBridgeDrawingOptionStd.cpp

"$(INTDIR)\ARcBridgeDrawingOptionStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionAps.cpp

"$(INTDIR)\DrawingOptionAps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionBoring.cpp

"$(INTDIR)\DrawingOptionBoring.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionCns.cpp

"$(INTDIR)\DrawingOptionCns.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionCommon.cpp

"$(INTDIR)\DrawingOptionCommon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionGeneral.cpp

"$(INTDIR)\DrawingOptionGeneral.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionHyunTa.cpp

"$(INTDIR)\DrawingOptionHyunTa.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionOthers.cpp

"$(INTDIR)\DrawingOptionOthers.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionRebar.cpp

"$(INTDIR)\DrawingOptionRebar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionShoeArrange.cpp

"$(INTDIR)\DrawingOptionShoeArrange.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionSlabStandardOutput.cpp

"$(INTDIR)\DrawingOptionSlabStandardOutput.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\DrawingOptionVerticalPlane.cpp

"$(INTDIR)\DrawingOptionVerticalPlane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\OptionBase.cpp

"$(INTDIR)\OptionBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\OptionSheet.cpp

"$(INTDIR)\OptionSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDrawingOption.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawingOption.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDrawingOption.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDrawingOption.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDrawingOption.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Release"

"ARcBridgeInput - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Release" 
   cd "..\..\OutDLL\ARcBridgeDrawingOption"

"ARcBridgeInput - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\OutDLL\ARcBridgeDrawingOption"

!ELSEIF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Debug"

"ARcBridgeInput - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Debug" 
   cd "..\..\OutDLL\ARcBridgeDrawingOption"

"ARcBridgeInput - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeInput"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeInput.mak CFG="ARcBridgeInput - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\OutDLL\ARcBridgeDrawingOption"

!ENDIF 


!ENDIF 

