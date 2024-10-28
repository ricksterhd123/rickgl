# Specify the target system as Windows
set(CMAKE_SYSTEM_NAME Windows)

# Set the compiler paths
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)

# Set the archiver and resource compiler
set(CMAKE_AR /usr/bin/x86_64-w64-mingw32-ar)
set(CMAKE_RC_COMPILER /usr/bin/x86_64-w64-mingw32-windres)

# Define the root path for finding headers and libraries
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Specify the search modes for the root path
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Optionally, specify if you want static libraries only
set(BUILD_SHARED_LIBS OFF)
