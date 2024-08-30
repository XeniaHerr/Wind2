#include "Logger.h"
#include <ConcreteActions.h>
#include <X11/Xlib.h>
#include <cstdio>
#include <memory>
#include <run.h>
#include <InputManager.h>
#include "Handlers.h"
#include "WindowManagerModel.h"
#include <unistd.h>
#include <X11_Abstraction.h>


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


auto spawnAction::execute() -> void {
    std::string str = std::get<0>(this->Arg);

    std::vector<char*> command;
    std::vector<std::string> splitted;


    Logger::GetInstance().Info("Trying to spawn the application {}", str);


    size_t a = 0,  b = 0;
    while (a != str.size()) {
	a = str.find_first_of(" ", b);

	if (a == -1) {
	    a = str.size();

	}
	splitted.push_back(std::move(str.substr(b, (a - b))));
	b = a+1;
    }

    for (int i = 0; i < splitted.size(); i++)
	command.push_back(&splitted[i][0]);

    command.push_back(nullptr);

    for(auto a : command) {
	Logger::GetInstance().Info("{}", a  != nullptr ? a : "nullptr" );
    }

    pid_t child;
    child = fork();


    if (child == 0) {

	X11Abstraction::getInstance().closeConnection();

	Logger::GetInstance().Info("Inside Childprocess");

	setsid();


	 int status = execvp(command[0], command.data());



	 if (status) {
	     Logger::GetInstance().Error("Something happened after spawning {}, Code is {}", str, status);
	     perror("execvp failed");
	 }
	exit(0);
    }

    


}


auto spawnAction::name() -> std::string {
    return "spawn";
}

auto spawnAction::wantArgument() -> bool {
    return true;
}


auto spawnAction::operator()() -> void {
    this->execute();
}


auto spawnAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new spawnAction);
}


auto closeAction::name() -> std::string {
return "close";
}


auto closeAction::wantArgument() -> bool {
    return false; //Close the focused client;
}

auto closeAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new closeAction);
}



auto closeAction::execute() -> void {

    auto& WMM = WindowManagerModel::getInstance();
    auto& Log = Logger::GetInstance();
    Log.Info("Preparing to close Client");

    if (!WMM.getFocusedMon() || !WMM.getFocusedMon()->getCurrent()) {

	Log.Warn("Either no FocusedMon or no Client active Topic (not possible)");
	return;
    }


    Client* c = WMM.getFocusedMon()->getCurrent()->getFocused();

    if (!c) {

	Logger::GetInstance().Warn("No focued client to close");
	return;
    }


    Log.Info("Closing window = {}", c->getWindow());
    X11Abstraction::getInstance().sendEvent(c->getWindow(), ATOMNAME::WMDelete);
Log.Info("Done with Closing the Client");




}

auto closeAction::operator()() -> void {
    this->execute();
}

// other Actions


