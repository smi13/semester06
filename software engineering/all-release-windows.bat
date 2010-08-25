if "%ProgramFiles(x86)%" == "" goto L1
call "%ProgramFiles(x86)%\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
goto L2
:L1
call "%ProgramFiles%\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
:L2

call "D:\Programs\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

devenv crulertest/crulertest.sln /rebuild "Release|Win32"