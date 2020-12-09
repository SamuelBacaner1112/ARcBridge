# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeCalculate.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeCalculate - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeCalculate - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeCalculate - Win32 Release" && "$(CFG)" != "ARcBridgeCalculate - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeCalculate.mak" CFG="ARcBridgeCalculate - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeCalculate - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeCalculate - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeCalculate - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeCalculate.dll"

!ELSE 

ALL : "ARcBridgeDBDraw - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeCalculate.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDBDraw - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeCalcAps.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcArchRib.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcColAndGir.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcCornerAndSupport.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcEarthQuake.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcFatigue.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcFooting.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcLoad.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcStd.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcSummary.obj"
	-@erase "$(INTDIR)\ARcBridgeCalculate.obj"
	-@erase "$(INTDIR)\ARcBridgeCalculate.pch"
	-@erase "$(INTDIR)\ARcBridgeCalculate.res"
	-@erase "$(INTDIR)\ARcBridgeCalcWing.obj"
	-@erase "$(INTDIR)\ARcBridgeDesignFrame.obj"
	-@erase "$(INTDIR)\ARcBridgeDesignShell.obj"
	-@erase "$(INTDIR)\ARcBridgeMeshCalc.obj"
	-@erase "$(INTDIR)\GirderAndColumn.obj"
	-@erase "$(INTDIR)\LoadFooting.obj"
	-@erase "$(INTDIR)\MeshFace.obj"
	-@erase "$(INTDIR)\RigidityFooting.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeCalculate.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeCalculate.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeCalculate.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeCalculate.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeCalculate.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeCalculate.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeData.lib ARcBridgeDBDraw.lib HgBaseDraw.lib HgBaseBM.lib HgBaseDesign.lib RCModule.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeCalculate.pdb" /machine:I386 /def:".\ARcBridgeCalculate.def" /out:"../../CommonCompile/Output/ARcBridgeCalculate.dll" /implib:"../../CommonCompile/lib/ARcBridgeCalculate.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeCalcAps.obj" \
	"$(INTDIR)\ARcBridgeCalcArchRib.obj" \
	"$(INTDIR)\ARcBridgeCalcColAndGir.obj" \
	"$(INTDIR)\ARcBridgeCalcCornerAndSupport.obj" \
	"$(INTDIR)\ARcBridgeCalcEarthQuake.obj" \
	"$(INTDIR)\ARcBridgeCalcFatigue.obj" \
	"$(INTDIR)\ARcBridgeCalcFooting.obj" \
	"$(INTDIR)\ARcBridgeCalcLoad.obj" \
	"$(INTDIR)\ARcBridgeCalcStd.obj" \
	"$(INTDIR)\ARcBridgeCalcSummary.obj" \
	"$(INTDIR)\ARcBridgeCalculate.obj" \
	"$(INTDIR)\ARcBridgeCalcWing.obj" \
	"$(INTDIR)\ARcBridgeDesignFrame.obj" \
	"$(INTDIR)\ARcBridgeDesignShell.obj" \
	"$(INTDIR)\ARcBridgeMeshCalc.obj" \
	"$(INTDIR)\GirderAndColumn.obj" \
	"$(INTDIR)\LoadFooting.obj" \
	"$(INTDIR)\MeshFace.obj" \
	"$(INTDIR)\RigidityFooting.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeCalculate.res" \
	"..\..\CommonCompile\lib\ARcBridgeDBDraw.lib"

"..\..\CommonCompile\Output\ARcBridgeCalculate.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeCalculate - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeCalculateD.dll"

!ELSE 

ALL : "ARcBridgeDBDraw - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeCalculateD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeDBDraw - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeCalcAps.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcArchRib.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcColAndGir.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcCornerAndSupport.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcEarthQuake.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcFatigue.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcFooting.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcLoad.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcStd.obj"
	-@erase "$(INTDIR)\ARcBridgeCalcSummary.obj"
	-@erase "$(INTDIR)\ARcBridgeCalculate.obj"
	-@erase "$(INTDIR)\ARcBridgeCalculate.pch"
	-@erase "$(INTDIR)\ARcBridgeCalculate.res"
	-@erase "$(INTDIR)\ARcBridgeCalcWing.obj"
	-@erase "$(INTDIR)\ARcBridgeDesignFrame.obj"
	-@erase "$(INTDIR)\ARcBridgeDesignShell.obj"
	-@erase "$(INTDIR)\ARcBridgeMeshCalc.obj"
	-@erase "$(INTDIR)\GirderAndColumn.obj"
	-@erase "$(INTDIR)\LoadFooting.obj"
	-@erase "$(INTDIR)\MeshFace.obj"
	-@erase "$(INTDIR)\RigidityFooting.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeCalculateD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeCalculateD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeCalculateD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeCalculateD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeCalculateD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeCalculate.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeCalculate.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeCalculate.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ARcBridgeDataD.lib ARcBridgeDBDrawD.lib HgBaseDrawD.lib HgBaseBMD.lib HgBaseDesignD.lib RCModuleD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeCalculateD.pdb" /debug /machine:I386 /def:".\ARcBridgeCalculateD.def" /out:"../../CommonCompile/OutputD/ARcBridgeCalculateD.dll" /implib:"../../CommonCompile/libD/ARcBridgeCalculateD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeCalcAps.obj" \
	"$(INTDIR)\ARcBridgeCalcArchRib.obj" \
	"$(INTDIR)\ARcBridgeCalcColAndGir.obj" \
	"$(INTDIR)\ARcBridgeCalcCornerAndSupport.obj" \
	"$(INTDIR)\ARcBridgeCalcEarthQuake.obj" \
	"$(INTDIR)\ARcBridgeCalcFatigue.obj" \
	"$(INTDIR)\ARcBridgeCalcFooting.obj" \
	"$(INTDIR)\ARcBridgeCalcLoad.obj" \
	"$(INTDIR)\ARcBridgeCalcStd.obj" \
	"$(INTDIR)\ARcBridgeCalcSummary.obj" \
	"$(INTDIR)\ARcBridgeCalculate.obj" \
	"$(INTDIR)\ARcBridgeCalcWing.obj" \
	"$(INTDIR)\ARcBridgeDesignFrame.obj" \
	"$(INTDIR)\ARcBridgeDesignShell.obj" \
	"$(INTDIR)\ARcBridgeMeshCalc.obj" \
	"$(INTDIR)\GirderAndColumn.obj" \
	"$(INTDIR)\LoadFooting.obj" \
	"$(INTDIR)\MeshFace.obj" \
	"$(INTDIR)\RigidityFooting.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeCalculate.res" \
	"..\..\CommonCompile\libD\ARcBridgeDBDrawD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeCalculateD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeCalculate.dep")
!INCLUDE "ARcBridgeCalculate.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeCalculate.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeCalculate - Win32 Release" || "$(CFG)" == "ARcBridgeCalculate - Win32 Debug"
SOURCE=.\ARcBridgeCalcAps.cpp

"$(INTDIR)\ARcBridgeCalcAps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcArchRib.cpp

"$(INTDIR)\ARcBridgeCalcArchRib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcColAndGir.cpp

"$(INTDIR)\ARcBridgeCalcColAndGir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcCornerAndSupport.cpp

"$(INTDIR)\ARcBridgeCalcCornerAndSupport.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcEarthQuake.cpp

"$(INTDIR)\ARcBridgeCalcEarthQuake.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcFatigue.cpp

"$(INTDIR)\ARcBridgeCalcFatigue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcFooting.cpp

"$(INTDIR)\ARcBridgeCalcFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcLoad.cpp

"$(INTDIR)\ARcBridgeCalcLoad.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcStd.cpp

"$(INTDIR)\ARcBridgeCalcStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalcSummary.cpp

"$(INTDIR)\ARcBridgeCalcSummary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalculate.cpp

"$(INTDIR)\ARcBridgeCalculate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeCalculate.rc

"$(INTDIR)\ARcBridgeCalculate.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeCalcWing.cpp

"$(INTDIR)\ARcBridgeCalcWing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeDesignFrame.cpp

"$(INTDIR)\ARcBridgeDesignFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeDesignShell.cpp

"$(INTDIR)\ARcBridgeDesignShell.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\ARcBridgeMeshCalc.cpp

"$(INTDIR)\ARcBridgeMeshCalc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\GirderAndColumn.cpp

"$(INTDIR)\GirderAndColumn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\LoadFooting.cpp

"$(INTDIR)\LoadFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\MeshFace.cpp

"$(INTDIR)\MeshFace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\RigidityFooting.cpp

"$(INTDIR)\RigidityFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeCalculate.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeCalculate - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeCalculate.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeCalculate.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeCalculate - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeCalculate.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeCalculate.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeCalculate - Win32 Release"

"ARcBridgeDBDraw - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeDBDraw"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDBDraw.mak CFG="ARcBridgeDBDraw - Win32 Release" 
   cd "..\ARcBridgeCalculate"

"ARcBridgeDBDraw - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeDBDraw"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDBDraw.mak CFG="ARcBridgeDBDraw - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeCalculate"

!ELSEIF  "$(CFG)" == "ARcBridgeCalculate - Win32 Debug"

"ARcBridgeDBDraw - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeDBDraw"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDBDraw.mak CFG="ARcBridgeDBDraw - Win32 Debug" 
   cd "..\ARcBridgeCalculate"

"ARcBridgeDBDraw - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeDBDraw"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeDBDraw.mak CFG="ARcBridgeDBDraw - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeCalculate"

!ENDIF 


!ENDIF 

