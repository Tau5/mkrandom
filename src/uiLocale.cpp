#include "uiLocale.hpp"

std::map<SceSystemParamLang, LocaleID> langs {
	{SCE_SYSTEM_PARAM_LANG_ENGLISH_GB, 	"en"},
	{SCE_SYSTEM_PARAM_LANG_ENGLISH_US, 	"en"},
	{SCE_SYSTEM_PARAM_LANG_SPANISH,		"es"}
};


UILocale ui_locale_en = {
	"Mario Kart randomizer",
	"Select a game/context",
	"X: Randomize /\\: Save scenario []: Scenario explorer",
	"Name for the scenario",
	"New scenario",
	"Save scenario",
	"Scenario saved successfully",
	"There was an error saving the scenario",
	"Ok",
	"O: Back /\\: Delete scenario"
};

UILocale ui_locale_es = {
	"Aleatorizador de Mario Kart",
	"Selecciona un juego/contexto",
	"X: Aleatorizar /\\: Guardar escenario []: Explorador de escenarios",
	"Nombre para el escenario",
	"Nuevo escenario",
	"Guardar escenario",
	"Escenario guardado satisfactoriamente",
	"Ha ocurrido un error al guardar el escenario",
	"Ok",
	"O: Atrás /\\: Borrar escenario"
};

std::map<LocaleID, UILocale> ui_locales {
	{"en", ui_locale_en},
	{"es", ui_locale_es}
};
