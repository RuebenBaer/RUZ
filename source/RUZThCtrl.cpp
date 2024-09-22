/***************************************************************
 * Name:		RUZThCtrl.cpp
 * Purpose:		Code for Application Frame
 * Author:		Ansgar Rütten ()
 * Created:		2018-08-25
 * Copyright:	Ansgar Rütten ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "RUZThCtrl.h"

ThCtrlPanel::ThCtrlPanel(thread_info* _thInf, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	:wxPanel(parent, id, pos, size)
{
	thInf = _thInf;

	Bind(wxEVT_PAINT, &ThCtrlPanel::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &ThCtrlPanel::OnEraseBackground, this);
}

ThCtrlPanel::~ThCtrlPanel()
{
}

void ThCtrlPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetPen(wxPen(wxColour(117, 129 ,115), 3));
	dc.SetBrush(wxBrush(wxColour(255, 211, 35)));
	dc.DrawRectangle(wxPoint(0, 0), dc.GetSize());
	if(thInf->BeendenAngefragt())
	{
		dc.DrawText(wxString::Format("Beende Thread\n"), 30, 30);
		return;
	}
	
	wxString msg(thInf->HoleMeldung().c_str(), wxConvUTF8);
	dc.DrawText(msg, 30, 30);
	return;
}

void ThCtrlPanel::OnEraseBackground(wxEraseEvent &event)
{
	/*nichts machen, damit es nicht flackert*/
	return;
}

RUZThCtrl::RUZThCtrl(thread_info* _thInf, int _timerTick, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size)
: wxDialog(parent, id, title, pos, size)
{
	thInf = _thInf;
	TIMERTICK = _timerTick;

	sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	sizerButtons->Add(new wxButton(this, tc_abbruchID, wxT("Abbruch")), 0, wxSHAPED|wxRIGHT, 5);

	sizerText = new wxBoxSizer(wxHORIZONTAL);
	pAnzeige = new ThCtrlPanel(thInf, this, wxID_ANY, wxPoint(0, 0), wxSize(1000,500));
	sizerText->Add(pAnzeige, 0, wxALL, 5);

	sizerHaupt = new wxBoxSizer(wxVERTICAL);

	sizerHaupt->Add(sizerText, 5, wxEXPAND|wxALL, 5);
	sizerHaupt->Add(sizerButtons, 1, wxSHAPED);

	timer.SetOwner(this, tc_timerID);
	timer.Start(TIMERTICK);

	SetSizerAndFit(sizerHaupt);

	Bind(wxEVT_TIMER, &RUZThCtrl::OnTimer, this, tc_timerID);
	Bind(wxEVT_BUTTON, &RUZThCtrl::OnAbbruch, this, tc_abbruchID);
	Bind(wxEVT_PAINT, RUZThCtrl::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &RUZThCtrl::OnEraseBackground, this);
	
	SetReturnCode(wxID_OK);
}

RUZThCtrl::~RUZThCtrl()
{

}

void RUZThCtrl::OnTimer(wxTimerEvent &event)
{
	if(thInf->IstBeendet())
	{
		timer.Stop();
		Destroy();
	}
	pAnzeige->Refresh();
	return;
}

void RUZThCtrl::OnAbbruch(wxCommandEvent &event)
{
	thInf->BeendenAnfragen();
	pAnzeige->Refresh();
	SetReturnCode(wxID_CANCEL);
	return;
}

void RUZThCtrl::OnPaint(wxPaintEvent &event)
{
	return;
}

void RUZThCtrl::OnEraseBackground(wxEraseEvent &event)
{
	/*wir machen nichts!*/
	return;
}
