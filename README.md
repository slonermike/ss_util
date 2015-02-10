# ss_util_test

This project contains game development goodies for 2D development.  CMake is recommended for building.

<h3>To add ss_util to your cmake project...</h3>

Add the following lines to your CMakeLists.txt file.

```
target_link_libraries([your_executable] ${CORELIBS} ss_util)
add_subdirectory(ss_util)
```