#ifndef CONCRETE_ACTIONS_H
#define CONCRETE_ACTIONS_H
#include <Action.h>
#include <memory>




/**
 * @file These are all Systemfunctions that the Windowmanager can do*/


namespace Wind {

class quitAction : public Action {

    public:

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



}


#endif /*CONCRETE_ACTIONS_H*/
