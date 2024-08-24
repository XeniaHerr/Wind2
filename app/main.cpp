#include <X11/X.h>
#include <run.h>
#include <X11_Abstraction.h>
#include <WindowManagerModel.h>
#include <ConfigReader.h>
#include <Logger.h>


int main() {

    using namespace Wind;

    Logger& Log = Logger::GetInstance();

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
    
    std::string configpath = "";
    Config.read(configpath);

    if (Config.empty) {
	Log.Error("Couldn't read Config at filepath {}", configpath);
    }



    WMM.loadConfig();


    Run& runloop = Run::getInstance();

    // Set Actions for Events

    runloop.startloop();


    // Do some cleanup




    exit(0);
}
