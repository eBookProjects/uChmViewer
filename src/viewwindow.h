#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

// We support both engines
#if defined (USE_WEBKIT)
	#include "qtwebkit/viewwindow.h"
#else
	#include "qtwebengine/viewwindow.h"
#endif

#endif // VIEWWINDOW_H
