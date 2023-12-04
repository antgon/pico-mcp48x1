add_library(pico-mcp48x1 INTERFACE)

target_include_directories(pico-mcp48x1
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico-mcp48x1
  INTERFACE
  hardware_spi
)

target_sources(pico-mcp48x1
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/mcp48x1.c
  ${CMAKE_CURRENT_LIST_DIR}/include/mcp48x1.h
)
