#ifndef DEFAULT_RENDERER_H
#define DEFAULT_RENDERER_H
#include "Logger.h"
#include <AbstractRenderer.h>
#include <X11_Abstraction.h>
#include <Topic.h>
#include <Client.h>


namespace Wind {

class DefaultRenderer : public AbstractRenderer {


    public:
	void render(Monitor& m) override {

	    for (auto a : m.getCurrent()->getClients()) {

		a->setDimensions(a->getTargetDimension());
		a->setPosition(a->getTargetPosition());
		Logger::GetInstance().Info("Setting values for a client");
	    }


	    Logger::GetInstance().Info("All clients have been updated");

	    X11Abstraction::getInstance().drawMonitor(m);
	}


};

}


#endif /*DEFAULT_RENDERER_H*/
