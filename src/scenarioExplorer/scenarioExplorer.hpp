#ifndef __SCENARIOEXPLORER_H__
#define __SCENARIOEXPLORER_H__

#include "../scenario.hpp"
#include "../context.hpp"
#include "../scenarioContext.hpp"
#include "../view.hpp"

class ScenarioExplorer: public View {
	public:
		ScenarioExplorer(Context context);
		void init();
		void loop(SceCtrlData pad, SceCtrlData pad_prev);
};

#endif // __SCENARIOEXPLORER_H__