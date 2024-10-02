#ifndef RUN_H
#define RUN_H


#include <X11/X.h>
#include <Action.h>
#include <array>



namespace Wind {

    class Run {
	public:



	    Run(Run& other) = delete;
	    Run(Run&& other) = delete;
	    
	    Run& operator=(Run& other) = delete;
	    Run& operator=(Run&& other) = delete;

	    void stop();

	    void startloop();


	    void setHandler(unsigned index, Action* act);
	    
	    const bool isrunning() {return _is_running;}

	    ~Run();

	    Run() : _is_running(true) {

		for (auto& a : _handlers) {
		    a = nullptr;
		}

	    }
	private:



	    bool _is_running;

	    std::array<Action*, LASTEvent> _handlers;
    };



    class RunningIndicator {

	public:
	bool _var;


	
	    RunningIndicator(RunningIndicator& other) = delete;
	    RunningIndicator(RunningIndicator&& other) = delete;
	    
	    RunningIndicator& operator=(RunningIndicator& other) = delete;
	    RunningIndicator& operator=(RunningIndicator&& other) = delete;



	    static RunningIndicator& getInstance() {

		static RunningIndicator s;
		return s;

	    }

	private:
	    RunningIndicator() : _var(true) {
	    }


    };
} 




#endif
