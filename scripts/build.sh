mkdir build

cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CC_COMPILER:FILEPATH=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-8 -S. -B/build -G "Unix Makefiles"

cmake --build ./build --config Debug --target all -j$((`nproc`+2)) --