#include "Handlers.h"
#include "InputManager.h"
#include <X11/Xlib.h>
#include <optional>
#include "WindowManagerModel.h"
#include "X11_Abstraction.h"



using namespace Wind;

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



auto ManageRequestAction::execute() -> void {

    XMapEvent e = std::get<XEvent*>(Arg)->xmap;

    auto& xc = X11Abstraction::getInstance();

    auto wa = xc.getWindowAttributes(e.window);


    if (wa == std::nullopt || wa->override_redirect)
	return;


    auto& WMM = WindowManagerModel::getInstance();

    Client * c = WMM.manageWindow(e.window);


    if (!c)
	return;

    auto str = xc.getWindowNameClass(e.window);

    c->setName(str.first);
    c->setClass(str.second);

    
    c->attachRule();
//    c->applyRule();
}




