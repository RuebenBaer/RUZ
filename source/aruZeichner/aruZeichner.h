/*TODO: Alphakanal?*/

#ifndef ARUZEICHNER_HPP_INCLUDED
#define ARUZEICHNER_HPP_INCLUDED

class aruZeichner
{
public:
    aruZeichner(unsigned char*, int, int);
    void HintergrundZeichnen(unsigned char, unsigned char, unsigned char);
    void ZeichneLinie(int, int, int, int);
    void ZeichneDreieck(int, int, int, int, int, int);
    void ZeichneKreis(int xm, int ym, int radius);
    void SetzeFarbe(unsigned char, unsigned char, unsigned char);
private:
    unsigned char *chZeichenFlaeche;
    unsigned char RGB_R, RGB_G, RGB_B;
    int iBreite, iHoehe;

    void ZeichneDreieckSenkrecht(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu);

    void ZeichneGeradesDreieck(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu);
    void ZeichneTrapezSenkrecht(int iMinX, int iMaxX, int iMinYu, int iMaxYu, int iMinYo, int iMaxYo);
    /*Logbuch*/
    void logSchreiben(const char*);
    void logSchreiben(const float msg, int i=0);
    void logSchreiben(const void *msg);
    /*ENDE Logbuch*/
};


#endif // ARUZEICHNER_HPP_INCLUDED
