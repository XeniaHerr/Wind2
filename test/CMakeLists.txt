
add_executable(
    Client_test
    client_unit_tests.cpp
)

add_executable(
    Topic_test
    topic_unit_tests.cpp
)


add_executable(
    Monitor_test
    monitor_unit_tests.cpp
)

target_link_libraries(Client_test

    WIND2_LIB
GTest::gtest_main)

target_link_libraries(Topic_test

    WIND2_LIB
GTest::gtest_main)


target_link_libraries(Monitor_test
WIND2_LIB
GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(Client_test)
gtest_discover_tests(Topic_test)
gtest_discover_tests(Monitor_test)

