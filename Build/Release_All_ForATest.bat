pushd ..\..\..\Engine\BaseEng\Build
call BuildReleaseV100.bat
popd

pushd ..\..\..\Engine\Bim3DEng\Build
call BuildReleaseRebuildV100.bat
popd

pushd ..\..\..\Engine\FrameEng\Build
call BuildReleaseV100.bat
popd

pushd ..\..\..\Engine\BaseComp\Build
call BuildReleaseV100.bat
popd


pushd ..\..\..\Engine\HgModel3D\Build
call BuildReleaseV100.bat
popd

pushd ..\..\..\Engine\HgStm\Build
call BuildReleaseV100.bat

pushd ..\..\..\Product\ARcBridge_v5\Build
call BuildReleaseV100.bat

popd