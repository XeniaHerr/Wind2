#include "Logger.h"
#include <ConcreteActions.h>
#include <X11/Xlib.h>
#include <memory>
#include <run.h>
#include <InputManager.h>
#include "Handlers.h"


using namespace Wind;

//quitting WindowManger

auto quitAction::execute() -> void {
    Logger::GetInstance().Info("Inside quitAction");

    auto& r = Run::getInstance();
    if (r.isrunning())
	r.stop();
    else
	Logger::GetInstance().Info("Trying to quit a non running windowManager");
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


auto quitAction::operator()() -> void {
    this->execute();
}


// other Actions


