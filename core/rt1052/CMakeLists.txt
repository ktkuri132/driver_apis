# 添加外部头文件路径



# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source 
    ${CMAKE_CURRENT_SOURCE_DIR}/sta rtup/gcc/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_MIMXRT1052.c
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/gcc/startup_MIMXRT1052.s
    ${CMAKE_CURRENT_SOURCE_DIR}/syscall.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_MIMXRT1052.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/LQRT1052/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/MIMXRT1052/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/MIMXRT1052/camera/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/fatfs/source/fsl_sd_disk/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/fatfs/source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/board/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xip/*.c
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/startup/gcc/MIMXRT1052xxxxx_flexspi_nor_sdram.ld PARENT_SCOPE)

#添加宏定义
add_definitions(
-D__USE_CMSIS 
-DLQRELEASE 
-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE 
-DXIP_EXTERNAL_FLASH
-DXIP_BOOT_HEADER_ENABLE
-DXIP_BOOT_HEADER_DCD_ENABLE
-DSKIP_SYSCLK_INIT
# -DPRINTF_FLOAT_ENABLE
-DSCANF_FLOAT_ENABLE
-DPRINTF_ADVANCED_ENABLE
-DSCANF_ADVANCED_ENABLE
-DCPU_MIMXRT1052DVL6B
)

# 生成库文件
add_library(${DIR_NAME} ${${DIR_NAME}_Source})
# 添加当前头文件路径
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../CMSIS/Include)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/board)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fatfs)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/fatfs/source)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/fatfs/source/fsl_sd_disk)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/sdmmc/inc)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fatfs/sdmmc/port)

target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drivers/MIMXRT1052)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drivers/MIMXRT1052/camera)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drivers/LQRT1052)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utilities)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/xip)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/startup/gcc)

# target_include_directories(${DIR_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_StdPeriph_Driver/inc)



