add_library(core_lib core.cpp pin_controller.cpp data_extraction.cpp)


target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include ${PIGPIO_INCLUDE_DIR})


target_link_libraries(core_lib PRIVATE cpr::cpr)
target_link_libraries(core_lib PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(core_lib PRIVATE ${PIGPIO_LINK_LIB})