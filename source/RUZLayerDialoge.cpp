#include "RUZLayerDialoge.h"

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
			neuesListItem.SetText(wxString(laeufer_Layer->HoleName()));
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
