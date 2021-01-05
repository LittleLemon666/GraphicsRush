cmake_minimum_required(VERSION 2.8)

project(GraphicsRush)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)

set(SRC_SHADER 
    ${SRC_DIR}shaders/simple.vert
    ${SRC_DIR}shaders/simple.frag
    ${SRC_DIR}shaders/skybox.vert
    ${SRC_DIR}shaders/skybox.frag
    ${SRC_DIR}shaders/text.vert
    ${SRC_DIR}shaders/text.frag
    ${SRC_DIR}shaders/screen.vert
    ${SRC_DIR}shaders/screen.frag
    ${SRC_DIR}shaders/door.vert
    ${SRC_DIR}shaders/door.frag
    ${SRC_DIR}shaders/water_surface.vert
    ${SRC_DIR}shaders/water_surface.frag
    ${SRC_DIR}shaders/shadow.vert
    ${SRC_DIR}shaders/shadow.frag
    ${SRC_DIR}shaders/choose.vert
    ${SRC_DIR}shaders/choose.frag
    ${SRC_DIR}shaders/choose_flat.vert
    ${SRC_DIR}shaders/choose_flat.frag
    ${SRC_DIR}shaders/environment.vert
    ${SRC_DIR}shaders/environment.frag
    ${SRC_DIR}shaders/blending.vert
    ${SRC_DIR}shaders/blending.frag
    ${SRC_DIR}shaders/blending_flat.vert
    ${SRC_DIR}shaders/blending_flat.frag
    ${SRC_DIR}shaders/blending_flat_gray.frag
    ${SRC_DIR}shaders/firework.vert
    ${SRC_DIR}shaders/firework.frag
    ${SRC_DIR}shaders/hp.vert
    ${SRC_DIR}shaders/hp.frag)

set(SRC_RENDER_UTILITIES
    ${SRC_DIR}RenderUtilities/BufferObject.h
    ${SRC_DIR}RenderUtilities/Shader.h
    ${SRC_DIR}RenderUtilities/Texture.h
    ${SRC_DIR}RenderUtilities/Shadow.h
    ${SRC_DIR}RenderUtilities/Environment.h)

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}glad4.6/include/)
include_directories(${INCLUDE_DIR}glm-0.9.8.5/glm/)

add_Definitions("-D_XKEYCHECK_H")

add_executable(GraphicsRush
    ${SRC_DIR}CallBacks.h
    ${SRC_DIR}ControlPoint.h
    ${SRC_DIR}Object.h
    ${SRC_DIR}Track.h
    ${SRC_DIR}TrainView.h
    ${SRC_DIR}TrainWindow.h
    ${SRC_DIR}GMTPipeline.h
    ${SRC_DIR}Model.h
    ${SRC_DIR}Player.h
    ${SRC_DIR}ShaderInfo.h
    ${SRC_DIR}Boss.h
    ${SRC_DIR}Shop.h
    ${SRC_DIR}Firework.h

    ${SRC_DIR}main.cpp
    ${SRC_DIR}CallBacks.cpp
    ${SRC_DIR}ControlPoint.cpp
    ${SRC_DIR}Object.cpp
    ${SRC_DIR}Track.cpp
    ${SRC_DIR}TrainView.cpp
    ${SRC_DIR}TrainWindow.cpp
    ${SRC_DIR}GMTPipeline.cpp
    ${SRC_DIR}Model.cpp
    ${SRC_DIR}Player.cpp
    ${SRC_DIR}Boss.cpp
    ${SRC_DIR}Shop.cpp

    ${SRC_SHADER}
    ${SRC_RENDER_UTILITIES}

    ${INCLUDE_DIR}glad4.6/src/glad.c
)
source_group("shaders" FILES ${SRC_SHADER})
source_group("RenderUtilities" FILES ${SRC_RENDER_UTILITIES})


add_library(Utilities 
    ${SRC_DIR}Utilities/ArcBallCam.h
    ${SRC_DIR}Utilities/3DUtils.h
    ${SRC_DIR}Utilities/Pnt3f.h
    ${SRC_DIR}Utilities/ArcBallCam.cpp
    ${SRC_DIR}Utilities/3DUtils.cpp
    ${SRC_DIR}Utilities/Pnt3f.cpp)

target_link_libraries(GraphicsRush 
    debug ${LIB_DIR}Debug/fltk_formsd.lib      optimized ${LIB_DIR}Release/fltk_forms.lib
    debug ${LIB_DIR}Debug/fltk_gld.lib         optimized ${LIB_DIR}Release/fltk_gl.lib
    debug ${LIB_DIR}Debug/fltk_imagesd.lib     optimized ${LIB_DIR}Release/fltk_images.lib
    debug ${LIB_DIR}Debug/fltk_jpegd.lib       optimized ${LIB_DIR}Release/fltk_jpeg.lib
    debug ${LIB_DIR}Debug/fltk_pngd.lib        optimized ${LIB_DIR}Release/fltk_png.lib
    debug ${LIB_DIR}Debug/fltk_zd.lib          optimized ${LIB_DIR}Release/fltk_z.lib
    debug ${LIB_DIR}Debug/fltkd.lib            optimized ${LIB_DIR}Release/fltk.lib
    debug ${LIB_DIR}Debug/opencv_world341d.lib optimized ${LIB_DIR}Release/opencv_world341.lib)

target_link_libraries(GraphicsRush 
    ${LIB_DIR}OpenGL32.lib
    ${LIB_DIR}glu32.lib
    ${LIB_DIR}common.lib
    ${LIB_DIR}ex-common.lib
    ${LIB_DIR}OpenAL32.lib
    ${LIB_DIR}alut.lib
    ${LIB_DIR}alut_static.lib
    ${LIB_DIR}freetyped.lib)

target_link_libraries(GraphicsRush Utilities)
    