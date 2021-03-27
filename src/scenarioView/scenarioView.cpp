#include <psp2/ctrl.h>
#include <random>
#include <imgui_vita2d/imgui_vita.h>
#include <vita2d.h>
#include <stdio.h>
#include "sqlite3.h"

#include "scenarioView.hpp"
#include "../view.hpp"
#include "../context.hpp"
#include "../scenario.hpp"
#include "../scenarioContext.hpp"
#include "msg.hpp"
#include "db.hpp"
#include "main.hpp"
#include "scenarioExplorer/scenarioExplorer.hpp"
#include "mainView/mainView.hpp"
#define UILOCALE context.ui_locale
std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator

#define ROW_HEIGHT 59
#define ROW_MARGIN 12

bool main_inputs_blocked = false;
void TextCenter(std::string text) {
	float font_size = ImGui::GetFontSize() * text.size() / 2;
	ImGui::SameLine(
	ImGui::GetColumnWidth() / 2 -
	font_size + (font_size / 2)
	);

	ImGui::Text(text.c_str());
}
std::string debug_out;
Scenario tmp_scenario;
ScenarioView::ScenarioView(Context context, Scenario scenario) {
	this->context = context;
	this->scenario = scenario;
	auto it = scenarioContexts.find(scenario.context_id);
	if (it == scenarioContexts.end()) {
		show_msg(
			std::string("Context ") + scenario.context_id + " does not exist",
			"Fatal error"
		);
		exit_app();
	}
	
	this->scenario_context = it->second;
	initialized = false;
	
}

std::string scenario_label;
void ScenarioView::init() {
	if ( scenario_context.locales.count(context.lang) < 1 ) {
		locale = &scenario_context.locales.begin()->second;	
	} else {
		locale = &scenario_context.locales[context.lang];
	}
	scenario_label = scenario.label;
	initialized = true;

    

}

int save_result_popup = 0; // 0 = Hidden, 1 = Success, 2 = Failure

void ime_callback_set_label(int ime_result) {
	show_msg("called", "ICSL");
	if (ime_result = IME_DIALOG_RESULT_FINISHED) {
		char *new_value = (char *)Dialog::getImeDialogInputTextUTF8();
		tmp_scenario.label.assign(new_value);
		int result = DB::add_scenario(tmp_scenario);
		save_result_popup = result+1;
	}
}

void ScenarioView::loop(SceCtrlData pad, SceCtrlData pad_prev) {	
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Once);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("Mario Kart Randomizer", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {

		//ImGui::BeginChild("scenarioWin", ImVec2(950, 544));
		ImGui::Spacing();
		ImGui::Columns(2);
		//ImGui::SetWindowFontScale(1.5f);
		for (int i=0; i < locale->settings.size(); i++) {
			ImGui::Text(locale->settings[i].c_str());
		}
		ImGui::NextColumn();
		for (int i=0; i < locale->options.size(); i++) {
			ImGui::Text(locale->options[i][scenario.settings[i]].c_str());
		}
		ImGui::Columns(1);
		//ImGui::EndChild();
		if ( ImGui::BeginPopupModal(UILOCALE.save_result_title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize) ) {
			//show_msg("Rendering InputText", "Debug");
			switch (save_result_popup) {
				case 1:
					ImGui::Text(UILOCALE.save_result_success.c_str());
					break;
				case 2:
					ImGui::Text(UILOCALE.save_result_error.c_str());
					break;
				default:
					ImGui::Text(("Unknown error: " + std::to_string(save_result_popup)).c_str());
					break;
			}

			if (ImGui::Button("Ok")) {
				save_result_popup = 0;
				main_inputs_blocked = false;
				ImGui::CloseCurrentPopup();
			}	
			

			ImGui::EndPopup();
		}
		if ( !(pad_prev.buttons & SCE_CTRL_CROSS) && (pad.buttons & SCE_CTRL_CROSS) && !main_inputs_blocked ) {
			for (int i = 0; i < scenario.settings.size(); i++) {
				scenario.settings[i] = std::uniform_int_distribution<>{
					0, scenario_context.options.setting_options[i]-1
				}(gen);
			}
		}

		if ( !(pad_prev.buttons & SCE_CTRL_TRIANGLE) && (pad.buttons & SCE_CTRL_TRIANGLE) && !main_inputs_blocked) {
			main_inputs_blocked = true;
			ime_callback = ime_callback_set_label;
			IME_MODE = true;
			tmp_scenario = scenario;
			Dialog::initImeDialog(UILOCALE.save_scenario_title.c_str(), UILOCALE.save_scenario_placeholder.c_str(), 32, SCE_IME_TYPE_DEFAULT, 0,0);
			main_inputs_blocked = false;
		}
		if ( !(pad_prev.buttons & SCE_CTRL_SQUARE) && (pad.buttons & SCE_CTRL_SQUARE) && !main_inputs_blocked) {
			ImGui::End();
			ImGui::PopStyleVar();
			current_view = new ScenarioExplorer(context);
			return;
		}
		if ( !(pad_prev.buttons & SCE_CTRL_CIRCLE) && (pad.buttons & SCE_CTRL_CIRCLE) && !main_inputs_blocked) {
			ImGui::End();
			ImGui::PopStyleVar();
			current_view = new MainView(context);
			return;
		}
		if (!ImGui::IsPopupOpen(UILOCALE.save_result_title.c_str()) && save_result_popup > 0) {
			ImGui::OpenPopup(UILOCALE.save_result_title.c_str());
		}

		ImGui::SetCursorPosY(480);
		ImGui::Separator();
		ImGui::Text(UILOCALE.randomizer_controls.c_str());


	
	ImGui::End();
	}
	
	ImGui::PopStyleVar();
}
