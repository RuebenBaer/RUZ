/***************************************************************
 * Name:      RUZThreadCtrl.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ansgar Rütten ()
 * Created:   2018-08-25
 * Copyright: Ansgar Rütten ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "RUZThreadCtrl.h"

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_PAINT(MyPanel::OnPaint)
END_EVENT_TABLE()

MyPanel::MyPanel(thread_info_verschnitt* _thInf, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    :wxPanel(parent, id, pos, size)
{
    thInf = _thInf;
    thInf->HoleLayer(&hilfsLayer, &randLayer1, &randLayer2, &Layer1, &Layer2);
}

MyPanel::~MyPanel()
{
}

void MyPanel::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  dc.SetPen(wxPen(wxColour(117, 129 ,115), 3));
  dc.SetBrush(wxBrush(wxColour(255, 211, 35)));
  dc.DrawRectangle(wxPoint(0, 0), dc.GetSize());
  if(thInf->BeendenAngefragt())
  {
    dc.DrawText(wxString::Format("Beende Thread 'Verschneidung'\n"), 30, 30);
  }else{
    wxString msg = wxString::Format("%d Schnittlinien gefunden\n\n", hilfsLayer->HoleLinien()->GetListenGroesse());
    int iStatus = thInf->HoleStatus();
    if(iStatus == 1){
      msg += wxString::Format("Verschneide %s: ... ", Layer1->HoleName());
      msg += wxString::Format("%d / %d\n", thInf->HoleBearbeitet(), thInf->HoleGesamtzahl());
      msg += wxString::Format("  Punkte:  %d\n  Linien:  %d\n  Flächen:  %d\n",
                                Layer1->HolePunkte()->GetListenGroesse(),
                                Layer1->HoleLinien()->GetListenGroesse(),
                                Layer1->HoleFlaechen()->GetListenGroesse());
    }

    if(iStatus == 2){
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: ... \n", Layer1->HoleName(), Layer1->HoleName());
      msg += wxString::Format("%d / %d\n", thInf->HoleBearbeitet(), thInf->HoleGesamtzahl());
      msg += wxString::Format("  Punkte:  %d\n  Linien:  %d\n  Flächen:  %d\n",
                                Layer1->HolePunkte()->GetListenGroesse(),
                                Layer1->HoleLinien()->GetListenGroesse(),
                                Layer1->HoleFlaechen()->GetListenGroesse());
    }
    if(iStatus == 3){
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: erledigt\n", Layer1->HoleName(), Layer1->HoleName());
      msg += wxString::Format("Verschneide %s: ... ", Layer2->HoleName());
      msg += wxString::Format("%d / %d\n", thInf->HoleBearbeitet(), thInf->HoleGesamtzahl());
      msg += wxString::Format("  Punkte:  %d\n  Linien:  %d\n  Flächen:  %d\n",
                                Layer2->HolePunkte()->GetListenGroesse(),
                                Layer2->HoleLinien()->GetListenGroesse(),
                                Layer2->HoleFlaechen()->GetListenGroesse());
    }
    if(iStatus == 4){
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: erledigt\n", Layer1->HoleName(), Layer1->HoleName());
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: ... \n", Layer2->HoleName(), Layer2->HoleName());
      msg += wxString::Format("%d / %d\n", thInf->HoleBearbeitet(), thInf->HoleGesamtzahl());
      msg += wxString::Format("  Punkte:  %d\n  Linien:  %d\n  Flächen:  %d\n",
                                Layer2->HolePunkte()->GetListenGroesse(),
                                Layer2->HoleLinien()->GetListenGroesse(),
                                Layer2->HoleFlaechen()->GetListenGroesse());
    }
    if(iStatus == 5){
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: erledigt\n", Layer1->HoleName(), Layer1->HoleName());
      msg += wxString::Format("Verschneide %s: erledigt\nSchneide %s am Rand ab: erledigt\n", Layer2->HoleName(), Layer2->HoleName());
      msg += wxString::Format("\nLösche freiliegende Flächen!\n\t%s: %d\n\t%s: %d",
                                Layer1->HoleName(),
                                Layer1->HoleFlaechen()->GetListenGroesse(),
                                Layer2->HoleName(),
                                Layer2->HoleFlaechen()->GetListenGroesse());
    }
    dc.DrawText(msg, 30, 30);
  }
  return;
}


BEGIN_EVENT_TABLE(RUZThreadCtrl, wxDialog)
  EVT_TIMER(tc_timerID, RUZThreadCtrl::OnTimer)
  EVT_BUTTON(tc_abbruchID, RUZThreadCtrl::OnAbbruch)
  EVT_PAINT(RUZThreadCtrl::OnPaint)
  EVT_ERASE_BACKGROUND(RUZThreadCtrl::OnEraseBackground)
END_EVENT_TABLE()

RUZThreadCtrl::RUZThreadCtrl(thread_info_verschnitt *_thInf, int _timerTick, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size)
: wxDialog(parent, id, title, pos, size)
{
  thInf = _thInf;
  TIMERTICK = _timerTick;

  sizerButtons = new wxBoxSizer(wxHORIZONTAL);
  sizerButtons->Add(new wxButton(this, tc_abbruchID, wxT("Abbruch")), 0, wxSHAPED|wxRIGHT, 5);

  sizerText = new wxBoxSizer(wxHORIZONTAL);
  pAnzeige = new MyPanel(thInf, this, wxID_ANY, wxPoint(0, 0), wxSize(1000,500));
  sizerText->Add(pAnzeige, 0, wxALL, 5);

  sizerHaupt = new wxBoxSizer(wxVERTICAL);

  sizerHaupt->Add(sizerText, 5, wxEXPAND|wxALL, 5);
  sizerHaupt->Add(sizerButtons, 1, wxSHAPED);

  timer.SetOwner(this, tc_timerID);
  timer.Start(TIMERTICK);

  SetSizerAndFit(sizerHaupt);
  //SetEscapeId(wxID_CANCEL);
}

RUZThreadCtrl::~RUZThreadCtrl()
{

}

void RUZThreadCtrl::OnTimer(wxTimerEvent &event)
{
  if(thInf->IstBeendet())
  {
    timer.Stop();
    Destroy();
  }
  pAnzeige->Refresh();
  return;
}

void RUZThreadCtrl::OnAbbruch(wxCommandEvent &event)
{
  //timer.Stop();
  thInf->BeendenAnfragen();
  pAnzeige->Refresh();
  return;
}

void RUZThreadCtrl::OnPaint(wxPaintEvent &event)
{
  return;
}

void RUZThreadCtrl::OnEraseBackground(wxEraseEvent &event)
{
  /*wir machen nichts!*/
  return;
}