SET PATH=%PATH%;C:\msys64\mingw64\bin\

cmake -E make_directory ./build

@REM TODO - This command doesn't work
cmake -DCMAKE_PREFIX_PATH:STRING=C:/msys64/mingw64/lib/cmake/ -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CC_COMPILER:FILEPATH=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:/msys64/mingw64/bin/g++.exe -S. -B/build -G "MinGW Makefiles"

set /a coreCount=%NUMBER_OF_PROCESSORS% + 2
cmake --build ./build --config Debug --target all -j %coreCount% --