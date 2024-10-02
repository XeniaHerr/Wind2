#ifndef CONCRETE_ACTIONS_H
#define CONCRETE_ACTIONS_H
#include <Action.h>
#include <memory>
#include <run.h>




/**
 * @file These are all Systemfunctions that the Windowmanager can do*/


namespace Wind {

class quitAction : public Action {


    public:

    Run* loop;
    std::string name() override;


    void execute() override;

    std::unique_ptr<Action> clone() override;



    bool wantArgument() override;


    void operator()() override;


    ~quitAction() override {}






};


class spawnAction : public Action {

    public:

	
    std::string name() override;


    void execute() override;

    std::unique_ptr<Action> clone() override;



    bool wantArgument() override;


    void operator()() override;


    ~spawnAction() override {}


};


class closeAction : public Action {

    public:

    std::string name() override;


    void execute() override;

    std::unique_ptr<Action> clone() override;



    bool wantArgument() override;


    void operator()() override;


    ~closeAction() override {}
};


class LayoutSwitchAction : public Action {

    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~LayoutSwitchAction() {}
};


class TopicSwitchAction : public Action {

    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~TopicSwitchAction() {}
};

class FullscreenAction : public Action {

    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~FullscreenAction() {}
};


class KeyMoveVertAction : public Action {


    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~KeyMoveVertAction() {}

};

class KeyMoveHorAction : public Action {


    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~KeyMoveHorAction() {}

};


class KeyResizeVertAction : public Action {

    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~KeyResizeVertAction() {}

};

class KeyResizeHorAction : public Action {

    public:

	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~KeyResizeHorAction() {}

};

class ToggleFloatingAction : public Action {


    public:


	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~ToggleFloatingAction() {}

};
}


#endif /*CONCRETE_ACTIONS_H*/
