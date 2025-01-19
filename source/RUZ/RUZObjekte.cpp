/*TODO*/

#include "RUZObjekte.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>

using namespace std;

/*RUZ_Hoehenlinie*/
RUZ_Hoehenlinie::RUZ_Hoehenlinie(double xa, double ya, double xe, double ye, double h):
	x_a(xa), y_a(ya), x_e(xe), y_e(ye), hoehe(h)
{

}

RUZ_Hoehenlinie::~RUZ_Hoehenlinie()
{

}

double RUZ_Hoehenlinie::x(int stelle)
{
	if(stelle == 0)return(x_a);
	return(x_e);
}
double RUZ_Hoehenlinie::y(int stelle)
{
	if(stelle == 0)return(y_a);
	return(y_e);
}

double RUZ_Hoehenlinie::h(void)
{
	return(hoehe);
}
/*ENDE RUZ_Hoehenlinie*/

/*Strich*/
Strich::Strich(double xa, double ya, double xe, double ye, RUZ_Layer *layer)
{
	m_AnfangX = xa;
	m_AnfangY = ya;
	m_EndeX = xe;
	m_EndeY = ye;
	m_layer = layer;
	layer->Hinzufuegen(this);
}

Strich::Strich(double xa, double ya, double xe, double ye)
{
	m_AnfangX = xa;
	m_AnfangY = ya;
	m_EndeX = xe;
	m_EndeY = ye;
}

Strich::~Strich()
{
	if(m_layer)m_layer->Entfernen(this);
}

double Strich::Xa(void)
{
	return m_AnfangX;
}

double Strich::Ya(void)
{
	return m_AnfangY;
}

double Strich::Xe(void)
{
	return m_EndeX;
}

double Strich::Ye(void)
{
	return m_EndeY;
}

void Strich::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	m_AnfangX = (m_AnfangX - festOrt.x())*fktX + festOrt.x();
	m_AnfangY = (m_AnfangY - festOrt.y())*fktY + festOrt.y();
	m_EndeX = (m_EndeX - festOrt.x())*fktX + festOrt.x();
	m_EndeY = (m_EndeY - festOrt.y())*fktY + festOrt.y();
	return;
}
/*ENDE Strich*/

/*Bogen*/
Bogen::Bogen(double xa, double ya, double xe, double ye, double xm, double ym, RUZ_Layer *layer):Strich(xa, ya, xe, ye)
{
	m_MitteX = xm;
	m_MitteY = ym;
	m_layer = layer;
	layer->Hinzufuegen(this);
}

Bogen::~Bogen()
{

}

double Bogen::Xm(void)
{
	return m_MitteX;
}

double Bogen::Ym(void)
{
	return m_MitteY;
}

void Bogen::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	m_MitteX = (m_MitteX - festOrt.x())*fktX + festOrt.x();
	m_MitteY = (m_MitteY - festOrt.y())*fktY + festOrt.y();
	Strich::Skalieren(festOrt, fktX, fktY, fktZ);
	return;
}
/*ENDE Bogen*/

/*RUZ_Objekt*/
RUZ_Objekt::RUZ_Objekt()
{
	/*Log starten*/
	ofstream Logbuch;
	const char *pfad = "log/Debug.log";
	Logbuch.open(pfad, ios_base::out|ios_base::app);
	if(Logbuch.good())
	{
		Logbuch.close();
	}
}

RUZ_Objekt::~RUZ_Objekt()
{

}

int RUZ_Objekt::QuadratischeGleichung(double a, double b, double c, double& lsg1, double& lsg2)
{
	if(a)
	{
		double dRadikant = b * b - 4 * a * c;
		if(!(dRadikant<0))
		{
			lsg1 = (-b + sqrt(dRadikant)) / (2 * a);
			lsg2 = (-b - sqrt(dRadikant)) / (2 * a);
			return 2; //zwei reale Lösungen
		}
		return 0; //keine realen Lösungen
	}
	if(b)
	{
		lsg1 = lsg2 = -c/b;
		return 1; //eine reale Doppellösung
	}
	return 0; //keine realen Lösungen
}

void RUZ_Objekt::logSchreiben(const char* msg, ...)
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
/*ENDE RUZ_Objekt*/

/*Punkt*/
Punkt::Punkt():RUZ_Objekt()
{
	pOrt = Vektor(0, 0, 0);
	adjLinie = new Liste<Linie>;
	m_layer = NULL;
}

Punkt::Punkt(const Punkt& Pkt):RUZ_Objekt()
{
	pOrt = Pkt.pOrt;
	adjLinie = Pkt.HoleLinien();
	m_layer = Pkt.HoleLayer();
	m_layer->Hinzufuegen(this);
}

Punkt::Punkt(const Vektor& vkt, RUZ_Layer *lay):RUZ_Objekt()
{
	pOrt = vkt;
	adjLinie = new Liste<Linie>;
	m_layer = lay;
	m_layer->Hinzufuegen(this);
}

Punkt::Punkt(double x, double y, double z, RUZ_Layer *lay):RUZ_Objekt()
{
	pOrt = Vektor(x, y, z);
	adjLinie = new Liste<Linie>;
	m_layer = lay;
	m_layer->Hinzufuegen(this);
}

Punkt::Punkt(double *koor, RUZ_Layer *lay):RUZ_Objekt()
{
	pOrt = Vektor(koor);
	adjLinie = new Liste<Linie>;
	m_layer = lay;
	m_layer->Hinzufuegen(this);
}

Punkt::~Punkt()
{
	Aufraeumen();
	delete adjLinie;

	if(m_layer)
	{
		m_layer->Entfernen(this);
	}
}

Punkt& Punkt::operator=(const Punkt& Pkt)
{
	if(this == &Pkt) return *this;

	Aufraeumen();
	if(m_layer)m_layer->Entfernen(this);
	pOrt = Pkt.pOrt;
	adjLinie = Pkt.HoleLinien();
	m_layer = Pkt.HoleLayer();
	m_layer->Hinzufuegen(this);
	return *this;
}

void Punkt::SetzeName(const char* n)
{
	strncpy(m_name, n, sizeof(m_name)-1);
	m_name[sizeof(m_name)-1] = '\0';
	return;
}

bool Punkt::operator==(const Punkt& pkt) const
{
	return(pOrt==pkt.HolePosition());
}

void Punkt::Hinzufuegen(Linie* Ln)
{
	adjLinie->ExklusivHinzufuegen(Ln);
	return;
}

void Punkt::Entfernen(Linie* Ln)
{
	adjLinie->Entfernen(Ln);
	return;
}

void Punkt::Verschieben(const Vektor& vkt)
{
	pOrt+=vkt;
	AenderungAnLayerMitteilen();
	return;
}

void Punkt::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	Vektor tempOrt = (pOrt - festOrt);
	tempOrt.SetKoordinaten(tempOrt.x() * fktX, tempOrt.y() * fktY, tempOrt.z() * fktZ);
	pOrt = tempOrt + festOrt;
	AenderungAnLayerMitteilen();
	return;
}

void Punkt::Positionieren(const Vektor& vkt)
{
	pOrt=vkt;
	AenderungAnLayerMitteilen();
	return;
}

Vektor Punkt::HolePosition(void) const
{
	return pOrt;
}

bool Punkt::HolePosition(Vektor& vkt)
{
	vkt = pOrt;
	return true;
}

RUZ_Objekt* Punkt::Kopieren(Liste<RUZ_Objekt>* lObjekte, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = m_layer;
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	Punkt* pktNeu = new Punkt(pOrt, layZielLayer);
	if(pktNeu)
	{
		lObjekte->Hinzufuegen(pktNeu);
		lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, pktNeu));
		return pktNeu;
	}
	return NULL;
}

Liste<Linie>* Punkt::HoleLinien(void) const
{
	return adjLinie;
}

RUZ_Layer* Punkt::HoleLayer(void) const
{
	return m_layer;
}

void Punkt::SetzeLayer(RUZ_Layer* layer)
{
	m_layer = layer;
	return;
}

void Punkt::SetzeFarbe(float r, float g, float b)
{
	m_farbe[0]=r;
	m_farbe[1]=g;
	m_farbe[2]=b;
	return;
}

float* Punkt::HoleFarbe(void)
{
	return m_farbe;
}

void Punkt::Aufraeumen(void)
{
	for(Linie* Ln = adjLinie->GetErstesElement(); Ln; Ln = adjLinie->GetNaechstesElement())
	{
		delete Ln;
		Ln=0;
	}
	return;
}

bool Punkt::IstNahebei(double qx, double qy, double maxAbstand, Achse prjRichtung)
{
	double entfernung;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	entfernung = sqrt(pow(this->HolePosition().GetKoordinaten(x) - qx,2)
					  + pow(this->HolePosition().GetKoordinaten(y) - qy,2));

	return (entfernung < maxAbstand);
}

void Punkt::ErsetzenDurch(Punkt* ersatzPkt)
{
	if(!(ersatzPkt->HoleLayer() == this->HoleLayer()))return;
	if(ersatzPkt == this)return;/*verhindert das Vereinigen eines Punktes mit sich selber*/
	for(Linie* aktLn = adjLinie->GetErstesElement(); aktLn != NULL; aktLn = adjLinie->GetNaechstesElement())
	{
		for(int i = 0; i < 2; i++)
		{
			if(aktLn->HolePunkt(i) == this)aktLn->SetzePunkt(i, ersatzPkt);
		}
	}
	/*Flächen auf Integrität prüfen*/
	Liste<Linie> *lnLst = ersatzPkt->HoleLinien();
	Liste<Flaeche> *flLst;
	for(Linie *aktLn = lnLst->GetErstesElement(); aktLn; aktLn = lnLst->GetNaechstesElement())
	{
		flLst = aktLn->HoleFlaechen();
		for(Flaeche *aktFl = flLst->GetErstesElement(); aktFl; aktFl = flLst->GetNaechstesElement())
		{
			aktFl->EckenNeuErmitteln();
		}
	}
	/*Linien auf Integrität, Nulllänge und doppeltes Vorhandensein prüfen*/
	Listenelement<Linie> *tempLE;
	for(Listenelement<Linie> *akt_LE_Ln = lnLst->GetErstesListenelement(); akt_LE_Ln;)
	{
		Linie *aktLn = akt_LE_Ln->GetElement();
		if(aktLn->HolePunkt(0) == aktLn->HolePunkt(1))
		{
			tempLE = akt_LE_Ln->GetNachfolger();
			lnLst->Entfernen(akt_LE_Ln);
			akt_LE_Ln = tempLE;
			delete aktLn;
			continue;/*nicht, dass akt_LE_Ln noch NULL ist!*/
		}
		for(Listenelement<Linie> *laeufer_LE_Ln = akt_LE_Ln->GetNachfolger(); laeufer_LE_Ln;)
		{
			Linie *laeuferLn = laeufer_LE_Ln->GetElement();
			if(((aktLn->HolePunkt(0) == laeuferLn->HolePunkt(0))&&(aktLn->HolePunkt(1) == laeuferLn->HolePunkt(1)))
			   ||
			   ((aktLn->HolePunkt(0) == laeuferLn->HolePunkt(1))&&(aktLn->HolePunkt(1) == laeuferLn->HolePunkt(0))))
			{
				tempLE = laeufer_LE_Ln->GetNachfolger();
				lnLst->Entfernen(laeufer_LE_Ln);
				laeufer_LE_Ln = tempLE;
				laeuferLn->ErsetzenDurch(aktLn);
				continue;/*nicht, dass laeufer_LE_Ln noch NULL ist!*/
			}
			laeufer_LE_Ln = laeufer_LE_Ln->GetNachfolger();
		}
		akt_LE_Ln = akt_LE_Ln->GetNachfolger();
	}
	delete this;
	return;
}

void Punkt::AenderungAnLayerMitteilen(void)
{
	for(Linie* tempLinie = adjLinie->GetErstesElement(); tempLinie != NULL; tempLinie = adjLinie->GetNaechstesElement())
	{
		Liste<Flaeche>* flLstLn = tempLinie->HoleFlaechen();
		for(Flaeche* tempFlaeche = flLstLn->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = flLstLn->GetNaechstesElement())
		{
			m_layer->GeaenderteFlaecheHinzufuegen(tempFlaeche);
			tempFlaeche->MinMax();
		}
	}
	return;
}

Linie* Punkt::Verbunden(Punkt* pkt)
{
	Linie *ln;

	for (ln = adjLinie->GetErstesElement(); ln != NULL; ln = adjLinie->GetNaechstesElement()) {
		if (ln->HolePunkt(0) == pkt || ln->HolePunkt(1) == pkt)
			return ln;
	}
	
	return NULL;
}

void Punkt::ImLogAusgeben(void)
{
	logSchreiben("%p:\n\t%.3f / %.3f / %.3f\n\n", this, pOrt.x(), pOrt.y(), pOrt.z());
}
/*Ende Punkt*/

/*Linie*/
Linie::Linie()
{
	p[0] = p[1] = NULL;
	teilLinie[0] = teilLinie[1] = NULL;
	geschuetzt = 0;
	kantenTyp = unklassifiziert;
	adjFlaeche = new Liste<Flaeche>;
	flaechenZaehler = 0;
	farbe[0] = farbe[1] = farbe[2] = -1;
}

Linie::Linie(const Linie &Ln):RUZ_Objekt()
{
	if(HoleLayer())HoleLayer()->Entfernen(this);
	p[0] = Ln.p[0];
	p[1] = Ln.p[1];
	p[0]->Hinzufuegen(this);
	p[1]->Hinzufuegen(this);
	unsigned char r, g, b;
	Ln.HoleFarbe(r, g, b);
	farbe[0] = r;
	farbe[1] = g;
	farbe[2] = b;
	teilLinie[0] = teilLinie[1] = NULL;
	geschuetzt = Ln.IstGeschuetzt();
	kantenTyp = Ln.HoleKantenklasse();
	adjFlaeche = Ln.HoleFlaechen();
	flaechenZaehler = Ln.HoleFlaechenZahl();
	HoleLayer()->Hinzufuegen(this);
	*this = Ln;
}

Linie& Linie::operator=(const Linie &Ln)
{
	if(this != &Ln)
	{
		if(HoleLayer())HoleLayer()->Entfernen(this);
		p[0] = Ln.p[0];
		p[1] = Ln.p[1];
		p[0]->Hinzufuegen(this);
		p[1]->Hinzufuegen(this);
		unsigned char r, g, b;
		Ln.HoleFarbe(r, g, b);
		farbe[0] = r;
		farbe[1] = g;
		farbe[2] = b;
		teilLinie[0] = teilLinie[1] = NULL;
		geschuetzt = Ln.IstGeschuetzt();
		kantenTyp = Ln.HoleKantenklasse();
		adjFlaeche = Ln.HoleFlaechen();
		flaechenZaehler = Ln.HoleFlaechenZahl();
		HoleLayer()->Hinzufuegen(this);
	}
	return *this;
}

bool Linie::operator==(const Linie& vergleichsLinie)
{
	for(int i = 0; i < 2; i++)
	{
		for(int k = 0; k < 2; k++)
		{
			if((vergleichsLinie.HolePunkt(i) == p[k])&&(vergleichsLinie.HolePunkt((i+1)%2) == p[(k+1)%2]))return true;
		}
	}
	return false;
}

Linie::Linie(Punkt* p1, Punkt* p2):RUZ_Objekt()
{
	p[0] = p1;
	p[1] = p2;
	p[0]->Hinzufuegen(this);
	p[1]->Hinzufuegen(this);
	teilLinie[0] = teilLinie[1] = NULL;
	geschuetzt = 0;
	kantenTyp = unklassifiziert;
	adjFlaeche = new Liste<Flaeche>;
	flaechenZaehler = 0;
	HoleLayer()->Hinzufuegen(this);
	farbe[0] = farbe[1] = farbe[2] = -1;
}

Linie* Linie::NeueLinie(Punkt* p0, Punkt* p1)
{
	if (p0->HoleLayer() != p1->HoleLayer())return NULL;
	if (p0->HoleLayer() == NULL)return NULL;
	
	Linie* ln;
	if ((ln = p0->Verbunden(p1)) != NULL) {
		std::cout << "Linien waren schon verbunden\n";
		return ln;
	}
	
	return new Linie(p0, p1);
}

Linie::~Linie()
{
	p[0]->Entfernen(this);
	p[1]->Entfernen(this);
	teilLinie[0] = teilLinie[1] = NULL;
	Aufraeumen();
	delete adjFlaeche;

	if(HoleLayer())
	{
		HoleLayer()->Entfernen(this);
	}
}

Punkt* Linie::HolePunkt(int index) const
{
	if(index > 0)
	{
		return p[1];
	}
	return p[0];
}

bool Linie::HolePosition(Vektor& vkt)
{
	vkt = p[0]->HolePosition() + p[1]->HolePosition();
	vkt /=2;
	return true;
}

RUZ_Objekt* Linie::Kopieren(Liste<RUZ_Objekt>* lObjekte, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = this->HoleLayer();
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	RUZ_Objekt *oObj;
	Punkt *pktNeu[2];
	for(int i = 0; i < 2; i++)
	{
		pktNeu[i] = NULL;
		oObj = p[i]->Kopieren(lObjekte, lBereitsKopiert, layZielLayer);
		if(oObj)
		{
			if(oObj->HoleTyp() == RUZ_Punkt)
			{
				pktNeu[i] = static_cast<Punkt*>(oObj);
			}
		}
	}
	if(pktNeu[0] && pktNeu[1])
	{
		Linie* lnNeu = Linie::NeueLinie(pktNeu[0], pktNeu[1]);
		if(lnNeu)
		{
			lObjekte->Hinzufuegen(lnNeu);
			lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, lnNeu));
			return lnNeu;
		}
	}
	return NULL;
}

RUZ_Layer* Linie::HoleLayer(void) const
{
	if(p[0])return (p[0]->HoleLayer());
	return NULL;
}

Vektor Linie::HoleRichtung(void) const
{
	Vektor vRueckgabe = p[1]->HolePosition() - p[0]->HolePosition();
	vRueckgabe /= vRueckgabe.Laenge();
	return vRueckgabe;
}

double Linie::Laenge(void) const
{
	Vektor Rueckgabe = p[0]->HolePosition() - p[1]->HolePosition();
	return(Rueckgabe.Laenge());
}

double Linie::ProjLaenge(Achse projektion) const
{
	Vektor Rueckgabe = p[0]->HolePosition() - p[1]->HolePosition();
	return(Rueckgabe.ProjLaenge((int)projektion));
}

bool Linie::schneidet(double t_cx, double t_cy, double t_dx, double t_dy, Achse prjRichtung)
{
	double cax, cay, cdx, cdy, bax, bay;
	double det0, det1, det2, mue, lambda;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	cax = t_cx - p[0]->HolePosition().GetKoordinaten(x);
	cay = t_cy - p[0]->HolePosition().GetKoordinaten(y);
	cdx = t_cx - t_dx;
	cdy = t_cy - t_dy;
	bax = p[1]->HolePosition().GetKoordinaten(x) - p[0]->HolePosition().GetKoordinaten(x);
	bay = p[1]->HolePosition().GetKoordinaten(y) - p[0]->HolePosition().GetKoordinaten(y);

	det0 = bax * cdy - cdx * bay;
	if(det0 == 0)return false;

	det1 = cax * cdy - cdx * cay;
	det2 = bax * cay - cax * bay;

	lambda = det1 / det0;
	mue = det2 / det0;

	if((lambda > 0)&&(lambda < 1)&&(mue > 0)&&(mue < 1))return true;

	return false;
}

bool Linie::schneidet(Linie* Ln, Achse prjRichtung)
{
	double ax, ay, bx, by, cx, cy, dx, dy, deltax, deltay, m1x, m1y, m2x, m2y, lambda, mue, det;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	ax = (double)p[0]->HolePosition().GetKoordinaten(x);
	ay = (double)p[0]->HolePosition().GetKoordinaten(y);
	bx = (double)p[1]->HolePosition().GetKoordinaten(x);
	by = (double)p[1]->HolePosition().GetKoordinaten(y);
	cx = (double)Ln->HolePunkt(0)->HolePosition().GetKoordinaten(x);
	cy = (double)Ln->HolePunkt(0)->HolePosition().GetKoordinaten(y);
	dx = (double)Ln->HolePunkt(1)->HolePosition().GetKoordinaten(x);
	dy = (double)Ln->HolePunkt(1)->HolePosition().GetKoordinaten(y);

	m1x = bx - ax;
	m1y = by - ay;
	m2x = dx - cx;
	m2y = dy - cy;

	deltax = cx - ax;
	deltay = cy - ay;

	det = -m1x * m2y + m1y * m2x;
	if(det == 0)
	{
		if((deltax * m1y - m1x * deltay) != 0)
		{
			return 0;
		}else{
			if((p[0] != Ln->HolePunkt(0)) && (p[1] != Ln->HolePunkt(0)))
			{
				if((bx - ax) !=0)
				{
					lambda = (cx - ax) / (bx - ax);
				}else{
					lambda = (cy - ay) / (by - ay);
				}
				if((lambda < 1) && (lambda > 0))
				{
					return 1;
				}
			}

			if((p[0] != Ln->HolePunkt(1)) && (p[1] != Ln->HolePunkt(1)))
			{
				if((bx - ax) !=0)
				{
					lambda = (dx - ax) / (bx - ax);
				}else{
					lambda = (dy - ay) / (by - ay);
				}
				if((lambda < 1) && (lambda > 0))
				{
					return 1;
				}
			}

			if((p[0] != Ln->HolePunkt(0)) && (p[0] != Ln->HolePunkt(1)))
			{
				if((dx - cx) !=0)
				{
					lambda = (ax - cx) / (dx - cx);
				}else{
					lambda = (ay - cy) / (dy - cy);
				}
				if((lambda < 1) && (lambda > 0))
				{
					return 1;
				}
			}

			if((p[1] != Ln->HolePunkt(0)) && (p[1] != Ln->HolePunkt(1)))
			{
				if((dx - cx) !=0)
				{
					lambda = (bx - cx) / (dx - cx);
				}else{
					lambda = (by - cy) / (dy - cy);
				}
				if((lambda < 1) && (lambda > 0))
				{
					return 1;
				}
			}
			return 0;
		}
	}

	if(p[0] == Ln->HolePunkt(0))return 0;
	if(p[0] == Ln->HolePunkt(1))return 0;
	if(p[1] == Ln->HolePunkt(0))return 0;
	if(p[1] == Ln->HolePunkt(1))return 0;

	lambda = (-m2y * deltax + m2x * deltay) / det;
	mue = (-m1y * deltax + m1x * deltay) / det;

	if((lambda > 0) && (lambda < 1) && (mue > 0) && (mue < 1))
	{
		return 1;
	}
	return 0;
}

bool Linie::schneidet(Linie* Ln, Vektor &vkt, Achse prjRichtung, bool virtuell)
{
	double cx, cy, dx, dy;
	cx = Ln->HolePunkt(0)->HolePosition().GetKoordinaten(x);
	cy = Ln->HolePunkt(0)->HolePosition().GetKoordinaten(y);
	dx = Ln->HolePunkt(1)->HolePosition().GetKoordinaten(x);
	dy = Ln->HolePunkt(1)->HolePosition().GetKoordinaten(y);

	if(p[0] == Ln->HolePunkt(0))return false;
	if(p[0] == Ln->HolePunkt(1))return false;
	if(p[1] == Ln->HolePunkt(0))return false;
	if(p[1] == Ln->HolePunkt(1))return false;

	return schneidet(cx, cy, dx, dy, vkt, prjRichtung, virtuell);
}

bool Linie::schneidet(double _cx, double _cy, double _dx, double _dy, Vektor &vkt, Achse prjRichtung, bool virtuell)
{
	double ax, ay, bx, by, cx, cy, dx, dy, deltax, deltay, m1x, m1y, m2x, m2y, lambda, mue, det;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	ax = p[0]->HolePosition().GetKoordinaten(x);
	ay = p[0]->HolePosition().GetKoordinaten(y);
	bx = p[1]->HolePosition().GetKoordinaten(x);
	by = p[1]->HolePosition().GetKoordinaten(y);
	cx = _cx;
	cy = _cy;
	dx = _dx;
	dy = _dy;

	m1x = bx - ax;
	m1y = by - ay;
	m2x = dx - cx;
	m2y = dy - cy;

	deltax = cx - ax;
	deltay = cy - ay;

	det = -m1x * m2y + m1y * m2x;
	if(det == 0)return 0;

	lambda = (-m2y * deltax + m2x * deltay) / det;
	mue = (-m1y * deltax + m1x * deltay) / det;

	if((lambda > 0) && (lambda < 1) && (mue > 0) && (mue < 1))
	{
		vkt = PunktBeiLambda(lambda);
		return 1;
	}else
	if(virtuell)
	{
		vkt = PunktBeiLambda(lambda);
		return 1;
	}
	return 0;
}

void Linie::SetzePunkte(Punkt* p1, Punkt* p2)
{
	if(p[0])p[0]->Entfernen(this);
	if(p[1])p[1]->Entfernen(this);
	p[0] = p1;
	p[1] = p2;
	p[0]->Hinzufuegen(this);
	p[1]->Hinzufuegen(this);
	return;
}

void Linie::SetzePunkt(int stelle, Punkt* neuP)
{
	if(p[stelle])p[stelle]->Entfernen(this);
	p[stelle] = neuP;
	p[stelle]->Hinzufuegen(this);
	return;
}

void Linie::Hinzufuegen(Flaeche* fL)
{
	adjFlaeche->ExklusivHinzufuegen(fL);
	flaechenZaehler++;
	return;
}

void Linie::Entfernen(Flaeche* fL)
{
	adjFlaeche->Entfernen(fL);
	flaechenZaehler--;
	return;
}

void Linie::Verschieben(const Vektor& vkt)
{
	for(int i = 0; i < 2; i++)
	{
		p[i]->Verschieben(vkt);
	}
	return;
}

bool Linie::Teilen(Punkt* teilPkt, Achse prjRichtung, bool ungefaehr)
{
	RUZ_Layer *aktLayer = this->HoleLayer();
	if(teilPkt->HoleLayer() != aktLayer)
	{
		logSchreiben("\tLinie::Teilen => Falscher Layer\n");
		return false;
	}
	if((abs(teilPkt->HolePosition().x() == p[0]->HolePosition().x()) &&
		abs(teilPkt->HolePosition().y() == p[0]->HolePosition().y()) &&
		abs(teilPkt->HolePosition().z() == p[0]->HolePosition().z()))
	   ||
	   (abs(teilPkt->HolePosition().x() == p[1]->HolePosition().x()) &&
		abs(teilPkt->HolePosition().y() == p[1]->HolePosition().y()) &&
		abs(teilPkt->HolePosition().z() == p[1]->HolePosition().z())))
	{
		return false;
	}

	if(ungefaehr)
	{
		Vektor t_vkt = teilPkt->HolePosition();
		if(LotFussPunkt(t_vkt, prjRichtung, false))
			teilPkt->Positionieren(t_vkt);
		else
			return false;
	}else
	if(!IstAufLinie(teilPkt->HolePosition(), z))
	{
		return false;
	}

	/*neue Linien erzeugen*/
	Linie *nl[3];
	nl[0] = Linie::NeueLinie(p[0], teilPkt);
	if(nl[0] == NULL)return 0;
	nl[0]->SetzeBesucht('j');
	nl[1] = Linie::NeueLinie(teilPkt, p[1]);
	if(nl[1] == NULL)
	{
		delete nl[0];
		return false;
	}
	nl[1]->SetzeBesucht('j');
	/*ENDE neue Linien erzeugen*/

	/*Für jedes Dreieck neue Dreiecke erzeugen*/
	for(Flaeche *t_fl = adjFlaeche->GetErstesElement(); t_fl != NULL; t_fl = adjFlaeche->GetNaechstesElement())
	{
		if(t_fl->HoleTyp() == RUZ_Dreieck)
		{
			nl[2] = NULL;
			for(int i = 0; i < 3; i++)
			{
				if((t_fl->HolePunkt(i) != p[0])&&(t_fl->HolePunkt(i) != p[1]))
				{
					nl[2] = Linie::NeueLinie(teilPkt, t_fl->HolePunkt(i));
					if(nl[2] == NULL)
					{
						delete nl[0];
						delete nl[1];
						return false;
					}
					nl[2]->SetzeBesucht('j');
				}
			}
			for(int i = 0; i < 3; i++)
			{
				Linie *tempLinie = t_fl->HoleLinie(i);
				if(tempLinie != this)
				{
					if((tempLinie->HolePunkt(0)==p[0])||(tempLinie->HolePunkt(1)==p[0]))
					{
						Dreieck::NeuesDreieck(tempLinie, nl[0], nl[2]);
					}
					if((tempLinie->HolePunkt(0)==p[1])||(tempLinie->HolePunkt(1)==p[1]))
					{
						Dreieck::NeuesDreieck(tempLinie, nl[1], nl[2]);
					}
				}
			}
		}
	}
	/*ENDE Für jedes Dreieck neue Dreiecke erzeugen*/
	delete this;
	return true;
}

bool Linie::LotFussPunkt(Vektor &t_vkt, Achse prjRichtung, bool ausserhalb)
{
	int z;
	z = prjRichtung;
	Vektor v_a, v_b;
	v_a = p[1]->HolePosition() - p[0]->HolePosition();
	v_b = t_vkt - p[0]->HolePosition();
	v_a.SetKoordinaten(z, 0.0);
	v_b.SetKoordinaten(z, 0.0);
	double skalierFaktor = (v_a * v_b) / (v_a * v_a);

	if(!ausserhalb)
	{
		if(skalierFaktor <= 0)
			return 0;
		if(skalierFaktor >= 1)
			return 0;
	}

	t_vkt = (p[0]->HolePosition() + ((p[1]->HolePosition() - p[0]->HolePosition()) * skalierFaktor));
	return 1;
}

void Linie::ZwangsTeilung(Punkt* teilPkt)
{
	RUZ_Layer *aktLayer = this->HoleLayer();
	if(teilPkt->HoleLayer() != aktLayer)
	{
		return;
	}

	/*neue Linien erzeugen*/
	Linie *nl[3];
	nl[0] = Linie::NeueLinie(p[0], teilPkt);
	if(nl[0] == NULL)return;
	nl[1] = Linie::NeueLinie(teilPkt, p[1]);
	if(nl[1] == NULL)
	{
		delete nl[0];
		return;
	}
	/*ENDE neue Linien erzeugen*/

	/*Für jedes Dreieck neue Dreiecke erzeugen*/
	for(Flaeche *t_fl = adjFlaeche->GetErstesElement(); t_fl != NULL; t_fl = adjFlaeche->GetNaechstesElement())
	{
		if(t_fl->HoleTyp() == RUZ_Dreieck)
		{
			nl[2] = NULL;
			for(int i = 0; i < 3; i++)
			{
				if((t_fl->HolePunkt(i) != p[0])&&(t_fl->HolePunkt(i) != p[1]))
				{
					nl[2] = Linie::NeueLinie(teilPkt, t_fl->HolePunkt(i));
					if(nl[2] == NULL)
					{
						delete nl[0];
						delete nl[1];
						return;
					}
				}
			}
			for(int i = 0; i < 3; i++)
			{
				Linie *tempLinie = t_fl->HoleLinie(i);
				if(tempLinie != this)
				{
					if((tempLinie->HolePunkt(0)==p[0])||(tempLinie->HolePunkt(1)==p[0]))
					{
						Dreieck::NeuesDreieck(tempLinie, nl[0], nl[2]);
					}
					if((tempLinie->HolePunkt(0)==p[1])||(tempLinie->HolePunkt(1)==p[1]))
					{
						Dreieck::NeuesDreieck(tempLinie, nl[1], nl[2]);
					}
				}
			}
		}
	}
	/*ENDE Für jedes Dreieck neue Dreiecke erzeugen*/
	delete this;
	return;
}

void Linie::ErsetzenDurch(Linie* ersatzLn)
{
	if(ersatzLn->HoleLayer() != HoleLayer())return;
	for(Flaeche* aktFl = adjFlaeche->GetErstesElement(); aktFl; aktFl = adjFlaeche->GetNaechstesElement())
	{
		aktFl->ErsetzeLinie(this, ersatzLn);
	}
	delete this;
	return;
}

Liste<Flaeche>* Linie::HoleFlaechen(void)const
{
	return adjFlaeche;
}

void Linie::Aufraeumen(void)
{
	for(Flaeche* dR = adjFlaeche->GetErstesElement(); dR; dR = adjFlaeche->GetErstesElement())
	{
		delete dR;
	}
	return;
}

bool Linie::HoleFarbe(unsigned char &r, unsigned char &g, unsigned char &b) const
{
	r = farbe[0];
	g = farbe[1];
	b = farbe[2];
	for(int i = 0; i < 3; i++)
	{
		if(farbe[i] < 0)return false;
	}
	return true;
}

void Linie::SetzeFarbe(unsigned char r, unsigned char g, unsigned char b)
{
	farbe[0] = r;
	farbe[1] = g;
	farbe[2] = b;
	return;
}

void Linie::LoescheFarbe(void)
{
	farbe[0] = farbe[1] = farbe[2] = -1;
	return;
}

void Linie::SetzeKantenklasse(KantenKlasse kK)
{
	if((kK > -1) && (kK < 7))
	{
		kantenTyp = kK;
	}
	return;
}

KantenKlasse Linie::HoleKantenklasse(void)const
{
	return kantenTyp;
}

double Linie::HoleGefaelle(Achse prjRichtung) const
{
	double rueckgabe = 0;
	double laenge = ProjLaenge(prjRichtung);
	if (laenge) {
		rueckgabe = (p[0]->HolePosition().GetKoordinaten(prjRichtung) - p[1]->HolePosition().GetKoordinaten(prjRichtung)) / laenge;
		return(abs(rueckgabe));
	} else {
		return NAN;
	}

	return(abs(rueckgabe));
}

void Linie::HoehenPunkteFinden(Liste<Hoehenpunkt>* pktLst, double hoehenSchritt, double suchRadius, double startHoehe)
{
	/*Zeitbegrenzung vorbereiten*/
	clock_t anfangsZeit = clock();

	double t_hoehenSchritt, t_startHoehe;
	t_hoehenSchritt = hoehenSchritt;
	t_startHoehe = startHoehe;

	double maxHoehe, minHoehe, aktHoehe;
	Vektor tempOrt, v_Anfang, v_Ende;

	v_Anfang = p[0]->HolePosition();
	v_Ende = p[1]->HolePosition();
	if(v_Anfang.z() > v_Ende.z())
	{
		v_Anfang = p[1]->HolePosition();
		v_Ende = p[0]->HolePosition();
	}

	minHoehe = v_Anfang.z();
	maxHoehe = v_Ende.z();
	if(minHoehe == maxHoehe)
	{
		pktLst->Hinzufuegen(new Hoehenpunkt(v_Anfang));
		pktLst->Hinzufuegen(new Hoehenpunkt(v_Ende));
		return;
	}
	if(t_hoehenSchritt == 0)return;
	aktHoehe = t_startHoehe + (ceil((minHoehe - t_startHoehe) / t_hoehenSchritt) * t_hoehenSchritt);
	int durchGangNr = 0;
	while(aktHoehe <= maxHoehe)
	{
		if(aktHoehe<minHoehe)
		{
			aktHoehe = minHoehe;
		}
		tempOrt = (v_Ende - v_Anfang);
		tempOrt *= ((minHoehe - aktHoehe) / (minHoehe - maxHoehe));
		tempOrt += v_Anfang;
		tempOrt.SetKoordinaten(2, aktHoehe);
		pktLst->Hinzufuegen(new Hoehenpunkt(tempOrt));
		aktHoehe += t_hoehenSchritt;
		durchGangNr++;
		/*Zeitbegrenzung*/
		if((abs(clock()-anfangsZeit)/CLOCKS_PER_SEC)>1.0)return;
	}
	return;
}

bool Linie::IstNahebei(double qx, double qy, double maxAbstand, Achse prjRichtung)
{
	double p0x, p0y, p1x, p1y, hpx[4], hpy[4], nx, ny, lage[4], laenge;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	p0x = p[0]->HolePosition().GetKoordinaten(x);
	p0y = p[0]->HolePosition().GetKoordinaten(y);
	p1x = p[1]->HolePosition().GetKoordinaten(x);
	p1y = p[1]->HolePosition().GetKoordinaten(y);

	laenge = sqrt(pow(p0x - p1x, 2) + pow(p0y - p1y, 2));

	nx = (p0y - p1y) * maxAbstand / laenge;
	ny = (p1x - p0x) * maxAbstand / laenge;

	hpx[0] = p0x + nx;
	hpy[0] = p0y + ny;
	hpx[1] = p1x + nx;
	hpy[1] = p1y + ny;
	hpx[2] = p1x - nx;
	hpy[2] = p1y - ny;
	hpx[3] = p0x - nx;
	hpy[3] = p0y - ny;

	for(int i = 0; i < 4; i++)
	{
		lage[i] = (qx-hpx[i])*(hpy[(i+1)%4]-hpy[i]) - (qy-hpy[i])*(hpx[(i+1)%4]-hpx[i]); //Kreuzprodukt!
	}
	if((lage[0]<0)&&(lage[1]<0)&&(lage[2]<0)&&(lage[3]<0))return 1;
	if((lage[0]>0)&&(lage[1]>0)&&(lage[2]>0)&&(lage[3]>0))return 1;
	return 0;
}

bool Linie::IstAufLinie(const Vektor& ortQ, Achse prjRichtung)
{
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	double qx, qy, ax, ay, bx, by;
	qx = ortQ.GetKoordinaten(x);
	qy = ortQ.GetKoordinaten(y);
	ax = this->HolePunkt(0)->HolePosition().GetKoordinaten(x);
	ay = this->HolePunkt(0)->HolePosition().GetKoordinaten(y);
	bx = this->HolePunkt(1)->HolePosition().GetKoordinaten(x);
	by = this->HolePunkt(1)->HolePosition().GetKoordinaten(y);

	double abweichung = (qx - ax) * (by - ay) - (qy - ay) * (bx - ax);//z-Koordinate des Kreuzprodukts (Flaeche des Spats)

	if(abweichung == 0)
	{
		double richtungA = (qx - ax) * (bx - ax) + (qy - ay) * (by - ay);
		double richtungB = (qx - bx) * (ax - bx) + (qy - by) * (ay - by);
		if((richtungA >= 0)&&(richtungB >= 0))return true;
	}

	return false;
}

bool Linie::IstEndPunkt(Punkt *pkt)
{
	for(int i = 0; i < 2; i++)
	{
		if(*pkt == *p[i])return true;
	}
	return false;
}

Vektor Linie::PunktBeiLambda(double lambda)
{
	Vektor rueckgabe;
	rueckgabe = p[0]->HolePosition() + (p[1]->HolePosition() - p[0]->HolePosition()) * lambda;

	return rueckgabe;
}

int Linie::Extrudieren(double gefaelle, Vektor richtungsPunkt, Achse prjRichtung, bool gefaelleBehalten)
{
	double resGefaelle, bestGefaelle, normGefaelle;
	if(gefaelleBehalten)
	{
		resGefaelle = this->HoleGefaelle(prjRichtung);
	}
	else
	{
		resGefaelle = gefaelle;
	}
	int x, y, z;
	z = prjRichtung;
	x = (z+1)%3;
	y = (z+2)%3;

	/*Lot des Punktes auf die Linie suchen*/
	double p0x, p0y, p1x, p1y, ax, ay;
	p0x = p[0]->HolePosition().GetKoordinaten(x);
	p0y = p[0]->HolePosition().GetKoordinaten(y);
	p1x = p[1]->HolePosition().GetKoordinaten(x);
	p1y = p[1]->HolePosition().GetKoordinaten(y);
	ax = richtungsPunkt.GetKoordinaten(x);
	ay = richtungsPunkt.GetKoordinaten(y);

	double projLaengeQuadrat = pow(p1x-p0x, 2) + pow(p1y-p0y, 2);
	if(projLaengeQuadrat)
	{
		bestGefaelle = HoleGefaelle(prjRichtung);
		if(abs(bestGefaelle)>abs(resGefaelle))return 1;/*1 als Code fuer bestehendes Gefaelle groesse als resultierendes Gefaelle*/
	}else
	{
		return 0;
	}

	double lambda = ((p1x-p0x)*(ax-p0x) + (p1y-p0y)*(ay-p0y)) / projLaengeQuadrat;

	Vektor lotFussPunkt;
	lotFussPunkt.SetKoordinaten(x, (p0x+lambda*(p1x-p0x)));
	lotFussPunkt.SetKoordinaten(y, (p0y+lambda*(p1y-p0y)));
	lotFussPunkt.SetKoordinaten(z, 0.0);
	/*ENDE Lot des Punktes auf die Linie suchen*/

	normGefaelle = sqrt(pow(resGefaelle, 2) - pow(bestGefaelle, 2)) * ((resGefaelle < 0) ? -1.0 : 1.0);
	Vektor v_extrRichtung = richtungsPunkt - lotFussPunkt;
	double delta_z = v_extrRichtung.ProjLaenge(prjRichtung) * normGefaelle;
	v_extrRichtung.SetKoordinaten(z, delta_z);

	Punkt *p3, *p2;
	p3 = new Punkt((p[0]->HolePosition()+v_extrRichtung), HoleLayer());
	p2 = new Punkt((p[1]->HolePosition()+v_extrRichtung), HoleLayer());

	Linie *l1, *l2, *l3, *l4;
	l1 = new Linie(p[1], p2);
	l2 = new Linie(p2, p[0]);
	l3 = new Linie(p2, p3);
	l4 = new Linie(p3, p[0]);

	if(l1 && l2 && l3 && l4)
	{
		Dreieck *tempDrk;
		if((tempDrk = Dreieck::NeuesDreieck(this, l1, l2))) {
			if(Dreieck::NeuesDreieck(l2, l3, l4))
				return -1;
			else
				delete tempDrk;
		}
	}
	return 3;/*3 ist Code fuer Viereck wurde nicht erzeugt*/
}

int Linie::Extrudieren(double gefaelle, Vektor richtungsPunkt, double abstand, Achse prjRichtung, bool gefaelleBehalten)
{
	double resGefaelle, bestGefaelle, normGefaelle, extrAbstand;
	if(gefaelleBehalten)
	{
		resGefaelle = this->HoleGefaelle(prjRichtung);
	}
	else
	{
		resGefaelle = gefaelle;
	}
	extrAbstand = abstand;
	int x, y, z;
	z = prjRichtung;
	x = (z+1)%3;
	y = (z+2)%3;

	/*Lot des Punktes auf die Linie suchen*/
	double p0x, p0y, p1x, p1y, ax, ay;
	p0x = p[0]->HolePosition().GetKoordinaten(x);
	p0y = p[0]->HolePosition().GetKoordinaten(y);
	p1x = p[1]->HolePosition().GetKoordinaten(x);
	p1y = p[1]->HolePosition().GetKoordinaten(y);
	ax = richtungsPunkt.GetKoordinaten(x);
	ay = richtungsPunkt.GetKoordinaten(y);

	double projLaengeQuadrat = pow(p1x-p0x, 2) + pow(p1y-p0y, 2);
	if(projLaengeQuadrat)
	{
		bestGefaelle = HoleGefaelle(prjRichtung);
		if(abs(bestGefaelle)>abs(resGefaelle))return 1;/*1 als Code fuer bestehendes Gefaelle groesse als resultierendes Gefaelle*/
	}else
	{
		return 0;
	}

	double lambda = ((p1x-p0x)*(ax-p0x) + (p1y-p0y)*(ay-p0y)) / projLaengeQuadrat;

	Vektor lotFussPunkt;
	lotFussPunkt.SetKoordinaten(x, (p0x+lambda*(p1x-p0x)));
	lotFussPunkt.SetKoordinaten(y, (p0y+lambda*(p1y-p0y)));
	lotFussPunkt.SetKoordinaten(z, 0.0);
	/*ENDE Lot des Punktes auf die Linie suchen*/

	normGefaelle = sqrt(pow(resGefaelle, 2) - pow(bestGefaelle, 2)) * ((resGefaelle < 0) ? -1.0 : 1.0);
	Vektor v_extrRichtung = richtungsPunkt - lotFussPunkt;
	v_extrRichtung /= v_extrRichtung.ProjLaenge(prjRichtung);   //Richtungsvektor auf gewuenschte Laenge skalieren
	v_extrRichtung *= extrAbstand;							  //Richtungsvektor auf gewuenschte Laenge skalieren
	double delta_z = v_extrRichtung.ProjLaenge(prjRichtung) * normGefaelle;
	v_extrRichtung.SetKoordinaten(z, delta_z);

	Punkt *p3, *p2;
	p3 = new Punkt((p[0]->HolePosition()+v_extrRichtung), HoleLayer());
	p2 = new Punkt((p[1]->HolePosition()+v_extrRichtung), HoleLayer());

	Linie *l1, *l2, *l3, *l4;
	l1 = new Linie(p[1], p2);
	l2 = new Linie(p2, p[0]);
	l3 = new Linie(p2, p3);
	l4 = new Linie(p3, p[0]);

	if(l1 && l2 && l3 && l4)
	{
		Dreieck *tempDrk;
		if((tempDrk = Dreieck::NeuesDreieck(this, l1, l2))) {
			if(Dreieck::NeuesDreieck(l2, l3, l4))
				return -1;
			else
				delete tempDrk;
		}
	}
	return 3;/*3 ist Code fuer Viereck wurde nicht erzeugt*/
}

Linie* Linie::Parallele(Vektor richtungsPunkt, Achse prjRichtung)
{
	int x, y, z;
	z = prjRichtung;
	x = (z+1)%3;
	y = (z+2)%3;

	/*Lot des Punktes auf die Linie suchen*/
	double p0x, p0y, p1x, p1y, ax, ay;
	p0x = p[0]->HolePosition().GetKoordinaten(x);
	p0y = p[0]->HolePosition().GetKoordinaten(y);
	p1x = p[1]->HolePosition().GetKoordinaten(x);
	p1y = p[1]->HolePosition().GetKoordinaten(y);
	ax = richtungsPunkt.GetKoordinaten(x);
	ay = richtungsPunkt.GetKoordinaten(y);

	double projLaengeQuadrat = pow(p1x-p0x, 2) + pow(p1y-p0y, 2);
	double lambda = ((p1x-p0x)*(ax-p0x) + (p1y-p0y)*(ay-p0y)) / projLaengeQuadrat;

	Vektor lotFussPunkt = Vektor((p0x+lambda*(p1x-p0x)), (p0y+lambda*(p1y-p0y)), 0.0);
	/*ENDE Lot des Punktes auf die Linie suchen*/

	Vektor vRichtung = richtungsPunkt - lotFussPunkt;

	Punkt *p3, *p2;
	p3 = new Punkt((p[0]->HolePosition()+vRichtung), HoleLayer());
	p2 = new Punkt((p[1]->HolePosition()+vRichtung), HoleLayer());

	Linie *ln;
	ln = new Linie(p2, p3);

	if(ln)return ln;
	return NULL;
}

Linie* Linie::Parallele(Vektor richtungsPunkt, double abstand, Achse prjRichtung)
{
	double extrAbstand = abstand;
	int x, y, z;
	z = prjRichtung;
	x = (z+1)%3;
	y = (z+2)%3;

	/*Lot des Punktes auf die Linie suchen*/
	double p0x, p0y, p1x, p1y, ax, ay;
	p0x = p[0]->HolePosition().GetKoordinaten(x);
	p0y = p[0]->HolePosition().GetKoordinaten(y);
	p1x = p[1]->HolePosition().GetKoordinaten(x);
	p1y = p[1]->HolePosition().GetKoordinaten(y);
	ax = richtungsPunkt.GetKoordinaten(x);
	ay = richtungsPunkt.GetKoordinaten(y);

	double projLaengeQuadrat = pow(p1x-p0x, 2) + pow(p1y-p0y, 2);
	double lambda = ((p1x-p0x)*(ax-p0x) + (p1y-p0y)*(ay-p0y)) / projLaengeQuadrat;

	Vektor lotFussPunkt = Vektor((p0x+lambda*(p1x-p0x)), (p0y+lambda*(p1y-p0y)), 0.0);
	/*ENDE Lot des Punktes auf die Linie suchen*/

	Vektor vRichtung = richtungsPunkt - lotFussPunkt;
	vRichtung /= vRichtung.ProjLaenge(prjRichtung);   //Richtungsvektor auf gewuenschte Laenge skalieren
	vRichtung *= extrAbstand;							  //Richtungsvektor auf gewuenschte Laenge skalieren

	Punkt *p3, *p2;
	p3 = new Punkt((p[0]->HolePosition()+vRichtung), HoleLayer());
	p2 = new Punkt((p[1]->HolePosition()+vRichtung), HoleLayer());

	Linie *ln;
	ln = new Linie(p2, p3);

	if(ln)return ln;
	return NULL;
}
/*ENDE Linie*/

/*Flaeche*/
Flaeche::Flaeche()
{
	adjHL = new Liste<RUZ_Hoehenlinie>;
	adjHoehenMarken = new Liste<HoehenMarke>;
	adjGefaelleMarken = new Liste<GefaelleMarke>;
	m_hoehenSchritt = 0.0;
	m_startHoehe = 0.0;
	m_suchRadius = 1.0;
	m_projektionsRichtung = z;
	farbe[0] = farbe[1] = farbe[2] = -1;
}

Flaeche::~Flaeche()
{
	for(HoehenMarke* t_HM = adjHoehenMarken->GetErstesElement(); t_HM != NULL; t_HM = adjHoehenMarken->GetNaechstesElement())
	{
		t_HM->FlaecheNullen();
	}
	delete adjHoehenMarken;

	for(GefaelleMarke* t_GM = adjGefaelleMarken->GetErstesElement(); t_GM != NULL; t_GM = adjGefaelleMarken->GetNaechstesElement())
	{
		t_GM->FlaecheNullen();
	}
	delete adjGefaelleMarken;

	adjHL->ListeLoeschen("");
	delete adjHL;
}

Liste<RUZ_Hoehenlinie>* Flaeche::HoleHL(void)
{
	return adjHL;
}

Vektor Flaeche::HoleNormale(void)
{
	return m_normale;
}

bool Flaeche::HoleFarbe(unsigned char &r, unsigned char &g, unsigned char &b)
{
	for(int i = 0; i < 3; i++)
	{
		if(farbe[i] < 0)return false;
	}
	r = farbe[0];
	g = farbe[1];
	b = farbe[2];
	return true;
}

void Flaeche::SetzeFarbe(unsigned char r, unsigned char g, unsigned char b)
{
	farbe[0] = r;
	farbe[1] = g;
	farbe[2] = b;
	return;
}

void Flaeche::LoescheFarbe(void)
{
	farbe[0] = farbe[1] = farbe[2] = -1;
	return;
}

void Flaeche::SetzeHoehenlinie(double x_a, double y_a, double x_e, double y_e, double h)
{
	RUZ_Hoehenlinie* t_HL = new RUZ_Hoehenlinie(x_a, y_a, x_e, y_e, h);
	if(t_HL != NULL)adjHL->Hinzufuegen(t_HL);
	return;
}

void Flaeche::HoehenlinienFinden()
{
	adjHL->ListeLoeschen("Flaeche::HoehenlinienFinden");
	RUZ_Layer *derLayer = HoleLayer();
	/*Parameter setzen*/
	m_hoehenSchritt = derLayer->HoleHLhoehenSchritt();
	if(m_hoehenSchritt == 0)
	{
		return;
	}
	m_suchRadius = derLayer->HoleHLsuchRadius();
	m_startHoehe = derLayer->HoleHLstartHoehe();
	m_projektionsRichtung = derLayer->HoleProjektion();
	/*ENDE Parameter setzen*/

	DoHoehenlinienFinden();
	return;
}

void Flaeche::LoescheDoppeltePunkte(Liste<Hoehenpunkt>* pktLst)
{
	Listenelement<Hoehenpunkt> *pkt_LE_laeufer, *pkt_LE_naechster;
	Hoehenpunkt *pkt_laeufer, *pkt_naechster;
	double ax, ay, az, bx, by, bz;

	for(pkt_LE_laeufer = pktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL; pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger())
	{
		pkt_LE_laeufer->Wert(pkt_LE_laeufer->GetElement()->z);
	}
	pktLst->ListeNachWertSortieren();

	for(pkt_LE_laeufer = pktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL;)
	{
		pkt_laeufer = pkt_LE_laeufer->GetElement();
		ax = pkt_laeufer->x;
		ay = pkt_laeufer->y;
		az = pkt_laeufer->z;

		pkt_LE_naechster = pkt_LE_laeufer->GetNachfolger();
		if(pkt_LE_naechster != NULL)
		{
			pkt_naechster = pkt_LE_naechster->GetElement();
			bx = pkt_naechster->x;
			by = pkt_naechster->y;
			bz = pkt_naechster->z;
			/*Wegen Rundungsungenauigkeiten schlägt die Gleichheitsabfrage gelegentlich
			fehl, deshalb wird im Folgenden die Beschränktheit der Differenzen überprüft*/
			if(abs(az - bz)<1e-16)
			{
				if(abs(ay - by)<1e-16)
				{
					if(abs(ax - bx)<1e-16)
					{
						pktLst->Entfernen(pkt_LE_naechster);
						delete pkt_naechster;
					}else
					{
						pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();
					}
				}else
				{
					pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();
				}
			}else
			{
				pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();
			}
		}else
		{
			pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();
		}
	}
	return;
}

void Flaeche::Hinzufuegen(HoehenMarke* hm)
{
	adjHoehenMarken->ExklusivHinzufuegen(hm);
	return;
}

void Flaeche::Entfernen(HoehenMarke* hm)
{
	adjHoehenMarken->Entfernen(hm);
	return;
}

void Flaeche::Hinzufuegen(GefaelleMarke* gm)
{
	adjGefaelleMarken->ExklusivHinzufuegen(gm);
	return;
}

void Flaeche::Entfernen(GefaelleMarke* gm)
{
	adjGefaelleMarken->Entfernen(gm);
	return;
}

void Flaeche::HoehenMarkenAktualisieren(void)
{
	for(HoehenMarke* aktHM = adjHoehenMarken->GetErstesElement(); aktHM != NULL; aktHM = adjHoehenMarken->GetNaechstesElement())
	{
		aktHM->Verschieben(Vektor(0, 0, 0));
	}
	return;
}

bool Flaeche::TeiltLinieMit(Flaeche* vergleichsFlaeche)
{
	int seitenZahl1, seitenZahl2;
	this->HoleTyp() == RUZ_Dreieck ? seitenZahl1 = 3 : seitenZahl1 = 4;
	vergleichsFlaeche->HoleTyp() == RUZ_Dreieck ? seitenZahl2 = 3 : seitenZahl2 = 4;
	for(int i = 0; i < seitenZahl1; i++)
	{
		for(int k = 0; k < seitenZahl2; k++)
		{
			if(HoleLinie(i) == vergleichsFlaeche->HoleLinie(k))return true;
		}
	}
	return false;
}
/*ENDE Flaeche*/

/*Dreieck*/
Dreieck::Dreieck():Flaeche()
{
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	l[0] = 0;
	l[1] = 0;
	l[2] = 0;
}

Dreieck::Dreieck(Punkt** p1, Punkt** p2, Punkt** p3, Linie* l1, Linie* l2, Linie* l3):Flaeche()
{
	p[0] = *p1;
	p[1] = *p2;
	p[2] = *p3;
	l[0] = l1;
	l[1] = l2;
	l[2] = l3;

	l[0]->Hinzufuegen(this);
	l[1]->Hinzufuegen(this);
	l[2]->Hinzufuegen(this);

	HoleLayer()->Hinzufuegen(this);

	NormaleBerechnen();
	MinMax();
	SetzeBesucht('j');
}

Dreieck* Dreieck::NeuesDreieck(Linie* l1, Linie* l2, Linie* l3)
{
	Punkt *p1, *p2, *p3;

	/*Aufgeschlossenheit testen*/
	bool geschlossen = LinienzugGeschlossen(&p1, &p2, &p3, l1, l2, l3);

	if(geschlossen)
	{
		return (new Dreieck(&p1, &p2, &p3, l1, l2, l3));
	}
	return 0;
}

Dreieck::Dreieck(const Dreieck& dE)
{
	if(HoleLayer())HoleLayer()->Entfernen(this);
	p[0] = dE.p[0];
	p[1] = dE.p[1];
	p[2] = dE.p[2];
	l[0] = dE.l[0];
	l[1] = dE.l[1];
	l[2] = dE.l[2];

	l[0]->Hinzufuegen(this);
	l[1]->Hinzufuegen(this);
	l[2]->Hinzufuegen(this);

	*this = dE;
	HoleLayer()->Hinzufuegen(this);
	NormaleBerechnen();
	MinMax();
	SetzeBesucht('j');
}

Dreieck& Dreieck::operator=(const Dreieck& dE)
{
	if(this != &dE)
	{
		if(HoleLayer())HoleLayer()->Entfernen(this);
		p[0] = dE.p[0];
		p[1] = dE.p[1];
		p[2] = dE.p[2];
		l[0] = dE.l[0];
		l[1] = dE.l[1];
		l[2] = dE.l[2];

		l[0]->Hinzufuegen(this);
		l[1]->Hinzufuegen(this);
		l[2]->Hinzufuegen(this);
	}
	HoleLayer()->Hinzufuegen(this);
	NormaleBerechnen();
	MinMax();
	return *this;
}

Dreieck::~Dreieck()
{
	l[0]->Entfernen(this);
	l[1]->Entfernen(this);
	l[2]->Entfernen(this);

	if(HoleLayer())
	{
		HoleLayer()->Entfernen(this);
	}
}

void Dreieck::MinMax(void)
{
	for(int i = 0; i < 3; i++)
	{
		if(p[0]->HolePosition().GetKoordinaten(i) < p[1]->HolePosition().GetKoordinaten(i))
		{
			max[i] = p[1]->HolePosition().GetKoordinaten(i);
			min[i] = p[0]->HolePosition().GetKoordinaten(i);
		}else
		{
			max[i] = p[1]->HolePosition().GetKoordinaten(i);
			min[i] = p[0]->HolePosition().GetKoordinaten(i);
		}
		
		if(max[i] < p[2]->HolePosition().GetKoordinaten(i))max[i] = p[2]->HolePosition().GetKoordinaten(i);
		if(min[i] > p[2]->HolePosition().GetKoordinaten(i))min[i] = p[2]->HolePosition().GetKoordinaten(i);
	}
	return;
}

Linie* Dreieck::HoleLinie(int index) const
{
	switch (index)
	{
		case 0:
			return l[0];
		case 1:
			return l[1];
	}
	return l[2];
}

Punkt* Dreieck::HolePunkt(int index) const
{
	switch (index)
	{
		case 0:
			return p[0];
		case 1:
			return p[1];
	}
	return p[2];
}

RUZ_Layer* Dreieck::HoleLayer(void) const
{
	if(p[0])return (p[0]->HoleLayer());
	return NULL;
}

double Dreieck::FlaechenInhalt(Achse prjRichtung)
{
	int x, y;
	x = (prjRichtung + 1) % 3;
	y = (prjRichtung + 2) % 3;

	Vektor a, b, erg;
	a = p[1]->HolePosition() - p[0]->HolePosition();
	b = p[2]->HolePosition() - p[0]->HolePosition();

	double dErg = abs(a.GetKoordinaten(x) * b.GetKoordinaten(y) - a.GetKoordinaten(y) * b.GetKoordinaten(x)) / 2;
	return dErg;
}

double Dreieck::Volumen(Achse projektion)
{
	double mittlereHoehe = (p[0]->HolePosition().GetKoordinaten(projektion)
							+ p[1]->HolePosition().GetKoordinaten(projektion)
							+ p[2]->HolePosition().GetKoordinaten(projektion))/3;
	double volumen = FlaechenInhalt(projektion) * mittlereHoehe;
	return volumen;
}

double Dreieck::Seitenverhaeltnis(void)
{
	double a = l[0]->Laenge();
	double b = l[1]->Laenge();
	double c = l[2]->Laenge();

	double aR = (-a+b+c)*(a-b+c)*(a+b-c);
	if(aR != 0)
	{
		aR = a*b*c / aR;
	}else{
		aR = -1.0;
	}
	return aR;
}

RUZ_Objekt* Dreieck::IstInnerhalb(double qx, double qy, Achse prjRichtung)
{
	double kPr[3];
	double p1x, p1y, p2x, p2y;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	for(int i = 0; i < 3; i++)
	{
		p1x = p[(0+i)%3]->HolePosition().GetKoordinaten(x);
		p1y = p[(0+i)%3]->HolePosition().GetKoordinaten(y);
		p2x = p[(1+i)%3]->HolePosition().GetKoordinaten(x);
		p2y = p[(1+i)%3]->HolePosition().GetKoordinaten(y);

		kPr[i] =  (p2x-p1x)*(qy-p1y) - (p2y-p1y)*(qx-p1x);
		if(abs(kPr[i]) < 1e-5)kPr[i] = 0;/*Rundungsprobleme eindämmen*/
	}
	if(((kPr[0]<=0)&&(kPr[1]<=0)&&(kPr[2]<=0))||((kPr[0]>=0)&&(kPr[1]>=0)&&(kPr[2]>=0)))
	{
		if(kPr[0] == 0)return l[0];
		if(kPr[1] == 0)return l[1];
		if(kPr[2] == 0)return l[2];
		return this;
	}
	return NULL;
}

RUZ_Objekt* Dreieck::IstInnerhalb(Punkt* pkt)
{
  Vektor vPkt = pkt->HolePosition();
  return(IstInnerhalb(pkt));
}

RUZ_Objekt* Dreieck::IstInnerhalb(Vektor pkt)
{
  Vektor vPkt = pkt;
  for(int i = 0; i < 3; i++)
  {
	if(p[i]->HolePosition() == vPkt)return this;
  }
  /*Seiten als Vektoren und Ecken zu Punkt -> Kreuzprodukt: Vergleich mit Normalen*/
  Vektor v_ap = p[0]->HolePosition() - vPkt;
  Vektor v_bp = p[1]->HolePosition() - vPkt;
  Vektor v_cp = p[2]->HolePosition() - vPkt;
  Vektor v_ab = p[0]->HolePosition() - p[1]->HolePosition();
  Vektor v_bc = p[1]->HolePosition() - p[2]->HolePosition();
  Vektor v_ca = p[2]->HolePosition() - p[0]->HolePosition();

  double dRa = m_normale * (v_ap.Kreuz(v_ab));
  double dRb = m_normale * (v_ap.Kreuz(v_ab));
  double dRc = m_normale * (v_ap.Kreuz(v_ab));

  if((dRa < 0) && (dRb < 0) && (dRc < 0))return this;
  if((dRa > 0) && (dRb > 0) && (dRc > 0))return this;

  return NULL;
}

bool Dreieck::Gefaelle(Vektor& punktQ, Vektor& v_gefaelle, Achse prjRichtung)
{
	int x, y;
	x = (prjRichtung + 1) % 3;
	y = (prjRichtung + 2) % 3;

	if(!(IstInnerhalb(punktQ.GetKoordinaten(x), punktQ.GetKoordinaten(y), prjRichtung)))return false;

	Vektor v_Richtung1, v_Richtung2;

	v_Richtung1 = p[2]->HolePosition() - p[0]->HolePosition();
	v_Richtung2 = p[1]->HolePosition() - p[0]->HolePosition();

	v_gefaelle = v_Richtung1.Kreuz(v_Richtung2);

	if(v_gefaelle.GetKoordinaten(prjRichtung) == 0)return false;
	v_gefaelle /= v_gefaelle.ProjLaenge(prjRichtung);

	double dTemp = -1 / v_gefaelle.GetKoordinaten(prjRichtung);
	v_gefaelle.SetKoordinaten(prjRichtung, dTemp);

	if(v_gefaelle.GetKoordinaten(prjRichtung)<0)v_gefaelle *= -1;
	return true;
}

bool Dreieck::OrtAufFlaeche(Vektor& punktQ, Achse prjRichtung, bool innenPruefung)
{
	int x, y, z;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;
	z = (prjRichtung+3)%3;

	if(innenPruefung)
	{
		if(!(IstInnerhalb(punktQ.GetKoordinaten(x), punktQ.GetKoordinaten(y), prjRichtung)))return false;
	}

	double qx, qy, ax, ay, bx, by, cx, cy;
	qx = punktQ.GetKoordinaten(x);
	qy = punktQ.GetKoordinaten(y);
	ax = p[0]->HolePosition().GetKoordinaten(x);
	ay = p[0]->HolePosition().GetKoordinaten(y);
	bx = p[1]->HolePosition().GetKoordinaten(x);
	by = p[1]->HolePosition().GetKoordinaten(y);
	cx = p[2]->HolePosition().GetKoordinaten(x);
	cy = p[2]->HolePosition().GetKoordinaten(y);

	double det_haupt = (bx-ax)*(cy-ay) - (cx-ax)*(by-ay);
	if(IstGleich(det_haupt, 0.0))return false;

	double det_lambda = (qx-ax)*(cy-ay) - (cx-ax)*(qy-ay);
	double det_mue = (bx-ax)*(qy-ay) - (qx-ax)*(by-ay);

	double qz = p[0]->HolePosition().GetKoordinaten(z)
				+ ( det_lambda * (p[1]->HolePosition().GetKoordinaten(z) - p[0]->HolePosition().GetKoordinaten(z))
				+ det_mue * (p[2]->HolePosition().GetKoordinaten(z) - p[0]->HolePosition().GetKoordinaten(z)) ) / det_haupt;

	punktQ.SetKoordinaten(z, qz);

	return true;
}

bool Dreieck::OrdinateAufEbene(Punkt* punktQ, Achse prjRichtung)
{
	int x, y, z;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;
	z = (prjRichtung+3)%3;
	if(m_normale.GetKoordinaten(z) == 0)return 0;

	double t_d = m_normale * p[0]->HolePosition();
	Vektor t_vkt = punktQ->HolePosition();

	double _z = (t_d - (m_normale.GetKoordinaten(x) * t_vkt.GetKoordinaten(x) + m_normale.GetKoordinaten(y) * t_vkt.GetKoordinaten(y)))/m_normale.GetKoordinaten(z);
	if(_z < min[z])_z = min[z];
	if(_z > max[z])_z = max[z];
	
	t_vkt.SetKoordinaten(z, _z);

	punktQ->Positionieren(t_vkt);
	return 1;
}

double Dreieck::OrdinateAufEbene(double dX, double dY, Achse prjRichtung)
{
	int x, y, z;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;
	z = (prjRichtung+3)%3;
	if(m_normale.GetKoordinaten(z) == 0)return NAN;

	double t_d = m_normale * p[0]->HolePosition();
	
	double _z = (t_d - (m_normale.GetKoordinaten(x) * dX + m_normale.GetKoordinaten(y) * dY))/m_normale.GetKoordinaten(z);
	if(_z < min[z])_z = min[z];
	if(_z > max[z])_z = max[z];

	return (_z);
}

void Dreieck::Teilen(Punkt* punktQ, Achse prjRichtung, bool ungefaehr)
{
	if(punktQ->HoleLayer() != this->HoleLayer())return;

	Vektor ortQ;
	ortQ = punktQ->HolePosition();

	if(punktQ->HolePosition() == p[0]->HolePosition())
	{
		return;
	}
	if(punktQ->HolePosition() == p[1]->HolePosition())
	{
		return;
	}
	if(punktQ->HolePosition() == p[2]->HolePosition())
	{
		return;
	}

	if(!(this->OrtAufFlaeche(ortQ, prjRichtung)))
	{
		return;
	}
	if(ungefaehr)
	{
		punktQ->Positionieren(ortQ);
	}

	for(int i = 0; i < 3; i++)
	{
		if(l[i]->IstAufLinie(ortQ, prjRichtung))
		{
			l[i]->Teilen(punktQ, prjRichtung, true);
			return;
		}
	}

	Linie* n_ln[3];
	for(int i = 0; i < 3; i++)
	{
		n_ln[i] = Linie::NeueLinie(punktQ, p[i]);
		if(!n_ln[i])
		{
			return;
		}
		n_ln[i]->SetzeBesucht('j');
	}
	for(int i = 0; i < 3; i++)
	{
		Dreieck::NeuesDreieck(n_ln[i], l[i], n_ln[(i+1)%3]);
	}
	delete this;
	return;
}

void Dreieck::ZwangsTeilung(Punkt* punktQ)
{
	if(punktQ->HoleLayer() != this->HoleLayer())return;

	Vektor ortQ;
	ortQ = punktQ->HolePosition();

	for(int i = 0; i < 3; i++)
	{
		if(l[i]->IstAufLinie(ortQ, z))
		{
			l[i]->ZwangsTeilung(punktQ);
			return;
		}
	}

	Linie* n_ln[3];
	for(int i = 0; i < 3; i++)
	{
		n_ln[i] = Linie::NeueLinie(punktQ, p[i]);
		if(!n_ln[i])
		{
			return;
		}
	}
	for(int i = 0; i < 3; i++)
	{
		Dreieck::NeuesDreieck(n_ln[i], l[i], n_ln[(i+1)%3]);
	}
	delete this;
	return;
}

void Dreieck::Verschieben(const Vektor& vkt)
{
	for(int i = 0; i < 3; i++)
	{
		p[i]->Verschieben(vkt);
	}
	return;
}

RUZ_Objekt* Dreieck::Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = this->HoleLayer();
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	RUZ_Objekt *oObj;
	Linie *lnNeu[3];
	for(int i = 0; i < 3; i++)
	{
		lnNeu[i] = NULL;
		oObj = l[i]->Kopieren(lObjQuelle, lBereitsKopiert, layZielLayer);
		if(oObj)
		{
			if(oObj->HoleTyp() == RUZ_Linie)
			{
				lnNeu[i] = static_cast<Linie*>(oObj);
			}
		}
	}
	if(lnNeu[0] && lnNeu[1] && lnNeu[2])
	{
		Dreieck* drNeu = Dreieck::NeuesDreieck(lnNeu[0], lnNeu[1], lnNeu[2]);
		if(drNeu)
		{
			lObjQuelle->Hinzufuegen(drNeu);
			lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, drNeu));
			return drNeu;
		}
	}
	return NULL;
}

void Dreieck::DoHoehenlinienFinden(void)
{
	adjHL->ListeLoeschen("Dreieck::HoehenlinienFinden");
	if(m_hoehenSchritt == 0)return;

	if(p[0]->HolePosition().z() == p[1]->HolePosition().z())
		if(p[0]->HolePosition().z() == p[2]->HolePosition().z())return;/*Abbruch bei waagerechten Flaechen*/

	Linie *aktLinie;
	Liste<Hoehenpunkt>* hoehenPktLst = new Liste<Hoehenpunkt>();
	for(int i = 0; i < 3; i++)
	{
		aktLinie = HoleLinie(i);
		aktLinie->HoehenPunkteFinden(hoehenPktLst, m_hoehenSchritt, m_suchRadius, m_startHoehe);
	}
	LoescheDoppeltePunkte(hoehenPktLst);/*sortiert nach Höhe und löscht doppelte Punkte*/

	Listenelement<Hoehenpunkt>* pkt_LE_naechster;
	double ax, ay, az, bx, by;
	for(Listenelement<Hoehenpunkt>* pkt_LE_laeufer = hoehenPktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL; pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger())
	{
		pkt_LE_naechster = pkt_LE_laeufer->GetNachfolger();
		if(pkt_LE_naechster == NULL)break;
		if(abs(pkt_LE_laeufer->GetElement()->z - pkt_LE_naechster->GetElement()->z)<1e-12)
		{
			ax = pkt_LE_laeufer->GetElement()->x;
			ay = pkt_LE_laeufer->GetElement()->y;
			az = pkt_LE_laeufer->GetElement()->z;
			bx = pkt_LE_naechster->GetElement()->x;
			by = pkt_LE_naechster->GetElement()->y;
			SetzeHoehenlinie(ax, ay, bx, by, az);
		}
	}
	delete hoehenPktLst;
	return;
}

void Dreieck::NormaleBerechnen(void)
{
	Vektor tempVek = (p[1]->HolePosition() - p[0]->HolePosition());
	m_normale = tempVek.Kreuz(p[2]->HolePosition() - p[0]->HolePosition());
	if(m_normale.z() < 0)m_normale *= (-1);
	m_normale /= m_normale.Laenge();
	return;
}

Vektor Dreieck::Schwerpunkt(void)
{
	Vektor rueckgabe = (p[0]->HolePosition() + p[1]->HolePosition() + p[2]->HolePosition());
	rueckgabe /= 3;
	return rueckgabe;
}

bool Dreieck::DurchstossPunkt(Linie* durchstossLn, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt)
{
	Vektor v_dr_a = p[0]->HolePosition();
	Vektor v_ln_a = durchstossLn->HolePunkt(0)->HolePosition();
	Vektor v_ln_b = durchstossLn->HolePunkt(1)->HolePosition();

	double lambda, d_dr, d_ln, richtung_ln;

	d_dr = m_normale * v_dr_a;
	d_ln = m_normale * v_ln_a;
	v_ln_b = (v_ln_b - v_ln_a);
	richtung_ln = v_ln_b * m_normale;

	if(richtung_ln != 0)
	{
		lambda = (d_dr - d_ln) / richtung_ln;
		if(aussenErlaubt)
		{
			Vektor t_vkt = durchstossLn->PunktBeiLambda(lambda);
			v_rueckgabe1.SetKoordinaten(t_vkt.x(), t_vkt.y(), t_vkt.z());
			return true;
		}else
		if((lambda <= 1) && (lambda >= 0))
		{
			Vektor t_vkt = durchstossLn->PunktBeiLambda(lambda);
			if(IstInnerhalb(t_vkt.x(), t_vkt.y(), z)/*OrtAufFlaeche(t_vkt, z)*/)
			{
				v_rueckgabe1.SetKoordinaten(t_vkt.x(), t_vkt.y(), t_vkt.z());
				return true;
			}
		}
		return false;
	}
	return false;
}

bool Dreieck::IstEckPunkt(Punkt* pkt)
{
	for(int i = 0; i < 3; i++)
	{
		if(*pkt == *p[i])return true;
	}
	return false;
}

Vektor Dreieck::MitteGegenueber(Punkt* pkt)
{
	 for(int i = 0; i < 3; i++)
	{
		if(*pkt == *p[i])
		{
			Vektor t_vkt = p[(i+1)%3]->HolePosition() + p[(i+2)%3]->HolePosition();
			t_vkt /= 2;
			return t_vkt;
		}
	}
	return Vektor(0, 0, 0);
}

void Dreieck::EckenNeuErmitteln(void)
{
	Punkt *t_p[3];

	/*Auf Geschlossenheit testen*/
	bool geschlossen = LinienzugGeschlossen(&t_p[0], &t_p[1], &t_p[2], l[0], l[1], l[2]);
	if(geschlossen)
	{
		for(int i = 0; i < 3; i++)
		{
			p[i] = t_p[i];
		}
		for(int i = 0; i < 2; i++)
		{
			if((p[i] == p[(i+1)%3])||(p[i] == p[(i+2)%3]))
			{
				delete this;
				return;
			}
		}
		HoleLayer()->GeaenderteFlaecheHinzufuegen(this);
	}
	else
	{
		delete this;
	}
	return;
}

void Dreieck::ErsetzeLinie(Linie* originalLn, Linie* ersatzLn)
{
	for(int i = 0; i < 3; i++)
	{
		if(l[i] == originalLn)
		{
			l[i] = ersatzLn;
			originalLn->Entfernen(this);
			ersatzLn->Hinzufuegen(this);
		}
	}
	EckenNeuErmitteln();
	return;
}

void Dreieck::ListeAusgeben(Liste<Hoehenpunkt>* hPktLst)
{
	logSchreiben("\nHoehenpunktliste\n");
	for(Listenelement<Hoehenpunkt>* pkt_LE_laeufer = hPktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL; pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger())
	{
		Hoehenpunkt* hPkt =  pkt_LE_laeufer->GetElement();
		logSchreiben("x, y, z:\t%.17f\t%.17f\t%.17f\n", hPkt->x, hPkt->y, hPkt->z);
	}
	logSchreiben("ENDE Hoehenpunktliste\n");
	return;
}

bool Dreieck::operator==(const Flaeche& vergleichsFlaeche)
{
	if(vergleichsFlaeche.HoleTyp() != RUZ_Dreieck)return false;
	int uebereinstimmungen = 0;
	for(int i = 0; i < 3; i++)
	{
		for(int k = 0; k < 3; k++)
		{
			if(p[i] == vergleichsFlaeche.HolePunkt(k))
			{
				uebereinstimmungen++;
				break;
			}
		}
	}
	if(uebereinstimmungen == 3)return true;
	return false;
}

void Dreieck::ImLogAusgeben(void)
{
	logSchreiben("%p:\n", this);
	for(int i = 0; i < 3; i++)
	{
		logSchreiben("\t%.3f / %.3f / %.3f\n", p[i]->HolePosition().x(), p[i]->HolePosition().y(), p[i]->HolePosition().z());
	}
	logSchreiben("\n");
}
/*ENDE Dreieck*/

/*Funktionen ohne Mitgliedschaft*/
bool LinienzugGeschlossen(Punkt **eck1, Punkt **eck2, Punkt **eck3, Linie* l1, Linie* l2, Linie* l3)
{
	/*testet, ob der Linienzug geschlossen ist und ordnet die Punkte so an, dass Linie 1 p1 mit p2, Linie 2 p2 mit p3 und Linie 3 p3 mit p1 verbindet*/
	*eck1 = l3->HolePunkt(0);
	*eck2 = 0;
	*eck3 = l3->HolePunkt(1);

	while(1)
	{
		if(*eck1 == l1->HolePunkt(0))
		{
			if(*eck3 != l1->HolePunkt(1))
			{
				*eck2 = l1->HolePunkt(1);
				break;
			}
		}else if(*eck1 == l1->HolePunkt(1))
		{
			if(*eck3 != l1->HolePunkt(0))
			{
				*eck2 = l1->HolePunkt(0);
				break;
			}
		}

		if(*eck1 == l3->HolePunkt(0))
		{
			*eck1 = l3->HolePunkt(1);
			*eck3 = l3->HolePunkt(0);
		}else{
			return 0;
		}
	}

	if((*eck2 == l2->HolePunkt(0))&&(*eck3 == l2->HolePunkt(1))) return 1;
	if((*eck2 == l2->HolePunkt(1))&&(*eck3 == l2->HolePunkt(0))) return 1;
	return 0;
}

void LinienExtrudieren(LinienFlaeche lnFl[], int gr, double reGef, double h0, Achse zet, Vektor &riPu)
{
	/* lnFl:	Liste der zu extrudierenden Linien	*/
	/* gr:		Groeese der LinienFlaechen-Liste	*/
	/* reGef:	gewünschtes resultierendes Gefaelle	*/
	/* h0:		Hoehe der neuen Eckpunkte			*/
	/* z:		Projektionsachse					*/
	/* riPu_	Richtungspunkt - Extrusionsrichtung	*/
	
	int x, y, z;
	z = zet % 3;
	x = (z + 1) % 3;
	y = (z + 2) % 3;
	
	for (int i = 0; i < gr; i++) { /* Normale zu allen Linienflaechen suchen */
		if (!LinienNormale(lnFl[i], riPu, reGef, (Achse)z))		
			lnFl[i].n = Vektor(NAN, NAN, NAN);
	}

	double n0x, n0y, n0z;
	double n1x, n1y, n1z;
	double qx, qy;
	Vektor p0, schnittOrt;
	double div;

	for (int i = 0; i < gr; i++) {
		if (std::isnan(n0x = lnFl[i].n.GetKoordinaten(x))) {
			continue; /* naechste Linie, weil keine (sinnvolle) Normal vorhanden */
		} 
		n0y = lnFl[i].n.GetKoordinaten(y);
		n0z = lnFl[i].n.GetKoordinaten(z);
		for (int pktNr = 0; pktNr < 2; pktNr++) {
			Punkt *schnittPkt;
			/* Nachbarsuchen */
			for (int k = i + 1; k < gr; k++) {
				if (std::isnan(n1x = lnFl[k].n.GetKoordinaten(x))) {
					continue; /* naechste Linie, weil keine (sinnvolle) Normal vorhanden */
				}
				n1y = lnFl[k].n.GetKoordinaten(y);
				n1z = lnFl[k].n.GetKoordinaten(z);
				for (int pkt2Nr = 0; pkt2Nr < 2; pkt2Nr++) {
					if ((lnFl[k].ln->HolePunkt(pkt2Nr) == lnFl[i].ln->HolePunkt(pktNr)) && (lnFl[k].p_neu[pkt2Nr] != NULL)) {
						/* Schnittpunkt auf Hoehe h0 finden	*/
						p0 = lnFl[i].ln->HolePunkt(pktNr)->HolePosition();

						div = (n0x * n1y - n1x * n0y);
						if (div  == 0) {
							/* Normalen colinear => Schnittpunkt rechtwinklig zur Linie */
							std::cout << "Kolineare Normalen => Schnittpunkt rechtwinklig zur Linie\n";
							schnittOrt = p0 + lnFl[i].extR;
						} else {
							qx = (n1y * (p0 * lnFl[i].n - h0 * n0z) - n0y * (p0 * lnFl[k].n - h0 * n1z)) / div;
							qy = (n0x * (p0 * lnFl[i].n - h0 * n1z) - n1x * (p0 * lnFl[k].n - h0 * n0z)) / div;
							
							schnittOrt.SetKoordinaten(x, qx);
							schnittOrt.SetKoordinaten(y, qy);
							schnittOrt.SetKoordinaten(z, h0);
						}
						schnittPkt = new Punkt(schnittOrt, lnFl[i].ln->HoleLayer());
						if (!schnittPkt) {
							std::cerr << "LinienExtrudieren: schnittPkt nicht instanziert\n";
							continue;
						}

						lnFl[i].p_neu[pktNr] = schnittPkt;
						lnFl[k].p_neu[pkt2Nr] = schnittPkt;
					}
				}
			}
		}
	}
	
	Punkt *pkt0, *pkt1;
	Linie *ln1, *ln2, *ln3, *ln4;
	for (int i = 0; i < gr; i++) {
		pkt0 = lnFl[i].ln->HolePunkt(0);
		pkt1 = lnFl[i].ln->HolePunkt(1);
		
		if (lnFl[i].p_neu[0] != NULL) {
			ln1 = Linie::NeueLinie(pkt0, lnFl[i].p_neu[0]);
			ln2 = Linie::NeueLinie(pkt1, lnFl[i].p_neu[0]);
			Dreieck::NeuesDreieck(lnFl[i].ln, ln1, ln2);
			if (lnFl[i].p_neu[1] != NULL) {
				ln3 = Linie::NeueLinie(lnFl[i].p_neu[1], lnFl[i].p_neu[0]);
				ln4 = Linie::NeueLinie(pkt1, lnFl[i].p_neu[1]);
				Dreieck::NeuesDreieck(ln2, ln3, ln4);				
			}
		} else if (lnFl[i].p_neu[1] != NULL) {
			ln1 = Linie::NeueLinie(pkt0, lnFl[i].p_neu[1]);
			ln2 = Linie::NeueLinie(pkt1, lnFl[i].p_neu[1]);
			Dreieck::NeuesDreieck(lnFl[i].ln, ln1, ln2);
		}
	}
	
	return;
}

bool LinienNormale(LinienFlaeche &lnFl, Vektor &richtungsPunkt, double resGefaelle, Achse prjRichtung)
{
	double bestGefaelle, normGefaelle;
	Vektor v_extrRichtung;
	int z = prjRichtung;
	
	Linie *ln = lnFl.ln;
	
	/*Lot des Punktes auf die Linie suchen*/
	Vektor lotFussPunkt(richtungsPunkt);
	if (!ln->LotFussPunkt(lotFussPunkt, prjRichtung, true))
		return false;
	/*ENDE Lot des Punktes auf die Linie suchen*/
	
	bestGefaelle = ln->HoleGefaelle(prjRichtung);
	if (std::isnan(bestGefaelle))
		return false;
	double a = pow(resGefaelle, 2);
	double b = pow(bestGefaelle, 2);
	if (a > b)
		return false;
	normGefaelle = sqrt(a - b) * ((resGefaelle < 0) ? -1.0 : 1.0);
	
	v_extrRichtung = richtungsPunkt - lotFussPunkt;
	double delta_z = v_extrRichtung.ProjLaenge(prjRichtung) * normGefaelle;
	v_extrRichtung.SetKoordinaten(z, delta_z);
		
	Vektor normale = v_extrRichtung.Kreuz(ln->HolePunkt(1)->HolePosition() - ln->HolePunkt(0)->HolePosition());
	if (normale.GetKoordinaten(z) < 0)
		normale *= -1;
	
	lnFl.n = normale;
	lnFl.extR = v_extrRichtung;
	
	return true;
}
/*ENDE Funktionen ohne Mitgliedschaft*/

/*Viereck*/
Viereck::Viereck():Flaeche()
{
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;
	l[0] = 0;
	l[1] = 0;
	l[2] = 0;
	l[3] = 0;

	NormaleBerechnen();
	MinMax();
}

Viereck::Viereck(Punkt** p1, Punkt** p2, Punkt** p3, Punkt** p4, Linie** l1, Linie** l2, Linie** l3, Linie** l4):Flaeche()
{
	p[0] = *p1;
	p[1] = *p2;
	p[2] = *p3;
	p[3] = *p4;
	l[0] = *l1;
	l[1] = *l2;
	l[2] = *l3;
	l[3] = *l4;

	l[0]->Hinzufuegen(this);
	l[1]->Hinzufuegen(this);
	l[2]->Hinzufuegen(this);
	l[3]->Hinzufuegen(this);

	HoleLayer()->Hinzufuegen(this);
	NormaleBerechnen();
	MinMax();
}

Viereck* Viereck::NeuesViereck(Dreieck* dA, Dreieck* dB)
{
	Punkt *t_p[4], *tempPunkt;
	Linie *t_l[4], *diagonale, *tempLinie;
	Dreieck *d[2];

	d[0] = dA;
	d[1] = dB;
	diagonale = NULL;

	for(int i = 0; i < 3; i++)
	{
		for(int k = 0; k < 3; k++)
		{
			if(d[0]->HoleLinie(i) == d[1]->HoleLinie(k))
			{
				diagonale = d[0]->HoleLinie(i);
				t_p[0] = diagonale->HolePunkt(0);
				t_p[2] = diagonale->HolePunkt(1);
			}
		}
	}
	if(diagonale == NULL)
	{
		return NULL;/*Keine gemeinsame Linie gefunden*/
	}

	for(int i = 0; i < 2; i++)/*Dreiecke*/
	{
		for(int k = 0; k < 3; k++)/*Linien*/
		{

			tempLinie = d[i]->HoleLinie(k);
			if(tempLinie != diagonale)
			{
				for(int m = 0; m < 2; m++)
				{
					tempPunkt = tempLinie->HolePunkt(m);
					if(tempPunkt == t_p[0])
					{
						t_p[2*i+1] = tempLinie->HolePunkt((m+1)%2);
						t_l[3*i] = tempLinie;
					}
					if(tempPunkt == t_p[2])
					{
						t_l[i+1] = d[i]->HoleLinie(k);
					}
				}
			}
		}
	}
	Viereck* rueckgabe = new Viereck(&t_p[0], &t_p[1], &t_p[2], &t_p[3], &t_l[0], &t_l[1], &t_l[2], &t_l[3]);
	if(rueckgabe)delete diagonale;
	return rueckgabe;
}

Viereck* Viereck::NeuesViereck(Linie *l0, Linie *l1, Linie *l2, Linie *l3)
{
	Linie *t_l[4], *tempLinie;
	Punkt *t_p[4];

	t_l[0] = l0;
	t_l[1] = l1;
	t_l[2] = l2;
	t_l[3] = l3;

	/*Linie Index 3 und Punkte Index 0 und 3 festlegen*/
	t_p[0] = t_l[3]->HolePunkt(0);
	t_p[1] = t_p[2] = NULL;
	t_p[3] = t_l[3]->HolePunkt(1);
	/*Linie Index 0 und Punkt Index 1 finden*/
	for(int i = 0; i < 3; i++)
	{
		for(int k = 0; k < 2; k++)
		{
			if(t_l[i]->HolePunkt(k) == t_p[0])
			{
				t_p[1] = t_l[i]->HolePunkt((k+1)%2);
				tempLinie = t_l[0];
				t_l[0] = t_l[i];
				t_l[i] = tempLinie;
			}
		}
	}
	/*Linie Index 2 und Punkt Index 2 finden*/
	for(int i = 1; i < 3; i++)
	{
		for(int k = 0; k < 2; k++)
		{
			if(t_l[i]->HolePunkt(k) == t_p[3])
			{
				t_p[2] = t_l[i]->HolePunkt((k+1)%2);
				tempLinie = t_l[2];
				t_l[2] = t_l[i];
				t_l[i] = tempLinie;
			}
		}
	}
	if((t_l[1]->HolePunkt(0) == t_p[1])&&(t_l[1]->HolePunkt(1) == t_p[2]))
	{
		Viereck* rueckgabe = new Viereck(&t_p[0], &t_p[1], &t_p[2], &t_p[3], &t_l[0], &t_l[1], &t_l[2], &t_l[3]);
		return (rueckgabe);
	}
	if((t_l[1]->HolePunkt(1) == t_p[1])&&(t_l[1]->HolePunkt(0) == t_p[2]))
	{
		Viereck* rueckgabe = new Viereck(&t_p[0], &t_p[1], &t_p[2], &t_p[3], &t_l[0], &t_l[1], &t_l[2], &t_l[3]);
		return (rueckgabe);
	}
	return NULL;/*kein zusammenhängender Linienzug*/
}

Viereck::Viereck(const Viereck& dV)
{
	if(HoleLayer())HoleLayer()->Entfernen(this);
	p[0] = dV.p[0];
	p[1] = dV.p[1];
	p[2] = dV.p[2];
	p[3] = dV.p[3];
	l[0] = dV.l[0];
	l[1] = dV.l[1];
	l[2] = dV.l[2];
	l[3] = dV.l[3];

	l[0]->Hinzufuegen(this);
	l[1]->Hinzufuegen(this);
	l[2]->Hinzufuegen(this);
	l[3]->Hinzufuegen(this);

	*this = dV;

	HoleLayer()->Hinzufuegen(this);
	NormaleBerechnen();
	MinMax();
}

Viereck& Viereck::operator=(const Viereck& dV)
{
	if(this != &dV)
	{
		if(HoleLayer())HoleLayer()->Entfernen(this);
		p[0] = dV.p[0];
		p[1] = dV.p[1];
		p[2] = dV.p[2];
		p[3] = dV.p[3];
		l[0] = dV.l[0];
		l[1] = dV.l[1];
		l[2] = dV.l[2];
		l[3] = dV.l[3];

		l[0]->Hinzufuegen(this);
		l[1]->Hinzufuegen(this);
		l[2]->Hinzufuegen(this);
		l[3]->Hinzufuegen(this);
	}

	HoleLayer()->Hinzufuegen(this);
	NormaleBerechnen();
	MinMax();
	return *this;
}

Viereck::~Viereck()
{
	l[0]->Entfernen(this);
	l[1]->Entfernen(this);
	l[2]->Entfernen(this);
	l[3]->Entfernen(this);

	if(HoleLayer())
	{
		HoleLayer()->Entfernen(this);
	}
}

void Viereck::HilfsVektorenBerechnen(void)
{
	m_p10 = p[1]->HolePosition()-p[0]->HolePosition();
	m_p30 = p[3]->HolePosition()-p[0]->HolePosition();
	m_p0123 = p[0]->HolePosition()-p[1]->HolePosition()+p[2]->HolePosition()-p[3]->HolePosition();
	return;
}

void Viereck::MinMax(void)
{
	for(int i = 0; i < 3; i++)
	{
		if(p[0]->HolePosition().GetKoordinaten(i) < p[1]->HolePosition().GetKoordinaten(i))
		{
			max[i] = p[1]->HolePosition().GetKoordinaten(i);
			min[i] = p[0]->HolePosition().GetKoordinaten(i);
		}else
		{
			max[i] = p[1]->HolePosition().GetKoordinaten(i);
			min[i] = p[0]->HolePosition().GetKoordinaten(i);
		}
		
		if(max[i] < p[2]->HolePosition().GetKoordinaten(i))max[i] = p[2]->HolePosition().GetKoordinaten(i);
		if(min[i] > p[2]->HolePosition().GetKoordinaten(i))min[i] = p[2]->HolePosition().GetKoordinaten(i);
		
		if(max[i] < p[3]->HolePosition().GetKoordinaten(i))max[i] = p[3]->HolePosition().GetKoordinaten(i);
		if(min[i] > p[3]->HolePosition().GetKoordinaten(i))min[i] = p[3]->HolePosition().GetKoordinaten(i);
	}
	return;
}

Linie* Viereck::HoleLinie(int index) const
{
	switch (index)
	{
		case 0:
			return l[0];
		case 1:
			return l[1];
		case 2:
			return l[2];
	}
	return l[3];
}

Punkt* Viereck::HolePunkt(int index) const
{
	switch (index)
	{
		case 0:
			return p[0];
		case 1:
			return p[1];
		case 2:
			return p[2];
	}
	return p[3];
}

RUZ_Layer* Viereck::HoleLayer(void) const
{
	if(p[0])return (p[0]->HoleLayer());
	return NULL;
}

RUZ_Objekt* Viereck::IstInnerhalb(double qx, double qy, Achse prjRichtung)
{
	/*Funktioniert nur, weil alle Vierecke der Klasse konvex sind*/
	double kPr[4];
	double p1x, p1y, p2x, p2y;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	for(int i = 0; i < 4; i++)
	{
		p1x = p[(0+i)%4]->HolePosition().GetKoordinaten(x);
		p1y = p[(0+i)%4]->HolePosition().GetKoordinaten(y);
		p2x = p[(1+i)%4]->HolePosition().GetKoordinaten(x);
		p2y = p[(1+i)%4]->HolePosition().GetKoordinaten(y);

		kPr[i] =  (p2x-p1x)*(qy-p1y) - (p2y-p1y)*(qx-p1x);
	}
	if((kPr[0]<=0)&&(kPr[1]<=0)&&(kPr[2]<=0)&&(kPr[3]<=0))
	{
		return this;
	}
	if((kPr[0]>=0)&&(kPr[1]>=0)&&(kPr[2]>=0)&&(kPr[3]>=0))
	{
		return this;
	}
	return NULL;
}

bool Viereck::LageAufFlaeche(Vektor& vOrt, RelativLage& rl_Lage_1, RelativLage& rl_Lage_2, Achse projRichtung)
{
	Vektor qp0 = vOrt - p[0]->HolePosition();
	
	double paraA = (m_p10.Kreuz(m_p0123)).GetKoordinaten(projRichtung);
	double paraB = (m_p10.Kreuz(m_p30)).GetKoordinaten(projRichtung)
					- (qp0.Kreuz(m_p0123)).GetKoordinaten(projRichtung);
	double paraC = (m_p30.Kreuz(qp0)).GetKoordinaten(projRichtung);

	double lambda1 = NAN;
	double lambda2 = NAN;
	if(!(QuadratischeGleichung(paraA, paraB, paraC, lambda1, lambda2)))
	{
		logSchreiben("keine Lsg quad. Gl. gefunden\n");
		return false;
	}

	if((lambda1<0)||(lambda1>1))
	{
		if(!(lambda2<0)||(lambda2>1))
		{
			double dTemp = lambda2;
			lambda2 = lambda1;
			lambda1 = dTemp;
		}else
		{
			logSchreiben("Lösungen der quadratischen Gleichung ausserhalb der Fläche\n");
			return false;
		}
	}

	double mue1 = NAN;
	double mue2 = NAN;
	double Divisor1 = m_p30.GetKoordinaten((projRichtung+1)%3) + m_p0123.GetKoordinaten((projRichtung+1)%3) * lambda1;
	double Divisor2 = m_p30.GetKoordinaten((projRichtung+1)%3) + m_p0123.GetKoordinaten((projRichtung+1)%3) * lambda2;
	
	if((Divisor1 == 0) || (Divisor2 == 0))return false;
	mue1 = (qp0.GetKoordinaten((projRichtung+1)%3) - m_p10.GetKoordinaten((projRichtung+1)%3) * lambda1) / Divisor1;
	mue2 = (qp0.GetKoordinaten((projRichtung+1)%3) - m_p10.GetKoordinaten((projRichtung+1)%3) * lambda2) / Divisor2;
	
	rl_Lage_1 = RelativLage(lambda1, mue1);
	rl_Lage_2 = RelativLage(lambda2, mue2);
	return true;
}

bool Viereck::Gefaelle(Vektor& punktQ, Vektor& v_gefaelle, Achse prjRichtung)
{
	if(!(IstInnerhalb(punktQ.GetKoordinaten(x), punktQ.GetKoordinaten(y), prjRichtung)))return false;

	HoleNormale(punktQ, v_gefaelle, prjRichtung);

	if(std::isnan(v_gefaelle.x()))return false;

	if(v_gefaelle.GetKoordinaten(prjRichtung) == 0)return false;
	v_gefaelle /= v_gefaelle.ProjLaenge(prjRichtung);

	double dTemp = -1 / v_gefaelle.GetKoordinaten(prjRichtung);
	v_gefaelle.SetKoordinaten(prjRichtung, dTemp);

	if(v_gefaelle.GetKoordinaten(prjRichtung)<0)v_gefaelle *= -1;

	return true;
}

void Viereck::HoleNormale(Vektor& punktQ, Vektor& vNormale, Achse prjRichtung)
{
	RelativLage relLage1 = RelativLage(0, 0);
	RelativLage relLage2 = RelativLage(0, 0);
	Vektor v_Richtung1, v_Richtung2;

	if(LageAufFlaeche(punktQ, relLage1, relLage2, (Achse)z))
	{
		vNormale = (m_p10 + m_p0123 * relLage1.mue); //Kreuzprodukt auf zwei operationen verteilt (const!)
		vNormale = vNormale.Kreuz(m_p30 + m_p0123 * relLage1.lambda);
		vNormale /= vNormale.Laenge();
		return;
	}
	vNormale = NULL_VEKTOR;
	return;
}

bool Viereck::OrtAufFlaeche(Vektor& punktQ, Achse prjRichtung, bool innenPruefung)
{
	if(innenPruefung)
	{
		if(!(IstInnerhalb(punktQ.GetKoordinaten(x), punktQ.GetKoordinaten(y), z)))return false;
	}
	
	RelativLage relLage1(0.0, 0.0);
	RelativLage relLage2(0.0, 0.0);
	if(LageAufFlaeche(punktQ, relLage1, relLage2, prjRichtung))
	{
		double ordinate = p[0]->HolePosition().GetKoordinaten(prjRichtung)
							+ relLage1.lambda * m_p10.GetKoordinaten(prjRichtung)
							+ relLage1.mue * m_p30.GetKoordinaten(prjRichtung)
							+ relLage1.lambda * relLage1.mue * m_p0123.GetKoordinaten(prjRichtung);
		punktQ.SetKoordinaten(prjRichtung, ordinate);
		return true;
	}
	return false;
}

bool Viereck::OrdinateAufEbene(Punkt* pktOrt, Achse prjRichtung)
{
	int z;
	z = (prjRichtung+3)%3;
	Vektor punktQ1, punktQ2;
	punktQ1 = punktQ2 = pktOrt->HolePosition();
	punktQ2.SetKoordinaten(z, punktQ2.GetKoordinaten(z)+1.0);

	Vektor vErgebnis1, vErgebnis2;
	if(DurchstossPunkt(punktQ1, punktQ2, vErgebnis1, vErgebnis2))
	{
		pktOrt->Positionieren(vErgebnis1);
		return true;
	}
	logSchreiben("OrdinateAufEbene: Punkt nicht gefunden\n");
	return false;
}

double Viereck::OrdinateAufEbene(double dX, double dY, Achse prjRichtung)
{
	int x, y, z;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;
	z = (prjRichtung+3)%3;

	Vektor punktQ1, punktQ2;
	punktQ1.SetKoordinaten(x, dX);
	punktQ1.SetKoordinaten(y, dY);
	punktQ1.SetKoordinaten(z, 0.0);

	punktQ2.SetKoordinaten(x, dX);
	punktQ2.SetKoordinaten(y, dY);
	punktQ2.SetKoordinaten(z, 1.0);

	Vektor vErgebnis1, vErgebnis2;

	if(DurchstossPunkt(punktQ1, punktQ2, vErgebnis1, vErgebnis2))
	{
		return vErgebnis1.GetKoordinaten(z);
	}
	logSchreiben("OrdinateAufEbene: Punkt nicht gefunden\n");
	return NAN;
}

bool Viereck::DurchstossPunkt(Linie* durchstossLn, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt)
{
	Vektor anfang, ende;
	anfang = durchstossLn->HolePunkt(0)->HolePosition();
	ende = durchstossLn->HolePunkt(1)->HolePosition();
	return(DurchstossPunkt(anfang, ende, v_rueckgabe1, v_rueckgabe2, aussenErlaubt));
}

bool Viereck::DurchstossPunkt(Vektor v_Ln_start, Vektor v_ln_ende, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt)
{
	bool bErfolg = false;
	Vektor d, e;
	d = v_ln_ende - v_Ln_start;
	e = v_Ln_start - p[0]->HolePosition();

	Vektor de, bd, ad, cd, vPara, vParb, vParc;
	de = d.Kreuz(e);
	bd = m_p30.Kreuz(d);
	ad = m_p10.Kreuz(d);
	cd = m_p0123.Kreuz(d);
	vPara = cd.Kreuz(bd);
	vParb = cd.Kreuz(de) + ad.Kreuz(bd);
	vParc = ad.Kreuz(de);

	double lambda1 = NAN;
	double lambda2 = NAN;
	int i;
	for(i = 0; i < 3; i++)
	{
		if(QuadratischeGleichung(vPara.GetKoordinaten(i), vParb.GetKoordinaten(i), vParc.GetKoordinaten(i), lambda1, lambda2))
		{
			break;
		}
	}
	if(i > 2)
	{
		logSchreiben("keine Lsg quad. Gl. gefunden\n");
		return false;
	}

	if((lambda1<0)||(lambda1>1))
	{
		if(abs(lambda1)>abs(lambda2))
		{
			double dTemp = lambda2;
			lambda2 = lambda1;
			lambda1 = dTemp;
		}
	}

	double mue1 = NAN;
	double mue2 = NAN;
	Vektor vDivisor1 = ad + cd * lambda1;
	Vektor vDivisor2 = ad + cd * lambda2;
	for(int k = 0; k < 3; k++)
	{
		if(vDivisor1.GetKoordinaten(k))
		{
			mue1 = -(de.GetKoordinaten(k) + bd.GetKoordinaten(k) * lambda1) / vDivisor1.GetKoordinaten(k);
			bErfolg = true;
			break;
		}
	}
	for(int k = 0; k < 3; k++)
	{
		if(vDivisor2.GetKoordinaten(k))
		{
			mue2 = -(de.GetKoordinaten(k) + bd.GetKoordinaten(k) * lambda2) / vDivisor2.GetKoordinaten(k);
			bErfolg = true;
			break;
		}
	}
	v_rueckgabe1 = p[0]->HolePosition() + m_p10 * mue1 + m_p30 * lambda1 + m_p0123 * mue1 * lambda1;
	v_rueckgabe2 = p[0]->HolePosition() + m_p10 * mue2 + m_p30 * lambda2 + m_p0123 * mue2 * lambda2;
	return bErfolg;
}

bool Viereck::IstEckPunkt(Punkt* pkt)
{
	for(int i = 0; i < 4; i++)
	{
		if(*pkt == *p[i])return true;
	}
	return false;
}

void Viereck::Verschieben(const Vektor& vkt)
{
	for(int i = 0; i < 4; i++)
	{
		p[i]->Verschieben(vkt);
	}
	return;
}

RUZ_Objekt* Viereck::Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = this->HoleLayer();
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	RUZ_Objekt *oObj;
	Linie *lnNeu[4];
	for(int i = 0; i < 4; i++)
	{
		lnNeu[i] = NULL;
		oObj = l[i]->Kopieren(lObjQuelle, lBereitsKopiert, layZielLayer);
		if(oObj)
		{
			if(oObj->HoleTyp() == RUZ_Linie)
			{
				lnNeu[i] = static_cast<Linie*>(oObj);
			}
		}
	}
	if(lnNeu[0] && lnNeu[1] && lnNeu[2] && lnNeu[3])
	{
		Viereck* vckNeu = Viereck::NeuesViereck(lnNeu[0], lnNeu[1], lnNeu[2], lnNeu[3]);
		if(vckNeu)
		{
			lObjQuelle->Hinzufuegen(vckNeu);
			lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, vckNeu));
			return vckNeu;
		}
	}
	return NULL;
}

void Viereck::DoHoehenlinienFinden(void)
{
	adjHL->ListeLoeschen("Viereck::HoehenlinienFinden");
	if(m_hoehenSchritt == 0)return;

	if(p[0]->HolePosition().z() == p[1]->HolePosition().z())
		if(p[2]->HolePosition().z() == p[3]->HolePosition().z())
			if(p[0]->HolePosition().z() == p[2]->HolePosition().z())return;/*Abbruch bei waagerechten Flaechen*/

	Linie *seite[2];
	double t_L0, t_L1, t_L2, t_L3;
	t_L0 = l[0]->Laenge();
	t_L1 = l[1]->Laenge();
	t_L2 = l[2]->Laenge();
	t_L3 = l[3]->Laenge();

	if((abs(t_L0 - t_L2) / ((t_L0 + t_L2)*2)) < (abs(t_L1 - t_L3) / ((t_L1 + t_L3)*2)))/*möglichst gleichlange Seiten finden*/
	{
		seite[0] = Linie::NeueLinie(p[0], p[1]);
		seite[1] = Linie::NeueLinie(p[3], p[2]);
	}else{
		seite[0] = Linie::NeueLinie(p[0], p[3]);
		seite[1] = Linie::NeueLinie(p[1], p[2]);
	}

	double mittlereLaenge = (seite[0]->Laenge() + seite[1]->Laenge()) / 2.0;
	double anzahlFelder = ceil((mittlereLaenge / m_suchRadius));
	Punkt *hilfsPunkt[4];
	Linie *hilfsLinie[4];
	for(int i = 0; i < 4; i++)
	{
		hilfsPunkt[i] = new Punkt(Vektor(0, 0, 0), HoleLayer());
	}
	for(int i = 0; i < 4; i++)
	{
		hilfsLinie[i] = Linie::NeueLinie(hilfsPunkt[i], hilfsPunkt[(i+1)%4]);
	}
	Vektor tempOrt;

	for(int i = 0; i < anzahlFelder; i++)
	{
		tempOrt = seite[0]->PunktBeiLambda((double)i / anzahlFelder);
		hilfsPunkt[0]->Positionieren(tempOrt);

		tempOrt = seite[0]->PunktBeiLambda((double)(i+1) / anzahlFelder);
		hilfsPunkt[1]->Positionieren(tempOrt);

		tempOrt = seite[1]->PunktBeiLambda((double)(i+1) / anzahlFelder);
		hilfsPunkt[2]->Positionieren(tempOrt);

		tempOrt = seite[1]->PunktBeiLambda((double)i / anzahlFelder);
		hilfsPunkt[3]->Positionieren(tempOrt);
		EinzelfeldHoehenlinien(hilfsLinie[0], hilfsLinie[1], hilfsLinie[2], hilfsLinie[3]);
	}

	for(int i = 0; i<4; i++)
	{
		delete hilfsPunkt[i];/*Punkt löscht Linie!*/
	}
	delete seite[0];
	delete seite[1];

	return;
}

void Viereck::NormaleBerechnen(void)
{
	HilfsVektorenBerechnen();
	m_normale = Vektor(0, 0, 0);
	return;
}

Vektor Viereck::Schwerpunkt(void)
{
	Vektor vkt0, vkt1;
	Vektor s1 = (p[0]->HolePosition() + p[1]->HolePosition() + p[2]->HolePosition());
	s1 /= 3;
	vkt0 = p[1]->HolePosition() - p[0]->HolePosition();
	vkt1 = p[2]->HolePosition() - p[0]->HolePosition();
	double a1 = abs(vkt0.Kreuz(vkt1).z());

	Vektor s2 = (p[0]->HolePosition() + p[2]->HolePosition() + p[3]->HolePosition());
	s2 /= 3;
	vkt0 = p[1]->HolePosition() - p[0]->HolePosition();
	vkt1 = p[3]->HolePosition() - p[0]->HolePosition();
	double a2 = abs(vkt0.Kreuz(vkt1).z());

	if((a1+a2) != 0)
	{
		vkt0 = s1*a1+s2*a2;
		vkt0 /= (a1+a2);
		return vkt0;
	}

	return Vektor(0, 0, 0);
}

bool Viereck::operator==(const Flaeche& vergleichsFlaeche)
{
	if(vergleichsFlaeche.HoleTyp() != RUZ_Viereck)return false;
	int uebereinstimmungen = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int k = 0; k < 4; k++)
		{
			if(p[i] == vergleichsFlaeche.HolePunkt(k))
			{
				uebereinstimmungen++;
				break;
			}
		}
	}
	if(uebereinstimmungen == 4)return true;
	return false;
}

void Viereck::EinzelfeldHoehenlinien(Linie *l0, Linie *l1, Linie *l2, Linie *l3)
{
	Linie *t_l[4];
	t_l[0] = l0;
	t_l[1] = l1;
	t_l[2] = l2;
	t_l[3] = l3;

	Liste<Hoehenpunkt> *listeHP = new Liste<Hoehenpunkt>;
	for(int i = 0; i < 4; i++)
	{
		t_l[i]->HoehenPunkteFinden(listeHP, m_hoehenSchritt, m_suchRadius, m_startHoehe);
	}
	LoescheDoppeltePunkte(listeHP);
	for(Listenelement<Hoehenpunkt> *hp_LE_laeufer = listeHP->GetErstesListenelement(); hp_LE_laeufer != NULL; hp_LE_laeufer = hp_LE_laeufer->GetNachfolger())
	{
		Hoehenpunkt *hp_laeufer, *hp_naechster;
		hp_laeufer = hp_LE_laeufer->GetElement();
		for(Listenelement<Hoehenpunkt> *hp_LE_naechster = hp_LE_laeufer->GetNachfolger(); hp_LE_naechster != NULL; hp_LE_naechster = hp_LE_naechster->GetNachfolger())
		{
			hp_naechster = hp_LE_naechster->GetElement();
			if(abs(hp_naechster->z - hp_laeufer->z) < 1e-12)
			{
				SetzeHoehenlinie(hp_laeufer->x, hp_laeufer->y, hp_naechster->x, hp_naechster->y, hp_naechster->z);
			}else{
				break;
			}
		}
	}
	listeHP->ListeLoeschen("");
	delete listeHP;
	return;
}

void Viereck::EckenNeuErmitteln(void)
{
	Punkt *t_p[4];
	Linie *t_l[4], *tempLinie;

	/*Vorab auf Linien mit Länge null prüfen*/
	for(int i = 0; i < 4; i++)
	{
		if(l[i]->HolePunkt(0) == l[i]->HolePunkt(1))
		{
			Dreieck::NeuesDreieck(l[(i+1)%4], l[(i+2)%4], l[(i+3)%4]);
			delete this;
			return;
		}
	}

	t_l[0] = l[0];
	t_l[1] = l[1];
	t_l[2] = l[2];
	t_l[3] = l[3];

	/*Linie Index 3 und Punkte Index 0 und 3 festlegen*/
	t_p[0] = t_l[3]->HolePunkt(0);
	t_p[1] = t_p[2] = NULL;
	t_p[3] = t_l[3]->HolePunkt(1);

	/*Linie Index 0 und Punkt Index 1 finden*/
	for(int i = 0; i < 3; i++)/*Linienindex*/
	{
		for(int k = 0; k < 2; k++)
		{
			if(t_l[i]->HolePunkt(k) == t_p[0])
			{
				t_p[1] = t_l[i]->HolePunkt((k+1)%2);
				tempLinie = t_l[0];
				t_l[0] = t_l[i];
				t_l[i] = tempLinie;
			}
		}
	}
	/*ENDE Linie Index 0 und Punkt Index 1 finden*/
	/*Linie und Punkt Index 2 finden*/
	for(int i = 1; i < 3; i++)/*Linienindex*/
	{
		for(int k = 0; k < 2; k++)
		{
			if(t_l[i]->HolePunkt(k) == t_p[3])
			{
				t_p[2] = t_l[i]->HolePunkt((k+1)%2);
				tempLinie = t_l[2];
				t_l[2] = t_l[i];
				t_l[i] = tempLinie;
			}
		}
	}
	/*ENDE Linie und Punkt Index 2 finden*/
	if(((t_l[1]->HolePunkt(0) == t_p[1])&&(t_l[1]->HolePunkt(1) == t_p[2]))
	   ||
	   ((t_l[1]->HolePunkt(1) == t_p[1])&&(t_l[1]->HolePunkt(0) == t_p[2])))
	{
			for(int i = 0; i < 4; i++)
			{
				l[i] = t_l[i];
				p[i] = t_p[i];
			}
	}else{
		delete this;
		return;
	}
	/*Auf doppelte Punkte prüfen*/
	for(int i = 0; i < 4; i++)
	{
		if(p[i] == p[(i+2)%4])
		{
			delete this;
			return;
		}
	}
	/*ENDE Auf doppelte Punkte prüfen*/
	HoleLayer()->GeaenderteFlaecheHinzufuegen(this);
	return;
}

void Viereck::ErsetzeLinie(Linie* originalLn, Linie* ersatzLn)
{
	for(int i = 0; i < 4; i++)
	{
		if(l[i] == originalLn)
		{
			l[i] = ersatzLn;
			originalLn->Entfernen(this);
			ersatzLn->Hinzufuegen(this);
		}
	}
	EckenNeuErmitteln();
	return;
}

void Viereck::Teilen(Punkt* punktQ, Achse prjRichtung, bool ungefaehr)
{
	logSchreiben("Viereck::Teilen(mit Variablen) betreten\n");
	if(punktQ->HoleLayer() != this->HoleLayer())return;

	Vektor ortQ;
	ortQ = punktQ->HolePosition();

	if(!(this->OrtAufFlaeche(ortQ, prjRichtung)))
	{
		return;
	}
	if(ungefaehr)
	{
		punktQ->Positionieren(ortQ);
	}

	/*Lage Teilpunkt auf Eckpunkten ausschließen*/
	if(punktQ->HolePosition() == p[0]->HolePosition())
	{
		return;
	}
	if(punktQ->HolePosition() == p[1]->HolePosition())
	{
		return;
	}
	if(punktQ->HolePosition() == p[2]->HolePosition())
	{
		return;
	}
	if(punktQ->HolePosition() == p[3]->HolePosition())
	{
		return;
	}
	/*ENDE Lage Teilpunkt auf Eckpunkten ausschließen*/


	return;
}

void Viereck::Teilen(void)
{
	Dreieck *drEins, *drZwei;
	drEins = drZwei = NULL;
	/*teilt das Viereck entlang der kurzen Diagonalen in zwei Dreiecke*/
	double t_diagonalLaenge[2];
	t_diagonalLaenge[0] = (p[0]->HolePosition() - p[2]->HolePosition()).Laenge();
	t_diagonalLaenge[1] = (p[1]->HolePosition() - p[3]->HolePosition()).Laenge();

	int t_startEcke;
	t_diagonalLaenge[0] < t_diagonalLaenge[1] ? t_startEcke = 0 : t_startEcke = 1;
	Linie* t_diagonale = Linie::NeueLinie(p[(t_startEcke)%4], p[(t_startEcke+2)%4]);

	if(t_diagonale)
	{
		drEins = Dreieck::NeuesDreieck(t_diagonale, l[(t_startEcke)%4], l[(t_startEcke+1)%4]);
		drZwei = Dreieck::NeuesDreieck(t_diagonale, l[(t_startEcke+2)%4], l[(t_startEcke+3)%4]);
	}
	else
	{
		logSchreiben("Diagonale NICHT erzeugt\n");
	}
	if(drEins && drZwei)
	{
		delete this;
		return;
	}else{
		logSchreiben("mind. ein Dreieck nicht erzeugt\n");
		logSchreiben("Startecke war: %i\n", t_startEcke);
		if(drEins)delete drEins;
		if(drZwei)delete drZwei;
		if(t_diagonale)delete t_diagonale;
	}
	return;
}

void Viereck::ZwangsTeilung(Punkt* punktQ)
{
	return;
}

void Viereck::ImLogAusgeben(void)
{
	Vektor vVkt;
	logSchreiben("Viereck:\t%p\n", this);
	logSchreiben("Eckpunkte:\n");
	for(int i = 0; i < 4; i++)
	{
		vVkt = p[i]->HolePosition();
		logSchreiben("\t%.3f,\t%.3f,\t%.3f\n", vVkt.x(), vVkt.y(), vVkt.z());
	}
	logSchreiben("Hilfsgrößen:\n");
	logSchreiben("m_p10:\t%.5f, \t%.5f, \t%.5f\n", m_p10.x(), m_p10.y(), m_p10.z());
	logSchreiben("m_p30:\t%.5f, \t%.5f, \t%.5f\n", m_p30.x(), m_p30.y(), m_p30.z());
	logSchreiben("m_p0123:\t%.5f, \t%.5f, \t%.5f\n", m_p0123.x(), m_p0123.y(), m_p0123.z());
	return;
}
/*ENDE Viereck*/

/*Hoehenmarke*/
HoehenMarke::HoehenMarke(const Vektor& vektor, RUZ_Layer* layer)
{
	vOrt = vektor;
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

HoehenMarke::HoehenMarke(double x, double y, RUZ_Layer* layer)
{
	vOrt = Vektor(x, y, 0);
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

HoehenMarke::HoehenMarke(double* dbl, RUZ_Layer* layer)
{
	vOrt = Vektor(dbl[0], dbl[1], 0);
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

HoehenMarke::~HoehenMarke()
{
	if(m_flaeche != NULL)
	{
		if(HoleTyp() == RUZ_HoehenMarke)m_flaeche->Entfernen(this);
	}
	if(m_layer)
	{
		if(HoleTyp() == RUZ_HoehenMarke)m_layer->Entfernen(this);
	}
}

void HoehenMarke::Verschieben(const Vektor& vektor)
{
	vOrt += vektor;
	LagePruefen();
	return;
}

void HoehenMarke::LagePruefen(void)
{
	if(m_flaeche != NULL)
	{
		if(!(m_flaeche->IstInnerhalb(vOrt.x(), vOrt.y(), z)))
		{
			m_flaeche->Entfernen(this);
			FlaecheFinden();
		}
		NeuBerechnen();
	}else{
		FlaecheFinden();
		NeuBerechnen();
	}
	return;
}

RUZ_Objekt* HoehenMarke::Kopieren(Liste<RUZ_Objekt>* lObjekte, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = m_layer;
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	HoehenMarke* hmNeu = new HoehenMarke(vOrt, layZielLayer);
	if(hmNeu)
	{
		lObjekte->Hinzufuegen(hmNeu);
		lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, hmNeu));
		return hmNeu;
	}
	return NULL;
}

void HoehenMarke::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	Vektor tempOrt = (vOrt - festOrt);
	tempOrt.SetKoordinaten(tempOrt.x() * fktX, tempOrt.y() * fktY, tempOrt.z() * fktZ);
	vOrt = tempOrt + festOrt;
	return;
}

void HoehenMarke::Positionieren(const Vektor& vektor)
{
	vOrt = vektor;
	if(m_flaeche != NULL)
	{
		if(!(m_flaeche->IstInnerhalb(vOrt.x(), vOrt.y(), z)))
		{
			m_flaeche->Entfernen(this);
			FlaecheFinden();
		}
		NeuBerechnen();
	}else{
		FlaecheFinden();
		NeuBerechnen();
	}
	return;
}

bool HoehenMarke::IstInFlaeche(void)
{
	FlaecheFinden();
	if(m_flaeche != NULL)return true;
	return false;
}

void HoehenMarke::FlaecheNullen(void)
{
	if(m_flaeche)
	{
		m_flaeche->Entfernen(this);
	}
	m_flaeche = NULL;
	return;
}

void HoehenMarke::FlaecheFinden(void)
{
	int aktX, aktY, aktZ;
	aktZ = m_layer->HoleProjektion();
	aktX = (aktZ + 1) % 3;
	aktY = (aktZ + 2) % 3;
	if(m_flaeche != NULL)
	{
		if(m_flaeche->IstInnerhalb(vOrt.GetKoordinaten(aktX), vOrt.GetKoordinaten(aktY), (Achse)aktZ))
		{
			return;
		}
	}
	Liste<Flaeche>* flaechenListe = m_layer->HoleFlaechen();
	for(Flaeche* sucher = flaechenListe->GetErstesElement(); sucher != NULL; sucher = flaechenListe->GetNaechstesElement())
	{
		if(sucher->IstInnerhalb(vOrt.GetKoordinaten(aktX), vOrt.GetKoordinaten(aktY), (Achse)aktZ))
		{
			m_flaeche = sucher;
			m_flaeche->Hinzufuegen(this);
			return;
		}
	}
	m_flaeche = NULL;
	return;
}

void HoehenMarke::NeuBerechnen(void)
{
	if(m_flaeche != NULL)
	{
		m_flaeche->OrtAufFlaeche(vOrt, m_layer->HoleProjektion());
	}
	return;
}

Vektor HoehenMarke::HolePosition(void) const
{
	return vOrt;
}

bool HoehenMarke::HolePosition(Vektor& vkt)
{
	vkt = vOrt;
	return true;
}

bool HoehenMarke::IstNahebei(double qx, double qy, double maxAbstand, Achse prjRichtung)
{
	double entfernung;
	int x, y;
	x = (prjRichtung+1)%3;
	y = (prjRichtung+2)%3;

	entfernung = sqrt(pow(this->HolePosition().GetKoordinaten(x) - qx,2)
					  + pow(this->HolePosition().GetKoordinaten(y) - qy,2));

	return (entfernung < maxAbstand);
}

/*ENDE Hoehenmarke*/

/*GefaelleMarke*/
GefaelleMarke::GefaelleMarke(float fMin, float fOpt, float fMax, const Vektor& vektor, RUZ_Layer* layer, Flaeche* flTemp)
{
	fMinimum = fMin;
	fOptimum = fOpt;
	fMaximum = fMax;
	vOrt = vektor;
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

GefaelleMarke::GefaelleMarke(float fMin, float fOpt, float fMax, double x, double y, RUZ_Layer* layer, Flaeche* flTemp)
{
	fMinimum = fMin;
	fOptimum = fOpt;
	fMaximum = fMax;
	vOrt = Vektor(x, y, 0);
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

GefaelleMarke::GefaelleMarke(float fMin, float fOpt, float fMax, double* dbl, RUZ_Layer* layer, Flaeche* flTemp)
{
	fMinimum = fMin;
	fOptimum = fOpt;
	fMaximum = fMax;
	vOrt = Vektor(dbl[0], dbl[1], 0);
	m_layer = layer;
	m_layer->Hinzufuegen(this);
	m_flaeche = NULL;
	FlaecheFinden();
	NeuBerechnen();
}

GefaelleMarke::~GefaelleMarke()
{
	if(m_flaeche != NULL)
	{
		m_flaeche->Entfernen(this);
	}
	if(m_layer)
	{
		m_layer->Entfernen(this);
	}
}

void GefaelleMarke::LagePruefen(void)
{
	if(m_flaeche != NULL)
	{
		if(!(m_flaeche->IstInnerhalb(vOrt.x(), vOrt.y(), z)))
		{
			m_flaeche->Entfernen(this);
			FlaecheFinden();
		}
		NeuBerechnen();
	}else{
		FlaecheFinden();
		NeuBerechnen();
	}
	return;
}

bool GefaelleMarke::HolePosition(Vektor& vkt)
{
	vkt = vOrt;
	return true;
}

Vektor GefaelleMarke::HolePosition(void) const
{
	return vOrt;
}

void GefaelleMarke::FlaecheNullen(void)
{
	if(m_flaeche)
	{
		m_flaeche->Entfernen(this);
	}
	m_flaeche = NULL;
	return;
}

bool GefaelleMarke::IstInFlaeche(void)
{
	FlaecheFinden();
	if(m_flaeche != NULL)return true;
	return false;
}

void GefaelleMarke::SetzeMinOptMax(float tMin, float tOpt, float tMax)
{
	fMinimum = tMin;
	fOptimum = tOpt;
	fMaximum = tMax;
	NeuBerechnen();
	return;
}

unsigned char* GefaelleMarke::HoleFarbe(void)
{
	return cFarbe;
}

Vektor GefaelleMarke::HoleGefaelle(void)
{
	return vGefaelle;
}

void GefaelleMarke::FlaecheFinden(void)
{
	int aktX, aktY, aktZ;
	aktZ = m_layer->HoleProjektion();
	aktX = (aktZ + 1) % 3;
	aktY = (aktZ + 2) % 3;
	if(m_flaeche != NULL)
	{
		if(m_flaeche->IstInnerhalb(vOrt.GetKoordinaten(aktX), vOrt.GetKoordinaten(aktY), (Achse)aktZ))
		{
			return;
		}
		m_flaeche->Entfernen(this);
		Flaeche* flNachbar;
		Linie* lnRand;
		int iEcken;
		m_flaeche->HoleTyp() == RUZ_Dreieck ? iEcken = 3 : iEcken = 4;
		for(int i = 0; i < iEcken; i++)
		{
			lnRand = m_flaeche->HoleLinie(i);
			Liste<Flaeche>* flLst = lnRand->HoleFlaechen();
			for(flNachbar = flLst->GetErstesElement(); flNachbar != NULL; flNachbar = flLst->GetNaechstesElement())
			{
				if(flNachbar->IstInnerhalb(vOrt.GetKoordinaten(aktX), vOrt.GetKoordinaten(aktY), (Achse)aktZ))
				{
					m_flaeche->Entfernen(this);
					m_flaeche = flNachbar;
					m_flaeche->Hinzufuegen(this);
					return;
				}
			}
		}
	}
	Liste<Flaeche>* flaechenListe = m_layer->HoleFlaechen();
	for(Flaeche* sucher = flaechenListe->GetErstesElement(); sucher != NULL; sucher = flaechenListe->GetNaechstesElement())
	{
		if(sucher->IstInnerhalb(vOrt.GetKoordinaten(aktX), vOrt.GetKoordinaten(aktY), (Achse)aktZ))
		{
			m_flaeche = sucher;
			m_flaeche->Hinzufuegen(this);
			return;
		}
	}
	m_flaeche = NULL;
	return;
}

void GefaelleMarke::NeuBerechnen(void)
{
	if(m_flaeche == NULL)
	{
		cFarbe[0] = 0;
		cFarbe[1] = 0;
		cFarbe[2] = 0;
		vGefaelle = Vektor(0, 0, 0);
		return;
	}

	m_flaeche->Gefaelle(vOrt, vGefaelle, m_layer->HoleProjektion());
	double dGefaelle = vGefaelle.GetKoordinaten(m_layer->HoleProjektion());// .ProjLaenge(m_layer->HoleProjektion());
	//if(dGefaelle)dGefaelle = 1 / dGefaelle;

	unsigned char b = (255 - 255 * (dGefaelle - fMinimum) / (fOptimum - fMinimum) * (dGefaelle > fMinimum)) * (dGefaelle <= fOptimum);
	unsigned char g = 255 * (dGefaelle - fMinimum) / (fOptimum - fMinimum) * (dGefaelle < fOptimum) * (dGefaelle >= fMinimum)
						+ (255 - 255 * (dGefaelle - fOptimum) / (fMaximum - fOptimum)) * (dGefaelle < fMaximum) * (dGefaelle >= fOptimum);
	unsigned char r = 255 * (dGefaelle - fOptimum) / (fMaximum - fOptimum) * (dGefaelle > fOptimum) * (dGefaelle < fMaximum) + 255 * (dGefaelle >= fMaximum);

	cFarbe[0] = r;
	cFarbe[1] = g;
	cFarbe[2] = b;

	return;
}
/*ENDE GefaelleMarke*/

/*Kreis*/
Kreis::Kreis() : RUZ_Objekt()
{
	vOrt = Vektor(0, 0, 0);
	dRadius = 0.0;
	m_layer = NULL;
	m_Fangpunkte = new Liste<Vektor>;
}

Kreis::Kreis(const Vektor& vkt, double rad, RUZ_Layer* lay) : RUZ_Objekt()
{
	vOrt = vkt;
	dRadius = rad;
	m_layer = lay;
	if(m_layer)
		m_layer->Hinzufuegen(this);
	m_Fangpunkte = new Liste<Vektor>;
}

Kreis::Kreis(const Kreis& t_kr) : RUZ_Objekt()
{
	vOrt = t_kr.HolePosition();
	dRadius = t_kr.HoleRadius();
	m_layer = t_kr.HoleLayer();
	if(m_layer)
		m_layer->Hinzufuegen(this);
	m_Fangpunkte = new Liste<Vektor>;
}

Kreis& Kreis::operator=(const Kreis& t_kr)
{
	if(&t_kr == this)return *this;
	vOrt = t_kr.HolePosition();
	dRadius = t_kr.HoleRadius();
	m_layer = t_kr.HoleLayer();
	if(m_layer)
		m_layer->Hinzufuegen(this);
	m_Fangpunkte = new Liste<Vektor>;
	return *this;
}

Kreis::~Kreis()
{
	if(m_layer)
		m_layer->Entfernen(this);
	m_Fangpunkte->ListeLoeschen("DTOR_Kreis");
	delete m_Fangpunkte;
}

RUZ_Typ Kreis::HoleTyp(void) const
{
	return RUZ_Kreis;
}

RUZ_Layer* Kreis::HoleLayer(void) const
{
	return m_layer;
}

void Kreis::Verschieben(const Vektor& vkt)
{
	vOrt += vkt;
	return;
}

void Kreis::Positionieren(const Vektor& vkt)
{
	vOrt = vkt;
	return;
}

RUZ_Objekt* Kreis::Kopieren(Liste<RUZ_Objekt>* lObjekte, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = m_layer;
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	Kreis* krNeu = new Kreis(vOrt, dRadius, layZielLayer);
	if(krNeu)
	{
		lObjekte->Hinzufuegen(krNeu);
		lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, krNeu));
		return krNeu;
	}
	return NULL;
}

void Kreis::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	Vektor tempOrt = (vOrt - festOrt);
	tempOrt.SetKoordinaten(tempOrt.x() * fktX, tempOrt.y() * fktY, tempOrt.z() * fktZ);
	vOrt = tempOrt + festOrt;
	double dRFkt = abs(fktX);
	dRadius *= dRFkt;
	return;
}

Vektor Kreis::HolePosition(void) const
{
	return vOrt;
}

bool Kreis::HolePosition(Vektor& vkt)
{
	vkt = vOrt;
	return true;
}

void Kreis::SetzeRadius(double rad)
{
	dRadius = rad;
	return;
}

double Kreis::HoleRadius(void) const
{
	return dRadius;
}

Liste<Vektor>* Kreis::HoleFangpunkte(void)
{
	return m_Fangpunkte;
}

bool Kreis::FindeSchnittpunkte(Linie* lnUebergabe, Achse aPrjRichtung)
{
	int aktZ;
	aktZ = (aPrjRichtung)%3;

	Vektor vLfp, vP21, lnP0, lnP1;//Lotfusspunkt, Richtungs der Linie
	double dQuAbstand, dStich, horLaenge;//Quadrierter Abstand Kreismittelpunkt zur Linie, Abstand Lotfusspunkt zu den Schnittpunkten

	vLfp = vOrt;
	if(lnUebergabe->LotFussPunkt(vLfp, aPrjRichtung, true))
	{
		Vektor vAbstand = (vLfp - vOrt);
		vAbstand.SetKoordinaten(aktZ, 0.0);
		dQuAbstand = vAbstand.QuadratLaenge();
		dStich = sqrt(dRadius * dRadius - dQuAbstand);

		lnP0 =  lnUebergabe->HolePunkt(0)->HolePosition();
		lnP1 =  lnUebergabe->HolePunkt(1)->HolePosition();
		vP21 = lnP0 - lnP1;

		lnP0.SetKoordinaten(aktZ, 0.0);
		lnP1.SetKoordinaten(aktZ, 0.0);

		horLaenge = (lnP0-lnP1).Laenge();
		if(!horLaenge)return false;

		Vektor* vSchnPkt = new Vektor();
			*vSchnPkt = vLfp + vP21 * (dStich / horLaenge);
			m_Fangpunkte->Hinzufuegen(vSchnPkt);
		vSchnPkt = new Vektor();
			*vSchnPkt = vLfp - vP21 * (dStich / horLaenge);
			m_Fangpunkte->Hinzufuegen(vSchnPkt);

		return true;
	}
	return false;
}

bool Kreis::FindeSchnittpunkte(Kreis* t_kr, Achse prjRichtung)
{
	int aktX, aktY, aktZ;
	aktX = (prjRichtung+1)%3;
	aktY = (prjRichtung+2)%3;
	aktZ = (prjRichtung)%3;

	Vektor dm, dm_n, q0;/*Abstand der Mittelpunkte, Normale zur Verbindungslinie der Mittelpunkte, LotFußpunkt der Verbindung der Schnittpunkte*/
	double delta, abstand;

	dm = t_kr->HolePosition() - vOrt;
	dm.SetKoordinaten(aktZ, 0.0);

	dm_n.SetKoordinaten(aktX, dm.GetKoordinaten(aktY));
	dm_n.SetKoordinaten(aktY, (-1) * dm.GetKoordinaten(aktX));
	dm_n.SetKoordinaten(aktZ, dm.GetKoordinaten(aktZ));
	dm_n /= dm.Laenge();

	double t_radius = t_kr->HoleRadius();
	delta = 0.5 * ((dRadius * dRadius - t_radius * t_radius) / (dm * dm) + 1);
	q0 = vOrt + dm * delta;

	double radikant = dRadius * dRadius - delta * delta * (dm * dm);
	if(radikant < 0)return false;
	abstand = sqrt(radikant);

	Vektor* schnPkt = new Vektor();
		*schnPkt = q0 + dm_n * abstand;
		m_Fangpunkte->Hinzufuegen(schnPkt);
	schnPkt = new Vektor();
		*schnPkt = q0 - dm_n * abstand;
		m_Fangpunkte->Hinzufuegen(schnPkt);

	return true;
}

void Kreis::LoescheFangpunkte(void)
{
	m_Fangpunkte->ListeLoeschen("Kreis::LoescheFangpunkte");
	return;
}

int Kreis::IstNahebei(double _x, double _y, double abstand, Achse aktProjektion)
{
	int aktX, aktY;
	aktX = (aktProjektion+1)%3;
	aktY = (aktProjektion+2)%3;

	double vorhAbstand = (vOrt.GetKoordinaten(aktX)-_x) * (vOrt.GetKoordinaten(aktX)-_x) +
							(vOrt.GetKoordinaten(aktY)-_y) * (vOrt.GetKoordinaten(aktY)-_y);
	if(vorhAbstand <= (abstand * abstand))return 1;
	if((vorhAbstand >= ((dRadius - abstand) * (dRadius - abstand)))&&(vorhAbstand <= ((dRadius + abstand) * (dRadius + abstand))))return 2;

	return 0;
}
/*ENDE Kreis*/

/*Fangpunkt*/
Fangpunkt::Fangpunkt(const Vektor& vkt, RUZ_Layer* lay)
{
	vOrt = vkt;
	if(lay)
	{
		m_layer = lay;
		m_layer->Hinzufuegen(this);
	}
}
Fangpunkt::Fangpunkt(double _x, double _y, double _z, RUZ_Layer*lay)
{
	vOrt.SetKoordinaten(_x, _y, _z);
	if(lay)
	{
		m_layer = lay;
		m_layer->Hinzufuegen(this);
	}
}
Fangpunkt::Fangpunkt(double* koor, RUZ_Layer* lay)
{
	vOrt.SetKoordinaten(koor);
	if(lay)
	{
		m_layer = lay;
		m_layer->Hinzufuegen(this);
	}
}
Fangpunkt::~Fangpunkt()
{
	if(m_layer)
	{
		m_layer->Entfernen(this);
	}
}
bool Fangpunkt::IstNahebei(double _x, double _y, double abstand, Achse aktProjektion)
{
	int aktX, aktY;
	aktX = (aktProjektion+1)%3;
	aktY = (aktProjektion+2)%3;

	double vorhAbstand = (vOrt.GetKoordinaten(aktX)-_x) * (vOrt.GetKoordinaten(aktX)-_x) +
							(vOrt.GetKoordinaten(aktY)-_y) * (vOrt.GetKoordinaten(aktY)-_y);
	if(vorhAbstand <= (abstand * abstand))return 1;

	return 0;
}

bool Fangpunkt::HolePosition(Vektor& vkt)
{
	vkt = vOrt;
	return true;
}

RUZ_Objekt* Fangpunkt::Kopieren(Liste<RUZ_Objekt>* lObjekte, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer)
{
	RUZ_Layer* layZielLayer;
	if(zielLayer)
	{
		layZielLayer = zielLayer;
	}else{
		layZielLayer = m_layer;
	}
	for(Listenelement<ObjektPaar>* LE_AktObj = lBereitsKopiert->GetErstesListenelement(); LE_AktObj; LE_AktObj = LE_AktObj->GetNachfolger())
	{
		ObjektPaar* objPaarAkt = LE_AktObj->GetElement();
		if(objPaarAkt->objReferenz == this)return objPaarAkt->objBezugsObj;/*Nicht kopieren, weil schon vorhanden*/
	}
	Fangpunkt* fpNeu = new Fangpunkt(vOrt, layZielLayer);
	if(fpNeu)
	{
		lObjekte->Hinzufuegen(fpNeu);
		lBereitsKopiert->Hinzufuegen(new ObjektPaar(this, fpNeu));
		return fpNeu;
	}
	return NULL;
}

void Fangpunkt::Skalieren(const Vektor& festOrt, double fktX, double fktY, double fktZ)
{
	Vektor tempOrt = (vOrt - festOrt);
	tempOrt.SetKoordinaten(tempOrt.x() * fktX, tempOrt.y() * fktY, tempOrt.z() * fktZ);
	vOrt = tempOrt + festOrt;
	return;
}

void Fangpunkt::Positionieren(const Vektor& vkt)
{
	vOrt = vkt;
	return;
}
/*ENDE Fangpunkt*/
