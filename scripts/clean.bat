SET PATH=%PATH%;C:\msys64\mingw64\bin\

cmake --build ./build --config Debug --target clean

rm -rf %~dp0..\build\
rm -rf %~dp0..\.cache\
rm -rf %~dp0..\Logs\
rm -rf %~dp0..\bin\Logs\
rm -rf %~dp0..\bin\SERVER.exe
rm -rf %~dp0..\bin\CONSOLE.exe
rm -rf %~dp0..\bin\GUI.exe