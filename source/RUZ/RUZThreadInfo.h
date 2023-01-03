#ifndef __RUZ_THREADINFO_
#define __RUZ_THREADINFO_

class thread_info;
class thread_info_verschnitt;
#include "RUZVerwaltung.h"

class thread_info
{
private:
  bool bAnfrage_beenden, bBeendet;
public:
  thread_info();

  bool BeendenAngefragt(void);
  void BeendenAnfragen(void);
  bool IstBeendet(void);
  void BeendungFeststellen(void);
  void logSchreiben(const char* msg, ...);
};

class thread_info_verschnitt : public thread_info
{
private:
  RUZ_Layer* hilfsLayer;
  RUZ_Layer* randLayer1;
  RUZ_Layer* randLayer2;

  RUZ_Layer *Layer1, *Layer2;

  int iStatus = 0; /*Layer2 Rand abschneiden | Layer2 verschneiden | Layer1 Rand abschneiden | Layer1 verschneiden | Schniiilinien finden*/
  int iBearbeitet, iGes;
public:
  thread_info_verschnitt(RUZ_Layer*, RUZ_Layer*);
  ~thread_info_verschnitt();

  void HoleLayer(RUZ_Layer**, RUZ_Layer**, RUZ_Layer**, RUZ_Layer**, RUZ_Layer**);
  int HoleStatus(void);
  void SetzeStatus(int);
  void SetzeGesamtzahl(int wert);
  int HoleGesamtzahl(void);
  void NulleBearbeitet(void);
  void InkrementBearbeitet(void);
  int HoleBearbeitet(void);
};

#endif //__RUZ_THREADINFO_
