cmake_minimum_required(VERSION 3.15)
project(laba_3 VERSION 1.0 LANGUAGES CXX)

# ================= НАСТРОЙКИ КОМПИЛЯЦИИ =================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Строгий режим предупреждений
if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ================= GOOGLE TEST =================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Для MSVC: используем динамическую CRT, чтобы избежать конфликтов линковки
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ================= ИСПОЛНЯЕМЫЕ ФАЙЛЫ =================
# Основной файл (консольный UI)
add_executable(${PROJECT_NAME}
        main.cpp
)

# Файл с модульными тестами
add_executable(${PROJECT_NAME}_tests
        tests.cpp
)

# Подключение заголовочных файлов
# Компилятор увидит .h через target_include_directories.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_SOURCE_DIR})

# Линковка Google Test только к таргету тестов
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        GTest::gtest_main
)

# ================= ИНТЕГРАЦИЯ ТЕСТОВ =================
enable_testing()
include(GoogleTest)
# Автоматически регистрирует все TEST() и TEST_F() в системе CTest
gtest_discover_tests(${PROJECT_NAME}_tests)