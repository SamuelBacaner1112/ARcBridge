pushd ..\..\..\Engine\BaseEng\Build
call BuildReleaseRebuildV100.bat
popd

pushd ..\..\..\Engine\Bim3DEng\Build
call BuildReleaseRebuildV100.bat
popd

pushd ..\..\..\Engine\FrameEng\Build
call BuildReleaseRebuildV100.bat
popd

pushd ..\..\..\Engine\BaseComp\Build
call BuildReleaseRebuildV100.bat
popd


pushd ..\..\..\Engine\HgModel3D\Build
call BuildReleaseRebuildV100.bat
popd

pushd ..\..\..\Engine\HgStm\Build
call BuildReleaseRebuildV100.bat

popd