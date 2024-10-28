# rickgl demo

A cross-compatible OpenGL demo

## Compile to linux x86_64
```sh
cmake -B build
```

## Cross-compile to Windows x86_64
```sh
# install tools
sudo apt install mingw-w64
# add `Windows.h` (hack)
sudo ln -s /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h
# build
cmake -DCMAKE_TOOLCHAIN_FILE=win64.cmake -B build
```

## credits
- https://learnopengl.com/
- https://github.com/SasLuca/GLFWCMakeTemplate
