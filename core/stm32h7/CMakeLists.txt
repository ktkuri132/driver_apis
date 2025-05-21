# 添加外部头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/inc/Legacy)

# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source 
    # ${CMAKE_CURRENT_SOURCE_DIR}/*.s
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/src/*.c
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/*.ld PARENT_SCOPE)


# 生成库文件
add_library(${DIR_NAME} ${${DIR_NAME}_Source})


# 添加当前头文件路径
target_include_directories(${DIR_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/inc)
target_include_directories(${DIR_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/inc/Legacy)



