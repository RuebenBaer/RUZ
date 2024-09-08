/*TODO: Alphakanal?*/

#ifndef ARUINTEGRAL_HPP_INCLUDED
#define ARUINTEGRAL_HPP_INCLUDED

#include "../RUZ/RUZObjekte.h"
#include "../RUZ/RUZThreadInfo.h"

class aruIntegral
{
public:
    aruIntegral(double *Integral, double dStartX, double dStartY, double dEndeX, double dEndeY, double Aufloesung, Achse Projektion);
    ~aruIntegral();
    void IntegralNullen(void);
    void thIntegriereFlaeche(Flaeche* obj, thread_info_integral *thInf);
    void thIntegriereFlaeche(Dreieck* obj, thread_info_integral *thInf);
    void thIntegriereFlaeche(Viereck* obj, thread_info_integral *thInf);

    int HoleHoehe(void)const;
    int HoleBreite(void)const;
    int HoleOffsetHoehe(void)const;
    int HoleOffsetBreite(void)const;
    double* HoleIntegral(void);
private:
    double *dIntegral;
    double dAufloesung;
    Flaeche *flObj;
    int iBreite, iHoehe, iOffsetHoehe, iOffsetBreite;
    Achse aProjektion;

    /*Logbuch*/
    void logSchreiben(const char*);
    void logSchreiben(const float msg, int i=0);
    void logSchreiben(const void *msg);
    /*ENDE Logbuch*/
};


#endif // ARUINTEGRAL_HPP_INCLUDED
