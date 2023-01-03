/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul Application (wxWidgets)

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
 * Name:      RUZmBIApp.h
 * Purpose:   Defines Application Class
 * Author:    Ansgar Rütten ()
 * Created:   2018-08-25
 * Copyright: Ansgar Rütten ()
 * License:
 **************************************************************/

#ifndef RUZMBIAPP_H
#define RUZMBIAPP_H

#include <wx/app.h>
#include "RUZmBIMain.h"

class RUZmBIApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // RUZMBIAPP_H
