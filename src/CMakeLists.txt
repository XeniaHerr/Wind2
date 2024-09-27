

add_executable(TEST test.cpp)



add_library(WIND2_LIB STATIC client.cpp
                            topic.cpp
                            monitor.cpp
                            WindowManagerModel.cpp
                            ConcreteArranger.cpp
                            ArrangerSelector.cpp
                            RuleBuilder.cpp
                            Rule.cpp
                            ConfigReader.cpp
                            KeyBuilder.cpp
                            InputManager.cpp
                            Action.cpp
                            ConcreteActions.cpp
			    X11Abstraction.cpp
			    run.cpp
			    InputManager.cpp
			    Inputstructs.cpp
			    InputBuilder.cpp
			    Handlers.cpp)


target_link_libraries(WIND2_LIB
    PUBLIC
    yaml-cpp::yaml-cpp
    X11
    Xft
    )

add_executable(Wind2 main.cpp)


target_link_libraries(Wind2 PUBLIC WIND2_LIB)
