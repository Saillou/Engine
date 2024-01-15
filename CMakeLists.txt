cmake_minimum_required(VERSION 3.12)

# ------------------------------ Solution ------------------------------
project("Game")

# Set global properties
set(CMAKE_SUPPRESS_REGENERATION true)

set(EXTERNAL_FOLDER  "${CMAKE_SOURCE_DIR}/External")
set(SOURCE_FOLDER    "${CMAKE_SOURCE_DIR}/Source")
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

# ------------------------------ Sample executable ------------------------------
set(SAMPLE_NAME    "Sample")
set(SAMPLE_SOURCE  "${SOURCE_FOLDER}/Sample")
file(GLOB_RECURSE _source_s_list CONFIGURE_DEPENDS "${SAMPLE_SOURCE}/*")

add_executable(${SAMPLE_NAME}
    ${_source_s_list}
)
target_include_directories(${SAMPLE_NAME} PRIVATE 
    "${SOURCE_FOLDER}/"

    "${EXTERNAL_FOLDER}/glm/"
    "${EXTERNAL_FOLDER}/glad/include/"
    "${EXTERNAL_FOLDER}/GLFW/include/"
    "${EXTERNAL_FOLDER}/stb/"
    "${EXTERNAL_FOLDER}/freetype/include/"
    "${EXTERNAL_FOLDER}/reactphysics3d/include/"
    "${EXTERNAL_FOLDER}/assimp/include/"
)
target_link_libraries(${SAMPLE_NAME}
    debug "${OUTPUT_FOLDER}/lib/debug/${ENGINE_NAME}.lib"
    debug "${EXTERNAL_FOLDER}/glfw/bin/Debug/glfw3.lib"
    debug "${EXTERNAL_FOLDER}/freetype/bin/Debug/freetype.lib"
    debug "${EXTERNAL_FOLDER}/reactphysics3d/bin/Debug/reactphysics3d.lib"

    optimized "${OUTPUT_FOLDER}/lib/release/${ENGINE_NAME}.lib"
    optimized "${EXTERNAL_FOLDER}/glfw/bin/Release/glfw3.lib"
    optimized "${EXTERNAL_FOLDER}/freetype/bin/Release/freetype.lib"
    optimized "${EXTERNAL_FOLDER}/reactphysics3d/bin/Release/reactphysics3d.lib"

    "${EXTERNAL_FOLDER}/assimp/lib/assimp-vc143-mt.lib"
)
set_target_properties(${SAMPLE_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON

    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_dependencies(${SAMPLE_NAME} 
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

    optimized "${OUTPUT_FOLDER}/lib/release/${ENGINE_NAME}.lib"
    optimized "${EXTERNAL_FOLDER}/glfw/bin/Release/glfw3.lib"
    optimized "${EXTERNAL_FOLDER}/freetype/bin/Release/freetype.lib"
    optimized "${EXTERNAL_FOLDER}/reactphysics3d/bin/Release/reactphysics3d.lib"

    "${EXTERNAL_FOLDER}/assimp/lib/assimp-vc143-mt.lib"
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

# -------------------------- Train game executable ---------------------------
set(TRAIN_NAME    "TrainGame")
set(TRAIN_SOURCE  "${SOURCE_FOLDER}/TrainGame")
file(GLOB_RECURSE _source_tr_list CONFIGURE_DEPENDS "${TRAIN_SOURCE}/*")

add_executable(${TRAIN_NAME}
    ${_source_tr_list}
)
target_include_directories(${TRAIN_NAME} PRIVATE 
    "${SOURCE_FOLDER}/"

    "${EXTERNAL_FOLDER}/glm/"
    "${EXTERNAL_FOLDER}/glad/include/"
    "${EXTERNAL_FOLDER}/GLFW/include/"
    "${EXTERNAL_FOLDER}/stb/"
    "${EXTERNAL_FOLDER}/freetype/include/"
    "${EXTERNAL_FOLDER}/reactphysics3d/include/"
    "${EXTERNAL_FOLDER}/assimp/include/"
)
target_link_libraries(${TRAIN_NAME}
    debug "${OUTPUT_FOLDER}/lib/debug/${ENGINE_NAME}.lib"
    debug "${EXTERNAL_FOLDER}/glfw/bin/Debug/glfw3.lib"
    debug "${EXTERNAL_FOLDER}/freetype/bin/Debug/freetype.lib"
    debug "${EXTERNAL_FOLDER}/reactphysics3d/bin/Debug/reactphysics3d.lib"

    optimized "${OUTPUT_FOLDER}/lib/release/${ENGINE_NAME}.lib"
    optimized "${EXTERNAL_FOLDER}/glfw/bin/Release/glfw3.lib"
    optimized "${EXTERNAL_FOLDER}/freetype/bin/Release/freetype.lib"
    optimized "${EXTERNAL_FOLDER}/reactphysics3d/bin/Release/reactphysics3d.lib"

    "${EXTERNAL_FOLDER}/assimp/lib/assimp-vc143-mt.lib"
)
set_target_properties(${TRAIN_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON

    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}

    LINK_FLAGS_DEBUG    "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE  "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)
add_dependencies(${TRAIN_NAME} 
    ${ENGINE_NAME}
)

# ------------------------------ Global ------------------------------
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${SAMPLE_NAME})

source_group(TREE ${SAMPLE_SOURCE}      FILES ${_source_s_list})
source_group(TREE ${TOOLS_SOURCE}       FILES ${_source_t_list})
source_group(TREE ${ENGINE_SOURCE}      FILES ${_source_e_list})
source_group(TREE ${TRAIN_SOURCE}       FILES ${_source_tr_list})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_external_list})
