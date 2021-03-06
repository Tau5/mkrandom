#include "msg.hpp"
#include <vita2d.h>
#include <imgui_vita2d/imgui_vita.h>
#include <psp2/shutter_sound.h> 
#include <psp2/ctrl.h>
#include <chrono>
#include <thread>
#include "sqlite3.h"
#include <string>
#include "db.hpp"
/*
#define NET_MEMORY_SIZE (4 * 1024 * 1024)
char *net_memory = nullptr;
std::string addr = "192.168.0.160:7069";
*/
std::string debug_log;
void msg_init() {

}
int sound = 0;
void show_msg(std::string message, std::string title) {
	debug_log += title + ": " + message + "\n";
}

void exit_app(bool close_db) {
	if (close_db) {
		DB::end();
	}
	ImGui_ImplVita2D_Shutdown();
	ImGui::DestroyContext();
	
	vita2d_fini();
	sceKernelExitProcess(1);
}

std::string msg_getlog() {
	return debug_log;
}