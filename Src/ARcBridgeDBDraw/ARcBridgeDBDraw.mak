# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeDBDraw.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeDBDraw - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeDBDraw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeDBDraw - Win32 Release" && "$(CFG)" != "ARcBridgeDBDraw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeDBDraw.mak" CFG="ARcBridgeDBDraw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeDBDraw - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeDBDraw - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeDBDraw.dll"

!ELSE 

ALL : "ARcBridgeData - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeDBDraw.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeData - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDBDraw.obj"
	-@erase "$(INTDIR)\ARcBridgeDBDraw.pch"
	-@erase "$(INTDIR)\ARcBridgeDBDraw.res"
	-@erase "$(INTDIR)\ARcBridgeDBStandard.obj"
	-@erase "$(INTDIR)\DimParabola.obj"
	-@erase "$(INTDIR)\DrawColumn.obj"
	-@erase "$(INTDIR)\DrawCrossSection.obj"
	-@erase "$(INTDIR)\DrawExFooting.obj"
	-@erase "$(INTDIR)\DrawFooting.obj"
	-@erase "$(INTDIR)\DrawLine.obj"
	-@erase "$(INTDIR)\DrawPlane.obj"
	-@erase "$(INTDIR)\DrawVertSection.obj"
	-@erase "$(INTDIR)\DrawWall.obj"
	-@erase "$(INTDIR)\DrawWingWall.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDBDraw.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeDBDraw.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeDBDraw.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDBDraw.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDBDraw.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDBDraw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeData.lib RCModule.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeDBDraw.pdb" /machine:I386 /def:".\ARcBridgeDBDraw.def" /out:"../../CommonCompile/Output/ARcBridgeDBDraw.dll" /implib:"../../CommonCompile/lib/ARcBridgeDBDraw.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDBDraw.obj" \
	"$(INTDIR)\ARcBridgeDBStandard.obj" \
	"$(INTDIR)\DimParabola.obj" \
	"$(INTDIR)\DrawColumn.obj" \
	"$(INTDIR)\DrawCrossSection.obj" \
	"$(INTDIR)\DrawExFooting.obj" \
	"$(INTDIR)\DrawFooting.obj" \
	"$(INTDIR)\DrawLine.obj" \
	"$(INTDIR)\DrawPlane.obj" \
	"$(INTDIR)\DrawVertSection.obj" \
	"$(INTDIR)\DrawWall.obj" \
	"$(INTDIR)\DrawWingWall.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDBDraw.res" \
	"..\..\CommonCompile\lib\ARcBridgeData.lib"

"..\..\CommonCompile\Output\ARcBridgeDBDraw.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeDBDrawD.dll"

!ELSE 

ALL : "ARcBridgeData - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeDBDrawD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeData - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeDBDraw.obj"
	-@erase "$(INTDIR)\ARcBridgeDBDraw.pch"
	-@erase "$(INTDIR)\ARcBridgeDBDraw.res"
	-@erase "$(INTDIR)\ARcBridgeDBStandard.obj"
	-@erase "$(INTDIR)\DimParabola.obj"
	-@erase "$(INTDIR)\DrawColumn.obj"
	-@erase "$(INTDIR)\DrawCrossSection.obj"
	-@erase "$(INTDIR)\DrawExFooting.obj"
	-@erase "$(INTDIR)\DrawFooting.obj"
	-@erase "$(INTDIR)\DrawLine.obj"
	-@erase "$(INTDIR)\DrawPlane.obj"
	-@erase "$(INTDIR)\DrawVertSection.obj"
	-@erase "$(INTDIR)\DrawWall.obj"
	-@erase "$(INTDIR)\DrawWingWall.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeDBDrawD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDBDrawD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDBDrawD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDBDrawD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDBDrawD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDBDraw.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeDBDraw.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeDBDraw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeDataD.lib RCModuleD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeDBDrawD.pdb" /debug /machine:I386 /def:".\ARcBridgeDBDrawD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDBDrawD.dll" /implib:"../../CommonCompile/libD/ARcBridgeDBDrawD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeDBDraw.obj" \
	"$(INTDIR)\ARcBridgeDBStandard.obj" \
	"$(INTDIR)\DimParabola.obj" \
	"$(INTDIR)\DrawColumn.obj" \
	"$(INTDIR)\DrawCrossSection.obj" \
	"$(INTDIR)\DrawExFooting.obj" \
	"$(INTDIR)\DrawFooting.obj" \
	"$(INTDIR)\DrawLine.obj" \
	"$(INTDIR)\DrawPlane.obj" \
	"$(INTDIR)\DrawVertSection.obj" \
	"$(INTDIR)\DrawWall.obj" \
	"$(INTDIR)\DrawWingWall.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeDBDraw.res" \
	"..\..\CommonCompile\libD\ARcBridgeDataD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeDBDrawD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeDBDraw.dep")
!INCLUDE "ARcBridgeDBDraw.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeDBDraw.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeDBDraw - Win32 Release" || "$(CFG)" == "ARcBridgeDBDraw - Win32 Debug"
SOURCE=.\ARcBridgeDBDraw.cpp

"$(INTDIR)\ARcBridgeDBDraw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\ARcBridgeDBDraw.rc

"$(INTDIR)\ARcBridgeDBDraw.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeDBStandard.cpp

"$(INTDIR)\ARcBridgeDBStandard.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DimParabola.cpp

"$(INTDIR)\DimParabola.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawColumn.cpp

"$(INTDIR)\DrawColumn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawCrossSection.cpp

"$(INTDIR)\DrawCrossSection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawExFooting.cpp

"$(INTDIR)\DrawExFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawFooting.cpp

"$(INTDIR)\DrawFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawLine.cpp

"$(INTDIR)\DrawLine.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawPlane.cpp

"$(INTDIR)\DrawPlane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawVertSection.cpp

"$(INTDIR)\DrawVertSection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawWall.cpp

"$(INTDIR)\DrawWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\DrawWingWall.cpp

"$(INTDIR)\DrawWingWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeDBDraw.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDBDraw.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDBDraw.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeDBDraw.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeDBDraw.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Release"

"ARcBridgeData - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\InDLL\ARcBridgeData"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeData.mak CFG="ARcBridgeData - Win32 Release" 
   cd "..\ARcBridgeDBDraw"

"ARcBridgeData - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\InDLL\ARcBridgeData"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeData.mak CFG="ARcBridgeData - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeDBDraw"

!ELSEIF  "$(CFG)" == "ARcBridgeDBDraw - Win32 Debug"

"ARcBridgeData - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\InDLL\ARcBridgeData"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeData.mak CFG="ARcBridgeData - Win32 Debug" 
   cd "..\ARcBridgeDBDraw"

"ARcBridgeData - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\InDLL\ARcBridgeData"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeData.mak CFG="ARcBridgeData - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeDBDraw"

!ENDIF 


!ENDIF 

