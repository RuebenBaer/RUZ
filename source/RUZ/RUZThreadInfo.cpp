//TODO (Ansi#1#): Leere Aufgabe

#include "RUZThreadInfo.h"
#include "RUZVerwaltung.h"

/*thread_info*/
thread_info::thread_info()
{
	bBeendet = false;
	bAnfrage_beenden = false;
}

bool thread_info::BeendenAngefragt(void)
{
	return bAnfrage_beenden;
}

void thread_info::BeendenAnfragen(void)
{
	bAnfrage_beenden = true;
	return;
}

bool thread_info::IstBeendet(void)
{
	return bBeendet;
}

void thread_info::BeendigungFeststellen(void)
{
	bBeendet = true;
	return;
}

void thread_info::logSchreiben(const char* msg, ...)
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
/*ENDE thread_info*/

/*thread_info_verschnitt*/
thread_info_verschnitt::thread_info_verschnitt(RUZ_Layer *_Lay1, RUZ_Layer *_Lay2)
{
	hilfsLayer = new RUZ_Layer("hilfsLayer");
	randLayer1 = new RUZ_Layer("randLayer1");
	randLayer2 = new RUZ_Layer("randLayer2");
	Layer1 = _Lay1;
	Layer2 = _Lay2;
	iBearbeitet = 0;
	iGes = 0;
}

thread_info_verschnitt::~thread_info_verschnitt()
{
	delete hilfsLayer;
	delete randLayer1;
	delete randLayer2;
	
	/*Hier noch Layer1 und Layer2 loeschen - vorher Layer_Auswahl_Dialog aendern (in RUZmiBIFrame immer neu erstellen!)*/
}

void thread_info_verschnitt::HoleLayer(RUZ_Layer** hl, RUZ_Layer** rl1, RUZ_Layer** rl2, RUZ_Layer** lay1, RUZ_Layer** lay2)
{
	*hl = hilfsLayer;
	*rl1 = randLayer1;
	*rl2 = randLayer2;
	*lay1 = Layer1;
	*lay2 = Layer2;
	return;
}

void thread_info_verschnitt::SetzeStatus(int i)
{
	iStatus = i;
	switch(iStatus)
	{
		case 0:
			grundMsg = "Layer verschneiden\n\n";
			grundMsg += "Verschneide ";
			grundMsg += Layer1->HoleName();
			grundMsg += ":";
			break;
		case 1:
			grundMsg += " erledigt\nSchneide ";
			grundMsg += Layer1->HoleName();
			grundMsg += " am Rand ab:";
			break;
		case 2:
			grundMsg += " erledigt\n";
			grundMsg += "Verschneide ";
			grundMsg += Layer2->HoleName();
			grundMsg += ":";
			break;
		case 3:
			grundMsg += " erledigt\nSchneide ";
			grundMsg += Layer2->HoleName();
			grundMsg += " am Rand ab:";
			break;
		case 4:
			grundMsg += " erledigt\n";
			break;
		default:
			grundMsg += "\n\nFEHLER\n";
	}
	return;
}

void thread_info_verschnitt::SetzeGesamtzahl(int wert)
{
	iGes = wert;
	return;
}

void thread_info_verschnitt::NulleBearbeitet(void)
{
	iBearbeitet = 0;
	return;
}

void thread_info_verschnitt::InkrementBearbeitet(void)
{
	iBearbeitet++;
	return;
}

std::string thread_info_verschnitt::HoleMeldung(void)
{
	std::string msg = grundMsg;
	switch(iStatus)
	{
		case 0:
		case 1:
			msg += " ... ";
			msg += std::to_string(iBearbeitet);
			msg += " / ";
			msg += std::to_string(iGes);
			msg += "    Punkte:	";
			msg += std::to_string(Layer1->HolePunkte()->GetListenGroesse());
			msg += "\n    Linien:	";
			msg += std::to_string(Layer1->HoleLinien()->GetListenGroesse());
			msg += "\n    Flächen:";
			msg += std::to_string(Layer1->HoleFlaechen()->GetListenGroesse());
			break;
		case 2:
		case 3:
			msg += " ... ";
			msg += std::to_string(iBearbeitet);
			msg += " / ";
			msg += std::to_string(iGes);
			msg += "    Punkte:	";
			msg += std::to_string(Layer2->HolePunkte()->GetListenGroesse());
			msg += "\n    Linien:	";
			msg += std::to_string(Layer2->HoleLinien()->GetListenGroesse());
			msg += "\n    Flächen:";
			msg += std::to_string(Layer2->HoleFlaechen()->GetListenGroesse());
			break;
		case 4:
			msg += "\nLösche freiliegende Flächen!\n    ";
			msg += Layer1->HoleName();
			msg += ": ";
			msg += std::to_string(Layer1->HoleFlaechen()->GetListenGroesse());
			msg += "\n    ";
			msg += Layer2->HoleName();
			msg += ": ";
			msg += std::to_string(Layer2->HoleFlaechen()->GetListenGroesse());
			break;
		default:
			msg += "\n\nFEHLER\n";
			break;
	}
	return msg;
}
/*ENDE thread_info_verschnitt*/

/*thread_info_ververnetzen*/
thread_info_vernetzen::thread_info_vernetzen(RUZ_Layer* lay)
{
	m_Layer = lay;
	m_anzVorhLinien = 0;
	m_anzNeueLinien = 0;
}

thread_info_vernetzen::~thread_info_vernetzen()
{
	if(BeendenAngefragt())m_Layer->UngeschuetzteLinienLoeschen();
}

void thread_info_vernetzen::SetzeStatus(int i)
{
	iStatus = i;
	grundMsg = "Punkte vernetzen:\n\n";

	switch(iStatus)
	{
		case 0:
			grundMsg += "Doppelte Punkte löschen...";
			break;
		case 1:
			grundMsg += "Doppelte Punkte löschen beendet.\n\n";
			grundMsg += "Punkteliste bereinigen...";
			break;
		case 2:
			grundMsg += "Doppelte Punkte löschen beendet.\n";
			grundMsg += "Punkteliste bereinigen beendet.\n\n";
			grundMsg += "Punkte untereinander verbinden:\n    ";
			break;
		case 3:
			grundMsg += "Doppelte Punkte löschen beendet.\n";
			grundMsg += "Punkteliste bereinigen beendet.\n\n";
			grundMsg += "Linien nach Länge sortieren...";
			break;
		case 4:
			grundMsg += "Doppelte Punkte löschen beendet.\n";
			grundMsg += "Punkteliste bereinigen beendet.\n";
			grundMsg += "Linien nach Länge sortieren beendet.\n\n";
			grundMsg += "Linien verschneiden\n    Linie ";
			break;
		default:
			grundMsg += "\n\nFEHLER\n";
			break;
	}
	return;
}

void thread_info_vernetzen::HoleLayer(RUZ_Layer** lay)
{
	*lay = m_Layer;
	return;
}

void thread_info_vernetzen::InkrVorhandeneLinie(void)
{
	m_anzVorhLinien++;
	return;
}

void thread_info_vernetzen::InkrNeueLinie(void)
{
	m_anzNeueLinien++;
	return;
}

void thread_info_vernetzen::BearbeiteteLinie(unsigned long long int Nr)
{
	m_aktLinieNr = Nr;
	return;
}

unsigned long long int thread_info_vernetzen::HoleBearbeitet(void)
{
	return m_aktLinieNr;
}

unsigned long long int thread_info_vernetzen::HoleVorhandeneLinien(void)
{
	return m_anzVorhLinien;
}

unsigned long long int thread_info_vernetzen::HoleNeueLinien(void)
{
	return m_anzNeueLinien;
}

std::string thread_info_vernetzen::HoleMeldung(void)
{
	std::string msg = grundMsg;
	switch(iStatus)
	{
		case 0:
		case 1:
			return grundMsg;
			break;
		case 2:
			msg += std::to_string(m_anzVorhLinien);
			msg += " geschützte Linien\n    ";
			msg += std::to_string(m_anzNeueLinien);
			msg += " neue Linien\n";
			break;
		case 3:
			return grundMsg;
			break;
		case 4:
			msg += std::to_string(m_aktLinieNr);
			msg += " von ";
			msg += std::to_string(m_Layer->HoleLinien()->GetListenGroesse());
			break;
		default:
			msg += "\n\nFEHLER\n";
	}
	return msg;
}
/*ENDE thread_info_ververnetzen*/

/*thread_info_integral*/
thread_info_integral::thread_info_integral()
{
}

thread_info_integral::~thread_info_integral()
{
	m_maxX = 0;
}

void thread_info_integral::SetVars(int *row, int *col, int minX, int maxX, int minY, int maxY)
{
	m_minX = minX;
	m_minY = minY;
	m_maxX = maxX - m_minX + 1;
	m_maxY = maxY - m_minY + 1;
	m_row = row;
	m_col = col;
	return;
}

void thread_info_integral::SetzeStatus(int i)
{
	grundMsg = "    ";
}

std::string thread_info_integral::HoleMeldung(void)
{
	std::string msg = grundMsg;
	if((m_maxX * m_maxY) == 0)
	{
		msg += "FEHLER!!";
		return msg;
	}
	int prozent = 100 * (float)((*m_row - m_minX + 1) * m_maxY + (*m_col - m_minY + 1)) / 
								(float)(m_maxX * m_maxY);
								
	int i;
	msg += "[";
	for(i = 0; i < prozent; i += 1)
	{
		msg += "|";
	}
	for(; i < 100; i += 1)
	{
		msg += ".";
	}
	msg += "] ";
	msg += std::to_string(prozent);
	msg += "%\n";
	
	return msg;
}
/*ENDE thread_info_integral*/
