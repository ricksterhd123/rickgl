source ~/emsdk/emsdk_env.sh
mkdir -p build
pushd build
emcmake cmake ..
cmake --build . --clean-first
popd