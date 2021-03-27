#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "../scenario.hpp"
#include "../context.hpp"
#include "../scenarioContext.hpp"
#include "../view.hpp"

class MainView: public View {
	private:
		int subview;
	public:
		MainView(Context context);
		void init();
		void loop(SceCtrlData pad, SceCtrlData pad_prev);
};


#endif // __MAINVIEW_H__