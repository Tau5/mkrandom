#ifndef SCENARIO_H
#define SCENARIO_H 
#include <string>
#include <vector>
#define CONTEXT_ID_MK8 "mk8"
struct Scenario {
	std::string context_id;
	std::string label;
	int setting_num; 			/* Number of settings */
	std::vector<int> settings; 	/* Settings values */
};

// Set of options and max values for the Scenario options
struct ScenarioOptions {
	int setting_num;					/* Number of settings */
	std::vector<int> setting_options; 	/* Max option for each setting */
};





#endif