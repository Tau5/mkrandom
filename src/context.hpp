#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include <psp2/ctrl.h>
#include <vita2d.h>
#include <imgui_vita2d/imgui_vita.h>
#include "scenarioLocale.hpp"
#include "uiLocale.hpp"

struct Context {
	LocaleID lang;
	UILocale ui_locale;
};

#endif // __CONTEXT_H__