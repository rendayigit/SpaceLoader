./scripts/clean.sh
mkdir build
cd test
echo $PWD
cmake -S . -B ../build/
make