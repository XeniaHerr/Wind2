#include "Handlers.h"
#include "InputManager.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <optional>
#include "Monitor.h"
#include "WindowManagerModel.h"
#include "X11_Abstraction.h"
#include <DefaultRenderer.h>
#include <Monitor.h>



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

    auto& Log = Logger::GetInstance();

    Log.Info("Inside ManagingRequestAction");

    XMapEvent e = std::get<XEvent*>(Arg)->xmap;

    auto& xc = X11Abstraction::getInstance();

    auto wa = xc.getWindowAttributes(e.window);


    if (wa == std::nullopt || wa->override_redirect)
	return;

    Log.Info("Client wants to be managed");


    auto& WMM = WindowManagerModel::getInstance();

    Client * c = WMM.manageWindow(e.window);

    Log.Info("Managed Client {}", c == nullptr ? "Unsucseccfull" : "Successful");


    if (!c)
	return;

    auto str = xc.getWindowNameClass(e.window);

    c->setName(str.first);
    c->setClass(str.second);

    Log.Info("Got name = {} and class = {}", c->getName(), c->getClass());

    
    c->attachRule();
    Log.Info("Searched for Rule");
    c->applyRule();

    Log.Info("Rules got attached");

    xc.subscribetoWindow(e.window, EnterWindowMask|SubstructureNotifyMask|FocusChangeMask|PropertyChangeMask);

    

    if (Monitor* m = c->getOwner().getHolder()) {
	m->arrange();
	xc.drawMonitor(*m);
    }

    xc.configureClient(c);

    xc.prependClientList(e.window);

    WMM.focusClient();

    xc.setfocus(c);

}


auto UnmanageRequestAction::execute() -> void {

    XUnmapEvent ev = std::get<XEvent*>(this->Arg)->xunmap;
    auto& xc = X11Abstraction::getInstance();
    auto& WMM = WindowManagerModel::getInstance();
    auto& Log = Logger::GetInstance();
    Log.Info("Inside UnmanageRequestAction");





    Client *c = WMM.getClient(ev.window);

    if (!c) {
	Log.Warn("Windows {} is not managed, cant unmanage", ev.window);
	return;
    }

    Log.Info("Client {} want's to be unmanaged", c->getName());

    Topic& t = c->getOwner();


   t.releaseOwnership(*c); 

   WMM.unmanageWindow(ev.window);
   Log.Info("Model has removed the client");

   if (t.getHolder() != nullptr) {

       t.getHolder()->arrange();
       xc.drawMonitor(*t.getHolder());
   }

   xc.updateClientList();

   WMM.focusClient();
   xc.setfocus(WMM.getFocusedMon()->getCurrent()->getFocused());
Log.Info("UnmanageRequestAction done");
}



auto EnterNotifyAction::execute() -> void {

    XEnterWindowEvent ev = std::get<XEvent*>(this->Arg)->xcrossing;
    auto& xc = X11Abstraction::getInstance();
    auto& WMM = WindowManagerModel::getInstance();
    auto& Log = Logger::GetInstance();
    Log.Info("Inside EnterNotifyAction");


    Client *c = WMM.getClient(ev.window);


    if (!c) {
	Log.Info("Window is not managed");
	return;
    }


    if (WMM.getFocusedMon()->getCurrent()->getFocused() != c) {
    WMM.focusClient(ev.window);

    xc.sendClientAtom(ev.window, ATOMNAME::WMTakeFocus);
    xc.setfocus(c);
    }

    
    Log.Info("Done with EnterNotifyAction");
}



