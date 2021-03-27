#ifndef VIEWFACTORY_H
#define VIEWFACTORY_H
#include "scenarioContext.hpp"
#include "context.hpp"

class View;
namespace viewFactory {
	void view(View* target, Context context);
	void scenarioView(View* target, Context context, Scenario scenario);
}

#endif