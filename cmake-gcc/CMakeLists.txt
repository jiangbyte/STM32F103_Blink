cmake_minimum_required(VERSION 3.20)

project(Blink C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld")

add_executable(${PROJECT_NAME}
  "${ROOT_DIR}/User/main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f103xb.s"
)

target_include_directories(${PROJECT_NAME} PRIVATE
  "${ROOT_DIR}/User"
  "${ROOT_DIR}/CMSIS"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  STM32F103xB
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -Wextra
  -ffunction-sections
  -fdata-sections
  -fno-common
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:-Os>
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-Og;-g3>
  $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:Debug>>:-g3>
)

target_link_options(${PROJECT_NAME} PRIVATE
  -T${LINKER_SCRIPT}
  -Wl,--gc-sections
  -Wl,-Map=$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.map
  --specs=nano.specs
  --specs=nosys.specs
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  SUFFIX ".elf"
  LINK_DEPENDS "${LINKER_SCRIPT}"
)

find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(CMAKE_SIZE arm-none-eabi-size REQUIRED)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}>
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}>
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.bin
  COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  COMMENT "Generating HEX/BIN and printing size"
)
