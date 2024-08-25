/*	RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
		Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
		Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
		Copyright (C) 2016-2024	Ansgar Rütten

		Modul ThreadControl (wxWidgets)

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.	If not, see <http://www.gnu.org/licenses/>.*/

/***************************************************************
 * Name:		RUZThCtrl.h
 * Purpose:		Defines Application Frame
 * Author:		Ansgar Rütten ()
 * Created:	 	2018-08-25
 * Copyright:	Ansgar Rütten ()
 * License:
 **************************************************************/

#ifndef RUZ_TH_CTRL_H
#define RUZ_TH_CTRL_H

#ifndef WX_PRECOMP
		#include <wx/wx.h>
#endif

#include <iostream>
#include <thread>

#include <wx/sizer.h>
#include "RUZ\RUZVerwaltung.h"

enum threadCtrlID{
	tc_abbruchID = 3000, tc_timerID
};

class ThCtrlPanel: public wxPanel
{
public:
	ThCtrlPanel(thread_info*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&);
	~ThCtrlPanel();
private:
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent &event);

	thread_info *thInf;
};

class RUZThCtrl: public wxDialog
{
public:
	RUZThCtrl(thread_info *_thInf, int _timerTick, wxWindow *parent,
	wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition,
	const wxSize &size=wxDefaultSize);
	~RUZThCtrl();

private:
	thread_info *thInf;
	wxBoxSizer *sizerButtons, *sizerText, *sizerHaupt;
	ThCtrlPanel *pAnzeige;
	RUZ_Layer *m_Layer;

	/*Eventhandling*/
	void OnTimer(wxTimerEvent &event);
	void OnAbbruch(wxCommandEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnEraseBackground(wxEraseEvent &event);

	wxTimer timer;
	int TIMERTICK;
};

#endif //RUZ_TH_CTRL_H
