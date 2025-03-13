cmake_minimum_required(VERSION 3.5.0)
set(PROJECT_NAME)
# 允许手动设置项目名称，未设置则使用目录名称
if(NOT DEFINED PROJECT_NAME)
    get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)  # 获取当前目录名作为项目名
endif()
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX ASM)

# 添加编译命令导出（用于生成compile_commands.json）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 配置参数（支持多个目录和文件）
set(SOURCE_DIRS "src" "inc" "./")       # 支持多个源文件目录
set(EXCLUDE_DIRS "doc" "build" ".vscode" ".cache" ".git" ".idea" "out")  # 支持多个排除目录
set(EXCLUDE_FILES ) # 支持多个排除文件

# 递归查找所有源文件
file(GLOB_RECURSE SOURCES
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_DIRS}/*.[cC]   # C文件
    ${SOURCE_DIRS}/*.cpp    # C++文件
    ${SOURCE_DIRS}/*.cxx    # C++文件
    ${SOURCE_DIRS}/*.[sS]      # 汇编文件
)
 
# 合并排除处理（支持多参数）
# 生成排除正则表达式模式（修复build目录排除问题）
set(EXCLUDE_PATTERNS)
foreach(excl_dir ${EXCLUDE_DIRS})
    list(APPEND EXCLUDE_PATTERNS "^${excl_dir}/.*")  # 匹配以目录名开头的路径
endforeach()
foreach(excl_file ${EXCLUDE_FILES})
    list(APPEND EXCLUDE_PATTERNS "/${excl_file}$")   # 匹配任意路径下的指定文件名
endforeach()

# 合并所有排除模式
if(EXCLUDE_PATTERNS)
    list(JOIN EXCLUDE_PATTERNS "|" EXCLUDE_REGEX)
    list(FILTER SOURCES EXCLUDE REGEX "${EXCLUDE_REGEX}")
endif()

# 自动收集头文件目录（改为基于头文件路径）
file(GLOB_RECURSE HEADERS
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_DIRS}/*.h
    ${SOURCE_DIRS}/*.hpp
    ${SOURCE_DIRS}/*.hxx
    ${SOURCE_DIRS}/*.inc
)
foreach(_header ${HEADERS})
    get_filename_component(_dir ${_header} DIRECTORY)
    list(APPEND _include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/${_dir})
endforeach()
list(REMOVE_DUPLICATES _include_dirs)
include_directories(${_include_dirs})

# 调试输出
# message(STATUS "======================================")
# message(STATUS "Discovered source files: ${SOURCES}")
# message(STATUS "Discovered headers: ${HEADERS}")
# message(STATUS "Include directories: ${_include_dirs}")
# message(STATUS "======================================")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

