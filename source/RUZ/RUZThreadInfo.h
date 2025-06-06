#ifndef __RUZ_THREADINFO_
#define __RUZ_THREADINFO_

class thread_info;
class thread_info_verschnitt;
class thread_info_vernetzen;
#include "RUZVerwaltung.h"
#include <string>
#include <time.h>

class thread_info
{
protected:
	bool bAnfrage_beenden, bBeendet;
	int iStatus = 0;
	std::string grundMsg;
public:
	thread_info();

	bool BeendenAngefragt(void);
	void BeendenAnfragen(void);
	bool IstBeendet(void);
	void BeendigungFeststellen(void);
	virtual void SetzeStatus(int) = 0;
	virtual std::string HoleMeldung(void) = 0;
	void logSchreiben(const char* msg, ...);
};

class thread_info_verschnitt : public thread_info
{
	/*iStatus:	1=Layer2 Rand abschneiden
				2=Layer2 verschneiden
				3=Layer1 Rand abschneiden
				4=Layer1 verschneiden
				5=Schnittlinien finden*/
private:
	RUZ_Layer* hilfsLayer;
	RUZ_Layer* randLayer1;
	RUZ_Layer* randLayer2;

	RUZ_Layer *Layer1, *Layer2;
	int iBearbeitet, iGes;
public:
	thread_info_verschnitt(RUZ_Layer*, RUZ_Layer*);
	~thread_info_verschnitt();
	
	void HoleLayer(RUZ_Layer**, RUZ_Layer**, RUZ_Layer**, RUZ_Layer**, RUZ_Layer**);
	void SetzeGesamtzahl(int wert);
	void NulleBearbeitet(void);
	void InkrementBearbeitet(void);

	virtual void SetzeStatus(int);
	virtual std::string HoleMeldung(void);
};

class thread_info_vernetzen : public thread_info
{
	/*iStatus:	0=Doppelte Punkte loeschen
				1=Layerfremde Punkte entfernen
				2=Punkte mit Linien verbinden
				3= Linien nach Laenge sortieren
				4=von kurzen Linien geschnittene Linien loeschen*/
private:
	RUZ_Layer* m_Layer;
	unsigned long long int m_anzVorhLinien, m_anzNeueLinien, m_aktLinieNr;
	clock_t m_schnittZeit, m_loeschZeit;
public:
	thread_info_vernetzen(RUZ_Layer* lay);
	~thread_info_vernetzen();

	void HoleLayer(RUZ_Layer**);
	void InkrVorhandeneLinie(void);
	void InkrNeueLinie(void);
	void DekrNeueLinie(void);
	void BearbeiteteLinie(unsigned long long int Nr);
	void SchnittZeit(clock_t sz);
	void LoeschZeit(clock_t lz);
	unsigned long long int HoleBearbeitet(void);
	unsigned long long int HoleVorhandeneLinien(void);
	unsigned long long int HoleNeueLinien(void);

	virtual void SetzeStatus(int);
	virtual std::string HoleMeldung(void);
};

class thread_info_integral : public thread_info
{
	/*iStatus:	0=
				1=
				2=
				3=
				4=*/
private:
	int m_minX, m_maxX, m_minY, m_maxY;
	int m_x, m_y;
public:
	thread_info_integral();
	~thread_info_integral();

	virtual void SetzeStatus(int);
	virtual std::string HoleMeldung(void);

	void SetzeGrenzen(int minX, int maxX, int minY, int maxY);
	void SetzeX(int x);
	void SetzeY(int y);
};

#endif //__RUZ_THREADINFO_
