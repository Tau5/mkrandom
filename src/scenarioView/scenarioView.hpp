#ifndef __SCENARIOVIEW_H__
#define __SCENARIOVIEW_H__
#include "../scenario.hpp"
#include "../context.hpp"
#include "../scenarioContext.hpp"
#include "../view.hpp"

class ScenarioView: public View {
	private:
		Scenario scenario;
		ScenarioContext scenario_context;
		ScenarioLocale* locale;
	public:
		ScenarioView(Context context, Scenario scenario);
		void init();
		void loop(SceCtrlData pad, SceCtrlData pad_prev);
};

#endif // __SCENARIOVIEW_H__