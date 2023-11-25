/*TODO: Alphakanal?*/

#ifndef ARUINTEGRAL_HPP_INCLUDED
#define ARUINTEGRAL_HPP_INCLUDED

#include "../RUZ/RUZObjekte.h"

class aruIntegral
{
public:
    aruIntegral(double *Integral, double dStartX, double dStartY, double dEndeX, double dEndeY, double Aufloesung, Achse Projektion);
    ~aruIntegral();
    void IntegralNullen(void);
    void IntegriereFlaeche(Flaeche* obj);
    void IntegriereFlaeche(Dreieck* obj);
    void IntegriereFlaeche(Viereck* obj);

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

    void ZeichneDreieckSenkrecht(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu);
    void ZeichneGeradesDreieck(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu, Flaeche* obj);
    void ZeichneTrapezSenkrecht(int iMinX, int iMaxX, int iMinYu, int iMaxYu, int iMinYo, int iMaxYo, Flaeche* obj);
    void ZeichneLinie(int p1x, int p1y, int p2x, int p2y, Flaeche* obj);
	
    void ZeichneLinie(int p1x, int p1y, int p2x, int p2y, int p1z, int p2z);
    /*Logbuch*/
    void logSchreiben(const char*);
    void logSchreiben(const float msg, int i=0);
    void logSchreiben(const void *msg);
    /*ENDE Logbuch*/
};


#endif // ARUINTEGRAL_HPP_INCLUDED
