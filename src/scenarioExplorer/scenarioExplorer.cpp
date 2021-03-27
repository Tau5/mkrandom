#include "scenarioExplorer.hpp"
#include "mainView/mainView.hpp"
#include "main.hpp"
#include "db.hpp"
#define UILOCALE context.ui_locale
#include "uiLocale.hpp"
ScenarioExplorer::ScenarioExplorer(Context context) {
	this->context = context;
	initialized = false;	
}

std::vector<std::string> db_contexts; // Vector of context IDs
// Find if the user LocaleID is available and if it's not found, get default LocaleID
LocaleID find_available_locale(LocaleID user_locale, ScenarioContext context) {
	if ( context.locales.count(user_locale) < 1 ) {
		return context.locales.begin()->first;
	} else {
		return user_locale;
	}
}

std::vector<Scenario> cached_scenarios;

void ScenarioExplorer::init() {
	db_contexts = DB::get_contexts();
	initialized = true;
}

enum ScenarioExplorer_SubViews {
	SEXP_SUBVIEW_CONTEXTS,
	SEXP_SUBVIEW_SCENARIOS
};
int subview = SEXP_SUBVIEW_CONTEXTS;
int scenario_hovered = 0;
std::string current_context;
ScenarioLocale cached_locale;
void ScenarioExplorer::loop(SceCtrlData pad, SceCtrlData pad_prev) {
	
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Once);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	if (ImGui::Begin("Scenario explorer", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
		switch (subview) {
			case SEXP_SUBVIEW_CONTEXTS:
				ImGui::Text(UILOCALE.select_a_game_or_context.c_str());
				ImGui::Separator();
				for (int i=0; i < db_contexts.size(); i++) {
					if ( ImGui::Button(scenarioContexts[db_contexts[i]].label.c_str(), ImVec2(ImGui::GetWindowWidth()-ImGui::GetStyle().WindowPadding.x*2, (ImGui::GetWindowHeight()/4)) ) ) {
						current_context = scenarioContexts[db_contexts[i]].context_id;
						cached_scenarios = DB::get_scenarios(current_context);
						subview = SEXP_SUBVIEW_SCENARIOS;
						cached_locale = scenarioContexts[current_context].locales[
							find_available_locale(context.lang, scenarioContexts[current_context])
						];
						scenario_hovered = 0; // Prevent going out of array
					
					}
					if (i == 0) ImGui::SetKeyboardFocusHere();
					
				}
				if ( !(pad_prev.buttons & SCE_CTRL_CIRCLE) && (pad.buttons & SCE_CTRL_CIRCLE)) {
					ImGui::End();
					ImGui::PopStyleVar();
					current_view = new MainView(context);
					return;
				}
				break;
			case SEXP_SUBVIEW_SCENARIOS:
				//scenario_hovered = 0;
				ImGui::SetNextWindowFocus();
				ImGui::BeginChild("scenarios available", ImVec2(ImGui::GetWindowWidth()/3, -ImGui::GetFontSize()*3));
				for (int i=0; i < cached_scenarios.size(); i++) {
					//Selecciona a todos porque tienen el mismo nombre
					ImGui::Selectable(
						(cached_scenarios[i].label+"###scenario"+std::to_string(i)).c_str()
					);
					if (ImGui::IsItemHovered()) {
						scenario_hovered = i;
					}
				}
				ImGui::EndChild();
				ImGui::SameLine();
				
				ImGui::BeginChild("scenario information", ImVec2(0, -ImGui::GetFontSize()*3));
				ImGui::Columns(2);
				for (int i=0; i < cached_locale.settings.size(); i++) {
					ImGui::Text(cached_locale.settings[i].c_str());
				}
				ImGui::NextColumn();
				ImGui::SetColumnWidth(-1, ImGui::GetWindowWidth()/4);
				for (int i=0; i < cached_locale.settings.size(); i++) {
					auto option = cached_scenarios[scenario_hovered].settings[i];
					ImGui::Text(cached_locale.options[i][option].c_str());
				}
				ImGui::EndChild();
				ImGui::SetCursorPosY(ImGui::GetWindowHeight()-ImGui::GetFontSize() *1.5);
				ImGui::Separator();
				ImGui::Text(UILOCALE.explorer_scenarios_controls.c_str());
				if ( !(pad_prev.buttons & SCE_CTRL_CIRCLE) && (pad.buttons & SCE_CTRL_CIRCLE)) {
					subview = SEXP_SUBVIEW_CONTEXTS;
				}
				if ( !(pad_prev.buttons & SCE_CTRL_TRIANGLE) && (pad.buttons & SCE_CTRL_TRIANGLE)) {
					DB::del_scenario(cached_scenarios[scenario_hovered]);
					cached_scenarios = DB::get_scenarios(current_context);
					scenario_hovered = 0;
					if (cached_scenarios.size() < 1) {
						db_contexts = DB::get_contexts();
						subview = SEXP_SUBVIEW_CONTEXTS;
					}
				}
				
				break;
			default:
				break;
		}


		ImGui::End();
	}
	ImGui::PopStyleVar();
}
