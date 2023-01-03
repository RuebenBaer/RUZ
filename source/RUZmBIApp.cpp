/***************************************************************
 * Name:      RUZmBIApp.cpp
 * Purpose:   Code for Application Class
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

#include "RUZmBIApp.h"
#include "RUZmBIMain.h"

IMPLEMENT_APP(RUZmBIApp);

bool RUZmBIApp::OnInit()
{
    RUZmBIFrame* frame = new RUZmBIFrame(0L, wxT("RechnerUnterstütztes Zeichnen"),wxPoint(0,0),wxSize(400,400),wxDEFAULT_FRAME_STYLE);
    frame->SetIcon(wxICON(A_ruzIcon)); // To Set App Icon
    SetExitOnFrameDelete(TRUE);
    frame->Show();
    SetTopWindow(frame);

    return true;
}
