SET PATH=%PATH%;C:\msys64\mingw64\bin\
CALL ./scripts/clean.bat
cmake -E make_directory ./build
cmake --build ./build --config Debug --target all -j 18 --