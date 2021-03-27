#ifndef __DB_H__
#define __DB_H__
#include <string>
#include <vector>
#include "scenario.hpp"

namespace DB {
    int init();
	int end();
    std::vector<Scenario> get_scenarios(std::string context_id);
	int add_scenario(Scenario scenario);
    int del_scenario(Scenario scenario);
    std::vector<std::string> get_contexts();
}
#endif // __DB_H__