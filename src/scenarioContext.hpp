#ifndef __SCENARIOCONTEXT_H__
#define __SCENARIOCONTEXT_H__
#include "scenario.hpp"
#include "scenarioLocale.hpp"
#include <vector>
#include <map>

// Stores relevant information in order to parse the Scenario
struct ScenarioContext {
	std::string context_id;
	std::string label;
	ScenarioOptions options;
	std::map<LocaleID, ScenarioLocale> locales;
};

// Standard Scenario contexts
extern std::map<std::string, ScenarioContext> scenarioContexts;
#endif // __SCENARIOCONTEXT_H__