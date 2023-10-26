set(CMAKE_SYSTEM_PROCESSOR x64)
find_program(CLANG-17 "clang-17")
if(CLANG-17)
  set(CMAKE_C_COMPILER clang-17)
  set(CMAKE_CXX_COMPILER clang++-17)
  set(CMAKE_CXX_FLAGS "-stdlib=libc++")
else()
  set(CMAKE_C_COMPILER /opt/clang-17.0.3/bin/clang)
  set(CMAKE_CXX_COMPILER /opt/clang-17.0.3/bin/clang++)
  set(CMAKE_CXX_FLAGS "-stdlib=libc++ -I/opt/clang-17.0.3/include/ -I/opt/clang-17.0.3/include/x86_64-unknown-linux-gnu/c++/v1")
endif()
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")
