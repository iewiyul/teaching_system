# =============================================================================
# 智慧教学管理软件系统 —— CMake 构建脚本
# 项目: 上海大学《数据结构与算法基础》课程项目 2026-2027
# =============================================================================
#
# 用法 (Windows / MinGW):
# cd <project_root>
# cmake -S . -B build -G "MinGW Makefiles"
# cmake --build build
# .\build\teaching_system.exe
#
# 用法 (Linux / macOS):
# cmake -S . -B build
# cmake --build build
# ./build/teaching_system
#
# 注意:
# - seed.c 由 add_executable 单独编译；main.c 通过 #include "data/seed.h" 拿声明，
# 不要再在 main.c 里 #include "data/seed.c"，否则会报 duplicate symbol。
# - 原 CLAUDE.md 的 gcc 命令漏列了 TeacherHashTable.c, 这里已补齐。
# =============================================================================

cmake_minimum_required(VERSION 3.10)
project(teaching_system C)

# ---------- 语言与编译选项 ----------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra)
elseif(MSVC)
    # MSVC 默认按系统代码页（GBK 936）读源文件，源码是 UTF-8 时中文会触发 C4819 警告。
    # /utf-8 让 MSVC 按 UTF-8 读源文件，消除 C4819 编码警告（也是连锁语法错误的根因）。
    add_compile_options(/utf-8)
endif()

# ---------- 包含路径 ----------
include_directories(${CMAKE_SOURCE_DIR}/src)

# ---------- 目标 ----------
# README 风格 Windows / Linux 平台都生成同名可执行文件 (Windows 自动加 .exe)
add_executable(teaching_system
    src/main.c
    src/ui/Menu.c

    # 数据结构 (一层)
    src/ds/SeqList.c
    src/ds/LinkedList.c
    src/ds/Stack.c
    src/ds/Queue.c
    src/ds/HashTable.c
    src/ds/TeacherHashTable.c # 与 HashTable 并列, 存 Teacher*, CLAUDE.md 原命令漏列
    src/ds/BST.c
    src/ds/Heap.c
    src/ds/Graph.c

    # 业务 (二层)
    src/service/TeacherService.c
    src/service/StudentService.c
    src/service/CourseService.c
    src/service/ScoreService.c
    src/service/NoticeService.c
    src/service/SearchService.c
    src/service/RankingService.c
    src/service/OperationHistory.c

    src/data/seed.c
)

# ---------- 平台相关链接 (仅 Windows + MinGW 需要 ws2_32 以支持 clock_gettime) ----------
if(MINGW)
    target_link_libraries(teaching_system ws2_32)
endif()

# ---------- 调试期方便: 启用 sanitizer, 用 -DENABLE_ASAN=ON 打开 ----------
option(ENABLE_ASAN "Enable AddressSanitizer (debug builds)" OFF)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()