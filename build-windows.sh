cmake -DCMAKE_TOOLCHAIN_FILE=win64.cmake -B build
pushd build
cmake --build .
popd
