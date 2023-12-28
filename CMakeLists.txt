cmake_minimum_required(VERSION 3.12)

# ------------------------------ Solution ------------------------------
project("Game")

# Set global properties
set(CMAKE_SUPPRESS_REGENERATION true)

set(EXTERNAL_FOLDER  "${CMAKE_SOURCE_DIR}/External")
set(SOURCE_FOLDER    "${CMAKE_SOURCE_DIR}/Source")
set(RESOURCES_FOLDER "${CMAKE_SOURCE_DIR}/Resources")
set(OUTPUT_FOLDER    "${CMAKE_SOURCE_DIR}/_output")

set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG}   /MDd")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${OUTPUT_FOLDER}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${OUTPUT_FOLDER}/bin")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# ------------------------------ Engine lib ------------------------------
set(ENGINE_NAME     "Engine")
set(ENGINE_SOURCE   "${SOURCE_FOLDER}/Engine")
file(GLOB_RECURSE _source_e_list CONFIGURE_DEPENDS "${ENGINE_SOURCE}/*")
file(GLOB_RECURSE _external_list CONFIGURE_DEPENDS "${EXTERNAL_FOLDER}/*.c*" "${EXTERNAL_FOLDER}/*.h*")

add_library(${ENGINE_NAME}
    ${_source_e_list}
    ${_external_list}
)
target_include_directories(${ENGINE_NAME} PRIVATE 
    "${EXTERNAL_FOLDER}/glm/"
    "${EXTERNAL_FOLDER}/glad/include/"
    "${EXTERNAL_FOLDER}/GLFW/include/"
    "${EXTERNAL_FOLDER}/stb/"
    "${EXTERNAL_FOLDER}/freetype/include/"
    "${EXTERNAL_FOLDER}/reactphysics3d/include/"
    "${EXTERNAL_FOLDER}/assimp/include/"
)
set_target_properties(${ENGINE_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ------------------------------ Application executable ------------------------------
set(APPLICATION_NAME    "Application")
set(APPLICATION_SOURCE  "${SOURCE_FOLDER}/Application")
file(GLOB_RECURSE _resource_list CONFIGURE_DEPENDS "${RESOURCES_FOLDER}/*")
file(GLOB_RECURSE _source_a_list CONFIGURE_DEPENDS "${APPLICATION_SOURCE}/*")

add_executable(${APPLICATION_NAME}
    ${_source_a_list}
    ${_resource_list}
)
target_include_directories(${APPLICATION_NAME} PRIVATE 
    "${SOURCE_FOLDER}/"

    "${EXTERNAL_FOLDER}/glm/"
    "${EXTERNAL_FOLDER}/glad/include/"
    "${EXTERNAL_FOLDER}/GLFW/include/"
    "${EXTERNAL_FOLDER}/stb/"
    "${EXTERNAL_FOLDER}/freetype/include/"
    "${EXTERNAL_FOLDER}/reactphysics3d/include/"
    "${EXTERNAL_FOLDER}/assimp/include/"
)
target_link_libraries(${APPLICATION_NAME}
    debug "${OUTPUT_FOLDER}/lib/debug/${ENGINE_NAME}.lib"
    debug "${EXTERNAL_FOLDER}/glfw/bin/Debug/glfw3.lib"
    debug "${EXTERNAL_FOLDER}/freetype/bin/Debug/freetype.lib"
    debug "${EXTERNAL_FOLDER}/reactphysics3d/bin/Debug/reactphysics3d.lib"
    debug "${EXTERNAL_FOLDER}/assimp/lib/Debug/assimp-vc143-mtd.lib"

    optimized "${OUTPUT_FOLDER}/lib/release/${ENGINE_NAME}.lib"
    optimized "${EXTERNAL_FOLDER}/glfw/bin/Release/glfw3.lib"
    optimized "${EXTERNAL_FOLDER}/freetype/bin/Release/freetype.lib"
    optimized "${EXTERNAL_FOLDER}/reactphysics3d/bin/Release/reactphysics3d.lib"
    optimized "${EXTERNAL_FOLDER}/assimp/lib/Release/assimp-vc143-mt.lib"
)
set_target_properties(${APPLICATION_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON

    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}

    LINK_FLAGS_DEBUG    "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE  "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)
add_dependencies(${APPLICATION_NAME} 
    ${ENGINE_NAME}
)

# ------------------------------ Tools executable ------------------------------
set(TOOLS_NAME     "Tools")
set(TOOLS_SOURCE   "${SOURCE_FOLDER}/Tools")
file(GLOB_RECURSE _source_t_list CONFIGURE_DEPENDS "${TOOLS_SOURCE}/*")

add_executable(${TOOLS_NAME}
    ${_source_t_list}
)
target_include_directories(${TOOLS_NAME} PRIVATE 
    "${SOURCE_FOLDER}/"

    "${EXTERNAL_FOLDER}/glm/"
    "${EXTERNAL_FOLDER}/glad/include/"
    "${EXTERNAL_FOLDER}/GLFW/include/"
    "${EXTERNAL_FOLDER}/stb/"
    "${EXTERNAL_FOLDER}/freetype/include/"
    "${EXTERNAL_FOLDER}/reactphysics3d/include/"
    "${EXTERNAL_FOLDER}/assimp/include/"
)
target_link_libraries(${TOOLS_NAME}    
    debug "${OUTPUT_FOLDER}/lib/debug/${ENGINE_NAME}.lib"
    debug "${EXTERNAL_FOLDER}/glfw/bin/Debug/glfw3.lib"
    debug "${EXTERNAL_FOLDER}/freetype/bin/Debug/freetype.lib"
    debug "${EXTERNAL_FOLDER}/reactphysics3d/bin/Debug/reactphysics3d.lib"
    debug "${EXTERNAL_FOLDER}/assimp/lib/Debug/assimp-vc143-mtd.lib"

    optimized "${OUTPUT_FOLDER}/lib/release/${ENGINE_NAME}.lib"
    optimized "${EXTERNAL_FOLDER}/glfw/bin/Release/glfw3.lib"
    optimized "${EXTERNAL_FOLDER}/freetype/bin/Release/freetype.lib"
    optimized "${EXTERNAL_FOLDER}/reactphysics3d/bin/Release/reactphysics3d.lib"
    optimized "${EXTERNAL_FOLDER}/assimp/lib/Release/assimp-vc143-mt.lib"
)
set_target_properties(${TOOLS_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON

    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}

    LINK_FLAGS_DEBUG    "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE  "/SUBSYSTEM:CONSOLE"
)
add_dependencies(${TOOLS_NAME} 
    ${ENGINE_NAME}
)

# ------------------------------ Global ------------------------------
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APPLICATION_NAME})

source_group(TREE ${TOOLS_SOURCE}       FILES ${_source_t_list})
source_group(TREE ${APPLICATION_SOURCE} FILES ${_source_a_list})
source_group(TREE ${ENGINE_SOURCE}      FILES ${_source_e_list})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_resource_list})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_external_list})
