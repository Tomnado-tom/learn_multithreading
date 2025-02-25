file(GLOB_RECURSE LIBRARY_SOURCES
    "library/*.cpp"
    "library/*.h"
)

# Define the library target
add_library(MyLibrary STATIC ${LIBRARY_SOURCES})

# Specify include directories
target_include_directories(MyLibrary 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor
)

set_target_properties(MyLibrary PROPERTIES FOLDER "Library")
create_source_groups(${CMAKE_CURRENT_SOURCE_DIR})
