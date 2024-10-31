mkdir -p build
pushd build
cmake ../..
cmake --build . --clean-first
popd
