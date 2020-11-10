# CppND-Route-Planning-Project
Project 1 for Udacity Nanodegree C++ Programme

Building locally on Windows was a nightmare, so here are the steps I ended up following to get things working. Sorry that they're a bit unclear, I tidied it up after the event...

## Building IO2D
- Install ALL libraries from msys2.org package manager
  - Don't try and use the recommended VS Package manager
  - Including pkgconfiglite on the list
  - Had to add 'find_package(PNG REQUIRED)' to CMakeLists.txt to force it to find the PNG library. Wouldn't, otherwise, even when in a CMake searchable area.
  - If you try and build with cmake -G "MinGW Makefiles" --config Debug "-DCMAKE_BUILD_TYPE=Debug" ../. you will successfully generate the make files, but you will get string conversion errors (cannot convert 'const wchar_t* const' to 'LPCSTR' {aka 'const char*'}). This is a UNICODE issue
    - I ended up using the CMAKE gui to do this. Either way, you need to add '-UMBCS -D_UNICODE -DUNICODE' without quotes to CMAKE_CXX_FLAGS
  - Then run in the build folder: mingw32-make
  - I had issues with a sample.
    - One sample didn't build. Couldn't figure it out, and didn't really care, so just deleted the offending sample
    - Should work with something likeset(IO2D_WITHOUT_SAMPLES 1), set(IO2D_WITHOUT_TESTS 1), but didn't work
  - If you had that, delete it, run mingw32-make again
  - Then run: make install (copies IO2D libraries to cmake searchable area)

## Project
- Now create the make files for the actual project as as before (with mingw calls), but shouldn't have any weird library dependencies
  - Ended up using the GUI again to add the compiler flags.
- mingw32-make to make every time.
