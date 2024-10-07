#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <run.h>
#include <X11_Abstraction.h>
#include <WindowManagerModel.h>
#include <ConfigReader.h>
#include <Logger.h>
#include <ConcreteActions.h>
#include <tuple>
#include <InputManager.h>
#include "Handlers.h"
#include <unistd.h>


int main() {

    using namespace Wind;

    Logger& Log = Logger::GetInstance();
    Log.setOutputfile("Wind2");
    Log.Info("Welcome to Wind2");

    X11Abstraction& xconnection = X11Abstraction::getInstance();

    if (!xconnection.isUsable()) {
	Log.Error("Couldn't open Display. Make shure that an X Server is running");
	exit(1);

    }

    if (xconnection.checkotherWM()) {
	Log.Error("There is another WindowManager running. Wind must be the only active WindowManager");
	exit(2);
    }


    WindowManagerModel& WMM = WindowManagerModel::getInstance();

    ConfigReader& Config = ConfigReader::getInstance();

    char* home_path;

    //std::string configpath = "/home/xenia/Projects/Wind2/test/Wind.yaml";
    
//    home_path = getenv("XDG_CONFIG_HOME");

 //   if (!home_path) // XDG_CONFIG_HOME is not set, defaulting to HOME
    home_path = getenv("HOME");

    if (!home_path) {
	Log.Error("Couldn't get path for config file, $HOME is not set");
	return 1;
    }

    std::string configpath = home_path;

    configpath.append("/.config/Wind2/Wind.yaml");



    
    Config.read(configpath);

    if (Config.empty) {
	Log.Error("Couldn't read Config at filepath {}", configpath);
    }


    //Ensure default Monitor for testing purposes
    std::vector<std::tuple<Dimensions, Position, unsigned>> monitor;
    monitor.push_back(std::make_tuple(Dimensions(800,400), Position(0,0), 0));
    WMM.registerMonitors(monitor);

    if (!WMM.loadConfig()) {
	Log.Error("Config is not valid, aborting");
	exit(2);
    }

    WMM.focusClient();
    xconnection.setEventMask(SubstructureRedirectMask|EnterWindowMask|SubstructureNotifyMask|StructureNotifyMask|PropertyChangeMask|LeaveWindowMask);

    xconnection.listenforKeys(InputManager::GetInstance().getKeys());

    xconnection.addAtom(ATOMNAME::WMDelete, "WM_DELETE_WINDOW");
    xconnection.addAtom(ATOMNAME::WMTakeFocus, "WM_TAKE_FOCUS");
    xconnection.addAtom(ATOMNAME::WMProtocols, "WM_PROTOCOLS");
    xconnection.addAtom(ATOMNAME::NetActiveWindow, "_NET_ACTIVE_WINDOW");
    xconnection.addAtom(ATOMNAME::NetWMName, "_NET_WM_NAME");
    xconnection.addAtom(ATOMNAME::NetClientList, "_NET_CLIENT_LIST");
    xconnection.addAtom(ATOMNAME::NetDesktopNames, "_NET_DESKTOP_NAMES");
    xconnection.addAtom(ATOMNAME::NetDesktopNumber, "_NET_NUMBER_OF_DESKTOPS");
    xconnection.addAtom(ATOMNAME::NetCurrentDesktop, "_NET_CURRENT_DESKTOP");
    xconnection.addAtom(ATOMNAME::NetSupported, "_NET_SUPPORTED");
    xconnection.addAtom(ATOMNAME::UTF8String, "UTF8_STRING");

    xconnection.addAtom(ATOMNAME::WindowNormalState, "NormalState");
    xconnection.addAtom(ATOMNAME::WindowIconicState, "IconicState");
    xconnection.addAtom(ATOMNAME::WindowState, "WM_CHANGE_STATE");

    xconnection.addAtom(ATOMNAME::WindowFullscreenState, "_NET_WM_STATE_FULLSCREEN");


    xconnection.addAtom(ATOMNAME::WindowType, "_NET_WM_WINDOW_TYPE");
    xconnection.addAtom(ATOMNAME::WindowTypeNormal, "_NET_WM_WINDOW_TYPE_NORMAL");
    xconnection.addAtom(ATOMNAME::WindowTypeDock, "_NET_WM_WINDOW_TYPE_DOCK");
    xconnection.addAtom(ATOMNAME::WindowTypeDialog, "_NET_WM_WINDOW_TYPE_DIALOG");
    xconnection.addAtom(ATOMNAME::WindowTypeSplash, "_NET_WM_WINDOW_TYPE_SPLASH");
    xconnection.addAtom(ATOMNAME::WindowTypePanel, "_NET_WM_WINDOW_TYPE_PANEL");

    xconnection.initAtoms();
    xconnection.setactiveColor(Config._configs.activeColor);
    xconnection.setpassiveColor(Config._configs.passiveColor);
    xconnection.seturgentColor(Config._configs.urgentColor);

    xconnection.updateDesktopHint(0);

    
    Run runloop; 

    runloop.setHandler(KeyPress, new keyHandlerAction);
    runloop.setHandler(MapRequest, new ManageRequestAction);
    runloop.setHandler(UnmapNotify, new UnmanageRequestAction);
    runloop.setHandler(EnterNotify, new EnterNotifyAction);
    runloop.setHandler(DestroyNotify, new DestroyNotifyAction);
    runloop.setHandler(MappingNotify, new MappingNotifyAction);
    runloop.setHandler(ConfigureRequest, new ConfigureRequestAction);
    //runloop.setHandler(UnMap, Action *act)


    // Set Actions for Events

    Log.Info("Entering Main Loop");
    runloop.startloop();
    Log.Info("Exited main run loop, prepare to shut down");


    {
    auto a  = WMM.getWindows();

    for (auto c : a) {

	xconnection.Unmapwindow(c);
	xconnection.sendEvent(c, ATOMNAME::WMDelete);
    }
    

    }




    exit(0);
}
