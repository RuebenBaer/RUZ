#include "RUZHgBild.h"

void HgBild_Init(RUZHgBild &Hg, double rot, double skal, double x, double y)
{
	if (!Hg.JPEGHandler) {
		Hg.JPEGHandler  = new wxJPEGHandler();
		wxImage::AddHandler(Hg.JPEGHandler);
	}
	
	if (!Hg.PNGHandler) {
		Hg.PNGHandler  = new wxPNGHandler();
		wxImage::AddHandler(Hg.PNGHandler);
	}
	
	wxFileDialog BildDlg((wxWindow*)NULL, wxT("Hintergrundbild öffnen"), wxT(""), wxT(""), wxT("PNG/JPEG-Dateien (*.png, *.jpg)|*png; *.jpg"),1);
	if(BildDlg.ShowModal()==wxID_CANCEL)return;
    wxString DateiEndung, Pfad;
    DateiEndung = Pfad = BildDlg.GetPath();

	/*Endung suchen*/
    for (int i = DateiEndung.Len(); i > 0; i--) {
		if (DateiEndung.GetChar(i) == '.') {
			DateiEndung = Pfad.Right(DateiEndung.Len()-i-1);
			break;
		}
	}

	if (!DateiEndung.CmpNoCase(wxT("jpg")) || !DateiEndung.CmpNoCase(wxT("jpeg"))) {
		Hg.m_original.LoadFile(Pfad, wxBITMAP_TYPE_JPEG);
		std::cout << "JPEG geöffnet: " << (Hg.m_original.IsOk() ? "Ok" : "Not Ok") << "\n";
	} else if (!DateiEndung.CmpNoCase(wxT("png"))) {
		Hg.m_original.LoadFile(Pfad, wxBITMAP_TYPE_PNG);
		std::cout << "PNG geöffnet: " << (Hg.m_original.IsOk() ? "Ok" : "Not Ok") << "\n";
	}else{
		std::cout << "Dateityp wird nicht unterstützt\n";
		return;
	}
	return;
}

void HgBild_DeInit(RUZHgBild &Hg)
{
	Hg.m_anzeige.Destroy();
	Hg.m_original.Destroy();
	return;
}

void HgBild_Rotieren(RUZHgBild &Hg, double dRot)
{
	return;
}

void HgBild_Verschieben(RUZHgBild &Hg, double dx, double dy)
{
	return;
}

void HgBild_Skalieren(RUZHgBild &Hg, double skal)
{
	return;
}

wxImage& HgBild_HoleHintergrund(RUZHgBild &Hg)
{
	return Hg.m_anzeige;
}

void HgBild_HolePosition(RUZHgBild &Hg, double *x, double *y)
{
	x = &(Hg.posX);
	y = &(Hg.posY);
	return;
}