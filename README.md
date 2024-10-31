# rickgl demo
A cross-compilable OpenGL demo for x86_64 Windows and GNU/Linux

## Compile to wasm webgl
```sh
# install emscripten
# (see https://emscripten.org/docs/getting_started/downloads.html or Dockerfile)
bash build-wasm.sh
```

## Compile to linux x86_64
```sh
bash build.sh
```

## Cross-compile to Windows x86_64
```sh
# install tools
sudo apt install mingw-w64
# add `Windows.h` (hack)
sudo ln -s /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h
# build
bash build-windows.sh
```

## credits
- https://learnopengl.com/
- https://www.mingw-w64.org/
- https://github.com/SasLuca/GLFWCMakeTemplate
- https://github.com/nothings/stb
- https://emscripten.org/
- https://github.com/adevaykin/minimal-cmake-emscripten-project
