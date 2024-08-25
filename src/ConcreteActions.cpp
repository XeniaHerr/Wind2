#include "Logger.h"
#include <ConcreteActions.h>
#include <X11/Xlib.h>
#include <memory>
#include <run.h>
#include <InputManager.h>


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


// Handle Key Events

auto keyHandlerAction::execute() -> void {

    XKeyEvent e = std::get<XEvent*>(Arg)->xkey;


    InputManager& m = InputManager::GetInstance();


    Key k;
    k.keysym = XLookupKeysym(&e, 0);
    k.modifier = e.state;


    m.handleKey(k);

}


auto keyHandlerAction::name() -> std::string {
    return "KeyHandler";
}

auto keyHandlerAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new keyHandlerAction);
}


auto keyHandlerAction::operator()() -> void {
    this->execute();
}

auto keyHandlerAction::wantArgument() -> bool {
    return true;
}
