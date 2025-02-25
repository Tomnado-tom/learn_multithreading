# Suppress developer warnings
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)

FetchContent_MakeAvailable(googletest)

set_target_properties(gmock PROPERTIES FOLDER "Vendor/Test")
set_target_properties(gmock_main PROPERTIES FOLDER "Vendor/Test")
set_target_properties(gtest PROPERTIES FOLDER "Vendor/Test")
set_target_properties(gtest_main PROPERTIES FOLDER "Vendor/Test")

#if add test submodules to this folder add them with
#add_subdirectory(googletest)