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

#ifndef RUZLAYERDIALOGE_H
#define RUZLAYERDIALOGE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/listctrl.h>

#include "Liste\Verkettete_Liste.h"
#include "RUZmBIApp.h"
#include "RUZ\RUZVerwaltung.h"

class RUZmBIFrame;
class aruLayerSizer;

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

#endif //RUZLAYERDIALOGE_H