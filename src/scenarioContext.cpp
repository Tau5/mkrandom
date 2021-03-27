#include "scenarioContext.hpp"
#include "scenarioLocales.hpp"
#include <map>
std::map<std::string, ScenarioContext> scenarioContexts {
	{"mk8", 
		{
			"mk8", "Mario Kart 8",
			7, {2, 8, 4, 4, 3, 3, 7},
			{ 
				{ "es", MK8_es },
				{ "en", MK8_en }
			}
		}
	},
	{"mkwii",
		{
			"mkwii", "Mario Kart Wii",
			6, {3,3,3,3,4,9},
			{ 
				{ "es", MKWii_es },
				{ "en", MKWii_en }
			}
		}
	}
};

