/***************************************************************
 * Name:		RUZVernetzenThCtrl.cpp
 * Purpose:		Code for Application Frame
 * Author:		Ansgar Rütten ()
 * Created:		2018-08-25
 * Copyright:	Ansgar Rütten ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "RUZVernetzenThCtrl.h"

VernetzenThCtrlPanel::VernetzenThCtrlPanel(thread_info_verschnitt* _thInf, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	:wxPanel(parent, id, pos, size)
{
	thInf = _thInf;
	thInf->HoleLayer(&m_Layer);

	Bind(wxEVT_PAINT, &VernetzenThCtrlPanel::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &VernetzenThCtrlPanel::OnEraseBackground, this);
}

VernetzenThCtrlPanel::~VernetzenThCtrlPanel()
{
}

void VernetzenThCtrlPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetPen(wxPen(wxColour(117, 129 ,115), 3));
	dc.SetBrush(wxBrush(wxColour(255, 211, 35)));
	dc.DrawRectangle(wxPoint(0, 0), dc.GetSize());
	if(thInf->BeendenAngefragt())
	{
		dc.DrawText(wxString::Format("Beende Thread 'Verschneidung'\n"), 30, 30);
		return;
	}

	wxString msg = wxString::Format("Vernetzen\n\n");
	int iStatus = thInf->HoleStatus();
	if(iStatus == 0){
		msg += wxString::Format("Status = %d", iStatus);
	}
	if(iStatus == 1){
		msg += wxString::Format("Status = %d", iStatus);
	}

	if(iStatus == 2){
		msg += wxString::Format("Status = %d", iStatus);
	}
	if(iStatus == 3){
		msg += wxString::Format("Status = %d", iStatus);
	}
	if(iStatus == 4){
		msg += wxString::Format("Status = %d", iStatus);
	}
	dc.DrawText(msg, 30, 30);
	return;
}

void VernetzenThCtrlPanel::OnEraseBackground(wxEraseEvent &event)
{
	/*nichts machen, damit es nicht flackert*/
	return;
}

RUZVernetzenThCtrl::RUZVernetzenThCtrl(thread_info_verschnitt *_thInf, int _timerTick, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size)
: wxDialog(parent, id, title, pos, size)
{
	thInf = _thInf;
	TIMERTICK = _timerTick;

	sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	sizerButtons->Add(new wxButton(this, tc_netz_abbruchID, wxT("Abbruch")), 0, wxSHAPED|wxRIGHT, 5);

	sizerText = new wxBoxSizer(wxHORIZONTAL);
	pAnzeige = new VernetzenThCtrlPanel(thInf, this, wxID_ANY, wxPoint(0, 0), wxSize(1000,500));
	sizerText->Add(pAnzeige, 0, wxALL, 5);

	sizerHaupt = new wxBoxSizer(wxVERTICAL);

	sizerHaupt->Add(sizerText, 5, wxEXPAND|wxALL, 5);
	sizerHaupt->Add(sizerButtons, 1, wxSHAPED);

	timer.SetOwner(this, tc_netz_timerID);
	timer.Start(TIMERTICK);

	SetSizerAndFit(sizerHaupt);

	Bind(wxEVT_TIMER, &RUZVernetzenThCtrl::OnTimer, this, tc_netz_timerID);
	Bind(wxEVT_BUTTON, &RUZVernetzenThCtrl::OnAbbruch, this, tc_netz_abbruchID);
	Bind(wxEVT_PAINT, RUZVernetzenThCtrl::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &RUZVernetzenThCtrl::OnEraseBackground, this);
}

RUZVernetzenThCtrl::~RUZVernetzenThCtrl()
{

}

void RUZVernetzenThCtrl::OnTimer(wxTimerEvent &event)
{
	if(thInf->IstBeendet())
	{
		timer.Stop();
		Destroy();
	}
	pAnzeige->Refresh();
	return;
}

void RUZVernetzenThCtrl::OnAbbruch(wxCommandEvent &event)
{
	thInf->BeendenAnfragen();
	pAnzeige->Refresh();
	return;
}

void RUZVernetzenThCtrl::OnPaint(wxPaintEvent &event)
{
	return;
}

void RUZVernetzenThCtrl::OnEraseBackground(wxEraseEvent &event)
{
	/*wir machen nichts!*/
	return;
}
