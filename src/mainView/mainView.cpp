#include "mainView.hpp"
#include "main.hpp"
#include "scenarioView/scenarioView.hpp"
#include "scenarioExplorer/scenarioExplorer.hpp"
#include "imgui_vita2d/imgui_impl_vita2d.h"
#include "imgui_vita2d/imgui_internal.h"
#define UILOCALE context.ui_locale
enum mainview_subviews {
	SUBVIEW_MAIN,
	SUBVIEW_GENERATOR
};

MainView::MainView(Context context) {
	this->context = context;
	this->subview = SUBVIEW_MAIN;
}
vita2d_texture* generator_btn_tex 	= nullptr;
vita2d_texture* explorer_btn_tex 	= nullptr;
ImGuiStyle style;
void MainView::init() {
	generator_btn_tex 	= vita2d_load_PNG_file("app0:/resources/generator.png");
	explorer_btn_tex 	= vita2d_load_PNG_file("app0:/resources/explorer.png");
	style = ImGui::GetStyle();
	initialized = true;
}



void MainView::loop(SceCtrlData pad, SceCtrlData pad_prev) {
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Once);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	if ( ImGui::Begin("Main menu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar) ) {
		switch (subview) {
			case SUBVIEW_MAIN:
				ImGui::Text(UILOCALE.title.c_str());
				ImGui::Separator();
				if ( ImGui::ImageButtonEx(ImGui::GetID("GeneratorBtn"), reinterpret_cast<ImTextureID>(generator_btn_tex), 
					ImVec2(16*22, 16*22) 
					, ImVec2(0,0), ImVec2(1,1), style.FramePadding, ImVec4(0,0,0,0), ImVec4(1,1,1,1))
					) {
						subview = SUBVIEW_GENERATOR;
				}
				ImGui::SameLine(ImGui::GetWindowWidth()-(16*24 + style.WindowPadding.x/2));
				if ( ImGui::ImageButtonEx(ImGui::GetID("ExplorerBtn"), reinterpret_cast<ImTextureID>(explorer_btn_tex), 
					ImVec2(16*22, 16*22) 
					, ImVec2(0,0), ImVec2(1,1), style.FramePadding, ImVec4(0,0,0,0), ImVec4(1,1,1,1))
					)  {
						ImGui::End();
						ImGui::PopStyleVar();
						current_view = new ScenarioExplorer(context);
						return;
				}
				break;
			case SUBVIEW_GENERATOR:
				ImGui::Text(UILOCALE.select_a_game_or_context.c_str());
				ImGui::Separator();
				for (auto const& pair : scenarioContexts) {
					if ( ImGui::Button(pair.second.label.c_str(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()/4)) ) {
						std::vector<int> options;
						for (int i=0;i<pair.second.options.setting_num;i++) options.push_back(0);
						Scenario scenario {
							pair.second.context_id,
							UILOCALE.save_scenario_placeholder,
							pair.second.options.setting_num,
							options
						};
						ImGui::End();
						ImGui::PopStyleVar();
						current_view = new ScenarioView(context, scenario);
						return;
					}
				}
				if ( !(pad_prev.buttons & SCE_CTRL_CIRCLE) && (pad.buttons & SCE_CTRL_CIRCLE)) {
					subview = SUBVIEW_MAIN;
				}
				break;
		}
		
		ImGui::End();
	}
	ImGui::PopStyleVar();
}