/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul ThreadControl (wxWidgets)

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
 * Name:      RUZmBIMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ansgar Rütten ()
 * Created:   2018-08-25
 * Copyright: Ansgar Rütten ()
 * License:
 **************************************************************/

#ifndef RUZTHREADCTRL_H
#define RUZTHREADCTRL_H

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

class MyPanel: public wxPanel
{
public:
	MyPanel(thread_info_verschnitt*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&);
	~MyPanel();
	DECLARE_EVENT_TABLE()
private:
  void OnPaint(wxPaintEvent& event);
  RUZ_Layer *hilfsLayer, *randLayer1, *randLayer2, *Layer1, *Layer2;

  thread_info_verschnitt *thInf;
};

class RUZThreadCtrl: public wxDialog
{
public:
  RUZThreadCtrl(thread_info_verschnitt *_thInf, int _timerTick, wxWindow *parent,
    wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition,
    const wxSize &size=wxDefaultSize);
  ~RUZThreadCtrl();

private:
  thread_info_verschnitt *thInf;
  wxBoxSizer *sizerButtons, *sizerText, *sizerHaupt;
  MyPanel *pAnzeige;

  /*Eventhandling*/
  void OnTimer(wxTimerEvent &event);
  void OnAbbruch(wxCommandEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnEraseBackground(wxEraseEvent &event);

  wxTimer timer;
  int TIMERTICK;

  DECLARE_EVENT_TABLE()
};

#endif //RUZTHREADCTRL_H