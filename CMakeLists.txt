# 参考CubeMX生成的CMakeLists.txt文件进行修改和添加注释,若有疑问,请联系作者邮箱3457532883@qq.com
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

# 设置目标处理器架构,芯片型号,和目标板简称
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)
set(Target_Board I.MXRT)
set(Board_Name rt1052)

# 指定交叉编译工具链
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 项目名称
project(mtc C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# 添加关于硬件浮点的支持的相关编译选项
if("${Target_Board}" STREQUAL "STM32F1xx")
    message(STATUS "${Target_Board}")
    add_compile_options(-mfloat-abi=soft)
    add_link_options(-mfloat-abi=soft)
elseif ("${Target_Board}" STREQUAL "STM32F4xx")
    message(STATUS "${Target_Board}")
    add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
    add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
elseif("${Target_Board}" STREQUAL "STM32H7xx")
    message(STATUS "${Target_Board}")
    add_compile_options(-mfloat-abi=hard -mfpu=fpv5-d16)
    add_link_options(-mfloat-abi=hard -mfpu=fpv5-d16)
elseif("${Target_Board}" STREQUAL "I.MXRT")
    message(STATUS "${Target_Board}")
    add_compile_options(-mfloat-abi=hard -mfpu=fpv5-d16)
    add_link_options(-mfloat-abi=hard -mfpu=fpv5-d16)
elseif("${Target_Board}" STREQUAL "")
    message(FATAL_ERROR "Unknown target board")
endif ()

# 指定芯片型号的相关编译选项
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-m4" OR 
   "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-m7" OR 
   "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-m3" OR 
   "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-m0")
    add_compile_options(-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb)
    add_link_options(-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb -mthumb-interwork)
elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-a7" OR
       "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "cortex-a8")
    add_compile_options(-mcpu=${CMAKE_SYSTEM_PROCESSOR})
    add_link_options(-mcpu=${CMAKE_SYSTEM_PROCESSOR})
endif()
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_link_options(-specs=nosys.specs -lc -lm)

# 添加子目录Cmake文件
add_subdirectory(core/${Board_Name})
add_subdirectory(bsp)

# 添加静态库目标
add_library(${PROJECT_NAME} STATIC main.c)

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE core/${Board_Name})
target_include_directories(${PROJECT_NAME} PRIVATE bsp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# 根据目标板添加特定的头文件路径
# include_directories(core/CMSIS/Include)
if("${Target_Board}" STREQUAL "STM32F1xx")
    target_include_directories(${PROJECT_NAME} PRIVATE core/stm32f1/STM32F1xx_StdPeriph_Driver/inc)
elseif ("${Target_Board}" STREQUAL "STM32F4xx")
    target_include_directories(${PROJECT_NAME} PRIVATE core/stm32f4/STM32F4xx_StdPeriph_Driver/inc)
elseif("${Target_Board}" STREQUAL "STM32H7xx")
    target_include_directories(${PROJECT_NAME} PRIVATE core/stm32h7/STM32H7xx_HAL_Driver/inc)
elseif("${Target_Board}" STREQUAL "I.MXRT")
    include_directories(${CMAKE_SOURCE_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/board)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/drivers/LQRT1052)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/drivers/MIMXRT1052)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/drivers/MIMXRT1052/camera)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/utilities)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/xip)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name}/staryup/gcc)
    include_directories(${CMAKE_SOURCE_DIR}/core/${Board_Name})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/arch/${Board_Name})
elseif("${Target_Board}" STREQUAL "")
    message(FATAL_ERROR "Unknown target board")
endif ()