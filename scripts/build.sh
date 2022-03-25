mkdir $(dirname $(readlink -f "$0"))/../build/

cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CC_COMPILER:FILEPATH=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-8 -S. -B/build -G "Unix Makefiles"

/home/pc/Qt/Tools/CMake/bin/cmake --no-warn-unused-cli -DCMAKE_PREFIX_PATH:STRING=/home/pc/Qt/5.15.2/gcc_64/lib/cmake/ -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CC_COMPILER:FILEPATH=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-8 -S$(dirname $(readlink -f "$0"))/../ -B$(dirname $(readlink -f "$0"))/../build -G "Unix Makefiles"

cmake --build ./build --config Debug --target all -j$((`nproc`+2)) --