
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


add_executable(
    Client_Topic_Integration

    ClientTopicIntegrationTest.cpp

)

add_executable(
    Topic_Mon_Integration

    TopicMonitorIntegrationTest.cpp
)


add_executable(
    WindowManagerModelTest
    WindowManagerTest.cpp
)


add_executable(
    ArrangeTest
    ArrangeTest.cpp
)

add_executable(
    RuleTest
    RuleandRuleBuilderTest.cpp
    )

add_executable(
    ConfigTest
    Configtest.cpp
    )

add_executable(
    ApplyConfig
    WindowManagerModelConfigReaderIntegration.cpp
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


target_link_libraries(Client_Topic_Integration

    WIND2_LIB
GTest::gtest_main)

target_link_libraries(Topic_Mon_Integration

    WIND2_LIB
GTest::gtest_main)



target_link_libraries(WindowManagerModelTest
       WIND2_LIB
    GTest::gtest_main)



target_link_libraries(ArrangeTest
    WIND2_LIB
GTest::gtest_main)
target_link_libraries(RuleTest
    WIND2_LIB

    GTest::gtest_main)

target_link_libraries(ConfigTest
    WIND2_LIB
    GTest::gtest_main)



target_link_libraries(ApplyConfig
    WIND2_LIB
    GTest::gtest_main)
include(GoogleTest)

gtest_discover_tests(Client_test)
gtest_discover_tests(Topic_test)
gtest_discover_tests(Monitor_test)
gtest_discover_tests(Client_Topic_Integration)
gtest_discover_tests(Topic_Mon_Integration)
gtest_discover_tests(WindowManagerModelTest)
gtest_discover_tests(ArrangeTest)
gtest_discover_tests(RuleTest)
gtest_discover_tests(ConfigTest)
gtest_discover_tests(ApplyConfig)

