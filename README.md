# rickgl demo
A cross-compilable OpenGL demo for x86_64 Windows and GNU/Linux and wasm webGL

https://rickgl.vercel.app/

## Compile for webgl
```sh
# install emscripten
# (see https://emscripten.org/docs/getting_started/downloads.html or Dockerfile)
bash build-wasm.sh
cd build
python3 -m http.server
# go to http://127.0.0.1:8000/rickgl.html
```

## Compile for linux x86_64
```sh
bash build.sh
```

## Compile for Windows x86_64
```sh
# install tools
sudo apt install mingw-w64
# add `Windows.h` (hack)
sudo ln -s /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h
# build
bash build-windows.sh
```

## Credits
- https://learnopengl.com/
- https://www.mingw-w64.org/
- https://github.com/SasLuca/GLFWCMakeTemplate
- https://github.com/nothings/stb
- https://emscripten.org/
- https://github.com/adevaykin/minimal-cmake-emscripten-project
