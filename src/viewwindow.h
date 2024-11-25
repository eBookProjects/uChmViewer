#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

// We support both engines
#if defined (USE_WEBENGINE)
	#include "qtwebengine/viewwindow.h" // IWYU pragma: export
#else
	#include "qtwebkit/viewwindow.h"    // IWYU pragma: export
#endif

#endif // VIEWWINDOW_H
