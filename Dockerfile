FROM ubuntu:latest

RUN apt update
RUN apt install -y build-essential
RUN apt install -y xorg-dev
RUN apt install -y libwayland-dev
RUN apt install -y cmake
RUN apt install -y nodejs npm
RUN apt install -y git
RUN apt install -y libxkbcommon-dev
RUN apt install -y mingw-w64
RUN ln -s /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h
RUN git clone https://github.com/emscripten-core/emsdk.git /root/emsdk

WORKDIR /root/emsdk
RUN bash emsdk install latest
RUN bash emsdk activate latest

WORKDIR /rickgl
COPY . .
