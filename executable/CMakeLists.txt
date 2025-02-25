project(MyCppProject)

set(CMAKE_CXX_STANDARD 11)

# Specify the source files for the executable
file(GLOB_RECURSE SOURCES
    "source/*.cpp"
    "source/*.h"
)

# Add the executable target
add_executable(MyExecutable ${SOURCES})

# Include directories for the executable
target_include_directories(MyExecutable 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/library
)

# Link the executable with the library
target_link_libraries(MyExecutable PRIVATE MyLibrary)

create_source_groups(${CMAKE_CURRENT_SOURCE_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyExecutable)

