#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <imgui_vita2d/imgui_vita.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <vita2d.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include "scenario.hpp"
#include "context.hpp"
#include "scenarioContext.hpp"
#include "mainView/mainView.hpp"
#include "msg.hpp"
#include "sqlite3.h"
#include "db.hpp"
#include "ime_dialog.h"
#include "math.h"
#include <psp2/system_param.h> 
#include <psp2/apputil.h> 
#include "uiLocale.hpp"

#define printf psvDebugScreenPrintf
#define WIDTH	960
#define HEIGHT	544
#define OPTIONS	7
#define UI_SCALE 2.25f
SceCtrlData pad_prev;
SceCtrlData pad;
View *current_view;
int IME_MODE = false;
ime_callback_t ime_callback;
/*Scenario generate_scenario() {

}*/
void recursive_mkdir(const std::string& ppath) {
        std::string path = ppath;
        path.push_back('/');
        auto ptr = path.begin();
        while (true) {
            ptr = std::find(ptr, path.end(), '/');
            if (ptr == path.end())
                break;

            char last = *ptr;
            *ptr = 0;
            int err = sceIoMkdir(path.c_str(), 0777);
            *ptr = last;
            ++ptr;
        }
    }

void reload_font() {
	static const ImWchar ranges[] = {
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0100, 0x024F, // Latin Extended
		0x0370, 0x03FF, // Greek
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x0590, 0x05FF, // Hebrew
		0x1E00, 0x1EFF, // Latin Extended Additional
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0,
	};
	ImGui::GetIO().Fonts->AddFontFromFileTTF("app0:/resources/NotoSansDisplay-Light.ttf", std::round(16.0f * UI_SCALE), NULL, ranges);
	ImGui::GetStyle().ScaleAllSizes(UI_SCALE);
}

void errorLogCallback(void *pArg, int iErrCode, const char *zMsg){
  show_msg(std::to_string(iErrCode)+zMsg, "SQLite native error");
}

int main(int argc, char *argv[]) {

	msg_init();
	
	//sceAppUtil
	sceSysmoduleLoadModule(SCE_SYSMODULE_APPUTIL); 
	SceAppUtilInitParam init_param;
	SceAppUtilBootParam boot_param;
	memset(&init_param, 0, sizeof(SceAppUtilInitParam));
	memset(&boot_param, 0, sizeof(SceAppUtilBootParam));
	sceAppUtilInit(&init_param, &boot_param);

	SceSystemParamLang sce_lang;
	int res = sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int*)&sce_lang);

	Context context;
	if ( langs.count(sce_lang) < 1 ) {
		context.lang = langs.begin()->second;
	} else {
		context.lang = langs[sce_lang];
	}

	if ( ui_locales.count(context.lang) < 1 ) {
		context.ui_locale = ui_locales.begin()->second;
	} else {
		context.ui_locale = ui_locales[context.lang];
	}
	
	sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback);
	sqlite3_os_init();
	//sqlite3_initialize();
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	// Setup ImGui binding
	ImGui::CreateContext();
	reload_font();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplVita2D_Init();
	// Setup style
	ImGui::StyleColorsDark();
	ImGui_ImplVita2D_TouchUsage(true);
	ImGui_ImplVita2D_UseIndirectFrontTouch(false);
	ImGui_ImplVita2D_UseRearTouch(false);
	ImGui_ImplVita2D_GamepadUsage(true);
	ImGui_ImplVita2D_MouseStickUsage(false);
	ImGui_ImplVita2D_DisableButtons(SCE_CTRL_SQUARE);
	ImGui_ImplVita2D_SetAnalogRepeatDelay(50000);
	io.MouseDrawCursor = false;

	// Check if ux0:/data/MKRANDOM0 exists, if not, create it
	SceIoStat stat;
	sceIoGetstat("ux0:/data/MKRANDOM0", &stat);
	if (!(stat.st_mode & SCE_S_IFDIR)) {
		recursive_mkdir("ux0:/data/MKRANDOM0");
	}

	DB::init();

	current_view = new MainView(context);

	bool done = false;
	bool show_demo = true;
	while (!done) {
			vita2d_start_drawing();
			vita2d_clear_screen();
			sceCtrlReadBufferPositiveExt2(0, &pad, 1);
			if (IME_MODE) {
				int status = Dialog::updateImeDialog();
				if (status == IME_DIALOG_RESULT_FINISHED || status == IME_DIALOG_RESULT_CANCELED) {
					if (ime_callback != nullptr) {
						ime_callback(status);
					} else {
						show_msg("Finished/Canceled detected, but callback is null", "IME_MODE");
					}
					IME_MODE = false;
				}
			} else {
				ImGui_ImplVita2D_NewFrame();

				if (!current_view->initialized) {
					show_msg("Initializing view", "debug");
					current_view->init();
					show_msg("First loop function of view", "debug");
				}
				current_view->loop(pad, pad_prev);
				/* UNCOMMENT FOR DEBUG WINDOW
				ImGui::Begin("Debug", NULL, ImGuiWindowFlags_AlwaysAutoResize);
					ImGui::Text(msg_getlog().c_str());
				ImGui::End();
				*/

				ImGui::Render();

				ImGui_ImplVita2D_RenderDrawData(ImGui::GetDrawData());
			}
			pad_prev = pad;
			vita2d_end_drawing();
			vita2d_common_dialog_update();
			vita2d_swap_buffers();
			sceDisplayWaitVblankStart();
		}
	

	DB::end();

	ImGui_ImplVita2D_Shutdown();
	ImGui::DestroyContext();
	vita2d_fini();
	
	return 0;
}
