#ifndef __SCENARIOLOCALE_H__
#define __SCENARIOLOCALE_H__
#include <string>
#include <vector>

// Identificator for a language
typedef std::string LocaleID;
// Strings for each setting label
typedef std::vector<std::string> SettingsStrings;
// Strings for each setting option
typedef std::vector<std::vector<std::string>> OptionsStrings;

// Set of strings to parse the Scenario data into the selected language
struct ScenarioLocale {
	LocaleID lang; // ID of the language
	OptionsStrings options; // Strings for each setting's option
	SettingsStrings settings; // Strings for each setting
};

#endif // __SCENARIOLOCALE_H__