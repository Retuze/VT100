@echo off
REM 创建构建目录
if not exist build mkdir build
cd build

REM 配置 CMake
cmake -G "MinGW Makefiles" ..

REM 编译
mingw32-make

REM 运行测试
ctest --output-on-failure

REM 检查 gcovr 是否可用
where gcovr >nul 2>nul
if %ERRORLEVEL% == 0 (
    REM 生成覆盖率报告
    gcovr -r .. ^
          --html --html-details ^
          -o coverage_report.html ^
          --exclude=".*tests.*" ^
          --exclude=".*_deps.*" ^
          --exclude=".*googletest.*" ^
          --exclude=".*CMake.*" ^
          --source-encoding utf8
    echo Coverage report generated at build\coverage_report.html
) else (
    echo gcovr not found. Coverage report not generated.
    echo To install gcovr, run: pip install gcovr
)

cd .. 