#ifndef __UILOCALE_H__
#define __UILOCALE_H__
#include <string>
#include <map>
#include "scenarioLocale.hpp"
#include <psp2/system_param.h>

extern std::map<SceSystemParamLang, LocaleID> langs;

struct UILocale {
	std::string title;
	std::string select_a_game_or_context;
	std::string randomizer_controls;
	std::string save_scenario_title;
	std::string save_scenario_placeholder;
	std::string save_result_title;
	std::string save_result_success;
	std::string save_result_error;
	std::string ok;
	std::string explorer_scenarios_controls;
};

extern std::map<LocaleID, UILocale> ui_locales;
#endif // __UILOCALE_H__