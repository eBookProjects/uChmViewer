/*
 *  uChmViewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2022  Nick Egorrov, nicegorov@yandex.ru
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

#ifndef BROWSER_TYPES_HPP
#define BROWSER_TYPES_HPP


namespace UBrowser
{

//------------------------------------------------------------------------------
// Start BrowserAPI group.
/// @addtogroup BrowserAPI
/// @{

enum OpenMode {
	/// The link should open in the current tab.
	OPEN_IN_CURRENT,
	/// The link should open in the new foregraund tab.
	OPEN_IN_NEW,
	/// The link should open in the new backgraund tab.
	OPEN_IN_BACKGROUND
};

/// @}
// End BrowserAPI group.
//------------------------------------------------------------------------------

} // namespace UBrowser

#endif // BROWSER_TYPES_HPP
