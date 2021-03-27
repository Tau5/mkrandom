#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>
void msg_init();
void show_msg(std::string message, std::string title = "Error");
void exit_app(bool close_db = true);
std::string msg_getlog();
#endif // __ERROR_H__