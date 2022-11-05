/*
 *  Kchmviewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2004-2014 George Yunaev, gyunaev@ulduzsoft.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KDE_QT_H
#define KDE_QT_H

#if defined (USE_KDE)

	#include <k4aboutdata.h>
	#include <kapplication.h>   // KDELibs4Support
	#include <kmainwindow.h>
	#include <kcmdlineargs.h>   // KDELibs4Support
	#include <kfiledialog.h>    // KDELibs4Support
	#include <kmessagebox.h>
	#include <krun.h>

#else

	#include <QApplication>
	#include <QFileDialog>
	#include <QDesktopServices>

#endif

#endif /* KDE_QT_H */
