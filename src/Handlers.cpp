#include "Handlers.h"
#include "InputManager.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <optional>
#include "Monitor.h"
#include "WindowManagerModel.h"
#include "X11_Abstraction.h"
#include "structs.h"
#include <DefaultRenderer.h>
#include <Monitor.h>



using namespace Wind;

void handle_client_type(Client*c, Windowtype type) {

auto& xc = X11Abstraction::getInstance();
auto& WMM = WindowManagerModel::getInstance();
auto& Log = Logger::GetInstance();

Monitor *m;
switch(type) {


    
    case Windowtype::SPLASH:
	c->setManaged(false);
	c->setFloating();
	break;
    case Windowtype::ANYTYPE:
    case Windowtype::BAR:
    case Windowtype::DIALOG:
    case Windowtype::PANEL:
    case Windowtype::DOCK:
    case Windowtype::WIDGET:
	Log.Info("Found type other than normal");
	c->setFloating();

    case Windowtype::NORMAL:

	Log.Info("Handling normal window");
    xc.subscribetoWindow(c->getWindow(), EnterWindowMask|SubstructureNotifyMask|FocusChangeMask|PropertyChangeMask);

    xc.showWindow(c->getWindow());

    Log.Info("window shown");
    
    if (( m = c->getOwner().getHolder()) != nullptr)
	m->arrange();

    Log.Info("Configuring Client");

    xc.configureClient(c);

    xc.prependClientList(c->getWindow());

    Log.Info("Focussing client");
    WMM.focusClient();

    xc.setfocus(c);

	break;
}
}



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
    auto& xc = X11Abstraction::getInstance();
    auto& WMM = WindowManagerModel::getInstance();

    Log.Info("Inside ManagingRequestAction");

    XMapEvent e = std::get<XEvent*>(Arg)->xmap;


    auto wa = xc.getWindowAttributes(e.window);


    if (wa == std::nullopt || wa->override_redirect)
	return;

    Log.Info("Client wants to be managed");



    Client * c = WMM.manageWindow(e.window);

    Log.Info("Managed Client {}", c == nullptr ? "Unsucseccfull" : "Successful");


    if (!c)
	return;

    auto str = xc.getWindowNameClass(e.window);

    c->setName(str.first);
    c->setClass(str.second);

    Log.Info("Got name = {} and class = {}", c->getName(), c->getClass());


    Windowtype windowtype = xc.getWindowType(e.window);

    Log.Info("Got Type");

    c->setType(windowtype);


    
    c->attachRule();
    Log.Info("Searched for Rule");
    c->applyRule();

    Log.Info("Rules got attached");


    //Entrypoint for handle type

    handle_client_type(c, windowtype);

}


auto UnmanageRequestAction::execute() -> void {

    XUnmapEvent ev = std::get<XEvent*>(this->Arg)->xunmap;
    auto& xc = X11Abstraction::getInstance();
    auto& WMM = WindowManagerModel::getInstance();
    auto& Log = Logger::GetInstance();
    Log.Info("Inside UnmanageRequestAction");



    if (ev.send_event == true) {
	Log.Info("Event was send, aborting");
	return;
    }


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
       //xc.drawMonitor(*t.getHolder());
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



auto DestroyNotifyAction::execute() -> void {

    
    auto& Log = Logger::GetInstance();
    Log.Info("Inside DestroyNotifyAction");

    XDestroyWindowEvent e = std::get<XEvent*>(this->Arg)->xdestroywindow;

    Client* c = WindowManagerModel::getInstance().getClient(e.window);


    if (c) {

	WindowManagerModel::getInstance().unmanageWindow(c->getWindow());
	Log.Info("Unmanaged Window");
    }

    ;

}


auto MappingNotifyAction::execute() -> void {


    XMappingEvent e = std::get<XEvent*>(this->Arg)->xmapping;



    XRefreshKeyboardMapping(&e);

    if (e.request == MappingKeyboard)
	X11Abstraction::getInstance().listenforKeys(InputManager::GetInstance().getKeys());

}


auto ConfigureRequestAction::execute() -> void {


    XConfigureRequestEvent e =  std::get<XEvent*>(this->Arg)->xconfigurerequest;

    auto& WMM = WindowManagerModel::getInstance();
    auto& xc = X11Abstraction::getInstance();
    auto& Log = Logger::GetInstance();

    Log.Info("Inside ConfigureRequestAction");

    Client* c = WMM.getClient(e.window);


    if(!c)
	return;

    if (!c->isFloating()) {

	Log.Info("Client is not floating, usinf default configuration");
	xc.configureClient(c);
    }
    else {
	Position op = c->getPosition();
	Dimensions od = c->getCurrentDimensions();

	if (e.value_mask & CWWidth)
	    od.width = e.width;
	if (e.value_mask & CWHeight)
	    od.height = e.height;
	if (e.value_mask & CWX)
	    op.x = e.x;
	if (e.value_mask & CWY)
	    op.y = e.y;


	c->setPosition(op);
	c->setDimensions(od);
	c->applyRule();

	xc.drawClient(*c);


	//Render the client;

    }
}


