# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeOutDesign.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeOutDesign - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeOutDesign - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeOutDesign - Win32 Release" && "$(CFG)" != "ARcBridgeOutDesign - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeOutDesign.mak" CFG="ARcBridgeOutDesign - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeOutDesign - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeOutDesign - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeOutDesign.dll"

!ELSE 

ALL : "ARcBridgeOutBM - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeOutDesign.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeOutBM - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeOutDesign.obj"
	-@erase "$(INTDIR)\ARcBridgeOutDesign.pch"
	-@erase "$(INTDIR)\ARcBridgeOutDesign.res"
	-@erase "$(INTDIR)\ARcBridgeOutDesignStd.obj"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\OptionItem.obj"
	-@erase "$(INTDIR)\OutBridgeResister.obj"
	-@erase "$(INTDIR)\OutDesignBridge.obj"
	-@erase "$(INTDIR)\OutDesignDlg.obj"
	-@erase "$(INTDIR)\OutDesignOptionSheet.obj"
	-@erase "$(INTDIR)\OutDesignXLProPage_Etc.obj"
	-@erase "$(INTDIR)\OutDesignXLProPage_Exel.obj"
	-@erase "$(INTDIR)\OutDesignXLPropPage.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeOutDesign.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeOutDesign.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeOutDesign.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutDesign.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeOutDesign.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeOutDesign.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ACommon.lib ARoadConfig.lib HgGhostConc.lib ARcBridgeData.lib ARcBridgeDBDraw.lib ARcBridgeCalculate.lib XLReport.lib HgBaseBM.lib ARcBridgeOutBM.lib RCModule.lib BearingInfo.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeOutDesign.pdb" /machine:I386 /def:".\ARcBridgeOutDesign.def" /out:"../../CommonCompile/Output/ARcBridgeOutDesign.dll" /implib:"../../CommonCompile/lib/ARcBridgeOutDesign.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeOutDesign.obj" \
	"$(INTDIR)\ARcBridgeOutDesignStd.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\OptionItem.obj" \
	"$(INTDIR)\OutBridgeResister.obj" \
	"$(INTDIR)\OutDesignBridge.obj" \
	"$(INTDIR)\OutDesignDlg.obj" \
	"$(INTDIR)\OutDesignOptionSheet.obj" \
	"$(INTDIR)\OutDesignXLProPage_Etc.obj" \
	"$(INTDIR)\OutDesignXLProPage_Exel.obj" \
	"$(INTDIR)\OutDesignXLPropPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeOutDesign.res" \
	"..\..\CommonCompile\lib\ARcBridgeOutBM.lib"

"..\..\CommonCompile\Output\ARcBridgeOutDesign.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeOutDesignD.dll"

!ELSE 

ALL : "ARcBridgeOutBM - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeOutDesignD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeOutBM - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ARcBridgeOutDesign.obj"
	-@erase "$(INTDIR)\ARcBridgeOutDesign.pch"
	-@erase "$(INTDIR)\ARcBridgeOutDesign.res"
	-@erase "$(INTDIR)\ARcBridgeOutDesignStd.obj"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\OptionItem.obj"
	-@erase "$(INTDIR)\OutBridgeResister.obj"
	-@erase "$(INTDIR)\OutDesignBridge.obj"
	-@erase "$(INTDIR)\OutDesignDlg.obj"
	-@erase "$(INTDIR)\OutDesignOptionSheet.obj"
	-@erase "$(INTDIR)\OutDesignXLProPage_Etc.obj"
	-@erase "$(INTDIR)\OutDesignXLProPage_Exel.obj"
	-@erase "$(INTDIR)\OutDesignXLPropPage.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeOutDesignD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeOutDesignD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeOutDesignD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeOutDesignD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeOutDesignD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutDesign.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeOutDesign.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeOutDesign.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ACommonD.lib ARoadConfigD.lib HgGhostConcD.lib ARcBridgeDataD.lib ARcBridgeDBDrawD.lib ARcBridgeCalculateD.lib XLReportD.lib HgBaseBMD.lib ARcBridgeOutBMD.lib RCModuleD.lib BearingInfoD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeOutDesignD.pdb" /debug /machine:I386 /def:".\ARcBridgeOutDesignD.def" /out:"../../CommonCompile/OutputD/ARcBridgeOutDesignD.dll" /implib:"../../CommonCompile/libD/ARcBridgeOutDesignD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeOutDesign.obj" \
	"$(INTDIR)\ARcBridgeOutDesignStd.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\OptionItem.obj" \
	"$(INTDIR)\OutBridgeResister.obj" \
	"$(INTDIR)\OutDesignBridge.obj" \
	"$(INTDIR)\OutDesignDlg.obj" \
	"$(INTDIR)\OutDesignOptionSheet.obj" \
	"$(INTDIR)\OutDesignXLProPage_Etc.obj" \
	"$(INTDIR)\OutDesignXLProPage_Exel.obj" \
	"$(INTDIR)\OutDesignXLPropPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ARcBridgeOutDesign.res" \
	"..\..\CommonCompile\libD\ARcBridgeOutBMD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeOutDesignD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeOutDesign.dep")
!INCLUDE "ARcBridgeOutDesign.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeOutDesign.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeOutDesign - Win32 Release" || "$(CFG)" == "ARcBridgeOutDesign - Win32 Debug"
SOURCE=.\ARcBridgeOutDesign.cpp

"$(INTDIR)\ARcBridgeOutDesign.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\ARcBridgeOutDesign.rc

"$(INTDIR)\ARcBridgeOutDesign.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeOutDesignStd.cpp

"$(INTDIR)\ARcBridgeOutDesignStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\CheckComboBox.cpp

"$(INTDIR)\CheckComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OptionItem.cpp

"$(INTDIR)\OptionItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutBridgeResister.cpp

"$(INTDIR)\OutBridgeResister.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignBridge.cpp

"$(INTDIR)\OutDesignBridge.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignDlg.cpp

"$(INTDIR)\OutDesignDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignOptionSheet.cpp

"$(INTDIR)\OutDesignOptionSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignXLProPage_Etc.cpp

"$(INTDIR)\OutDesignXLProPage_Etc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignXLProPage_Exel.cpp

"$(INTDIR)\OutDesignXLProPage_Exel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\OutDesignXLPropPage.cpp

"$(INTDIR)\OutDesignXLPropPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeOutDesign.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutDesign.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeOutDesign.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeOutDesign.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeOutDesign.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Release"

"ARcBridgeOutBM - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutBM"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutBM.mak CFG="ARcBridgeOutBM - Win32 Release" 
   cd "..\ARcBridgeOutDesign"

"ARcBridgeOutBM - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutBM"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutBM.mak CFG="ARcBridgeOutBM - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeOutDesign"

!ELSEIF  "$(CFG)" == "ARcBridgeOutDesign - Win32 Debug"

"ARcBridgeOutBM - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutBM"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutBM.mak CFG="ARcBridgeOutBM - Win32 Debug" 
   cd "..\ARcBridgeOutDesign"

"ARcBridgeOutBM - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\OutDLL\ARcBridgeOutBM"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeOutBM.mak CFG="ARcBridgeOutBM - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeOutDesign"

!ENDIF 


!ENDIF 

