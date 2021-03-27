#include <istream>
#include <sstream>
#include "db.hpp"
#include "sqlite3.h"
#include "msg.hpp"
bool IS_OPEN = false;

namespace DB {
	std::string stringify_settings(std::vector<int> settings) {
		std::string settings_str;
		for (int i = 0; i < settings.size(); i++) {
			if (i == 0) {
				settings_str += std::to_string(settings[i]);
			} else {
				settings_str += ","+std::to_string(settings[i]);
			}
		}
		return settings_str;
	}
	std::vector<int> parse_settings_str(std::string settings_str) {
		std::istringstream stream(settings_str);
		std::vector<int> settings;
		std::string buffer;
		while (std::getline(stream, buffer, ',')) {
			settings.push_back(std::stoi(buffer));
		}
		return settings;
	}
	void open_db(sqlite3** db) {
		sqlite3_open("ux0:/data/MKRANDOM0/scenarios.db", db);
	}

    int init() {
		
		sqlite3* db;
		sqlite3_open("ux0:/data/MKRANDOM0/scenarios.db", &db);
		show_msg("Opening database => " + std::to_string(sqlite3_extended_errcode(db)), "SQLite Init");
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db,
			"CREATE TABLE IF NOT EXISTS scenarios (context_id text, label text, settings text);",
			-1, &stmt, NULL
		);
		sqlite3_step(stmt);

		sqlite3_close(db);
		return 0;
	}
	int end() {
		return 0;
	}
    std::vector<Scenario> get_scenarios(std::string context_id) {
		sqlite3* db;
		open_db(&db);

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db,
			"SELECT * FROM scenarios WHERE context_id = ?",
			-1, &stmt, NULL
		);
		sqlite3_bind_text(stmt, 1, context_id.c_str(), -1, NULL);
		std::vector<Scenario> scenarios;

		while ( sqlite3_step(stmt) == SQLITE_ROW) {
			Scenario scenario;

			scenario.context_id = context_id;

			const unsigned char* column = sqlite3_column_text(stmt, 1);
			scenario.label = std::string(reinterpret_cast<const char*>(column));
			
			column = sqlite3_column_text(stmt, 2);
			std::string settings_str = reinterpret_cast<const char*>(column);
			scenario.settings = parse_settings_str(settings_str);
			scenario.setting_num = scenario.settings.size();
			scenarios.push_back(scenario);
		}
		sqlite3_close(db);
		return scenarios;
		
	}
	int add_scenario(Scenario scenario) {
		sqlite3* db;
		open_db(&db);

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db,
			"INSERT INTO scenarios VALUES (?,?,?);",
			-1, &stmt, NULL
		);
		sqlite3_bind_text(stmt, 1, scenario.context_id.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 2, scenario.label.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 3, stringify_settings(scenario.settings).c_str(), -1, NULL);
		int res = sqlite3_step(stmt);
		if (res != SQLITE_DONE) {
			show_msg((std::to_string(res)+", "+sqlite3_errmsg(db)).c_str(), "SQLite error");
			sqlite3_close(db);
			return 1;
		}
		sqlite3_close(db);
		return 0;
	}
    int del_scenario(Scenario scenario) {
		sqlite3* db;
		open_db(&db);
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db,
			"DELETE FROM scenarios WHERE context_id = ? AND label = ? AND settings = ?;",
			-1, &stmt, NULL
		);

		sqlite3_bind_text(stmt, 1, scenario.context_id.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 2, scenario.label.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 3, stringify_settings(scenario.settings).c_str(), -1, NULL);

		if (sqlite3_step(stmt) != SQLITE_OK) {
			show_msg(sqlite3_errmsg(db), "SQLite error");
			sqlite3_close(db);
			return 1;
		}
		sqlite3_close(db);
		return 0;
	}

    std::vector<std::string> get_contexts() {
		sqlite3* db;
		open_db(&db);

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db,
			"SELECT DISTINCT context_id FROM scenarios;",
			-1, &stmt, NULL
		);
		std::vector<std::string> contexts;
		while ( sqlite3_step(stmt) == SQLITE_ROW) {
			std::string context = reinterpret_cast<const char*>(
				sqlite3_column_text(stmt, 0)
			);
			contexts.push_back(context);
		}
		sqlite3_close(db);
		return contexts;
	}
}