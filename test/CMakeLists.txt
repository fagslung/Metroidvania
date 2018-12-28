project(MetroidvaniaTests)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


add_subdirectory(lib/googletest-release-1.8.1)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(MetroidvaniaTest
        dynamicsTest.cpp
        geometryTest.cpp)

target_link_libraries(MetroidvaniaTest gtest gtest_main)