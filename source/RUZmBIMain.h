/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul Benutzeroberfläche (wxWidgets)

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

#ifndef RUZMBIMAIN_H
#define RUZMBIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <cstring>

#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/listctrl.h>
#include <wx/process.h>
#include <wx/sizer.h>

#include "aruIntegral/aruIntegral.h"
#include "Dbl_Eingabe/Dbl_Eingabe.h"
#include "DXF\DXF_Handler.h"
#include "Liste\Verkettete_Liste.h"
#include "RUZmBIApp.h"
#include "RUZ\RUZObjekte.h"
#include "RUZ\RUZVerwaltung.h"
#include "RUZThreadCtrl.h"
#include "Vektor\Vektor.h"

using namespace std;

class RUZmBIFrame;
class DXF_Parameter_Dialog;
class HL_Parameter_Dialog;
class aruLayerButton;
class aruLayerSizer;
class aruLayerTextCtrl;
class Layer_Verwaltungs_Dialog;
class Programm_Einstellungen_Dialog;
class Koordinaten_Eingabe_Dialog;
class Double_Eingabe_Dialog;
class Objekt_Anzeige_Auswahl_Dialog;
class aruVektorEvent;
class aruDblEvent;
class aruColourEvent;
class aruLayerListeEvent;

void KommaGegenPunkt(wxString*);

enum dxfParameterID{
    IDpktXKenn=1000, IDpktYKenn, IDpktZKenn, IDpktSW
};
enum hlParameterID{
    IDhoehenSchritt=1250, IDsuchRadius, IDstartHoehe
};
enum programmEinstellungID{
    IDverschubWeite=2000,
    IDgefaelleRasterGroesse, IDgefaelleMinimum, IDgefaelleOptimum, IDgefaelleMaximum,
    IDflaechenRasterGroesse, IDpxSuchEntfernung, IDlnWandelGenauigkeit, IDFarbePktLn,
    IDFarbeHoehenMarke, IDFarbeHoehenlinie, IDFarbeMarkiertesObjekt, IDFarbeAusgewaehltesObjekt,
    IDFarbeHintergrundLayer, IDFarbeZeichenHintergrund, IDFarbeAuswahlRechteck, IDFarbeStrich,
    IDFarbeFlaecheDarueber, IDFarbeFlaecheDarunter, IDFarbeGefaelle, IDFarbeFangpunkt,
    IDanzeigeGenauigkeit, IDpseudoSchattenFkt
};
enum doubleRueckgabeID{
    idDrehwinkel_temp = 2250,
    idDrehwinkel_perm,
    idRadius_temp,
    idRadius_perm
};

struct Punkt2D
{
    Punkt2D()
    {
        x = y = 0;
    };
    Punkt2D(double _x, double _y)
    {
        x = _x;
        y = _y;
        return;
    };
    double x, y;
};

struct PunktSpeicher
{
    private:
        Vektor vOrt;
        RUZ_Objekt *obj;
    public:
        PunktSpeicher(Punkt *_obj)
        {
            obj = _obj;
            vOrt = _obj->HolePosition();
        }
        PunktSpeicher(HoehenMarke *_obj)
        {
            obj = _obj;
            vOrt = _obj->HolePosition();
        }
        PunktSpeicher(Kreis *_obj)
        {
            obj = _obj;
            vOrt = _obj->HolePosition();
        }
        PunktSpeicher(Fangpunkt *_obj)
        {
            obj = _obj;
            vOrt = _obj->HolePosition();
        }
        Vektor HoleOrt(void)
        {
            return vOrt;
        }
        RUZ_Objekt* HoleObj(void)
        {
            return obj;
        }
};

struct Leinwand
{
    public:
        Leinwand()
        {
            ucLeinwand = NULL;
        };
        void TabulaRasa(void)
        {
            if(ucLeinwand)delete []ucLeinwand;
            ucLeinwand = NULL;
            return;
        };
        ~Leinwand()
        {
            TabulaRasa();
        };
        void NeueLeinwand(int b, int h, double skal, double offx, double offy)
        {
            TabulaRasa();
            ucLeinwand = new unsigned char[b*h*3];
            iBreite = b;
            iHoehe = h;
            dSkalierung = skal;
            dOffsetX = offx;
            dOffsetY = offy;
            return;
        };
        int iBreite, iHoehe;
        double dSkalierung;
        double dOffsetX, dOffsetY;
        unsigned char* ucLeinwand;
};

struct MassstabsBalken
{
	int iLaenge1, iLaenge2;
	double dWert1, dWert2;
	int iRes;
	void MassstabErmitteln(double skalierFkt)
	{
		if(!skalierFkt)return;
		double result = log10(skalierFkt);
		iRes = result - (result < 0);
		result -= iRes;
		dWert1 = 10 / pow(10, iRes);
		dWert2 = dWert1 * 10;
		iLaenge1 = dWert1 * skalierFkt; //pow(10, result) * 10;
		iLaenge2 = iLaenge1 * 10;
		return;
	};
};

void PrismaSchreiben(std::string &prismenListe, Flaeche* drk, double flaeche, double volumen);

class RUZmBIFrame: public wxFrame
{
    public:
        RUZmBIFrame(wxFrame *frame, const wxString& title, const wxPoint &pos, const wxSize &siz, long);
        ~RUZmBIFrame();

        void SetzeAktuellenLayer(int neuerAktLayerNr, RUZ_Layer* neuerAktLayer);
        RUZ_Layer* HoleAktuellenLayer(void)const;
        void LayerEntfernen(RUZ_Layer *layer);
        void LayerHinzufuegen(RUZ_Layer *layer);

        enum/*Menus*/
        {
            idMenuQuit = 1000,
            idMenuAbout, idMenuFileSave, idMenuQuickSave, idMenuExportPrismen, idMenuExportPunkte, idMenuDeleteLayer, idMenuLayerAuswahl, idMenuLayerKopieren,
            idMenuHintergrundEinlesen, idMenuHintergrundLoeschen,
            idMenuDxfImp_mitLay_Pkt, idMenuDxfImp_mitLay_Ln_Pkt, idMenuDxfImp_ohneLay_Pkt, idMenuDxfImp_ohneLay_Ln_Pkt,
            idMenuRuzImport, idMenuD45Import, idMenuD58Import,
            idMenuKantenWandeln, idMenuVernetzen, idMenuDreieckeFinden, idMenuViereckeFinden, idMenuViereckTeilen, idMenuKomplettVernetzen, idMenuDoppeltePunkteLoeschen,
            idMenuObjektLoeschen, idMenuStricheLoeschen, idMenuObjektVerschieben, idMenuPunktVersetzen, idMenuObjektDrehen, idMenuObjektKopieren, idMenuObjektKopierenNachLayer,
            idMenuHLZeigen, idMenuHLEinstellen, idMenuGefaelleZeigen, idMenuGefaelleRasterZeigen, idMenuSonnenstand, idMenuHintergrundMalen,
            idMenuPunktZeichnen, idMenuLinieZeichnen, idMenuLinieExtrudieren, idMenuLinieParallel,
            idMenuDreieckZeichnen, idMenuViereckZeichnen, idMenuKreisZeichnen,
            idMenuLayerVerschneiden, idMenuLayerInSichVerschneiden, idMenuFlaechenVerschneiden, idMenuLayerRandabschneiden, idMenuPunktVereinigen,
            idVolumenZwischenLayern_Kons, idVolumenZwischenLayern_Integral, idMenuSchnittPunktLinie, idMenuSchnittPunktFlaeche, idMenuFangpunkteFinden, idMenuFangpunkteLoeschen,
            idMenuHoehenMarkeZeichnen, idMenuStreckeMessen, idMenuEinstellungen, idMenuLayerinhaltAnzeigen, idMenuLizenz,
            idMenuGesamtansicht, idMenuPunkteSkalieren, idMenuLayerSkalieren, idMenuHintergrundSkalieren, idMenuAllesSkalieren, idMenuSkalierFaktor, idTimer,
            idMenuZeigeWaehle, idMenuEntferneTieferes, idMenuEntferneHoeheres, idMenuUeberlappungFinden,
            idZeigePunkt, idZeigeHoehe, idZeigeLinie, idZeigeFlaeche, idZeigeHoehenmarke, idZeigeStrich, idZeigeBogen, idZeigeKreis, idZeigeFangpunkt,
            idWaehlePunkt, idWaehleLinie, idWaehleFlaeche, idWaehleHoehenmarke, idWaehleStrich, idWaehleBogen, idWaehleKreis, idWaehleFangpunkt
        };
        enum befehlsID{
            bef_ID_nichts = 2000, bef_ID_loeschen, bef_ID_verschieben, bef_ID_drehen, bef_ID_kopieren, bef_ID_kopierenNachLayer,
            bef_ID_punktZeichnen, bef_ID_linieZeichnen, bef_ID_dreieckZeichnen, bef_ID_viereckZeichnen, bef_ID_kreisZeichnen,
            bef_ID_kreisRadiusAendern, bef_ID_fangpunkteFinden,
            bef_ID_viereckTeilen, bef_ID_flaechenVerschneiden,
            bef_ID_hoehenMarkeZeichnen, bef_ID_StreckeMessen, bef_ID_versetzen, bef_ID_punktEinfuegen,
            bef_ID_linieExtrudieren, bef_ID_linieParallel, bef_ID_punkteVereinigen, bef_ID_SchnittPunktLinie, bef_ID_SchnittPunktFlaeche,
            bef_ID_punkteSkalieren, bef_ID_layerSkalieren, bef_ID_hintergrundSkalieren, bef_ID_allesSkalieren,
        };

        enum ansichtID{
            ansicht_ID_normal = 2500, ansicht_ID_pseudoschatten, ansicht_ID_3D, ansicht_ID_hoehenkarte
        }aktuelleAnsicht;

    private:
        Achse aktProjX, aktProjY, aktProjZ; /*aktuelle Projektionsrichtung in orthogonaler Projektion*/
		void ProjektionsrichtungFestlegen(int neueXRichtung);

        void ParamIni(void);

        /*wxObjekte*/
        wxFileDialog *FileOpener, *FileSaver;
        DXF_Parameter_Dialog *dxfParameterDlg;
        HL_Parameter_Dialog *hlParameterDlg;
        Layer_Verwaltungs_Dialog *LayerAuswahl;
        Programm_Einstellungen_Dialog *peEinstellungenDlg;
        Koordinaten_Eingabe_Dialog *KoordinatenMaske;
        Double_Eingabe_Dialog *DoubleEingabe;
        Objekt_Anzeige_Auswahl_Dialog *ObjAnzAuswDlg;
        /*ENDE wxObjekte*/

        wxString strAktuellerSpeicherpfad;

        /*wxMenus wxCheck*/
        wxMenuItem
            /*Gruppe 0*/
            *menuLoeschen,
            *menuVerschieben,
            *menuKopieren,
            *menuKopierenNachLayer,
            *menuVersetzen,
            *menuDrehen,
            *menuPunktZeichnen,
            *menuLinieZeichnen,
            *menuKreisZeichnen,
            *menuLinieExtrudieren,
            *menuLinieParallel,
            *menuDreieckZeichnen,
            *menuViereckZeichnen,
            *menuStreckeMessen,
            *menuHoehenMarkeZeichnen,
            *menuPunktVereinigen,
            *menuPunkteSkalieren,
            *menuViereckTeilen,
            *menuFlaecheVerschneiden,
            *menuSchnittPunktFlaeche,
            *menuSchnittPunktLinie,
            *menuFangpunkteFinden,
            /*ENDE Gruppe 0*/
            *menuHLZeigen,
            *menuGefaelleZeigen,
            *menuGefaelleRasterZeigen,
            *menuHintergrundMalen;
        /*ENDE wxMenus*/

        /*Farben*/
        wxColour col_Pkt_Ln, col_HoehenMarke, col_Hoehenlinie,
                col_markiert_Obj, col_ausgewaehlt_Obj, col_HintergrundLayer,
                col_ZeichenHintergrund, col_AuswahlRechteck, col_Strich,
                col_Flaeche_darueber, col_Flaeche_darunter, col_Gefaelle, col_Fangpunkt;
        /*ENDE Farben*/

        /*Eventhandling*/
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpenFile(wxCommandEvent &event);
        void OnHintergrundEinlesen(wxCommandEvent &event);
        void OnHintergrundLoeschen(wxCommandEvent &event);
        void OnSaveFile(wxCommandEvent &event);
        void OnLoescheLayer(wxCommandEvent &event);
        void OnLoescheStriche(wxCommandEvent &event);
        void OnLayerAuswahl(wxCommandEvent &event);
        void OnLayerKopieren(wxCommandEvent &event);
        void OnBearbeitungsBefehl(wxCommandEvent &event);
        void OnAnsichtswechsel(wxCommandEvent &event);
        void OnToggleHintergrund(wxCommandEvent &event);
        void OnAusdehnungFinden(wxCommandEvent &event);
        void OnSkalierfaktor(wxCommandEvent &event);
        void OnKantenWandeln(wxCommandEvent &event);
        void OnPunkteVernetzen(wxCommandEvent &event);
        void OnDreieckeFinden(wxCommandEvent &event);
        void OnViereckeFinden(wxCommandEvent &event);
        void OnKomplettVernetzen(wxCommandEvent &event);
        void OnLayerVerschneiden(wxCommandEvent &event);
        void OnLayerInSichVerschneiden(wxCommandEvent &event);
        void OnLayerRandabschneiden(wxCommandEvent &event);
        void OnHLEinstellen(wxCommandEvent &event);
        void OnHLZeigen(wxCommandEvent &event);
        void OnGefaelleZeigen(wxCommandEvent &event);
        void OnGefaelleRasterZeigen(wxCommandEvent &event);
        void OnEinstellungen(wxCommandEvent &event);
        void OnSonnenstandEinstellen(wxCommandEvent &event);
        void OnLayerinhaltAnzeigen(wxCommandEvent &event);
        void OnVolumenZwischenLayern(wxCommandEvent &event);
        void OnZeigeWaehle(wxCommandEvent& event);
        void OnObjekteZeigen(wxCommandEvent& event);
        void OnObjekteWaehlen(wxCommandEvent& event);
        void OnEntferneTieferes(wxCommandEvent& event);
        void OnEntferneHoeheres(wxCommandEvent& event);
        void OnUeberlappungFinden(wxCommandEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnEraseBackground(wxEraseEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        /*Mouse*/
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseWheel(wxMouseEvent& event);
        void OnMouseLeftClick(wxMouseEvent& event);
        void OnMouseMiddleClick(wxMouseEvent& event);
        void OnMouseRightClick(wxMouseEvent& event);
        void OnMouseLeftUp(wxMouseEvent& event);
        void OnMouseLDClick(wxMouseEvent& event);
        void OnMouseRDClick(wxMouseEvent& event);
        /*ENDE Mouse*/
        void FangeKoor(aruVektorEvent& event);
        void FangeSonnenstand(aruVektorEvent& event);
        void FangePE(aruDblEvent& event);
        void FangeDrehwinkel(aruDblEvent& event);
        void FangeRadius(aruDblEvent& event);
        void FangeFarben(aruColourEvent& event);
        void FangeLayerSkalieren(aruLayerListeEvent& event);
        /*ENDE Eventhandling*/

        void AusdehnungFinden(void);
        void AuswahlLoeschen(void);
        void AuswahlAufPunkteReduzieren(void);
        void AuswahlKopieren(void);
    public:
        void AuswahlLeeren(void);
    private:
        void ObjekteNullen(void);
        void BefehleZuruecksetzen(void);
        void MenuEntmarkieren(void);
        bool LayerLoeschen(wxString);
        void LayerauswahlAktualisieren(void);
        void LinieExtrudieren(Vektor);
        void LinieParallel(Vektor);
        void SkalierungAusfuehren(Vektor);
        void LayerSkalieren(Vektor);
        bool SkalierFaktorenEingabe(void);
        void ViereckeAuswahlTeilen(void);
        void VerschneideAuswahlFlaechen(void);
        void DreheAuswahl(void);
        bool ObjektMarkieren(int, int);
        void FangpunkteFinden(Liste<RUZ_Objekt>*);
        void FangpunkteFinden(RUZ_Objekt*, RUZ_Objekt*);
        void FangpunkteLoeschen(wxCommandEvent& event);
        void DoppeltePunkteLoeschen(wxCommandEvent& event);

        void HoehenkarteZeichnen(void);
		void LayerMalen(wxDC &dc, RUZ_Layer* tempLayer);

        /*RUZ*/
        Liste<RUZ_Layer> *m_layer;
        RUZ_Layer *aktLayer, *alternativAktLayer, *aktLayerBAK;
        RUZ_Layer *m_hintergrundLayer;
        int aktLayerNr;
        RUZ_Objekt *markiertesObjekt, *objFang1, *objFang2;
        Punkt* m_aktPunkt;
        Linie* m_aktLinie;
        Kreis* m_aktKreis;
        Vektor m_aktPosition;
        double m_aktRadius;
        Liste<RUZ_Objekt> *m_auswahl, *m_kopierAuswahl;
        Vektor m_aktGefaelle;
        Vektor m_vktSchnittPkt1, m_vktSchnittPkt2;

        RUZ_Layer* LayerKopieren(RUZ_Layer*, char*);/*nach RUZVerwaltung auslagern*/
        /*ENDE RUZ*/

        /*Höhenlinienparameter + Gefälle*/
        double hoehenSchritt, suchRadius, startHoehe, m_gefaelleRaster, m_flaechenRaster;
        bool hlAnzeigen, gefaelleAnzeigen, gefaelleRasterAnzeigen;
        /*ENDE Höhenlinienparameter + Gefälle*/

        /*Objekte und Parameter der Ansicht*/
        Leinwand lwBild;
		MassstabsBalken mbSkalierung;
		void SkalierungSetzen(double t_skal);

        double m_skalierung, m_pseudoSchattenFkt;
        double dc_Offset[2];
        wxPoint AlteMousePosition, NeueMousePosition, MarkierMousePosition;
        bool m_hintergrundMalen, anzeigeSkalieren;
        Vektor m_sonnenRichtung;
        /*ggf. bitweise speichern... in unsigned int*/
        bool m_zeigePunkt, m_zeigeLinie, m_zeigeFlaeche, m_zeigeHoehenmarke, m_zeigeStrich, m_zeigeBogen, m_zeigeKreis, m_zeigeFangpunkt, m_zeigeHoehe;
        bool m_waehlePunkt, m_waehleLinie, m_waehleFlaeche, m_waehleHoehenmarke, m_waehleStrich, m_waehleBogen, m_waehleKreis, m_waehleFangpunkt;
        /*ENDE Objekte und Parameter der Ansicht*/

        /*Messen*/
        Punkt2D messAnfang, messEnde;
        void (RUZmBIFrame::*Messen)(double, double);
        void AnfangMessen(double, double);
        void EndeMessen(double, double);
        /*ENDE Messen*/

        /*Verschieben*/
        Vektor *vVerschubStart;
        Liste<PunktSpeicher> *m_verschubAuswahlOrte;
        bool Vschb_Ort_Exklusiv_Hinzufuegen(PunktSpeicher*); //von Kopieren mitbenutzt!
        void Vschb_Auswahl_Bestaetigung(void);
        void Vschb_Punkt_Festlegen(Vektor);
        void Vschb_Verschieben(Vektor);
        void Vschb_Abbrechen(void);
        void Vschb_Punktspeicher_Schreiben(void);
        /*ENDE Verschieben*/

        void Kop_Vsch_Punktspeicher_Schreiben(Liste<RUZ_Objekt>* t_auswahl);

        /*Kopieren*/
        void Kop_Auswahl_Bestaetigung(void);
        void Kop_Punkt_Festlegen(Vektor);
        void Kop_Verschieben(Vektor);
        void Kop_Abbrechen(void);
        void Kop_Punktspeicher_Schreiben(void);
        /*ENDE Kopieren*/

        /*Kreiszeichnen*/
        void KreisPunktEingabe(Vektor&, bool);
        void KreisDoubleEingabe(double, bool);
        /*ENDE Kreiszeichnen*/

        /*Drehen*/
            /*Variablen*/
            Vektor *m_drehungDrPkt, *m_drehungRichtung1, *m_drehungRichtung2;
            Liste<Vektor> *m_drehungAuswahlOrte;
            /*Funktionen*/
            void (RUZmBIFrame::*DrehungPktEingabe)(Vektor);/*Punkteingabe*/
                void DrehungPktEingabe_DrhPkt(Vektor);
                void DrehungPktEingabe_RP_1(Vektor);
                void DrehungPktEingabe_RP_2(Vektor);
            void DrehungBestaetigung(void);
            void (RUZmBIFrame::*DrehungMouseMove)(wxMouseEvent &event);
                void DrehungMouseMoveAktiv(wxMouseEvent &event);
                void DrehungMouseMovePassiv(wxMouseEvent &event);
            void (RUZmBIFrame::*DrehungBefehlsketteVor)(void);/*Nächster Befehl in der Befehlskette*/
                void DrehungBefehlsketteVor_0(void);
                void DrehungBefehlsketteVor_1(void);
                void DrehungBefehlsketteVor_2(void);
                void DrehungBefehlsketteVor_3(void);
            void (RUZmBIFrame::*DrehungBefehlsketteZurueck)(void);/*Vorangegangener Befehl in der Befehlskette*/
                void DrehungBefehlsketteZurueck_0(void);
                void DrehungBefehlsketteZurueck_1(void);
                void DrehungBefehlsketteZurueck_2(void);
                void DrehungBefehlsketteZurueck_3(void);
            void (RUZmBIFrame::*DrehungAbbruch)(void);/*Abbruch der Drehung*/
                void DrehungAbbruch_0(void);
                void DrehungAbbruch_1(void);
                void DrehungAbbruch_2(void);
        /*ENDE Drehen*/

        /*Schnittpunkt*/
        /*Variablen*/
        Punkt* m_schP_OrgPkt;   /*Punkt der verschoben werden soll*/
        Punkt *m_schP_Richtung_1, *m_schP_Richtung_2;
        Vektor m_normale;
        double m_Abstand;
        Vektor m_schP_alterOrt; /*Ort an dem der Punkt vorher war*/
        Linie* m_schP_Ln;       /*Linie entlang derer der Punkt verschoben werden soll*/
        Dreieck* m_schP_Dr;     /*Dreieck innerhalb dessen Ebene der Punkt verschoben werden soll*/
        RUZ_Objekt* m_schP_Obj;     /*Objekt mit dem verschnitten werden soll*/
        /*Funktionen*/
        void SchnittpunktFlaecheAbschliessen(void);
        void SchnittpunktLinieAbschliessen(void);
        /*ENDE Schnittpunkt*/

        /*Punkte Vereinigen*/
        Punkt *vereinOrigPkt, *vereinErsatzPkt;
        void (RUZmBIFrame::*Vereinigen)(Punkt*);
        void VereinigenErsterPkt(Punkt*);
        void VereinigenZweiterPkt(Punkt*);
        /*ENDE Punkte Vereinigen*/

        /*Skalieren*/
        Liste<RUZ_Layer>* m_skalierListe;
        double m_skalFkt[3];
        /*ENDE Skalieren*/

        /*Gefaelle verfolgen*/
        void GefaelleVerfolgen(Vektor);
        /*ENDE Gefaelle verfolgen*/

        /*Markierung*/
        int wertFkt, pxSuchEntfernung;
        bool m_markierungsRechteck, m_kreuzen, m_markierModus, m_markierModus2;
        /*ENDE Markierung*/

        /*Programmparameter*/
        befehlsID aktBefehl;
        double m_verschubWeite, m_lnWandelGenauigkeit;
        int m_anzeigeGenauigkeit;
		bool bKeyADown;
        /*ENDE Programmparameter*/

        /*Logbuch*/
        //ofstream *Logbuch;
        void logSchreiben(const char* msg, ...);
        void logSchreiben(std::string msg);
        void logSchreiben(const Vektor, int i=0);
        /*ENDE Logbuch*/

        /*Dateien lesen und schreiben*/
        bool SchreibeInDatei(char* dateiName);
        bool ExportiereDreiecksPrismen(char* dateiName);
        bool ExportierePunkte(char* dateiName);
        bool LeseAusRUZ(char* dateiName);
        bool LeseAusD45(char* dateiName);
        bool LeseAusD58(char* dateiName);
        /*ENDE Dateien lesen und schreiben*/

        DECLARE_EVENT_TABLE()
};

class Programm_Einstellungen_Dialog: public wxDialog
{
    public:
        Programm_Einstellungen_Dialog(wxWindow* parent);
        ~Programm_Einstellungen_Dialog();
        double HoleWert(programmEinstellungID retID) const;
        wxColour HoleFarbe(programmEinstellungID retID) const;
    private:
        wxWindow* m_mama;
        /*Parameter*/
        double pe_verschubWeite, pe_pxSuchEntfernung, pe_lnWandelGenauigkeit, pe_anzeigeGenauigkeit, pe_flaechenRasterGroesse,
                pe_pseudoSchattenFkt, pe_gefaelleRasterGroesse, pe_gefaelleMinimum, pe_gefaelleOptimum, pe_gefaelleMaximum;
        wxColour pe_FarbePktLn, pe_FarbeHoehenMarke, pe_FarbeHoehenlinie, pe_FarbeMarkiertesObjekt,
                    pe_FarbeAusgewaehltesObjekt, pe_FarbeHintergrundLayer, pe_FarbeZeichenHintergrund,
                    pe_FarbeAuswahlRechteck, pe_FarbeStrich, pe_FarbeFlaecheDarueber, pe_FarbeFlaecheDarunter,
                    pe_FarbeGefaelle, pe_FarbeFangpunkt;

        void PEDialogIni(void);
        void IniSpeichern(void);

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);
        void OnTextChange(wxCommandEvent &event);
        void OnColourButton(wxCommandEvent &event);
        DECLARE_EVENT_TABLE()
};

class Sonnenstand_Dialog: public wxDialog
{
    public:
        Sonnenstand_Dialog(wxWindow* parent, Vektor);
        ~Sonnenstand_Dialog();
        void SetzeSonnenstand(Vektor);
    private:
        wxWindow* m_mama;
        /*Parameter*/
        Vektor m_sonnenstand;

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnMouseClick(wxMouseEvent &event);
        void OnMouseMove(wxMouseEvent &event);
        void OnPaint(wxPaintEvent &event);
        /*ENDE Event handling*/

        /*Sonnenstand*/
        void SonnenstandMelden(void);
        void SonnenstandErrechnen(wxPoint);
        /*ENDE Sonnenstand*/
        DECLARE_EVENT_TABLE()
};

class DXF_Parameter_Dialog: public wxDialog
{
    public:
        DXF_Parameter_Dialog(wxWindow* parent);
        ~DXF_Parameter_Dialog();
        void HoleKennung(char* retVar, dxfParameterID retID) const;
    private:
        wxWindow* m_mama;
        /*Parameter*/
        wxString m_pktXKenn, m_pktYKenn, m_pktZKenn, m_pktSW;
        void DxfDialogIni(void);
        void IniSpeichern(void);

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);
        void OnTextChange(wxCommandEvent &event);
        DECLARE_EVENT_TABLE()
};

class DXF_Import_Auswahl_Dialog: public wxDialog
{
    public:
        DXF_Import_Auswahl_Dialog(wxWindow* parent, Liste<Char_Speicher>* layerLst);
        ~DXF_Import_Auswahl_Dialog();
    private:
        wxWindow* m_mama;
        wxListBox *layerListBox;
        Liste<Char_Speicher>* m_layerLst;

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);
        void OnMarkierungDrehen(wxCommandEvent &event);

        void MarkierungDrehen(void);

        DECLARE_EVENT_TABLE()
};

class HL_Parameter_Dialog: public wxDialog
{
    public:
        HL_Parameter_Dialog(wxWindow* parent);
        ~HL_Parameter_Dialog();
        double HoleWert(hlParameterID retID) const;
    private:
        wxWindow* m_mama;
        /*Parameter*/
        double hoehenSchritt, suchRadius, startHoehe;
        void HlDialogIni(void);
        void IniSpeichern(void);

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);
        void OnTextChange(wxCommandEvent &event);
        DECLARE_EVENT_TABLE()
};

class Koordinaten_Eingabe_Dialog:public wxDialog
{
    public:
        Koordinaten_Eingabe_Dialog(wxWindow* parent, wxString titel);
        ~Koordinaten_Eingabe_Dialog();
        void SetzeKoordinaten(const Vektor);
        void SetzeKoordinaten(Achse t_achse, double wert);
        double HoleKoordinaten(Achse t_achse);
    private:
        wxWindow* m_mama;
        /*Parameter*/
        aruDblTxtCtrl *xCtrl, *yCtrl, *zCtrl;

        /*Event handling*/
        void OnKeyPress(wxKeyEvent &event);
        DECLARE_EVENT_TABLE()
};

class Double_Eingabe_Dialog:public wxDialog
{
    public:
        Double_Eingabe_Dialog(wxWindow* parent, wxString titel, wxString eingabeBezeichnung, wxPoint ort);
        ~Double_Eingabe_Dialog();
        void ErscheinungAnpassen(wxString neueEingabeBezeichnung, wxString wert, int befID);
        /*Event handling*/
        void OnOKButton(wxCommandEvent &event);
        void OnTextChange(wxCommandEvent &event);
    private:
        wxWindow* m_mama;
        wxButton* ok_Button;
        /*Parameter*/
        wxTextCtrl *txtCtrlEingabeBezeichnung;
        aruDblTxtCtrl *eingabeCtrl;
        int nAktBefehl;
        bool txtCtrlInternGeaendert;

        /*Event handling*/
        void OnKeyPress(wxKeyEvent &event);
        DECLARE_EVENT_TABLE()
};

class Layer_Verwaltungs_Dialog: public wxDialog
{
public:
    Layer_Verwaltungs_Dialog(RUZmBIFrame* parent);
    ~Layer_Verwaltungs_Dialog();
    void LayerHinzufuegen(wxString name, RUZ_Layer* neuerLayer);
    void LayerAuswahlLeeren(void);
private:
    enum
    {
        idMenuNeuerLayer = 1000
    };
    RUZmBIFrame* m_mama;
    wxBoxSizer *hauptSizer, *layerSizer;
    wxScrolledWindow* scroller;
    Liste<RUZ_Layer> *layerLst;
    Liste<aruLayerSizer> *layerSizerLst;

    /*Event handling*/
    void OnLayerAuswaehlen(wxCommandEvent& event);
    void OnLayerSichtbarkeitWechseln(wxCommandEvent& event);
    void OnLayerLoeschen(wxCommandEvent& event);
    void OnNeuerLayerButton(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);

    void LayerAuswahlFinden(void);
    DECLARE_EVENT_TABLE()
};

class Layer_Auswahl_Dialog: public wxDialog
{
    public:
        Layer_Auswahl_Dialog(wxWindow* parent, Liste<RUZ_Layer> *layerLst, wxString caption, bool singleSelection = true);
        ~Layer_Auswahl_Dialog();
    private:
        wxWindow* m_mama;
        Liste<RUZ_Layer> *m_layerLst;
        RUZ_Layer* sel_layer;
        wxListCtrl *layerListCtrl;
        bool m_singleSelection;

        /*Event handling*/
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);
        void OnCancelButton(wxCommandEvent &event);
        void OnDblClick(wxListEvent &event);

        DECLARE_EVENT_TABLE()
};

class Objekt_Anzeige_Auswahl_Dialog: public wxDialog
{
    public:
        Objekt_Anzeige_Auswahl_Dialog(wxWindow* parent);
        ~Objekt_Anzeige_Auswahl_Dialog();
    private:
        wxWindow* m_mama;
        wxCheckBox *PunktZeigeChkBx, *PunktWaehleChkBx;
        wxCheckBox *LinieZeigeChkBx, *LinieWaehleChkBx;
        wxCheckBox *FlaecheZeigeChkBx, *FlaecheWaehleChkBx;
        wxCheckBox *HMZeigeChkBx, *HMWaehleChkBx;
        wxCheckBox *StrichZeigeChkBx, *StrichWaehleChkBx;
        wxCheckBox *BogenZeigeChkBx, *BogenWaehleChkBx;
        wxCheckBox *KreisZeigeChkBx, *KreisWaehleChkBx;
        wxCheckBox *FangpunktZeigeChkBx, *FangpunktWaehleChkBx;
        wxCheckBox *HoeheZeigeChkBx, *HoeheWaehleChkBx;

        /*Event handling*/
        void OnChkBx(wxCommandEvent &event);
        void OnClose(wxCloseEvent &event);
        void OnOKButton(wxCommandEvent &event);

        DECLARE_EVENT_TABLE()
};

class aruLayerButton: public wxButton
{
public:
    aruLayerButton(aruLayerSizer *data, wxWindow *parent, wxWindowID id, const wxString &label=wxEmptyString);
    ~aruLayerButton();

    void SetClientData(aruLayerSizer *data);
    aruLayerSizer *GetClientData(void) const;
private:
    aruLayerSizer *cl_LayerSizer;
};

class aruLayerTextCtrl: public wxTextCtrl
{
public:
    aruLayerTextCtrl(RUZ_Layer*, wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&,
                     long style=0, const wxValidator&validator=wxDefaultValidator, const wxString&name=wxTextCtrlNameStr);
    ~aruLayerTextCtrl();

    void OnLostFocus(wxFocusEvent &event);
private:
    RUZ_Layer *cl_Layer;
    DECLARE_EVENT_TABLE()
};

class aruLayerSizer: public wxBoxSizer
{
public:
    aruLayerSizer(RUZmBIFrame* parent, RUZ_Layer *data);
    ~aruLayerSizer();

    void SetClientData(RUZ_Layer *data);
    RUZ_Layer* GetClientData(void) const;
    void SetLayerNameCtrl(aruLayerTextCtrl* txtCtrl);
    aruLayerTextCtrl* GetLayerNameCtrl(void) const;
private:
    RUZ_Layer *cl_Layer;
    RUZmBIFrame* m_mama;
    aruLayerTextCtrl* m_layerNameCtrl;
};

/********************CUSTOM EVENTS************************/

/*aruDblEvent*/
wxDEFINE_EVENT(aruEVT_DOUBLE, aruDblEvent);
class aruDblEvent: public wxCommandEvent
{
public:
    aruDblEvent(wxEventType commandType = aruEVT_DOUBLE, int id = 0);
    aruDblEvent(const aruDblEvent&);
    wxEvent* Clone()const;
    double HoleWert(void)const;
    void SetzeWert(const double);
private:
    double m_wert;
};
#define EVT_RUZ_DOUBLE(id, func) wx__DECLARE_EVT1(aruEVT_DOUBLE, id, &func)
/*ENDE aruDblEvent*/

/*aruColourEvent*/
wxDEFINE_EVENT(aruEVT_COLOUR, aruColourEvent);
class aruColourEvent: public wxCommandEvent
{
public:
    aruColourEvent(wxEventType commandType = aruEVT_COLOUR, int id = 0);
    aruColourEvent(const aruColourEvent&);
    wxEvent* Clone()const;
    wxColour HoleWert(void)const;
    void SetzeWert(const wxColour);
private:
    wxColour m_wert;
};
#define EVT_RUZ_COLOUR(func) wx__DECLARE_EVT0(aruEVT_COLOUR, &func)
/*ENDE aruColourEvent*/

/*aruVektorEvent*/
wxDEFINE_EVENT(aruEVT_VEKTOR, aruVektorEvent);
class aruVektorEvent: public wxCommandEvent
{
    public:
        aruVektorEvent(int id = 0, wxEventType commandType = aruEVT_VEKTOR);
        aruVektorEvent(const aruVektorEvent& event);
        wxEvent* Clone()const;
        Vektor HoleKoordinaten(void)const;
        void SetzeKoordinaten(const Vektor);
    private:
        Vektor koor;
};
#define EVT_RUZ_VEKTOR(id, func) wx__DECLARE_EVT1(aruEVT_VEKTOR, id, &func)
/*ENDE aruVektorEvent*/

/*aruLayerListeEvent*/
wxDEFINE_EVENT(aruEVT_LAYER_LISTE, aruLayerListeEvent);
class aruLayerListeEvent: public wxCommandEvent
{
public:
    aruLayerListeEvent(int id = 0, wxEventType commandType = aruEVT_LAYER_LISTE);
    aruLayerListeEvent(const aruLayerListeEvent&);
    ~aruLayerListeEvent();
    wxEvent* Clone()const;
    Liste<RUZ_Layer>* HoleLayerListe(void)const;
    void Hinzufuegen(RUZ_Layer*);
private:
    Liste<RUZ_Layer> *m_lst;
};
#define EVT_RUZ_LAYER_LISTE(func) wx__DECLARE_EVT0(aruEVT_LAYER_LISTE, &func)
/*ENDE aruLayerListeEvent*/

#endif // RUZMBIMAIN_H
