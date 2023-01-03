/*  RUZmBI - "Rechnerunterst�tztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von H�henlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Fl�chen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar R�tten

    Modul zur Verarbeitung von dxf-Dateien

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

#ifndef __DXF_Handler
#define __DXF_Handler

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cmath>

#include "..\Liste\Verkettete_Liste.h"
#include "..\RUZ\RUZObjekte.h"
#include "..\RUZ\RUZVerwaltung.h"

namespace DXF_Handler
{
    double aru_strtod(char* str);

    /*Logbuch*/
    void logSchreiben(const char* msg, ...);
    /*ENDE Logbuch*/
}

class Char_Speicher
{
public:
    Char_Speicher(const char*);
    const char* HoleInhalt(void) const{return m_inhalt;};
private:
    const static int m_char_MAX = 2050;
    char m_inhalt[m_char_MAX];
};

class DXF_Import
{
private:
    std::ifstream Datei;
    /*Kennungen und Variablen zum Einlesen*/
    const static int m_char_MAX = 2050;
    char Schluesselwort[m_char_MAX], DateiName[256], aktLayer[m_char_MAX];
    char Kennung_X[m_char_MAX], Kennung_Y[m_char_MAX], Kennung_Z[m_char_MAX];
    char x[m_char_MAX], y[m_char_MAX], z[m_char_MAX];
    char x0[m_char_MAX], y0[m_char_MAX], z0[m_char_MAX];
    char x1[m_char_MAX], y1[m_char_MAX], z1[m_char_MAX];
    char x2[m_char_MAX], y2[m_char_MAX], z2[m_char_MAX];
    char xe[m_char_MAX], ye[m_char_MAX];
    char xm[m_char_MAX], ym[m_char_MAX];
    char radius[m_char_MAX], alphaA[m_char_MAX], alphaE[m_char_MAX];
    char zeile[m_char_MAX];
    int i;
    /*Layer und Namenslisten*/
    Liste<RUZ_Layer>* layerLst;
    Liste<Char_Speicher>* namensLst;
    RUZ_Layer* einzelLayer;

    RUZ_Layer* HoleLayer(const char* layerName);
    bool LayerAufListe(const char* layerName);
    /*Lese-Abteile*/
    void SucheEntitieSection(void);
    void LesePunkt(void);
    void LeseStrich(void);
    void LesePolyLinie(void);
    void LeseFlaeche(void);
    void LeseBogen(void);
    void LeseVertex(double& _x, double& _y);
public:
    DXF_Import(char* DatName, char* XKenn, char* YKenn, char* ZKenn, char* SW);
    ~DXF_Import();
    Liste<Char_Speicher> *HoleLayerNamen(void);
    /*Mit Layern einlesen*/
    bool EinlesenPunkte(Liste<RUZ_Layer>* layLst, Liste<Char_Speicher>* namLst = NULL);
    bool EinlesenPunkteKanten(Liste<RUZ_Layer>* layLst, Liste<Char_Speicher>* namLst = NULL);
    /*Auf �bergebenen Layer einlesen*/
    bool EinlesenPunkte(RUZ_Layer* layer, Liste<Char_Speicher>* namLst = NULL);
    bool EinlesenPunkteKanten(RUZ_Layer* layer, Liste<Char_Speicher>* namLst = NULL);
    bool EinlesenHintergrund(RUZ_Layer* hintergrundLayer);
};

class DXF_Export
{
private:
    std::ofstream Datei;
    //std::ifstream BlockVorlage;
    char DateiName[256];
    int iGenauigkeit, iHmGenauigkeit;
public:
    DXF_Export(Liste<RUZ_Layer>* layerListe, char* DatName, int genauigkeit, unsigned int cSichtFlags = 0xFFFF);
    //DXF_Export(RUZ_Layer* layer, char* DatName, int genauigkeit); Entfaell
    //~DXF_Export();
};

#endif // __DXF_Handler
