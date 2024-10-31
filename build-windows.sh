mkdir -p build
pushd build
cmake -DCMAKE_TOOLCHAIN_FILE=win64.cmake ../..
cmake --build . --clean-first
popd
