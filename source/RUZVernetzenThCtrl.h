/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul VernetzenThreadControl (wxWidgets)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

/***************************************************************
 * Name:      RUZVernetzenThCtrl.h
 * Purpose:   Defines Application Frame
 * Author:    Ansgar Rütten ()
 * Created:   2018-08-25
 * Copyright: Ansgar Rütten ()
 * License:
 **************************************************************/

#ifndef RUZVERNETZEN_TH_CTRL_H
#define RUZVERNETZEN_TH_CTRL_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <iostream>
#include <thread>

#include <wx/sizer.h>
#include "RUZ\RUZVerwaltung.h"

enum threadNetzCtrlID{
  tc_netz_abbruchID = 3000, tc_netz_timerID
};

class VernetzenThCtrlPanel: public wxPanel
{
public:
	VernetzenThCtrlPanel(thread_info_vernetzen*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&);
	~VernetzenThCtrlPanel();
private:
  void OnPaint(wxPaintEvent& event);
  void OnEraseBackground(wxEraseEvent &event);
  RUZ_Layer *m_Layer;

  thread_info_vernetzen *thInf;
};

class RUZVernetzenThCtrl: public wxDialog
{
public:
  RUZVernetzenThCtrl(thread_info_vernetzen *_thInf, int _timerTick, wxWindow *parent,
    wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition,
    const wxSize &size=wxDefaultSize);
  ~RUZVernetzenThCtrl();

private:
  thread_info_vernetzen *thInf;
  wxBoxSizer *sizerButtons, *sizerText, *sizerHaupt;
  VernetzenThCtrlPanel *pAnzeige;

  /*Eventhandling*/
  void OnTimer(wxTimerEvent &event);
  void OnAbbruch(wxCommandEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnEraseBackground(wxEraseEvent &event);

  wxTimer timer;
  int TIMERTICK;
};

#endif //RUZVERNETZEN_TH_CTRL_H
