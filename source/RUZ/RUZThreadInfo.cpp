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

int thread_info::HoleStatus(void)
{
	return iStatus;
}

void thread_info::SetzeStatus(int i)
{
	iStatus = i;
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
}

void thread_info_verschnitt::HoleLayer(RUZ_Layer** hl, RUZ_Layer** rl1, RUZ_Layer** rl2,	RUZ_Layer** lay1, RUZ_Layer** lay2)
{
	*hl = hilfsLayer;
	*rl1 = randLayer1;
	*rl2 = randLayer2;
	*lay1 = Layer1;
	*lay2 = Layer2;
	return;
}

void thread_info_verschnitt::SetzeGesamtzahl(int wert)
{
	iGes = wert;
	return;
}

int thread_info_verschnitt::HoleGesamtzahl(void)
{
	return iGes;
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

int thread_info_verschnitt::HoleBearbeitet(void)
{
	return iBearbeitet;
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
/*ENDE thread_info_ververnetzen*/
