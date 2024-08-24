#include <X11/X.h>
#include <Action.h>
#include <array>



namespace Wind {

    class Run {
	public:

	    static Run& getInstance();


	    Run(Run& other) = delete;
	    Run(Run&& other) = delete;
	    
	    Run& operator=(Run& other) = delete;
	    Run& operator=(Run&& other) = delete;

	    void stop();

	    void startloop();


	    void setHandler(unsigned index, Action* act);
	    
	    const bool isrunning() {return _is_running;}

	private:

	    Run() : _is_running(true) {}


	    bool _is_running;

	    std::array<Action*, LASTEvent> _handlers;
    };
} 
