#ifndef VIEW_H
#define VIEW_H
#include <psp2/ctrl.h>
#include "context.hpp"

class View {
	public:
		bool initialized;
		Context context;
		View(Context context);
		View();
		virtual void loop(SceCtrlData pad, SceCtrlData pad_prev) {}
		virtual void init() {
			initialized = true;
		}
};


#endif