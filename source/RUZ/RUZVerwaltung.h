/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul zur Verwaltung der Zeichnungsobjekte

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

#ifndef __RUZ_VERWALTUNG_
#define __RUZ_VERWALTUNG_
class RUZ_Layer;
#include "RUZObjekte.h"
#include "RUZThreadInfo.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <string>


class RUZ_Layer
{
private:
    Liste<Punkt> *m_punktLst;
    Liste<Linie> *m_linienLst;
    Liste<Flaeche> *m_flaechenLst;
    Liste<Strich> *m_strichLst;
    Liste<Bogen> *m_bogenLst;
    Liste<Kreis> *m_kreisLst;
    Liste<Fangpunkt> *m_fngPktLst;
    Liste<HoehenMarke> *m_hoehenMarkenLst;
    Liste<GefaelleMarke> *m_gefaelleMarkenLst;
    Liste<Vektor> *m_fehlerMarkierung;

    Liste<Flaeche> *m_aktualisierungsFlaechen;

    char m_name[256];

    bool sichtbar;
    double m_dAusdehnung[2];
    Linie* Verbunden(Punkt *von, Punkt *nach);//Prüft, ob zwischen den beiden Punkten bereits eine Verbindungslinie besteht
    bool SindDiagonalEndpunkte(Punkt *von, Punkt *nach);//Prüft, ob die beiden Punkte sich in einem vorhandenen Viereck gegenüberliegen
    /*HL*/
    double m_hoehenSchritt, m_suchRadius, m_startHoehe;
    Achse m_projektionsRichtung;
    /*ENDE HL*/
protected:
    /*Logbuch*/
    void logSchreiben(const char*, ...);
    /*ENDE Logbuch*/
public:
    RUZ_Layer(const char*);
    ~RUZ_Layer();
    void PunkteVernetzen(thread_info_vernetzen *thInfo, Liste<Punkt>* t_pktLst = NULL);
    RUZ_Layer* Kopieren(char* name);

    void LinienNachLaengeSortieren(void);

    void DreieckeFinden(void);
    void ViereckeFinden(void);

    void Hinzufuegen(Punkt* obj);
    void Hinzufuegen(Linie* obj);
    void Hinzufuegen(Flaeche* obj);
    void Hinzufuegen(Strich* obj);
    void Hinzufuegen(Bogen* obj);
    void Hinzufuegen(Kreis* obj);
    void Hinzufuegen(Fangpunkt* obj);
    void Hinzufuegen(HoehenMarke* obj);
    void Hinzufuegen(GefaelleMarke* obj);
    void Hinzufuegen(Vektor* Fehler);

    void Entfernen(Punkt* obj);
    void Entfernen(Linie* obj);
    void Entfernen(Flaeche* obj);
    void Entfernen(Strich* obj);
    void Entfernen(Bogen* obj);
    void Entfernen(Kreis* obj);
    void Entfernen(Fangpunkt* obj);
    void Entfernen(HoehenMarke* obj);
    void Entfernen(GefaelleMarke* obj);
    void FehlerEntfernen(void);

    void Wert(Punkt* obj, double wert);
    void Wert(Linie* obj, double wert);

    /*Höhenlinien, Höhenmarken*/
    void SetzeHLParameter(double hoehenSchritt, double suchRadius, double starthoehe = 0.0, Achse projektion = z);
    double HoleHLhoehenSchritt(void){return m_hoehenSchritt;};
    double HoleHLsuchRadius(void){return m_suchRadius;};
    double HoleHLstartHoehe(void){return m_startHoehe;};
    void GeaenderteFlaecheHinzufuegen(Flaeche* obj);
    void FlaechenAktualisieren(bool);
    /*ENDE Höhenlinien, Höhenmarken*/

    /*Gefälleraster*/
    bool gefaelleRasterAktiv;
    double dGefMinimum, dGefOptimum, dGefMaximum;

    void GefaelleRasterAnlegen(double rasGr, double minimum, double optimum, double maximum);
    void GefaelleRasterLoeschen(void);
private:
    void GefaelleRasterAnpassen(void);
    /*ENDE Gefälleraster*/

public:
    bool AusdehnungFinden(double &min_x, double &min_y, double &max_x, double &max_y, double &min_z, double &max_z);
    bool FlaecheVorhanden(Flaeche *testFlaeche);
    bool LinieDoppeltVorhanden(Linie *testLinie);
    Linie* HoleLinie(Punkt* p1, Punkt* p2);/*Holt die Linie, die p1 und p2 verbindet oder erzeugt eine neue*/
    bool PunktDoppeltVorhanden(Punkt *testPunkt);

    /*Verschiedenes*/
    Achse HoleProjektion(void){return m_projektionsRichtung;};
    void SetzeProjektion(Achse);

    void LoescheDoppeltePunkte(int genauigkeit = 10);
    void LoescheDoppeltePunkte(Liste<Punkt>* pktLst, int genauigkeit = 10);
    void LoescheDoppeltePunkte(Liste<Hoehenpunkt>* pktLst);
    void LoescheFangpunkte(void);
    void ViereckeTeilen(Liste<Flaeche>* = NULL);
    void VorhandeneLinienSchuetzen(bool geschuetzt);
    void LinienAusStrichen(double genauigkeit, Achse proj_Richtung = z);
    void RandAbschneiden(RUZ_Layer* andererLayer, int genauigkeit = 10);
    void LoescheFreiliegendeFlaechen(RUZ_Layer*);
    void Verschneiden(RUZ_Layer *andererLayer, thread_info_verschnitt *thInf, int *genauigkeit);
    void VerschneideFlaechen(Liste<Flaeche>* flLst = NULL, int genauigkeit = 10);
    void SchnittlinienFinden(Liste<Flaeche>*, Liste<Flaeche>*, RUZ_Layer* , thread_info_verschnitt*);
    void EntlangLinienSchneiden(Liste<Linie>*, thread_info_verschnitt*);
    void EntferneTieferes(Achse);
    void EntferneHoeheres(Achse);

	void UngeschuetzteLinienLoeschen(void);
    /*ENDE Verschiedenes*/

    Liste<Punkt>* HolePunkte(void) const;
    Liste<Linie>* HoleLinien(void) const;
    Liste<Flaeche>* HoleFlaechen(void) const;
    Liste<Strich>* HoleStriche(void) const;
    Liste<Bogen>* HoleBoegen(void) const;
    Liste<Kreis>* HoleKreise(void) const;
    Liste<Fangpunkt>* HoleFangpunkte(void) const;
    Liste<HoehenMarke>* HoleHoehenMarken(void) const;
    Liste<Vektor>* HoleFehlermarken(void) const;
    Liste<GefaelleMarke>* HoleGefaelleMarken(void) const;
    const char* HoleName(void) const{return m_name;};
    void Benennen(const char* name);

    bool IstSichtbar(void){return sichtbar;};
    void SetzeSichtbarkeit(bool);

    void ElementlisteAusgeben(void);
};

#endif // __RUZ_VERWALTUNG_
