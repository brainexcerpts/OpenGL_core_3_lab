:: call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"

mkdir build
cd build
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat" 
cmake -G "NMake Makefiles JOM" -DCMAKE_BUILD_TYPE=Debug ..
jom.exe
pause