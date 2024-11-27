source ~/emsdk/emsdk_env.sh
mkdir -p build

pushd build
emcmake cmake ..
cmake --build .
popd

pushd webgl
cp ../build/index.js public
npm install && npm run dev
popd