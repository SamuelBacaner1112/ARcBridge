call ../../../Build/BuildBaseV100.bat ../ARcBridge ARcBridge.vcxproj Clean Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeData/ARcBridgeData.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeCalculate/ARcBridgeCalculate.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeInput/ARcBridgeInput.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeDrawing/ARcBridgeDrawing.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeOutBM/ARcBridgeOutBM.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ../Src/ARcBridgeOutDesign/ARcBridgeOutDesign.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../ARcBridge ARcBridge.vcxproj // Release

REM code sign ////////////////////////////////////////////////
rem target directory 보관
SET TARGETDIR=%cd%/../CommonCompile/Output_v100

rem 코드사인 폴더로 이동
cd ../../../ToolInhouse/CodeSignTool/CertificateFile_pfx

rem 코드사인 실행
call codesign_all %TARGETDIR%
REM code sign end //////////////////////////////////////////////////
