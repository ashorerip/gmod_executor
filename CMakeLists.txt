cmake_minimum_required(VERSION 3.16)
project("dll")
project("injector")

if (MSVC)

add_library(dll SHARED "src/dll/main.cpp")
add_executable(injector "src/injector/main.cpp")

include_directories(include/)
include(FetchContent)

# safetyhook
#set(SAFETYHOOK_FETCH_ZYDIS ON)
#FetchContent_Declare(
#    safetyhook
#    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
#    GIT_TAG "origin/main"
#)

#target_link_libraries(dll safetyhook::safetyhook)
#

# imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG "origin/master"
)
#

#FetchContent_MakeAvailable(safetyhook)
FetchContent_MakeAvailable(imgui)

# compiler stuff
set_property(TARGET dll PROPERTY CXX_STANDARD 23)
target_compile_options(dll PUBLIC /utf-8)

set_property(TARGET injector PROPERTY CXX_STANDARD 23)
target_compile_options(injector PUBLIC /utf-8)
#

endif()