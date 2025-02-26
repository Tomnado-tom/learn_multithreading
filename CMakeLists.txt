cmake_minimum_required(VERSION 3.23)
#function to create source groups in the IDE
function(create_source_groups root_folder)
    file(GLOB_RECURSE all_files
        LIST_DIRECTORIES false
        RELATIVE ${root_folder}
        ${root_folder}/*
    )
    foreach(file_path ${all_files})
        get_filename_component(file_dir "${file_path}" PATH)
        string(REPLACE "/" "\\" file_dir "${file_dir}")
        source_group("${file_dir}" FILES "${root_folder}/${file_path}")
    endforeach()
endfunction()

# Function to include all subdirectories of a given folder
function(include_all_subdirectories folder)
    file(GLOB subdirs LIST_DIRECTORIES true ${folder}/*)
    foreach(subdir ${subdirs})
        if(IS_DIRECTORY ${subdir})
            include_directories(${subdir})
            include_all_subdirectories(${subdir})
        endif()
    endforeach()
endfunction()

# Enable folder structure in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(my_cpp_project)


# Include subdirectories
add_subdirectory(vendor)
add_subdirectory(library)
add_subdirectory(executable)

# Uncomment the line below to include tests
add_subdirectory(tests)

# Set the startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyExecutable)
