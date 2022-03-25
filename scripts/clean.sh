cmake --build ./build --config Debug --target clean

rm -rf $(dirname $(readlink -f "$0"))/../build
rm -rf $(dirname $(readlink -f "$0"))/../.cache
rm -rf $(dirname $(readlink -f "$0"))/../bin
rm -rf $(dirname $(readlink -f "$0"))/../Logs