#include <X11/X.h>
#include <run.h>
#include <X11_Abstraction.h>
#include <WindowManagerModel.h>
#include <ConfigReader.h>
#include <Logger.h>
#include <ConcreteActions.h>
#include <tuple>
#include <InputManager.h>


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
    
    std::string configpath = "/home/xenia/Projects/Wind2/test/Wind.yaml";
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

    xconnection.setEventMask(SubstructureRedirectMask);

    xconnection.listenforKeys(InputManager::GetInstance().getKeys());


    Run& runloop = Run::getInstance();

    runloop.setHandler(KeyPress, new keyHandlerAction);


    // Set Actions for Events

    Log.Info("Entering Main Loop");
    runloop.startloop();
    Log.Info("Exited main run loop, prepare to shut down");


    // Do some cleanup




    exit(0);
}
