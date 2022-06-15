cmake_minimum_required(VERSION 3.16)

project(sws)

# Manually set sources.
set(sources
    src/main.c
)

# GPIO Stub file
option(STUB_GPIO "use gpio stub file in build")
if(STUB_GPIO)
    list(APPEND sources stubs/sws_gpio_stub.c)
else()
    list(APPEND sources src/sws_gpio.c)
endif(STUB_GPIO)

# Time stub file
option(STUB_TIME "use time stub file in build")
if(STUB_TIME)
    list(APPEND sources stubs/sws_time_stub.c)
else()
    list(APPEND sources src/sws_time.c)
endif(STUB_TIME)

# Config stub file.
option(STUB_CONFIG "use config stub file in build")
if(STUB_CONFIG)
    list(APPEND sources stubs/sws_config_stub.c)
else()
    list(APPEND sources src/sws_config.c)
endif(STUB_CONFIG)

add_executable(sws ${sources})
target_include_directories(sws PUBLIC src)

# For windows, remove annoying warnings about "unsafe" functions.
if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_C_STANDARD 11)
