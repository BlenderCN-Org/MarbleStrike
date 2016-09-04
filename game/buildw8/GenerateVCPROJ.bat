pushd %CD%
cd ..\
set rootDirectory=%CD%
popd
..\tools\vcprojgenerator\vcprojgenerator.exe win8.xml %rootDirectory%\ %CD%\enginewin8.vcxproj