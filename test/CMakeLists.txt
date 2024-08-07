
add_executable(
    Client_test
    client_unit_tests.cpp
)


target_link_libraries(Client_test

    WIND2_LIB
GTest::gtest_main)


include(GoogleTest)

gtest_discover_tests(Client_test)
