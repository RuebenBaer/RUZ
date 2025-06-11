#ifndef RUZBILDHG_H
#define RUZBILDHG_H

#include <wx/wx.h>
#include <iostream>

struct RUZHgBild{
	wxImage m_anzeige;
	wxImage m_original;
	double m_rotation, m_skalierung;
	double posX, posY;
	
	wxImageHandler *JPEGHandler = NULL;
	wxImageHandler *PNGHandler = NULL;
};

void HgBild_Init(RUZHgBild &Hg, double rot, double skal, double x, double y);
void HgBild_Rotieren(RUZHgBild &Hg, double dRot);
void HgBild_Verschieben(RUZHgBild &Hg, double dx, double dy);
void HgBild_Skalieren(RUZHgBild &Hg, double skal);

wxImage& HgBild_HoleHintergrund(RUZHgBild &Hg);
void HgBild_HolePosition(RUZHgBild &Hg, double *x, double *y);

#endif