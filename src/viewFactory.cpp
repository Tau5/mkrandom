#include "view.hpp"
#include "viewFactory.hpp"
#include "scenarioView/scenarioView.hpp"

void viewFactory::view(View* target, Context context) {
	*target = View(context);
}

void viewFactory::scenarioView(View* target, Context context, Scenario scenario) {
	*target = ScenarioView(context, scenario);
}