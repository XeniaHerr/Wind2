#include <ConcreteActions.h>
#include <memory>


using namespace Wind;



auto quitAction::execute() -> void {

    ;
}


auto quitAction::name() -> std::string {
    return "quit";
}


auto quitAction::clone() -> std::unique_ptr<Action> {

    return std::unique_ptr<Action>(new quitAction);
}


auto quitAction::wantArgument() -> bool {
    return false;
}


auto quitAction::operator()() -> void {}
