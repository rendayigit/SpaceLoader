CALL ./scripts/clean.bat
cmake -E make_directory ./build
cmake --build ./build --config Debug --target all -j 18 --