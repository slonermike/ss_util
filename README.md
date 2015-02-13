# ss_util

This project contains game development goodies for 2D development.  CMake is recommended for building.

For an example of how this works, see...
https://github.com/slonermike/ss_util_test

<h3>To add ss_util to your cmake project...</h3>

Add the following lines to your CMakeLists.txt file.

```
target_link_libraries([your_executable] ${CORELIBS} ss_util)
add_subdirectory(ss_util)
```
