


# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source
        arch/${Board_Name}/*.c
        Middlewave/sh/*.c
        # Hardware/w25qxx/*.c
)

add_subdirectory(Middlewave/rtos)

add_library(${DIR_NAME} ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME} ${Board_Name} freertos_kernel)

target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/CMSIS/Include)
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/arch/${Board_Name})

# 添加当前头文件路径
if("${Board_Name}" STREQUAL "rt1052")
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name})
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name}/board)
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name}/drivers/MIMXRT1052)
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name}/drivers/MIMXRT1052/camera)
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name}/utilities)
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name}/xip)
elseif("${Board_Name}" STREQUAL "stm32h7")
        include_directories(${CMAKE_SOURCE_DIR}/core/stm32h723/STM32H7xx_HAL_Driver/inc)
elseif("${Board_Name}" STREQUAL "mspm0g3507")
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/${Board_Name})
        target_include_directories(${DIR_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/arch/${Board_Name})
endif()