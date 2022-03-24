SET PATH=%PATH%;C:\msys64\mingw64\bin\

cmake --build ./build --config Debug --target clean

rm -rf "build/"
rm -rf ".cache/"
rm -rf "bin/"