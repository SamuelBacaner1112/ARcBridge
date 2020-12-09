pushd ..\..\..\Engine\BaseEng\Build
call BuildDebugV100.bat
popd

pushd ..\..\..\Engine\Bim3DEng\Build
call BuildDebugV100.bat
popd

pushd ..\..\..\Engine\FrameEng\Build
call BuildDebugV100.bat
popd

pushd ..\..\..\Engine\BaseComp\Build
call BuildDebugV100.bat
popd

pushd ..\..\..\Engine\HgModel3D\Build
call BuildDebugV100.bat
popd

pushd ..\..\..\Engine\HgFem3D\Build
call BuildDebugV100.bat

pushd ..\..\..\Product\ARcBridge_v5\Build
call BuildDebugV100.bat

popd
