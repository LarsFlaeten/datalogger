set(EXENAME app)

# Sources
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/libraries" "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCE_FILES main.cpp Datalogger.cpp)

# Add defines for exe build
# add_definitions("-DXXX=")

# Common deps
#find_package(PkgConfig)
#PKG_CHECK_MODULES(ORK REQUIRED ork)
#add_definitions(${ORK_CFLAGS})
#PKG_CHECK_MODULES(PROLAND-CORE REQUIRED proland-core)
#add_definitions(${PROLAND-CORE_CFLAGS})

message("${PROJECT_SOURCE_DIR}") 

#add_definitions("-DORK_API=")

#link_directories(${PROJECT_SOURCE_DIR}/libraries ${PROJECT_SOURCE_DIR}/libraries/spice ${PROJECT_SOURCE_DIR}/lib)


add_executable(${EXENAME} ${SOURCE_FILES})
target_compile_options(${EXENAME} PUBLIC -Wall)
#target_link_libraries(${EXENAME} ork-extensions spicepp cspice -Wl,--whole-archive proland-space simulator proland-core ork -Wl,--no-whole-archive pthread GL GLU GLEW glut glfw3   rt dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 tiff AntTweakBar  stb_image tinyxml)

add_executable(blackboxreader blackboxreader.cpp)
target_compile_options(blackboxreader PUBLIC -Wall)
