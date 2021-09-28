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
	
	#define KQ_CLASSNAME(name)			K##name
	#define KQ_DECLARECLASS(name)		class KQ##name : public K##name

	#include <kapplication.h>
	#include <kmainwindow.h>
	#include <kstatusbar.h>
	#include <kmenubar.h>
	#include <kcmdlineargs.h>
	#include <klocale.h>
	#include <kfiledialog.h>
	#include <khtml_part.h>
	#include <ktabwidget.h>
	#include <kmenu.h>
	#include <kmessagebox.h>
	#include <kprogressdialog.h>
	#include <krun.h>

	#include <QProgressDialog>

#else /* !USE_KDE */

	#define KQ_CLASSNAME(name)			Q##name

	#include <QTabWidget>
	#include <QProgressDialog>

#endif /* USE_KDE */

class KQProgressModalDialog : public KQ_CLASSNAME(ProgressDialog)
{
	public:
		KQProgressModalDialog ( const QString & captionText, const QString & labelText, const QString & cancelButtonText, int totalSteps, QWidget * creator = 0 );
		
		// Seems like people have fun making classes incompatible
#if defined (USE_KDE)		
		void   setValue( int value ) { progressBar()->setValue( value ); }
#else
		bool   wasCancelled() { return wasCanceled(); }
#endif

};

class KQTabWidget : public KQ_CLASSNAME(TabWidget)
{
public:
	KQTabWidget (QWidget *parent = 0 )
		: KQ_CLASSNAME(TabWidget) (parent) {}
};

#endif /* KDE_QT_H */
