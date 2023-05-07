/***************************************************************
 * Name:      RUZmBIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ansgar Rütten ()
 * Created:   2018-08-25
 * Copyright: Ansgar Rütten ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#define PI 3.14159265358979

#include "RUZmBIMain.h"

using namespace std;

void KommaGegenPunkt(wxString *Txt)
{
	for(unsigned int i=0; i<Txt->Len(); i++)
	{
		if(Txt->GetChar(i) == ',') Txt->SetChar(i, '.');
	}
	return;
}

void ExclusivHinzufuegen(Liste<PunktSpeicher>* lstPS, PunktSpeicher* psPS)
{
    for(PunktSpeicher* psLaeufer = lstPS->GetErstesElement(); psLaeufer; psLaeufer = lstPS->GetNaechstesElement())
    {
        /*wenn psPS schon in Liste: Abbruch*/
        if(psLaeufer == psPS)return;
    }
    lstPS->Hinzufuegen(psPS);
    return;
}

BEGIN_EVENT_TABLE(RUZmBIFrame, wxFrame)
    EVT_CLOSE(RUZmBIFrame::OnClose)
    EVT_MENU(idMenuQuit, RUZmBIFrame::OnQuit)
    EVT_MENU(idMenuAbout, RUZmBIFrame::OnAbout)
    EVT_MENU(idMenuRuzImport, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuD45Import, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuD58Import, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuDxfImp_mitLay_Ln_Pkt, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuDxfImp_mitLay_Pkt, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuDxfImp_ohneLay_Ln_Pkt, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuDxfImp_ohneLay_Pkt, RUZmBIFrame::OnOpenFile)
    EVT_MENU(idMenuHintergrundEinlesen, RUZmBIFrame::OnHintergrundEinlesen)
    EVT_MENU(idMenuHintergrundLoeschen, RUZmBIFrame::OnHintergrundLoeschen)
    EVT_MENU(idMenuFileSave, RUZmBIFrame::OnSaveFile)
    EVT_MENU(idMenuQuickSave, RUZmBIFrame::OnSaveFile)
    EVT_MENU(idMenuExportPrismen, RUZmBIFrame::OnSaveFile)
    EVT_MENU(idMenuExportPunkte, RUZmBIFrame::OnSaveFile)
    //EVT_MENU(idMenuDeleteLayer, RUZmBIFrame::OnLoescheLayer)
	EVT_MENU(idMenuStricheLoeschen, RUZmBIFrame::OnLoescheStriche)
    EVT_MENU(idMenuLayerKopieren, RUZmBIFrame::OnLayerKopieren)
    EVT_MENU(idMenuLayerAuswahl, RUZmBIFrame::OnLayerAuswahl)
    EVT_MENU(idMenuKantenWandeln, RUZmBIFrame::OnKantenWandeln)
    EVT_MENU(idMenuVernetzen, RUZmBIFrame::OnPunkteVernetzen)
    EVT_MENU(idMenuDreieckeFinden, RUZmBIFrame::OnDreieckeFinden)
    EVT_MENU(idMenuViereckeFinden, RUZmBIFrame::OnViereckeFinden)
    EVT_MENU(idMenuKomplettVernetzen, RUZmBIFrame::OnKomplettVernetzen)
    EVT_MENU(idMenuLayerVerschneiden, RUZmBIFrame::OnLayerVerschneiden)
    EVT_MENU(idMenuLayerInSichVerschneiden, RUZmBIFrame::OnLayerInSichVerschneiden)
    EVT_MENU(idMenuLayerRandabschneiden, RUZmBIFrame::OnLayerRandabschneiden)
    EVT_MENU(idMenuHLEinstellen, RUZmBIFrame::OnHLEinstellen)
    EVT_MENU(idMenuHLZeigen, RUZmBIFrame::OnHLZeigen)
    EVT_MENU(idMenuGefaelleZeigen, RUZmBIFrame::OnGefaelleZeigen)
    EVT_MENU(idMenuGefaelleRasterZeigen, RUZmBIFrame::OnGefaelleRasterZeigen)
    EVT_MENU(idMenuEinstellungen, RUZmBIFrame::OnEinstellungen)
    EVT_MENU(idMenuSonnenstand, RUZmBIFrame::OnSonnenstandEinstellen)
    EVT_MENU(idMenuLayerinhaltAnzeigen, RUZmBIFrame::OnLayerinhaltAnzeigen)
    EVT_MENU(idVolumenZwischenLayern_Kons, RUZmBIFrame::OnVolumenZwischenLayern)
    EVT_MENU(idVolumenZwischenLayern_Integral, RUZmBIFrame::OnVolumenZwischenLayern)
    EVT_MENU(idMenuEntferneTieferes, RUZmBIFrame::OnEntferneTieferes)
    EVT_MENU(idMenuEntferneHoeheres, RUZmBIFrame::OnEntferneHoeheres)
    EVT_MENU(idMenuUeberlappungFinden, RUZmBIFrame::OnUeberlappungFinden)
    EVT_MENU(idMenuFangpunkteLoeschen, RUZmBIFrame::FangpunkteLoeschen)
    EVT_MENU(idMenuDoppeltePunkteLoeschen, RUZmBIFrame::DoppeltePunkteLoeschen)
    /*Befehlsauswahl*/
    EVT_MENU(idMenuObjektLoeschen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuObjektVerschieben, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuObjektDrehen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuObjektKopieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuObjektKopierenNachLayer, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuPunktZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuLinieZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuLinieExtrudieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuLinieParallel, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuKreisZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuDreieckZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuViereckZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuHoehenMarkeZeichnen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuStreckeMessen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuPunktVersetzen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuPunktVereinigen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuPunkteSkalieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuLayerSkalieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuHintergrundSkalieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuAllesSkalieren, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuViereckTeilen, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuFlaechenVerschneiden, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuSchnittPunktFlaeche, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuSchnittPunktLinie, RUZmBIFrame::OnBearbeitungsBefehl)
    EVT_MENU(idMenuFangpunkteFinden, RUZmBIFrame::OnBearbeitungsBefehl)
    /*ENDE Befehlsauswahl*/
    /*Auswahl der Ansichten bzw. mit der Ansicht Verbundenes*/
    EVT_MENU(ansicht_ID_normal, RUZmBIFrame::OnAnsichtswechsel)
    EVT_MENU(ansicht_ID_pseudoschatten, RUZmBIFrame::OnAnsichtswechsel)
    EVT_MENU(ansicht_ID_3D, RUZmBIFrame::OnAnsichtswechsel)
    EVT_MENU(ansicht_ID_hoehenkarte, RUZmBIFrame::OnAnsichtswechsel)
    EVT_MENU(idMenuGesamtansicht, RUZmBIFrame::OnAusdehnungFinden)
    EVT_MENU(idMenuHintergrundMalen, RUZmBIFrame::OnToggleHintergrund)
	EVT_MENU(idMenuSkalierFaktor, RUZmBIFrame::OnSkalierfaktor)

    EVT_MENU(idMenuZeigeWaehle, RUZmBIFrame::OnZeigeWaehle)
    EVT_CHECKBOX(idZeigeBogen, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeKreis, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeFlaeche, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeHoehenmarke, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeLinie, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigePunkt, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeHoehe, RUZmBIFrame::OnObjekteZeigen)
    EVT_CHECKBOX(idZeigeStrich, RUZmBIFrame::OnObjekteZeigen)

    EVT_CHECKBOX(idWaehleBogen, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehleKreis, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehleFlaeche, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehleHoehenmarke, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehleLinie, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehlePunkt, RUZmBIFrame::OnObjekteWaehlen)
    EVT_CHECKBOX(idWaehleStrich, RUZmBIFrame::OnObjekteWaehlen)
    /*ENDE Auswahl der Ansichten bzw. mit der Ansicht Verbundenes*/
    EVT_PAINT(RUZmBIFrame::OnPaint)
    EVT_ERASE_BACKGROUND(RUZmBIFrame::OnEraseBackground)
    EVT_SIZE(RUZmBIFrame::OnSize)
    EVT_KEY_DOWN(RUZmBIFrame::OnKeyDown)
    EVT_KEY_UP(RUZmBIFrame::OnKeyUp)
    EVT_MOUSE_EVENTS(RUZmBIFrame::OnMouseMove)
    EVT_MOUSEWHEEL(RUZmBIFrame::OnMouseWheel)
    EVT_LEFT_DOWN(RUZmBIFrame::OnMouseLeftClick)
    EVT_LEFT_DCLICK(RUZmBIFrame::OnMouseLDClick)
    EVT_RIGHT_DOWN(RUZmBIFrame::OnMouseRightClick)
    EVT_MIDDLE_DOWN(RUZmBIFrame::OnMouseMiddleClick)
    EVT_LEFT_UP(RUZmBIFrame::OnMouseLeftUp)
    EVT_RIGHT_DCLICK(RUZmBIFrame::OnMouseRDClick)
    /*Fangen verschiedener Custom Events*/
    EVT_RUZ_VEKTOR(3000, RUZmBIFrame::FangeKoor)
    EVT_RUZ_VEKTOR(3001, RUZmBIFrame::FangeSonnenstand)

    EVT_RUZ_DOUBLE(wxID_ANY, RUZmBIFrame::FangePE)
    EVT_RUZ_DOUBLE(idDrehwinkel_temp, RUZmBIFrame::FangeDrehwinkel)
    EVT_RUZ_DOUBLE(idDrehwinkel_perm, RUZmBIFrame::FangeDrehwinkel)
    EVT_RUZ_DOUBLE(idRadius_temp, RUZmBIFrame::FangeRadius)
    EVT_RUZ_DOUBLE(idRadius_perm, RUZmBIFrame::FangeRadius)
    EVT_RUZ_COLOUR(RUZmBIFrame::FangeFarben)
    EVT_RUZ_LAYER_LISTE(RUZmBIFrame::FangeLayerSkalieren)
    /*ENDE Fangen verschiedener Custom Events*/
END_EVENT_TABLE()

RUZmBIFrame::RUZmBIFrame(wxFrame *frame, const wxString& title, const wxPoint &pos, const wxSize &siz, long style)
    : wxFrame(frame, -1, title, pos, siz, style)
{
    /*wxObjekte initialisieren*/
    FileOpener = new wxFileDialog(this, wxT("Zeichnung öffnen"),wxT(""),wxT(""),wxT("DXF- oder RUZ-Datei (*.dxf;*.ruz)|*.dxf;*.ruz|DXF-Datei (*.dxf)|*.dxf|RUZ-Datei (*.ruz)|*.ruz"), 1);
    FileSaver = new wxFileDialog(this, wxT("Zeichnung speichern"),wxT(""),wxT(""),wxT("DXF-Datei (*.dxf)|*.dxf|RUZ-Datei (*.ruz)|*.ruz"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    dxfParameterDlg = new DXF_Parameter_Dialog(this);
    hlParameterDlg = new HL_Parameter_Dialog(this);
    LayerAuswahl = new Layer_Verwaltungs_Dialog(this);
    peEinstellungenDlg = new Programm_Einstellungen_Dialog(this);
    KoordinatenMaske = new Koordinaten_Eingabe_Dialog(this, wxT("Koordinaten"));
    DoubleEingabe = new Double_Eingabe_Dialog(this, wxT("Eingabe"), wxT("Drehwinkel"), wxPoint(KoordinatenMaske->GetPosition().x, KoordinatenMaske->GetPosition().y + 180));
    ObjAnzAuswDlg = new Objekt_Anzeige_Auswahl_Dialog(this);
    /*ENDE wxObjekte initialisieren*/

    strAktuellerSpeicherpfad = wxEmptyString;

    /*Layer, Auswahl, etc*/
    m_layer = new Liste<RUZ_Layer>();
    m_skalierListe = new Liste<RUZ_Layer>();
    m_hintergrundLayer = new RUZ_Layer("Hintergrund");
    m_auswahl = new Liste<RUZ_Objekt>();
    m_kopierAuswahl = new Liste<RUZ_Objekt>();
    markiertesObjekt = NULL;

    aktLayer = alternativAktLayer = aktLayerBAK = NULL;
    aktLayer = new RUZ_Layer("STANDARD");
    if(aktLayer!=NULL)
    {
        m_layer->Hinzufuegen(aktLayer);
        LayerAuswahl->LayerHinzufuegen(wxT("STANDARD"), aktLayer);
    }
    /*ENDE Layer, Auswahl, etc*/

    ParamIni();

    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    /*Hauptmenus*/
    wxMenu* fileMenu = new wxMenu();
    wxMenu* layerMenu = new wxMenu();
    wxMenu* backgroundMenu = new wxMenu();
    wxMenu* editMenu = new wxMenu();
    wxMenu* drawMenu = new wxMenu();
    wxMenu* viewMenu = new wxMenu();
    wxMenu* miscMenu = new wxMenu();
    wxMenu* netzMenu = new wxMenu();
    wxMenu* planeMenu = new wxMenu();
    wxMenu* deleteMenu = new wxMenu();
    /*Dropdown*/
    wxMenu* openMenu = new wxMenu();
    wxMenu* open_m_lay_Menu = new wxMenu();
    wxMenu* open_o_lay_Menu = new wxMenu();
    wxMenu* open_stratis_Menu = new wxMenu();

    /*Menu: Datei*/
    /*Dropdown Untermenü*/
    open_m_lay_Menu->Append(idMenuDxfImp_mitLay_Ln_Pkt, wxT("&Punkte und Kanten"), wxT("Öffnen einer Datei mit Strukturinformationen"));
    open_m_lay_Menu->Append(idMenuDxfImp_mitLay_Pkt, wxT("&Nur Punkte"), wxT("Öffnen einer Datei mit Strukturinformationen"));
    open_o_lay_Menu->Append(idMenuDxfImp_ohneLay_Ln_Pkt, wxT("&Punkte und Kanten"), wxT("Öffnen einer Datei mit Strukturinformationen"));
    open_o_lay_Menu->Append(idMenuDxfImp_ohneLay_Pkt, wxT("&Nur Punkte"), wxT("Öffnen einer Datei mit Strukturinformationen"));
    openMenu->Append(wxID_ANY, wxT("DXF-Layer importieren"), open_m_lay_Menu);
    openMenu->Append(wxID_ANY, wxT("Ohne DXF-Layern"), open_o_lay_Menu);

    open_stratis_Menu->Append(idMenuD45Import, wxT("&Punkte einlesen (.d45)"), wxT("Importiert STRATIS-Punkte"));
    open_stratis_Menu->Append(idMenuD58Import, wxT("&Dreiecke einlesen (.d58)"), wxT("Importiert STRATIS-Dreiecke, zu vorher eingelesenen Punkten auf aktuellem Layer"));

    fileMenu->Append(wxID_ANY, wxT("DXF &öffnen\tF4"), openMenu);
    fileMenu->Append(idMenuRuzImport, wxT("&RUZ öffnen\tF5"), wxT("Öffnen einer RUZ-Datei"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_ANY, wxT("Import STRATIS-Schnittstelle"), open_stratis_Menu);
    fileMenu->AppendSeparator();
    fileMenu->Append(idMenuQuickSave, wxT("&Schnellspeichern\tF2"), wxT("Speichert Zeichnung in RUZ-Datei an aktuellem Pfad"));
    fileMenu->Append(idMenuFileSave, wxT("&Datei speichern\tF3"), wxT("Speichert Zeichnung in Datei"));
    fileMenu->Append(idMenuExportPrismen, wxT("&Exportiere Prismen"), wxT("Speichert Dreiecke zur externen Verarbeitung"));
    fileMenu->Append(idMenuExportPunkte, wxT("E&xportiere Punkte"), wxT("Speichert Punktes nach Layer sortiert (manuelle Sortierung mit SPACE mgl.)"));
    fileMenu->Append(idMenuQuit, wxT("&Beenden\tAlt-F4"), wxT("Anwendung schließen"));
    /*ENDE Menu: Datei*/

    /*Menu: Hintergrund*/
    backgroundMenu->Append(idMenuHintergrundEinlesen, wxT("&Hintergrund öffnen\tF6"), wxT("Öffnen einer Datei als Hintergrund"));
    backgroundMenu->Append(idMenuHintergrundSkalieren, wxT("Hintergrund skalieren"), wxT("Skaliert den Hintergrund"));
    backgroundMenu->Append(idMenuHintergrundLoeschen, wxT("Hintergrund löschen"), wxT("Löscht den Inhalt des Hintergrundlayers"));
    backgroundMenu->AppendSeparator();
    menuHintergrundMalen = new wxMenuItem(backgroundMenu, idMenuHintergrundMalen, wxT("Hintergrund anzeigen"), wxT("Zeigt den Hintergrund an"), wxITEM_CHECK);
    backgroundMenu->Append(menuHintergrundMalen);
    menuHintergrundMalen->Check(m_hintergrundMalen);
    /*ENDE Menu: Hintergrund*/

    /*Menu: Layer*/
    layerMenu->Append(idMenuLayerAuswahl, wxT("&Layerauswahl\tF9"), wxT("Öffnet das Layerauswahlmenü"));
    layerMenu->Append(idMenuLayerSkalieren, wxT("La&yer skalieren"), wxT("Skaliert kompletten Layer"));
    layerMenu->Append(idMenuLayerKopieren, wxT("Layer &kopieren"), wxT("Kopiert den aktuellen Layer samt Inhalt"));
    layerMenu->AppendSeparator();
    layerMenu->Append(idMenuLayerVerschneiden, wxT("L&ayer verschneiden"), wxT("Kopiert zwei Layer und verschneidet die Kopien miteinander"));
    layerMenu->Append(new wxMenuItem(layerMenu, idVolumenZwischenLayern_Kons, wxT("&Volumen zwischen Layern ermitteln"), wxT("Ausgabe getrennt nach Lage der Teilflächen in log-Datei")));
    layerMenu->Append(new wxMenuItem(layerMenu, idVolumenZwischenLayern_Integral, wxT("&Volumen zwischen Layern (Flächenintegral)"), wxT("Ausgabe getrennt nach Lage der Teilflächen in log-Datei")));
    //layerMenu->AppendSeparator();
    //layerMenu->Append(idMenuDeleteLayer, wxT("&Alle Layer löschen\tF10"), wxT("Löscht alle Layer inkl. Inhalt"));
    layerMenu->AppendSeparator();
    layerMenu->Append(idMenuLayerinhaltAnzeigen, wxT("&Layerinhalt anzeigen"), wxT("Zeigt Anzahl der Objekte im aktuellen Layer"));
    /*ENDE Menu: Layer*/

    /*Menu: Zeichnen*/
    /*CTRL-D*/menuDreieckZeichnen = new wxMenuItem(drawMenu, idMenuDreieckZeichnen, wxT("&Dreieck zeichnen\tCTRL-D"), wxT("Zeichnet Dreieck aus vorhandenen Linien"), wxITEM_CHECK);
    /*CTRL-H*/menuHoehenMarkeZeichnen = new wxMenuItem(drawMenu, idMenuHoehenMarkeZeichnen, wxT("&Setze Höhenmarken\tCTRL-H"), wxT("Platziert Höhenmarken auf aktiven Layer"), wxITEM_CHECK);
    /*CTRL-K*/menuKreisZeichnen = new wxMenuItem(drawMenu, idMenuKreisZeichnen, wxT("&Kreis zeichnen\tCTRL-K"), wxT("Zeichnet einen Kreis"), wxITEM_CHECK);
    /*CTRL-L*/menuLinieZeichnen = new wxMenuItem(drawMenu, idMenuLinieZeichnen, wxT("&Linien zeichnen\tCTRL-L"), wxT("Zeichnet Linien auf aktiven Layer"), wxITEM_CHECK);
    /*CTRL-P*/menuPunktZeichnen = new wxMenuItem(drawMenu, idMenuPunktZeichnen, wxT("&Punkte zeichnen\tCTRL-P"), wxT("Zeichnet Punkte auf aktiven Layer"), wxITEM_CHECK);
    /*CTRL-V*/menuViereckZeichnen = new wxMenuItem(drawMenu, idMenuViereckZeichnen, wxT("&Viereck zeichnen\tCTRL-V"), wxT("Zeichnet Vierecke aus vorhandenen Linien"), wxITEM_CHECK);

    drawMenu->Append(menuPunktZeichnen);
    drawMenu->Append(menuLinieZeichnen);
    drawMenu->Append(menuDreieckZeichnen);
    drawMenu->Append(menuViereckZeichnen);
    drawMenu->AppendSeparator();
    drawMenu->Append(menuKreisZeichnen);
    drawMenu->Append(menuHoehenMarkeZeichnen);
    /*ENDE Menu: Zeichnen*/

    /*Menu: Netz*/
    netzMenu->Append(idMenuKantenWandeln, wxT("&Linien finden"), wxT("Wandelt Striche in Linien"));
    netzMenu->Append(idMenuVernetzen, wxT("&Punkte vernetzen"), wxT("Verbindet alle Punkte mit sich nicht schneidenden Linien"));
    netzMenu->Append(idMenuDreieckeFinden, wxT("&Dreiecke finden"), wxT("Sucht geschlossene Dreiecke"));
    netzMenu->Append(idMenuViereckeFinden, wxT("&Vierecke finden"), wxT("Verschmilzt Dreiecke zu Vierecken"));
    netzMenu->AppendSeparator();
    netzMenu->Append(idMenuKomplettVernetzen, wxT("&Komplett vernetzen"), wxT("Vierecksnetz aus Punktwolke"));
    netzMenu->AppendSeparator();
    netzMenu->Append(idMenuHLEinstellen, wxT("Höhenlinien &einstellen"), wxT("Einstellungen für Höhenlinien vornehmen"));
    menuHLZeigen = new wxMenuItem(netzMenu, idMenuHLZeigen, wxT("Höhenlinien &anzeigen"), wxT("Findet und zeichnet Höhenlinie der Flächen"), wxITEM_CHECK);
    netzMenu->Append(menuHLZeigen);
    netzMenu->AppendSeparator();
    menuGefaelleZeigen = new wxMenuItem(netzMenu, idMenuGefaelleZeigen, wxT("&Gefälle bei Punkt"), wxT("Findet und zeichnet Gefälle der Flächen"), wxITEM_CHECK);
    netzMenu->Append(menuGefaelleZeigen);
    menuGefaelleRasterZeigen = new wxMenuItem(netzMenu, idMenuGefaelleRasterZeigen, wxT("&Gefälle im Raster"), wxT("Zeigt ein Vektorfeld der Gefälle im Raster an"), wxITEM_CHECK);
    netzMenu->Append(menuGefaelleRasterZeigen);
    /*Menu: Netz*/

    /*Menu: Ändern*/
    /*ALT-D*/menuDrehen = new wxMenuItem(editMenu, idMenuObjektDrehen, wxT("&Drehen\tALT-D"), wxT("Dreht gewählte Objekte"), wxITEM_CHECK);
    /*ALT-E*/menuLinieExtrudieren = new wxMenuItem(editMenu, idMenuLinieExtrudieren, wxT("&Fläche aus Linie erzeugen\tALT-E"),
                                          wxT("Erzeugt Fläche mit vorgegebenem Gefälle aus gewählten Linien auf aktiven Layer"), wxITEM_CHECK);
    /*ALT-F*/menuFangpunkteFinden = new wxMenuItem(editMenu, idMenuFangpunkteFinden, wxT("Fangpunkte aus Auswahl finden\tALT-F"),
                                             wxT("Kreise und Linien miteinander verschneiden - findet Fangpunkte"), wxITEM_CHECK);
    /*ALT-J*/menuPunktVereinigen = new wxMenuItem(editMenu, idMenuPunktVereinigen, wxT("Punkte ver&einigen\tALT-J"), wxT("Vereinigt den ersten mit dem zweiten Punkt"), wxITEM_CHECK);
    /*ALT-K*/menuKopieren = new wxMenuItem(editMenu, idMenuObjektKopieren, wxT("&Kopieren\tALT-K"), wxT("Kopiert gewählte Objekte"), wxITEM_CHECK);
    /*ALT-K*/menuKopierenNachLayer = new wxMenuItem(editMenu, idMenuObjektKopierenNachLayer, wxT("Kopieren - nach Layer"), wxT("Kopiert gewählte Objekte auf einen anderen Layer"), wxITEM_CHECK);
    /*ALT-M*/menuVersetzen = new wxMenuItem(editMenu, idMenuPunktVersetzen, wxT("&Punkt versetzen\tALT-M"), wxT("Versetzt einen Punkt"), wxITEM_CHECK);
    /*ALT-P*/menuLinieParallel = new wxMenuItem(editMenu, idMenuLinieParallel, wxT("P&arallele Linien erzeugen\tALT-P"),
                                          wxT("Erzeugt eine Parallele Linie zu gewählten Linien auf aktiven Layer"), wxITEM_CHECK);
    /*ALT-Q: Alles skalieren!*/
    /*ALT-S*/menuPunkteSkalieren = new wxMenuItem(editMenu, idMenuPunkteSkalieren, wxT("&Skalieren\tALT-S"), wxT("Skaliert die gewählten Objekte"), wxITEM_CHECK);
    /*ALT-V*/menuVerschieben = new wxMenuItem(editMenu, idMenuObjektVerschieben, wxT("&Verschieben\tALT-V"), wxT("Verschiebt gewählte Objekte"), wxITEM_CHECK);

    editMenu->Append(menuVerschieben);
    editMenu->Append(menuDrehen);
    editMenu->Append(menuKopieren);
    editMenu->Append(menuKopierenNachLayer);
    editMenu->Append(menuPunkteSkalieren);
    editMenu->Append(idMenuAllesSkalieren, wxT("Alles skalieren\tALT-Q"), wxT("Alle Layer und den Hintergrund skalieren"));
    editMenu->AppendSeparator();
    editMenu->Append(menuVersetzen);
    editMenu->Append(menuPunktVereinigen);
    editMenu->AppendSeparator();
    editMenu->Append(menuLinieParallel);
    editMenu->Append(menuLinieExtrudieren);
    editMenu->AppendSeparator();
    editMenu->Append(menuFangpunkteFinden);
    /*ENDE Menu: Ändern*/

    /*Menu: Flächen*/
    menuViereckTeilen = new wxMenuItem(planeMenu, idMenuViereckTeilen, wxT("Viereck in Dreiecke teilen\tCTRL-ALT-T"),
                                       wxT("Teilt Vierecke in Dreiecke entlang der kurzen Diagonalen"), wxITEM_CHECK);
    planeMenu->Append(menuViereckTeilen);
    planeMenu->AppendSeparator();
    menuSchnittPunktFlaeche = new wxMenuItem(planeMenu, idMenuSchnittPunktFlaeche, wxT("Schnittpunkt zweier Flächen finden"),
                                             wxT("Schnittpunkt der ersten Fäche mit der Zweiten finden (verschiebt den gewählten Punkt)"), wxITEM_CHECK);
    planeMenu->Append(menuSchnittPunktFlaeche);
    menuSchnittPunktLinie = new wxMenuItem(planeMenu, idMenuSchnittPunktLinie, wxT("Schnittpunkt einer Linie mit einem Objekt finden"),
                                             wxT("Schnittpunkt der Linie mit einem Objekt finden (verschiebt den gewählten Punkt)"), wxITEM_CHECK);
    planeMenu->Append(menuSchnittPunktLinie);
    planeMenu->AppendSeparator();
    menuFlaecheVerschneiden = new wxMenuItem(planeMenu, idMenuFlaechenVerschneiden, wxT("Verschneiden gewählter Flächen"),
                                             wxT("Verschneidet die gewählten Flächen miteinander"), wxITEM_CHECK);
    planeMenu->Append(menuFlaecheVerschneiden);
    planeMenu->Append(idMenuLayerInSichVerschneiden, wxT("Alle Flächen verschneiden"), wxT("Verschneidet alle Flächen im Layer miteinander"));
    planeMenu->AppendSeparator();
    planeMenu->Append(idMenuLayerRandabschneiden, wxT("Ränder abschneiden"), wxT("Schneidet zwei Layer entlang der Ränder ab"));
    planeMenu->Append(new wxMenuItem(planeMenu, idMenuUeberlappungFinden, wxT("&Überlappende Flächen finden"), wxT("Färbt überlappende Flächen ein")));
    /*ENDE Menu: Flächen*/

    /*Menu: Löschen*/
    deleteMenu->AppendSeparator();
    menuLoeschen = new wxMenuItem(deleteMenu, idMenuObjektLoeschen, wxT("&Löschen"), wxT("Löscht Objekte"), wxITEM_CHECK);
    deleteMenu->Append(menuLoeschen);
    deleteMenu->Append(idMenuDoppeltePunkteLoeschen, wxT("Doppelte Punkte löschen"), wxT("Vereint Punkte, die näher als ein Schwellenwert zusammenliegen"));
    deleteMenu->Append(idMenuStricheLoeschen, wxT("Striche löschen"), wxT("Löscht alle Striche des aktuellen Layers"));
    deleteMenu->Append(idMenuFangpunkteLoeschen, wxT("Alle Fangpunkte löschen"), wxT("Löscht alle Fangpunkte"));
    deleteMenu->AppendSeparator();
    deleteMenu->Append(idMenuEntferneTieferes, wxT("Lösche Pkte unterhalb"), wxT("Löscht Punkte, die unter Flächen liegen"));
    deleteMenu->Append(idMenuEntferneHoeheres, wxT("Lösche Pkte oberhalb"), wxT("Löscht Punkte, die über Flächen liegen"));
    /*ENDE Menu: Löschen*/

    /*Menu: Ansicht*/
    viewMenu->Append(ansicht_ID_normal, wxT("Orthogonale Aufsicht"), wxT("Aufsicht mit allen Anzeigen"));
    viewMenu->Append(ansicht_ID_pseudoschatten, wxT("Pseudoschattierte Aufsicht"), wxT("Aufsicht mit Schattierung der Flächen je nach Neigung der Fläche"));
    viewMenu->Append(ansicht_ID_3D, wxT("3D-Kamera"), wxT("Perspektivische Darstellung"));
    viewMenu->Append(ansicht_ID_hoehenkarte, wxT("Höhenkarte"), wxT("Zeigt Höhenschritte als gestaffelte Farben an"));
    viewMenu->AppendSeparator();
    viewMenu->Append(idMenuSonnenstand, wxT("Sonnenstand"), wxT("Stellt den Sonnenstand ein"));
    viewMenu->AppendSeparator();
	viewMenu->Append(idMenuSkalierFaktor, wxT("Anzeigeskalierung\tF11"), wxT("Öffnet einen Dialog zur Eingabe der gewünschten Anzeigeskalierung"));
    viewMenu->Append(idMenuGesamtansicht, wxT("Zeige alles\tF12"), wxT("Sucht die Ausdehnung der sichtbaren Objekte und zeigt alles"));
    aktuelleAnsicht = ansicht_ID_normal;
    /*ENDE Menu: Ansicht*/

    /*Menu: Verschiedenes*/
    miscMenu->Append(idMenuEinstellungen, wxT("&Programmeinstellungen"), wxT("Öffnet das Menü für Programmeinstellungen"));
    miscMenu->Append(idMenuZeigeWaehle, wxT("Sichtbarkeit / Wählbarkeit\tCTRL-ALT-S"),wxT("Öffnet den Dialog zur Auswahl und Anzeige der Objekte."));
    menuStreckeMessen = new wxMenuItem(miscMenu, idMenuStreckeMessen, wxT("&Messen\tCTRL-ALT-M"), wxT("Misst Entfernungen"), wxITEM_CHECK);
    miscMenu->Append(menuStreckeMessen);
    miscMenu->AppendSeparator();
		miscMenu->Append(idMenuAbout, wxT("Handbuch"), wxT("Zeigt das Handbuch an"));
    miscMenu->Append(idMenuLizenz, wxT("&Lizenz"), wxT("Zeigt die Lizenz an"));
    /*ENDE Menu: Verschiedenes*/


    mbar->Append(fileMenu, wxT("&Datei"));
    mbar->Append(layerMenu, wxT("&Layer"));
    mbar->Append(drawMenu, wxT("&Zeichnen"));
    mbar->Append(editMenu, wxT("&Ändern"));
    mbar->Append(planeMenu, wxT("&Flächen"));
    mbar->Append(netzMenu, wxT("&Netz"));
    mbar->Append(deleteMenu, wxT("&Löschen"));
    mbar->Append(viewMenu, wxT("Ansicht"));
    mbar->Append(backgroundMenu, wxT("&Hintergrund"));
    mbar->Append(miscMenu, wxT("&Verschiedenes"));

    SetMenuBar(mbar);

    /*Log starten*/
	logSchreiben("Logbuch gestartet\n");

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(4); /*0 = Status, 1 = Informationen zu Parametern etc., 2 = Befehlsbereich, 3 = CursorPosition*/
    Maximize(true);
}

RUZmBIFrame::~RUZmBIFrame()
{
    m_auswahl->ListeLeeren("RUZ");
    m_kopierAuswahl->ListeLeeren("RUZ");
    m_skalierListe->ListeLeeren("RUZ");
    m_drehungAuswahlOrte->ListeLeeren("RUZ");
    m_verschubAuswahlOrte->ListeLeeren("RUZ");
    m_layer->ListeLeeren("RUZ");
    delete FileOpener;
    delete FileSaver;
    delete dxfParameterDlg;
    delete hlParameterDlg;
    delete LayerAuswahl;
    delete peEinstellungenDlg;
    delete m_auswahl;
    delete m_kopierAuswahl;
    delete m_layer;
    delete m_skalierListe;
    delete m_hintergrundLayer;
    delete KoordinatenMaske;
    delete DoubleEingabe;
    delete m_drehungAuswahlOrte;
    delete m_verschubAuswahlOrte;
}

void RUZmBIFrame::AnfangMessen(double x, double y)
{
    messAnfang.x = x;
    messAnfang.y = y;

    SetStatusText(wxString::Format("Messpunkt: %.3f, %.3f", messAnfang.x, messAnfang.y), 1);
    Messen = &RUZmBIFrame::EndeMessen;
    return;
}

void RUZmBIFrame::AusdehnungFinden(void)
{
    //if(aktLayer == NULL)return;
    wxClientDC cl_dc(this);
    double abmessungCL_dc_X = cl_dc.GetSize().GetWidth();
    double abmessungCL_dc_Y = cl_dc.GetSize().GetHeight();

    double max_x, max_y, min_x, min_y;
    max_x = max_y = min_x = min_y = 0.0;
    RUZ_Layer* lokAktLayer;
    Punkt *tempPkt;
    HoehenMarke *tempHM;
    Kreis *tempKr;
    double tempRadius;
    Fangpunkt *tempFP;

    lokAktLayer = m_layer->GetErstesElement();
    if(lokAktLayer == NULL)return;

    bool ersterSichtbarerLayer = true;
    for(Listenelement<RUZ_Layer>* aktLayer_LE = m_layer->GetErstesListenelement(); aktLayer_LE != NULL; aktLayer_LE = aktLayer_LE->GetNachfolger())
    {
        lokAktLayer = aktLayer_LE->GetElement();
        if(lokAktLayer == NULL)continue;
        if(!(lokAktLayer->IstSichtbar()))continue;
        if(ersterSichtbarerLayer)
        {
            tempPkt = lokAktLayer->HolePunkte()->GetErstesElement();
            tempHM = lokAktLayer->HoleHoehenMarken()->GetErstesElement();
            tempKr = lokAktLayer->HoleKreise()->GetErstesElement();
            tempFP = lokAktLayer->HoleFangpunkte()->GetErstesElement();
            if(tempPkt != NULL)
            {
                max_x = min_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
                max_y = min_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
                ersterSichtbarerLayer = false;
            }
            if(tempHM != NULL)
            {
                max_x = min_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
                max_y = min_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
                ersterSichtbarerLayer = false;
            }
            if(tempKr != NULL)
            {
                max_x = min_x = tempKr->HolePosition().GetKoordinaten(aktProjX);
                max_y = min_y = tempKr->HolePosition().GetKoordinaten(aktProjY);
                ersterSichtbarerLayer = false;
            }
            if(tempFP != NULL)
            {
                max_x = min_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
                max_y = min_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
                ersterSichtbarerLayer = false;
            }
        }
        for(tempPkt = lokAktLayer->HolePunkte()->GetErstesElement(); tempPkt != NULL; tempPkt = lokAktLayer->HolePunkte()->GetNaechstesElement())
        {
            if(tempPkt->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
            if(tempPkt->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
            if(tempPkt->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
            if(tempPkt->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
        }
        for(tempHM = lokAktLayer->HoleHoehenMarken()->GetErstesElement(); tempHM != NULL; tempHM = lokAktLayer->HoleHoehenMarken()->GetNaechstesElement())
        {
            if(tempHM->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
            if(tempHM->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
            if(tempHM->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
            if(tempHM->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
        }
        for(tempKr = lokAktLayer->HoleKreise()->GetErstesElement(); tempKr != NULL; tempKr = lokAktLayer->HoleKreise()->GetNaechstesElement())
        {
            tempRadius = tempKr->HoleRadius();
            if((tempKr->HolePosition().GetKoordinaten(aktProjX) - tempRadius) < min_x)min_x = tempKr->HolePosition().GetKoordinaten(aktProjX) - tempRadius;
            if((tempKr->HolePosition().GetKoordinaten(aktProjX) + tempRadius) > max_x)max_x = tempKr->HolePosition().GetKoordinaten(aktProjX) + tempRadius;
            if((tempKr->HolePosition().GetKoordinaten(aktProjY) - tempRadius) < min_y)min_y = tempKr->HolePosition().GetKoordinaten(aktProjY) - tempRadius;
            if((tempKr->HolePosition().GetKoordinaten(aktProjY) + tempRadius) > max_y)max_y = tempKr->HolePosition().GetKoordinaten(aktProjY) + tempRadius;
        }
        for(tempFP = lokAktLayer->HoleFangpunkte()->GetErstesElement(); tempFP != NULL; tempFP = lokAktLayer->HoleFangpunkte()->GetNaechstesElement())
        {
            if(tempFP->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
            if(tempFP->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
            if(tempFP->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
            if(tempFP->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
        }
    }
    lokAktLayer = m_hintergrundLayer;
    Strich* tempStrich = lokAktLayer->HoleStriche()->GetErstesElement();
    if(tempStrich != NULL)
    {
        if(min_x == 0)min_x = tempStrich->Xa();
        if(max_x == 0)max_x = tempStrich->Xa();
        if(min_y == 0)min_y = tempStrich->Ya();
        if(max_y == 0)max_y = tempStrich->Ya();
    }
    for(; tempStrich != NULL; tempStrich = lokAktLayer->HoleStriche()->GetNaechstesElement())
    {
        if(tempStrich->Xa() < min_x)min_x = tempStrich->Xa();
        if(tempStrich->Xa() > max_x)max_x = tempStrich->Xa();
        if(tempStrich->Ya() < min_y)min_y = tempStrich->Ya();
        if(tempStrich->Ya() > max_y)max_y = tempStrich->Ya();

        if(tempStrich->Xe() < min_x)min_x = tempStrich->Xe();
        if(tempStrich->Xe() > max_x)max_x = tempStrich->Xe();
        if(tempStrich->Ye() < min_y)min_y = tempStrich->Ye();
        if(tempStrich->Ye() > max_y)max_y = tempStrich->Ye();
    }
    if((max_x == min_x)||(max_y == min_y))
    {
		SkalierungSetzen(1.0);
    }else{
        double t_skal = ((abmessungCL_dc_X/(max_x - min_x)) < (abmessungCL_dc_Y/(max_y - min_y))) ?
                            (abmessungCL_dc_X/(max_x - min_x)) : (abmessungCL_dc_Y/(max_y - min_y));
		SkalierungSetzen(t_skal * 0.9);
    }
    dc_Offset[0] = 0.5 * ((min_x + max_x) - abmessungCL_dc_X / m_skalierung);
    dc_Offset[1] = 0.5 * ((min_y + max_y) - abmessungCL_dc_Y / m_skalierung);
    SetStatusText(wxString::Format("Offset x, y: %1.5f, %1.5f - Skalierung: %5.5f", dc_Offset[0], dc_Offset[1], m_skalierung), 1);
    Refresh();
    return;
}

void RUZmBIFrame::AuswahlAufPunkteReduzieren(void)
{
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* ln = static_cast<Linie*>(obj);
            m_auswahl->Entfernen(obj);
            m_auswahl->ExklusivHinzufuegen(ln->HolePunkt(0));
            m_auswahl->ExklusivHinzufuegen(ln->HolePunkt(1));
        }else
        if(obj->HoleTyp() == RUZ_Dreieck)
        {
            Dreieck* drk = static_cast<Dreieck*>(obj);
            m_auswahl->Entfernen(obj);
            m_auswahl->ExklusivHinzufuegen(drk->HolePunkt(0));
            m_auswahl->ExklusivHinzufuegen(drk->HolePunkt(1));
            m_auswahl->ExklusivHinzufuegen(drk->HolePunkt(2));
        }else
        if(obj->HoleTyp() == RUZ_Viereck)
        {
            Viereck* vrk = static_cast<Viereck*>(obj);
            m_auswahl->Entfernen(obj);
            m_auswahl->ExklusivHinzufuegen(vrk->HolePunkt(0));
            m_auswahl->ExklusivHinzufuegen(vrk->HolePunkt(1));
            m_auswahl->ExklusivHinzufuegen(vrk->HolePunkt(2));
            m_auswahl->ExklusivHinzufuegen(vrk->HolePunkt(3));
        }

    }
    return;
}

void RUZmBIFrame::AuswahlKopieren(void)
{
    RUZ_Layer* sel_Layer = aktLayer;
    if(aktBefehl == bef_ID_kopierenNachLayer)
    {
        int layNr = Layer_Auswahl_Dialog(this, m_layer, wxT("Zu kopierenden Layer wählen")).ShowModal();
        sel_Layer = NULL;
        for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
        {
            if(layer_LE->Wert() == layNr)
            {
                sel_Layer = layer_LE->GetElement();
                break;
            }
        }
        if(!sel_Layer)sel_Layer = aktLayer;
    }

    m_kopierAuswahl->ListeLeeren("RUZmBIFrame::AuswahlKopieren");
    Liste<ObjektPaar>* lKopierliste = new Liste<ObjektPaar>;
    for(RUZ_Objekt *oAktObj = m_auswahl->GetErstesElement(); oAktObj; oAktObj = m_auswahl->GetNaechstesElement())
    {
        if(!(oAktObj->Kopieren(m_kopierAuswahl, lKopierliste, sel_Layer)))
        {
            logSchreiben("Kopieren von %p fehlgeschlagen\n", oAktObj);
        }
    }
    delete lKopierliste;
    return;
}

void RUZmBIFrame::AuswahlLeeren(void)
{
    ObjekteNullen();
    m_auswahl->ListeLeeren("");
    return;
}

void RUZmBIFrame::AuswahlLoeschen(void)
{
    ObjekteNullen();
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            Punkt* pkt = static_cast<Punkt*>(obj);
            Liste<Linie>* ln_Lst = pkt->HoleLinien();
            for(Linie* ln = ln_Lst->GetErstesElement(); ln != NULL; ln = ln_Lst->GetNaechstesElement())
            {
                m_auswahl->Entfernen(ln);
                Liste<Flaeche>* fl_Lst = ln->HoleFlaechen();
                for(Flaeche* fl = fl_Lst->GetErstesElement(); fl != NULL; fl = fl_Lst->GetNaechstesElement())
                {
                    m_auswahl->Entfernen(fl);
                }
            }
        }
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* ln = static_cast<Linie*>(obj);
            Liste<Flaeche>* fl_Lst = ln->HoleFlaechen();
            for(Flaeche* fl = fl_Lst->GetErstesElement(); fl != NULL; fl = fl_Lst->GetNaechstesElement())
            {
                m_auswahl->Entfernen(fl);
            }
        }
    }
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
    {
        m_auswahl->Entfernen(obj);
        delete obj;
    }
    m_auswahl->ListeLeeren("");
    return;
}

void RUZmBIFrame::BefehleZuruecksetzen(void)
{
    MenuEntmarkieren();
    AuswahlLeeren();

    /*Verschieben*/
    if(vVerschubStart)delete vVerschubStart;
    vVerschubStart = NULL;
    /*ENDE Verschieben*/

    m_auswahl->ListeLeeren("");
    if((aktBefehl == bef_ID_kreisZeichnen) && m_aktKreis)
    {
        delete m_aktKreis;
        m_aktKreis = NULL;
    }
    if(aktBefehl == bef_ID_kreisRadiusAendern)
    {
        if(m_aktKreis)m_aktKreis->SetzeRadius(m_aktRadius);
        m_aktRadius = -1.0;
        m_aktKreis = NULL;
    }
    if(!std::isnan(m_vktSchnittPkt1.x()))
        m_vktSchnittPkt1 = NULL_VEKTOR;
    if(!std::isnan(m_vktSchnittPkt2.x()))
        m_vktSchnittPkt2 = NULL_VEKTOR;

    aktBefehl = bef_ID_nichts;
    SetStatusText(wxT("kein aktiver Befehl"), 2);
    m_markierModus = false;
    Refresh();
    return;
}

void RUZmBIFrame::DoppeltePunkteLoeschen(wxCommandEvent& event)
{
    /*evtl. noch Layerauswahldialog???*/
    /*for(RUZ_Layer* tempLayer = m_layer->GetErstesElement(); tempLayer; tempLayer = m_layer->GetNaechstesElement())*/
    AuswahlLeeren();
    if(aktLayer)aktLayer->LoescheDoppeltePunkte(m_anzeigeGenauigkeit + 2);
    return;
}

void RUZmBIFrame::DreheAuswahl(void)
{
    if((!m_drehungDrPkt) || (!m_drehungRichtung1) || (!m_drehungRichtung2))return;
    if(m_drehungRichtung1 == m_drehungRichtung2)return;

    Vektor vDrehungsRichtung1 = *m_drehungRichtung1 - *m_drehungDrPkt;
    Vektor vDrehungsRichtung2 = *m_drehungRichtung2 - *m_drehungDrPkt;

    /*if(!freieAnsicht) falls irgendwann freie Ansicht dazu kommt*/
    vDrehungsRichtung1.SetKoordinaten(aktProjZ, 0.0f);
    vDrehungsRichtung2.SetKoordinaten(aktProjZ, 0.0f);

    double dL1, dL2;
    dL1 = vDrehungsRichtung1.Laenge();
        if(dL1 == 0)return;
        vDrehungsRichtung1 /= dL1;
    dL2 = vDrehungsRichtung2.Laenge();
        if(dL2 == 0)return;
        vDrehungsRichtung2 /= dL2;

    if(vDrehungsRichtung1 == vDrehungsRichtung2)return;
    double drCos, drSin;

    drCos = (vDrehungsRichtung1) * (vDrehungsRichtung2); /*Vektoren sind normiert! ansonsten Division durch deren Länge nötig*/
    drSin = sqrt(1 - drCos * drCos);

    Vektor vDrehAchse;
    if(drCos == -1)
    {
        vDrehAchse = Vektor(0.0, 0.0, 0.0);
        vDrehAchse.SetKoordinaten(aktProjZ, 1.0);
    }
    else
    {
        vDrehAchse = vDrehungsRichtung1.Kreuz(vDrehungsRichtung2);
    }
    vDrehAchse /= vDrehAchse.Laenge();

    Vektor *vOrt = m_drehungAuswahlOrte->GetErstesElement();
    RUZ_Objekt *obj = m_auswahl->GetErstesElement();

    Vektor vAltePos, vNeuePos;

    while(vOrt && obj)
    {
        vAltePos = *vOrt;
        vAltePos -= *m_drehungDrPkt;
        vNeuePos = vDrehAchse * (vDrehAchse * vAltePos) + (vDrehAchse.Kreuz(vAltePos)).Kreuz(vDrehAchse) * drCos + vDrehAchse.Kreuz(vAltePos) * drSin;
        vNeuePos += *m_drehungDrPkt;

        if(obj->HoleTyp() == RUZ_Punkt)
        {
            Punkt* pkt = static_cast<Punkt*>(obj);
            pkt->Positionieren(vNeuePos);
        }else if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            HoehenMarke* hm = static_cast<HoehenMarke*>(obj);
            hm->Positionieren(vNeuePos);
        }else if(obj->HoleTyp() == RUZ_Kreis)
        {
            Kreis* kr = static_cast<Kreis*>(obj);
            kr->Positionieren(vNeuePos);
        }
        vOrt = m_drehungAuswahlOrte->GetNaechstesElement();
        obj = m_auswahl->GetNaechstesElement();
    }
    return;
}

/*Drehung*/

void RUZmBIFrame::DrehungAbbruch_0(void)
{
    m_drehungAuswahlOrte->ListeLoeschen("");
    m_auswahl->ListeLeeren("");
    return;
}
void RUZmBIFrame::DrehungAbbruch_1(void)
{
    m_drehungAuswahlOrte->ListeLoeschen("");
    m_auswahl->ListeLeeren("");
    if(m_drehungDrPkt)delete m_drehungDrPkt;
    m_drehungDrPkt = NULL;
    return;
}
void RUZmBIFrame::DrehungAbbruch_2(void)
{
    m_drehungAuswahlOrte->ListeLoeschen("");
    m_auswahl->ListeLeeren("");
    if(m_drehungDrPkt)delete m_drehungDrPkt;
    if(m_drehungRichtung1)delete m_drehungRichtung1;
    if(m_drehungRichtung2)delete m_drehungRichtung2;
    m_drehungDrPkt = m_drehungRichtung1 = m_drehungRichtung2 = NULL;
    return;
}

void RUZmBIFrame::DrehungBefehlsketteVor_0(void)
{
    m_markierModus = false;
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_1;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_1;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_DrhPkt;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_1;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    KoordinatenMaske->Show();
    SetStatusText(wxT("Festpunkt der Drehung setzen"), 2);
    return;
}
void RUZmBIFrame::DrehungBefehlsketteVor_1(void)
{
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_2;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_2;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_RP_1;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_2;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    DoubleEingabe->ErscheinungAnpassen(wxString("Drehwinkel"), wxEmptyString, aktBefehl);
    DoubleEingabe->Show();
    SetStatusText(wxT("1.Richtung der Drehung festlegen"), 2);
    return;
}
void RUZmBIFrame::DrehungBefehlsketteVor_2(void)
{
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_3;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_3;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_RP_2;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_2;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMoveAktiv;
    SetStatusText(wxT("2. Richtung der Drehung festlegen"), 2);
    return;
}
void RUZmBIFrame::DrehungBefehlsketteVor_3(void)
{
    m_markierModus = true;
    m_drehungAuswahlOrte->ListeLoeschen("");
    m_auswahl->ListeLeeren("");
    if(m_drehungDrPkt)delete m_drehungDrPkt;
    if(m_drehungRichtung1)delete m_drehungRichtung1;
    if(m_drehungRichtung2)delete m_drehungRichtung2;
    m_drehungDrPkt = m_drehungRichtung1 = m_drehungRichtung2 = NULL;
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_0;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_0;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_DrhPkt;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_0;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    DoubleEingabe->Show(false);
    KoordinatenMaske->Show(false);
    SetStatusText(wxT("Objekte zum Drehen wählen"), 2);
    return;
}

void RUZmBIFrame::DrehungBefehlsketteZurueck_0(void)
{
    (this->*DrehungAbbruch)();
    BefehleZuruecksetzen();
    return;
}
void RUZmBIFrame::DrehungBefehlsketteZurueck_1(void)
{
    m_markierModus = true;
    m_drehungAuswahlOrte->ListeLoeschen("");
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_0;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_0;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_DrhPkt;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_0;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    KoordinatenMaske->Show(false);
    SetStatusText(wxT("Objekte zu Drehen wählen"), 2);
    return;
}
void RUZmBIFrame::DrehungBefehlsketteZurueck_2(void)
{
    if(m_drehungDrPkt)delete m_drehungDrPkt;
    m_drehungDrPkt = NULL;
    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_1;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_1;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_DrhPkt;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_1;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    DoubleEingabe->Show(false);
    SetStatusText(wxT("Festpunkt der Drehung setzen"), 2);
    return;
}
void RUZmBIFrame::DrehungBefehlsketteZurueck_3(void)
{
    if(m_drehungRichtung1)delete m_drehungRichtung1;
    m_drehungRichtung1 = NULL;
    if(m_drehungRichtung2)delete m_drehungRichtung2;
    m_drehungRichtung2 = NULL;

    Vektor *vOrt = m_drehungAuswahlOrte->GetErstesElement();
    RUZ_Objekt *obj = m_auswahl->GetErstesElement();
    while(vOrt && obj)
    {
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            Punkt* pkt = static_cast<Punkt*>(obj);
            pkt->Positionieren(*vOrt);
        }
        if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            HoehenMarke* hm = static_cast<HoehenMarke*>(obj);
            hm->Positionieren(*vOrt);
        }
        if(obj->HoleTyp() == RUZ_Kreis)
        {
            Kreis* kr = static_cast<Kreis*>(obj);
            kr->Positionieren(*vOrt);
        }
        vOrt = m_drehungAuswahlOrte->GetNaechstesElement();
        obj = m_auswahl->GetNaechstesElement();
    }

    DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_2;
    DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_2;
    DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_RP_1;
    DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_2;
    DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
    SetStatusText(wxT("1.Richtugspunkt der Drehung setzen"), 2);
    return;
}

void RUZmBIFrame::DrehungBestaetigung(void)
{
    AuswahlAufPunkteReduzieren();
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            m_drehungAuswahlOrte->Hinzufuegen(new Vektor((static_cast<Punkt*>(obj))->HolePosition()));
        }
        if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            m_drehungAuswahlOrte->Hinzufuegen(new Vektor((static_cast<HoehenMarke*>(obj))->HolePosition()));
        }
        if(obj->HoleTyp() == RUZ_Kreis)
        {
            m_drehungAuswahlOrte->Hinzufuegen(new Vektor((static_cast<Kreis*>(obj))->HolePosition()));
        }
    }
    (this->*DrehungBefehlsketteVor)();
    return;
}

void RUZmBIFrame::DrehungMouseMoveAktiv(wxMouseEvent &event)
{
    m_drehungRichtung2->SetKoordinaten(aktProjX, NeueMousePosition.x/m_skalierung + dc_Offset[0]);
    m_drehungRichtung2->SetKoordinaten(aktProjY, NeueMousePosition.y/m_skalierung + dc_Offset[1]);
    m_drehungRichtung2->SetKoordinaten(aktProjZ, 0.0);
    DreheAuswahl();
    return;
}

void RUZmBIFrame::DrehungMouseMovePassiv(wxMouseEvent &event)
{
    return;
}

void RUZmBIFrame::DrehungPktEingabe_DrhPkt(Vektor vPkt)
{
    m_drehungDrPkt = new Vektor(vPkt);

    Vektor vOrt = vPkt;
    //KoordinatenMaske->SetzeKoordinaten(vOrt);
    DoubleEingabe->ErscheinungAnpassen(wxString("Drehwinkel"), wxEmptyString, aktBefehl);
    DoubleEingabe->Show();

    (this->*DrehungBefehlsketteVor)();
    return;
}

void RUZmBIFrame::DrehungPktEingabe_RP_1(Vektor vPkt)
{
    if(m_drehungRichtung1)
    {
        *m_drehungRichtung1 = vPkt;
    }else
    {
        m_drehungRichtung1 = new Vektor(vPkt);
    }
    if(m_drehungRichtung2)
    {
        *m_drehungRichtung2 = vPkt;
    }else
    {
        m_drehungRichtung2 = new Vektor(vPkt);
    }

    m_drehungRichtung1->SetKoordinaten(aktProjZ, 0.0);
    m_drehungRichtung2->SetKoordinaten(aktProjZ, 0.0);

    //KoordinatenMaske->SetzeKoordinaten(vPkt);

    (this->*DrehungBefehlsketteVor)();
    return;
}

void RUZmBIFrame::DrehungPktEingabe_RP_2(Vektor vPkt)
{
    *m_drehungRichtung2 = vPkt;
    m_drehungRichtung2->SetKoordinaten(aktProjZ, 0.0);

    DreheAuswahl();
    (this->*DrehungBefehlsketteVor)();
    return;
}

/*ENDE Drehung*/

void RUZmBIFrame::EndeMessen(double x, double y)
{
    messEnde.x = x;
    messEnde.y = y;

    double messung = sqrt(pow((messAnfang.x - messEnde.x), 2) + pow((messAnfang.y - messEnde.y), 2));

    SetStatusText(wxString::Format("Messung: %.3f", messung), 1);
    Messen = &RUZmBIFrame::AnfangMessen;
    return;
}

bool RUZmBIFrame::ExportiereDreiecksPrismen(char* dateiName)
{
    std::ofstream Datei;
    Datei.open(dateiName, ios_base::out|ios_base::trunc);
    logSchreiben("%s konnte geoffnet werden\n", dateiName);

    if(Datei.good())
    {
        Datei.setf( ios::fixed, ios::floatfield );
        Datei.precision(15);

        Punkt* tempPunkt;
        Flaeche* tempFlaeche;

        if(aktLayer)
        {
            Liste<Punkt>* pktSammlung = aktLayer->HolePunkte();
            for(tempPunkt = pktSammlung->GetErstesElement(); tempPunkt != NULL; tempPunkt = pktSammlung->GetNaechstesElement())
            {
                Datei<<tempPunkt<<"\t"<<tempPunkt->HolePosition().x()<<"\t"<<-tempPunkt->HolePosition().y()<<"\t"<<tempPunkt->HolePosition().z()<<"\n";
            }

            Liste<Flaeche>* flSammlung = aktLayer->HoleFlaechen();
            for(tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
            {
                if(tempFlaeche->HoleTyp() == RUZ_Dreieck)
                {
                    Datei<<"\n"<<tempFlaeche->HolePunkt(0)<<"\n"<<tempFlaeche->HolePunkt(1)<<"\n"<<tempFlaeche->HolePunkt(2)<<"\n";
                }
            }
        }
    }
    else
    {
        return false;
    }
    Datei.close();
    return true;
}

bool RUZmBIFrame::ExportierePunkte(char* dateiName)
{
    std::ofstream Datei;
    Datei.open(dateiName, ios_base::out|ios_base::trunc);
    logSchreiben("%s konnte geoffnet werden\n", dateiName);

    if(Datei.good())
    {
        Datei.setf( ios::fixed, ios::floatfield );
        Datei.precision(15);

        Punkt* tempPunkt;

        for(RUZ_Layer* layLauefer = m_layer->GetErstesElement(); layLauefer != NULL; layLauefer = m_layer->GetNaechstesElement())
        {
            Datei<<layLauefer->HoleName()<<"\n";
            Liste<Punkt>* pktSammlung = layLauefer->HolePunkte();
            for(tempPunkt = pktSammlung->GetErstesElement(); tempPunkt != NULL; tempPunkt = pktSammlung->GetNaechstesElement())
            {
                Datei<<tempPunkt<<"\t"<<tempPunkt->HolePosition().x()<<"\t"<<-tempPunkt->HolePosition().y()<<"\t"<<tempPunkt->HolePosition().z()<<"\n";
            }
            Datei<<"\n";
        }
    }
    else
    {
        return false;
    }
    Datei.close();
    return true;
}

void RUZmBIFrame::FangeDrehwinkel(aruDblEvent& event)
{
    int tempID = event.GetId();
    double drehWinkel = event.HoleWert();

    while(drehWinkel < -180)drehWinkel += 360;
    while(drehWinkel > 180)drehWinkel -= 360;

    SetStatusText(wxString::Format(wxT("Drehung um %0.2f °"),drehWinkel));

    Vektor vAdd(0.0, 0.0, 0.0);
    vAdd.SetKoordinaten(aktProjX, 1.0);
    if(m_drehungRichtung1)
    {
        *m_drehungRichtung1 = *m_drehungDrPkt + vAdd;
    }
    else
    {
        m_drehungRichtung1 = new Vektor(*m_drehungDrPkt);
        *m_drehungRichtung1 += vAdd;
    }

    vAdd = Vektor(0.0, 0.0, 0.0);
    if(drehWinkel == 90)
    {
        vAdd.SetKoordinaten(aktProjY, 1.0);
    }else
    if(drehWinkel == 180)
    {
        vAdd.SetKoordinaten(aktProjX, -1.0);
    }else
    if(drehWinkel == -90)
    {
        vAdd.SetKoordinaten(aktProjY, -1.0);
    }else
    if(drehWinkel == 0)
    {
        vAdd.SetKoordinaten(aktProjX, 1.0);
    }else
    {

        vAdd.SetKoordinaten(aktProjX, cos(drehWinkel*PI/180));
        vAdd.SetKoordinaten(aktProjY, sin(drehWinkel*PI/180));
    }
    if(m_drehungRichtung2)
    {
        *m_drehungRichtung2 = *m_drehungDrPkt + vAdd;
    }
    else
    {
        m_drehungRichtung2 = new Vektor(*m_drehungDrPkt);
        *m_drehungRichtung2 += vAdd;

    }
    DreheAuswahl();

    switch(tempID)
    {
    case idDrehwinkel_temp:
        break;
    case idDrehwinkel_perm:
        (this->*DrehungBefehlsketteVor)();
        (this->*DrehungBefehlsketteVor)();
        break;
    default:;
    }
    Refresh();
    return;
}

void RUZmBIFrame::FangeFarben(aruColourEvent& event)
{
    int tempID = event.GetInt();
    wxColour Wert = event.HoleWert();

    switch(tempID)
    {
    case IDFarbePktLn:
        col_Pkt_Ln = Wert;
        break;
    case IDFarbeStrich:
        col_Strich = Wert;
        break;
    case IDFarbeHoehenMarke:
        col_HoehenMarke = Wert;
        break;
    case IDFarbeHoehenlinie:
        col_Hoehenlinie = Wert;
        break;
    case IDFarbeMarkiertesObjekt:
        col_markiert_Obj = Wert;
        break;
    case IDFarbeAusgewaehltesObjekt:
        col_ausgewaehlt_Obj = Wert;
        break;
    case IDFarbeHintergrundLayer:
        col_HintergrundLayer = Wert;
        break;
    case IDFarbeZeichenHintergrund:
        col_ZeichenHintergrund = Wert;
        break;
    case IDFarbeAuswahlRechteck:
        col_AuswahlRechteck = Wert;
        break;
    case IDFarbeFlaecheDarueber:
        col_Flaeche_darueber = Wert;
        break;
    case IDFarbeFlaecheDarunter:
        col_Flaeche_darunter = Wert;
        break;
    case IDFarbeGefaelle:
        col_Gefaelle = Wert;
        break;
    case IDFarbeFangpunkt:
        col_Fangpunkt = Wert;
        break;
    }
    Refresh();
    return;
}

void RUZmBIFrame::FangeKoor(aruVektorEvent& event)
{
    Vektor vOrt = event.HoleKoordinaten();
        if(markiertesObjekt!=NULL)
        {
            Vektor tempVkt;
            if(markiertesObjekt->HolePosition(tempVkt))
                vOrt += tempVkt;
        }

    if(aktLayer == NULL)
    {
        wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
        return;
    }
    if(aktBefehl == bef_ID_punktZeichnen)
    {
        Punkt *tempPunkt = new Punkt(vOrt, aktLayer);
        if(tempPunkt)
        {
            if(aktLayer->PunktDoppeltVorhanden(tempPunkt))
            {
                delete tempPunkt;
                wxMessageDialog(this, wxT("Punkt gab es schon!"), wxT("Achtung")).ShowModal();
            }else{
            }
        }
    }
    if(aktBefehl == bef_ID_linieZeichnen)
    {
        if(!m_aktPunkt)
        {
            Punkt *pktPkt = new Punkt(vOrt, aktLayer);
            if(pktPkt)
            {
                m_aktPunkt = pktPkt;
                Linie::NeueLinie(m_aktPunkt, new Punkt(vOrt, aktLayer));
                m_aktPosition = vOrt;
            }
        }else
        {
            m_aktPunkt->Positionieren(vOrt);
            m_aktPunkt = NULL;
            m_aktLinie = NULL;
        }
    }
    if(aktBefehl == bef_ID_verschieben)
    {
        Vschb_Punkt_Festlegen(vOrt);
    }
    if((aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
    {
        Kop_Punkt_Festlegen(vOrt);
    }
    if(aktBefehl == bef_ID_versetzen)
    {
        RUZ_Objekt* obj = m_auswahl->GetErstesElement();
        if(obj == NULL)return;
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            static_cast<Punkt*>(obj)->Positionieren(vOrt);
        }
        if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            static_cast<HoehenMarke*>(obj)->Positionieren(vOrt);
        }
    }
    if(aktBefehl == bef_ID_punkteSkalieren)
    {
        SetStatusText(wxT("Skalierfaktoren eingeben"), 2);
        SkalierungAusfuehren(Vektor(vOrt.x(), vOrt.y(), vOrt.z()));
        SetStatusText(wxT("Objekte zum Skalieren auswählen"), 2);
        m_markierModus = true;
    }
    if((aktBefehl == bef_ID_layerSkalieren)||
       (aktBefehl == bef_ID_hintergrundSkalieren)||
       (aktBefehl == bef_ID_allesSkalieren))
    {
        if(!SkalierFaktorenEingabe())
        {
            BefehleZuruecksetzen();
            return;
        }
        LayerSkalieren(vOrt);
    }
    if(aktBefehl == bef_ID_drehen)
    {
        (this->*DrehungPktEingabe)(vOrt);
    }
    if(aktBefehl == bef_ID_kreisZeichnen)
    {
        Vektor t_vkt = Vektor(vOrt.x(), vOrt.y(), vOrt.z());
        KreisPunktEingabe(t_vkt, true);
    }
    Refresh();
    return;
}

void RUZmBIFrame::FangeLayerSkalieren(aruLayerListeEvent& event)
{
    Liste<RUZ_Layer>* t_lst = event.HoleLayerListe();
    for(RUZ_Layer* t_layer = t_lst->GetErstesElement(); t_layer; t_layer = t_lst->GetNaechstesElement())
    {
        m_skalierListe->Hinzufuegen(t_layer);
    }
    SetStatusText(wxT("Festpunkt der Skalierung wählen"), 2);
    KoordinatenMaske->Show();
    return;
}

void RUZmBIFrame::FangePE(aruDblEvent& event)
{
    int tempID = event.GetId();
    double Wert = event.HoleWert();

    if(tempID == IDgefaelleRasterGroesse)
    {
        m_gefaelleRaster = Wert;
        Refresh();
        return;
    }
    if(tempID == IDflaechenRasterGroesse)
    {
        m_flaechenRaster = Wert;
        Refresh();
        return;
    }
    if(tempID == IDanzeigeGenauigkeit)
    {
        m_anzeigeGenauigkeit = int(Wert);
        Refresh();
        return;
    }
    if(tempID == IDpseudoSchattenFkt)
    {
        m_pseudoSchattenFkt = Wert;
        if(m_pseudoSchattenFkt < 0)m_pseudoSchattenFkt = 0.0;
        Refresh();
        return;
    }
    event.Skip();
    return;
}

void RUZmBIFrame::FangeRadius(aruDblEvent& event)
{
    int tempID = event.GetId();
    double t_radius = event.HoleWert();

    KreisDoubleEingabe(t_radius, (tempID == idRadius_perm));
    Refresh();

    return;
}

void RUZmBIFrame::FangeSonnenstand(aruVektorEvent &event)
{
    m_sonnenRichtung = event.HoleKoordinaten();
    Refresh(false);
    return;
}

void RUZmBIFrame::FangpunkteFinden(Liste<RUZ_Objekt>* m_objLst)
{
    RUZ_Objekt *objEins, *objZwei;
    if(!aktLayer)
    {
        SetStatusText("Kein aktueller Layer! Finden der Fangpunkte abgebrochen", 2);
        return;
    }
    for(Listenelement<RUZ_Objekt>* LE_obj_a = m_objLst->GetErstesListenelement(); LE_obj_a; LE_obj_a = LE_obj_a->GetNachfolger())
    {
        objEins = LE_obj_a->GetElement();
        for(Listenelement<RUZ_Objekt>* LE_obj_b = LE_obj_a->GetNachfolger(); LE_obj_b; LE_obj_b = LE_obj_b->GetNachfolger())
        {
            objZwei = LE_obj_b->GetElement();
            FangpunkteFinden(objEins, objZwei);
        }
    }
    return;
}

void RUZmBIFrame::FangpunkteFinden(RUZ_Objekt *objEins, RUZ_Objekt *objZwei)
{
    if(objEins == objZwei)return;
    Linie *ln_a, *ln_b;
    Kreis *kr_a, *kr_b;
    if(!aktLayer)
    {
        SetStatusText("Kein aktueller Layer! Finden der Fangpunkte abgebrochen", 2);
        return;
    }
    if(objEins->HoleTyp() == RUZ_Linie)
    {
        ln_a = static_cast<Linie*>(objEins);
        if(objZwei->HoleTyp() == RUZ_Linie)
        {
            ln_b = static_cast<Linie*>(objZwei);
            Vektor vkt;
            if(ln_a->schneidet(ln_b, vkt, aktProjZ, true))
            {
                Fangpunkt* fngPkt = new Fangpunkt(vkt, aktLayer);
                if(!fngPkt)logSchreiben("Fehler bei Speicheranforderung\n");
            }
        }else
        if(objZwei->HoleTyp() == RUZ_Kreis)
        {
            kr_b = static_cast<Kreis*>(objZwei);
            if(kr_b->FindeSchnittpunkte(ln_a, aktProjZ))
            {
                Liste<Vektor>* vktLst = kr_b->HoleFangpunkte();
                for(Vektor* vkt = vktLst->GetErstesElement(); vkt; vkt = vktLst->GetNaechstesElement())
                {
                    Fangpunkt* fngPkt = new Fangpunkt(*vkt, aktLayer);
                    if(!fngPkt)logSchreiben("Fehler bei Speicheranforderung\n");
                }
                kr_b->LoescheFangpunkte();
            }else
            {
                logSchreiben("keinSchnittpunkt Kreis mit Linie\n");
            }
        }
    }else
    if(objEins->HoleTyp() == RUZ_Kreis)
    {
        kr_a = static_cast<Kreis*>(objEins);
        if(objZwei->HoleTyp() == RUZ_Linie)
        {
            ln_b = static_cast<Linie*>(objZwei);
            if(kr_a->FindeSchnittpunkte(ln_b, aktProjZ))
            {
                Liste<Vektor>* vktLst = kr_a->HoleFangpunkte();
                for(Vektor *vkt = vktLst->GetErstesElement(); vkt; vkt = vktLst->GetNaechstesElement())
                {
                    Fangpunkt* fngPkt = new Fangpunkt(*vkt, aktLayer);
                    if(!fngPkt)logSchreiben("Fehler bei Speicheranforderung\n");
                }
                kr_a->LoescheFangpunkte();
            }
        }
        if(objZwei->HoleTyp() == RUZ_Kreis)
        {
            kr_b = static_cast<Kreis*>(objZwei);
            if(kr_a->FindeSchnittpunkte(kr_b, aktProjZ))
            {
                Liste<Vektor>* vktLst = kr_a->HoleFangpunkte();
                for(Vektor *vkt = vktLst->GetErstesElement(); vkt; vkt = vktLst->GetNaechstesElement())
                {
                    Fangpunkt* fngPkt = new Fangpunkt(*vkt, aktLayer);
                    logSchreiben("Fangpunkt erstellt: %p\n", fngPkt);
                }
                kr_a->LoescheFangpunkte();
            }
        }
    }
    return;
}

void RUZmBIFrame::FangpunkteLoeschen(wxCommandEvent& event)
{
    /*evtl. noch Layerauswahldialog???*/
    AuswahlLeeren();
    for(RUZ_Layer* tempLayer = m_layer->GetErstesElement(); tempLayer; tempLayer = m_layer->GetNaechstesElement())
        tempLayer->LoescheFangpunkte();
    return;
}

void RUZmBIFrame::GefaelleVerfolgen(Vektor vStart)
{
    if(!aktLayer)return;

    Vektor vAktOrt = vStart;
    Vektor vNaeOrt, vGefaelle;
    Flaeche *aktFl, *letzteFl;
    double dLaenge;

    if(markiertesObjekt)
    {
        if((markiertesObjekt->HoleTyp() == RUZ_Dreieck)||(markiertesObjekt->HoleTyp() == RUZ_Viereck))
        {
            aktFl = static_cast<Flaeche*>(markiertesObjekt);
            if(aktFl->Gefaelle(vAktOrt, vGefaelle, aktProjZ))
            {
                dLaenge = vGefaelle.Laenge();
                if(dLaenge)
                {
                    vNaeOrt = vAktOrt + vGefaelle * (suchRadius / dLaenge);
                }else
                {
                    return;
                }
            }else
            {
                return;
            }
            int j = 0;
            while((j < 100000)&&(aktFl))
            {
                if(aktFl->Gefaelle(vNaeOrt, vGefaelle, aktProjZ))
                {
                    new Strich(vAktOrt.GetKoordinaten(aktProjX), vAktOrt.GetKoordinaten(aktProjY),
                               vNaeOrt.GetKoordinaten(aktProjX), vNaeOrt.GetKoordinaten(aktProjY), aktLayer);
                    vAktOrt = vNaeOrt;
                    dLaenge = vGefaelle.Laenge();
                    if(dLaenge)
                    {
                        vNaeOrt = vAktOrt + vGefaelle * (suchRadius / dLaenge);
                    }else
                    {
                        return;
                    }
                }else
                {
                    int maxI;
                    aktFl->HoleTyp() == RUZ_Dreieck ? maxI = 3 : maxI = 4;
                    for(int i = 0; i < maxI; i++)
                    {
                        Linie* aktLn = aktFl->HoleLinie(i);
                        if(aktLn->schneidet(vAktOrt.GetKoordinaten(aktProjX), vAktOrt.GetKoordinaten(aktProjY),
                                            vNaeOrt.GetKoordinaten(aktProjX), vNaeOrt.GetKoordinaten(aktProjY), vNaeOrt, aktProjZ))
                        {
                            new Strich(vAktOrt.GetKoordinaten(aktProjX), vAktOrt.GetKoordinaten(aktProjY),
                                       vNaeOrt.GetKoordinaten(aktProjX), vNaeOrt.GetKoordinaten(aktProjY), aktLayer);
                            break;
                        }
                    }
                    letzteFl = aktFl;
                    aktFl = NULL;
                    Liste<Flaeche> *lstFl = aktLayer->HoleFlaechen();
                    for(Flaeche *flLaeufer = lstFl->GetErstesElement(); flLaeufer; flLaeufer = lstFl->GetNaechstesElement())
                    {
                        if(flLaeufer == letzteFl)continue;
                        if(flLaeufer->IstInnerhalb(vNaeOrt.GetKoordinaten(aktProjX), vNaeOrt.GetKoordinaten(aktProjY), aktProjZ))
                        {
                            aktFl = flLaeufer;
                            if(aktFl->Gefaelle(vNaeOrt, vGefaelle, aktProjZ))
                            {
                                dLaenge = vGefaelle.Laenge();
                                Vektor vTempOrt;
                                if(dLaenge)
                                {
                                    vTempOrt = vNaeOrt + vGefaelle * (suchRadius / dLaenge);
                                    if(!(aktFl->IstInnerhalb(vTempOrt.GetKoordinaten(aktProjX), vTempOrt.GetKoordinaten(aktProjY), aktProjZ)))return;
                                }else
                                {
                                    return;
                                }
                            }
                            break;
                        }
                    }
                }
                j++;
            }
        }
    }
    return;
}

void RUZmBIFrame::HoehenkarteZeichnen(void)
{
    clock_t tLaufzeit;
    if(aktLayer)
    {
        double minX, minY, maxX, maxY, minZ, maxZ;
        if(aktLayer->AusdehnungFinden(minX, minY, maxX, maxY, minZ, maxZ))
        {
            double* dIntegral = NULL;
            aruIntegral tempIntegral(dIntegral, minX, minY, maxX, maxY, m_flaechenRaster, aktProjZ);
            dIntegral = tempIntegral.HoleIntegral();
            if(!dIntegral)
            {
                wxMessageDialog(this, wxT("Fehler beim Anlegen des Integrals\n(evtl. zu wenig Speicher?)"), wxT("Abbruch der Berechnung")).ShowModal();
                return;
            }
            Liste<Flaeche>* lstFl = aktLayer->HoleFlaechen();
            tLaufzeit = clock();
            int iAktFlaeche = 0;
            SetStatusText(wxT("Starte Integration"), 1);
            Refresh();
            for(Flaeche* aktFl = lstFl->GetErstesElement(); aktFl != NULL; aktFl = lstFl->GetNaechstesElement())
            {
                if(aktFl->HoleTyp() == RUZ_Dreieck)
                {
                    Vektor vNormale = aktFl->HoleNormale();
                    Vektor vSenkrechte(0, 0, 0);
                    vSenkrechte.SetKoordinaten(aktProjZ, 1.0);
                    if(vNormale*vSenkrechte < 0.05)continue;//0.05 = ca. cos(87°) - fast senkrechte Flächen überspringen (ergibt an den Rändern unbrauchbar hohe Werte)
                }
                tempIntegral.IntegriereFlaeche(aktFl);
            }

            double maxWert, minWert;
            unsigned char aktWert;
            int iB = tempIntegral.HoleBreite();
            int iH = tempIntegral.HoleHoehe();

            maxWert = minWert = 0.0;
            bool nochNAN = true;

            SetStatusText(wxT("Integration erledigt - stopfe Löcher"), 1);
            Refresh();
            for(int i = 0; i < iB; i++)
            {
                for(int k = 0; k < iH; k++)
                {
                    if(!isnan(dIntegral[i+k*iB]))
                    {

                        if(nochNAN)
                        {
                            maxWert = minWert = dIntegral[i+k*iB];
                            nochNAN = false;
                        }
                        if(maxWert < dIntegral[i+k*iB])maxWert = dIntegral[i+k*iB];
                        if(minWert > dIntegral[i+k*iB])minWert = dIntegral[i+k*iB];
                    }else
                    {
                        //Löcher stopfen
                        if((i>0)&&(k>0)&&(i<iB-1)&&(k<iH-1))
                        {
                            int iNachbarn = 0;
                            double dSumme = 0;
                            for(int di = -1; di < 2; di++)
                                for(int dk = -1; dk < 2; dk++)
                                {
                                    if(!isnan(dIntegral[i+di+(k+dk)*iB]))
                                    {
                                        iNachbarn++;
                                        dSumme += dIntegral[i+di+(k+dk)*iB];
                                    }
                                }
                            if(iNachbarn > 5)//Loch ist (vermutlich) in Fläche und nicht am Rand
                            {
                                dIntegral[i+k*iB] = dSumme/iNachbarn;
                            }
                        }
                    }
                }
            }
            lwBild.NeueLeinwand(iB, iH, 1/m_flaechenRaster, minX, minY);
            double dSchrittzahl = (maxWert - minWert)/hoehenSchritt;
            if(!dSchrittzahl)return;
            SetStatusText(wxT("Löcher stopfen erledigt - bemale Leinwand"), 1);
            Refresh();
            for(int i = 0; i < (iB * iH); i++)
            {
                if(isnan(dIntegral[i]))
                {
                    lwBild.ucLeinwand[i*3] = col_ZeichenHintergrund.Red();
                    lwBild.ucLeinwand[i*3+1] = col_ZeichenHintergrund.Green();
                    lwBild.ucLeinwand[i*3+2] = col_ZeichenHintergrund.Blue();
                }else if(dIntegral[i] > maxZ)
                {
                    lwBild.ucLeinwand[i*3] = 255;
                    lwBild.ucLeinwand[i*3+1] = 78;
                    lwBild.ucLeinwand[i*3+2] = 46;
                }
                else if(dIntegral[i] < minZ)
                {
                    lwBild.ucLeinwand[i*3] = 78;
                    lwBild.ucLeinwand[i*3+1] = 46;
                    lwBild.ucLeinwand[i*3+2] = 255;
                }else
                {
                    aktWert = (unsigned char)((dIntegral[i]-minWert)/hoehenSchritt)*(unsigned char)(max(256 / dSchrittzahl, 1.0));
                    lwBild.ucLeinwand[i*3] = aktWert;
                    lwBild.ucLeinwand[i*3+1] = aktWert;
                    lwBild.ucLeinwand[i*3+2] = aktWert;
                }
            }
            SetStatusText(wxString::Format(wxT("min Z: %f | max Z: %f"), minZ, maxZ),1);
        }
    }
    return;
}

RUZ_Layer* RUZmBIFrame::HoleAktuellenLayer(void) const
{
    return aktLayer;
}

void RUZmBIFrame::Kop_Abbrechen(void)
{
    if(m_markierModus)
    {
        if(m_auswahl->GetListenGroesse() != 0)
        {
            m_auswahl->ListeLeeren("");
        }else
        {
            BefehleZuruecksetzen();
        }
        return;
    }
    if(vVerschubStart)
    {
        Kop_Verschieben(*vVerschubStart);
        delete vVerschubStart;
        vVerschubStart = NULL;
        SetStatusText(wxT("Kopierte Objekte verschieben: 'Von' (Punkt wählen / eingeben)"), 2);
        return;
    }
    m_markierModus = true;
    KoordinatenMaske->Show(false);
    for(RUZ_Objekt* obj = m_kopierAuswahl->GetErstesElement(); obj; obj = m_kopierAuswahl->GetNaechstesElement())
    {
        if((obj->HoleTyp() == RUZ_Linie)||(obj->HoleTyp() == RUZ_Dreieck)||(obj->HoleTyp() == RUZ_Viereck))
        {
            m_kopierAuswahl->Entfernen(obj);
        }
    }
    m_kopierAuswahl->ListeLoeschen("");
    m_verschubAuswahlOrte->ListeLeeren("");
    SetStatusText(wxT("Objekte zum Verschieben auswählen"), 2);
    return;
}

void RUZmBIFrame::Kop_Auswahl_Bestaetigung(void)
{
    if(m_markierModus)
    {
        m_markierModus = false;
        AuswahlKopieren();
        Kop_Punktspeicher_Schreiben();
        KoordinatenMaske->Show();
        SetStatusText(wxT("Kopierte Objekte verschieben: 'Von' (Punkt wählen / eingeben)"), 2);
    }else
    {
        m_markierModus = true;
        KoordinatenMaske->Show(false);
        m_verschubAuswahlOrte->ListeLoeschen("");
        m_auswahl->ListeLeeren("");
        if(aktBefehl == bef_ID_kopieren)
        {
            for(RUZ_Objekt* obj = m_kopierAuswahl->GetErstesElement(); obj; obj = m_kopierAuswahl->GetNaechstesElement())
                m_auswahl->ExklusivHinzufuegen(obj);
        }
        m_kopierAuswahl->ListeLeeren("");
        SetStatusText(wxT("Objekte zum Kopieren auswählen"), 2);
        if(aktBefehl == bef_ID_kopierenNachLayer)
        {
            BefehleZuruecksetzen();
        }
    }
    return;
}

void RUZmBIFrame::Kop_Punkt_Festlegen(Vektor vkt)
{
    if(vVerschubStart)
    {
        Vschb_Verschieben(vkt);
        delete vVerschubStart;
        vVerschubStart = NULL;
        SetStatusText(wxT("Kopierte Objekte verschieben: 'Von' (Punkt wählen / eingeben)"), 2);
    }else
    {
        vVerschubStart = new Vektor(vkt);
        if(vVerschubStart)
            SetStatusText(wxT("Kopierte Objekte verschieben: 'Nach' (Punkt wählen / eingeben)"), 2);
    }
    return;
}

void RUZmBIFrame::Kop_Punktspeicher_Schreiben(void)
{
    m_verschubAuswahlOrte->ListeLoeschen("Kop_Punktspeicher_Schreiben");
    PunktSpeicher* pktSp = NULL;

    for(RUZ_Objekt* obj = m_kopierAuswahl->GetErstesElement(); obj != NULL; obj = m_kopierAuswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            pktSp = new PunktSpeicher(static_cast<Punkt*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* ln = static_cast<Linie*>(obj);
            for (int i = 0; i < 2; i++)
            {
                pktSp = new PunktSpeicher(ln->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_Dreieck)
        {
            Dreieck* drk = static_cast<Dreieck*>(obj);
            for (int i = 0; i < 3; i++)
            {
                pktSp = new PunktSpeicher(drk->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_Viereck)
        {
            Viereck* vrk = static_cast<Viereck*>(obj);
            for (int i = 0; i < 4; i++)
            {
                pktSp = new PunktSpeicher(vrk->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            pktSp = new PunktSpeicher(static_cast<HoehenMarke*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Kreis)
        {
            pktSp = new PunktSpeicher(static_cast<Kreis*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Fangpunkt)
        {
            pktSp = new PunktSpeicher(static_cast<Fangpunkt*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        {
            m_kopierAuswahl->Entfernen(obj);
        }
    }
    return;
}

void RUZmBIFrame::Kop_Verschieben(Vektor vkt)
{
    Vschb_Verschieben(vkt);
    return;
}

void RUZmBIFrame::KreisDoubleEingabe(double dbl, bool abschliessen)
{
    if(m_aktKreis)
    {
        m_aktKreis->SetzeRadius(dbl);
        if(abschliessen)
        {
            m_aktKreis = NULL;
            DoubleEingabe->Show(false);
            if(aktBefehl == bef_ID_kreisRadiusAendern)aktBefehl = bef_ID_nichts;
        }
    }
    return;
}

void RUZmBIFrame::KreisPunktEingabe(Vektor& t_vkt, bool abschliessen)
{
    if(m_aktKreis)
    {
        Vektor t_mitte = m_aktKreis->HolePosition();
        t_mitte.SetKoordinaten(aktProjZ, 0.0);
        double dx, dy;
        dx = t_mitte.GetKoordinaten(aktProjX) - t_vkt.GetKoordinaten(aktProjX);
        dy = t_mitte.GetKoordinaten(aktProjY) - t_vkt.GetKoordinaten(aktProjY);
        double t_radius = sqrt(dx * dx + dy * dy);
        KreisDoubleEingabe(t_radius, abschliessen);
    }else{
        Kreis *tempKreis = new Kreis(t_vkt, -1.0, aktLayer);
        if(tempKreis)m_aktKreis = tempKreis;
        DoubleEingabe->ErscheinungAnpassen(wxString("Radius"), wxEmptyString, aktBefehl);
        DoubleEingabe->Show(true);
    }
    return;
}

void RUZmBIFrame::LayerauswahlAktualisieren(void)
{
    for(Listenelement<RUZ_Layer>* layerLELaeufer = m_layer->GetErstesListenelement(); layerLELaeufer != NULL; layerLELaeufer = layerLELaeufer->GetNachfolger())
    {
        const char* name = (layerLELaeufer->GetElement())->HoleName();
        LayerAuswahl->LayerHinzufuegen(wxString(name), layerLELaeufer->GetElement());
    }
    return;
}

void RUZmBIFrame::LayerEntfernen(RUZ_Layer *layer)
{
    if(layer)
    {
        BefehleZuruecksetzen();
        m_layer->Entfernen(layer);
        if(layer == aktLayer)
        {
            aktLayer = m_layer->GetErstesElement();
            LayerauswahlAktualisieren();
        }
    }else
    {
        wxMessageDialog(this, wxT("Layer NICHT gefunden")).ShowModal();
    }
    return;
}

void RUZmBIFrame::LayerHinzufuegen(RUZ_Layer *layer)
{
    if(layer)
    {
        BefehleZuruecksetzen();
        aktLayer = layer;
        m_layer->Hinzufuegen(layer);
    }
    return;
}

RUZ_Layer* RUZmBIFrame::LayerKopieren(RUZ_Layer* altLayer, char* name)
{
    if(altLayer == NULL)return NULL;
    RUZ_Layer* neuLayer = altLayer->Kopieren(name);
    if(neuLayer == NULL)
    {
        wxMessageDialog(this, wxT("Anlegen neuer Layer fehlgeschlagen."), wxT("Layer kopieren")).ShowModal();
        return NULL;
    }

    m_layer->Hinzufuegen(neuLayer);
    LayerauswahlAktualisieren();
    return neuLayer;
}

bool RUZmBIFrame::LayerLoeschen(wxString msg)
{
    return true;
}

void RUZmBIFrame::LayerMalen(wxDC &dc, RUZ_Layer* tempLayer)
{
	if(tempLayer->IstSichtbar() == false)return;
	
    wxColor loc_col_Pkt_Ln, loc_col_HoehenMarke, loc_col_Hoehenlinie, loc_col_Strich, loc_col_Fangpunkt;
	unsigned char r, g, b;Punkt* tempPunkt;
    Linie* tempLinie;
    Strich* tempStrich;
    Flaeche* tempFlaeche;
    HoehenMarke* tempHM;
    Kreis* tempKreis;
    Liste<Strich>* strSammlung;
    Fangpunkt* tempFangpunkt;
    int anzEcken;
	
	int nFntSize = dc.GetFont().GetPointSize();
	
	if(tempLayer == aktLayer)
	{
		loc_col_Pkt_Ln = col_Pkt_Ln;
		loc_col_Strich = col_Strich;
		loc_col_HoehenMarke = col_HoehenMarke;
		loc_col_Hoehenlinie = col_Hoehenlinie;
		loc_col_Fangpunkt = col_Fangpunkt;
	}else
	{
		float delta = 0.5;
		r = (1 - delta) * col_ZeichenHintergrund.Red() + delta * col_Pkt_Ln.Red();
		g = (1 - delta) * col_ZeichenHintergrund.Green() + delta * col_Pkt_Ln.Green();
		b = (1 - delta) * col_ZeichenHintergrund.Blue() + delta * col_Pkt_Ln.Blue();
		loc_col_Pkt_Ln = wxColor(r, g, b);

		r = (1 - delta) * col_ZeichenHintergrund.Red() + delta * col_Strich.Red();
		g = (1 - delta) * col_ZeichenHintergrund.Green() + delta * col_Strich.Green();
		b = (1 - delta) * col_ZeichenHintergrund.Blue() + delta * col_Strich.Blue();
		loc_col_Strich = wxColor(r, g, b);

		r = (1 - delta) * col_ZeichenHintergrund.Red() + delta * col_HoehenMarke.Red();
		g = (1 - delta) * col_ZeichenHintergrund.Green() + delta * col_HoehenMarke.Green();
		b = (1 - delta) * col_ZeichenHintergrund.Blue() + delta * col_HoehenMarke.Blue();
		loc_col_HoehenMarke = wxColor(r, g, b);

		r = (1 - delta) * col_ZeichenHintergrund.Red() + delta * col_Hoehenlinie.Red();
		g = (1 - delta) * col_ZeichenHintergrund.Green() + delta * col_Hoehenlinie.Green();
		b = (1 - delta) * col_ZeichenHintergrund.Blue() + delta * col_Hoehenlinie.Blue();
		loc_col_Hoehenlinie = wxColor(r, g, b);

		r = (1 - delta) * col_ZeichenHintergrund.Red() + delta * col_Fangpunkt.Red();
		g = (1 - delta) * col_ZeichenHintergrund.Green() + delta * col_Fangpunkt.Green();
		b = (1 - delta) * col_ZeichenHintergrund.Blue() + delta * col_Fangpunkt.Blue();
		loc_col_Fangpunkt = wxColor(r, g, b);
	}

	Liste<Punkt>* pktSammlung = tempLayer->HolePunkte();
	Liste<Linie>* lnSammlung = tempLayer->HoleLinien();
    strSammlung = tempLayer->HoleStriche();
	Liste<Flaeche>* flSammlung = tempLayer->HoleFlaechen();
	Liste<HoehenMarke>* hmSammlung = tempLayer->HoleHoehenMarken();
	Liste<Kreis>* krSammlung = tempLayer->HoleKreise();
	Liste<Fangpunkt>* fngPktSammlung = tempLayer->HoleFangpunkte();

	if(m_zeigeFlaeche && hlAnzeigen)
	{
		for(tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
		{
			dc.SetPen(wxPen(loc_col_Hoehenlinie, 1));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			Liste<RUZ_Hoehenlinie>* hlListe = tempFlaeche->HoleHL();
			for(RUZ_Hoehenlinie* aktHL = hlListe->GetErstesElement(); aktHL != NULL; aktHL = hlListe->GetNaechstesElement())
			{
				dc.DrawLine((aktHL->x(0)-dc_Offset[0])*m_skalierung, (aktHL->y(0)-dc_Offset[1])*m_skalierung,
					(aktHL->x(1)-dc_Offset[0])*m_skalierung, (aktHL->y(1)-dc_Offset[1])*m_skalierung);
			}
		}
	}
	if(m_zeigePunkt)
	{
		for(tempPunkt = pktSammlung->GetErstesElement(); tempPunkt != NULL; tempPunkt = pktSammlung->GetNaechstesElement())
		{
			if(tempPunkt == markiertesObjekt)
			{
				continue;
			}else{
				dc.SetPen(wxPen(loc_col_Pkt_Ln, 1));
				dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
			}
			dc.DrawCircle((tempPunkt->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
					  (tempPunkt->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, pxSuchEntfernung);
			if(m_zeigeHoehe)
			{
				dc.SetTextForeground(loc_col_Pkt_Ln);
				wxString genauigkeit = wxString::Format("%1.%df", m_anzeigeGenauigkeit);
				dc.DrawText(wxString::Format(genauigkeit, tempPunkt->HolePosition().GetKoordinaten(aktProjZ)),
							(tempPunkt->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung + 1.2f * pxSuchEntfernung,
							(tempPunkt->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung - nFntSize);
			}
		}
	}
	if(m_zeigeLinie)
	{
		for(tempLinie = lnSammlung->GetErstesElement(); tempLinie != NULL; tempLinie = lnSammlung->GetNaechstesElement())
		{
			if(tempLinie == markiertesObjekt)
			{
				continue;
			}else
			{
				int penDicke;
				if(tempLayer != aktLayer)
				{
					penDicke = 1;
				}
				else if(tempLinie->HoleFlaechen()->GetListenGroesse() < 2)
				{
					penDicke = 2;
				}else
				{
					penDicke = 1;
				}
				dc.SetPen(wxPen(loc_col_Pkt_Ln, penDicke));
				dc.SetBrush(wxBrush(loc_col_Pkt_Ln));
			}
			dc.DrawLine((tempLinie->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (tempLinie->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
						(tempLinie->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (tempLinie->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
		}

	}
	dc.SetPen(wxPen(loc_col_Strich, 1));
	dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
	if(m_zeigeKreis)
	{
		for(tempKreis = krSammlung->GetErstesElement(); tempKreis != NULL; tempKreis = krSammlung->GetNaechstesElement())
		{
			dc.DrawCircle((tempKreis->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
						  (tempKreis->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
						  (int)(tempKreis->HoleRadius()*m_skalierung));
		}
	}
	dc.SetBrush(wxBrush(loc_col_Strich));
	if(m_zeigeStrich)
	{
		for(tempStrich = strSammlung->GetErstesElement(); tempStrich != NULL; tempStrich = strSammlung->GetNaechstesElement())
		{
			dc.DrawLine((tempStrich->Xa()-dc_Offset[0])*m_skalierung, (tempStrich->Ya()-dc_Offset[1])*m_skalierung,
						(tempStrich->Xe()-dc_Offset[0])*m_skalierung, (tempStrich->Ye()-dc_Offset[1])*m_skalierung);
		}
	}
	if(m_zeigeHoehenmarke)
	{
		for(tempHM = hmSammlung->GetErstesElement(); tempHM != NULL; tempHM = hmSammlung->GetNaechstesElement())
		{
			if(tempHM == markiertesObjekt)
			{
				continue;
			}else{
				dc.SetPen(wxPen(loc_col_HoehenMarke, 1));
				dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
			}
			dc.DrawCircle((tempHM->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
					  (tempHM->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, (int)(pxSuchEntfernung/3));
			dc.SetTextForeground(loc_col_HoehenMarke);
			if(tempHM->IstInFlaeche())
			{
				wxString genauigkeit = wxString::Format("%1.%df", m_anzeigeGenauigkeit);
				dc.DrawText(wxString::Format(genauigkeit, std::nearbyint(tempHM->HolePosition().GetKoordinaten(aktProjZ) * 100000) / 100000.0),
							(tempHM->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung + 1.2f * (pxSuchEntfernung/3),
							(tempHM->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung - nFntSize);
			}else{
				dc.DrawText(wxT("#NiF"),
							(tempHM->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung + 1.2f * (pxSuchEntfernung/3),
							(tempHM->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung - nFntSize);
			}
		}
	}
	if(m_zeigeFangpunkt)
	{
		dc.SetPen(wxPen(loc_col_Fangpunkt, 1));
		int posX, posY;
		for(tempFangpunkt = fngPktSammlung->GetErstesElement(); tempFangpunkt; tempFangpunkt = fngPktSammlung->GetNaechstesElement())
		{
			posX = (tempFangpunkt->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
			posY = (tempFangpunkt->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
			int symbolGroesse = 1.2f * pxSuchEntfernung;
			dc.DrawLine(posX - symbolGroesse, posY - symbolGroesse, posX + symbolGroesse, posY + symbolGroesse);
			dc.DrawLine(posX + symbolGroesse, posY - symbolGroesse, posX - symbolGroesse, posY + symbolGroesse);
		}
	}
	return;
}

void RUZmBIFrame::LayerSkalieren(Vektor festPkt)
{
    for(RUZ_Layer* t_layer = m_skalierListe->GetErstesElement(); t_layer; t_layer = m_skalierListe->GetNaechstesElement())
    {
        Liste<Punkt>* pktLst = t_layer->HolePunkte();
        for(Punkt* t_pkt = pktLst->GetErstesElement(); t_pkt; t_pkt = pktLst->GetNaechstesElement())
        {
            t_pkt->Skalieren(festPkt, m_skalFkt[0], m_skalFkt[1], m_skalFkt[2]);
        }

        Liste<Strich>* strLst = t_layer->HoleStriche();
        for(Strich* t_str = strLst->GetErstesElement(); t_str; t_str = strLst->GetNaechstesElement())
        {
            t_str->Skalieren(festPkt, m_skalFkt[0], m_skalFkt[1], m_skalFkt[2]);
        }

        Liste<Bogen>* bgnLst = t_layer->HoleBoegen();
        for(Bogen* t_bgn = bgnLst->GetErstesElement(); t_bgn; t_bgn = bgnLst->GetNaechstesElement())
        {
            t_bgn->Skalieren(festPkt, m_skalFkt[0], m_skalFkt[1], m_skalFkt[2]);
        }
    }
    m_skalierListe->ListeLeeren("");
    BefehleZuruecksetzen();
    return;
}

bool RUZmBIFrame::LeseAusD45(char* dateiName)
{
    AuswahlLeeren();

    std::ifstream Datei;
    Datei.open(dateiName, ios_base::in);
    logSchreiben("%s konnte geöffnet werden (D45)\n", dateiName);

    char zeile[11];
    char x[11], y[11], z[11], pktName[8];
    Punkt* tempPkt;
    /*alles Nullen*/
    x[0] = y[0] = z[0] = '0';
    x[1] = y[1] = z[1] = '\0';
    pktName[0] = '\0';
    /*ENDE alles Nullen*/

    if(Datei.good())
    {
        while(!Datei.eof())
        {
            Datei.ignore(2);/*Eintrag 45 aus dem stream entfernen*/

            /*Punktname lesen*/
            Datei.get(zeile, 8);
            strncpy(pktName, zeile, 8);
            pktName[7] = '\0';

            Datei.ignore(1);/*Leerzeichen überspringen*/

            /*Y-Koordinate lesen*/
            Datei.get(zeile, 11);
            strncpy(x, zeile, 11);
            //y[11] = '\0';

            /*X-Koordinate lesen*/
            Datei.get(zeile, 11);
            strncpy(y, zeile, 11);
            //x[11] = '\0';

            /*Z-Koordinate lesen*/
            Datei.get(zeile, 11);
            strncpy(z, zeile, 11);
            //z[11] = '\0';

            while(Datei.peek()=='\n')Datei.ignore();

            if(aktLayer!=NULL)
            {
                tempPkt = new Punkt((double)(atof(x))/1000, -(double)(atof(y))/1000, (double)(atof(z))/1000, aktLayer);
                tempPkt->SetzeName(pktName);
            }
            /*int ergebnis = wxMessageDialog(this, wxT(".")+wxString(pktName)+wxT(".")+wxT("\n")+wxT(".")+wxString(x)+wxT(".")
                                           +wxT("\n")+wxT(".")+wxString(y)+wxT(".")+wxT("\n")+wxT(".")+wxString(z)+wxT("."), wxT("aktPkt"), wxOK|wxCANCEL).ShowModal();
            if(ergebnis == wxID_CANCEL) return 0;*/
        }
    }
    return 1;
}


bool RUZmBIFrame::LeseAusD58(char* dateiName)
{
    AuswahlLeeren();
    int fehlendeDreiecke = 0;

    std::ifstream Datei;
    Datei.open(dateiName, ios_base::in);
    logSchreiben("%s konnte geöffnet werden (D58)\n", dateiName);

    char zeile[64];
    char p1[8], p2[8], p3[8];
    /*alles Nullen*/
    p1[0] = p2[0] = p3[0] = '0';
    p1[1] = p2[1] = p3[1] = '\0';
    /*ENDE alles Nullen*/

    if(Datei.good())
    {
        while(!Datei.eof())
        {
            Datei.ignore(23);/*Eintrag 58 bis Dreieckname aus dem stream entfernen*/

            /*Punkt 1 lesen*/
            Datei.get(zeile, 8);
            strncpy(p1, zeile, 8);

            Datei.ignore(3);/*Drei Zeichen überspringen (in D45 sind Punktnamen nur 7 lang!!)*/

            /*Punkt 2 lesen*/
            Datei.get(zeile, 8);
            strncpy(p2, zeile, 8);

            Datei.ignore(3);/*Drei Zeichen überspringen (in D45 sind Punktnamen nur 7 lang!!)*/

            /*Punkt 3 lesen*/
            Datei.get(zeile, 8);
            strncpy(p3, zeile, 8);

            Datei.getline(zeile, 64, '\n');
            while(Datei.peek()=='\n')Datei.ignore();

            if(aktLayer != NULL)
            {
                Liste<Punkt> *pktLst = aktLayer->HolePunkte();
                Punkt *eckPunkt[3];
                eckPunkt[0] = eckPunkt[1] = eckPunkt[2] = NULL;
                int i = 0;
                for(Punkt* aktPkt = pktLst->GetErstesElement(); aktPkt != NULL; aktPkt = pktLst->GetNaechstesElement())
                {
                    if((strcmp(aktPkt->HoleName(), p1) == 0)||
                       (strcmp(aktPkt->HoleName(), p2) == 0)||
                       (strcmp(aktPkt->HoleName(), p3) == 0))
                    {
                        eckPunkt[i] = aktPkt;
                        i++;
                    }
                    if(i>2)break;
                }
                Linie *t_rand[3];
                t_rand[0] = t_rand[1] = t_rand[2] = NULL;
                for(int k = 0; k < 3; k++)/*Prüfen, ob alle Punkte gefunden wurden*/
                {
                    if(eckPunkt[k] == NULL)
                    {
                        fehlendeDreiecke++;
                        continue;
                    }
                }
                t_rand[0] = aktLayer->HoleLinie(eckPunkt[0], eckPunkt[1]);
                t_rand[1] = aktLayer->HoleLinie(eckPunkt[1], eckPunkt[2]);
                t_rand[2] = aktLayer->HoleLinie(eckPunkt[2], eckPunkt[0]);
                if(t_rand[0] && t_rand[1] && t_rand[2])
                {
                    Dreieck::NeuesDreieck(t_rand[0], t_rand[1], t_rand[2]);
                }
            }

            /*int ergebnis = wxMessageDialog(this, wxT(".")+wxString(p1)+wxT(".")+wxT("\n")+wxT(".")+wxString(p2)+wxT(".")
                                           +wxT("\n")+wxT(".")+wxString(p3)+wxT("."), wxT("Kontrolle"), wxOK|wxCANCEL).ShowModal();
            if(ergebnis == wxID_CANCEL) return 0;*/
        }
    }
    if(aktLayer != NULL)
    {
        Liste<Punkt> *pktLst = aktLayer->HolePunkte();
        for(Punkt* aktPkt = pktLst->GetErstesElement(); aktPkt != NULL; aktPkt = pktLst->GetNaechstesElement())
        {
            if(aktPkt->HoleLinien()->GetListenGroesse() == 0)delete aktPkt;
        }
    }
    SetStatusText(wxString::Format("%d Dreieck(e) nicht erzeugt", fehlendeDreiecke), 1);
    return 1;
}

bool RUZmBIFrame::LeseAusRUZ(char* dateiName)
{
    AuswahlLeeren();

		std::string strLogEintrag;

    std::ifstream Datei;
    Datei.open(dateiName, ios_base::in);
    if(Datei.good())
    {
        logSchreiben("%s konnte geöffnet werden\n", dateiName);
    }else
    {
        logSchreiben("%s konnte nicht geöffnet werden\n", dateiName);
        return false;
    }

    RUZ_Layer *schreibLayer = NULL;
    bool layerNameVorhanden;
    char zeile[64];
    double wert[6];
    long long int pointer[4];
    Punkt *p[2];
    Linie *l[4];
    /*alles Nullen*/
    wert[0] = wert[1] = wert[2] = wert[3] = wert[4] = wert[5] = 0.0;
    pointer[0] = pointer[1] = pointer[2] = pointer[3] = 0;
    p[0] = p[1] = 0;
    l[0] = l[1] = l[2] = l[3] = 0;
    /*ENDE alles Nullen*/

    if(Datei.good())
    {
        while(!Datei.eof())
        {
            Datei.get(zeile, 64);
            while(Datei.peek()=='\n')Datei.ignore();
            if(strcmp(zeile, "EOF")==0)
            {
                Datei.close();
								if(schreibLayer == NULL)return true;
                Liste<HoehenMarke>* hmSammlung = schreibLayer->HoleHoehenMarken();
                HoehenMarke* tempHM;
                for(tempHM = hmSammlung->GetErstesElement(); tempHM != NULL; tempHM = hmSammlung->GetNaechstesElement())
                {
                    tempHM->Verschieben(Vektor(0, 0, 0));
                }
                return true;
            }
            /*Layer*/
            if(strcmp(zeile, "  L")==0)
            {
                Datei.get(zeile, 64);
                while (Datei.peek()=='\n')Datei.ignore();
                layerNameVorhanden = false;
                for(RUZ_Layer* t_layer = m_layer->GetErstesElement(); t_layer != NULL; t_layer = m_layer->GetNaechstesElement())
                {
                    if(!t_layer)continue;
                    if(strcmp(t_layer->HoleName(), zeile) == 0)
                    {
                        layerNameVorhanden = true;
                        schreibLayer = t_layer;
                        break;
                    }
                }
                if(layerNameVorhanden == false)
                {
                    schreibLayer = new RUZ_Layer(zeile);
                    if(schreibLayer!=NULL)
                    {
                        m_layer->Hinzufuegen(schreibLayer);
                        SetzeAktuellenLayer(0, schreibLayer);
                        LayerAuswahl->LayerHinzufuegen(wxString((const char*) zeile), schreibLayer);
                    }
                }
            }
            /*Hintergrundlayer*/
            if(strcmp(zeile, " HG")==0)
            {
                schreibLayer = m_hintergrundLayer;
            }
            /*Punkt*/
            if(strcmp(zeile, "POINT")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, "  X")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[0] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, "  Y")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[1] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, "  Z")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[2] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, "  @")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[0] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if(pointer[0]!=0)
                        {
                            if(schreibLayer!=NULL)
                            {
                                schreibLayer->Wert(new Punkt(wert[0], wert[1], wert[2], schreibLayer), (double)pointer[0]);
                                wert[0] = wert[1] = wert[2] = wert[3] = wert[4] = wert[5] = 0.0;
                                pointer[0] = pointer[1] = pointer[2] = pointer[3] = 0;
                                break;
                            }
                        }
                    }
                }
            }
            /*HoehenMarke*/
            if(strcmp(zeile, "HOEHENMARKE")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, "  X")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[0] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, "  Y")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[1] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if(schreibLayer!=NULL)
                        {
                            new HoehenMarke(Vektor(wert[0], wert[1], 0), schreibLayer);
                            wert[0] = wert[1] = wert[2] = wert[3] = wert[4] = wert[5] = 0.0;
                            pointer[0] = pointer[1] = pointer[2] = pointer[3] = 0;
                            break;
                        }
                    }
                }
            }
            /*Linien*/
            if(strcmp(zeile, "LINE")==0)
            {
                while(!Datei.eof())
                {
										Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, " P0")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[0] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " P1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[1] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, "  @")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[2] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if((pointer[0]!=0)&&(pointer[1]!=0)&&(pointer[2]!=0))
                        {
                            if(schreibLayer!=NULL)
                            {
                                for(Listenelement<Punkt>* akt_LE_Punkt = schreibLayer->HolePunkte()->GetErstesListenelement(); akt_LE_Punkt != NULL; akt_LE_Punkt = akt_LE_Punkt->GetNachfolger())
                                {
                                    for(int i = 0; i < 2; i++)
                                    {
                                        if(akt_LE_Punkt->Wert() == (double)pointer[i])
                                        {
                                            p[i] = akt_LE_Punkt->GetElement();
                                            break;
                                        }
                                    }
                                    if((p[0]!=0)&&(p[1]!=0))break;
                                }
                                if((p[0]!=0)&&(p[1]!=0))
                                {
                                    Linie *neueLinie = Linie::NeueLinie(p[0], p[1]);
                                    //neueLinie->SetzeGeschuetzt(true);
                                    schreibLayer->Wert(neueLinie, (double)pointer[2]);
                                    pointer[0] = pointer[1] = pointer[2] = 0;
                                    p[0] = p[1] = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            /*Dreieck*/
            if(strcmp(zeile, "TRIANGLE")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, " L0")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[0] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " L1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[1] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " L2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[2] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if((pointer[0]!=0)&&(pointer[1]!=0)&&(pointer[2]!=0))
                        {
                            if(schreibLayer!=NULL)
                            {
                                for(Listenelement<Linie>* akt_LE_Linie = schreibLayer->HoleLinien()->GetErstesListenelement(); akt_LE_Linie != NULL; akt_LE_Linie = akt_LE_Linie->GetNachfolger())
                                {
                                    for(int i = 0; i < 3; i++)
                                    {
                                        if(akt_LE_Linie->Wert() == (double)pointer[i])
                                        {
                                            l[i] = akt_LE_Linie->GetElement();
                                            break;
                                        }
                                    }
                                    if((l[0]!=0)&&(l[1]!=0)&&(l[2]!=0))break;
                                }
                                if((l[0]!=0)&&(l[1]!=0)&&(l[2]!=0))
                                {
                                    Dreieck* testDreieck = Dreieck::NeuesDreieck(l[0], l[1], l[2]);
                                    if(testDreieck != NULL)
                                    {
                                        pointer[0] = pointer[1] = pointer[2] = 0;
                                        l[0] = l[1] = l[2] = 0;
                                        break;
                                    }

                                }
                            }
                        }
                    }
                }
            }
            /*Viereck*/
            if(strcmp(zeile, "QUAD")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, " L0")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[0] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " L1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[1] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " L2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[2] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, " L3")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        pointer[3] = strtoll(zeile, NULL, 16);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if((pointer[0]!=0)&&(pointer[1]!=0)&&(pointer[2]!=0)&&(pointer[3]!=0))
                        {
                            if(schreibLayer!=NULL)
                            {
                                for(Listenelement<Linie>* akt_LE_Linie = schreibLayer->HoleLinien()->GetErstesListenelement(); akt_LE_Linie != NULL; akt_LE_Linie = akt_LE_Linie->GetNachfolger())
                                {
                                    for(int i = 0; i < 4; i++)
                                    {
                                        if(akt_LE_Linie->Wert() == (double)pointer[i])
                                        {
                                            l[i] = akt_LE_Linie->GetElement();
                                            break;
                                        }
                                    }
                                    if((l[0]!=0)&&(l[1]!=0)&&(l[2]!=0)&&(l[3]!=0))break;
                                }
                                if((l[0]!=0)&&(l[1]!=0)&&(l[2]!=0)&&(l[3]!=0))
                                {
                                    Viereck* testViereck = Viereck::NeuesViereck(l[0], l[1], l[2], l[3]);
                                    if(testViereck != NULL)
                                    {
                                        pointer[0] = pointer[1] = pointer[2] = pointer[3] = 0;
                                        l[0] = l[1] = l[2] = l[3] = 0;
                                        break;
                                    }

                                }
                            }
                        }
                    }
                }
            }
            /*Strich*/
            if(strcmp(zeile, "STRICH")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, " X1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[0] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, " Y1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[1] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, " X2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[2] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, " Y2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[3] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if(schreibLayer!=NULL)
                        {
                            new Strich(wert[0], wert[1], wert[2], wert[3], schreibLayer);
                            wert[0] = wert[1] = wert[2] = wert[3] = wert[4] = wert[5] = 0.0;
                            break;
                        }
                    }
                }
            }
            /*Bogen*/
            if(strcmp(zeile, "BOGEN")==0)
            {
                while(!Datei.eof())
                {
                    Datei.get(zeile, 64);
                    while (Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, " X1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[0] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, " Y1")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[1] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, " X2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[2] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, " Y2")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[3] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, " XM")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[4] = (double)atof(zeile);
                    }
                    if(strcmp(zeile, " YM")==0)
                    {
                        Datei.get(zeile, 64);
                        while (Datei.peek()=='\n')Datei.ignore();
                        wert[5] = -(double)atof(zeile);
                    }
                    if(strcmp(zeile, "  0")==0)
                    {
                        if(schreibLayer!=NULL)
                        {
                            new Bogen(wert[0], wert[1], wert[2], wert[3], wert[4], wert[5], schreibLayer);
                            wert[0] = wert[1] = wert[2] = wert[3] = wert[4] = wert[5] = 0.0;
                            break;
                        }
                    }
                }
            }
            /*neuer Abschnitt*/
            if(strcmp(zeile, "  0")==0)
            {
                wert[0] = wert[1] = wert[2] = 0.0;
                pointer[0] = pointer[1] = pointer[2] = pointer[3] = 0;
                p[0] = p[1] = 0;
                l[0] = l[1] = l[2] = l[3] = 0;
                /*letzten Abschnitt abschließen*/
            }
        }
    }

    Liste<HoehenMarke>* hmSammlung;
    HoehenMarke* tempHM;
    for(RUZ_Layer* tLay = m_layer->GetErstesElement(); tLay; tLay = m_layer->GetNaechstesElement())
    {
        hmSammlung = tLay->HoleHoehenMarken();
        for(tempHM = hmSammlung->GetErstesElement(); tempHM != NULL; tempHM = hmSammlung->GetNaechstesElement())
        {
            tempHM->LagePruefen();
        }
    }
    logSchreiben("RUZ einlesen abgeschlossen\n");
		logSchreiben(strLogEintrag);
    return true;
}

void RUZmBIFrame::LinieExtrudieren(Vektor nachPos)
{
    double gefaelle = 0.0;
    double value, t_abstand;
    bool t_abstandVerwenden = false;
    bool t_gefaelleVerwenden = false;

    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())//Alle Nicht-Linien aus der Auswahl entfernen
    {
        if(obj->HoleTyp() != RUZ_Linie)m_auswahl->Entfernen(obj);
    }

    if(m_auswahl->GetListenGroesse() != 0)
    {
        if(m_auswahl->GetErstesElement()->HoleTyp() == RUZ_Linie)
        {
            gefaelle = static_cast<Linie*>(m_auswahl->GetErstesElement())->HoleGefaelle(z);
            bool exitSchleife = false;
            while(!exitSchleife) //Gefaelle eingeben
            {
                wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie das gewünschte resultierende Gefälle an\nLeer, um das Gefälle der Linie beizubehalten"),
                                          wxT("Resultierendes Gefälle"), wxString::Format(wxT("%0.6f"), gefaelle));
                if(abfrage.ShowModal() == wxID_ABORT)return;
                wxString number = abfrage.GetValue();
                /*komma gegen punkt tauschen*/
                wxString punkt = wxT(".");
                wxString komma = wxT(",");
                number.Replace(komma, punkt);
                if(!number.ToDouble(&value))
                {
                    if(strcmp(number, "")!=0)
                    {
                        if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)break;
                    }else{
                        value = gefaelle;
                        t_gefaelleVerwenden = true;
                        exitSchleife = true;
                    }
                }
                exitSchleife = true;
            }
            exitSchleife = false;
            while(!exitSchleife) //Abstand eingeben
            {
                wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie einen Abstand ein\nLeer, um den Gewählten Punkt zu verwenden"),
                                          wxT("Extrusionabstand"), wxT(""));
                if(abfrage.ShowModal() == wxID_ABORT)
                {
                    t_abstandVerwenden = false;
                    exitSchleife = true;
                    break;
                }
                wxString number = abfrage.GetValue();
                /*komma gegen punkt tauschen*/
                wxString punkt = wxT(".");
                wxString komma = wxT(",");
                number.Replace(komma, punkt);
                if(!number.ToDouble(&t_abstand))
                {
                    if(strcmp(number, "")!=0)
                    {
                        if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)break;
                    }else{
                        t_abstandVerwenden = false;
                        exitSchleife = true;
                        break;
                    }
                }
                t_abstandVerwenden = true;
                exitSchleife = true;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    RUZ_Objekt* t_erstesElement = m_auswahl->GetErstesElement();
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* tempLn = static_cast<Linie*>(obj);

            int retVal;
            if(t_abstandVerwenden)
            {
                retVal = tempLn->Extrudieren(value, nachPos, t_abstand, z, t_gefaelleVerwenden);
            }else{
                retVal = tempLn->Extrudieren(value, nachPos, z, t_gefaelleVerwenden);
            }
            if(retVal == 1)
            {
                if(tempLn == t_erstesElement)
                {
                    if(wxMessageDialog(this, wxT("Gewünschtes resultierendes Gefälle kleiner als Gefälle der Linie!\nBitte neues Gefälle eingeben"),
                                       wxT("Neues Gefälle eingeben")).ShowModal() == wxID_ABORT)
                    {
                        return;
                    }
                }else{
                    logSchreiben("Gewünschtes resultierendes Gefälle kleiner als Gefälle der Linie!\n");
                }
            }
            if(retVal == 2)
            {
                if(tempLn == t_erstesElement)
                {
                    if(wxMessageDialog(this, wxT("Linie und Extrusionspunkt sind Kolinear!\nNeuen Extrusionpunkt wählen"),
                                       wxT("Punktfehler")).ShowModal() == wxID_ABORT)
                    {
                        return;
                    }
                }else{
                    logSchreiben("Linie und Extrusionspunkt sind Kolinear!\n");
                }
            }
            if(retVal == 3)
            {
                if(tempLn == t_erstesElement)
                {
                    if(wxMessageDialog(this, wxT("Viereck konnte nicht erzeugt werden.\nUnbekannter Fehler."),
                                       wxT("Abbruch")).ShowModal() == wxID_ABORT)
                    {
                        return;
                    }
                }else{
                    logSchreiben("Viereck konnte nicht erzeugt werden.\nUnbekannter Fehler.\n");
                }
            }
            if(retVal == -1)
            {
                /*alles Super*/
            }
        }
    }
    m_auswahl->ListeLeeren("RUZmBIFrame::LinieExtrudieren");
    return;
}

void RUZmBIFrame::LinieParallel(Vektor nachPos)
{
    double t_abstand;
    bool t_abstandVerwenden = false;

    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())//Alle Nicht-Linien aus der Auswahl entfernen
    {
        if(obj->HoleTyp() != RUZ_Linie)m_auswahl->Entfernen(obj);
    }

    if(m_auswahl->GetListenGroesse() != 0)
    {
        if(m_auswahl->GetErstesElement()->HoleTyp() == RUZ_Linie)
        {
            bool exitSchleife = false;
            while(!exitSchleife) //Abstand eingeben
            {
                wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie einen Abstand ein\nLeer, um den Gewählten Punkt zu verwenden"),
                                          wxT("Extrusionabstand"), wxT(""));
                if(abfrage.ShowModal() == wxID_ABORT)return;
                wxString number = abfrage.GetValue();
                /*komma gegen punkt tauschen*/
                wxString punkt = wxT(".");
                wxString komma = wxT(",");
                number.Replace(komma, punkt);
                if(!number.ToDouble(&t_abstand))
                {
                    if(strcmp(number, "")!=0)
                    {
                        if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)return;
                    }else{
                        t_abstandVerwenden = false;
                        exitSchleife = true;
                        continue;
                    }
                }
                t_abstandVerwenden = true;
                exitSchleife = true;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    Liste<RUZ_Objekt> *neue_auswahl = new Liste<RUZ_Objekt>();
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* tempLn = static_cast<Linie*>(obj);

            Linie* retLn;
            if(t_abstandVerwenden)
            {
                retLn = tempLn->Parallele(nachPos, t_abstand, z);
            }else{
                retLn = tempLn->Parallele(nachPos, z);
            }

            if(!retLn)
            {
                wxMessageDialog(this, wxT("Paralelle Linien konnten\nnicht erzeugt werden!"), wxT("Speicheranforderung")).ShowModal();
            }else
            {
                neue_auswahl->Hinzufuegen(retLn);
            }
        }
    }
    m_auswahl->ListeLeeren("RUZmBIFrame::LinieParallel");
    delete m_auswahl;
    m_auswahl = neue_auswahl;
    return;
}

void RUZmBIFrame::logSchreiben(const char* msg, ...)
{
    FILE *Logbuch;
    const char *pfad = "log/Debug.log";
    Logbuch = fopen(pfad, "a");
    va_list args;
    va_start (args, msg);
    vfprintf (Logbuch, msg, args);
    va_end (args);
    fclose(Logbuch);
    return;
}

void RUZmBIFrame::logSchreiben(std::string msg)
{
    ofstream Logbuch;
    const char *pfad = "log/Debug.log";
    Logbuch.open(pfad, ios_base::out|ios_base::app);
		if(Logbuch.good())
    {
      Logbuch<<msg;
    	Logbuch.close();
		}
    return;
}

void RUZmBIFrame::logSchreiben(const Vektor msg, int i)
{
    ofstream Logbuch;
    const char *pfad = "log/Debug.log";
    Logbuch.open(pfad, ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch.setf( ios::fixed, ios::floatfield );
        Logbuch.precision(i);
        Logbuch<<"x: "<<msg.x()<<" | y: "<<msg.y()<<" | z:"<<msg.z();
        Logbuch.close();
    }
    return;
}

void RUZmBIFrame::MenuEntmarkieren(void)
{
    KoordinatenMaske->Show(false);
    DoubleEingabe->Show(false);

    /*menuItems zurücksetzen*/
    menuLoeschen->Check(false);
    menuVerschieben->Check(false);
    menuKopieren->Check(false);
    menuKopierenNachLayer->Check(false);
    menuVersetzen->Check(false);
    menuDrehen->Check(false);
    menuPunktZeichnen->Check(false);
    menuLinieZeichnen->Check(false);
    menuKreisZeichnen->Check(false);
    menuLinieExtrudieren->Check(false);
    menuLinieParallel->Check(false);
    menuDreieckZeichnen->Check(false);
    menuViereckZeichnen->Check(false);
    menuStreckeMessen->Check(false);
    menuHoehenMarkeZeichnen->Check(false);
    menuPunktVereinigen->Check(false);
    menuPunkteSkalieren->Check(false);
    menuViereckTeilen->Check(false);
    menuFlaecheVerschneiden->Check(false);
    menuSchnittPunktFlaeche->Check(false);
    menuSchnittPunktLinie->Check(false);
    menuFangpunkteFinden->Check(false);
    /*ENDE menuItems zurücksetzen*/

    Refresh();
    return;
}

void RUZmBIFrame::ObjekteNullen(void)
{
    /*SchnittpunktSuche*/
    m_schP_OrgPkt = m_schP_Richtung_1 = m_schP_Richtung_2 = NULL;
    m_schP_Ln = NULL;
    m_schP_Dr = NULL;
    m_schP_Obj = NULL;
    if(aktLayer == alternativAktLayer)
    {
        aktLayer = aktLayerBAK;
    }
    aktLayerBAK = alternativAktLayer = NULL;
    /*ENDE SchnittpunktSuche*/

    markiertesObjekt = objFang1 = objFang2 = NULL;
    m_aktPunkt = NULL;
    m_aktLinie = NULL;
    m_aktKreis = NULL;
    vereinOrigPkt = vereinErsatzPkt = NULL;

    m_kopierAuswahl->ListeLoeschen("Objekte Nullen");
}

void RUZmBIFrame::OnAbout(wxCommandEvent &event)
{
	//wxProcess::Open("/../../HTML/RUZ-Handbuch/start.bat");
	return;
}

void RUZmBIFrame::OnAnsichtswechsel(wxCommandEvent &event)
{
    if(aktuelleAnsicht == ansicht_ID_hoehenkarte)
    {
        lwBild.TabulaRasa();
    }
    aktuelleAnsicht = (ansichtID)event.GetId();
    if(aktuelleAnsicht == ansicht_ID_hoehenkarte)
    {
        HoehenkarteZeichnen();
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnAusdehnungFinden(wxCommandEvent &event)
{
    AusdehnungFinden();
    return;
}

void RUZmBIFrame::OnBearbeitungsBefehl(wxCommandEvent &event)
{
    int aktID = event.GetId();
    RUZ_Objekt *tempObj;

    int layNr;

    befehlsID letzterBefehl = aktBefehl;
    MenuEntmarkieren();
    ObjekteNullen();

    switch(aktID)
    {
    case idMenuObjektLoeschen:
        if(letzterBefehl == bef_ID_loeschen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuLoeschen->Check(true);
        aktBefehl = bef_ID_loeschen;
        SetStatusText(wxT("Objekte löschen"), 2);
        break;

    case idMenuObjektVerschieben:
        if(letzterBefehl == bef_ID_verschieben)
        {
            m_markierModus = false;
            BefehleZuruecksetzen();
            break;
        }
        menuVerschieben->Check(true);
        aktBefehl = bef_ID_verschieben;
        m_markierModus = true;
        SetStatusText(wxT("Objekte zum Verschieben auswählen"), 2);
        break;

    case idMenuObjektKopieren:
        if(letzterBefehl == bef_ID_kopieren)
        {
            m_markierModus = false;
            BefehleZuruecksetzen();
            break;
        }
        menuKopieren->Check(true);
        aktBefehl = bef_ID_kopieren;
        m_markierModus = true;
        SetStatusText(wxT("Objekte zum Kopieren auswählen"), 2);
        break;

    case idMenuObjektKopierenNachLayer:
        if(letzterBefehl == bef_ID_kopierenNachLayer)
        {
            m_markierModus = false;
            BefehleZuruecksetzen();
            break;
        }
        menuKopierenNachLayer->Check(true);
        aktBefehl = bef_ID_kopierenNachLayer;
        m_markierModus = true;
        SetStatusText(wxT("Objekte zum Kopieren auswählen"), 2);
        break;

    case idMenuObjektDrehen:
        if(letzterBefehl == bef_ID_drehen)
        {
            m_markierModus = false;
            BefehleZuruecksetzen();
            (this->*DrehungAbbruch)();
            break;
        }
        menuDrehen->Check(true);
        aktBefehl = bef_ID_drehen;
        m_markierModus = true;
        SetStatusText(wxT("Objekte zum Drehen auswählen"), 2);
        /*Befehle einrichten*/
        DrehungBefehlsketteVor = &RUZmBIFrame::DrehungBefehlsketteVor_0;
        DrehungBefehlsketteZurueck = &RUZmBIFrame::DrehungBefehlsketteZurueck_0;
        DrehungPktEingabe = &RUZmBIFrame::DrehungPktEingabe_DrhPkt;
        DrehungAbbruch = &RUZmBIFrame::DrehungAbbruch_0;
        DrehungMouseMove = &RUZmBIFrame::DrehungMouseMovePassiv;
        /*ENDE Befehle einrichten*/
        break;

    case idMenuPunktVersetzen:
        if(letzterBefehl == bef_ID_versetzen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuVersetzen->Check(true);
        aktBefehl = bef_ID_versetzen;
        tempObj = m_auswahl->GetErstesElement();
        if(tempObj !=NULL)
        {
            if(tempObj->HoleTyp() == RUZ_Punkt)
            {
                KoordinatenMaske->SetzeKoordinaten(static_cast<Punkt*>(tempObj)->HolePosition());
            }
        }
        KoordinatenMaske->Show();
        SetStatusText(wxT("Punkt zum Versetzen auswählen"), 2);
        break;

    case idMenuPunkteSkalieren:
        if(letzterBefehl == bef_ID_punkteSkalieren)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuPunkteSkalieren->Check(true);
        aktBefehl = bef_ID_punkteSkalieren;
        m_markierModus = true;
        SetStatusText(wxT("Objekte zum Skalieren auswählen"), 2);
        break;

    case idMenuLayerSkalieren:
        if(letzterBefehl == bef_ID_layerSkalieren)
        {
            BefehleZuruecksetzen();
            break;
        }
        aktBefehl = bef_ID_layerSkalieren;
        Layer_Auswahl_Dialog(this, m_layer, wxT("Zu skalierende Layer auswählen"), false).ShowModal();
        break;

    case idMenuHintergrundSkalieren:
        if(letzterBefehl == bef_ID_hintergrundSkalieren)
        {
            BefehleZuruecksetzen();
            break;
        }
        aktBefehl = bef_ID_hintergrundSkalieren;
        m_skalierListe->Hinzufuegen(m_hintergrundLayer);
        SetStatusText(wxT("Festpunkt der Skalierung wählen"), 2);
        KoordinatenMaske->Show();
        break;

    case idMenuAllesSkalieren:
        if(letzterBefehl == bef_ID_allesSkalieren)
        {
            BefehleZuruecksetzen();
            break;
        }
        aktBefehl = bef_ID_allesSkalieren;
        for(RUZ_Layer* tempLayer = m_layer->GetErstesElement(); tempLayer; tempLayer = m_layer->GetNaechstesElement())
        {
            BefehleZuruecksetzen();
            m_skalierListe->Hinzufuegen(tempLayer);
        }
        m_skalierListe->Hinzufuegen(m_hintergrundLayer);
        SetStatusText(wxT("Festpunkt der Skalierung wählen"), 2);
        KoordinatenMaske->Show();
        break;

    case idMenuPunktZeichnen:
        if(letzterBefehl == bef_ID_punktZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuPunktZeichnen->Check(true);
        aktBefehl = bef_ID_punktZeichnen;
        KoordinatenMaske->Show();
        SetStatusText(wxT("Linke Mouse klicken, um Punkt zu setzen"), 2);
        break;

    case idMenuKreisZeichnen:
        if(letzterBefehl == bef_ID_kreisZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuKreisZeichnen->Check(true);
        aktBefehl = bef_ID_kreisZeichnen;
        KoordinatenMaske->Show();
        SetStatusText(wxT("Linke Mouse klicken, um Mittelpunkt zu setzen"), 2);
        break;

    case idMenuLinieZeichnen:
        if(letzterBefehl == bef_ID_linieZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuLinieZeichnen->Check(true);
        aktBefehl = bef_ID_linieZeichnen;
        if(aktLayer != NULL)
        {
            SetStatusText(wxT("Linien zeichnen auf Layer ") + wxString::FromUTF8(aktLayer->HoleName()), 2);
        }
        KoordinatenMaske->Show();
        m_auswahl->ListeLeeren("");
        break;

    case idMenuLinieParallel:
        if(letzterBefehl == bef_ID_linieParallel)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuLinieParallel->Check(true);
        aktBefehl = bef_ID_linieParallel;
        m_markierModus = true;
        if(aktLayer != NULL)
        {
            SetStatusText(wxT("Parallelen zeichnen auf Layer") + wxString::FromUTF8(aktLayer->HoleName()), 2);
        }
        m_auswahl->ListeLeeren("");
        break;

    case idMenuLinieExtrudieren:
        if(letzterBefehl == bef_ID_linieExtrudieren)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuLinieExtrudieren->Check(true);
        aktBefehl = bef_ID_linieExtrudieren;
        if(aktLayer != NULL)
        {
            SetStatusText(wxT("Linie extrudieren auf Layer") + wxString::FromUTF8(aktLayer->HoleName()), 2);
        }
        m_auswahl->ListeLeeren("");
        break;

    case idMenuDreieckZeichnen:
        if(letzterBefehl == bef_ID_dreieckZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuDreieckZeichnen->Check(true);
        aktBefehl = bef_ID_dreieckZeichnen;
        SetStatusText(wxT("Dreieck zeichnen - Linien als Rand auswählen"), 2);
        m_auswahl->ListeLeeren("");
        break;

    case idMenuViereckZeichnen:
        if(letzterBefehl == bef_ID_viereckZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuViereckZeichnen->Check(true);
        aktBefehl = bef_ID_viereckZeichnen;
        SetStatusText(wxT("Viereck zeichnen - Linien als Rand auswählen"), 2);
        m_auswahl->ListeLeeren("");
        break;

    case idMenuViereckTeilen:
        if(letzterBefehl == bef_ID_viereckTeilen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuViereckTeilen->Check(true);
        aktBefehl = bef_ID_viereckTeilen;
        SetStatusText(wxT("Vierecke teilen"), 2);
        break;

    case idMenuFlaechenVerschneiden:
        if(letzterBefehl == bef_ID_flaechenVerschneiden)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuFlaecheVerschneiden->Check(true);
        aktBefehl = bef_ID_flaechenVerschneiden;
        SetStatusText(wxT("Fläche verschneiden"), 2);
        break;

    case idMenuHoehenMarkeZeichnen:
        if(letzterBefehl == bef_ID_hoehenMarkeZeichnen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuHoehenMarkeZeichnen->Check(true);
        aktBefehl = bef_ID_hoehenMarkeZeichnen;
        SetStatusText(wxT("Setzt Hoehenmarken"), 2);
        break;

    case idMenuFangpunkteFinden:
        if(letzterBefehl == bef_ID_fangpunkteFinden)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuFangpunkteFinden->Check(true);
        aktBefehl = bef_ID_fangpunkteFinden;
        SetStatusText(wxT("Fangpunkt durch Verschneidung finden"), 2);
        break;

    case idMenuStreckeMessen:
        if(letzterBefehl == bef_ID_StreckeMessen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuStreckeMessen->Check(true);
        aktBefehl = bef_ID_StreckeMessen;
        SetStatusText(wxT("Strecke messen"), 2);
        m_auswahl->ListeLeeren("");
        Messen = &RUZmBIFrame::AnfangMessen;
        break;

    case idMenuPunktVereinigen:
        if(letzterBefehl == bef_ID_punkteVereinigen)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuPunktVereinigen->Check(true);
        aktBefehl = bef_ID_punkteVereinigen;
        SetStatusText(wxT("Punkt wählen"), 2);
        m_auswahl->ListeLeeren("");
        Vereinigen = &RUZmBIFrame::VereinigenErsterPkt;
        break;

    case idMenuSchnittPunktFlaeche:
        if(letzterBefehl == bef_ID_SchnittPunktFlaeche)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuSchnittPunktFlaeche->Check(true);
        aktBefehl = bef_ID_SchnittPunktFlaeche;
        m_markierModus = true;
        m_schP_OrgPkt = NULL;
        m_schP_Dr = NULL;
        m_schP_Obj = NULL;
        SetStatusText(wxT("Schnittpunkt: Punkt und Fläche wählen"), 2);
        m_auswahl->ListeLeeren("");
        layNr = Layer_Auswahl_Dialog(this, m_layer, "Schnittlayer (abbrechen = innerhalb des Layers)", true).ShowModal();
        if(layNr > -1)
        {
            for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
            {
                if(layer_LE->Wert() == layNr)
                {
                    alternativAktLayer = layer_LE->GetElement();
                    break;
                }
            }
        }else{
            alternativAktLayer = NULL;
            logSchreiben("kein Layer gewählt\n");
        }
        break;

    case idMenuSchnittPunktLinie:
        if(letzterBefehl == bef_ID_SchnittPunktLinie)
        {
            BefehleZuruecksetzen();
            break;
        }
        menuSchnittPunktLinie->Check(true);
        aktBefehl = bef_ID_SchnittPunktLinie;
        m_markierModus = true;
        m_schP_OrgPkt = NULL;
        m_schP_Ln = NULL;
        m_schP_Obj = NULL;
        SetStatusText(wxT("Schnittpunkt: Punkt und Linie wählen"), 2);
        m_auswahl->ListeLeeren("");
        layNr = Layer_Auswahl_Dialog(this, m_layer, "Schnittlayer (abbrechen = innerhalb des Layers)", true).ShowModal();
        if(layNr > -1)
        {
            for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
            {
                if(layer_LE->Wert() == layNr)
                {
                    alternativAktLayer = layer_LE->GetElement();
                    break;
                }
            }
        }else{
            alternativAktLayer = NULL;
        }
        break;
    }
    return;
}

void RUZmBIFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void RUZmBIFrame::OnDreieckeFinden(wxCommandEvent &event)
{
    AuswahlLeeren();
    if(aktLayer!=NULL)
    {
        aktLayer->DreieckeFinden();
        Refresh();
    }else{
        SetStatusText(wxT("Kein Layer vorhanden!"), 1);
    }
    return;
}

void RUZmBIFrame::OnEinstellungen(wxCommandEvent &event)
{
    peEinstellungenDlg->ShowModal();

    m_verschubWeite = peEinstellungenDlg->HoleWert(IDverschubWeite);
    m_lnWandelGenauigkeit = peEinstellungenDlg->HoleWert(IDlnWandelGenauigkeit);
    m_anzeigeGenauigkeit = (int)(peEinstellungenDlg->HoleWert(IDanzeigeGenauigkeit));
        if(m_anzeigeGenauigkeit < 0)m_anzeigeGenauigkeit = 0;
    m_pseudoSchattenFkt = peEinstellungenDlg->HoleWert(IDpseudoSchattenFkt);
        if(m_pseudoSchattenFkt < 0)m_pseudoSchattenFkt = 0;
    pxSuchEntfernung = (int)(peEinstellungenDlg->HoleWert(IDpxSuchEntfernung));
        if(pxSuchEntfernung < 1)pxSuchEntfernung = 1;

    Refresh();
    return;
}

void RUZmBIFrame::OnEntferneHoeheres(wxCommandEvent& event)
{
    AuswahlLeeren();
    if(aktLayer)
    {
        aktLayer->EntferneHoeheres(aktProjZ);
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnEntferneTieferes(wxCommandEvent& event)
{
    AuswahlLeeren();
    if(aktLayer)
    {
        aktLayer->EntferneTieferes(aktProjZ);
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnEraseBackground(wxEraseEvent& event)
{
    return;
}

void RUZmBIFrame::OnGefaelleRasterZeigen(wxCommandEvent &event)
{
    if(!menuGefaelleRasterZeigen->IsChecked())
    {
        if(aktLayer)aktLayer->GefaelleRasterLoeschen();
        gefaelleRasterAnzeigen = false;
        return;
    }
    gefaelleRasterAnzeigen = true;
    m_gefaelleRaster = peEinstellungenDlg->HoleWert(IDgefaelleRasterGroesse);
    m_flaechenRaster = peEinstellungenDlg->HoleWert(IDflaechenRasterGroesse);
    double dTempMin = peEinstellungenDlg->HoleWert(IDgefaelleMinimum)/100;
    double dTempOpt = peEinstellungenDlg->HoleWert(IDgefaelleOptimum)/100;
    double dTempMax = peEinstellungenDlg->HoleWert(IDgefaelleMaximum)/100;

    if(aktLayer)aktLayer->GefaelleRasterAnlegen(m_gefaelleRaster, dTempMin, dTempOpt, dTempMax);
    //wxMessageDialog(this, wxT("Angelegt")).ShowModal();
    Refresh();
    return;
}

void RUZmBIFrame::OnGefaelleZeigen(wxCommandEvent &event)
{
    if(menuGefaelleZeigen->IsChecked())
    {
        //m_gefaelleRaster = peEinstellungenDlg->HoleWert(IDgefaelleRasterGroesse);
        //wxMessageDialog(this, wxString::Format("Gefälleraster: %0.2f", gefaelle)).ShowModal();
        gefaelleAnzeigen = 1;
    }else{
        gefaelleAnzeigen = 0;
    }
    return;
}

void RUZmBIFrame::OnHintergrundEinlesen(wxCommandEvent &event)
{
    FileOpener->SetWildcard(wxT("DXF-Datei (*.dxf)|*.dxf"));
    int Rueckgabe = FileOpener->ShowModal();
    if(Rueckgabe==wxID_CANCEL)return;
    SetStatusText(FileOpener->GetPath(), 1);

    /*DXF Einlesen*/
    if((FileOpener->GetPath()).EndsWith(wxT("f"))||(FileOpener->GetPath()).EndsWith(wxT("F")))/*Wenn dxf ausgewählt wurde*/
    {
        char x_kennung[64], y_kennung[64], z_kennung[64], sw_kennung[64];
        dxfParameterDlg->HoleKennung(x_kennung, IDpktXKenn);
        dxfParameterDlg->HoleKennung(y_kennung, IDpktYKenn);
        dxfParameterDlg->HoleKennung(z_kennung, IDpktZKenn);
        dxfParameterDlg->HoleKennung(sw_kennung, IDpktSW);

        DXF_Import *dxfImporteur = new DXF_Import((char*)static_cast<const char*>(FileOpener->GetPath().c_str()),
                                                         x_kennung, y_kennung, z_kennung, sw_kennung);
        if(!dxfImporteur->EinlesenHintergrund(m_hintergrundLayer))
        {
            wxMessageDialog(this, wxString::FromUTF8("Beim Einlesen ist ein Fehler aufgetreten.\nEvtl. wurde nicht alles eingelesen")).ShowModal();
        }
        wxMessageDialog(this, wxString::FromUTF8(wxString::Format("%d Striche\n%d Bögen", m_hintergrundLayer->HoleStriche()->GetListenGroesse()
                                               , m_hintergrundLayer->HoleBoegen()->GetListenGroesse()))).ShowModal();
        AusdehnungFinden();
        delete dxfImporteur;
    }/*ENDE DXF Einlesen*/
    else/*Ansonsten ist es eine ruz-datei*/
    {
        wxMessageDialog(this, wxT("Einlesen von RUZ-Dateien wird derzeit nicht unterstützt."), wxT("Abbruch")).ShowModal();
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnHintergrundLoeschen(wxCommandEvent &event)
{
    m_hintergrundLayer->HolePunkte()->ListeLoeschen("Hintergrundlayer löschen");
    m_hintergrundLayer->HoleLinien()->ListeLoeschen("Hintergrundlayer löschen");
    m_hintergrundLayer->HoleFlaechen()->ListeLoeschen("Hintergrundlayer löschen");
    m_hintergrundLayer->HoleStriche()->ListeLoeschen("Hintergrundlayer löschen");
    m_hintergrundLayer->HoleBoegen()->ListeLoeschen("Hintergrundlayer löschen");
    m_hintergrundLayer->HoleHoehenMarken()->ListeLoeschen("Hintergrundlayer löschen");
    Refresh();
    return;
}

void RUZmBIFrame::OnHLEinstellen(wxCommandEvent &event)
{
    hlParameterDlg->ShowModal();

    hoehenSchritt = hlParameterDlg->HoleWert(IDhoehenSchritt);
    suchRadius = hlParameterDlg->HoleWert(IDsuchRadius);
    startHoehe = hlParameterDlg->HoleWert(IDstartHoehe);

    Liste<Flaeche>* flSammlung;
    if(aktLayer!=NULL)
    {
        aktLayer->SetzeHLParameter(hoehenSchritt, suchRadius, startHoehe);
        flSammlung = aktLayer->HoleFlaechen();
        for(Flaeche* tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
        {
            tempFlaeche->HoehenlinienFinden();
        }
        Refresh();
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnHLZeigen(wxCommandEvent &event)
{
    if(menuHLZeigen->IsChecked())
    {
        Liste<Flaeche>* flSammlung;
        if(aktLayer!=NULL)
        {
            aktLayer->SetzeHLParameter(hoehenSchritt, suchRadius, startHoehe);
            flSammlung = aktLayer->HoleFlaechen();
            for(Flaeche* tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
            {
                tempFlaeche->HoehenlinienFinden();
            }
            Refresh();
        }
        hlAnzeigen = 1;
    }else{
        hlAnzeigen = 0;
    }
    return;
}

void RUZmBIFrame::OnKantenWandeln(wxCommandEvent &event)
{
    SetStatusText(wxT("Striche in Linien wandeln"));
    if(aktLayer!=NULL)
    {
        aktLayer->LinienAusStrichen(peEinstellungenDlg->HoleWert(IDlnWandelGenauigkeit), z);
    }else{
        SetStatusText(wxT("Kein aktueller Layer vorhanden!"), 1);
    }
    return;
}

void RUZmBIFrame::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == WXK_ESCAPE)
    {
        if(aktBefehl == bef_ID_linieZeichnen)
        {
            if(!m_aktPunkt)//m_auswahl->GetListenGroesse() == 0)
            {
                BefehleZuruecksetzen();
            }else{
                //m_auswahl->ListeLeeren("");
                delete m_aktPunkt;
                m_aktPunkt = NULL;
                m_aktLinie = NULL;
            }
            Refresh();
            return;
        }
        if(aktBefehl == bef_ID_drehen)
        {
            (this->*DrehungBefehlsketteZurueck)();
        }else
        if(aktBefehl == bef_ID_SchnittPunktFlaeche)
        {
            if(m_markierModus)
            {
                if(m_schP_OrgPkt || m_schP_Dr)
                {
                    m_schP_OrgPkt = NULL;
                    m_schP_Dr = NULL;
                }
                else
                {
                    BefehleZuruecksetzen();
                }
            }
            else if(m_markierModus2)
            {
                if(m_schP_Obj)
                {
                    m_schP_Obj = NULL;
                    if(m_schP_OrgPkt)m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
                }
                else
                {
                    m_markierModus2 = false;
                    m_markierModus = true;
                    SetStatusText(wxT("Schnittpunkt: Punkt und Fläche wählen"), 2);
                    if(alternativAktLayer)aktLayer = aktLayerBAK;
                }
            }
            else
            {
                if(m_schP_Obj)
                {
                    m_schP_Obj = NULL;
                    delete m_schP_Richtung_1;
                    delete m_schP_Richtung_2;
                    m_schP_Richtung_1 = m_schP_Richtung_2 = NULL;
                    m_schP_Ln = NULL;
                    markiertesObjekt = NULL;
                    if(m_schP_OrgPkt)m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
                    m_markierModus = false;
                    m_markierModus2 = true;
                }
                else
                {
                    m_schP_Richtung_1 = m_schP_Richtung_2 = NULL;
                    m_schP_Ln = NULL;
                    BefehleZuruecksetzen();
                }
            }
        }else
        if(aktBefehl == bef_ID_SchnittPunktLinie)
        {
            if(m_markierModus)
            {
                if(m_schP_OrgPkt || m_schP_Ln)
                {
                    m_schP_OrgPkt = NULL;
                    m_schP_Ln = NULL;
                }
                else
                {
                    BefehleZuruecksetzen();
                }
            }
            else if(m_markierModus2)
            {
                if(m_schP_Obj)
                {
                    m_schP_Obj = NULL;
                    if(m_schP_OrgPkt)m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
                }
                else
                {
                    m_markierModus = true;
                    m_markierModus2 = false;
                    SetStatusText(wxT("Schnittpunkt: Punkt und Linie wählen"), 2);
                    if(alternativAktLayer)aktLayer = aktLayerBAK;
                }
            }
            else
            {
                if(m_schP_Obj)
                {
                    m_schP_Obj = NULL;
                    m_markierModus = false;
                    m_markierModus2 = true;
                }
                else
                {
                    BefehleZuruecksetzen();
                }
            }
        }else
        if((aktBefehl == bef_ID_verschieben))
        {
            Vschb_Abbrechen();
        }else
        if((aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
        {
            Kop_Abbrechen();
        }else
        if(aktBefehl == bef_ID_fangpunkteFinden)
        {
            if(m_auswahl->GetListenGroesse() == 0)
                BefehleZuruecksetzen();
            else
                m_auswahl->ListeLeeren("");
        }else
        if(aktBefehl == bef_ID_linieParallel)
        {
            if(!m_markierModus)m_markierModus = true;
            else BefehleZuruecksetzen();
        }
        else{
            BefehleZuruecksetzen();
        }
        Refresh();
    }
    if((event.GetKeyCode() == WXK_RETURN)||(event.GetKeyCode() == WXK_NUMPAD_ENTER))
    {
        if(aktBefehl == bef_ID_linieParallel)
        {
            if(m_markierModus)
            {
                m_markierModus = false;
            }else
            if(m_auswahl->GetListenGroesse() != 0)
            {
                Vektor vPosition(0, 0, 0);
                vPosition.SetKoordinaten(aktProjX, (double)MarkierMousePosition.x / m_skalierung + dc_Offset[0]);
                vPosition.SetKoordinaten(aktProjY, (double)MarkierMousePosition.y / m_skalierung + dc_Offset[1]);
                LinieParallel(vPosition);
            }
        }else if(aktBefehl == bef_ID_SchnittPunktFlaeche)
        {
            SchnittpunktFlaecheAbschliessen();
        }else if(aktBefehl == bef_ID_SchnittPunktLinie)
        {
            SchnittpunktLinieAbschliessen();
        }else if(aktBefehl == bef_ID_fangpunkteFinden)
        {
            FangpunkteFinden(m_auswahl);
            m_auswahl->ListeLeeren("");
        }else if(aktBefehl == bef_ID_verschieben)
        {
            Vschb_Auswahl_Bestaetigung();
        }else if((aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
        {
            Kop_Auswahl_Bestaetigung();
        }else if(aktBefehl == bef_ID_drehen)
        {
            if(m_markierModus)
            {
                DrehungBestaetigung();
                SetStatusText(wxT("Festpunkt der Drehung setzen"), 2);
            }
        }
        Refresh();
        return;
    }
    if(event.GetKeyCode() == WXK_F12)
    {
        AusdehnungFinden();
        Refresh();
        return;
    }
    if(event.GetKeyCode() == WXK_DELETE)
    {
        AuswahlLoeschen();
        SetStatusText(wxT("Objekte gelöscht"), 2);
        Refresh();
        return;
    }
    if(event.GetKeyCode() == WXK_SHIFT)
    {
        wertFkt = 10;
        m_kreuzen = true;
        return;
    }
    if(event.GetKeyCode() == WXK_CONTROL)
    {
        anzeigeSkalieren = true;
        return;
    }
    if(event.GetKeyCode() == WXK_SPACE)
    {
        if(aktBefehl == bef_ID_SchnittPunktLinie)
        {
            if(!std::isnan(m_vktSchnittPkt1.x()) && !std::isnan(m_vktSchnittPkt2.x()))
            {
                Vektor tempvkt = m_vktSchnittPkt1;
                m_vktSchnittPkt1 = m_vktSchnittPkt2;
                m_vktSchnittPkt2 = tempvkt;
                m_schP_OrgPkt->Positionieren(m_vktSchnittPkt1);
            }
        }
        if(markiertesObjekt)
        {
            RUZ_Layer* t_lay = markiertesObjekt->HoleLayer();
            if((markiertesObjekt->HoleTyp() == RUZ_Dreieck)||(markiertesObjekt->HoleTyp() == RUZ_Viereck))t_lay->HoleFlaechen()->AnsEndeStellen(static_cast<Flaeche*>(markiertesObjekt));
            if(markiertesObjekt->HoleTyp() == RUZ_Punkt)t_lay->HolePunkte()->AnsEndeStellen(static_cast<Punkt*>(markiertesObjekt));
            if(markiertesObjekt->HoleTyp() == RUZ_Linie)t_lay->HoleLinien()->AnsEndeStellen(static_cast<Linie*>(markiertesObjekt));
            if(markiertesObjekt->HoleTyp() == RUZ_HoehenMarke)t_lay->HoleHoehenMarken()->AnsEndeStellen(static_cast<HoehenMarke*>(markiertesObjekt));
            if(ObjektMarkieren(MarkierMousePosition.x, MarkierMousePosition.y))Refresh();
        }
        return;
    }
    if(event.GetKeyCode() == WXK_TAB)
    {
        if(DoubleEingabe->IsShown())DoubleEingabe->SetFocus();
        if(KoordinatenMaske->IsShown())KoordinatenMaske->SetFocus();
        return;
    }
    if(event.GetKeyCode() == 'C')
    {
        AuswahlKopieren();
        return;
    }
    return;
}

void RUZmBIFrame::OnKeyUp(wxKeyEvent& event)
{
    if(event.GetKeyCode() == WXK_SHIFT)
    {
        wertFkt = 1;
        m_kreuzen = false;
    }
    if(event.GetKeyCode() == WXK_CONTROL)
    {
        anzeigeSkalieren = false;
    }
    return;
}

void RUZmBIFrame::OnKomplettVernetzen(wxCommandEvent &event)
{
    AuswahlLeeren();
    if(aktLayer!=NULL)
    {
        aktLayer->LinienAusStrichen(peEinstellungenDlg->HoleWert(IDlnWandelGenauigkeit), z);
        SetStatusText(wxT("Punkte vernetzen"));
        double dauer = aktLayer->PunkteVernetzen();
        SetStatusText(wxString::Format("Vernetzen dauerte %1.5f Sekunden", dauer), 1);
        aktLayer->DreieckeFinden();
        aktLayer->ViereckeFinden();
        Refresh();
    }else{
        SetStatusText(wxT("Kein Layer vorhanden!"), 1);
    }
    return;
}

void RUZmBIFrame::OnLayerAuswahl(wxCommandEvent& event)
{
    LayerAuswahl->ShowModal();
    Refresh();
    return;
}

void RUZmBIFrame::OnLayerinhaltAnzeigen(wxCommandEvent &event)
{
    if(aktLayer)
    {
        int anzPkt = aktLayer->HolePunkte()->GetListenGroesse();
        int anzLn = aktLayer->HoleLinien()->GetListenGroesse();
        int anzKr = aktLayer->HoleKreise()->GetListenGroesse();
        int anzFP = aktLayer->HoleFangpunkte()->GetListenGroesse();
        int anzDrk = 0;
        int anzVrk = 0;
        Liste<Flaeche> *flLst = aktLayer->HoleFlaechen();
        for(Flaeche *tFl = flLst->GetErstesElement(); tFl; tFl = flLst->GetNaechstesElement())
        {
            (tFl->HoleTyp() == RUZ_Dreieck) ? anzDrk++ : anzVrk++;
        }
        int anzHP = aktLayer->HoleHoehenMarken()->GetListenGroesse();
        int anzGM = aktLayer->HoleGefaelleMarken()->GetListenGroesse();
        wxMessageDialog(this, wxString::FromUTF8(aktLayer->HoleName()) +
                            wxString::FromUTF8(wxString::Format(" enthält: \n%d Punkt(e)\n%d Linie(n)\n%d Kreis(e)\n%d Fangpunkt(e)\n%d Dreieck(e)\n%d Viereck(e)\n%d Höhenmarke(n)\n%d Gefällemarke(n)",
                                             anzPkt, anzLn, anzKr, anzFP, anzDrk, anzVrk, anzHP, anzGM))).ShowModal();
    }
    return;
}

void RUZmBIFrame::OnLayerInSichVerschneiden(wxCommandEvent &event)
{
    AuswahlLeeren();
    if(aktLayer)
    {
        aktLayer->VerschneideFlaechen(aktLayer->HoleFlaechen(), m_anzeigeGenauigkeit+3);
    }
    return;
}

void RUZmBIFrame::OnLayerKopieren(wxCommandEvent &event)
{
    int layNr = Layer_Auswahl_Dialog(this, m_layer, wxT("Zu kopierenden Layer wählen")).ShowModal();
    RUZ_Layer* sel_Layer = aktLayer;

    for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
    {
        if(layer_LE->Wert() == layNr)
        {
            sel_Layer = layer_LE->GetElement();
            break;
        }
    }

    if(sel_Layer)
    {
        char layerName[256];
        strcpy(layerName, "Cc_");
        strncat(layerName, sel_Layer->HoleName(), 253);
        LayerKopieren(sel_Layer, layerName);
    }
    return;
}

void RUZmBIFrame::OnLayerRandabschneiden(wxCommandEvent &event)
{
    AuswahlLeeren();
    int layNr1 = Layer_Auswahl_Dialog(this, m_layer, wxT("Ersten Layer wählen")).ShowModal();
    int layNr2;
    if(layNr1 == -1)return;
    do
    {
        layNr2 = Layer_Auswahl_Dialog(this, m_layer, wxT("Zweiten Layer wählen")).ShowModal();
        if(layNr2 == -1)return;
        if(layNr1 == layNr2)
        {
            if(wxMessageDialog(this, wxT("Layer kann nicht am eigenen Rand abgeschnitten werden!\nBitte neuen zweiten Layer wählen."), wxT("Layer identisch")).ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }
    }while(layNr1 == layNr2);

    //char layerName[256];
    RUZ_Layer* erster_Layer = NULL;
    RUZ_Layer* zweiter_Layer = NULL;
    //RUZ_Layer *erster_neuer_Layer, *zweiter_neuer_Layer;

    for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
    {
        if(layer_LE->Wert() == layNr1)
        {
            erster_Layer = layer_LE->GetElement();
        }
        if(layer_LE->Wert() == layNr2)
        {
            zweiter_Layer = layer_LE->GetElement();
        }
        if(erster_Layer && zweiter_Layer)break;
    }
    if(!erster_Layer || !zweiter_Layer)
    {
        wxMessageDialog(this, wxT("Layerauswahl war nicht erfolgreich."), wxT("Abbruch")).ShowModal();
    }

    erster_Layer->RandAbschneiden(zweiter_Layer);
    erster_Layer->LoescheFreiliegendeFlaechen(zweiter_Layer);
    zweiter_Layer->LoescheFreiliegendeFlaechen(erster_Layer);
    return;
}

void RUZmBIFrame::OnLayerVerschneiden(wxCommandEvent &event)
{
    AuswahlLeeren();
    /*Die zu verschneidenden Layer auswählen*/
    int layNr1 = Layer_Auswahl_Dialog(this, m_layer, wxT("Ersten Layer wählen")).ShowModal();
    int layNr2;
    if(layNr1 == -1)return;
    do
    {
        layNr2 = Layer_Auswahl_Dialog(this, m_layer, wxT("Zweiten Layer wählen")).ShowModal();
        if(layNr2 == -1)return;
        if(layNr1 == layNr2)
        {
            if(wxMessageDialog(this, wxT("Layer kann nicht mit sich selber verschnitten werden!\nBitte neuen zweiten Layer wählen."), wxT("Layer identisch")).ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }
    }while(layNr1 == layNr2);

    char layerName[256];
    RUZ_Layer* erster_Layer = NULL;
    RUZ_Layer* zweiter_Layer = NULL;
    RUZ_Layer *erster_neuer_Layer, *zweiter_neuer_Layer;

    for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
    {
        if(layer_LE->Wert() == layNr1)
        {
            erster_Layer = layer_LE->GetElement();
        }
        if(layer_LE->Wert() == layNr2)
        {
            zweiter_Layer = layer_LE->GetElement();
        }
        if(erster_Layer && zweiter_Layer)break;
    }
    if(!erster_Layer || !zweiter_Layer)
    {
        wxMessageDialog(this, wxT("Layerauswahl war nicht erfolgreich."), wxT("Abbruch")).ShowModal();
        return;
    }
    wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie einen Namen an"), wxT("Name der Verschneidung"), wxT("Verschneidung"));
    abfrage.ShowModal();
    wxString namensZusatzString = abfrage.GetValue();
    wxCharBuffer buffer = namensZusatzString.ToUTF8();
    char* namensZusatzchar(buffer.data());  // data() returns const char *

    strncpy(layerName, namensZusatzchar, 128);
    strncat(layerName, "_", (256 - 128));
    strncat(layerName, erster_Layer->HoleName(), (256 - 128 -1));
    erster_neuer_Layer = LayerKopieren(erster_Layer, layerName);

    strncpy(layerName, namensZusatzchar, 128);
    strncat(layerName, "_", (256 - 128));
    strncat(layerName, zweiter_Layer->HoleName(), (256 - 128 -1));
    zweiter_neuer_Layer = LayerKopieren(zweiter_Layer, layerName);

    if((!erster_neuer_Layer)||(!zweiter_neuer_Layer))return;
    /*ENDE Die zu verschneidenden Layer auswählen*/

    /*Das eigentliche Verschneiden*/
		thread_info_verschnitt thInf(erster_neuer_Layer, zweiter_neuer_Layer);
		//RUZ_Layer *hilfsLayer, *randLayer1, *randLayer2;
		//thInf.HoleLayer(&hilfsLayer, &randLayer1, &randLayer2, &erster_neuer_Layer, &zweiter_neuer_Layer);
		int t_genauigkeit = m_anzeigeGenauigkeit+3;
		std::thread thVerschnitt(&RUZ_Layer::Verschneiden, erster_neuer_Layer, zweiter_neuer_Layer, &thInf, &t_genauigkeit);
		thVerschnitt.detach();

		RUZThreadCtrl(&thInf, 200, this, wxID_ANY, wxString::Format("Layer verschneiden")).ShowModal();
    /*ENDE Das eigentliche Verschneiden*/
    return;
}

void RUZmBIFrame::OnLoescheLayer(wxCommandEvent &event)
{
    AuswahlLeeren();
    LayerLoeschen(wxT("Wollen Sie ALLE Layer\nunwiederbringlich löschen?"));
    return;
}

void RUZmBIFrame::OnLoescheStriche(wxCommandEvent &event)
{
		if(!aktLayer)return;
		Liste<Strich> *LstStriche = aktLayer->HoleStriche();
		LstStriche->ListeLoeschen("RUZmBIFrame::OnLoescheStriche");
		return;
}

void RUZmBIFrame::OnMouseLDClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    AlteMousePosition = event.GetLogicalPosition(dc);
    if(markiertesObjekt)
    {
        int objTyp = markiertesObjekt->HoleTyp();
        if(objTyp == RUZ_Kreis)
        {
            Kreis* t_aktKreis = static_cast<Kreis*>(markiertesObjekt);
            int naehe = t_aktKreis->IstNahebei((AlteMousePosition.x / m_skalierung) + dc_Offset[0],
                                               (AlteMousePosition.y / m_skalierung) + dc_Offset[1],
                                               pxSuchEntfernung / m_skalierung, aktProjZ);
            if(naehe == 1)
            {
                /*m_auswahl->ListeLeeren("");
                m_auswahl->Hinzufuegen(m_aktKreis);*/
                m_aktKreis = NULL;
            }
            if(naehe == 2)
            {
                SetStatusText(wxString::Format("Kreis markiert: %p", t_aktKreis), 1);
                m_aktRadius = t_aktKreis->HoleRadius();
                BefehleZuruecksetzen();
                aktBefehl = bef_ID_kreisRadiusAendern;
                m_aktKreis = t_aktKreis;
                DoubleEingabe->ErscheinungAnpassen(wxString("Radius"), wxString::Format("%0.4f", m_aktRadius), aktBefehl);
                DoubleEingabe->Show(true);
                this->SetFocus();
            }
        }
        if(objTyp == RUZ_Punkt)
        {
            /*m_aktPunkt = static_cast<Punkt*>(markiertesObjekt);
            m_aktPosition = m_aktPunkt->HolePosition();*/
        }
    }
    event.Skip();
    return;
}

void RUZmBIFrame::OnMouseLeftClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    RUZ_Objekt* obj;
    Vektor vPosition;

    m_markierungsRechteck = true;
    AlteMousePosition = event.GetLogicalPosition(dc);

    if(aktLayer != NULL)
    {
        vPosition.SetKoordinaten(aktProjX, (double)AlteMousePosition.x / m_skalierung + dc_Offset[0]);
        vPosition.SetKoordinaten(aktProjY, (double)AlteMousePosition.y / m_skalierung + dc_Offset[1]);
        vPosition.SetKoordinaten(aktProjZ, 0.0);

        if(markiertesObjekt!=NULL)
        {
            int objTyp = markiertesObjekt->HoleTyp();
            if(objTyp == RUZ_Fangpunkt)
            {
                vPosition = static_cast<Fangpunkt*>(markiertesObjekt)->HolePosition();
            }else
            if(objTyp == RUZ_Punkt)
            {
                vPosition = static_cast<Punkt*>(markiertesObjekt)->HolePosition();
            }else
            if(objTyp == RUZ_Linie)
            {
                static_cast<Linie*>(markiertesObjekt)->LotFussPunkt(vPosition, aktProjZ, false);
            }
        }
        if(KoordinatenMaske->IsShown())
        {
            vPosition += Vektor(KoordinatenMaske->HoleKoordinaten(aktProjX),
                                -KoordinatenMaske->HoleKoordinaten(aktProjY),
                                KoordinatenMaske->HoleKoordinaten(aktProjZ));
            KoordinatenMaske->SetzeKoordinaten(Vektor(0.0, 0.0, 0.0));
        }
    }else{
        wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
        event.Skip();
        Refresh();
        return;
    }

    switch(aktBefehl)
    {
    case bef_ID_nichts:
        if(gefaelleAnzeigen)
        {
            m_markierungsRechteck = false;
            GefaelleVerfolgen(vPosition);
            break;
        }
    case bef_ID_loeschen:
        if(markiertesObjekt!=NULL)
        {
            m_auswahl->Entzufuegen(markiertesObjekt);
        }
        break;

    case bef_ID_verschieben:
        if(m_markierModus)
        {
            if(markiertesObjekt!=NULL)
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
            }
        }else
        {
            Vschb_Punkt_Festlegen(vPosition);
        }
        break;

    case bef_ID_kopieren:
    case bef_ID_kopierenNachLayer:
        if(m_markierModus)
        {
            if(markiertesObjekt!=NULL)
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
            }
        }else
        {
            Kop_Punkt_Festlegen(vPosition);
        }
        break;

    case bef_ID_drehen:
        if(m_markierModus)
        {
            if(markiertesObjekt!=NULL)
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
            }
        }else
        {
            (this->*DrehungPktEingabe)(vPosition);
        }
        break;

    case bef_ID_punktZeichnen:
        if(markiertesObjekt!=NULL)
        {
            if(aktLayer != NULL)
            {
                int objTyp = markiertesObjekt->HoleTyp();
                if(objTyp == RUZ_Punkt)
                {
                    Vektor tVkt;
                    markiertesObjekt->HolePosition(tVkt);
                    if(tVkt == vPosition)break;
                }
                Punkt *tempPunkt = new Punkt(vPosition, aktLayer);
                if(objTyp == RUZ_Dreieck)
                {
                    Dreieck* tempDrk = static_cast<Dreieck*>(markiertesObjekt);
                    if(tempDrk->IstInnerhalb(vPosition.GetKoordinaten(aktProjX), vPosition.GetKoordinaten(aktProjY), aktProjZ))
                    {
                        tempDrk->Teilen(tempPunkt, z, true);
                        markiertesObjekt = NULL;
                        break;
                    }
                }else
                if(objTyp == RUZ_Linie)
                {
                    Linie* tempLn = static_cast<Linie*>(markiertesObjekt);
                    tempLn->Teilen(tempPunkt, z, true);
                    markiertesObjekt = NULL;
                    break;
                }
            }else
            {
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
            }
        }else
        {
            if(aktLayer != NULL)
            {
                new Punkt(vPosition, aktLayer);
            }else{
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
            }
        }
        break;

    case bef_ID_kreisZeichnen:
    {
        if(markiertesObjekt!=NULL)
        {
            if(aktLayer != NULL)
            {
                int objTyp = markiertesObjekt->HoleTyp();
                if(objTyp == RUZ_Linie)
                {
                    static_cast<Linie*>(markiertesObjekt)->LotFussPunkt(vPosition, aktProjZ, true);
                }
            }else{
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
            }
            markiertesObjekt = NULL;
        }else
        {
            if(aktLayer == NULL)
            {
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
                break;
            }
        }
        KreisPunktEingabe(vPosition, true);
    }
    break;

    case bef_ID_kreisRadiusAendern:
    {
        if(markiertesObjekt!=NULL)
        {
            if(aktLayer != NULL)
            {
                int objTyp = markiertesObjekt->HoleTyp();
                if(objTyp == RUZ_Linie)
                {
                    static_cast<Linie*>(markiertesObjekt)->LotFussPunkt(vPosition, aktProjZ, true);
                }
            }else{
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
            }
            markiertesObjekt = NULL;
        }else
        {
            if(aktLayer == NULL)
            {
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
                break;
            }
        }
        KreisPunktEingabe(vPosition, true);
    }
    break;

    case bef_ID_linieZeichnen:
        if(markiertesObjekt!=NULL)
        {
            if(markiertesObjekt->HoleTyp() == RUZ_Fangpunkt)
            {
                if(aktLayer)
                {
                    if(!m_aktPunkt)
                    {
                        Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                        if(pktTemp)
                        {
                            m_aktPunkt = pktTemp;
                            m_aktLinie = Linie::NeueLinie(m_aktPunkt, new Punkt(vPosition, aktLayer));
                            m_aktPosition = vPosition;
                        }
                    }else
                    {
                        if(!m_kreuzen)
                        {
                            m_aktPunkt->Positionieren(vPosition);
                            Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                            if(pktTemp)
                            {
                                m_aktLinie = Linie::NeueLinie(m_aktPunkt, pktTemp);
                                m_aktPunkt = pktTemp;
                                m_aktPosition = vPosition;
                            }else
                            {
                                m_aktPunkt = NULL;
                                m_aktLinie = NULL;
                            }
                        }
                    }
                }
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_Punkt)
            {
                if(aktLayer)
                {
                    if(!m_aktPunkt)
                    {
                        Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                        if(pktTemp)
                        {
                            m_aktPunkt = pktTemp;
                            m_aktLinie = Linie::NeueLinie(m_aktPunkt, static_cast<Punkt*>(markiertesObjekt));
                            markiertesObjekt->HolePosition(m_aktPosition);
                        }
                    }else
                    {
                        if(!m_kreuzen)
                        {
                            m_aktPunkt->ErsetzenDurch(static_cast<Punkt*>(markiertesObjekt));
                            m_aktPunkt = static_cast<Punkt*>(markiertesObjekt);
                            Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                            if(pktTemp)
                            {
                                m_aktLinie = Linie::NeueLinie(m_aktPunkt, pktTemp);
                                m_aktPunkt = pktTemp;
                                m_aktPosition = vPosition;
                            }else
                            {
                                m_aktPunkt = NULL;
                                m_aktLinie = NULL;
                            }
                        }
                    }
                }
            }
            else
            {
                if(!m_aktPunkt)
                {
                    m_aktPunkt = new Punkt(vPosition, aktLayer);
                }
                Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                if(pktTemp && m_aktPunkt)
                {
                    m_aktLinie = Linie::NeueLinie(m_aktPunkt, pktTemp);
                    m_aktPunkt = pktTemp;
                    m_aktPosition = vPosition;
                }else
                {
                    if(pktTemp)delete pktTemp;
                    if(m_aktPunkt)delete m_aktPunkt;
                    m_aktPunkt = NULL;
                    m_aktLinie = NULL;
                }
            }
        }
        else
        {
            if(aktLayer != NULL)
            {
                if(!m_aktPunkt)
                {
                    Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                    m_aktPunkt = new Punkt(vPosition, aktLayer);
                    if(pktTemp && m_aktPunkt)
                    {
                        m_aktLinie = Linie::NeueLinie(m_aktPunkt, pktTemp);
                        m_aktPunkt = pktTemp;
                        m_aktPosition = vPosition;
                    }else
                    {
                        if(pktTemp)delete pktTemp;
                        if(m_aktPunkt)delete m_aktPunkt;
                        m_aktPunkt = NULL;
                        m_aktLinie = NULL;
                    }
                }else
                {
                    Punkt* pktTemp = new Punkt(vPosition, aktLayer);
                    if(pktTemp)
                    {
                        m_aktLinie = Linie::NeueLinie(m_aktPunkt, pktTemp);
                        m_aktPunkt = pktTemp;
                        m_aktPosition = vPosition;
                    }else
                    {
                        m_aktPunkt = NULL;
                        m_aktLinie = NULL;
                    }
                }
            }
            else
            {
                wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
            }
        }
        break;

    case bef_ID_dreieckZeichnen:
        if(markiertesObjekt!=NULL)
        {
            if(markiertesObjekt->HoleTyp() == RUZ_Linie)
            {
                m_auswahl->Hinzufuegen(markiertesObjekt);
            }
            if(m_auswahl->GetListenGroesse() == 3)
            {
                Linie *t_l[3];
                t_l[0] = static_cast<Linie*> (m_auswahl->GetErstesElement());
                t_l[1] = static_cast<Linie*> (m_auswahl->GetNaechstesElement());
                t_l[2] = static_cast<Linie*> (m_auswahl->GetNaechstesElement());
                Dreieck::NeuesDreieck(t_l[0], t_l[1], t_l[2]);
                m_auswahl->ListeLeeren("");
            }
        }
        break;

    case bef_ID_viereckZeichnen:
        if(markiertesObjekt!=NULL)
        {
            if(m_auswahl->GetListenGroesse() != 0)
            {
                if(m_auswahl->GetErstesElement()->HoleTyp() == RUZ_Linie)
                {
                    if(markiertesObjekt->HoleTyp() == RUZ_Linie)
                    {
                        m_auswahl->Entzufuegen(markiertesObjekt);
                    }
                    if(m_auswahl->GetListenGroesse() == 4)
                    {
                        Linie *t_l[4];
                        t_l[0] = static_cast<Linie*> (m_auswahl->GetErstesElement());
                        t_l[1] = static_cast<Linie*> (m_auswahl->GetNaechstesElement());
                        t_l[2] = static_cast<Linie*> (m_auswahl->GetNaechstesElement());
                        t_l[3] = static_cast<Linie*> (m_auswahl->GetNaechstesElement());
                        Viereck::NeuesViereck(t_l[0], t_l[1], t_l[2], t_l[3]);
                        m_auswahl->ListeLeeren("");
                    }
                }else
                if(m_auswahl->GetErstesElement()->HoleTyp() == RUZ_Dreieck)
                {
                    if(markiertesObjekt->HoleTyp() == RUZ_Dreieck)
                    {
                        m_auswahl->Entzufuegen(markiertesObjekt);
                    }
                    if(m_auswahl->GetListenGroesse() == 2)
                    {
                        Dreieck *t_d[2];
                        t_d[0] = static_cast<Dreieck*> (m_auswahl->GetErstesElement());
                        t_d[1] = static_cast<Dreieck*> (m_auswahl->GetNaechstesElement());
                        Viereck::NeuesViereck(t_d[0], t_d[1]);
                        m_auswahl->ListeLeeren("");
                        markiertesObjekt = NULL;
                    }
                }
            }else{
                if((markiertesObjekt->HoleTyp() == RUZ_Dreieck && m_zeigeFlaeche && m_waehleFlaeche)
                   ||(markiertesObjekt->HoleTyp() == RUZ_Linie && m_zeigeLinie && m_waehleLinie))
                {
                    m_auswahl->Hinzufuegen(markiertesObjekt);
                }
            }
        }
        break;

    case bef_ID_fangpunkteFinden:
    case bef_ID_viereckTeilen:
    case bef_ID_flaechenVerschneiden:
        if(markiertesObjekt!=NULL)
        {
            m_auswahl->Entzufuegen(markiertesObjekt);
        }
        break;

    case bef_ID_hoehenMarkeZeichnen:
        if(aktLayer != NULL)
        {
            new HoehenMarke(vPosition, aktLayer);
        }else{
            wxMessageDialog(this, wxT("Kein Layer ist aktiv!"), wxT("Layer prüfen")).ShowModal();
        }
        break;

    case bef_ID_StreckeMessen:/*Noch Fangpunkte implementieren*/
        (this->*Messen)(vPosition.GetKoordinaten(aktProjX), vPosition.GetKoordinaten(aktProjY));
        break;

    case bef_ID_versetzen:
        if(markiertesObjekt!=NULL)
        {
            if(m_markierModus)
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
                break;
            }
            m_auswahl->ListeLeeren("");/*evtl. Überholt*/
            m_auswahl->Hinzufuegen(markiertesObjekt);

            KoordinatenMaske->SetzeKoordinaten(vPosition);
        }
        else
        {
            obj = m_auswahl->GetErstesElement();
            if(obj != NULL)
            {
                if(obj->HoleTyp() == RUZ_Punkt)
                {
                    Punkt* pkt = static_cast<Punkt*>(obj);
                    pkt->Positionieren(vPosition);
                    KoordinatenMaske->SetzeKoordinaten(vPosition);

                }else if(obj->HoleTyp() == RUZ_HoehenMarke)
                {
                    HoehenMarke* hm = static_cast<HoehenMarke*>(obj);
                    hm->Positionieren(vPosition);
                    KoordinatenMaske->SetzeKoordinaten(vPosition);
                }
            }
        }
        break;

    case bef_ID_punktEinfuegen:
        /*momentan ohne Funktion*/
        break;

    case bef_ID_linieParallel:
        if(m_markierModus)
        {
            if((markiertesObjekt!=NULL)&&(markiertesObjekt->HoleTyp() == RUZ_Linie))
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
            }
        }
        else
        {
            m_markierungsRechteck = false;
            if(m_auswahl->GetListenGroesse() != 0)
            {
                LinieParallel(vPosition);
            }
        }
        break;

    case bef_ID_linieExtrudieren:
        m_markierungsRechteck = false;
        if((markiertesObjekt!=NULL)&&(markiertesObjekt->HoleTyp() == RUZ_Linie))
        {
            m_auswahl->Entzufuegen(markiertesObjekt);
        }else
        {
            if(m_auswahl->GetListenGroesse() != 0)
            {
                LinieExtrudieren(vPosition);
            }
        }
        break;

    case bef_ID_punkteVereinigen:
        if(markiertesObjekt!=NULL)
        {
            if(markiertesObjekt->HoleTyp() == RUZ_Punkt)
            {
                (this->*Vereinigen)(static_cast<Punkt*>(markiertesObjekt));
            }
        }
        break;

    case bef_ID_punkteSkalieren:
        if(m_markierModus)
        {
            if(markiertesObjekt!=NULL)
            {
                m_auswahl->Entzufuegen(markiertesObjekt);
                break;
            }
        }else
        {
            SetStatusText(wxT("Skalierfaktoren eingeben"), 2);
            m_markierungsRechteck = false;
            SkalierungAusfuehren(vPosition);
            SetStatusText(wxT("Objekte zum Skalieren auswählen"), 2);
            m_markierModus = true;
        }
        break;

    case bef_ID_layerSkalieren:
    case bef_ID_hintergrundSkalieren:
    case bef_ID_allesSkalieren:
        {
        m_markierungsRechteck = false;
        Vektor t_Ort;
        if(!SkalierFaktorenEingabe())
        {
            BefehleZuruecksetzen();
            break;
        }
        LayerSkalieren(vPosition);
        break;
        }

    case bef_ID_SchnittPunktFlaeche:
        if(m_markierModus)
        {
            if(markiertesObjekt != NULL)
            {
                if(markiertesObjekt->HoleTyp() == RUZ_Punkt)
                {
                    m_schP_OrgPkt = static_cast<Punkt*>(markiertesObjekt);
                    m_schP_alterOrt = m_schP_OrgPkt->HolePosition();
                    if(m_schP_Dr)
                    {
                        if(!m_schP_Dr->IstEckPunkt(m_schP_OrgPkt))m_schP_OrgPkt = NULL;
                    }
                }
                if(markiertesObjekt->HoleTyp() == RUZ_Dreieck)
                {
                    m_schP_Dr = static_cast<Dreieck*>(markiertesObjekt);
                    if(m_schP_OrgPkt)
                    {
                        if(!m_schP_Dr->IstEckPunkt(m_schP_OrgPkt))m_schP_Dr = NULL;
                    }
                }
            }
        }
        else if(m_markierModus2)
        {
            if(markiertesObjekt != NULL)
            {
                if((markiertesObjekt->HoleTyp() == RUZ_Dreieck) || (markiertesObjekt->HoleTyp() == RUZ_Viereck))
                {
                    m_schP_Obj = markiertesObjekt;
                }
            }
        }
        else
        {
            SetStatusText(wxT("Schnittpunkt: Punkt und Linie wählen"), 2);
            m_markierModus = true;
            m_markierModus2 = false;
            m_schP_Dr = NULL;
            delete m_schP_Richtung_1;
            delete m_schP_Richtung_2;
            markiertesObjekt = NULL;
            m_schP_Ln = NULL;
            m_schP_Richtung_1 = m_schP_Richtung_2 = NULL;
            m_schP_Obj = NULL;
            m_schP_OrgPkt = NULL;
        }
        break;

    case bef_ID_SchnittPunktLinie:
        if(m_markierModus)
        {
            if(markiertesObjekt != NULL)
            {
                if(markiertesObjekt->HoleTyp() == RUZ_Punkt)
                {
                    m_schP_OrgPkt = static_cast<Punkt*>(markiertesObjekt);
                    m_schP_alterOrt = m_schP_OrgPkt->HolePosition();
                    if(m_schP_Ln)
                    {
                        if(!m_schP_Ln->IstEndPunkt(m_schP_OrgPkt))m_schP_OrgPkt = NULL;
                    }
                }
                if(markiertesObjekt->HoleTyp() == RUZ_Linie)
                {
                    m_schP_Ln = static_cast<Linie*>(markiertesObjekt);
                    if(m_schP_OrgPkt)
                    {
                        if(!m_schP_Ln->IstEndPunkt(m_schP_OrgPkt))m_schP_Ln = NULL;
                    }
                }
            }
        }
        else if(m_markierModus2)
        {
            if(markiertesObjekt != NULL)
            {
                if((markiertesObjekt->HoleTyp() == RUZ_Dreieck) || (markiertesObjekt->HoleTyp() == RUZ_Viereck))
                {
                    m_schP_Obj = markiertesObjekt;
                    if((static_cast<Flaeche*>(m_schP_Obj))->DurchstossPunkt(m_schP_Ln, m_vktSchnittPkt1, m_vktSchnittPkt2, true))
                    {
                        m_schP_OrgPkt->Positionieren(m_vktSchnittPkt1);
                    }else{
                        m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
                    }
                }else
                if(markiertesObjekt->HoleTyp() == RUZ_Linie)
                {
                    m_schP_Obj = markiertesObjekt;
                    Vektor tempOrt;
                    if(m_schP_Ln->schneidet((static_cast<Linie*>(m_schP_Obj)), tempOrt, aktProjZ, true))
                    {
                        m_schP_OrgPkt->Positionieren(tempOrt);
                    }else{
                        m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
                    }
                }
            }
        }
        break;

    default:
        break;
    }
    event.Skip();
    Refresh();
    return;
}

void RUZmBIFrame::OnMouseLeftUp(wxMouseEvent& event)
{
    wxClientDC dc(this);
    m_markierungsRechteck = false;
    NeueMousePosition = event.GetLogicalPosition(dc);
    double dPosX, dPosY;
    double d_N_MPosX, d_N_MPosY, d_A_MPosX, d_A_MPosY;
    d_N_MPosX = (NeueMousePosition.x / m_skalierung + dc_Offset[0]);
    d_N_MPosY = (NeueMousePosition.y / m_skalierung + dc_Offset[1]);
    d_A_MPosX = (AlteMousePosition.x / m_skalierung + dc_Offset[0]);
    d_A_MPosY = (AlteMousePosition.y / m_skalierung + dc_Offset[1]);
    /*Markierungsrechteck auswerten!*/
    if((aktBefehl == bef_ID_loeschen) ||
       (aktBefehl == bef_ID_nichts) ||
       ((aktBefehl == bef_ID_verschieben) && (m_markierModus)) ||
       ((aktBefehl == bef_ID_kopieren) && (m_markierModus)) ||
       ((aktBefehl == bef_ID_kopierenNachLayer) && (m_markierModus)) ||
       ((aktBefehl == bef_ID_drehen) && (m_markierModus)) ||
       ((aktBefehl == bef_ID_punkteSkalieren) && (m_markierModus)) ||
       ((aktBefehl == bef_ID_linieParallel)&&(m_markierModus)))
    {
        if(aktLayer != NULL)
        {
            RUZ_Layer* layer_laeufer = aktLayer;

            if(m_zeigePunkt && m_waehlePunkt)
            {
                Liste<Punkt>* pktLst = layer_laeufer->HolePunkte();
                for(Punkt* pktObj = pktLst->GetErstesElement(); pktObj != NULL; pktObj = pktLst->GetNaechstesElement())
                {
                    dPosX = pktObj->HolePosition().GetKoordinaten(aktProjX);
                    dPosY = pktObj->HolePosition().GetKoordinaten(aktProjY);
                    if(((dPosX - d_N_MPosX) * (dPosX - d_A_MPosX)) < 0)
                        if(((dPosY - d_N_MPosY) * (dPosY - d_A_MPosY)) < 0)
                            m_auswahl->Entzufuegen(pktObj);
                }
            }
            if(m_zeigeHoehenmarke && m_waehleHoehenmarke)
            {
                Liste<HoehenMarke>* hmLst = layer_laeufer->HoleHoehenMarken();
                for(HoehenMarke* hmObj = hmLst->GetErstesElement(); hmObj != NULL; hmObj = hmLst->GetNaechstesElement())
                {
                    dPosX = hmObj->HolePosition().GetKoordinaten(aktProjX);
                    dPosY = hmObj->HolePosition().GetKoordinaten(aktProjY);
                    if(((dPosX - d_N_MPosX) * (dPosX - d_A_MPosX)) < 0)
                        if(((dPosY - d_N_MPosY) * (dPosY - d_A_MPosY)) < 0)
                            m_auswahl->Entzufuegen(hmObj);
                }
            }
            if(m_zeigeFangpunkt && m_waehleFangpunkt)
            {
                Liste<Fangpunkt>* fpLst = layer_laeufer->HoleFangpunkte();
                for(Fangpunkt *fpObj = fpLst->GetErstesElement(); fpObj; fpObj = fpLst->GetNaechstesElement())
                {
                    dPosX = fpObj->HolePosition().GetKoordinaten(aktProjX);
                    dPosY = fpObj->HolePosition().GetKoordinaten(aktProjY);
                    if(((dPosX - d_N_MPosX) * (dPosX - d_A_MPosX)) < 0)
                        if(((dPosY - d_N_MPosY) * (dPosY - d_A_MPosY)) < 0)
                            m_auswahl->Entzufuegen(fpObj);
                }
            }
            if(m_zeigeKreis && m_waehleKreis)
            {
                double dRadius;
                Liste<Kreis>* krLst = layer_laeufer->HoleKreise();
                for(Kreis *krObj = krLst->GetErstesElement(); krObj; krObj = krLst->GetNaechstesElement())
                {
                    dPosX = krObj->HolePosition().GetKoordinaten(aktProjX);
                    dPosY = krObj->HolePosition().GetKoordinaten(aktProjY);
                    dRadius = krObj->HoleRadius();
                    if((((dPosX + dRadius - d_N_MPosX) * (dPosX + dRadius - d_A_MPosX)) < 0) && (((dPosY - d_N_MPosY) * (dPosY - d_A_MPosY)) < 0))
                        if((((dPosX - dRadius - d_N_MPosX) * (dPosX - dRadius - d_A_MPosX)) < 0) && (((dPosY - d_N_MPosY) * (dPosY - d_A_MPosY)) < 0))
                            if((((dPosX - d_N_MPosX) * (dPosX - d_A_MPosX)) < 0) && (((dPosY + dRadius - d_N_MPosY) * (dPosY + dRadius - d_A_MPosY)) < 0))
                                if((((dPosX - d_N_MPosX) * (dPosX - d_A_MPosX)) < 0) && (((dPosY - dRadius - d_N_MPosY) * (dPosY - dRadius - d_A_MPosY)) < 0))
                                    m_auswahl->Entzufuegen(krObj);
                }
            }

            if(m_kreuzen)
            {
                Liste<Linie>* lnLst = layer_laeufer->HoleLinien();
                if(m_zeigeLinie && m_waehleLinie)
                {
                    for(Linie* aktLinie = lnLst->GetErstesElement(); aktLinie != NULL; aktLinie = lnLst->GetNaechstesElement())
                    {
                        if(aktLinie->schneidet(d_A_MPosX, d_A_MPosY, d_N_MPosX, d_A_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktLinie);
                            continue;
                        }
                        if(aktLinie->schneidet(d_A_MPosX, d_A_MPosY, d_A_MPosX, d_N_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktLinie);
                            continue;
                        }
                        if(aktLinie->schneidet(d_A_MPosX, d_N_MPosY, d_N_MPosX, d_N_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktLinie);
                            continue;
                        }
                        if(aktLinie->schneidet(d_N_MPosX, d_A_MPosY, d_N_MPosX, d_N_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktLinie);
                            continue;
                        }
                    }
                }
            }
        }
    }else
    if((aktBefehl == bef_ID_viereckTeilen)||(aktBefehl == bef_ID_flaechenVerschneiden))
    {
        if(aktLayer != NULL)
        {
            RUZ_Layer* layer_laeufer = aktLayer;
            Liste<Flaeche>* flLst = layer_laeufer->HoleFlaechen();
            Punkt* aktPkt;
            int eckenInnerhalb, eckenZahl;

            if(m_zeigeFlaeche && m_waehleFlaeche)
            {
                for(Flaeche* aktFl = flLst->GetErstesElement(); aktFl != NULL; aktFl = flLst->GetNaechstesElement())
                {
                    eckenZahl = 4;
                    if(aktFl->HoleTyp() == RUZ_Dreieck)eckenZahl = 3;

                    eckenInnerhalb = 0;
                    for(int i = 0; i < eckenZahl; i++)
                    {
                        aktPkt = aktFl->HolePunkt(i);
                        if(((aktPkt->HolePosition().x() - d_N_MPosX) * (aktPkt->HolePosition().x() - d_A_MPosX)) < 0)
                        {
                            if(((aktPkt->HolePosition().y() - d_N_MPosY) * (aktPkt->HolePosition().y() - d_A_MPosY)) < 0)
                            {
                                if(m_kreuzen)
                                {
                                    eckenInnerhalb += eckenZahl;
                                    break;
                                }else{
                                    eckenInnerhalb++;
                                }
                            }
                        }
                    }
                    if(eckenInnerhalb >= eckenZahl)
                    {
                        m_auswahl->Entzufuegen(aktFl);
                    }else if(m_kreuzen)
                    {
                        if(aktFl->IstInnerhalb(d_A_MPosX, d_A_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktFl);
                            continue;
                        }
                        if(aktFl->IstInnerhalb(d_A_MPosX, d_N_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktFl);
                            continue;
                        }
                        if(aktFl->IstInnerhalb(d_N_MPosX, d_A_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktFl);
                            continue;
                        }
                        if(aktFl->IstInnerhalb(d_N_MPosX, d_N_MPosY, z))
                        {
                            m_auswahl->Entzufuegen(aktFl);
                            continue;
                        }
                    }
                }
            }
        }
    }
    /*ENDE Markierungsrechteck auswerten!*/
    event.Skip();
    Refresh();
    return;
}

void RUZmBIFrame::OnMouseMiddleClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    AlteMousePosition = event.GetLogicalPosition(dc);
    event.Skip();
    return;
}

void RUZmBIFrame::OnMouseMove(wxMouseEvent& event)
{
    wxClientDC dc(this);
    NeueMousePosition = event.GetLogicalPosition(dc);

    double dX = NeueMousePosition.x / m_skalierung + dc_Offset[0];
    double dY = -(NeueMousePosition.y / m_skalierung + dc_Offset[1]);
    SetStatusText(wxString::Format("Cursor (x/y): %.3f/%.3f", dX, dY), 3);

    if(event.Dragging() && event.MiddleIsDown())
    {
        dc_Offset[0] -= ((NeueMousePosition.x-AlteMousePosition.x)/m_skalierung);
        dc_Offset[1] -= ((NeueMousePosition.y-AlteMousePosition.y)/m_skalierung);

        AlteMousePosition = NeueMousePosition;
        Refresh();
        event.Skip();
        return;
    }
    if(event.Dragging() && event.LeftIsDown())
    {
        if(aktBefehl == bef_ID_verschieben)
        {
            if(m_markierModus)
            {
                if(markiertesObjekt != NULL)
                {
                    markiertesObjekt->Verschieben(Vektor(((NeueMousePosition.x-AlteMousePosition.x)/m_skalierung), ((NeueMousePosition.y-AlteMousePosition.y)/m_skalierung), 0));
                    m_auswahl->Entfernen(markiertesObjekt);
                    AlteMousePosition = NeueMousePosition;
                }
            }
            else
            {
                for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
                {
                    obj->Verschieben(Vektor(((NeueMousePosition.x-AlteMousePosition.x)/m_skalierung), ((NeueMousePosition.y-AlteMousePosition.y)/m_skalierung), 0));
                }
                if(vVerschubStart)
                {
                    delete vVerschubStart;
                    vVerschubStart = NULL;
                }
                AlteMousePosition = NeueMousePosition;
            }
        }
        Refresh();
        event.Skip();
        return;
    }else{
        if(aktBefehl == bef_ID_verschieben)
        {
            Vschb_Verschieben(Vektor(((NeueMousePosition.x)/m_skalierung) + dc_Offset[0], ((NeueMousePosition.y)/m_skalierung) + dc_Offset[1], 0));
        }
        if((aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
        {
            Kop_Verschieben(Vektor(((NeueMousePosition.x)/m_skalierung) + dc_Offset[0], ((NeueMousePosition.y)/m_skalierung) + dc_Offset[1], 0));
        }
    }

    MarkierMousePosition = NeueMousePosition;
    if(aktLayer != NULL)
    {
        if(aktBefehl == bef_ID_drehen)
        {
            (this->*DrehungMouseMove)(event);
        }
        if(aktBefehl == bef_ID_SchnittPunktFlaeche && !m_markierModus2)
        {
            if(m_schP_OrgPkt && m_schP_Dr && m_schP_Obj && m_schP_Richtung_2)
            {
                int objTyp = m_schP_Obj->HoleTyp();
                if(objTyp == RUZ_Dreieck || objTyp == RUZ_Viereck)
                {
                    m_schP_Richtung_2->Positionieren(Vektor((NeueMousePosition.x / m_skalierung) + dc_Offset[0],
                                                        (NeueMousePosition.y / m_skalierung) + dc_Offset[1], 0));
                    Vektor t_r2 = m_schP_Richtung_2->HolePosition();

                    if(m_normale.GetKoordinaten(2) != 0.0)
                    {
                        double t_z = (m_Abstand - m_normale.GetKoordinaten(0) * t_r2.GetKoordinaten(0)
                                      - m_normale.GetKoordinaten(1) * t_r2.GetKoordinaten(1)) / m_normale.GetKoordinaten(2);
                        t_r2.SetKoordinaten(2, t_z);
                        m_schP_Richtung_2->Positionieren(t_r2);
                        if((static_cast<Flaeche*>(m_schP_Obj))->DurchstossPunkt(m_schP_Ln, m_vktSchnittPkt1, m_vktSchnittPkt2, true))
                        {
                            m_schP_OrgPkt->Positionieren(m_vktSchnittPkt1);
                        }
                    }
                }
            }
        }
        if(m_aktKreis)
        {
            Vektor t_vkt = Vektor(((NeueMousePosition.x) / m_skalierung) + dc_Offset[0],
                                     ((NeueMousePosition.y) / m_skalierung) + dc_Offset[1], 0.0);
            SetStatusText(wxString::Format("Kreis aktiv - Mouse bei %.3f - %.3f", t_vkt.x(), t_vkt.y()), 1);
            KreisPunktEingabe(t_vkt, false);
        }
        if(m_aktPunkt)
        {
            Vektor t_vkt;
            t_vkt.SetKoordinaten(aktProjX, NeueMousePosition.x / m_skalierung + dc_Offset[0]);
            t_vkt.SetKoordinaten(aktProjY, NeueMousePosition.y / m_skalierung + dc_Offset[1]);
            if(KoordinatenMaske->IsShown())
            {
                t_vkt.SetKoordinaten(aktProjZ, KoordinatenMaske->HoleKoordinaten(aktProjZ));
            }else
            {
                t_vkt.SetKoordinaten(aktProjZ, 0.0);
            }
            if(markiertesObjekt)
            {
                markiertesObjekt->HolePosition(t_vkt);
            }

            if(m_kreuzen && aktBefehl == bef_ID_linieZeichnen)
            {
                double dProjektionslaenge[2];
                Vektor vTemp1;
                vTemp1 = (t_vkt - m_aktPosition);

                Vektor vCaRichtung(1, 0, 0);
                dProjektionslaenge[0] = vTemp1 * vCaRichtung;

                Vektor vAndereRichtung(0, 1, 0);
                dProjektionslaenge[1] = vTemp1 * vAndereRichtung;

                if(abs(dProjektionslaenge[0]) < abs(dProjektionslaenge[1]))
                {
                    vCaRichtung = vAndereRichtung;
                    dProjektionslaenge[0] = dProjektionslaenge[1];
                }
                t_vkt = vCaRichtung * dProjektionslaenge[0];
                if(KoordinatenMaske->IsShown())
                {
                    t_vkt.SetKoordinaten(aktProjZ, KoordinatenMaske->HoleKoordinaten(aktProjZ));
                }else
                {
                    t_vkt.SetKoordinaten(aktProjZ, 0.0);
                }
                t_vkt += m_aktPosition;
            }
            m_aktPunkt->Positionieren(t_vkt);
        }

        if(ObjektMarkieren(NeueMousePosition.x, NeueMousePosition.y))
        {
            if(gefaelleAnzeigen && (markiertesObjekt != NULL))
            {
                Vektor t_vkt((NeueMousePosition.x / m_skalierung) + dc_Offset[0],
                             (NeueMousePosition.y / m_skalierung) + dc_Offset[1], 0);
                if(markiertesObjekt->HoleTyp() == RUZ_Dreieck || markiertesObjekt->HoleTyp() == RUZ_Viereck)
                {
                    if(!(static_cast<Flaeche*>(markiertesObjekt))->Gefaelle(t_vkt, m_aktGefaelle, aktProjZ))
                    {
                        m_aktGefaelle = NULL_VEKTOR;
                    }
                }
            }
        }else
        {
            if(gefaelleAnzeigen)m_aktGefaelle = NULL_VEKTOR;
        }
    }
    event.Skip();
    Refresh();
    return;
}

void RUZmBIFrame::OnMouseRightClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    RUZ_Objekt **objAktFang;
    if((markiertesObjekt)&&(m_kreuzen))
    {
        objFang1 != NULL ? objAktFang = &objFang2 : objAktFang = &objFang1;
        if(markiertesObjekt->HoleTyp() == RUZ_Linie)*objAktFang = markiertesObjekt;
        else if(markiertesObjekt->HoleTyp() == RUZ_Kreis)*objAktFang = markiertesObjekt;

        if(objFang2)
        {
            FangpunkteFinden(objFang1, objFang2);
            objFang1 = objFang2 = NULL;
        }
    }
    AlteMousePosition = event.GetLogicalPosition(dc);
    event.Skip();
    return;
}

void RUZmBIFrame::OnMouseRDClick(wxMouseEvent& event)
{
    if(aktBefehl == bef_ID_loeschen)
    {
        AuswahlLoeschen();
        SetStatusText(wxT("Objekte gelöscht"), 2);
    }else
    if(aktBefehl == bef_ID_verschieben)
    {
        Vschb_Auswahl_Bestaetigung();
    }else
    if((aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
    {
        Kop_Auswahl_Bestaetigung();
    }else
    if(aktBefehl == bef_ID_drehen)
    {
        if(m_markierModus)
        {
            DrehungBestaetigung();
            SetStatusText(wxT("Festpunkt der Drehung setzen"), 2);
        }
    }else
    if(aktBefehl == bef_ID_punkteSkalieren)
    {
        if(m_markierModus)
        {
            AuswahlAufPunkteReduzieren();
            m_markierModus = false;
            KoordinatenMaske->Show();
            SetStatusText(wxT("Festpunkt der Skalierung wählen"), 2);
        }
        else
        {
            m_markierModus = true;
            KoordinatenMaske->Show(false);
            SetStatusText(wxT("Objekte zum Skalieren auswählen"), 2);
        }
    }else
    if(aktBefehl == bef_ID_viereckTeilen)
    {
        ViereckeAuswahlTeilen();
    }else
    if(aktBefehl == bef_ID_flaechenVerschneiden)
    {
        VerschneideAuswahlFlaechen();
    }else
    if(aktBefehl == bef_ID_SchnittPunktFlaeche)
    {
        SchnittpunktFlaecheAbschliessen();
    }else
    if(aktBefehl == bef_ID_SchnittPunktLinie)
    {
        SchnittpunktLinieAbschliessen();
    }else
    if(aktBefehl == bef_ID_fangpunkteFinden)
    {
        FangpunkteFinden(m_auswahl);
        m_auswahl->ListeLeeren("");
    }else
    if(aktBefehl == bef_ID_linieParallel)
    {
        if(m_markierModus)m_markierModus = false;
        else m_markierModus = true;
    }
    //BefehleZuruecksetzen();
    Refresh();
    event.Skip();
    return;
}

void RUZmBIFrame::OnMouseWheel(wxMouseEvent& event)
{
    wxClientDC dc(this);
    wxPoint MousePosition = event.GetLogicalPosition(dc);
    double alteSkalierung = m_skalierung;
    double verschub = 0.0;
    if(event.GetWheelRotation() < 0)
    {
        verschub = (m_verschubWeite * wertFkt);
    }
    if(event.GetWheelRotation() > 0)
    {
        verschub = -(m_verschubWeite * wertFkt);
    }
    if(anzeigeSkalieren)
    {
       if(KoordinatenMaske->IsShown() && !m_markierModus)
        {
            KoordinatenMaske->SetzeKoordinaten(z, KoordinatenMaske->HoleKoordinaten(aktProjZ) + verschub);
        }
        if((aktBefehl == bef_ID_verschieben)||(aktBefehl == bef_ID_kopieren)||(aktBefehl == bef_ID_kopierenNachLayer))
        {
            for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
            {
                obj->Verschieben(Vektor(0, 0, verschub));
            }
        }
    }else
    {
		if(event.GetWheelRotation() < 0)
		{
			SkalierungSetzen(m_skalierung / (1 + 0.1 * wertFkt));
		}
        if(event.GetWheelRotation() > 0)
		{
			SkalierungSetzen(m_skalierung * (1 + 0.1 * wertFkt));
        }

        dc_Offset[0] += MousePosition.x * ((1/alteSkalierung)-(1/m_skalierung));
        dc_Offset[1] += MousePosition.y * ((1/alteSkalierung)-(1/m_skalierung));
        SetStatusText(wxString::Format("Offset: %1.5f - %1.5f / Skalierung: %5.5f", dc_Offset[0], dc_Offset[1], m_skalierung), 1);
    }
    Refresh();
    event.Skip();
    return;
}

void RUZmBIFrame::OnObjekteWaehlen(wxCommandEvent& event)
{
    int aktID = event.GetId();
    switch(aktID)
    {
    case idWaehleBogen:
        m_waehleBogen == false ? m_waehleBogen = true : m_waehleBogen = false;
        break;
    case idWaehleFlaeche:
        m_waehleFlaeche == false ? m_waehleFlaeche = true : m_waehleFlaeche = false;
        break;
    case idWaehleHoehenmarke:
        m_waehleHoehenmarke == false ? m_waehleHoehenmarke = true : m_waehleHoehenmarke = false;
        break;
    case idWaehleLinie:
        m_waehleLinie == false ? m_waehleLinie = true : m_waehleLinie = false;
        break;
    case idWaehlePunkt:
        m_waehlePunkt == false ? m_waehlePunkt = true : m_waehlePunkt = false;
        break;
    case idWaehleStrich:
        m_waehleStrich == false ? m_waehleStrich = true : m_waehleStrich = false;
        break;
    case idWaehleKreis:
        m_waehleKreis == false ? m_waehleKreis = true : m_waehleKreis = false;
        break;
    case idWaehleFangpunkt:
        m_waehleFangpunkt == false ? m_waehleFangpunkt = true : m_waehleFangpunkt = false;
        break;
    default:
        ;
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnObjekteZeigen(wxCommandEvent& event)
{
    int aktID = event.GetId();
    switch(aktID)
    {
    case idZeigeBogen:
        m_zeigeBogen == false ? m_zeigeBogen = true : m_zeigeBogen = false;
        break;
    case idZeigeFlaeche:
        m_zeigeFlaeche == false ? m_zeigeFlaeche = true : m_zeigeFlaeche = false;
        break;
    case idZeigeHoehe:
        m_zeigeHoehe == false ? m_zeigeHoehe = true : m_zeigeHoehe = false;
        break;
    case idZeigeHoehenmarke:
        m_zeigeHoehenmarke == false ? m_zeigeHoehenmarke = true : m_zeigeHoehenmarke = false;
        break;
    case idZeigeLinie:
        m_zeigeLinie == false ? m_zeigeLinie = true : m_zeigeLinie = false;
        break;
    case idZeigePunkt:
        m_zeigePunkt == false ? m_zeigePunkt = true : m_zeigePunkt = false;
        break;
    case idZeigeStrich:
        m_zeigeStrich == false ? m_zeigeStrich = true : m_zeigeStrich = false;
        break;
    case idZeigeKreis:
        m_zeigeKreis == false ? m_zeigeKreis = true : m_zeigeKreis = false;
        break;
    case idZeigeFangpunkt:
        m_zeigeFangpunkt == false ? m_zeigeFangpunkt = true : m_zeigeFangpunkt = false;
        break;
    default:
        ;
    }
    Refresh();
    return;
}

void RUZmBIFrame::OnOpenFile(wxCommandEvent &event)
{
    if(event.GetId() == idMenuRuzImport)
    {
        FileOpener->SetWildcard(wxT("RUZ-Datei (*.ruz)|*.ruz"));
    }else if(event.GetId() == idMenuD45Import)
    {
        FileOpener->SetWildcard(wxT("D45-Datei (*.d45)|*.d45"));
    }else if(event.GetId() == idMenuD58Import)
    {
        FileOpener->SetWildcard(wxT("D58-Datei (*.d58)|*.d58"));
    }else{

        FileOpener->SetWildcard(wxT("DXF-Datei (*.dxf)|*.dxf"));
    }
    int Rueckgabe = FileOpener->ShowModal();
    if(Rueckgabe==wxID_CANCEL)return;
    SetStatusText(FileOpener->GetPath(), 1);

    /*DXF Einlesen*/
    if((FileOpener->GetPath()).EndsWith(wxT("f"))||(FileOpener->GetPath()).EndsWith(wxT("F")))/*Wenn dxf ausgewählt wurde*/
    {
        Rueckgabe = dxfParameterDlg->ShowModal();

        //abteil = NULL;
        char x_kennung[64], y_kennung[64], z_kennung[64], sw_kennung[64];
        dxfParameterDlg->HoleKennung(x_kennung, IDpktXKenn);
        dxfParameterDlg->HoleKennung(y_kennung, IDpktYKenn);
        dxfParameterDlg->HoleKennung(z_kennung, IDpktZKenn);
        dxfParameterDlg->HoleKennung(sw_kennung, IDpktSW);

        DXF_Import *dxfImporteur = new DXF_Import((char*)static_cast<const char*>(FileOpener->GetPath().c_str()),
                                                         x_kennung, y_kennung, z_kennung, sw_kennung);
        Liste<Char_Speicher>* layerNamenListe = dxfImporteur->HoleLayerNamen();
        DXF_Import_Auswahl_Dialog(this, layerNamenListe).ShowModal();

        if(event.GetId() == idMenuDxfImp_mitLay_Ln_Pkt)
        {
            if(!dxfImporteur->EinlesenPunkteKanten(m_layer, layerNamenListe))
            {
                wxMessageDialog(this, wxString::FromUTF8("Beim Einlesen ist ein Fehler aufgetreten.\nEvtl. wurde nicht alles eingelesen")).ShowModal();
            }
            aktLayer = m_layer->GetErstesElement();
        }else
        if(event.GetId() == idMenuDxfImp_mitLay_Pkt)
        {
            if(!dxfImporteur->EinlesenPunkte(m_layer, layerNamenListe))
            {
                wxMessageDialog(this, wxString::FromUTF8("Beim Einlesen ist ein Fehler aufgetreten.\nEvtl. wurde nicht alles eingelesen")).ShowModal();
            }
            aktLayer = m_layer->GetErstesElement();
        }else
        if((event.GetId() == idMenuDxfImp_ohneLay_Ln_Pkt)||(event.GetId() == idMenuDxfImp_ohneLay_Pkt))
        {
            wxString t_Pfad = FileOpener->GetPath().c_str();
            int t_pos = t_Pfad.Find('.', true);
            t_Pfad = t_Pfad.Left(t_pos);
            t_pos = t_Pfad.Find('\\', true);
            t_Pfad = t_Pfad.Right(t_Pfad.Len() - t_pos -1);

            aktLayer = NULL;
            aktLayer = new RUZ_Layer((char*)static_cast<const char*>(t_Pfad));
            if(aktLayer!=NULL)
            {
                m_layer->Hinzufuegen(aktLayer);
                LayerAuswahl->LayerHinzufuegen(wxString(aktLayer->HoleName()), aktLayer);

                if(event.GetId() == idMenuDxfImp_ohneLay_Ln_Pkt)
                {
                    if(!dxfImporteur->EinlesenPunkteKanten(aktLayer, layerNamenListe))
                    {
                        wxMessageDialog(this, wxString::FromUTF8("Beim Einlesen ist ein Fehler aufgetreten.\nEvtl. wurde nicht alles eingelesen")).ShowModal();
                    }
                }else
                if(event.GetId() == idMenuDxfImp_ohneLay_Pkt)
                {
                    if(!dxfImporteur->EinlesenPunkte(aktLayer, layerNamenListe))
                    {
                        wxMessageDialog(this, wxString::FromUTF8("Beim Einlesen ist ein Fehler aufgetreten.\nEvtl. wurde nicht alles eingelesen")).ShowModal();
                    }
                }
            }
        }
        LayerauswahlAktualisieren();
        AusdehnungFinden();
        delete dxfImporteur;
    }/*ENDE DXF Einlesen*/
    else if((FileOpener->GetPath()).EndsWith(wxT("45")))/*m_zeigeStrichWenn d45 ausgewählt wurde*/
    {
        LeseAusD45((char*)static_cast<const char*>(FileOpener->GetPath().c_str()));
        if(aktLayer!=NULL)
        {
            AusdehnungFinden();
        }
    }
    else if((FileOpener->GetPath()).EndsWith(wxT("58")))/*Wenn d45 ausgewählt wurde*/
    {
        LeseAusD58((char*)static_cast<const char*>(FileOpener->GetPath().c_str()));
        if(aktLayer!=NULL)
        {
            AusdehnungFinden();
        }
    }
    else/*Ansonsten ist es eine ruz-datei*/
    {
        LeseAusRUZ((char*)static_cast<const char*>(FileOpener->GetPath().c_str()));
        if(aktLayer!=NULL)
        {
            AusdehnungFinden();
        }
    }
    AuswahlLeeren();
    markiertesObjekt = NULL;
    Refresh();
    return;
}

void RUZmBIFrame::OnPaint(wxPaintEvent &event)
{
    wxClientDC CL_dc(this);
    if(CL_dc.GetSize().GetHeight() == 0)return;
    if(CL_dc.GetSize().GetWidth() == 0)return;

    wxBufferedPaintDC dc(this);

    RUZ_Layer* tempLayer;
    wxColor loc_col_Hoehenlinie;
    unsigned char r, g, b;
    int posX, posY;
	
    Strich* tempStrich;
    Flaeche* tempFlaeche;
    Liste<Strich>* strSammlung;
    wxPoint dP[4];
    int anzEcken;

    /*Hintergrund zeichnen*/
    wxRect rect(wxPoint(0, 0), CL_dc.GetSize());
    dc.SetBrush(wxBrush(col_ZeichenHintergrund));
    dc.SetPen(wxPen(col_ZeichenHintergrund, 1));
    dc.DrawRectangle(rect);

    if(lwBild.ucLeinwand)//Wenn die Leinwand Daten enthaelt, zeichnen!
    {
        if(lwBild.dSkalierung)
        {
            wxImage imBild = wxImage(lwBild.iBreite, lwBild.iHoehe, lwBild.ucLeinwand, true);

            double oleX, oleY, ureX, ureY;//obere Linke und untere rechte Ecke des Zuschnitts
            oleX = (lwBild.dOffsetX > dc_Offset[0]) ? 0.0 : (dc_Offset[0] - lwBild.dOffsetX);
            oleY = (lwBild.dOffsetY > dc_Offset[1]) ? 0.0 : (dc_Offset[1] - lwBild.dOffsetY);

            ureX = (lwBild.iBreite / lwBild.dSkalierung);
            if(ureX > dc_Offset[0]+ CL_dc.GetSize().GetWidth() / m_skalierung - lwBild.dOffsetX)
                ureX = dc_Offset[0] + CL_dc.GetSize().GetWidth() / m_skalierung - lwBild.dOffsetX ;

            ureY = (lwBild.iHoehe / lwBild.dSkalierung);
            if(ureY > dc_Offset[1] + CL_dc.GetSize().GetHeight() / m_skalierung - lwBild.dOffsetY)
                ureY = dc_Offset[1] + CL_dc.GetSize().GetHeight() / m_skalierung - lwBild.dOffsetY;

            if((oleX < ureX)&&(oleY < ureY))
            {
                int iB, iH;
                iB = (ureX-oleX)*lwBild.dSkalierung;
                iH = (ureY-oleY)*lwBild.dSkalierung;
                if(iB!=0 && iH!=0)
                {
                    imBild = imBild.Resize(wxSize(iB, iH), wxPoint(-(oleX)*lwBild.dSkalierung, -(oleY)*lwBild.dSkalierung), 128, 0,76);
                    iB = (ureX-oleX)*m_skalierung;
                    iH = (ureY-oleY)*m_skalierung;
                    if(iB!=0 && iH!=0)
                    {
                        imBild.Rescale(iB, iH);
                        dc.DrawBitmap(wxBitmap(imBild, dc), (lwBild.dOffsetX + oleX - dc_Offset[0])*m_skalierung,
							(lwBild.dOffsetY + oleY - dc_Offset[1])*m_skalierung);
                    }
                }
            }
        }
    }

    if(aktLayer)
    {
        SetStatusText(wxT("Aktiver Layer: " + wxString::FromUTF8(aktLayer->HoleName())));
        if(m_zeigeFlaeche)aktLayer->FlaechenAktualisieren(hlAnzeigen);
    }else{
        SetStatusText("Kein aktiver Layer");
    }

    if((aktuelleAnsicht == ansicht_ID_normal)||(aktuelleAnsicht == ansicht_ID_hoehenkarte))
    {
        /*Farbige Flächen malen*/
        if(aktLayer)
        {
            if(m_zeigeFlaeche)
            {
                Liste<Flaeche> *flLst = aktLayer->HoleFlaechen();
                for(Flaeche *aktFl = flLst->GetErstesElement(); aktFl; aktFl = flLst->GetNaechstesElement())
                {
                    unsigned char r, g, b;
                    if(aktFl->HoleFarbe(r, g, b))
                    {
                        dc.SetPen(wxPen(*wxTRANSPARENT_PEN));
                        dc.SetBrush(wxBrush(wxColour(r, g, b)));
                        if(aktFl->HoleTyp() == RUZ_Dreieck)
                        {
                            Dreieck* obj = static_cast<Dreieck*> (aktFl);
                            if(obj)
                            {
                                for(int k=0; k<3; k++)
                                {
                                    dP[k] = wxPoint((int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                                    (int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                                }
                                dc.DrawPolygon(3, dP, 0, 0, wxODDEVEN_RULE);
                            }
                        }else
                        if(aktFl->HoleTyp() == RUZ_Viereck)
                        {
                            Viereck* obj = static_cast<Viereck*> (aktFl);
                            if(obj)
                            {
                                for(int k=0; k<4; k++)
                                {
                                    dP[k] = wxPoint((int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                                    (int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                                }
                                dc.DrawPolygon(4, dP, 0, 0, wxODDEVEN_RULE);
                            }
                        }
                    }
                }
            }
        }
        /*ENDE Farbige Flächen malen*/

        /*Markierung Malen - Flaechen*/
        dc.SetPen(wxPen(col_markiert_Obj, 5));
        dc.SetBrush(wxBrush(col_markiert_Obj));
        if(markiertesObjekt)
        {
            if(markiertesObjekt->HoleTyp() == RUZ_Dreieck && m_zeigeFlaeche)
            {
                Dreieck* obj = static_cast<Dreieck*> (markiertesObjekt);
                if(obj)
                {
                    for(int k=0; k<3; k++)
                    {
                        dP[k] = wxPoint((int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                        (int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                    }
                    dc.DrawPolygon(3, dP, 0, 0, wxODDEVEN_RULE);
                }
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_Viereck  && m_zeigeFlaeche)
            {
                Viereck* obj = static_cast<Viereck*> (markiertesObjekt);
                if(obj)
                {
                    for(int k=0; k<4; k++)
                    {
                        dP[k] = wxPoint((int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                        (int)((obj->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                    }
                    dc.DrawPolygon(4, dP, 0, 0, wxODDEVEN_RULE);
                }
            }
        }
        /*ENDE Markierung Malen - Flaechen*/

        /*Auswahl malen*/
        dc.SetPen(wxPen(col_ausgewaehlt_Obj, 3));
        dc.SetBrush(wxBrush(col_ausgewaehlt_Obj));

        Liste<RUZ_Objekt> *t_auswahl = m_auswahl;
        if(m_kopierAuswahl->GetListenGroesse() != 0)
            t_auswahl = m_kopierAuswahl;

        for(RUZ_Objekt* RUZobj = t_auswahl->GetErstesElement(); RUZobj != NULL; RUZobj = t_auswahl->GetNaechstesElement())
        {
            Punkt* objPkt;
            HoehenMarke* objHM;
            Linie* objLn;
            Kreis* objKr;
            Fangpunkt* objFP;
            Dreieck* objDr;
            Viereck* objVck;

            int objTyp = RUZobj->HoleTyp();
            switch(objTyp)
            {
            case RUZ_Punkt:
                objPkt = static_cast<Punkt*> (RUZobj);
                dc.DrawCircle((objPkt->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                          (objPkt->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, pxSuchEntfernung);
                break;
            case RUZ_HoehenMarke:
                objHM = static_cast<HoehenMarke*> (RUZobj);
                dc.DrawCircle((objHM->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                          (objHM->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, (int)(pxSuchEntfernung/3));
                break;
            case RUZ_Kreis:
                dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
                objKr = static_cast<Kreis*> (RUZobj);
                dc.DrawCircle((objKr->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                                  (objKr->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                                  (int)(objKr->HoleRadius()*m_skalierung));
                dc.SetBrush(wxBrush(col_ausgewaehlt_Obj));
                break;
            case RUZ_Fangpunkt:
                objFP = static_cast<Fangpunkt*>(RUZobj);
                posX = (objFP->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
                posY = (objFP->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
                {
                    int symbolGroesse = 1.2f * pxSuchEntfernung;
                    dc.DrawLine(posX - symbolGroesse, posY - symbolGroesse, posX + symbolGroesse, posY + symbolGroesse);
                    dc.DrawLine(posX + symbolGroesse, posY - symbolGroesse, posX - symbolGroesse, posY + symbolGroesse);
                }
                break;
            case RUZ_Linie:
                objLn = static_cast<Linie*> (RUZobj);
                dc.DrawLine((objLn->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (objLn->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                            (objLn->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (objLn->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
                break;
            case RUZ_Dreieck:
                objDr = static_cast<Dreieck*> (RUZobj);
                for(int k=0; k<3; k++)
                {
                    dP[k] = wxPoint((int)((objDr->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                    (int)((objDr->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                }
                dc.DrawPolygon(3, dP, 0, 0, wxODDEVEN_RULE);
                break;
            case RUZ_Viereck:
                objVck = static_cast<Viereck*> (RUZobj);
                for(int k=0; k<4; k++)
                {
                    dP[k] = wxPoint((int)((objVck->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                    (int)((objVck->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                }
                dc.DrawPolygon(4, dP, 0, 0, wxODDEVEN_RULE);
                break;
            default:
                break;
            }
        }
        if(m_schP_OrgPkt)
        {
            dc.DrawCircle((m_schP_OrgPkt->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                          (m_schP_OrgPkt->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, pxSuchEntfernung);
        }
        if(m_schP_Ln)
        {
            dc.DrawLine((m_schP_Ln->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                        (m_schP_Ln->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                        (m_schP_Ln->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                        (m_schP_Ln->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
        }
        /*ENDE Auswahl malen*/

        /*Schnittpunkt Flächen malen*/
        dc.SetPen(wxPen(col_markiert_Obj, 3));
        dc.SetBrush(wxBrush(col_markiert_Obj));
        if(m_schP_Dr)
        {
            for(int k=0; k<3; k++)
                {
                    dP[k] = wxPoint((int)((m_schP_Dr->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                    (int)((m_schP_Dr->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                }
                dc.DrawPolygon(3, dP, 0, 0, wxODDEVEN_RULE);
        }
        if(m_schP_Obj)
        {
            Flaeche* objFl;
            Linie* objLn;

            int objTyp = m_schP_Obj->HoleTyp();
            switch(objTyp)
            {
            case RUZ_Dreieck:
            case RUZ_Viereck:
                objTyp == RUZ_Dreieck ? anzEcken = 3 : anzEcken = 4;
                objFl = static_cast<Flaeche*> (m_schP_Obj);
                for(int k=0; k<anzEcken; k++)
                    {
                        dP[k] = wxPoint((int)((objFl->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                        (int)((objFl->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                    }
                dc.DrawPolygon(anzEcken, dP, 0, 0, wxODDEVEN_RULE);
                break;
            case RUZ_Linie:
                objLn = static_cast<Linie*> (m_schP_Obj);
                dc.DrawLine((objLn->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (objLn->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                            (objLn->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (objLn->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
                            break;
            default:
                break;
            }
        }
        /*ENDE Schnittpunkt Flächen malen*/

        /*Hintergrundlayer malen*/
        if(m_hintergrundMalen)
        {
            dc.SetPen(wxPen(col_HintergrundLayer, 1));
            dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));

            strSammlung = m_hintergrundLayer->HoleStriche();
            Liste<Bogen>* bogSammlung = m_hintergrundLayer->HoleBoegen();
            if(m_zeigeStrich)
            {
                for(tempStrich = strSammlung->GetErstesElement(); tempStrich != NULL; tempStrich = strSammlung->GetNaechstesElement())
                {
                    dc.DrawLine((tempStrich->Xa()-dc_Offset[0])*m_skalierung, (tempStrich->Ya()-dc_Offset[1])*m_skalierung,
                                (tempStrich->Xe()-dc_Offset[0])*m_skalierung, (tempStrich->Ye()-dc_Offset[1])*m_skalierung);
                }
            }
            if(m_zeigeBogen)
            {
                for(Bogen* tempBogen = bogSammlung->GetErstesElement(); tempBogen != NULL; tempBogen = bogSammlung->GetNaechstesElement())
                {
                    dc.DrawArc((tempBogen->Xa()-dc_Offset[0])*m_skalierung, (tempBogen->Ya()-dc_Offset[1])*m_skalierung,
                               (tempBogen->Xe()-dc_Offset[0])*m_skalierung, (tempBogen->Ye()-dc_Offset[1])*m_skalierung,
                               (tempBogen->Xm()-dc_Offset[0])*m_skalierung, (tempBogen->Ym()-dc_Offset[1])*m_skalierung);
                }
            }
        }
        /*ENDE Hintergrundlayer malen*/

        /*Alle Layer malen*/
        for(tempLayer = m_layer->GetErstesElement(); tempLayer != NULL; tempLayer = m_layer->GetNaechstesElement())
        {
			if(tempLayer == aktLayer)continue;
            LayerMalen(dc, tempLayer);
        }
		if(aktLayer)
		{
            LayerMalen(dc, aktLayer);
		}
        /*ENDE Alle Layer malen*/

        /*Fehler malen*/
        Liste<Vektor>* vFehlerLst = aktLayer->HoleFehlermarken();
        for(Vektor* vFehler = vFehlerLst->GetErstesElement(); vFehler != NULL; vFehler = vFehlerLst->GetNaechstesElement())
        {
            dc.SetPen(wxPen(wxColor(255, 110, 110), 3));
            dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));

            dc.DrawCircle((vFehler->GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                          (vFehler->GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, pxSuchEntfernung * 1.5);
        }
        Liste<Linie>* lnLst = aktLayer->HoleLinien();
        for(Linie* lnAkt = lnLst->GetErstesElement(); lnAkt != NULL; lnAkt = lnLst->GetNaechstesElement())
        {
            unsigned char r, g, b;
            if(lnAkt->HoleFarbe(r, g, b))
            {
                dc.SetPen(wxPen(wxColor(r, g, b), 3));
                dc.DrawLine((lnAkt->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (lnAkt->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                            (lnAkt->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (lnAkt->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
            }
        }
        /*ENDE Fehler malen*/

        /*Markierung Malen*/
        dc.SetPen(wxPen(col_markiert_Obj, 1));
        dc.SetBrush(wxBrush(col_markiert_Obj));
        if(markiertesObjekt)
        {
            if(markiertesObjekt->HoleTyp() == RUZ_Punkt && m_zeigePunkt)
            {
                Punkt* obj = static_cast<Punkt*>(markiertesObjekt);
                if(obj)
                {
                    dc.DrawCircle((obj->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                              (obj->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, pxSuchEntfernung);
                    if(m_zeigeHoehe)
                    {
                        dc.SetTextForeground(col_markiert_Obj);
                        wxString genauigkeit = wxString::Format("%1.%df", m_anzeigeGenauigkeit);
                        wxFont fnt = dc.GetFont();
                        int nFntSize = fnt.GetPointSize();
                        fnt.SetPointSize((int)(nFntSize * 1.75f));
                        dc.SetFont(fnt);
                        dc.DrawText(wxString::Format(genauigkeit, obj->HolePosition().GetKoordinaten(aktProjZ)),
                                    (obj->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung + 1.2f * pxSuchEntfernung,
                                    (obj->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung - nFntSize * 4.0f);
                        fnt.SetPointSize(nFntSize);
                        dc.SetFont(fnt);
                    }
                }
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_Kreis && m_zeigeKreis)
            {
                dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
                Kreis* objKr = static_cast<Kreis*> (markiertesObjekt);
                if(objKr->HoleRadius()>0)
                    dc.DrawCircle((objKr->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                                      (objKr->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                                      (int)(objKr->HoleRadius()*m_skalierung));
                dc.SetBrush(wxBrush(col_markiert_Obj));
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_Linie && m_zeigeLinie)
            {
                Linie* obj = static_cast<Linie*> (markiertesObjekt);
                if(obj)
                {
                    dc.DrawLine((obj->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (obj->HolePunkt(0)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung,
                                (obj->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (obj->HolePunkt(1)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
                }
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_HoehenMarke && m_zeigeHoehenmarke)
            {
                HoehenMarke* obj = static_cast<HoehenMarke*> (markiertesObjekt);
                if(obj)
                {
                    dc.DrawCircle((obj->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung,
                              (obj->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung, (int)(pxSuchEntfernung/3));
                    dc.SetTextForeground(col_markiert_Obj);
                    wxString genauigkeit = wxString::Format("%1.%df", m_anzeigeGenauigkeit);
                    wxFont fnt = dc.GetFont();
                    int nFntSize = fnt.GetPointSize();
                    fnt.SetPointSize((int)(nFntSize * 1.75f));
                    dc.SetFont(fnt);
                    dc.DrawText(wxString::Format(genauigkeit, obj->HolePosition().GetKoordinaten(aktProjZ)),
                                (obj->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung + 1.2f * pxSuchEntfernung,
                                (obj->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung - nFntSize * 4.0f);
                    fnt.SetPointSize(nFntSize);
                    dc.SetFont(fnt);
                }
            }else
            if(markiertesObjekt->HoleTyp() == RUZ_Fangpunkt)
            {
                Fangpunkt* obj = static_cast<Fangpunkt*> (markiertesObjekt);
                if(obj)
                {
                    posX = (obj->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
                    posY = (obj->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
                    int symbolGroesse = 1.2f * pxSuchEntfernung;
                    dc.DrawLine(posX - symbolGroesse, posY - symbolGroesse, posX + symbolGroesse, posY + symbolGroesse);
                    dc.DrawLine(posX + symbolGroesse, posY - symbolGroesse, posX - symbolGroesse, posY + symbolGroesse);
                }
            }
        }
        /*ENDE Markierung Malen*/

        /*Gefälle malen*/
        if(gefaelleAnzeigen)
        {
            dc.SetPen(wxPen(col_Gefaelle, 1));
            dc.SetBrush(wxBrush(col_Gefaelle));
            wxPoint ln_start, ln_ende;
            ln_start = NeueMousePosition;
            ln_ende = wxPoint(NeueMousePosition.x - (m_aktGefaelle.GetKoordinaten(aktProjX)*suchRadius)*m_skalierung,
                              NeueMousePosition.y - (m_aktGefaelle.GetKoordinaten(aktProjY)*suchRadius)*m_skalierung);
                dc.DrawLine(ln_start, ln_ende);
            wxString t_text = wxString::Format("%0.2f\%", m_aktGefaelle.GetKoordinaten(aktProjZ)*100);
            dc.DrawText(t_text, ln_start.x+5, ln_start.y-15);
        }
        if(gefaelleRasterAnzeigen)
        {
            if(aktLayer)
            {
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                unsigned char *uchFarbe;
                wxPoint ln_start, ln_ende;
                double dLaenge;
                Vektor vAktOrt, vGefaelle;
                Liste<GefaelleMarke>* gmSammlung = aktLayer->HoleGefaelleMarken();

                for(GefaelleMarke* gmLaeufer = gmSammlung->GetErstesElement(); gmLaeufer != NULL; gmLaeufer = gmSammlung->GetNaechstesElement())
                {
                    uchFarbe = gmLaeufer->HoleFarbe();
                    vAktOrt = gmLaeufer->HolePosition();
                    vGefaelle = gmLaeufer->HoleGefaelle();
                    dc.SetPen(wxPen(wxColor(uchFarbe[0], uchFarbe[1], uchFarbe[2]), 1));

                    ln_start = wxPoint((vAktOrt.GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung, (vAktOrt.GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung);
                    dLaenge = vGefaelle.ProjLaenge(aktProjZ);
                    if(dLaenge)
                    {
                        ln_ende = wxPoint(ln_start.x - (vGefaelle.GetKoordinaten(aktProjX)*m_gefaelleRaster)*m_skalierung/dLaenge/2,
                                      ln_start.y - (vGefaelle.GetKoordinaten(aktProjY)*m_gefaelleRaster)*m_skalierung/dLaenge/2);
                        dc.DrawLine(ln_start, ln_ende);
                        dc.DrawCircle(ln_start, m_gefaelleRaster*m_skalierung*0.075);
                    }
                }
            }
        }
        /*ENDEGefälle malen*/
    }else
    if(aktuelleAnsicht == ansicht_ID_pseudoschatten)
    {
        dc.SetPen(wxPen(wxColour(255, 0, 128), 1));
        dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
        dc.DrawText(wxString::Format("Pseudoschattenfaktor: %0.3f", m_pseudoSchattenFkt), wxPoint(0, 0));
        for(tempLayer = m_layer->GetErstesElement(); tempLayer != NULL; tempLayer = m_layer->GetNaechstesElement())
        {
            if(tempLayer->IstSichtbar() == false)continue;
            if(tempLayer == aktLayer)continue;
            loc_col_Hoehenlinie = col_Hoehenlinie;
            Liste<Flaeche>* flSammlung = tempLayer->HoleFlaechen();


            for(tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
            {
                if(tempFlaeche->HoleTyp() == RUZ_Dreieck)
                {
                    anzEcken = 3;
                }else{
                    anzEcken = 4;
                }

                Vektor tempNormale = tempFlaeche->HoleNormale();
                double neigungsFkt = (tempNormale * m_sonnenRichtung)/(tempNormale.Laenge() * m_sonnenRichtung.Laenge());
                neigungsFkt = acos(neigungsFkt);
                neigungsFkt = 1 - neigungsFkt * (2 / (3.14159265358979 * m_pseudoSchattenFkt));
                if(neigungsFkt < 0)neigungsFkt = 0.0;
                if(neigungsFkt > 1)neigungsFkt = 1.0;
                r = g = b = 255 * neigungsFkt;
                dc.SetPen(wxPen(wxColour(r, g, b), 1));
                dc.SetBrush(wxBrush(wxColour(r, g, b)));
                for(int k=0; k<anzEcken; k++)
                {
                    dP[k] = wxPoint((int)((tempFlaeche->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                    (int)((tempFlaeche->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                }
                dc.DrawPolygon(anzEcken, dP, 0, 0, wxODDEVEN_RULE);

                dc.SetPen(wxPen(loc_col_Hoehenlinie, 1));
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                if(hlAnzeigen)
                {
                    Liste<RUZ_Hoehenlinie>* hlListe = tempFlaeche->HoleHL();
                    for(RUZ_Hoehenlinie* aktHL = hlListe->GetErstesElement(); aktHL != NULL; aktHL = hlListe->GetNaechstesElement())
                    {
                        dc.DrawLine((aktHL->x(0)-dc_Offset[0])*m_skalierung, (aktHL->y(0)-dc_Offset[1])*m_skalierung,
                            (aktHL->x(1)-dc_Offset[0])*m_skalierung, (aktHL->y(1)-dc_Offset[1])*m_skalierung);
                    }
                }
            }
        }
        if(aktLayer)
        {
            loc_col_Hoehenlinie = col_Hoehenlinie;
            Liste<Flaeche>* flSammlung = aktLayer->HoleFlaechen();

            for(tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
            {
                if(tempFlaeche->HoleTyp() == RUZ_Dreieck)
                {
                    anzEcken = 3;
                }else{
                    anzEcken = 4;
                }

                Vektor tempNormale = tempFlaeche->HoleNormale();
                double neigungsFkt = (tempNormale * m_sonnenRichtung)/(tempNormale.Laenge() * m_sonnenRichtung.Laenge());
                neigungsFkt = acos(neigungsFkt);
                neigungsFkt = 1 - neigungsFkt * (2 / (3.14159265358979 * m_pseudoSchattenFkt));
                if(neigungsFkt < 0)neigungsFkt = 0.0;
                if(neigungsFkt > 1)neigungsFkt = 1.0;
                r = g = b = 255 * neigungsFkt;
                dc.SetPen(wxPen(wxColour(r, g, b), 1));
                dc.SetBrush(wxBrush(wxColour(r, g, b)));
                for(int k=0; k<anzEcken; k++)
                {
                    dP[k] = wxPoint((int)((tempFlaeche->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung),
                                    (int)((tempFlaeche->HolePunkt(k)->HolePosition().GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung));
                }
                dc.DrawPolygon(anzEcken, dP, 0, 0, wxODDEVEN_RULE);

                dc.SetPen(wxPen(loc_col_Hoehenlinie, 1));
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                if(hlAnzeigen)
                {
                    Liste<RUZ_Hoehenlinie>* hlListe = tempFlaeche->HoleHL();
                    for(RUZ_Hoehenlinie* aktHL = hlListe->GetErstesElement(); aktHL != NULL; aktHL = hlListe->GetNaechstesElement())
                    {
                        dc.DrawLine((aktHL->x(0)-dc_Offset[0])*m_skalierung, (aktHL->y(0)-dc_Offset[1])*m_skalierung,
                            (aktHL->x(1)-dc_Offset[0])*m_skalierung, (aktHL->y(1)-dc_Offset[1])*m_skalierung);
                    }
                }
            }
        }
    }
    if(m_markierungsRechteck)
    {
        dc.SetPen(wxPen(col_AuswahlRechteck, 1));
        dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
        dc.DrawRectangle(wxRect(AlteMousePosition, NeueMousePosition));
    }

    dc.SetPen(wxPen(col_markiert_Obj, 2));
    if(vVerschubStart)
    {
        posX = (vVerschubStart->GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
        posY = (vVerschubStart->GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
        dc.DrawLine(posX - 3, posY - 3, posX + 3, posY + 3);
        dc.DrawLine(posX + 3, posY - 3, posX - 3, posY + 3);
    }
    if(m_drehungDrPkt)
    {
        posX = (m_drehungDrPkt->GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
        posY = (m_drehungDrPkt->GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
        dc.DrawLine(posX - 3, posY - 3, posX + 3, posY + 3);
        dc.DrawLine(posX + 3, posY - 3, posX - 3, posY + 3);
    }
    if(m_drehungRichtung1)
    {
        posX = (m_drehungRichtung1->GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
        posY = (m_drehungRichtung1->GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
        dc.DrawLine(posX - 3, posY - 3, posX + 3, posY + 3);
        dc.DrawLine(posX + 3, posY - 3, posX - 3, posY + 3);
    }
    if(m_drehungRichtung2)
    {
        posX = (m_drehungRichtung2->GetKoordinaten(aktProjX)-dc_Offset[0])*m_skalierung;
        posY = (m_drehungRichtung2->GetKoordinaten(aktProjY)-dc_Offset[1])*m_skalierung;
        dc.DrawLine(posX - 3, posY - 3, posX + 3, posY + 3);
        dc.DrawLine(posX + 3, posY - 3, posX - 3, posY + 3);
    }
	/*Masstabsbalken*/
	dc.SetPen(wxPen(col_Pkt_Ln, 1));
	dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
	dc.SetTextForeground(col_Pkt_Ln);
	
	int mbStart = dc.GetSize().GetHeight() - 10;
	dc.DrawLine(10, mbStart, 10, mbStart - 5);
	dc.DrawLine(mbSkalierung.iLaenge1 + 10, mbStart, mbSkalierung.iLaenge1 + 10, mbStart - 5);
	dc.DrawLine(mbSkalierung.iLaenge2 + 10, mbStart, mbSkalierung.iLaenge2 + 10, mbStart - 5);
	dc.DrawLine(10, mbStart, mbSkalierung.iLaenge2 + 10, mbStart);

	char sNachkomma1[10], sNachkomma2[10];
	std::strcpy(sNachkomma1, "%.");
	std::strcpy(sNachkomma2, "%.");
	
	char nmb[4];
	itoa((mbSkalierung.iRes - 1) * (mbSkalierung.iRes > 0), nmb, 10);
	std::strcat(sNachkomma1, nmb);
	std::strcat(sNachkomma1, "f");
	
	itoa((mbSkalierung.iRes - 2) * (mbSkalierung.iRes - 1 > 0), nmb, 10);
	std::strcat(sNachkomma2, nmb);
	std::strcat(sNachkomma2, "f");
	
	dc.DrawText(wxString::Format(sNachkomma1, mbSkalierung.dWert1), mbSkalierung.iLaenge1 + 10, mbStart - 20);
	dc.DrawText(wxString::Format(sNachkomma2, mbSkalierung.dWert2), mbSkalierung.iLaenge2 + 10, mbStart - 20);

    return;
}

void RUZmBIFrame::OnPunkteVernetzen(wxCommandEvent &event)
{
    ObjekteNullen();
    SetStatusText(wxT("Punkte vernetzen"));
    if(aktLayer!=NULL)
    {
        Liste<Punkt>* pktLst = new Liste<Punkt>();
        for(RUZ_Objekt *obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
        {
            if(obj->HoleTyp() == RUZ_Punkt)
            {
                pktLst->Hinzufuegen(static_cast<Punkt*>(obj));
            }
        }
        m_auswahl->ListeLeeren("");
        double dauer;
        if(pktLst->GetListenGroesse() == 0)
        {
            dauer = aktLayer->PunkteVernetzen();
        }else{
            dauer = aktLayer->PunkteVernetzen(pktLst);
        }
        delete pktLst;
        SetStatusText(wxString::Format("Vernetzen dauerte %1.5f Sekunden", dauer), 1);
        Refresh();
    }else{
        SetStatusText(wxT("Kein aktueller Layer vorhanden!"), 1);
    }
    return;
}

void RUZmBIFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void RUZmBIFrame::OnSaveFile(wxCommandEvent &event)
{
    if(event.GetId() == idMenuExportPrismen)
    {
        FileSaver->SetWildcard(wxT("Prismen (*.prs)|*.prs"));
        FileSaver->SetMessage(wxT("Dreiecksprismen in Datei speichern"));
    }else
    if(event.GetId() == idMenuExportPunkte)
    {
        FileSaver->SetWildcard(wxT("Punkte (*.pnt)|*.pnt"));
        FileSaver->SetMessage(wxT("Punkte in Datei speichern"));
    }else
    if(event.GetId() == idMenuQuickSave)
    {
        if(strAktuellerSpeicherpfad != wxEmptyString)
        {
            SchreibeInDatei((char*)static_cast<const char*>(strAktuellerSpeicherpfad.c_str()));
            return;
        }
        FileSaver->SetWildcard(wxT("RUZ-Datei (*.ruz)|*.ruz"));
        FileSaver->SetMessage(wxT("Zeichnung in RUZ-Datei speichern"));
    }else
    {
        FileSaver->SetWildcard(wxT("DXF-Datei (*.dxf)|*.dxf|RUZ-Datei (*.ruz)|*.ruz"));
        FileSaver->SetMessage(wxT("Zeichnung in DXF-Datei speichern"));
    }

    int Rueckgabe = FileSaver->ShowModal();
    if(Rueckgabe==wxID_CANCEL)return;
    SetStatusText(FileSaver->GetPath(), 1);

    if(event.GetId() == idMenuExportPrismen)
    {
        ExportiereDreiecksPrismen((char*)static_cast<const char*>(FileSaver->GetPath().c_str()));
    }else
    if(event.GetId() == idMenuExportPunkte)
    {
        ExportierePunkte((char*)static_cast<const char*>(FileSaver->GetPath().c_str()));
    }else
    {
        if((FileSaver->GetPath()).EndsWith(wxT("ruz"))||(FileSaver->GetPath()).EndsWith(wxT("RUZ")))
        {
            strAktuellerSpeicherpfad = FileSaver->GetPath();
            SchreibeInDatei((char*)static_cast<const char*>(FileSaver->GetPath().c_str()));
        }
        if((FileSaver->GetPath()).EndsWith(wxT("dxf"))||(FileSaver->GetPath()).EndsWith(wxT("DXF")))
        {
            Liste<RUZ_Layer>* exportListe = new Liste<RUZ_Layer>;
            for(RUZ_Layer* t_lay = m_layer->GetErstesElement(); t_lay; t_lay = m_layer->GetNaechstesElement())
            {
                if(t_lay->IstSichtbar())exportListe->Hinzufuegen(t_lay);
            }
            if(m_hintergrundMalen)exportListe->Hinzufuegen(m_hintergrundLayer);
						/*Sichtbarkeitsflags setzen*/
						unsigned int cSichtbar = 0x0000;
						cSichtbar = (m_zeigePunkt | (m_zeigeLinie << 1) | (m_zeigeFlaeche << 2) |
												(hlAnzeigen << 3) | (m_zeigeStrich << 4) | (m_zeigeBogen << 5) |
												(m_zeigeHoehenmarke << 6));
						/*ENDE Sichtbarkeitsflags setzen*/
            DXF_Export(exportListe, (char*)static_cast<const char*>(FileSaver->GetPath().c_str()), m_anzeigeGenauigkeit, cSichtbar);
            exportListe->ListeLeeren("");
            delete exportListe;
        }
    }
    return;
}

void RUZmBIFrame::OnSize(wxSizeEvent& event)
{
    Refresh();
}

void RUZmBIFrame::OnSkalierfaktor(wxCommandEvent &event)
{
	bool exitSchleife = false;
	while(!exitSchleife)
	{
		wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie den gewünschten Skalierfaktor ein."),
								  wxT("Anzeigeskalierung"), wxString::Format(wxT("%0.2f"), 1.0));
		if(abfrage.ShowModal() == wxID_ABORT)return;
		wxString number = abfrage.GetValue();
		/*komma gegen punkt tauschen*/
		wxString punkt = wxT(".");
		wxString komma = wxT(",");
		number.Replace(komma, punkt);
		double t_skal = 1.0;
		if(!number.ToDouble(&t_skal))
		{
			if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)break;
			continue;
		}else
		{
			SkalierungSetzen(t_skal);
			exitSchleife = true;
		}
	}
	SetStatusText(wxString::Format("Offset: %1.5f - %1.5f / Skalierung: %5.5f", dc_Offset[0], dc_Offset[1], m_skalierung), 1);
	Refresh();
	return;
}

void RUZmBIFrame::OnSonnenstandEinstellen(wxCommandEvent &event)
{
    Sonnenstand_Dialog(this, m_sonnenRichtung).ShowModal();
    return;
}

void RUZmBIFrame::OnToggleHintergrund(wxCommandEvent &event)
{
    m_hintergrundMalen == true? m_hintergrundMalen = false : m_hintergrundMalen = true;
    menuHintergrundMalen->Check(m_hintergrundMalen);
    Refresh();
    return;
}

void RUZmBIFrame::OnUeberlappungFinden(wxCommandEvent& event)
{
    Liste<Flaeche>* flLst = aktLayer->HoleFlaechen();
    Liste<Linie>* lnLst = aktLayer->HoleLinien();
    Flaeche *flAkt, *flVergleich;
    int iEckenAkt, iEckenVergleich;
    for(Listenelement<Flaeche>* flLEAkt = flLst->GetErstesListenelement(); flLEAkt != NULL; flLEAkt = flLEAkt->GetNachfolger())
    {
        flAkt = flLEAkt->GetElement();
        flAkt->LoescheFarbe();
        flAkt->SetzeBesucht('-');
    }
    for(Linie* lnAkt = lnLst->GetErstesElement(); lnAkt != NULL; lnAkt = lnLst->GetNaechstesElement())
    {
        lnAkt->LoescheFarbe();
    }
    aktLayer->FehlerEntfernen();

    bool weiter;
    double dFlaechenInhaltAkt, dFlaechenInhalteVergleich;
    for(Listenelement<Flaeche>* flLEAkt = flLst->GetErstesListenelement(); flLEAkt != NULL; flLEAkt = flLEAkt->GetNachfolger())
    {
        flAkt = flLEAkt->GetElement();
        dFlaechenInhaltAkt = flAkt->FlaechenInhalt(aktProjZ);
        (flAkt->HoleTyp() == RUZ_Dreieck) ? iEckenAkt = 3 : iEckenAkt = 4;
        for(Listenelement<Flaeche>* flLEVergleich = flLEAkt->GetNachfolger(); flLEVergleich != NULL; flLEVergleich = flLEVergleich->GetNachfolger())
        {
            flVergleich = flLEVergleich->GetElement();
            dFlaechenInhalteVergleich = flVergleich->FlaechenInhalt(aktProjZ);
            (flVergleich->HoleTyp() == RUZ_Dreieck) ? iEckenVergleich = 3 : iEckenVergleich = 4;

            for(int i = 0; i < iEckenAkt; i++)
            {
                weiter = false;
                Punkt* pktTemp = flAkt->HolePunkt(i);
                for(int k = 0; k < iEckenVergleich; k++)
                {
                    if(pktTemp == flVergleich->HolePunkt(k))
                    {
                        weiter = true;
                        break;
                    }
                }
                if(weiter)continue;
                if(flVergleich->IstInnerhalb(pktTemp))
                {
                    if(dFlaechenInhaltAkt > dFlaechenInhalteVergleich)
                    {
                        flVergleich->SetzeBesucht('+');
                    }else{
                        flAkt->SetzeBesucht('+');
                    }
                    flVergleich->SetzeFarbe(128, 55, 55);
                    flAkt->SetzeFarbe(128, 55, 55);

                    aktLayer->Hinzufuegen(new Vektor(pktTemp->HolePosition()));
                }
                for(int k = 0; k < iEckenVergleich; k++)
                {
                    if(flVergleich->HoleLinie(k)->schneidet(flAkt->HoleLinie(i), aktProjZ))
                    {
                        if(dFlaechenInhaltAkt > dFlaechenInhalteVergleich)
                        {
                            flVergleich->SetzeBesucht('+');
                        }else{
                            flAkt->SetzeBesucht('+');
                        }
                        flVergleich->SetzeFarbe(128, 55, 55);
                        flAkt->SetzeFarbe(128, 55, 55);

                        flVergleich->HoleLinie(k)->SetzeFarbe(255, 110, 110);
                        flAkt->HoleLinie(i)->SetzeFarbe(255, 110, 110);
                    }
                }
            }

            for(int i = 0; i < iEckenVergleich; i++)
            {
                weiter = false;
                Punkt* pktTemp = flVergleich->HolePunkt(i);
                for(int k = 0; k < iEckenAkt; k++)
                {
                    if(pktTemp == flAkt->HolePunkt(k))
                    {
                        weiter = true;
                        break;
                    }
                }
                if(weiter)continue;
                if(flAkt->IstInnerhalb(flVergleich->HolePunkt(i)))
                {
                    if(dFlaechenInhaltAkt > dFlaechenInhalteVergleich)
                    {
                        flVergleich->SetzeBesucht('+');
                    }else{
                        flAkt->SetzeBesucht('+');
                    }
                    flVergleich->SetzeFarbe(128, 55, 55);
                    flAkt->SetzeFarbe(128, 55, 55);

                    aktLayer->Hinzufuegen(new Vektor(pktTemp->HolePosition()));
                }
            }
        }
    }
    Refresh();

    if(wxMessageDialog(this, wxT("Soll die Überlappung gelöscht werden?\n(kleinere Fläche wird gelöscht)")
                       , wxT("Überlappung löschen"), wxYES_NO).ShowModal() == wxID_YES)
    {
        for(Flaeche* flAkt = flLst->GetErstesElement(); flAkt != NULL; flAkt = flLst->GetNaechstesElement())
        {
            if(flAkt->HoleBesucht() == '+')
            {
                flLst->Entfernen(flAkt);
                delete flAkt;
            }
        }
        Refresh();
    }
    return;
}

void RUZmBIFrame::OnViereckeFinden(wxCommandEvent &event)
{
    AuswahlLeeren();
    if(aktLayer!=NULL)
    {
        aktLayer->ViereckeFinden();
        Refresh();
    }else{
        SetStatusText(wxT("Kein Layer vorhanden!"), 1);
    }
    return;
}

void RUZmBIFrame::OnVolumenZwischenLayern(wxCommandEvent &event)
{
    /*Die Layer auswählen zwischen denen das Volumen ermittelt werden soll*/
    int layNr1 = Layer_Auswahl_Dialog(this, m_layer, wxT("Layer wählen (Ursprungsgelände)")).ShowModal();
    int layNr2;
    if(layNr1 == -1)return;
    do
    {
        layNr2 = Layer_Auswahl_Dialog(this, m_layer, wxT("Layer wählen (Neues Gelände)")).ShowModal();
        if(layNr2 == -1)return;
        if(layNr1 == layNr2)
        {
            if(wxMessageDialog(this, wxT("Zweimal derselbe Layer gewählt!\nBitte neuen zweiten Layer wählen."), wxT("Layer identisch"), wxOK|wxCANCEL).ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }
    }while(layNr1 == layNr2);

    RUZ_Layer* erster_Layer = NULL;
    RUZ_Layer* zweiter_Layer = NULL;
    //RUZ_Layer *erster_neuer_Layer, *zweiter_neuer_Layer;

    for(Listenelement<RUZ_Layer>* layer_LE = m_layer->GetErstesListenelement(); layer_LE; layer_LE = layer_LE->GetNachfolger())
    {
        if(layer_LE->Wert() == layNr1)
        {
            erster_Layer = layer_LE->GetElement();
        }
        if(layer_LE->Wert() == layNr2)
        {
            zweiter_Layer = layer_LE->GetElement();
        }
        if(erster_Layer && zweiter_Layer)break;
    }
    if(!erster_Layer || !zweiter_Layer)
    {
        wxMessageDialog(this, wxT("Layerauswahl war nicht erfolgreich."), wxT("Abbruch")).ShowModal();
        return;
    }

    /*Volumen ermitteln*/
    clock_t tLaufzeit;

    double dAuftrag, dAbtrag, dOffsetNeu, dOffsetUr;
    dAuftrag = dAbtrag = 0.0;
    dOffsetNeu = dOffsetUr = 0.0;

    if(event.GetId() == idVolumenZwischenLayern_Integral)
    {
        /*Höhenverschub der Layer*/
        bool exitSchleife = false;
        while(!exitSchleife) //dOffsetNeu
        {
            wxTextEntryDialog abfrage(this, wxT(" (Neues Gelände):\nHöhen für für die Berechnung anpassen"),
                                      wxT("Höhenverschub"), wxString::Format(wxT("%0.2f"), 0.0));
            if(abfrage.ShowModal() == wxID_ABORT)return;
            wxString number = abfrage.GetValue();
            /*komma gegen punkt tauschen*/
            wxString punkt = wxT(".");
            wxString komma = wxT(",");
            number.Replace(komma, punkt);
            if(!number.ToDouble(&dOffsetNeu))
            {
                if(strcmp(number, "")!=0)
                {
                    if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)break;
                }else{
                    dOffsetNeu = 0.0;
                    exitSchleife = true;
                }
            }
            exitSchleife = true;
        }
        exitSchleife = false;
        while(!exitSchleife) //dOffsetUr
        {
            wxTextEntryDialog abfrage(this, wxT(" (Urgelände):\nHöhen für für die Berechnung anpassen"),
                                      wxT("Höhenverschub"), wxString::Format(wxT("%0.2f"), 0.0));
            if(abfrage.ShowModal() == wxID_ABORT)return;
            wxString number = abfrage.GetValue();
            /*komma gegen punkt tauschen*/
            wxString punkt = wxT(".");
            wxString komma = wxT(",");
            number.Replace(komma, punkt);
            if(!number.ToDouble(&dOffsetUr))
            {
                if(strcmp(number, "")!=0)
                {
                    if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)break;
                }else{
                    dOffsetUr = 0.0;
                    exitSchleife = true;
                }
            }
            exitSchleife = true;
        }
        /*ENDE Höhenverschub der Layer*/

        double minX, minY, maxX, maxY, minZ, maxZ;
		long long int anzAuftragsflaechen = 0;
		long long int anzAbtragsflaechen = 0;
        if(zweiter_Layer->AusdehnungFinden(minX, minY, maxX, maxY, minZ, maxZ))
        {
            SetStatusText(wxT("Starte Integration (Neues Gelände)"), 1);
            Refresh();

            double* dIntegral_NeuesGelaende = NULL;
            aruIntegral tempIntegral_Neu(dIntegral_NeuesGelaende, minX, minY, maxX, maxY, m_flaechenRaster, aktProjZ);
            dIntegral_NeuesGelaende = tempIntegral_Neu.HoleIntegral();
            if(!dIntegral_NeuesGelaende)
            {
                wxMessageDialog(this, wxT("Fehler beim Anlegen des Integrals\n(evtl. zu wenig Speicher?)"), wxT("Abbruch der Berechnung")).ShowModal();
                return;
            }

            Liste<Flaeche>* lstFl = zweiter_Layer->HoleFlaechen();
            int iAktFlaeche = 0;
            for(Flaeche* aktFl = lstFl->GetErstesElement(); aktFl != NULL; aktFl = lstFl->GetNaechstesElement())
            {
                if(aktFl->HoleTyp() == RUZ_Dreieck)
                {
                    Vektor vNormale = aktFl->HoleNormale();
                    Vektor vSenkrechte(0, 0, 0);
                    vSenkrechte.SetKoordinaten(aktProjZ, 1.0);
                    if(vNormale*vSenkrechte < 0.05)continue;//0.05 = ca. cos(87°) - fast senkrechte Flächen überspringen (ergibt an den Rändern unbrauchbar hohe Werte)
                }
                tempIntegral_Neu.IntegriereFlaeche(aktFl);
            }

            SetStatusText(wxT("Starte Integration (Urgelände)"), 1);
            Refresh();

            double* dIntegral_Urgelaende = NULL;
            aruIntegral tempIntegral_Ur(dIntegral_Urgelaende, minX, minY, maxX, maxY, m_flaechenRaster, aktProjZ);
            dIntegral_Urgelaende = tempIntegral_Ur.HoleIntegral();
            if(!dIntegral_Urgelaende)
            {
                wxMessageDialog(this, wxT("Fehler beim Anlegen des Integrals\n(evtl. zu wenig Speicher?)"), wxT("Abbruch der Berechnung")).ShowModal();
                return;
            }

            lstFl = erster_Layer->HoleFlaechen();
            tLaufzeit = clock();
            iAktFlaeche = 0;
            for(Flaeche* aktFl = lstFl->GetErstesElement(); aktFl != NULL; aktFl = lstFl->GetNaechstesElement())
            {
                if(aktFl->HoleTyp() == RUZ_Dreieck)
                {
                    Vektor vNormale = aktFl->HoleNormale();
                    Vektor vSenkrechte(0, 0, 0);
                    vSenkrechte.SetKoordinaten(aktProjZ, 1.0);
                    if(vNormale*vSenkrechte < 0.05)continue;//0.05 = ca. cos(87°) - fast senkrechte Flächen überspringen (ergibt an den Rändern unbrauchbar hohe Werte)
                }
                tempIntegral_Ur.IntegriereFlaeche(aktFl);
            }

            double maxWert, minWert;
            int iB = tempIntegral_Neu.HoleBreite();
            int iH = tempIntegral_Neu.HoleHoehe();

            maxWert = minWert = 0.0;
            bool nochNAN = true;

            SetStatusText(wxT("Stopfe Löcher (Neues Gelände)"), 1);
            Refresh();
            for(int i = 0; i < iB; i++)
            {
                for(int k = 0; k < iH; k++)
                {
                    if(!isnan(dIntegral_NeuesGelaende[i+k*iB]))
                    {

                        if(nochNAN)
                        {
                            maxWert = minWert = dIntegral_NeuesGelaende[i+k*iB];
                            nochNAN = false;
                        }
                        if(maxWert < dIntegral_NeuesGelaende[i+k*iB])maxWert = dIntegral_NeuesGelaende[i+k*iB];
                        if(minWert > dIntegral_NeuesGelaende[i+k*iB])minWert = dIntegral_NeuesGelaende[i+k*iB];
                    }else
                    {
                        //Löcher stopfen
                        if((i>0)&&(k>0)&&(i<iB-1)&&(k<iH-1))
                        {
                            int iNachbarn = 0;
                            double dSumme = 0;
                            for(int di = -1; di < 2; di++)
                                for(int dk = -1; dk < 2; dk++)
                                {
                                    if(!isnan(dIntegral_NeuesGelaende[i+di+(k+dk)*iB]))
                                    {
                                        iNachbarn++;
                                        dSumme += dIntegral_NeuesGelaende[i+di+(k+dk)*iB];
                                    }
                                }
                            if(iNachbarn > 5)//Loch ist (vermutlich) in Fläche und nicht am Rand
                            {
                                dIntegral_NeuesGelaende[i+k*iB] = dSumme/iNachbarn;
                            }
                        }
                    }
                }
            }

            SetStatusText(wxT("Stopfe Löcher (Urgelände)"), 1);
            Refresh();

            for(int i = 0; i < iB; i++)
            {
                for(int k = 0; k < iH; k++)
                {
                    if(!isnan(dIntegral_Urgelaende[i+k*iB]))
                    {
                        if(nochNAN)
                        {
                            maxWert = minWert = dIntegral_Urgelaende[i+k*iB];
                            nochNAN = false;
                        }
                        if(maxWert < dIntegral_Urgelaende[i+k*iB])maxWert = dIntegral_Urgelaende[i+k*iB];
                        if(minWert > dIntegral_Urgelaende[i+k*iB])minWert = dIntegral_Urgelaende[i+k*iB];
                    }else
                    {
                        //Löcher stopfen
                        if((i>0)&&(k>0)&&(i<iB-1)&&(k<iH-1))
                        {
                            int iNachbarn = 0;
                            double dSumme = 0;
                            for(int di = -1; di < 2; di++)
                                for(int dk = -1; dk < 2; dk++)
                                {
                                    if(!isnan(dIntegral_Urgelaende[i+di+(k+dk)*iB]))
                                    {
                                        iNachbarn++;
                                        dSumme += dIntegral_Urgelaende[i+di+(k+dk)*iB];
                                    }
                                }
                            if(iNachbarn > 5)//Loch ist (vermutlich) in Fläche und nicht am Rand
                            {
                                dIntegral_Urgelaende[i+k*iB] = dSumme/iNachbarn;
                            }
                        }
                    }
                }
            }


            for(int i = 0; i < iB; i++)
            {
                for(int k = 0; k < iH; k++)
                {
                    dIntegral_NeuesGelaende[i+k*iB] -= dIntegral_Urgelaende[i+k*iB] + dOffsetUr - dOffsetNeu;
                }
            }

            lwBild.NeueLeinwand(iB, iH, 1/m_flaechenRaster, minX, minY);
            for(int i = 0; i < (iB * iH); i++)
            {
                if(isnan(dIntegral_NeuesGelaende[i]))
                {
                    lwBild.ucLeinwand[i*3] = col_ZeichenHintergrund.Red();
                    lwBild.ucLeinwand[i*3+1] = col_ZeichenHintergrund.Green();
                    lwBild.ucLeinwand[i*3+2] = col_ZeichenHintergrund.Blue();
                }else if(dIntegral_NeuesGelaende[i] >= 0)
                {
                    dAuftrag += dIntegral_NeuesGelaende[i];
                    lwBild.ucLeinwand[i*3] = col_Flaeche_darunter.Red();
                    lwBild.ucLeinwand[i*3+1] = col_Flaeche_darunter.Green();
                    lwBild.ucLeinwand[i*3+2] = col_Flaeche_darunter.Blue();
					anzAuftragsflaechen++;
                }
                else if(dIntegral_NeuesGelaende[i] < 0)
                {
                    dAbtrag -= dIntegral_NeuesGelaende[i];
                    lwBild.ucLeinwand[i*3] = col_Flaeche_darueber.Red();
                    lwBild.ucLeinwand[i*3+1] = col_Flaeche_darueber.Green();
                    lwBild.ucLeinwand[i*3+2] = col_Flaeche_darueber.Blue();
					anzAbtragsflaechen++;
                }
            }
        }
        /*Ergebnisausgabe*/
        dAuftrag *= m_flaechenRaster * m_flaechenRaster;
        dAbtrag *= m_flaechenRaster * m_flaechenRaster;
        SetStatusText(wxString::Format("Auftrag:\t%0.3f\tAbtrag:\t%0.3f", dAuftrag, dAbtrag), 1);
        wxMessageDialog(this, wxString::Format("Auftrag:\t%0.3f\nAbtrag:\t%0.3f", dAuftrag, dAbtrag), wxT("Ergebnis")).ShowModal();
        logSchreiben("\n\n/**Ergebnis der Volumenberechnung zwischen Layern**/\n/***************Flächenintegration****************/\n");
        logSchreiben("Urgelände:\t%s\tVerschub:\t%0.3f\n", erster_Layer->HoleName(), dOffsetUr);
        logSchreiben("neues Gelände:\t%s\tVerschub:\t%0.3f\n", zweiter_Layer->HoleName(), dOffsetNeu);
        logSchreiben("Rastergröße:\t%0.3f\n", m_flaechenRaster);
        char buffer[50];
        sprintf(buffer, "Auftrag:\t%c0.%df auf Flaeche:\t%c0.%df\n", '%', m_anzeigeGenauigkeit, '%', m_anzeigeGenauigkeit);
        logSchreiben(buffer, dAuftrag, m_flaechenRaster * m_flaechenRaster * anzAuftragsflaechen);
        sprintf(buffer, "Abtrag:\t%c0.%df auf Flaeche:\t%c0.%df\n", '%', m_anzeigeGenauigkeit, '%', m_anzeigeGenauigkeit);
        logSchreiben(buffer, dAbtrag, m_flaechenRaster * m_flaechenRaster * anzAbtragsflaechen);
        logSchreiben("/**ENDE Volumenberechnung**/\n");
        /*ENDE Ergebnisausgabe*/
        return;
    }

	/*if(event.GetId() == idVolumenZwischenLayern_Kons)*/
    Liste<Flaeche> *flLst1 = erster_Layer->HoleFlaechen();
    Liste<Flaeche> *flLst2 = zweiter_Layer->HoleFlaechen();

	Liste<Punkt> *pktLst1 = erster_Layer->HolePunkte();
	Liste<Punkt> *pktLst2 = zweiter_Layer->HolePunkte();
    Vektor swPkt;
    double vergleichsHoehe;

	ofstream fsErgebnis;
	std::string prismaNeuDrueber, prismaNeuDrunter, prismaAltDrueber, prismaAltDrunter;
	prismaAltDrueber = "Abtrag Layer ";
	prismaAltDrueber += erster_Layer->HoleName();
	prismaAltDrueber += "\n";
	
	prismaAltDrunter = "Auftrag Layer ";
	prismaAltDrunter += erster_Layer->HoleName();
	prismaAltDrunter += "\n";
	
	prismaNeuDrueber = "Auftrag Layer ";
	prismaNeuDrueber += zweiter_Layer->HoleName();
	prismaNeuDrueber += "\n";
	
	prismaNeuDrunter = "Abtrag Layer ";
	prismaNeuDrunter += zweiter_Layer->HoleName();
	prismaNeuDrunter += "\n";
	
	fsErgebnis.open("log/VolumenErmittlung.txt", std::fstream::out|std::fstream::trunc);
	if(!fsErgebnis.good())
	{
		wxMessageDialog(this, wxT("Ausgabedateien konnten nicht geöffnet werden")).ShowModal();
		return;
	}
<<<<<<< HEAD
	
=======
	
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
	/*Damit das sicher funktioniert, muessen die Layer vorher verschnitten werden*/
	double volumen, flaeche;
    for(Flaeche *aktFl1 = flLst1->GetErstesElement(); aktFl1; aktFl1 = flLst1->GetNaechstesElement())//Urgelaende
    {
		volumen = aktFl1->Volumen(aktProjZ);
		flaeche = aktFl1->FlaechenInhalt(aktProjZ);
        swPkt = aktFl1->Schwerpunkt();
        vergleichsHoehe = swPkt.GetKoordinaten(aktProjZ);
        for(Flaeche *aktFl2 = flLst2->GetErstesElement(); aktFl2; aktFl2 = flLst2->GetNaechstesElement())
        {
            if(aktFl2->OrtAufFlaeche(swPkt, aktProjZ))
            {
                if(swPkt.GetKoordinaten(aktProjZ) > vergleichsHoehe)
                {
                    dAuftrag -= volumen;
					PrismaSchreiben(prismaAltDrunter, aktFl1, flaeche, volumen);
                    aktFl1->SetzeFarbe(col_Flaeche_darunter.Red(), col_Flaeche_darunter.Green(), col_Flaeche_darunter.Blue());
                    break;
                }else
                if(swPkt.GetKoordinaten(aktProjZ) < vergleichsHoehe)
                {
                    dAbtrag += volumen;
					PrismaSchreiben(prismaAltDrueber, aktFl1, flaeche, volumen);
                    aktFl1->SetzeFarbe(col_Flaeche_darueber.Red(), col_Flaeche_darueber.Green(), col_Flaeche_darueber.Blue());
                    break;
                }
            }
        }
    }
    for(Flaeche *aktFl2 = flLst2->GetErstesElement(); aktFl2; aktFl2 = flLst2->GetNaechstesElement())//Neues Gelaende
    {
		volumen = aktFl2->Volumen(aktProjZ);
		flaeche = aktFl2->FlaechenInhalt(aktProjZ);
        swPkt = aktFl2->Schwerpunkt();
        vergleichsHoehe = swPkt.GetKoordinaten(aktProjZ);
        for(Flaeche *aktFl1 = flLst1->GetErstesElement(); aktFl1; aktFl1 = flLst1->GetNaechstesElement())
        {
            if(aktFl1->OrtAufFlaeche(swPkt, aktProjZ))
            {
                if(swPkt.GetKoordinaten(aktProjZ) > vergleichsHoehe)
                {
                    dAbtrag -= aktFl2->Volumen(aktProjZ);
					PrismaSchreiben(prismaNeuDrunter, aktFl2, flaeche, volumen);
                    aktFl2->SetzeFarbe(col_Flaeche_darunter.Red(), col_Flaeche_darunter.Green(), col_Flaeche_darunter.Blue());
                    break;
                }else
                if(swPkt.GetKoordinaten(aktProjZ) < vergleichsHoehe)
                {
                    dAuftrag += aktFl2->Volumen(aktProjZ);
					PrismaSchreiben(prismaNeuDrueber, aktFl2, flaeche, volumen);
                    aktFl2->SetzeFarbe(col_Flaeche_darueber.Red(), col_Flaeche_darueber.Green(), col_Flaeche_darueber.Blue());
                    break;
                }
            }
        }
    }
    /*ENDE Volumen ermitteln*/
    wxMessageDialog(this, wxString::Format("Auftrag:\t%0.3f\nAbtrag:\t%0.3f", dAuftrag, dAbtrag)).ShowModal();
    logSchreiben("\n\n/**Ergebnis der Volumenberechnung zwischen Layern**/\n/***************Prismenberechnung****************/\n");
    logSchreiben("Urgelände:\t%s\tVerschub:\t%0.3f\n", erster_Layer->HoleName(), dOffsetUr);
    logSchreiben("neues Gelände:\t%s\tVerschub:\t%0.3f\n", zweiter_Layer->HoleName(), dOffsetNeu);
    logSchreiben("Rastergröße:\t%0.3f\n", m_flaechenRaster);
    char buffer[50];
    sprintf(buffer, "Auftrag:\t%c0.%df\n", '%', m_anzeigeGenauigkeit);
	logSchreiben(buffer, dAuftrag);
    sprintf(buffer, "Abtrag:\t%c0.%df\n", '%', m_anzeigeGenauigkeit);
	logSchreiben(buffer, dAbtrag);
    logSchreiben("/**ENDE Volumenberechnung**/\n");

<<<<<<< HEAD
	fsErgebnis<<"Ergebnis der Volumenberechnung zwischen Layern\nPrismenberechnung\n\n";
	fsErgebnis<<setw(15)<<"Urgelände: "<<setw(20)<<erster_Layer->HoleName()<<"\n";
	fsErgebnis<<setw(15)<<"Neues Gelände: "<<setw(20)<<zweiter_Layer->HoleName()<<"\n";
	snprintf(buffer, 50, "%0.3f", dAuftrag);
	fsErgebnis<<setw(15)<<"Auftrag: "<<setw(20)<<buffer<<"\n";
	snprintf(buffer, 50, "%0.3f", dAbtrag);
	fsErgebnis<<setw(15)<<"Abtrag: "<<setw(20)<<buffer<<"\n";
	fsErgebnis<<"\nDreiecksprismen\n";
	fsErgebnis<<setw(20)<<"X"<<setw(20)<<"Y"<<setw(20)<<"Z"<<setw(20)<<"Fläche"<<setw(20)<<"Volumen\n";
=======
	fsErgebnis<<"Ergebnis der Volumenberechnung zwischen Layern\nPrismenberechnung\n\n";
	fsErgebnis<<setw(15)<<"Urgelände: "<<setw(20)<<erster_Layer->HoleName()<<"\n";
	fsErgebnis<<setw(15)<<"Neues Gelände: "<<setw(20)<<zweiter_Layer->HoleName()<<"\n";
	snprintf(buffer, 50, "%0.3f", dAuftrag);
	fsErgebnis<<setw(15)<<"Auftrag: "<<setw(20)<<buffer<<"\n";
	snprintf(buffer, 50, "%0.3f", dAbtrag);
	fsErgebnis<<setw(15)<<"Abtrag: "<<setw(20)<<buffer<<"\n";
	fsErgebnis<<"\nDreiecksprismen\n";
	fsErgebnis<<setw(20)<<"X"<<setw(20)<<"Y"<<setw(20)<<"Z"<<setw(20)<<"Fläche"<<setw(20)<<"Volumen\n";
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
	fsErgebnis<<"\n"<<prismaAltDrueber<<"\n"<<prismaAltDrunter;
	fsErgebnis<<"\n"<<prismaNeuDrueber<<"\n"<<prismaNeuDrunter;
	fsErgebnis.close();
    return;
}

void RUZmBIFrame::OnZeigeWaehle(wxCommandEvent &event)
{
    ObjAnzAuswDlg->ShowModal();
    return;
}

void RUZmBIFrame::ParamIni(void)
{
    /*Paint*/
    dc_Offset[0] = dc_Offset[1] = 0;
    SkalierungSetzen(1.0);
    m_hintergrundMalen = true;
    anzeigeSkalieren = false;
    /*ENDE Paint*/

    /*Schalter*/
    hlAnzeigen = false;
    gefaelleAnzeigen = false;
    m_markierungsRechteck = false;
    m_kreuzen = false;
    m_markierModus = false;

    m_zeigeFlaeche = true;
    m_zeigeHoehe = true;
    m_zeigeHoehenmarke = true;
    m_zeigeLinie = true;
    m_zeigePunkt = true;
    m_zeigeStrich = true;
    m_zeigeBogen = true;
    m_zeigeKreis = true;
    m_zeigeFangpunkt = true;

    m_waehlePunkt = true;
    m_waehleLinie = true;
    m_waehleFlaeche = true;
    m_waehleHoehenmarke = true;
    m_waehleStrich = true;
    m_waehleBogen = true;
    m_waehleKreis = true;
    m_waehleFangpunkt = true;
    /*ENDE Schalter*/

    m_sonnenRichtung = Vektor(-0.577, -0.577, 0.577);
    m_aktGefaelle = Vektor(0, 0, 0);
    m_vktSchnittPkt1 = NULL_VEKTOR;
    m_vktSchnittPkt2 = NULL_VEKTOR;

    wertFkt = 1;
    aktBefehl = bef_ID_nichts;

    aktProjX = x;
    aktProjY = y;
    aktProjZ = z;

    hoehenSchritt = hlParameterDlg->HoleWert(IDhoehenSchritt);
    suchRadius = hlParameterDlg->HoleWert(IDsuchRadius);
    startHoehe = hlParameterDlg->HoleWert(IDstartHoehe);

    m_verschubWeite = peEinstellungenDlg->HoleWert(IDverschubWeite);
    m_lnWandelGenauigkeit = peEinstellungenDlg->HoleWert(IDlnWandelGenauigkeit);
    m_pseudoSchattenFkt = peEinstellungenDlg->HoleWert(IDpseudoSchattenFkt);
    m_gefaelleRaster = peEinstellungenDlg->HoleWert(IDgefaelleRasterGroesse);
    m_flaechenRaster = peEinstellungenDlg->HoleWert(IDflaechenRasterGroesse);
    m_anzeigeGenauigkeit = (int)(peEinstellungenDlg->HoleWert(IDanzeigeGenauigkeit));
        if(m_anzeigeGenauigkeit < 0)m_anzeigeGenauigkeit = 0;
    pxSuchEntfernung = (int)(peEinstellungenDlg->HoleWert(IDpxSuchEntfernung));
        if(pxSuchEntfernung < 1)pxSuchEntfernung = 1;

    col_Pkt_Ln = peEinstellungenDlg->HoleFarbe(IDFarbePktLn);
    col_Strich = peEinstellungenDlg->HoleFarbe(IDFarbeStrich);
    col_HoehenMarke = peEinstellungenDlg->HoleFarbe(IDFarbeHoehenMarke);
    col_Hoehenlinie = peEinstellungenDlg->HoleFarbe(IDFarbeHoehenlinie);
    col_markiert_Obj = peEinstellungenDlg->HoleFarbe(IDFarbeMarkiertesObjekt);
    col_ausgewaehlt_Obj = peEinstellungenDlg->HoleFarbe(IDFarbeAusgewaehltesObjekt);
    col_HintergrundLayer = peEinstellungenDlg->HoleFarbe(IDFarbeHintergrundLayer);
    col_ZeichenHintergrund = peEinstellungenDlg->HoleFarbe(IDFarbeZeichenHintergrund);
    col_AuswahlRechteck = peEinstellungenDlg->HoleFarbe(IDFarbeAuswahlRechteck);
    col_Flaeche_darueber = peEinstellungenDlg->HoleFarbe(IDFarbeFlaecheDarueber);
    col_Flaeche_darunter = peEinstellungenDlg->HoleFarbe(IDFarbeFlaecheDarunter);
    col_Gefaelle = peEinstellungenDlg->HoleFarbe(IDFarbeGefaelle);
    col_Fangpunkt = peEinstellungenDlg->HoleFarbe(IDFarbeFangpunkt);

    /*Drehung*/
    m_drehungAuswahlOrte = new Liste<Vektor>();
    if(!m_drehungAuswahlOrte)
    {
        logSchreiben("m_drehungAuswahlOrte wurde nicht initialisiert\nProgrammabbruch!");
        this->Destroy();
    }
    m_drehungDrPkt = m_drehungRichtung1 = m_drehungRichtung2 = NULL;
    /*ENDE Drehung*/

    /*Verschieben*/
    vVerschubStart = NULL;
    m_verschubAuswahlOrte = new Liste<PunktSpeicher>();
    if(!m_verschubAuswahlOrte)
    {
        logSchreiben("m_drehungAuswahlOrte wurde nicht initialisiert\nProgrammabbruch!");
        this->Destroy();
    }
    /*ENDE Verschieben*/

    /*Schnittpunkt*/
    m_schP_OrgPkt = NULL;
    m_schP_Ln = NULL;
    m_schP_Dr = NULL;
    m_schP_Obj = NULL;
    m_schP_Richtung_1 = NULL;
    m_schP_Richtung_2 = NULL;
    /*ENDE Schnittpunkt*/

    /*Fangpunkte*/
    objFang1 = objFang2 = NULL;
    /*ENDE Fangpunkte*/

    return;
}

void RUZmBIFrame::SchnittpunktFlaecheAbschliessen(void)
{
    if(m_markierModus && m_schP_Dr && m_schP_OrgPkt)
    {
        m_markierModus = false;
        m_markierModus2 = true;
        m_normale = m_schP_Dr->HoleNormale();
        m_Abstand = m_normale * m_schP_Dr->HolePunkt(0)->HolePosition();
        SetStatusText(wxT("Zielfläche wählen"), 2);
        if(alternativAktLayer)
        {
            aktLayerBAK = aktLayer;
            aktLayer = alternativAktLayer;
        }
    }else
    if(m_markierModus2 && m_schP_Obj)
    {
        m_markierModus2 = false;
        m_schP_Richtung_1 = new Punkt(m_schP_Dr->MitteGegenueber(m_schP_OrgPkt), aktLayer);
        m_schP_Richtung_2 = new Punkt(m_schP_OrgPkt->HolePosition(), aktLayer);
        m_schP_Ln = Linie::NeueLinie(m_schP_Richtung_1, m_schP_Richtung_2);
        SetStatusText(wxT("Richtung der Verschneidung wählen"), 2);
        if(alternativAktLayer)
        {
            aktLayer = aktLayerBAK;
        }
    }else
    {
        SetStatusText(wxT("Schnittpunkt: Punkt und Fläche wählen"), 2);
        m_markierModus = true;
        m_markierModus2 = false;
        m_schP_Ln = NULL;
        m_schP_Obj = NULL;
        m_schP_OrgPkt = NULL;
    }
    return;
}

void RUZmBIFrame::SchnittpunktLinieAbschliessen(void)
{
    if(m_markierModus && m_schP_Ln && m_schP_OrgPkt)
    {
        m_markierModus = false;
        m_markierModus2 = true;
        SetStatusText(wxT("Zielfläche oder -linie wählen"), 2);
        if(alternativAktLayer)
        {
            aktLayerBAK = aktLayer;
            aktLayer = alternativAktLayer;
        }
    }
    if(m_markierModus2 && m_schP_Obj)
    {
        #if 0
        if((m_schP_Obj->HoleTyp() == RUZ_Dreieck) || (m_schP_Obj->HoleTyp() == RUZ_Viereck))
        {
            if((static_cast<Flaeche*>(m_schP_Obj))->DurchstossPunkt(m_schP_Ln, m_vktSchnittPkt1, m_vktSchnittPkt2, true))
            {
                m_schP_OrgPkt->Positionieren(m_vktSchnittPkt1);
            }else{
                m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
            }
        }else
        #else
        if(!std::isnan(m_vktSchnittPkt1.x()))
        {
            m_schP_OrgPkt->Positionieren(m_vktSchnittPkt1);
        }else{
            m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
        }
        #endif
        if(m_schP_Obj->HoleTyp() == RUZ_Linie)
        {
            Vektor tempOrt;
            if(m_schP_Ln->schneidet((static_cast<Linie*>(m_schP_Obj)), tempOrt, aktProjZ, true))
            {
                m_schP_OrgPkt->Positionieren(tempOrt);
            }else{
                m_schP_OrgPkt->Positionieren(m_schP_alterOrt);
            }
        }
        if(alternativAktLayer)
        {
            aktLayer = aktLayerBAK;
        }
        SetStatusText(wxT("Schnittpunkt: Punkt und Linie wählen"), 2);
        m_markierModus = true;
        m_markierModus2 = false;
        m_schP_Ln = NULL;
        m_schP_Obj = NULL;
        m_schP_OrgPkt = NULL;
        if(!std::isnan(m_vktSchnittPkt1.x()))
            m_vktSchnittPkt1 = NULL_VEKTOR;
        if(!std::isnan(m_vktSchnittPkt2.x()))
            m_vktSchnittPkt2 = NULL_VEKTOR;
    }
    return;
}

bool RUZmBIFrame::SchreibeInDatei(char* dateiName)
{
    std::ofstream Datei;
    int anzEcken;
    Datei.open(dateiName, ios_base::out|ios_base::trunc);
    logSchreiben("%s konnte geoffnet werden\n", dateiName);

    if(Datei.good())
    {
        Datei.setf( ios::fixed, ios::floatfield );
        Datei.precision(15);

        Punkt* tempPunkt;
        HoehenMarke* tempHM;
        Linie* tempLinie;
        Flaeche* tempFlaeche;

        for(RUZ_Layer *tempLayer = m_layer->GetErstesElement(); tempLayer != NULL; tempLayer = m_layer->GetNaechstesElement())
        {
            Datei<<"  0\n  L\n"<<tempLayer->HoleName()<<"\n";
            //if(tempLayer->IstSichtbar() == false)continue;

            Liste<Punkt>* pktSammlung = tempLayer->HolePunkte();
            Liste<HoehenMarke>* hmSammlung = tempLayer->HoleHoehenMarken();
            Liste<Linie>* lnSammlung = tempLayer->HoleLinien();
            Liste<Flaeche>* flSammlung = tempLayer->HoleFlaechen();
            for(tempPunkt = pktSammlung->GetErstesElement(); tempPunkt != NULL; tempPunkt = pktSammlung->GetNaechstesElement())
            {
                Datei<<"  0\nPOINT\n  X\n"<<tempPunkt->HolePosition().x()<<"\n  Y\n"<<-(tempPunkt->HolePosition().y())<<"\n  Z\n"
                    <<tempPunkt->HolePosition().z()<<"\n  @\n"<<tempPunkt<<"\n";
            }

            for(tempLinie = lnSammlung->GetErstesElement(); tempLinie != NULL; tempLinie = lnSammlung->GetNaechstesElement())
            {
                Datei<<"  0\nLINE\n P0\n"<<tempLinie->HolePunkt(0)<<"\n P1\n"<<tempLinie->HolePunkt(1)<<"\n  @\n"<<tempLinie<<"\n";
            }

            for(tempFlaeche = flSammlung->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flSammlung->GetNaechstesElement())
            {
                if(tempFlaeche->HoleTyp() == RUZ_Dreieck)
                {
                    Datei<<"  0\nTRIANGLE\n";
                    anzEcken = 3;
                }else{
                    Datei<<"  0\nQUAD\n";
                    anzEcken = 4;
                }
                for(int k=0; k<anzEcken; k++)
                {
                    Datei<<" L"<<k<<"\n"<<tempFlaeche->HoleLinie(k)<<"\n";
                }
            }
            for(tempHM = hmSammlung->GetErstesElement(); tempHM != NULL; tempHM = hmSammlung->GetNaechstesElement())
            {
                Datei<<"  0\nHOEHENMARKE\n  X\n"<<tempHM->HolePosition().x()<<"\n  Y\n"<<-(tempHM->HolePosition().y())<<"\n";
            }
        }
        Datei<<"  0\n HG\n";
        Liste<Strich>* strichSammlung = m_hintergrundLayer->HoleStriche();
        Liste<Bogen>* bogenSammlung = m_hintergrundLayer->HoleBoegen();
        for(Strich* aktStrich = strichSammlung->GetErstesElement(); aktStrich; aktStrich = strichSammlung->GetNaechstesElement())
        {
            Datei<<"  0\nSTRICH\n X1\n"<<aktStrich->Xa()<<"\n Y1\n"<<-(aktStrich->Ya())<<"\n X2\n"<<aktStrich->Xe()<<"\n Y2\n"<<-(aktStrich->Ye())<<"\n";
        }
        for(Bogen* aktBogen = bogenSammlung->GetErstesElement(); aktBogen; aktBogen = bogenSammlung->GetNaechstesElement())
        {
            Datei<<"  0\nBOGEN\n X1\n"<<aktBogen->Xa()<<"\n Y1\n"<<-(aktBogen->Ya())<<"\n X2\n"<<aktBogen->Xe()<<"\n Y2\n"<<-(aktBogen->Ye())
                    <<"\n XM\n"<<aktBogen->Xm()<<"\n YM\n"<<-(aktBogen->Ym())<<"\n";
        }
        Datei<<"  0\nEOF\n";
    }
    else
    {
        return false;
    }
    Datei.close();
    SetStatusText(strAktuellerSpeicherpfad + wxT(" erfolgreich gespeichert."), 1);
    return true;
}

void RUZmBIFrame::SetzeAktuellenLayer(int neuerAktLayerNr, RUZ_Layer* neuerAktLayer)
{
    if(!neuerAktLayer)return;
    if(aktLayer)aktLayer->GefaelleRasterLoeschen();

    aktLayer = neuerAktLayer;
    aktLayerNr = neuerAktLayerNr;

    this->AddPendingEvent(wxCommandEvent(wxEVT_MENU, idMenuHLZeigen));

    if(gefaelleRasterAnzeigen)
        aktLayer->GefaelleRasterAnlegen(peEinstellungenDlg->HoleWert(IDgefaelleRasterGroesse),
                                        peEinstellungenDlg->HoleWert(IDgefaelleMinimum),
                                        peEinstellungenDlg->HoleWert(IDgefaelleMaximum),
                                        peEinstellungenDlg->HoleWert(IDgefaelleOptimum));
    return;
}

bool RUZmBIFrame::SkalierFaktorenEingabe()
{
    double t_wert = 1.0;
    for(int i = 0; i < 3; i++)
    {
        do
        {
            wxString t_richtung;
            switch(i)
            {
            case 1:
                t_richtung = wxT("y");
                break;
            case 2:
                t_richtung = wxT("z");
                break;
            default:
                t_richtung = wxT("x");
            }
            wxTextEntryDialog abfrage(this, wxT("Bitte geben Sie den Skalierfaktor in ") + t_richtung + wxT("-Richtung an."), wxT("Skalierfaktor"), wxString::Format(wxT("%0.6f"), t_wert));
            if(abfrage.ShowModal() == wxID_ABORT)return false;
            wxString number = abfrage.GetValue();
            /*komma gegen punkt tauschen*/
            wxString punkt = wxT(".");
            wxString komma = wxT(",");
            number.Replace(komma, punkt);
            double value;
            if(!number.ToDouble(&value))
            {
                if(strcmp(number, "")!=0)
                {
                    if(wxMessageDialog(this, wxT("Bitte geben Sie eine Zahl ein"), wxT("Eingabe fehlerhaft")).ShowModal() == wxID_ABORT)return false;
                }else{
                    value = t_wert;
                    continue;
                }
            }
            m_skalFkt[i] = value;
            break;
        }while(1);
    }
    return true;
}

void RUZmBIFrame::SkalierungAusfuehren(Vektor festPkt)
{
    if(m_auswahl->GetListenGroesse() == 0)
    {
        return;
    }
    if(!SkalierFaktorenEingabe())
    {
        BefehleZuruecksetzen();
        return;
    }
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        (obj)->Skalieren(festPkt, m_skalFkt[0], m_skalFkt[1], m_skalFkt[2]);
    }
    m_markierModus = true;
    KoordinatenMaske->Show(false);
    SetStatusText(wxT("Objekte zum Skalieren auswählen"), 2);
    return;
}

void RUZmBIFrame::SkalierungSetzen(double t_skal)
{
	m_skalierung = t_skal;
	mbSkalierung.MassstabErmitteln(m_skalierung);
	return;
}

bool RUZmBIFrame::ObjektMarkieren(int xPos, int yPos)
{
    RUZ_Layer* layer_laeufer = aktLayer;
    markiertesObjekt = NULL;
    Liste<Punkt>* pktLst = layer_laeufer->HolePunkte();
    Liste<Kreis>* krLst = layer_laeufer->HoleKreise();
    Liste<Linie>* lnLst = layer_laeufer->HoleLinien();
    Liste<Flaeche>* flLst = layer_laeufer->HoleFlaechen();
    Liste<HoehenMarke>* hmLst = layer_laeufer->HoleHoehenMarken();
    Liste<Fangpunkt>* fngPktLst = layer_laeufer->HoleFangpunkte();

    if(m_zeigePunkt && m_waehlePunkt)
    {
        for(Punkt* aktPkt = pktLst->GetErstesElement(); aktPkt != NULL; aktPkt = pktLst->GetNaechstesElement())
        {
            if(aktPkt == m_aktPunkt)continue;
            if(aktPkt->IstNahebei((xPos / m_skalierung) + dc_Offset[0],
                                  (yPos / m_skalierung) + dc_Offset[1], pxSuchEntfernung / m_skalierung, aktProjZ))
            {
                markiertesObjekt = aktPkt;
                return true;
            }
        }
    }
    if(m_zeigeFangpunkt && m_waehleFangpunkt)
    {
        for(Fangpunkt* aktfngPkt = fngPktLst->GetErstesElement(); aktfngPkt != NULL; aktfngPkt = fngPktLst->GetNaechstesElement())
        {
            if(aktfngPkt->IstNahebei((xPos / m_skalierung) + dc_Offset[0],
                                  (yPos / m_skalierung) + dc_Offset[1], 1.2f * 1.4142f * pxSuchEntfernung / m_skalierung, aktProjZ))
            {
                markiertesObjekt = aktfngPkt;
                return true;
            }
        }
    }
    if(m_zeigeKreis && m_waehleKreis)
    {
        for(Kreis* aktKreis = krLst->GetErstesElement(); aktKreis != NULL; aktKreis = krLst->GetNaechstesElement())
        {
            if(aktKreis->IstNahebei((xPos / m_skalierung) + dc_Offset[0],
                                  (yPos / m_skalierung) + dc_Offset[1], pxSuchEntfernung / m_skalierung, aktProjZ))
            {
                markiertesObjekt = aktKreis;
                return true;
            }
        }
    }
    if(m_zeigeHoehenmarke && m_waehleHoehenmarke)
    {
        for(HoehenMarke* aktHM = hmLst->GetErstesElement(); aktHM != NULL; aktHM = hmLst->GetNaechstesElement())
        {
            if(aktHM->IstNahebei((xPos / m_skalierung) + dc_Offset[0],
                                  (yPos / m_skalierung) + dc_Offset[1], pxSuchEntfernung / (3 * m_skalierung), aktProjZ))
            {
                markiertesObjekt = aktHM;
                return true;
            }
        }
    }
    if((aktBefehl != bef_ID_versetzen))
    {
        if(m_zeigeLinie && m_waehleLinie)
        {
            for(Linie* aktLn = lnLst->GetErstesElement(); aktLn != NULL; aktLn = lnLst->GetNaechstesElement())
            {
                if(aktLn == m_aktLinie)continue;
                if(aktLn->IstNahebei((xPos / m_skalierung) + dc_Offset[0],
                                      (yPos / m_skalierung) + dc_Offset[1], pxSuchEntfernung / m_skalierung, aktProjZ))
                {
                    markiertesObjekt = aktLn;
                    return true;
                }
            }
        }
        if(m_zeigeFlaeche && m_waehleFlaeche)
        {
            for(Flaeche* aktFl = flLst->GetErstesElement(); aktFl != NULL; aktFl = flLst->GetNaechstesElement())
            {
                if(aktFl->IstInnerhalb((xPos/m_skalierung)+dc_Offset[0], (yPos/m_skalierung)+dc_Offset[1], aktProjZ))
                {
                    if(aktFl != markiertesObjekt)
                    {
                        markiertesObjekt = aktFl;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void RUZmBIFrame::VereinigenErsterPkt(Punkt *pkt)
{
    vereinOrigPkt = pkt;
    SetStatusText(wxT("Durch welchen Punkt ersetzen?"), 2);
    Vereinigen = &RUZmBIFrame::VereinigenZweiterPkt;
    return;
}

void RUZmBIFrame::VereinigenZweiterPkt(Punkt *pkt)
{
    if(vereinOrigPkt == pkt)return;/*das Vereinigen eines Punktes mit sich selber verhindern*/
    vereinErsatzPkt = pkt;
    SetStatusText(wxT("Punkt wählen"), 2);
    Vereinigen = &RUZmBIFrame::VereinigenErsterPkt;

    vereinOrigPkt->ErsetzenDurch(vereinErsatzPkt);

    return;
}

void RUZmBIFrame::VerschneideAuswahlFlaechen(void)
{
    ObjekteNullen();
    Liste<Flaeche>* schnittListe = new Liste<Flaeche>();
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        if((obj->HoleTyp() == RUZ_Viereck)||(obj->HoleTyp() == RUZ_Dreieck))
        {
            schnittListe->Hinzufuegen(static_cast<Flaeche*>(obj));
        }
        m_auswahl->Entfernen(obj);
    }
    m_auswahl->ListeLeeren("VerschneideAuswahlFlaechen(void)");
    aktLayer->VerschneideFlaechen(schnittListe, m_anzeigeGenauigkeit+3);

    schnittListe->ListeLeeren("VerschneideAuswahlFlaechen(void)");
    delete schnittListe;
    return;
}

void RUZmBIFrame::ViereckeAuswahlTeilen(void)
{
    markiertesObjekt = NULL;
    objFang1 = objFang2 = NULL;
    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Viereck)
        {
            m_auswahl->Entfernen(obj);
            (static_cast<Viereck*>(obj))->Teilen();
        }
    }
    m_auswahl->ListeLeeren("ViereckeAuswahlTeilen(void)");
    return;
}

void RUZmBIFrame::Vschb_Abbrechen(void)
{
    if(vVerschubStart)
    {
        Vschb_Verschieben(*vVerschubStart);
        delete vVerschubStart;
        vVerschubStart = NULL;
        m_markierModus = true;
        KoordinatenMaske->Show(false);
        m_verschubAuswahlOrte->ListeLoeschen("");
        SetStatusText(wxT("Objekte zum Verschieben auswählen"), 2);
    }else
    if(m_markierModus)
    {
        if(m_auswahl->GetListenGroesse() != 0)
        {
            m_auswahl->ListeLeeren("");
        }else
        {
            BefehleZuruecksetzen();
        }
    }else
    {
        m_markierModus = true;
        KoordinatenMaske->Show(false);
        m_verschubAuswahlOrte->ListeLoeschen("");
        SetStatusText(wxT("Objekte zum Verschieben auswählen"), 2);
    }
    return;
}

void RUZmBIFrame::Vschb_Auswahl_Bestaetigung(void)
{
    if(m_markierModus)
    {
        m_markierModus = false;
        Vschb_Punktspeicher_Schreiben();
        KoordinatenMaske->Show();
        SetStatusText(wxT("Objekte verschieben: 'Von' (Punkt wählen / eingeben)"), 2);
    }
    else
    {
        m_markierModus = true;
        KoordinatenMaske->Show(false);
        m_verschubAuswahlOrte->ListeLoeschen("");
        SetStatusText(wxT("Objekte zum Verschieben auswählen"), 2);
    }
    return;
}

bool RUZmBIFrame::Vschb_Ort_Exklusiv_Hinzufuegen(PunktSpeicher* _pktSp)
{
    for(PunktSpeicher* aktPktSp = m_verschubAuswahlOrte->GetErstesElement(); aktPktSp != NULL; aktPktSp = m_verschubAuswahlOrte->GetNaechstesElement())
    {
        if(aktPktSp->HoleObj() == _pktSp->HoleObj())
            return 0;
    }
    m_verschubAuswahlOrte->Hinzufuegen(_pktSp);
    return 1;
}

void RUZmBIFrame::Vschb_Punkt_Festlegen(Vektor vkt)
{
    if(vVerschubStart)
    {
        Vschb_Verschieben(vkt);
        delete vVerschubStart;
        vVerschubStart = NULL;

        m_markierModus = true;
        KoordinatenMaske->Show(false);
        m_verschubAuswahlOrte->ListeLoeschen("");
        SetStatusText(wxT("Auswahl verschieben: 'Von' (Punkt wählen / eingeben)"), 2);
    }else
    {
        vVerschubStart = new Vektor(vkt);
        if(vVerschubStart)
            SetStatusText(wxT("Auswahl verschieben: 'Nach' (Punkt wählen / eingeben)"), 2);
    }
    return;
}

void RUZmBIFrame::Vschb_Punktspeicher_Schreiben(void)
{
    m_verschubAuswahlOrte->ListeLoeschen("Vschb_Punktspeicher_Schreiben");
    PunktSpeicher* pktSp = NULL;

    for(RUZ_Objekt* obj = m_auswahl->GetErstesElement(); obj != NULL; obj = m_auswahl->GetNaechstesElement())
    {
        if(obj->HoleTyp() == RUZ_Punkt)
        {
            pktSp = new PunktSpeicher(static_cast<Punkt*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Linie)
        {
            Linie* ln = static_cast<Linie*>(obj);
            //m_auswahl->Entfernen(obj);
            for (int i = 0; i < 2; i++)
            {
                pktSp = new PunktSpeicher(ln->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_Dreieck)
        {
            Dreieck* drk = static_cast<Dreieck*>(obj);
            //m_auswahl->Entfernen(obj);
            for (int i = 0; i < 3; i++)
            {
                pktSp = new PunktSpeicher(drk->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_Viereck)
        {
            Viereck* vrk = static_cast<Viereck*>(obj);
            //m_auswahl->Entfernen(obj);
            for (int i = 0; i < 4; i++)
            {
                pktSp = new PunktSpeicher(vrk->HolePunkt(i));
                if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
                {
                    delete pktSp;
                    pktSp = NULL;
                }
            }
        }else
        if(obj->HoleTyp() == RUZ_HoehenMarke)
        {
            pktSp = new PunktSpeicher(static_cast<HoehenMarke*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Kreis)
        {
            pktSp = new PunktSpeicher(static_cast<Kreis*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        if(obj->HoleTyp() == RUZ_Fangpunkt)
        {
            pktSp = new PunktSpeicher(static_cast<Fangpunkt*>(obj));
            if(!(Vschb_Ort_Exklusiv_Hinzufuegen(pktSp)))
            {
                delete pktSp;
                pktSp = NULL;
            }
        }else
        {
            m_auswahl->Entfernen(obj);
        }
    }
    return;
}

void RUZmBIFrame::Vschb_Verschieben(Vektor vkt)
{
    if(vVerschubStart)
    {
        RUZ_Objekt* obj;
        for(PunktSpeicher* aktPktSp = m_verschubAuswahlOrte->GetErstesElement(); aktPktSp != NULL; aktPktSp = m_verschubAuswahlOrte->GetNaechstesElement())
        {
            obj = aktPktSp->HoleObj();
            if(obj->HoleTyp() == RUZ_Punkt)
                static_cast<Punkt*>(obj)->Positionieren(aktPktSp->HoleOrt() + (vkt - *vVerschubStart));

            if(obj->HoleTyp() == RUZ_HoehenMarke)
                static_cast<HoehenMarke*>(obj)->Positionieren(aktPktSp->HoleOrt() + (vkt - *vVerschubStart));

            if(obj->HoleTyp() == RUZ_Kreis)
                static_cast<Kreis*>(obj)->Positionieren(aktPktSp->HoleOrt() + (vkt - *vVerschubStart));

            if(obj->HoleTyp() == RUZ_Fangpunkt)
                static_cast<Fangpunkt*>(obj)->Positionieren(aktPktSp->HoleOrt() + (vkt - *vVerschubStart));
        }
    }
    return;
}

/*ENDE RUZmBI*/

/*Programm_Einstellungen_Dialog*/
BEGIN_EVENT_TABLE(Programm_Einstellungen_Dialog, wxDialog)
    EVT_CLOSE(Programm_Einstellungen_Dialog::OnClose)
    EVT_TEXT(wxID_ANY, Programm_Einstellungen_Dialog::OnTextChange)
    EVT_TEXT_ENTER(wxID_ANY, Programm_Einstellungen_Dialog::OnTextChange)
    EVT_BUTTON(wxID_OK, Programm_Einstellungen_Dialog::OnOKButton)
END_EVENT_TABLE()

Programm_Einstellungen_Dialog::Programm_Einstellungen_Dialog(wxWindow* parent):wxDialog(parent, wxID_ANY, wxT("Allgmeine Programmeinstellungen"))
{
    m_mama = parent;
    PEDialogIni();

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *parameterSizer = new wxBoxSizer(wxHORIZONTAL);

    /*Programmsteuerung*/
    wxBoxSizer *steuerungsSizer = new wxBoxSizer(wxVERTICAL);
    steuerungsSizer->Add(new wxTextCtrl(this, -1, wxT("Programmsteuerung"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);

    wxBoxSizer *verschubWeiteSizer = new wxBoxSizer(wxHORIZONTAL);
    verschubWeiteSizer->Add(new wxTextCtrl(this, -1, wxT("Schrittweite Verschub"), wxDefaultPosition, wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *verschubWeiteCtrl = new aruDblTxtCtrl(this, IDverschubWeite, wxString::Format(wxT("%.2f"), pe_verschubWeite), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("verschubWeiteCtrl"));
    verschubWeiteSizer->Add(verschubWeiteCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(verschubWeiteSizer, 1, wxEXPAND, 5);

    wxBoxSizer *pxSuchEntfernungSizer = new wxBoxSizer(wxHORIZONTAL);
    pxSuchEntfernungSizer->Add(new wxTextCtrl(this, -1, wxT("max. Abstand Markierung (px)"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *pxSuchEntfernungCtrl = new aruDblTxtCtrl(this, IDpxSuchEntfernung, wxString::Format(wxT("%.2f"), pe_pxSuchEntfernung), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("pxSuchEntfernungCtrl"));
    pxSuchEntfernungSizer->Add(pxSuchEntfernungCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(pxSuchEntfernungSizer, 1, wxEXPAND, 5);

    wxBoxSizer *lnWandelGenauigkeitSizer = new wxBoxSizer(wxHORIZONTAL);
    lnWandelGenauigkeitSizer->Add(new wxTextCtrl(this, -1, wxT("Genauigkeit Strich->Linie"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *lnWandelGenauigkeitCtrl = new aruDblTxtCtrl(this, IDlnWandelGenauigkeit, wxString::Format(wxT("%.2f"), pe_lnWandelGenauigkeit), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("lnWandelGenauigkeitCtrl"));
    lnWandelGenauigkeitSizer->Add(lnWandelGenauigkeitCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(lnWandelGenauigkeitSizer, 1, wxEXPAND, 5);

    wxBoxSizer *anzeigeGenauigkeitSizer = new wxBoxSizer(wxHORIZONTAL);
    anzeigeGenauigkeitSizer->Add(new wxTextCtrl(this, -1, wxT("Anzeigegenauigkeit (Nachkommastellen Zahlen)"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *anzeigeGenauigkeitCtrl = new aruDblTxtCtrl(this, IDanzeigeGenauigkeit, wxString::Format(wxT("%.2f"), pe_anzeigeGenauigkeit), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("anzeigeGenauigkeitCtrl"));
    anzeigeGenauigkeitSizer->Add(anzeigeGenauigkeitCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(anzeigeGenauigkeitSizer, 1, wxEXPAND, 5);

    wxBoxSizer *pseudoSchattenFktSizer = new wxBoxSizer(wxHORIZONTAL);
    pseudoSchattenFktSizer->Add(new wxTextCtrl(this, -1, wxT("Pseudoschatten anpassen"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *pseudoSchattenFktCtrl = new aruDblTxtCtrl(this, IDpseudoSchattenFkt, wxString::Format(wxT("%.2f"), pe_pseudoSchattenFkt), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("pseudoSchattenFktCtrl"));
    pseudoSchattenFktSizer->Add(pseudoSchattenFktCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(pseudoSchattenFktSizer, 1, wxEXPAND, 5);

    wxBoxSizer *gefaelleRasterSizer = new wxBoxSizer(wxHORIZONTAL);
    gefaelleRasterSizer->Add(new wxTextCtrl(this, -1, wxT("Rastergröße Gefälle"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *rasterGroesseCtrl = new aruDblTxtCtrl(this, IDgefaelleRasterGroesse, wxString::Format(wxT("%.2f"), pe_gefaelleRasterGroesse), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("rasterGroesseCtrl"));
    gefaelleRasterSizer->Add(rasterGroesseCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(gefaelleRasterSizer, 1, wxEXPAND, 5);

    wxBoxSizer *gefaelleMinimumSizer = new wxBoxSizer(wxHORIZONTAL);
    gefaelleMinimumSizer->Add(new wxTextCtrl(this, -1, wxT("Mindestgefälle [%]"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *gefaelleMinimumCtrl = new aruDblTxtCtrl(this, IDgefaelleMinimum, wxString::Format(wxT("%.2f"), pe_gefaelleMinimum), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("gefaelleMinimumCtrl"));
    gefaelleMinimumSizer->Add(gefaelleMinimumCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(gefaelleMinimumSizer, 1, wxEXPAND, 5);

    wxBoxSizer *gefaelleOptimumSizer = new wxBoxSizer(wxHORIZONTAL);
    gefaelleOptimumSizer->Add(new wxTextCtrl(this, -1, wxT("optimales Gefälle [%]"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *gefaelleOptimumCtrl = new aruDblTxtCtrl(this, IDgefaelleOptimum, wxString::Format(wxT("%.2f"), pe_gefaelleOptimum), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("gefaelleOptimumCtrl"));
    gefaelleOptimumSizer->Add(gefaelleOptimumCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(gefaelleOptimumSizer, 1, wxEXPAND, 5);

    wxBoxSizer *gefaelleMaximumSizer = new wxBoxSizer(wxHORIZONTAL);
    gefaelleMaximumSizer->Add(new wxTextCtrl(this, -1, wxT("Maximalgefälle [%]"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *gefaelleMaximumCtrl = new aruDblTxtCtrl(this, IDgefaelleMaximum, wxString::Format(wxT("%.2f"), pe_gefaelleMaximum), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("gefaelleMaximumCtrl"));
    gefaelleMaximumSizer->Add(gefaelleMaximumCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(gefaelleMaximumSizer, 1, wxEXPAND, 5);

    wxBoxSizer *flaechenRasterSizer = new wxBoxSizer(wxHORIZONTAL);
    flaechenRasterSizer->Add(new wxTextCtrl(this, -1, wxT("Rastergröße Flächenberechnung"), wxDefaultPosition,  wxSize(180, 20), wxTE_READONLY|wxBORDER_NONE|wxTE_BESTWRAP),0, wxEXPAND|wxALL, 5);
    aruDblTxtCtrl *flaechenGroesseCtrl = new aruDblTxtCtrl(this, IDflaechenRasterGroesse, wxString::Format(wxT("%.2f"), pe_flaechenRasterGroesse), wxDefaultPosition, wxSize(50, 20),
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("flaechenGroesseCtrl"));
    flaechenRasterSizer->Add(flaechenGroesseCtrl, 0, wxALL, 5);
    steuerungsSizer->Add(flaechenRasterSizer, 1, wxEXPAND, 5);
    /*ENDE Programmsteuerung*/

    /*Farben*/
    wxBoxSizer *farbenSizer = new wxBoxSizer(wxVERTICAL);
    farbenSizer->Add(new wxTextCtrl(this, -1, wxT("Farbauswahl"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE), 0, wxEXPAND|wxALL, 5);

    wxBoxSizer *col_Pkt_Ln_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_Pkt_Ln_Sizer->Add(new wxTextCtrl(this, -1, wxT("Punkte und Linien"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_Pkt_Ln_Button = new wxButton(this, IDFarbePktLn, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_Pkt_Ln_Button->SetBackgroundColour(pe_FarbePktLn);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbePktLn);
        col_Pkt_Ln_Sizer->Add(col_Pkt_Ln_Button, 0,wxALL, 5);
    farbenSizer->Add(col_Pkt_Ln_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_Strich_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_Strich_Sizer->Add(new wxTextCtrl(this, -1, wxT("Striche"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_Strich_Button = new wxButton(this, IDFarbeStrich, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_Strich_Button->SetBackgroundColour(pe_FarbeStrich);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeStrich);
        col_Strich_Sizer->Add(col_Strich_Button, 0,wxALL, 5);
    farbenSizer->Add(col_Strich_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_HM_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_HM_Sizer->Add(new wxTextCtrl(this, -1, wxT("Höhenmarken"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_HM_Button = new wxButton(this, IDFarbeHoehenMarke, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_HM_Button->SetBackgroundColour(pe_FarbeHoehenMarke);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeHoehenMarke);
        col_HM_Sizer->Add(col_HM_Button, 0,wxALL, 5);
    farbenSizer->Add(col_HM_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_HL_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_HL_Sizer->Add(new wxTextCtrl(this, -1, wxT("Höhenlinien"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_HL_Button = new wxButton(this, IDFarbeHoehenlinie, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_HL_Button->SetBackgroundColour(pe_FarbeHoehenlinie);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeHoehenlinie);
        col_HL_Sizer->Add(col_HL_Button, 0,wxALL, 5);
    farbenSizer->Add(col_HL_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_mark_Obj_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_mark_Obj_Sizer->Add(new wxTextCtrl(this, -1, wxT("Markierte Objekte"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_mark_Obj_Button = new wxButton(this, IDFarbeMarkiertesObjekt, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_mark_Obj_Button->SetBackgroundColour(pe_FarbeMarkiertesObjekt);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeMarkiertesObjekt);
        col_mark_Obj_Sizer->Add(col_mark_Obj_Button, 0,wxALL, 5);
    farbenSizer->Add(col_mark_Obj_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_ausgew_Obj_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_ausgew_Obj_Sizer->Add(new wxTextCtrl(this, -1, wxT("Ausgewählte Objekte"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_ausgew_Obj_Button = new wxButton(this, IDFarbeAusgewaehltesObjekt, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_ausgew_Obj_Button->SetBackgroundColour(pe_FarbeAusgewaehltesObjekt);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeAusgewaehltesObjekt);
        col_ausgew_Obj_Sizer->Add(col_ausgew_Obj_Button, 0,wxALL, 5);
    farbenSizer->Add(col_ausgew_Obj_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_flaeche_darueber_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_flaeche_darueber_Sizer->Add(new wxTextCtrl(this, -1, wxT("Flächen oberhalb"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_flaeche_darueber_Button = new wxButton(this, IDFarbeFlaecheDarueber, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_flaeche_darueber_Button->SetBackgroundColour(pe_FarbeFlaecheDarueber);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeFlaecheDarueber);
        col_flaeche_darueber_Sizer->Add(col_flaeche_darueber_Button, 0,wxALL, 5);
    farbenSizer->Add(col_flaeche_darueber_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_flaeche_darunter_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_flaeche_darunter_Sizer->Add(new wxTextCtrl(this, -1, wxT("Flächen unterhalb"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_flaeche_darunter_Button = new wxButton(this, IDFarbeFlaecheDarunter, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_flaeche_darunter_Button->SetBackgroundColour(pe_FarbeFlaecheDarunter);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeFlaecheDarunter);
        col_flaeche_darunter_Sizer->Add(col_flaeche_darunter_Button, 0,wxALL, 5);
    farbenSizer->Add(col_flaeche_darunter_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_Hintergrund_Layer_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_Hintergrund_Layer_Sizer->Add(new wxTextCtrl(this, -1, wxT("Hintergrundlayer"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_Hintergrund_Layer_Button = new wxButton(this, IDFarbeHintergrundLayer, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_Hintergrund_Layer_Button->SetBackgroundColour(pe_FarbeHintergrundLayer);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeHintergrundLayer);
        col_Hintergrund_Layer_Sizer->Add(col_Hintergrund_Layer_Button, 0,wxALL, 5);
    farbenSizer->Add(col_Hintergrund_Layer_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_ZeichenHintergrund_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_ZeichenHintergrund_Sizer->Add(new wxTextCtrl(this, -1, wxT("Hintergrund Zeichenfl."), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_ZeichenHintergrund_Button = new wxButton(this, IDFarbeZeichenHintergrund, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_ZeichenHintergrund_Button->SetBackgroundColour(pe_FarbeZeichenHintergrund);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeZeichenHintergrund);
        col_ZeichenHintergrund_Sizer->Add(col_ZeichenHintergrund_Button, 0,wxALL, 5);
    farbenSizer->Add(col_ZeichenHintergrund_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_AuswahlRechteck_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_AuswahlRechteck_Sizer->Add(new wxTextCtrl(this, -1, wxT("Auswahlrechteck"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_AuswahlRechteck_Button = new wxButton(this, IDFarbeAuswahlRechteck, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_AuswahlRechteck_Button->SetBackgroundColour(pe_FarbeAuswahlRechteck);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeAuswahlRechteck);
        col_AuswahlRechteck_Sizer->Add(col_AuswahlRechteck_Button, 0,wxALL, 5);
    farbenSizer->Add(col_AuswahlRechteck_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_Gefaelle_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_Gefaelle_Sizer->Add(new wxTextCtrl(this, -1, wxT("Gefälle"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_Gefaelle_Button = new wxButton(this, IDFarbeGefaelle, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_Gefaelle_Button->SetBackgroundColour(pe_FarbeGefaelle);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeGefaelle);
        col_Gefaelle_Sizer->Add(col_Gefaelle_Button, 0,wxALL, 5);
    farbenSizer->Add(col_Gefaelle_Sizer, 1, wxEXPAND, 5);

    wxBoxSizer *col_Fangpunkt_Sizer = new wxBoxSizer(wxHORIZONTAL);
        col_Fangpunkt_Sizer->Add(new wxTextCtrl(this, -1, wxT("Fangpunkte"), wxDefaultPosition,  wxSize(150, 20), wxTE_READONLY|wxBORDER_NONE),0, wxALL, 5);
            wxButton *col_Fangpunkt_Button = new wxButton(this, IDFarbeFangpunkt, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
                col_Fangpunkt_Button->SetBackgroundColour(pe_FarbeFangpunkt);
            Bind(wxEVT_BUTTON, &Programm_Einstellungen_Dialog::OnColourButton, this, IDFarbeFangpunkt);
        col_Fangpunkt_Sizer->Add(col_Fangpunkt_Button, 0,wxALL, 5);
    farbenSizer->Add(col_Fangpunkt_Sizer, 1, wxEXPAND, 5);
    /*ENDE Farben*/


    topsizer->Add(new wxTextCtrl(this, -1, wxT("Einstellungen"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    parameterSizer->Add(steuerungsSizer, 0, 0, 5);
    parameterSizer->Add(farbenSizer, 0, 0, 5);
    topsizer->Add(parameterSizer, 0, wxEXPAND, 5);
    topsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

Programm_Einstellungen_Dialog::~Programm_Einstellungen_Dialog()
{

}

void Programm_Einstellungen_Dialog::OnClose(wxCloseEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void Programm_Einstellungen_Dialog::PEDialogIni(void)
{
    /*Standardwerte setzen*/
    pe_verschubWeite = 0.01;                    //Schrittweite des Verschubs der Höhen
    pe_gefaelleRasterGroesse = 1.0;             //Rastergrösse in dem die Gefällemarken gesetzt werden
    pe_gefaelleMinimum = 1.8;             //Unteres Ende der Farbskala des Gefälles
    pe_gefaelleOptimum = 2.0;             //Mittelpunkt der Farbskala des Gefälles
    pe_gefaelleMaximum = 2.5;             //Oberes Ende der Farbskala des Gefälles
    pe_flaechenRasterGroesse = 1.0;             //Rastergrösse in dem Flächenpunkte berechnet werden (Integral, Schattierung etc.)
    pe_pxSuchEntfernung = 8;                    //Entfernung der Objekte zum Cursor in Pixeln bis zu der diese 'in der Nähe' sind
    pe_lnWandelGenauigkeit = 0.01;              //Entfernung von Linienendpunkt zu einem Punkt (absolut) bis zu dem diese als identisch erkannt werden
    pe_anzeigeGenauigkeit = 2;                  //Nachkommastellen der Zahlen
    pe_pseudoSchattenFkt = 1.0;                 //Faktor zur Anpassung der Schattenwirkung
    pe_FarbePktLn = wxColour(0, 0, 0);          //Farbe der Punkt und Linien
    pe_FarbeStrich = wxColour(74, 186, 240);    //usw.
    pe_FarbeHoehenMarke = wxColour(0, 128, 0);
    pe_FarbeHoehenlinie = wxColour(0, 0, 128);
    pe_FarbeMarkiertesObjekt = wxColour(255, 64, 64);
    pe_FarbeAusgewaehltesObjekt = wxColour(60, 160, 60);
    pe_FarbeHintergrundLayer = wxColour(145,145,64);
    pe_FarbeZeichenHintergrund = m_mama->GetBackgroundColour();
    pe_FarbeAuswahlRechteck = wxColour(255, 255, 240);
    pe_FarbeFlaecheDarueber = wxColour(180, 40, 0);
    pe_FarbeFlaecheDarunter = wxColour(40, 180, 0);
    pe_FarbeGefaelle = wxColour(0, 180, 255);
    pe_FarbeGefaelle = wxColour(85, 170, 255);

    char Zeile[50];
	ifstream PE_Parameter_Ini_Datei("Programmeinstellungen.ini", ios::in);
	if(PE_Parameter_Ini_Datei.good())
	{
	    double r, g, b;
		//cout<<"Neue_Ini_Datei existiert"<<endl;
		while(!PE_Parameter_Ini_Datei.eof())
		{
			PE_Parameter_Ini_Datei.getline(Zeile,50,':');
			if(!strcmp(Zeile,"verschubWeite"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_verschubWeite = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"flaechenRasterGroesse"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_flaechenRasterGroesse = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"gefaelleRasterGroesse"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_gefaelleRasterGroesse = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"gefaelleMinimum"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_gefaelleMinimum = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"gefaelleOptimum"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_gefaelleOptimum = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"gefaelleMaximum"))
			{
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				pe_gefaelleMaximum = atof(Zeile);
				continue;
			}
            if(!strcmp(Zeile,"pxSuchEntfernung"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50);
                pe_pxSuchEntfernung = atof(Zeile);
				continue;
			}
            if(!strcmp(Zeile,"lnWandelGenauigkeit"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50);
                pe_lnWandelGenauigkeit = atof(Zeile);
				continue;
			}
            if(!strcmp(Zeile,"anzeigeGenauigkeit"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50);
                pe_anzeigeGenauigkeit = atof(Zeile);
				continue;
			}
            if(!strcmp(Zeile,"pseudoSchattenFkt"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50);
                pe_pseudoSchattenFkt = atof(Zeile);
				continue;
			}
            if(!strcmp(Zeile,"FarbePktLn"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbePktLn = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeStrich"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeStrich = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeHoehenMarke"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeHoehenMarke = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeHoehenlinie"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeHoehenlinie = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeMarkiertesObjekt"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeMarkiertesObjekt = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeAusgewaehltesObjekt"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeAusgewaehltesObjekt = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeHintergrundLayer"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeHintergrundLayer = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeZeichenHintergrund"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeZeichenHintergrund = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeAuswahlRechteck"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeAuswahlRechteck = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeFlaecheDarueber"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeFlaecheDarueber = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeFlaecheDarunter"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeFlaecheDarunter = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeGefaelle"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeGefaelle = wxColour(r, g, b);
				continue;
			}
            if(!strcmp(Zeile,"FarbeFangpunkt"))
            {
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				r = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50,',');
				g = atof(Zeile);
				PE_Parameter_Ini_Datei.getline(Zeile,50);
				b = atof(Zeile);
                pe_FarbeFangpunkt = wxColour(r, g, b);
				continue;
			}
		}
		PE_Parameter_Ini_Datei.close();
	}
	else{
        wxMessageDialog(m_mama, wxString("Keine Ini-Datei")).ShowModal();
        IniSpeichern();
	}
    return;
}

void Programm_Einstellungen_Dialog::OnOKButton(wxCommandEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void Programm_Einstellungen_Dialog::OnTextChange(wxCommandEvent &event)
{
    wxString inhalt = event.GetString();
    KommaGegenPunkt(&inhalt);
    int textFeldID = event.GetId();
    double uebergabe = -1;
    switch(textFeldID)
    {
    case IDverschubWeite:
        uebergabe = pe_verschubWeite = atof(inhalt);
        break;
    case IDgefaelleMinimum:
        uebergabe = pe_gefaelleMinimum = atof(inhalt);
        break;
    case IDgefaelleOptimum:
        uebergabe = pe_gefaelleOptimum = atof(inhalt);
        break;
    case IDgefaelleMaximum:
        uebergabe = pe_gefaelleMaximum = atof(inhalt);
        break;
    case IDgefaelleRasterGroesse:
        uebergabe = pe_gefaelleRasterGroesse = atof(inhalt);
        break;
    case IDflaechenRasterGroesse:
        uebergabe = pe_flaechenRasterGroesse = atof(inhalt);
        break;
    case IDpxSuchEntfernung:
        uebergabe = pe_pxSuchEntfernung = atof(inhalt);
        break;
    case IDlnWandelGenauigkeit:
        uebergabe = pe_lnWandelGenauigkeit = atof(inhalt);
        break;
    case IDanzeigeGenauigkeit:
        uebergabe = pe_anzeigeGenauigkeit = atof(inhalt);
        break;
    case IDpseudoSchattenFkt:
        pe_pseudoSchattenFkt = atof(inhalt);
        if(pe_pseudoSchattenFkt < 0)
        {
            pe_pseudoSchattenFkt = 0;
        }
        uebergabe = pe_pseudoSchattenFkt;
        break;
    case IDFarbePktLn:
    case IDFarbeHoehenMarke:
    case IDFarbeHoehenlinie:
    case IDFarbeMarkiertesObjekt:
    case IDFarbeAusgewaehltesObjekt:
    case IDFarbeHintergrundLayer:
    case IDFarbeZeichenHintergrund:
    case IDFarbeAuswahlRechteck:
    case IDFarbeFlaecheDarueber:
    case IDFarbeFlaecheDarunter:
    case IDFarbeGefaelle:
    case IDFarbeFangpunkt:
        return;
    }
    aruDblEvent *myevent = new aruDblEvent();
    myevent->SetzeWert(uebergabe);
    myevent->SetId(event.GetId());
    m_mama->GetEventHandler()->AddPendingEvent(*myevent);
    return;
}

void Programm_Einstellungen_Dialog::OnColourButton(wxCommandEvent &event)
{
    wxColour farb;
    int ButtonID = event.GetId();
    wxColour farbUebergabe;
    wxString Title;
    switch(ButtonID)
    {
    case IDFarbePktLn:
        farbUebergabe = pe_FarbePktLn;
        Title = wxT("Punkte und Linien");
        break;
    case IDFarbeStrich:
        farbUebergabe = pe_FarbeStrich;
        Title = wxT("Striche");
        break;
    case IDFarbeHoehenMarke:
        farbUebergabe = pe_FarbeHoehenMarke;
        Title = wxT("Höhenmarken");
        break;
    case IDFarbeHoehenlinie:
        farbUebergabe = pe_FarbeHoehenlinie;
        Title = wxT("Höhenlinien");
        break;
    case IDFarbeMarkiertesObjekt:
        farbUebergabe = pe_FarbeMarkiertesObjekt;
        Title = wxT("Markierte Objekte");
        break;
    case IDFarbeAusgewaehltesObjekt:
        farbUebergabe = pe_FarbeAusgewaehltesObjekt;
        Title = wxT("Ausgewählte Objkete");
        break;
    case IDFarbeHintergrundLayer:
        farbUebergabe = pe_FarbeHintergrundLayer;
        Title = wxT("Hintergrundlayer");
        break;
    case IDFarbeZeichenHintergrund:
        farbUebergabe = pe_FarbeZeichenHintergrund;
        Title = wxT("Hintergrund Zeichenfläche");
        break;
    case IDFarbeAuswahlRechteck:
        farbUebergabe = pe_FarbeAuswahlRechteck;
        Title = wxT("Auswahlrechteck");
        break;
    case IDFarbeFlaecheDarueber:
        farbUebergabe = pe_FarbeFlaecheDarueber;
        Title = wxT("Farbe oberhalb");
        break;
    case IDFarbeFlaecheDarunter:
        farbUebergabe = pe_FarbeFlaecheDarunter;
        Title = wxT("Farbe unterhalb");
        break;
    case IDFarbeGefaelle:
        farbUebergabe = pe_FarbeGefaelle;
        Title = wxT("Gefälle");
        break;
    case IDFarbeFangpunkt:
        farbUebergabe = pe_FarbeFangpunkt;
        Title = wxT("Fangpunkte");
        break;
    }
    wxColourData farbDaten;
    farbDaten.SetColour(farbUebergabe);
    wxColourDialog CoDi((wxWindow*)this, &farbDaten);
    CoDi.SetTitle(Title);
    CoDi.ShowModal();
    farbDaten = CoDi.GetColourData();
    farbUebergabe = farbDaten.GetColour();

    static_cast<wxButton*>(event.GetEventObject())->SetBackgroundColour(farbUebergabe);

    switch(ButtonID)
    {
    case IDFarbePktLn:
        pe_FarbePktLn = farbUebergabe;
        break;
    case IDFarbeStrich:
        pe_FarbeStrich = farbUebergabe;
        break;
    case IDFarbeHoehenMarke:
        pe_FarbeHoehenMarke = farbUebergabe;
        break;
    case IDFarbeHoehenlinie:
        pe_FarbeHoehenlinie = farbUebergabe;
        break;
    case IDFarbeMarkiertesObjekt:
        pe_FarbeMarkiertesObjekt = farbUebergabe;
        break;
    case IDFarbeAusgewaehltesObjekt:
        pe_FarbeAusgewaehltesObjekt = farbUebergabe;
        break;
    case IDFarbeHintergrundLayer:
        pe_FarbeHintergrundLayer = farbUebergabe;
        break;
    case IDFarbeZeichenHintergrund:
        pe_FarbeZeichenHintergrund = farbUebergabe;
        break;
    case IDFarbeAuswahlRechteck:
        pe_FarbeAuswahlRechteck = farbUebergabe;
        break;
    case IDFarbeFlaecheDarueber:
        pe_FarbeFlaecheDarueber = farbUebergabe;
        break;
    case IDFarbeFlaecheDarunter:
        pe_FarbeFlaecheDarunter = farbUebergabe;
        break;
    case IDFarbeGefaelle:
        pe_FarbeGefaelle = farbUebergabe;
        break;
    case IDFarbeFangpunkt:
        pe_FarbeFangpunkt = farbUebergabe;
        break;
    default:
        break;
    }

    aruColourEvent *myevent = new aruColourEvent();
    myevent->SetzeWert(farbUebergabe);
    myevent->SetInt(event.GetId());
    m_mama->GetEventHandler()->AddPendingEvent(*myevent);
    return;
}

void Programm_Einstellungen_Dialog::IniSpeichern(void)
{
    ofstream Neue_Ini_Datei("Programmeinstellungen.ini", ios::out|ios_base::trunc);
    if(Neue_Ini_Datei.good())
    {
        Neue_Ini_Datei<<"verschubWeite:"<<pe_verschubWeite<<"\n";
        Neue_Ini_Datei<<"gefaelleRasterGroesse:"<<pe_gefaelleRasterGroesse<<"\n";
        Neue_Ini_Datei<<"gefaelleMinimum:"<<pe_gefaelleMinimum<<"\n";
        Neue_Ini_Datei<<"gefaelleOptimum:"<<pe_gefaelleOptimum<<"\n";
        Neue_Ini_Datei<<"gefaelleMaximum:"<<pe_gefaelleMaximum<<"\n";
        Neue_Ini_Datei<<"flaechenRasterGroesse:"<<pe_flaechenRasterGroesse<<"\n";
        Neue_Ini_Datei<<"pxSuchEntfernung:"<<pe_pxSuchEntfernung<<"\n";
        Neue_Ini_Datei<<"lnWandelGenauigkeit:"<<pe_lnWandelGenauigkeit<<"\n";
        Neue_Ini_Datei<<"anzeigeGenauigkeit:"<<pe_anzeigeGenauigkeit<<"\n";
        Neue_Ini_Datei<<"pseudoSchattenFkt:"<<pe_pseudoSchattenFkt<<"\n";
        Neue_Ini_Datei<<"FarbePktLn:"<<(int)pe_FarbePktLn.Red()<<","<<(int)pe_FarbePktLn.Green()<<","<<(int)pe_FarbePktLn.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeStrich:"<<(int)pe_FarbeStrich.Red()<<","<<(int)pe_FarbeStrich.Green()<<","<<(int)pe_FarbeStrich.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeHoehenMarke:"<<(int)pe_FarbeHoehenMarke.Red()<<","<<(int)pe_FarbeHoehenMarke.Green()<<","<<(int)pe_FarbeHoehenMarke.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeHoehenlinie:"<<(int)pe_FarbeHoehenlinie.Red()<<","<<(int)pe_FarbeHoehenlinie.Green()<<","<<(int)pe_FarbeHoehenlinie.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeMarkiertesObjekt:"<<(int)pe_FarbeMarkiertesObjekt.Red()<<","<<(int)pe_FarbeMarkiertesObjekt.Green()<<","<<(int)pe_FarbeMarkiertesObjekt.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeAusgewaehltesObjekt:"<<(int)pe_FarbeAusgewaehltesObjekt.Red()<<","<<(int)pe_FarbeAusgewaehltesObjekt.Green()<<","<<(int)pe_FarbeAusgewaehltesObjekt.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeHintergrundLayer:"<<(int)pe_FarbeHintergrundLayer.Red()<<","<<(int)pe_FarbeHintergrundLayer.Green()<<","<<(int)pe_FarbeHintergrundLayer.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeZeichenHintergrund:"<<(int)pe_FarbeZeichenHintergrund.Red()<<","<<(int)pe_FarbeZeichenHintergrund.Green()<<","<<(int)pe_FarbeZeichenHintergrund.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeAuswahlRechteck:"<<(int)pe_FarbeAuswahlRechteck.Red()<<","<<(int)pe_FarbeAuswahlRechteck.Green()<<","<<(int)pe_FarbeAuswahlRechteck.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeFlaecheDarueber:"<<(int)pe_FarbeFlaecheDarueber.Red()<<","<<(int)pe_FarbeFlaecheDarueber.Green()<<","<<(int)pe_FarbeFlaecheDarueber.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeFlaecheDarunter:"<<(int)pe_FarbeFlaecheDarunter.Red()<<","<<(int)pe_FarbeFlaecheDarunter.Green()<<","<<(int)pe_FarbeFlaecheDarunter.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeGefaelle:"<<(int)pe_FarbeGefaelle.Red()<<","<<(int)pe_FarbeGefaelle.Green()<<","<<(int)pe_FarbeGefaelle.Blue()<<"\n";
        Neue_Ini_Datei<<"FarbeFangpunkt:"<<(int)pe_FarbeFangpunkt.Red()<<","<<(int)pe_FarbeFangpunkt.Green()<<","<<(int)pe_FarbeFangpunkt.Blue()<<"\n";
        Neue_Ini_Datei.close();
    }
    return;
}

double Programm_Einstellungen_Dialog::HoleWert(programmEinstellungID retID) const
{
    switch (retID)
    {
    case IDverschubWeite:
        return pe_verschubWeite;
    case IDgefaelleRasterGroesse:
        return pe_gefaelleRasterGroesse;
    case IDflaechenRasterGroesse:
        return pe_flaechenRasterGroesse;
    case IDgefaelleMinimum:
        return pe_gefaelleMinimum;
    case IDgefaelleOptimum:
        return pe_gefaelleOptimum;
    case IDgefaelleMaximum:
        return pe_gefaelleMaximum;
    case IDpxSuchEntfernung:
        return pe_pxSuchEntfernung;
    case IDlnWandelGenauigkeit:
        return pe_lnWandelGenauigkeit;
    case IDanzeigeGenauigkeit:
        return pe_anzeigeGenauigkeit;
    case IDpseudoSchattenFkt:
        return pe_pseudoSchattenFkt;
    case IDFarbePktLn:
    case IDFarbeHoehenMarke:
    case IDFarbeHoehenlinie:
    case IDFarbeMarkiertesObjekt:
    case IDFarbeAusgewaehltesObjekt:
    case IDFarbeHintergrundLayer:
    case IDFarbeZeichenHintergrund:
    case IDFarbeAuswahlRechteck:
    case IDFarbeStrich:
    case IDFarbeFlaecheDarueber:
    case IDFarbeFlaecheDarunter:
    case IDFarbeGefaelle:
    case IDFarbeFangpunkt:
        break;
    }
    return 0.0;
}

wxColour Programm_Einstellungen_Dialog::HoleFarbe(programmEinstellungID retID) const
{
    switch (retID)
    {
    case IDverschubWeite:
    case IDgefaelleRasterGroesse:
    case IDgefaelleMinimum:
    case IDgefaelleOptimum:
    case IDgefaelleMaximum:
    case IDpxSuchEntfernung:
    case IDlnWandelGenauigkeit:
    case IDanzeigeGenauigkeit:
    case IDpseudoSchattenFkt:
        return wxColour(0, 0, 0);
    case IDFarbePktLn:
        return pe_FarbePktLn;
    case IDFarbeStrich:
        return pe_FarbeStrich;
    case IDFarbeHoehenMarke:
        return pe_FarbeHoehenMarke;
    case IDFarbeHoehenlinie:
        return pe_FarbeHoehenlinie;
    case IDFarbeMarkiertesObjekt:
        return pe_FarbeMarkiertesObjekt;
    case IDFarbeAusgewaehltesObjekt:
        return pe_FarbeAusgewaehltesObjekt;
    case IDFarbeHintergrundLayer:
        return pe_FarbeHintergrundLayer;
    case IDFarbeZeichenHintergrund:
        return pe_FarbeZeichenHintergrund;
    case IDFarbeAuswahlRechteck:
        return pe_FarbeAuswahlRechteck;
    case IDFarbeFlaecheDarueber:
        return pe_FarbeFlaecheDarueber;
    case IDFarbeFlaecheDarunter:
        return pe_FarbeFlaecheDarunter;
    case IDFarbeGefaelle:
        return pe_FarbeGefaelle;
    case IDFarbeFangpunkt:
        return pe_FarbeFangpunkt;
    default:
        break;
    }
    return wxColour(0, 0, 0);
}
/*ENDE Programm_Einstellungen_Dialog*/

/*Objekt_Anzeige_Auswahl_Dialog*/
BEGIN_EVENT_TABLE(Objekt_Anzeige_Auswahl_Dialog, wxDialog)
    EVT_CHECKBOX(wxID_ANY, Objekt_Anzeige_Auswahl_Dialog::OnChkBx)
    EVT_CLOSE(Objekt_Anzeige_Auswahl_Dialog::OnClose)
    EVT_BUTTON(wxID_OK, Objekt_Anzeige_Auswahl_Dialog::OnOKButton)
END_EVENT_TABLE()

Objekt_Anzeige_Auswahl_Dialog::Objekt_Anzeige_Auswahl_Dialog(wxWindow* parent):wxDialog(parent, wxID_ANY, wxT("Anzeige und Wählbarkeit"))
{
    m_mama = parent;

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *auswahlSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *objekteSizer = new wxBoxSizer(wxVERTICAL);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Objekte"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Punkte"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Höhe"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Linien"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Flächen"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Höhenmarken"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Striche"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Bögen"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Kreise"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    objekteSizer->Add(new wxTextCtrl(this, -1, wxT("Fangpunkte"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);

    wxBoxSizer *anzeigeSizer = new wxBoxSizer(wxVERTICAL);
    anzeigeSizer->Add(new wxTextCtrl(this, -1, wxT("anzeigen"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    PunktZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigePunkt, wxT(""));
        PunktZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(PunktZeigeChkBx, 0, wxALL, 5);
    HoeheZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeHoehe, wxT(""));
        HoeheZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(HoeheZeigeChkBx, 0, wxALL, 5);
    LinieZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeLinie, wxT(""));
        LinieZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(LinieZeigeChkBx, 0, wxALL, 5);
    FlaecheZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeFlaeche, wxT(""));
        FlaecheZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(FlaecheZeigeChkBx, 0, wxALL, 5);
    HMZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeHoehenmarke, wxT(""));
        HMZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(HMZeigeChkBx, 0, wxALL, 5);
    StrichZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeStrich, wxT(""));
        StrichZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(StrichZeigeChkBx, 0, wxALL, 5);
    BogenZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeBogen, wxT(""));
        BogenZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(BogenZeigeChkBx, 0, wxALL, 5);
    KreisZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeKreis, wxT(""));
        KreisZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(KreisZeigeChkBx, 0, wxALL, 5);
    FangpunktZeigeChkBx = new wxCheckBox(this, RUZmBIFrame::idZeigeFangpunkt, wxT(""));
        FangpunktZeigeChkBx->SetValue(true);
        anzeigeSizer->Add(FangpunktZeigeChkBx, 0, wxALL, 5);

    wxBoxSizer *wahlSizer = new wxBoxSizer(wxVERTICAL);
    wahlSizer->Add(new wxTextCtrl(this, -1, wxT("wählbar"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    PunktWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehlePunkt, wxT(""));
        PunktWaehleChkBx->SetValue(true);
        wahlSizer->Add(PunktWaehleChkBx, 0, wxALL, 5);
    HoeheWaehleChkBx = new wxCheckBox(this, wxID_ANY, wxT(""));
        HoeheWaehleChkBx->Enable(false);
        wahlSizer->Add(HoeheWaehleChkBx, 0, wxALL, 5);
    LinieWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleLinie, wxT(""));
        LinieWaehleChkBx->SetValue(true);
        wahlSizer->Add(LinieWaehleChkBx, 0, wxALL, 5);
    FlaecheWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleFlaeche, wxT(""));
        FlaecheWaehleChkBx->SetValue(true);
        wahlSizer->Add(FlaecheWaehleChkBx, 0, wxALL, 5);
    HMWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleHoehenmarke, wxT(""));
        HMWaehleChkBx->SetValue(true);
        wahlSizer->Add(HMWaehleChkBx, 0, wxALL, 5);
    StrichWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleStrich, wxT(""));
        StrichWaehleChkBx->SetValue(true);
        wahlSizer->Add(StrichWaehleChkBx, 0, wxALL, 5);
    BogenWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleBogen, wxT(""));
        BogenWaehleChkBx->SetValue(true);
        wahlSizer->Add(BogenWaehleChkBx, 0, wxALL, 5);
    KreisWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleKreis, wxT(""));
        KreisWaehleChkBx->SetValue(true);
        wahlSizer->Add(KreisWaehleChkBx, 0, wxALL, 5);
    FangpunktWaehleChkBx = new wxCheckBox(this, RUZmBIFrame::idWaehleFangpunkt, wxT(""));
        FangpunktWaehleChkBx->SetValue(true);
        wahlSizer->Add(FangpunktWaehleChkBx, 0, wxALL, 5);

    auswahlSizer->Add(objekteSizer, 0, 0, 5);
    auswahlSizer->Add(anzeigeSizer, 0, 0, 5);
    auswahlSizer->Add(wahlSizer, 0, 0, 5);
    topsizer->Add(auswahlSizer, 0, wxEXPAND, 5);
    topsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

Objekt_Anzeige_Auswahl_Dialog::~Objekt_Anzeige_Auswahl_Dialog()
{
    delete PunktZeigeChkBx;
    delete PunktWaehleChkBx;
    delete HoeheZeigeChkBx;
    delete HoeheWaehleChkBx;
    delete LinieZeigeChkBx;
    delete LinieWaehleChkBx;
    delete FlaecheZeigeChkBx;
    delete FlaecheWaehleChkBx;
    delete HMZeigeChkBx;
    delete HMWaehleChkBx;
    delete StrichZeigeChkBx;
    delete StrichWaehleChkBx;
    delete BogenZeigeChkBx;
    delete BogenWaehleChkBx;
    delete KreisZeigeChkBx;
    delete KreisWaehleChkBx;
    delete FangpunktZeigeChkBx;
    delete FangpunktWaehleChkBx;
}

void Objekt_Anzeige_Auswahl_Dialog::OnChkBx(wxCommandEvent &event)
{
    wxCommandEvent *myevent = new wxCommandEvent(wxEVT_COMMAND_CHECKBOX_CLICKED, event.GetId());
    m_mama->GetEventHandler()->AddPendingEvent(*myevent);
    event.Skip();
    return;
}

void Objekt_Anzeige_Auswahl_Dialog::OnClose(wxCloseEvent &event)
{
    event.Skip();
    return;
}

void Objekt_Anzeige_Auswahl_Dialog::OnOKButton(wxCommandEvent &event)
{
    event.Skip();
    return;
}
/*ENDE Objekt_Anzeige_Auswahl_Dialog*/

/*DXFParameterDialog*/
BEGIN_EVENT_TABLE(DXF_Parameter_Dialog, wxDialog)
    EVT_CLOSE(DXF_Parameter_Dialog::OnClose)
    EVT_TEXT(wxID_ANY, DXF_Parameter_Dialog::OnTextChange)
    EVT_BUTTON(wxID_OK, DXF_Parameter_Dialog::OnOKButton)
END_EVENT_TABLE()

DXF_Parameter_Dialog::DXF_Parameter_Dialog(wxWindow* parent):wxDialog(parent, wxID_ANY, wxT("Parameter für DXF Import"))
{
    m_mama = parent;
    DxfDialogIni();

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *blockStartSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *xKoorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *yKoorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *zKoorSizer = new wxBoxSizer(wxHORIZONTAL);

    blockStartSizer->Add(new wxTextCtrl(this, -1, wxT("Block-Start"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    blockStartSizer->Add(new wxTextCtrl(this, IDpktSW, m_pktSW, wxDefaultPosition, wxDefaultSize ),0, wxALL, 5);
    xKoorSizer->Add(new wxTextCtrl(this, -1, wxT("X-Koordinate"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    xKoorSizer->Add(new wxTextCtrl(this, IDpktXKenn, m_pktXKenn, wxDefaultPosition, wxDefaultSize ),0, wxALL, 5);
    yKoorSizer->Add(new wxTextCtrl(this, -1, wxT("Y-Koordinate"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    yKoorSizer->Add(new wxTextCtrl(this, IDpktYKenn, m_pktYKenn, wxDefaultPosition, wxDefaultSize ),0, wxALL, 5);
    zKoorSizer->Add(new wxTextCtrl(this, -1, wxT("Z-Koordinate"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    zKoorSizer->Add(new wxTextCtrl(this, IDpktZKenn, m_pktZKenn, wxDefaultPosition, wxDefaultSize ),0, wxALL, 5);

    topsizer->Add(new wxTextCtrl(this, -1, wxT("Punkt - Kennungen"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    topsizer->Add(blockStartSizer, 1, wxEXPAND, 5);
    topsizer->Add(xKoorSizer, 1, wxEXPAND, 5);
    topsizer->Add(yKoorSizer, 1, wxEXPAND, 5);
    topsizer->Add(zKoorSizer, 1, wxEXPAND, 5);
    topsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

DXF_Parameter_Dialog::~DXF_Parameter_Dialog()
{
}

void DXF_Parameter_Dialog::OnClose(wxCloseEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void DXF_Parameter_Dialog::DxfDialogIni(void)
{
    /*Standardwerte setzen*/
    m_pktXKenn = wxT(" 10");
    m_pktYKenn = wxT(" 20");
    m_pktZKenn = wxT(" 30");
    m_pktSW = wxT("TEXT");

    char Zeile[50];
	ifstream DXF_Parameter_Ini_Datei("DXF_Parameter.ini", ios::in);
	if(DXF_Parameter_Ini_Datei.good())
	{
		//cout<<"Neue_Ini_Datei existiert"<<endl;
		while(!DXF_Parameter_Ini_Datei.eof())
		{
			DXF_Parameter_Ini_Datei.getline(Zeile,50,':');
			if(!strcmp(Zeile,"Punkt Kennung X"))
			{
				DXF_Parameter_Ini_Datei.getline(Zeile,50);
				m_pktXKenn=wxString::FromUTF8(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"Punkt Kennung Y"))
			{
				DXF_Parameter_Ini_Datei.getline(Zeile,50);
				m_pktYKenn=wxString::FromUTF8(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"Punkt Kennung Z"))
			{
				DXF_Parameter_Ini_Datei.getline(Zeile,50);
				m_pktZKenn=wxString::FromUTF8(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"Punkt Kennung SW"))
			{
				DXF_Parameter_Ini_Datei.getline(Zeile,50);
				m_pktSW=wxString::FromUTF8(Zeile);
				continue;
			}
		}
		DXF_Parameter_Ini_Datei.close();
	}
	else{
        ofstream Neue_Ini_Datei("DXF_Parameter.ini", ios::out|ios_base::trunc);
        if(Neue_Ini_Datei.good())
        {
            Neue_Ini_Datei<<"Punkt Kennung SW:"<<m_pktSW.mb_str()<<"\n";
            Neue_Ini_Datei<<"Punkt Kennung X:"<<m_pktXKenn.mb_str()<<"\n";
            Neue_Ini_Datei<<"Punkt Kennung Y:"<<m_pktYKenn.mb_str()<<"\n";
            Neue_Ini_Datei<<"Punkt Kennung Z:"<<m_pktZKenn.mb_str()<<"\n";
            Neue_Ini_Datei.close();
        }
	}
    return;
}

void DXF_Parameter_Dialog::OnOKButton(wxCommandEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void DXF_Parameter_Dialog::OnTextChange(wxCommandEvent &event)
{
    wxString inhalt = event.GetString();
    int textFeldID = event.GetId();
    switch(textFeldID){
    case IDpktXKenn:
        m_pktXKenn = inhalt;
        break;
    case IDpktYKenn:
        m_pktYKenn = inhalt;
        break;
    case IDpktZKenn:
        m_pktZKenn = inhalt;
        break;
    case IDpktSW:
        m_pktSW = inhalt;
        break;
    }
    return;
}

void DXF_Parameter_Dialog::IniSpeichern(void)
{
    ofstream Neue_Ini_Datei("DXF_Parameter.ini", ios::out|ios_base::trunc);
    if(Neue_Ini_Datei.good())
    {
        Neue_Ini_Datei<<"Punkt Kennung X:"<<m_pktXKenn.mb_str()<<"\n";
        Neue_Ini_Datei<<"Punkt Kennung Y:"<<m_pktYKenn.mb_str()<<"\n";
        Neue_Ini_Datei<<"Punkt Kennung Z:"<<m_pktZKenn.mb_str()<<"\n";
        Neue_Ini_Datei<<"Punkt Kennung SW:"<<m_pktSW.mb_str()<<"\n";
        Neue_Ini_Datei.close();
    }
    return;
}

void DXF_Parameter_Dialog::HoleKennung(char* retVar, dxfParameterID retID) const
{
    switch (retID)
    {
    case IDpktXKenn:
        strcpy(retVar, static_cast<const char*>(m_pktXKenn.c_str()));
        return;
    case IDpktYKenn:
        strcpy(retVar, static_cast<const char*>(m_pktYKenn.c_str()));
        return;
    case IDpktZKenn:
        strcpy(retVar, static_cast<const char*>(m_pktZKenn.c_str()));
        return;
    case IDpktSW:
        strcpy(retVar, static_cast<const char*>(m_pktSW.c_str()));
        return;
    }
    return;
}
/*ENDE DXFParameterDialog*/

/*DXF_Import_Auswahl_Dialog*/
BEGIN_EVENT_TABLE(DXF_Import_Auswahl_Dialog, wxDialog)
    EVT_CLOSE(DXF_Import_Auswahl_Dialog::OnClose)
    EVT_BUTTON(1000, DXF_Import_Auswahl_Dialog::OnMarkierungDrehen)
    EVT_BUTTON(wxID_OK, DXF_Import_Auswahl_Dialog::OnOKButton)
END_EVENT_TABLE()

DXF_Import_Auswahl_Dialog::DXF_Import_Auswahl_Dialog(wxWindow* parent, Liste<Char_Speicher>* layerLst):wxDialog(parent, wxID_ANY, wxT("Layer für Import auswählen"))
{
    m_mama = parent;
    m_layerLst = layerLst;

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    int listenGroesse = layerLst->GetListenGroesse();
    wxString layerArray[listenGroesse];
    if(!listenGroesse)wxMessageDialog(this, wxT("Liste Leer")).ShowModal();
    int i = 0;
    for(Listenelement<Char_Speicher> *char_LE_Laeufer = layerLst->GetErstesListenelement(); char_LE_Laeufer != NULL; char_LE_Laeufer = char_LE_Laeufer->GetNachfolger())
    {
        layerArray[i] = wxString::FromUTF8(char_LE_Laeufer->GetElement()->HoleInhalt());
        i++;
    }
    //layerLst->ListeLoeschen("");

    layerListBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, listenGroesse, layerArray,
                                          wxLB_MULTIPLE|wxLB_ALWAYS_SB, wxDefaultValidator, wxListBoxNameStr);
    layerListBox->SetMinSize(wxSize(200,-1));
    layerListBox->SetMaxSize(wxSize(-1,200));

    topsizer->Add(layerListBox, 0, wxEXPAND|wxALL, 5);
    topsizer->Add(new wxButton(this, 1000, wxT("Auswahl umkehren")), 0, wxSHAPED|wxRIGHT, 5);
    topsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);

    MarkierungDrehen();
}

DXF_Import_Auswahl_Dialog::~DXF_Import_Auswahl_Dialog()
{
}

void DXF_Import_Auswahl_Dialog::OnClose(wxCloseEvent &event)
{
    return;
}

void DXF_Import_Auswahl_Dialog::OnOKButton(wxCommandEvent &event)
{
    m_layerLst->ListeLoeschen("");
    unsigned int listenGroesse = layerListBox->GetCount();
    for(unsigned int i = 0; i < listenGroesse; i++)
    {
        if(layerListBox->IsSelected(i))
        {
            Char_Speicher *tempChar = new Char_Speicher(layerListBox->GetString(i).mb_str());
            //wxMessageDialog(this, wxString(tempChar->HoleInhalt())).ShowModal();
            m_layerLst->Hinzufuegen(tempChar);
        }
    }
    event.Skip();
    return;
}

void DXF_Import_Auswahl_Dialog::MarkierungDrehen(void)
{
    unsigned int listenGroesse = layerListBox->GetCount();
    for(unsigned int i = 0; i < listenGroesse; i++)
    {
        if(layerListBox->IsSelected(i))
        {
            layerListBox->Deselect(i);
        }else{
            layerListBox->Select(i);
        }
    }
    return;
}

void DXF_Import_Auswahl_Dialog::OnMarkierungDrehen(wxCommandEvent &event)
{
    MarkierungDrehen();
    return;
}
/*ENDE DXF_Import_Auswahl_Dialog*/

/*Layer_Verwaltungs_Dialog*/
BEGIN_EVENT_TABLE(Layer_Verwaltungs_Dialog, wxDialog)
    EVT_BUTTON(idMenuNeuerLayer, Layer_Verwaltungs_Dialog::OnNeuerLayerButton)
    EVT_SIZE(Layer_Verwaltungs_Dialog::OnSize)
END_EVENT_TABLE()

Layer_Verwaltungs_Dialog::Layer_Verwaltungs_Dialog(RUZmBIFrame* parent):wxDialog(parent, wxID_ANY, wxT("Layerauswahl"), wxDefaultPosition, wxSize(450, 350))
{
    layerLst = new Liste<RUZ_Layer>();
    layerSizerLst = new Liste<aruLayerSizer>();
    m_mama = parent;

    hauptSizer = new wxBoxSizer(wxVERTICAL);
    layerSizer = new wxBoxSizer(wxVERTICAL);

    scroller = new wxScrolledWindow(this);
    scroller->SetSizer(layerSizer);
    //scroller->FitInside();
    scroller->SetScrollRate(0, 5);

    wxBoxSizer *knopfSizer = new wxBoxSizer(wxHORIZONTAL);
    knopfSizer->Add(new wxButton(this, idMenuNeuerLayer, wxT("Neuer Layer")), 0, wxSHAPED|wxRIGHT, 5);
    knopfSizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);

    hauptSizer->Add(new wxTextCtrl(this, -1, wxT("Layer"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE), 0, wxRIGHT, 5);
    hauptSizer->Add(scroller, 1, wxEXPAND, 5);
    hauptSizer->Add(knopfSizer, 0, wxEXPAND, 5);

    this->SetSizer(hauptSizer);
    //hauptSizer->Fit(this);

    SetEscapeId(wxID_CANCEL);
}

Layer_Verwaltungs_Dialog::~Layer_Verwaltungs_Dialog()
{
    LayerAuswahlLeeren();
    layerLst->ListeLeeren("~Layer_Verwaltungs_Dialog()");
    delete layerLst;
    layerSizerLst->ListeLeeren("~Layer_Verwaltungs_Dialog()");
    delete layerSizerLst;
    delete scroller;
}

void Layer_Verwaltungs_Dialog::LayerHinzufuegen(wxString name, RUZ_Layer* neuerLayer)
{
    if(layerLst->ExklusivHinzufuegen(neuerLayer))
    {
        aruLayerSizer *neuerLayerSizer = new aruLayerSizer(m_mama, neuerLayer);
        neuerLayer->SetzeSichtbarkeit(true);

        aruLayerTextCtrl *neuerLayerName = new aruLayerTextCtrl(neuerLayer, scroller, wxID_ANY, name, wxDefaultPosition, wxSize(250, 15));
            neuerLayerSizer->Add(neuerLayerName, 1, wxEXPAND|wxALL, 5);
            neuerLayerSizer->SetLayerNameCtrl(neuerLayerName);
            layerSizerLst->Hinzufuegen(neuerLayerSizer);

        aruLayerButton *SichtKnopf = new aruLayerButton(neuerLayerSizer, scroller, wxID_ANY, wxT("Sichtbarkeit"));
            SichtKnopf->SetBitmap(wxBitmap(wxICON(buttonVisibleOn_normal_ico)));
            SichtKnopf->SetBitmapCurrent(wxBitmap(wxICON(buttonVisibleOn_current_ico)));
            SichtKnopf->SetBitmapFocus(wxBitmap(wxICON(buttonVisibleOn_current_ico)));
            SichtKnopf->SetBitmapPressed(wxBitmap(wxICON(buttonVisibleOn_pressed_ico)));
            Bind(wxEVT_BUTTON, &Layer_Verwaltungs_Dialog::OnLayerSichtbarkeitWechseln, this, SichtKnopf->GetId());
        /*Bei Eventhandling anmelden!*/

        aruLayerButton *BearbeitKnopf = new aruLayerButton(neuerLayerSizer, scroller, wxID_ANY, wxT("Bearbeiten"));
            BearbeitKnopf->SetBitmap(wxBitmap(wxICON(buttonEdit_normal_ico)));
            BearbeitKnopf->SetBitmapCurrent(wxBitmap(wxICON(buttonEdit_current_ico)));
            BearbeitKnopf->SetBitmapFocus(wxBitmap(wxICON(buttonEdit_current_ico)));
            BearbeitKnopf->SetBitmapPressed(wxBitmap(wxICON(buttonEdit_pressed_ico)));
            Bind(wxEVT_BUTTON, &Layer_Verwaltungs_Dialog::OnLayerAuswaehlen, this, BearbeitKnopf->GetId());
        /*Bei Eventhandling anmelden!*/

        aruLayerButton *LoeschKnopf = new aruLayerButton(neuerLayerSizer, scroller, wxID_ANY, wxT("Löschen"));
            LoeschKnopf->SetBitmap(wxBitmap(wxICON(buttonDelete_normal_ico)));
            LoeschKnopf->SetBitmapCurrent(wxBitmap(wxICON(buttonDelete_current_ico)));
            LoeschKnopf->SetBitmapFocus(wxBitmap(wxICON(buttonDelete_current_ico)));
            LoeschKnopf->SetBitmapPressed(wxBitmap(wxICON(buttonDelete_pressed_ico)));
            Bind(wxEVT_BUTTON, &Layer_Verwaltungs_Dialog::OnLayerLoeschen, this, LoeschKnopf->GetId());
        /*Bei Eventhandling anmelden!*/

        neuerLayerSizer->Add(SichtKnopf, 0, wxSHAPED|wxALL, 5);
        neuerLayerSizer->Add(BearbeitKnopf, 0, wxSHAPED|wxALL, 5);
        neuerLayerSizer->Add(LoeschKnopf, 0, wxSHAPED|wxALL, 5);

        layerSizer->Add(neuerLayerSizer, 0, wxEXPAND, 0);
        layerSizer->Layout();
        hauptSizer->Layout();
        //hauptSizer->Fit(this);

        LayerAuswahlFinden();
    }
    Refresh();
    return;
}

void Layer_Verwaltungs_Dialog::OnLayerAuswaehlen(wxCommandEvent& event)
{
    m_mama->SetzeAktuellenLayer(1, static_cast<aruLayerButton*>(event.GetEventObject())->GetClientData()->GetClientData());
    m_mama->AuswahlLeeren();
    LayerAuswahlFinden();
    Refresh();
    return;
}

void Layer_Verwaltungs_Dialog::LayerAuswahlFinden(void)
{
    for(aruLayerSizer* tempLayerSizer = layerSizerLst->GetErstesElement(); tempLayerSizer != NULL; tempLayerSizer = layerSizerLst->GetNaechstesElement())
    {
        if(tempLayerSizer->GetClientData() == NULL) return;
        if(tempLayerSizer->GetClientData() == m_mama->HoleAktuellenLayer())
        {
            tempLayerSizer->GetLayerNameCtrl()->SetBackgroundColour(wxColour(135, 206, 250));
        }else{
            tempLayerSizer->GetLayerNameCtrl()->SetBackgroundColour(wxColour(255, 255, 255));
        }
    }
    return;
}

void Layer_Verwaltungs_Dialog::OnLayerSichtbarkeitWechseln(wxCommandEvent& event)
{
    RUZ_Layer *tempLayer = static_cast<aruLayerButton*>(event.GetEventObject())->GetClientData()->GetClientData();
    if(tempLayer->IstSichtbar())
    {
        tempLayer->SetzeSichtbarkeit(false);
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmap(wxBitmap(wxICON(buttonVisibleOff_normal_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapCurrent(wxBitmap(wxICON(buttonVisibleOff_current_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapFocus(wxBitmap(wxICON(buttonVisibleOff_current_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapPressed(wxBitmap(wxICON(buttonVisibleOff_pressed_ico)));
    }else{
        tempLayer->SetzeSichtbarkeit(true);
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmap(wxBitmap(wxICON(buttonVisibleOn_normal_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapCurrent(wxBitmap(wxICON(buttonVisibleOn_current_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapFocus(wxBitmap(wxICON(buttonVisibleOn_current_ico)));
        static_cast<aruLayerButton*>(event.GetEventObject())->SetBitmapPressed(wxBitmap(wxICON(buttonVisibleOn_pressed_ico)));
    }
    return;
}

void Layer_Verwaltungs_Dialog::OnLayerLoeschen(wxCommandEvent& event)
{
    aruLayerSizer *tempLayerSizer = static_cast<aruLayerButton*>(event.GetEventObject())->GetClientData();

    m_mama->LayerEntfernen(static_cast<aruLayerButton*>(event.GetEventObject())->GetClientData()->GetClientData());
    layerLst->Entfernen(static_cast<aruLayerButton*>(event.GetEventObject())->GetClientData()->GetClientData());
    layerSizerLst->Entfernen(tempLayerSizer);
    //LayerSizer samt Inhalt (RUZ_Layer) loeschen
    tempLayerSizer->Clear(true);
    layerSizer->Remove(tempLayerSizer);
    layerSizer->Layout();
    hauptSizer->Layout();
    //hauptSizer->Fit(this);
    //m_mama->SetzeAktuellenLayer(0, NULL);
    LayerAuswahlFinden();
    return;
}

void Layer_Verwaltungs_Dialog::LayerAuswahlLeeren(void)
{
    layerLst->ListeLeeren("");
    layerSizerLst->ListeLeeren("");
    layerSizer->Clear(true);
    layerSizer->Layout();
    hauptSizer->Layout();
    //hauptSizer->Fit(this);
    return;
}

void Layer_Verwaltungs_Dialog::OnNeuerLayerButton(wxCommandEvent& event)
{
    RUZ_Layer *neuerLayer = new RUZ_Layer((char*)("neuer Layer"));
    if(neuerLayer != NULL)
    {
        m_mama->LayerHinzufuegen(neuerLayer);
        m_mama->SetzeAktuellenLayer(0, neuerLayer);
        LayerHinzufuegen(neuerLayer->HoleName(), neuerLayer);
    }
    LayerAuswahlFinden();
    return;
}

void Layer_Verwaltungs_Dialog::OnSize(wxSizeEvent& event)
{
    if(this->GetSize().GetHeight() > 400)this->SetSize(wxSize(this->GetSize().GetWidth(), 400));
    event.Skip();
    return;
}
/*ENDE Layer_Verwaltungs_Dialog*/

/*Layer_Auswahl_Dialog*/
BEGIN_EVENT_TABLE(Layer_Auswahl_Dialog, wxDialog)
    EVT_BUTTON(wxID_OK, Layer_Auswahl_Dialog::OnOKButton)
    EVT_BUTTON(wxID_CANCEL, Layer_Auswahl_Dialog::OnCancelButton)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, Layer_Auswahl_Dialog::OnDblClick)
    EVT_CLOSE(Layer_Auswahl_Dialog::OnClose)
END_EVENT_TABLE()

Layer_Auswahl_Dialog::Layer_Auswahl_Dialog(wxWindow* parent, Liste<RUZ_Layer> *layerLst, wxString caption, bool singleSelection):wxDialog(parent, wxID_ANY, wxT("Layerauswahl"))
{
    m_mama = parent;
    m_layerLst = new Liste<RUZ_Layer>;
    if(m_layerLst)/*deep copy*/
    {
        for(RUZ_Layer* aktLayer = layerLst->GetErstesElement(); aktLayer; aktLayer = layerLst->GetNaechstesElement())
        {
            m_layerLst->Hinzufuegen(aktLayer);
        }
    }
    m_singleSelection = singleSelection;
    sel_layer = NULL;
    if(m_singleSelection)
    {
        layerListCtrl = new wxListCtrl(this, 1000, wxDefaultPosition, wxSize(250, -1), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_HEADER);
    }else{
        layerListCtrl = new wxListCtrl(this, 1000, wxDefaultPosition, wxSize(250, -1), wxLC_REPORT|wxLC_NO_HEADER);
    }
    wxListItem col0;
    col0.SetId(0);
    col0.SetText( wxT(" ") );
    col0.SetWidth(250);
    layerListCtrl->InsertColumn(0, col0);

    long itemID = 0;
    if(m_layerLst)
    {
        for(RUZ_Layer* laeufer_Layer = m_layerLst->GetErstesElement(); laeufer_Layer != NULL; laeufer_Layer = m_layerLst->GetNaechstesElement())
        {
            wxListItem neuesListItem;
            neuesListItem.SetText(wxString::FromUTF8(laeufer_Layer->HoleName()));
            neuesListItem.SetId(itemID);
            m_layerLst->Wert(laeufer_Layer, itemID);
            layerLst->Wert(laeufer_Layer, itemID);
            layerListCtrl->InsertItem(neuesListItem);
            layerListCtrl->SetItem(itemID, 0, neuesListItem.GetText());
            itemID++;
        }
    }

    wxBoxSizer *hauptSizer = new wxBoxSizer(wxVERTICAL);

    hauptSizer->Add(new wxTextCtrl(this, -1, caption, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE), 0, wxEXPAND);
    hauptSizer->Add(layerListCtrl, 1);
    hauptSizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);
    hauptSizer->Add(new wxButton(this, wxID_CANCEL, wxT("Abbrechen")), 0, wxSHAPED|wxRIGHT, 5);

    this->SetSizer(hauptSizer);
    hauptSizer->Fit(this);
}

Layer_Auswahl_Dialog::~Layer_Auswahl_Dialog()
{
    m_layerLst->ListeLeeren("");
    delete m_layerLst;
}

void Layer_Auswahl_Dialog::OnClose(wxCloseEvent &event)
{
    layerListCtrl->ClearAll();
    delete layerListCtrl;
    Destroy();
    return;
}

void Layer_Auswahl_Dialog::OnOKButton(wxCommandEvent &event)
{
    if(m_singleSelection)
    {
        long itemNr = -1;
        itemNr = layerListCtrl->GetNextItem(itemNr, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (itemNr != -1)/*Wenn ein Item ausgewählt war*/
        {
            if(m_layerLst)
            {
                for(Listenelement<RUZ_Layer>* layer_LE_sucher = m_layerLst->GetErstesListenelement();
                    layer_LE_sucher; layer_LE_sucher = layer_LE_sucher->GetNachfolger())
                {
                    if(layer_LE_sucher->Wert() == itemNr)
                    {
                        if(this->IsModal())
                        {
                            this->EndModal((int)itemNr);
                        }
                        break;
                    }
                }
            }
        }
    }else{
        aruLayerListeEvent* lyrLstEvent = new aruLayerListeEvent();
        long itemNr = -1;
        do
        {
            itemNr = layerListCtrl->GetNextItem(itemNr, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            if(m_layerLst)
            {
                for(Listenelement<RUZ_Layer>* layer_LE_sucher = m_layerLst->GetErstesListenelement();
                    layer_LE_sucher; layer_LE_sucher = layer_LE_sucher->GetNachfolger())
                {
                    if(layer_LE_sucher->Wert() == itemNr)
                    {
                        lyrLstEvent->Hinzufuegen(layer_LE_sucher->GetElement());
                    }
                }
            }
        }while(itemNr != -1);
        if(this->IsModal())this->EndModal(-1);
        m_mama->GetEventHandler()->QueueEvent(lyrLstEvent);
    }
    return;
}

void Layer_Auswahl_Dialog::OnCancelButton(wxCommandEvent &event)
{
    this->EndModal(-1);
    return;
}

void Layer_Auswahl_Dialog::OnDblClick(wxListEvent &event)
{
    if(!m_singleSelection)return;
    long itemNr = -1;
    itemNr = layerListCtrl->GetNextItem(itemNr, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemNr != -1)/*Wenn ein Item ausgewählt war*/
    {
        if(m_layerLst)
        {
            for(Listenelement<RUZ_Layer>* layer_LE_sucher = m_layerLst->GetErstesListenelement();
                layer_LE_sucher; layer_LE_sucher = layer_LE_sucher->GetNachfolger())
            {
                if(layer_LE_sucher->Wert() == itemNr)
                {
                    if(this->IsModal())
                    {
                        this->EndModal((int)itemNr);
                    }
                    break;
                }
            }
        }
    }
    return;
}
/*ENDE Layer_Auswahl_Dialog*/

/*aruLayerButton*/
aruLayerButton::aruLayerButton(aruLayerSizer *data, wxWindow *parent, wxWindowID id, const wxString &label):wxButton(parent, id, label, wxDefaultPosition, wxSize(25, 25), wxBU_NOTEXT)
{
    cl_LayerSizer = data;
}

aruLayerButton::~aruLayerButton()
{

}

void aruLayerButton::SetClientData(aruLayerSizer *data)
{
    cl_LayerSizer = data;
    return;
}

aruLayerSizer *aruLayerButton::GetClientData(void) const
{
    return cl_LayerSizer;
}
/*ENDE aruLayerButton*/

/*aruLayerTextCtrl*/
BEGIN_EVENT_TABLE(aruLayerTextCtrl, wxTextCtrl)
	EVT_KILL_FOCUS(aruLayerTextCtrl::OnLostFocus)
END_EVENT_TABLE()

aruLayerTextCtrl::aruLayerTextCtrl(RUZ_Layer* data, wxWindow* parent, wxWindowID ID, const wxString& Value, const wxPoint& t_pos, const wxSize& t_size,
						long style, const wxValidator& validator, const wxString& name)
						:wxTextCtrl(parent, ID, Value, t_pos, t_size, style, validator, name)
{
    cl_Layer = data;
}

aruLayerTextCtrl::~aruLayerTextCtrl()
{

}

void aruLayerTextCtrl::OnLostFocus(wxFocusEvent &event)
{
    if(cl_Layer != NULL)
    {
        cl_Layer->Benennen(static_cast<const char*>(GetValue().c_str()));
    }
    event.Skip();
    return;
}
/*ENDE aruLayerTextCtrl*/

/*aruLayerSizer*/
aruLayerSizer::aruLayerSizer(RUZmBIFrame* parent, RUZ_Layer *data):wxBoxSizer(wxHORIZONTAL)
{
    m_mama = parent;
    cl_Layer = data;
    //wxMessageDialog(NULL, wxString::Format("Lege Layer %p an", cl_Layer)).ShowModal();
}
aruLayerSizer::~aruLayerSizer()
{
    //wxMessageDialog(NULL, wxString::Format("Lösche Layer %p", cl_Layer)).ShowModal();
    m_mama->LayerEntfernen(cl_Layer);
    delete cl_Layer;
}

void aruLayerSizer::SetClientData(RUZ_Layer *data)
{
    cl_Layer = data;
    return;
}

RUZ_Layer* aruLayerSizer::GetClientData(void)const
{
    return cl_Layer;
}

void aruLayerSizer::SetLayerNameCtrl(aruLayerTextCtrl* txtCtrl)
{
    m_layerNameCtrl = txtCtrl;
    return;
}

aruLayerTextCtrl* aruLayerSizer::GetLayerNameCtrl(void)const
{
    return m_layerNameCtrl;
}
/*ENDE aruLayerSizer*/

/*HLParameterDialog*/
BEGIN_EVENT_TABLE(HL_Parameter_Dialog, wxDialog)
    EVT_CLOSE(HL_Parameter_Dialog::OnClose)
    EVT_TEXT(wxID_ANY, HL_Parameter_Dialog::OnTextChange)
    EVT_BUTTON(wxID_OK, HL_Parameter_Dialog::OnOKButton)
END_EVENT_TABLE()

HL_Parameter_Dialog::HL_Parameter_Dialog(wxWindow* parent):wxDialog(parent, wxID_ANY, wxT("Parameter für die Erzeugung von Höhenlinien"))
{
    m_mama = parent;
    HlDialogIni();

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerHoehenSchritt = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizerSuchRadius = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizerStartHoehe = new wxBoxSizer(wxHORIZONTAL);

    sizerHoehenSchritt->Add(new wxTextCtrl(this, -1, wxT("Abstand HL"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    sizerSuchRadius->Add(new wxTextCtrl(this, -1, wxT("Suchradius"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    sizerStartHoehe->Add(new wxTextCtrl(this, -1, wxT("Starthöhe"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);

    aruDblTxtCtrl* hoehenSchrittCtrl = new aruDblTxtCtrl(this, IDhoehenSchritt, wxString::Format(wxT("%.2f"), hoehenSchritt), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("hoehenSchrittCtrl"));
    aruDblTxtCtrl* suchRadiusCtrl = new aruDblTxtCtrl(this, IDsuchRadius, wxString::Format(wxT("%.2f"), suchRadius), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("suchRadiusCtrl"));
    aruDblTxtCtrl* starthoeheCtrl = new aruDblTxtCtrl(this, IDstartHoehe, wxString::Format(wxT("%.2f"), startHoehe), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("starthoeheCtrl"));

    hoehenSchrittCtrl->SetNachfolger(suchRadiusCtrl);
    hoehenSchrittCtrl->SetVorgaenger(starthoeheCtrl);
    suchRadiusCtrl->SetNachfolger(starthoeheCtrl);
    suchRadiusCtrl->SetVorgaenger(hoehenSchrittCtrl);
    starthoeheCtrl->SetNachfolger(hoehenSchrittCtrl);
    starthoeheCtrl->SetVorgaenger(suchRadiusCtrl);

    sizerHoehenSchritt->Add(hoehenSchrittCtrl, 0, wxALL, 5);
    sizerSuchRadius->Add(suchRadiusCtrl, 0, wxALL, 5);
    sizerStartHoehe->Add(starthoeheCtrl, 0, wxALL, 5);

    topsizer->Add(sizerHoehenSchritt, 1, wxEXPAND, 5);
    topsizer->Add(sizerSuchRadius, 1, wxEXPAND, 5);
    topsizer->Add(sizerStartHoehe, 1, wxEXPAND, 5);
    topsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxSHAPED|wxRIGHT, 5);


    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

HL_Parameter_Dialog::~HL_Parameter_Dialog()
{
}

void HL_Parameter_Dialog::OnClose(wxCloseEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void HL_Parameter_Dialog::HlDialogIni(void)
{
    /*Standardwerte setzen*/
    hoehenSchritt = 0.01;
    suchRadius = 0.5;
    startHoehe = 0.0;

    char Zeile[50];
	ifstream HL_Parameter_Ini_Datei("HL_Parameter.ini", ios::in);
	if(HL_Parameter_Ini_Datei.good())
	{
		while(!HL_Parameter_Ini_Datei.eof())
		{
			HL_Parameter_Ini_Datei.getline(Zeile,50,':');
			if(!strcmp(Zeile,"hoehenSchritt"))
			{
				HL_Parameter_Ini_Datei.getline(Zeile,50);
				hoehenSchritt = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"suchRadius"))
			{
				HL_Parameter_Ini_Datei.getline(Zeile,50);
				suchRadius = atof(Zeile);
				continue;
			}
			if(!strcmp(Zeile,"starthoehe"))
			{
				HL_Parameter_Ini_Datei.getline(Zeile,50);
				startHoehe = atof(Zeile);
				continue;
			}
		}
		HL_Parameter_Ini_Datei.close();
	}
	else{
        ofstream Neue_Ini_Datei("HL_Parameter.ini", ios::out|ios_base::trunc);
        if(Neue_Ini_Datei.good())
        {
            Neue_Ini_Datei<<"hoehenSchritt:"<<hoehenSchritt<<"\n";
            Neue_Ini_Datei<<"suchRadius:"<<suchRadius<<"\n";
            Neue_Ini_Datei<<"starthoehe:"<<startHoehe<<"\n";
            Neue_Ini_Datei.close();
        }
	}
    return;
}

void HL_Parameter_Dialog::OnOKButton(wxCommandEvent &event)
{
    IniSpeichern();
    event.Skip();
    return;
}

void HL_Parameter_Dialog::OnTextChange(wxCommandEvent &event)
{
    wxString inhalt = event.GetString();
    int textFeldID = event.GetId();
    switch(textFeldID){
    case IDhoehenSchritt:
        hoehenSchritt = atof(inhalt);
        break;
    case IDsuchRadius:
        suchRadius = atof(inhalt);
        break;
    case IDstartHoehe:
        startHoehe = atof(inhalt);
        break;
    }
    return;
}

void HL_Parameter_Dialog::IniSpeichern(void)
{
    ofstream Neue_Ini_Datei("HL_Parameter.ini", ios::out|ios_base::trunc);
    if(Neue_Ini_Datei.good())
    {
        Neue_Ini_Datei<<"hoehenSchritt:"<<hoehenSchritt<<"\n";
        Neue_Ini_Datei<<"suchRadius:"<<suchRadius<<"\n";
        Neue_Ini_Datei<<"starthoehe:"<<startHoehe<<"\n";
        Neue_Ini_Datei.close();
    }
    return;
}

double HL_Parameter_Dialog::HoleWert(hlParameterID retID) const
{
    switch (retID)
    {
    case IDhoehenSchritt:
        return hoehenSchritt;
    case IDsuchRadius:
        return suchRadius;
    case IDstartHoehe:
        return startHoehe;
    }
    return 0.0;
}
/*ENDE HLParameterDialog*/

/*Koordinaten_Eingabe_Dialog*/
BEGIN_EVENT_TABLE(Koordinaten_Eingabe_Dialog, wxDialog)
    EVT_KEY_DOWN(Koordinaten_Eingabe_Dialog::OnKeyPress)
END_EVENT_TABLE()

Koordinaten_Eingabe_Dialog::Koordinaten_Eingabe_Dialog(wxWindow* parent, wxString titel)
    :wxDialog(parent, wxID_ANY, titel, wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxSTAY_ON_TOP)
{
    m_mama = parent;

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerX = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizerY = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizerZ = new wxBoxSizer(wxHORIZONTAL);

    sizerX->Add(new wxTextCtrl(this, -1, wxT("X"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    sizerY->Add(new wxTextCtrl(this, -1, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    sizerZ->Add(new wxTextCtrl(this, -1, wxT("Z"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);

    xCtrl = new aruDblTxtCtrl(this, wxID_ANY, wxString::Format(wxT("%.2f"), 0.0), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("xCtrl"));
    yCtrl = new aruDblTxtCtrl(this, wxID_ANY, wxString::Format(wxT("%.2f"), 0.0), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("yCtrl"));
    zCtrl = new aruDblTxtCtrl(this, wxID_ANY, wxString::Format(wxT("%.2f"), 0.0), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("zCtrl"));

    xCtrl->SetNachfolger(yCtrl);
    xCtrl->SetVorgaenger(zCtrl);
    yCtrl->SetNachfolger(zCtrl);
    yCtrl->SetVorgaenger(xCtrl);
    zCtrl->SetNachfolger(xCtrl);
    zCtrl->SetVorgaenger(yCtrl);

    sizerX->Add(xCtrl, 0, wxALL, 5);
    sizerY->Add(yCtrl, 0, wxALL, 5);
    sizerZ->Add(zCtrl, 0, wxALL, 5);

    topsizer->Add(sizerX, 1, wxEXPAND, 5);
    topsizer->Add(sizerY, 1, wxEXPAND, 5);
    topsizer->Add(sizerZ, 1, wxEXPAND, 5);

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

Koordinaten_Eingabe_Dialog::~Koordinaten_Eingabe_Dialog()
{

}

void Koordinaten_Eingabe_Dialog::SetzeKoordinaten(const Vektor vek)
{
    xCtrl->ChangeValue(wxString::Format("%0.4f", vek.x()));
    yCtrl->ChangeValue(wxString::Format("%0.4f", vek.y()));
    zCtrl->ChangeValue(wxString::Format("%0.4f", vek.z()));
    return;
}

void Koordinaten_Eingabe_Dialog::SetzeKoordinaten(Achse t_achse, double wert)
{
    switch(t_achse)
    {
    case x:
        xCtrl->ChangeValue(wxString::Format("%0.4f", wert));
        break;
    case y:
        yCtrl->ChangeValue(wxString::Format("%0.4f", wert));
        break;
    case z:
        zCtrl->ChangeValue(wxString::Format("%0.4f", wert));
        break;
    }
    return;
}

double Koordinaten_Eingabe_Dialog::HoleKoordinaten(Achse t_achse)
{
    double rueckgabe = 0;
    switch(t_achse)
    {
    case x:
        rueckgabe = atof(xCtrl->GetValue());
        break;
    case y:
        rueckgabe = atof(yCtrl->GetValue());
        break;
    case z:
        rueckgabe = atof(zCtrl->GetValue());
        break;
    }
    return rueckgabe;
}

void Koordinaten_Eingabe_Dialog::OnKeyPress(wxKeyEvent &event)
{
    if(event.GetKeyCode() == WXK_RETURN||event.GetKeyCode() == WXK_NUMPAD_ENTER)
    {
        aruVektorEvent* dlgEvent = new aruVektorEvent(3000);
        Vektor pOrt = Vektor(atof(xCtrl->GetValue()), -atof(yCtrl->GetValue()), atof(zCtrl->GetValue()));
        dlgEvent->SetzeKoordinaten(pOrt);
        m_mama->GetEventHandler()->QueueEvent(dlgEvent);
        /*NULLEN*/
        xCtrl->ChangeValue(wxString::Format("%0.4f", 0.0));
        yCtrl->ChangeValue(wxString::Format("%0.4f", 0.0));
        zCtrl->ChangeValue(wxString::Format("%0.4f", 0.0));
    }
    if(event.GetKeyCode() == WXK_ESCAPE)
    {
        m_mama->SetFocus();
    }
    event.Skip();
    return;
}
/*ENDE Koordinaten_Eingabe_Dialog*/

/*Double_Eingabe_Dialog*/
BEGIN_EVENT_TABLE(Double_Eingabe_Dialog, wxDialog)
    EVT_TEXT(wxID_ANY, Double_Eingabe_Dialog::OnTextChange)
    EVT_BUTTON(wxID_OK, Double_Eingabe_Dialog::OnOKButton)
    EVT_KEY_DOWN(Double_Eingabe_Dialog::OnKeyPress)
END_EVENT_TABLE()

Double_Eingabe_Dialog::Double_Eingabe_Dialog(wxWindow* parent, wxString titel, wxString eingabeBezeichnung, wxPoint ort)
    :wxDialog(parent, wxID_ANY, titel, ort, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxSTAY_ON_TOP)
{
    m_mama = parent;
    nAktBefehl = -1;

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *eingabeSizer = new wxBoxSizer(wxHORIZONTAL);

    txtCtrlEingabeBezeichnung = new wxTextCtrl(this, -1, eingabeBezeichnung, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE);
    eingabeSizer->Add(txtCtrlEingabeBezeichnung,0, wxEXPAND|wxALL, 5);

    eingabeCtrl = new aruDblTxtCtrl(this, wxID_ANY, wxString::Format(wxT("%.2f"), 0.0), wxDefaultPosition, wxDefaultSize,
                                                         wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL, wxDefaultValidator, wxT("eingabeCtrl"));
    eingabeSizer->Add(eingabeCtrl, 0, wxALL, 5);

    topsizer->Add(new wxTextCtrl(this, -1, wxT("Bitte Zahl eingeben"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE),0, wxEXPAND|wxALL, 5);
    topsizer->Add(eingabeSizer, 0, wxEXPAND, 5);
    ok_Button = new wxButton(this, wxID_OK, wxT("OK"));
    topsizer->Add(ok_Button, 0, wxSHAPED|wxRIGHT, 5);

    //eingabeCtrl->SetVorgaenger(m_mama);
    //eingabeCtrl->SetNachfolger(m_mama);
    txtCtrlInternGeaendert = false;

    SetSizerAndFit(topsizer);
    SetEscapeId(wxID_CANCEL);
}

Double_Eingabe_Dialog::~Double_Eingabe_Dialog()
{

}

void Double_Eingabe_Dialog::ErscheinungAnpassen(wxString neueEingabeBezeichnung, wxString wert, int befID)
{
    txtCtrlInternGeaendert = true;
    txtCtrlEingabeBezeichnung->SetValue(neueEingabeBezeichnung);
    if(wert != wxEmptyString)eingabeCtrl->SetValue(wert);
    nAktBefehl = befID;
    return;
}

void Double_Eingabe_Dialog::OnOKButton(wxCommandEvent &event)
{
    wxString inhalt = eingabeCtrl->GetValue();
    KommaGegenPunkt(&inhalt);
    double uebergabe = atof(inhalt);

    aruDblEvent *myevent = new aruDblEvent();
    myevent->SetzeWert(uebergabe);
    if(nAktBefehl == RUZmBIFrame::bef_ID_drehen)myevent->SetId(idDrehwinkel_perm);
    if(nAktBefehl == RUZmBIFrame::bef_ID_kreisZeichnen)myevent->SetId(idRadius_perm);
    if(nAktBefehl == RUZmBIFrame::bef_ID_kreisRadiusAendern)myevent->SetId(idRadius_perm);
    m_mama->GetEventHandler()->AddPendingEvent(*myevent);
    event.Skip();
    return;
}

void Double_Eingabe_Dialog::OnTextChange(wxCommandEvent &event)
{
    if(!txtCtrlInternGeaendert)
    {
        wxString inhalt = eingabeCtrl->GetValue();
        KommaGegenPunkt(&inhalt);
        double uebergabe = atof(inhalt);

        aruDblEvent *myevent = new aruDblEvent();
        myevent->SetzeWert(uebergabe);
        if(nAktBefehl == RUZmBIFrame::bef_ID_drehen)myevent->SetId(idDrehwinkel_temp);
        if(nAktBefehl == RUZmBIFrame::bef_ID_kreisZeichnen)myevent->SetId(idRadius_temp);
        if(nAktBefehl == RUZmBIFrame::bef_ID_kreisRadiusAendern)myevent->SetId(idRadius_temp);
        m_mama->GetEventHandler()->AddPendingEvent(*myevent);
    }
    txtCtrlInternGeaendert = false;

    event.Skip();
    return;
}

void Double_Eingabe_Dialog::OnKeyPress(wxKeyEvent &event)
{
    if(event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER)
    {
        ok_Button->SetFocus();
    }
    event.Skip();
    return;
}
/*ENDE Double_Eingabe_Dialog*/

/*Sonnenstand_Dialog*/
BEGIN_EVENT_TABLE(Sonnenstand_Dialog, wxDialog)
    EVT_PAINT(Sonnenstand_Dialog::OnPaint)
    EVT_LEFT_DOWN(Sonnenstand_Dialog::OnMouseClick)
    EVT_MOTION(Sonnenstand_Dialog::OnMouseMove)
END_EVENT_TABLE()

Sonnenstand_Dialog::Sonnenstand_Dialog(wxWindow* parent, Vektor sonnenstand)
    :wxDialog(parent, wxID_ANY, wxT("Sonnenstand"), wxDefaultPosition, wxSize(300, 300), wxCAPTION|wxCLOSE_BOX|wxSTAY_ON_TOP)
{
    m_mama = parent;
    m_sonnenstand = sonnenstand;
    Refresh(false);
    return;
}

Sonnenstand_Dialog::~Sonnenstand_Dialog()
{

}

void Sonnenstand_Dialog::SetzeSonnenstand(Vektor vek)
{
    m_sonnenstand = vek;
    m_sonnenstand /= vek.Laenge();
    Refresh(false);
    return;
}
void Sonnenstand_Dialog::OnClose(wxCloseEvent &event)
{
    SonnenstandMelden();
    Destroy();
    return;
}

void Sonnenstand_Dialog::OnMouseClick(wxMouseEvent &event)
{
    wxClientDC dc(this);
    wxPoint mousePosition = event.GetLogicalPosition(dc);
    SonnenstandErrechnen(mousePosition);
    SonnenstandMelden();
    Refresh(false);
    return;
}

void Sonnenstand_Dialog::OnMouseMove(wxMouseEvent &event)
{
    if(event.Dragging() && event.LeftIsDown())
    {
        wxClientDC dc(this);
        wxPoint mousePosition = event.GetLogicalPosition(dc);
        SonnenstandErrechnen(mousePosition);
        SonnenstandMelden();
        Refresh(false);
    }
    return;
}

void Sonnenstand_Dialog::SonnenstandMelden(void)
{
    aruVektorEvent* dlgEvent = new aruVektorEvent(3001);
    dlgEvent->SetzeKoordinaten(m_sonnenstand);
    m_mama->GetEventHandler()->QueueEvent(dlgEvent);
    return;
}

void Sonnenstand_Dialog::SonnenstandErrechnen(wxPoint pkt)
{
    wxClientDC CL_dc(this);
    int groesse = CL_dc.GetSize().GetHeight();
    if(groesse > CL_dc.GetSize().GetWidth())
    {
        groesse = CL_dc.GetSize().GetWidth();
    }

    double x, y, z, grundLaenge;
    x = (pkt.x - groesse/2) / double((groesse-50)/2);
    y = (pkt.y - groesse/2) / double((groesse-50)/2);
    grundLaenge = sqrt(pow(x, 2) + pow(y, 2));
    if(grundLaenge > 1)
    {
        x /= grundLaenge;
        y /= grundLaenge;
        z = 0;
    }else{
        z = sqrt(1 - pow(x, 2) - pow(y, 2));
    }
    m_sonnenstand.SetKoordinaten(x, y, z);
    return;
}

void Sonnenstand_Dialog::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    wxClientDC CL_dc(this);

    /*Hintergrund zeichnen*/
    wxRect rect(wxPoint(0, 0), CL_dc.GetSize());
    dc.SetBrush(wxBrush(this->GetBackgroundColour()));
    dc.SetPen(wxPen(this->GetBackgroundColour(), 1));
    dc.DrawRectangle(rect);

    int groesse = CL_dc.GetSize().GetHeight();
    if(groesse > CL_dc.GetSize().GetWidth())
    {
        groesse = CL_dc.GetSize().GetWidth();
    }

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_DOT));
    dc.DrawCircle(wxPoint(groesse/2, groesse/2), (groesse-50)/2);

    dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_SOLID));
    dc.DrawLine(wxPoint(groesse/2, groesse/2),
                wxPoint(int(((groesse-50)*m_sonnenstand.GetKoordinaten(0)+groesse)/2),
                        int(((groesse-50)*m_sonnenstand.GetKoordinaten(1)+groesse)/2)));

    dc.DrawText(wxString::Format("%0.5f / %0.5f / %0.5f", m_sonnenstand.GetKoordinaten(0),
                                 m_sonnenstand.GetKoordinaten(1), m_sonnenstand.GetKoordinaten(2)), wxPoint(0, 0));
    return;
}
/*ENDE Sonnenstand_Dialog*/

/*Event für Vektoreingabe*/
aruVektorEvent::aruVektorEvent(int id, wxEventType commandType):wxCommandEvent(commandType, id)
{

}
 aruVektorEvent::aruVektorEvent(const aruVektorEvent& event):wxCommandEvent(event)
{
    this->SetzeKoordinaten(event.HoleKoordinaten());
}

wxEvent* aruVektorEvent::Clone()const
{
    return(new aruVektorEvent(*this));
}

Vektor aruVektorEvent::HoleKoordinaten(void)const
{
    return(koor);
}

void aruVektorEvent::SetzeKoordinaten(const Vektor vek)
{
    koor = vek;
    return;
}
/*ENDE Event für Koordinateneingabe*/

/*Event für double Übergabe*/
aruDblEvent::aruDblEvent(wxEventType commandType, int id):wxCommandEvent(commandType, id)
{

}
 aruDblEvent::aruDblEvent(const aruDblEvent& event):wxCommandEvent(event)
{
    this->SetzeWert(event.HoleWert());
}

wxEvent* aruDblEvent::Clone()const
{
    return(new aruDblEvent(*this));
}

double aruDblEvent::HoleWert(void)const
{
    return(m_wert);
}

void aruDblEvent::SetzeWert(const double wert)
{
    m_wert = wert;
    return;
}
/*ENDE Event für double Übergabe*/

/*Event für wxColour Übergabe*/
aruColourEvent::aruColourEvent(wxEventType commandType, int id):wxCommandEvent(commandType, id)
{

}

aruColourEvent::aruColourEvent(const aruColourEvent& event):wxCommandEvent(event)
{
    this->SetzeWert(event.HoleWert());
}

wxEvent* aruColourEvent::Clone()const
{
    return(new aruColourEvent(*this));
}

wxColour aruColourEvent::HoleWert(void)const
{
    return(m_wert);
}

void aruColourEvent::SetzeWert(const wxColour wert)
{
    m_wert = wert;
    return;
}
/*ENDE Event für wxColour Übergabe*/

/*Event für Liste<RUZ_Layer> Übergabe*/
aruLayerListeEvent::aruLayerListeEvent(int id, wxEventType commandType):wxCommandEvent(commandType, id)
{
    m_lst = new Liste<RUZ_Layer>;
}

aruLayerListeEvent::aruLayerListeEvent(const aruLayerListeEvent& event):wxCommandEvent(event)
{
    wxMessageDialog(NULL, wxT("aruLayerListeEvent kopieren")).ShowModal();
    m_lst = new Liste<RUZ_Layer>;
    Liste<RUZ_Layer>* t_lst = event.HoleLayerListe();
    if(m_lst && t_lst)
    {
        for(RUZ_Layer* layer = t_lst->GetErstesElement(); layer; layer = t_lst->GetNaechstesElement())
        {
            m_lst->Hinzufuegen(layer);
        }
    }else
    {
        wxMessageDialog(NULL, wxT("aruLayerListeEvent Fehlschlag")).ShowModal();
    }
}

aruLayerListeEvent::~aruLayerListeEvent()
{
    m_lst->ListeLeeren("");
    delete m_lst;
}

wxEvent* aruLayerListeEvent::Clone()const
{
    wxMessageDialog(NULL, wxT("aruLayerListeEvent Clonen")).ShowModal();
    return(new aruLayerListeEvent(*this));
}

Liste<RUZ_Layer>* aruLayerListeEvent::HoleLayerListe(void)const
{
    return(m_lst);
}

void aruLayerListeEvent::Hinzufuegen(RUZ_Layer* layer)
{
    m_lst->Hinzufuegen(layer);
    return;
}
/*ENDE Event für Liste<RUZ_Layer> Übergabe*/

void PrismaSchreiben(std::string &prismenListe, Flaeche* drk, double flaeche, double volumen)
{
	char tempStr[21];
<<<<<<< HEAD
	long long int i;
	
	if(drk->HoleTyp() != RUZ_Dreieck)return;
=======
	long long int i;
	
	if(drk->HoleTyp() != RUZ_Dreieck)return;
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
	
	prismenListe += "\n";
	/*1. Punkt*/
	Vektor ort = drk->HolePunkt(0)->HolePosition();
	snprintf(tempStr, 21, "%0.3f", ort.x());
<<<<<<< HEAD
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", flaeche);
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", volumen);
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
=======
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", flaeche);
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", volumen);
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
	prismenListe += tempStr;
	prismenListe += "\n";
	
	/*2. Punkt*/
	ort = drk->HolePunkt(1)->HolePosition();
	snprintf(tempStr, 21, "%0.3f", ort.x());
<<<<<<< HEAD
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
=======
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
	prismenListe += tempStr;
	prismenListe += "\n";
	
	/*3. Punkt*/
	ort = drk->HolePunkt(2)->HolePosition();
	snprintf(tempStr, 21, "%0.3f", ort.x());
<<<<<<< HEAD
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	prismenListe += "\n";
	return;
}
=======
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.y());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	snprintf(tempStr, 21, "%0.3f", ort.z());
	for(i = 0; i < 20 - (long long int)(strlen(tempStr)); i++)
	{
		prismenListe += " ";
	}
	prismenListe += tempStr;
	prismenListe += "\n";
	return;
}
>>>>>>> 0df679c371227fdccb6c484a0394e490e384aaeb
