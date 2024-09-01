#include "Logger.h"
#include <ConcreteActions.h>
#include <X11/Xlib.h>
#include <cassert>
#include <cstddef>
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



auto LayoutSwitchAction::name() -> std::string {
    return "LayoutSwitch";

}

auto LayoutSwitchAction::wantArgument() -> bool {
    return true;
}


auto LayoutSwitchAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new LayoutSwitchAction);
}


auto LayoutSwitchAction::execute() -> void {

    auto& Log = Logger::GetInstance();
    Log.Info("Inside {}", __func__);

    int direction = std::get<int>(this->Arg);


    Monitor* m = WindowManagerModel::getInstance().getFocusedMon();

    auto& Selector = m->getSelector();

    if ( direction > 0) {
	Selector.next();

	Log.Info("Setting Arranger to {}", Selector.getArranger().name());


    }

    else if (direction < 0) {
	Selector.previous();
	Log.Info("Setting Arranger to {}", Selector.getArranger().name());
    }

    m->arrange();
    //X11Abstraction::getInstance().drawMonitor(*m);

    
}


auto LayoutSwitchAction::operator()() -> void {
    this->execute();
}



auto TopicSwitchAction::name() -> std::string {
    return "SwitchTopic";
}

auto TopicSwitchAction::wantArgument() -> bool {
    return true;
}


auto TopicSwitchAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new TopicSwitchAction);
}


auto TopicSwitchAction::operator()() -> void {
    this->execute();
}


auto TopicSwitchAction::execute() -> void {


    unsigned long target = static_cast<unsigned long>(std::get<int>(this->Arg));


    auto& Log = Logger::GetInstance();
    auto& WMM = WindowManagerModel::getInstance();
    auto& xc = X11Abstraction::getInstance();

    unsigned long orig_count = WMM.getTopic(target)->getClients().size();
    unsigned long orig_old = WMM.getFocusedMon()->getCurrent()->getClients().size();
    Log.Info("Inside TopicSwitchAction, target = {}, Topic = {} ", target, WMM.getTopic(target)->getName());
    Log.Info("Currenlty focused Topic = {}", WMM.getFocusedMon()->getCurrent()->getName());
    Log.Info("Clientcount of current monitor = {}", WMM.getFocusedMon()->getCurrent()->getClients().size());

    if (target > WMM.getTopicCount()) {
	Log.Warn("Topicnumber to big");
	return;
    }

    Log.Info("Clientcount of current monitor = {}", WMM.getFocusedMon()->getCurrent()->getClients().size());
    Topic* old = WMM.getFocusedMon()->getCurrent();
    Topic* targetTopic = WMM.getTopic(target);
    Log.Info("Clientcount of other topic = {}",old->getClients().size());
    xc.hideTopic(old);
    xc.hideTopic(targetTopic);

    Log.Info("Clientcount of current monitor = {}", WMM.getFocusedMon()->getCurrent()->getClients().size());
    Log.Info("Clientcount of other topic = {}",old->getClients().size());
    WMM.moveTopictoMonitor(*WMM.getTopic(target), *WMM.getFocusedMon());

    Log.Info("Clientcount of current monitor = {}", WMM.getFocusedMon()->getCurrent()->getClients().size());
    Log.Info("Clientcount of other topic = {}",old->getClients().size());
    Log.Info("Now focused Topic = {}", WMM.getFocusedMon()->getCurrent()->getName());

    Log.Info("Old = {}, targetTopic = {}, arranging targetTopic definetly", old->getName(), targetTopic->getName());

    WMM.getFocusedMon()->arrange();
    if (old->getHolder()) {
	old->getHolder()->arrange();
	Logger::GetInstance().Info("Arranging other monitor too");
    }
    unsigned int new_count = WMM.getTopic(target)->getClients().size();
    unsigned int new_old = old->getClients().size();

    Log.Info("orig_count = {}, new_count = {}", orig_count,new_count);
    Log.Info("orig_old = {}, new_old = {}", orig_old, new_old);


    assert(orig_count == new_count);


    WMM.focusClient();
    xc.setfocus(nullptr);

    Log.Info("Done with TopicSwitchAction");



}



auto FullscreenAction::name() -> std::string {
    return "Fullscreen";
}


auto FullscreenAction::wantArgument() -> bool {
    return false;
}


auto FullscreenAction::clone() -> std::unique_ptr<Action> {
    return std::unique_ptr<Action>(new FullscreenAction);
}



auto FullscreenAction::operator()() -> void {
    this->execute();
}


auto FullscreenAction::execute() -> void {
    auto& WMM = WindowManagerModel::getInstance();

    Client* c = WMM.getFocusedMon()->getCurrent()->getFocused();
    Logger::GetInstance().Info("Inside FullscreenAction");



    if (c && c->isManaged() && c->isVisible()) {
	c->toggleFullscreen();
	Logger::GetInstance().Info("Client is now {}", c->isFullscreen() ? "Fullscreen" : "Normal");

	WMM.getFocusedMon()->toggleBar();
	WMM.getFocusedMon()->arrange();
	if (c->isFullscreen()) {
	    X11Abstraction::getInstance().sendClientAtom(c->getWindow(), ATOMNAME::WindowFullscreenState);
	}
	else {
	    X11Abstraction::getInstance().removeClientAtom(c->getWindow(), ATOMNAME::WindowFullscreenState);
	}
//	if (c->isFullscreen()) {
//	c->setDimensions(WMM.getFocusedMon()->getDimensions());
//	c->setPosition(WMM.getFocusedMon()->getPosition());
//	X11Abstraction::getInstance().drawClient(*c);
//	}
//	else {
//	c->setDimensions(c->getOldDimensions());
//	c->setPosition(c->getOldPosition());
//	X11Abstraction::getInstance().drawMonitor(*WMM.getFocusedMon());
//	WMM.getFocusedMon()->arrange();
// 
//	     }

    }

    else {
	if (!c)
	    return;
	Logger::GetInstance().Info("This is not met: {}",c->isManaged() ? "Managed" : "Visibility" );
    }
}
// other Actions


