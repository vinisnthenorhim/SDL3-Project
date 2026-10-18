cmake_minimum_required(VERSION 3.20)

project(BrokenWorld LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(include/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(include/glm EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" )
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h"   )

find_package(Lua 5.4 REQUIRED)
find_package(sol2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/script)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/script/logger)


target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rapidjson/include"
    ${LUA_INCLUDE_DIR}
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL3::SDL3
    glm::glm
    sol2::sol2
    ${LUA_LIBRARIES}
)

if(WIN32)
    target_link_libraries(BrokenWorld PRIVATE dwmapi)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    VERBATIM
)