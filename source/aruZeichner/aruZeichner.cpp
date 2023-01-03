/*
diese Datei ist überholt
aktuelle Datei in Linien_Fraktale: aruZeichner
*/



#include "aruZeichner.h"
#include <fstream>
#include <iostream>
#include <cmath>

aruZeichner::aruZeichner(unsigned char* zeichenFlaeche, int hoehe, int breite)
{
    chZeichenFlaeche = zeichenFlaeche;
    iHoehe = hoehe;
    iBreite = breite;
    RGB_R = RGB_G = RGB_B = 128;
}

void aruZeichner::ZeichneLinie(int p1x, int p1y, int p2x, int p2y)
{
    int yMax, yMin, xMax, xMin;

    xMin = xMax = p1x;
    if(p2x > xMax)(xMax = p2x);
    if(p2x < xMin)(xMin = p2x);
    if(xMin < 0) xMin = 0;
    if(xMax < 0) xMax = 0;
    if(xMin > iBreite) xMin = iBreite;
    if(xMax > iBreite) xMax = iBreite;

    yMin = yMax = p1y;
    if(p2y > yMax)(yMax = p2y);
    if(p2y < yMin)(yMin = p2y);
    if(yMin < 0) yMin = 0;
    if(yMax < 0) yMax = 0;
    if(yMin > iHoehe) yMin = iHoehe;
    if(yMax > iHoehe) yMax = iHoehe;

    double dSteigung, dx, dy;

    dx = p2x - p1x;
    dy = p2y - p1y;

    int x, y;

    if(abs(dx) < abs(dy))
    {
        dSteigung = dx / dy;
        for(y = yMin; y < yMax; y++)
        {
            x = p1x + dSteigung * (y - p1y);
            if((x < iBreite)&&(x >= 0))
            {
                chZeichenFlaeche[(x + y * iBreite) * 3] = RGB_R;
                chZeichenFlaeche[(x + y * iBreite) * 3 + 1] = RGB_G;
                chZeichenFlaeche[(x + y * iBreite) * 3 + 2] = RGB_B;
            }
        }
    }else
    {
        dSteigung = dy/ dx;
        for(x = xMin; x < xMax; x++)
        {
            y = p1y + dSteigung * (x - p1x);
            if((y < iHoehe)&&(y >= 0))
            {
                chZeichenFlaeche[(x + y * iBreite) * 3] = RGB_R;
                chZeichenFlaeche[(x + y * iBreite) * 3 + 1] = RGB_G;
                chZeichenFlaeche[(x + y * iBreite) * 3 + 2] = RGB_B;
            }
        }
    }
    return;
}

void aruZeichner::SetzeFarbe(unsigned char r, unsigned char g, unsigned char b)
{
    RGB_R = r;
    RGB_G = g;
    RGB_B = b;

    return;
}

void aruZeichner::ZeichneDreieck(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y)
{
    int iXLinks, iXMitte, iXRechts;
    int iYLinks, iYMitte, iYRechts;
    /*Eckpunkte von links nach rechts ordnen*/
    iXLinks = iXMitte = iXRechts = p1x;
    iYLinks = iYMitte = iYRechts = p1y;
    if(p2x < iXLinks)
    {
        iXLinks = p2x;
        iYLinks = p2y;
    }
    else
    {
        iXMitte = iXRechts = p2x;
        iYMitte = iYRechts = p2y;
    }

    if(p3x < iXLinks)
    {
        if(iXLinks > iXRechts)
        {
            iXRechts = iXLinks;
            iYRechts = iYLinks;
        }
        else
        {
            iXMitte = iXLinks;
            iYMitte = iYLinks;
        }
        iXLinks = p3x;
        iYLinks = p3y;
    }else{
        if(p3x > iXRechts)
        {
            iXRechts = p3x;
            iYRechts = p3y;
        }
        else
        {
            iXMitte = p3x;
            iXMitte = p3y;
        }
    }
    /*ENDE Eckpunkte von links nach rechts ordnen*/
    if(iXRechts == iXLinks)return;

    /*Dreieck senkrecht in der Mitte durchschneiden*/
    int iYoben, iYunten;
    iYoben = iYLinks + (iYRechts - iYLinks) * (iXMitte - iXLinks) / (iXRechts - iXLinks);
    if(iYMitte > iYoben)
    {
        iYunten = iYoben;
        iYoben = iYMitte;
    }else
    {
        iYunten = iYMitte;
    }
    ZeichneGeradesDreieck(iXLinks, iYLinks, iYLinks, iXMitte, iYoben, iYunten);
    ZeichneGeradesDreieck(iXMitte, iYoben, iYunten, iXRechts, iYRechts, iYRechts);
    /*ENDE Dreieck senkrecht in der Mitte durchschneiden*/
    return;
}

void aruZeichner::ZeichneGeradesDreieck(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu)
{
    /*Abbruchkriterien - Dreieck au�erhalb*/
    if(iXL > iBreite)return;
    if(iXR < 0)return;
    if((iYLu > iHoehe)&&(iYRu > iHoehe))return;
    if((iYLo < 0)&&(iYRo < 0))return;
    if(iXL == iXR)
    {
        ZeichneLinie(iXL, iYLo, iXL, iYLu);
        return;
    }
    /*ENDE Abbruchkriterien - Dreieck au�erhalb*/

    /*Schnitte mit R�ndern finden*/
    int iAnzAbteile = 0;
    int iX[6], iYu[6], iYo[6];

    /*Lage linker Rand*/
    iX[iAnzAbteile] = iXL;
    iYu[iAnzAbteile] = iYLu;
    iYo[iAnzAbteile] = iYLo;
    if(iXL < 0)
    {
        iX[iAnzAbteile] = 0;
        iYu[iAnzAbteile] = iYLu + (iYRu - iYLu) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
        iYo[iAnzAbteile] = iYLo + (iYRo - iYLo) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
    }
    if(iYu[iAnzAbteile] < 0)iYu[iAnzAbteile] = 0;
    if(iYo[iAnzAbteile] >= iHoehe)iYo[iAnzAbteile] = iHoehe - 1;
    if(!(iYu[iAnzAbteile] >= iHoehe) && !(iYo[iAnzAbteile] < 0))iAnzAbteile++;

    /*obere Linie schneidet oberen Rand*/
    if((iHoehe - 1 - iYLo) * (iHoehe - 1 - iYRo) < 0)
    {
        iYo[iAnzAbteile] = iHoehe - 1;
        iX[iAnzAbteile] = iXL + (iXR - iXL) * (iYo[iAnzAbteile] - iYLo) / (float)(iYRo - iYLo);
        if(!(iX[iAnzAbteile] < 0) && !(iX[iAnzAbteile] >= iBreite))
        {
            iYu[iAnzAbteile] = iYLu + (iYRu - iYLu) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
            if(iYu[iAnzAbteile] < 0)iYu[iAnzAbteile] = 0;
            iAnzAbteile++;
        }
    }

    /*obere Linie schneidet unteren Rand*/
    if((0 - iYLo) * (0 - iYRo) < 0)
    {
        iYo[iAnzAbteile] = 0;
        iYu[iAnzAbteile] = 0;
        iX[iAnzAbteile] = iXL + (iXR - iXL) * (iYo[iAnzAbteile] - iYLo) / (float)(iYRo - iYLo);
        if(!(iX[iAnzAbteile] < 0) && !(iX[iAnzAbteile] >= iBreite))
        {
            iAnzAbteile++;
        }
    }

    /*untere Linie schneidet oberen Rand*/
    if((iHoehe - 1 - iYLu) * (iHoehe - 1 - iYRu) < 0)
    {
        iYu[iAnzAbteile] = iHoehe;
        iYo[iAnzAbteile] = iHoehe;
        iX[iAnzAbteile] = iXL + (iXR - iXL) * (iYu[iAnzAbteile] - iYLu) / (float)(iYRu - iYLu);
        if(!(iX[iAnzAbteile] < 0) && !(iX[iAnzAbteile] >= iBreite))
        {
            iAnzAbteile++;
        }
    }

    /*untere Linie schneidet unteren Rand*/
    if((0 - iYLu) * (0 - iYRu) < 0)
    {
        iYu[iAnzAbteile] = 0;
        iX[iAnzAbteile] = iXL + (iXR - iXL) * (iYu[iAnzAbteile] - iYLu) / (float)(iYRu - iYLu);
        if(!(iX[iAnzAbteile] < 0) && !(iX[iAnzAbteile] >= iBreite))
        {
            iYo[iAnzAbteile] = iYLo + (iYRo - iYLo) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
            if(iYo[iAnzAbteile] >= iHoehe)iYo[iAnzAbteile] = iHoehe - 1;
            iAnzAbteile++;
        }
    }

    /*Lage rechter Rand*/
    iX[iAnzAbteile] = iXR;
    iYu[iAnzAbteile] = iYRu;
    iYo[iAnzAbteile] = iYRo;
    if(iXR >= iBreite)
    {
        iX[iAnzAbteile] = iBreite - 1;
        iYu[iAnzAbteile] = iYLu + (iYRu - iYLu) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
        iYo[iAnzAbteile] = iYLo + (iYRo - iYLo) * (iX[iAnzAbteile] - iXL) / (float)(iXR - iXL);
    }
    if(iYu[iAnzAbteile] < 0)iYu[iAnzAbteile] = 0;
    if(iYo[iAnzAbteile] >= iHoehe)iYo[iAnzAbteile] = iHoehe - 1;
    if(!(iYu[iAnzAbteile] > iHoehe) && !(iYo[iAnzAbteile] < 0))iAnzAbteile++;
    /*ENDE Schnitte mit R�ndern finden*/

    /*Abteile sortieren*/
    int *iIndex = new int[iAnzAbteile];
    for(int i = 0; i < iAnzAbteile; i++)
    {
        iIndex[i] = i;
    }
    for(int i = 0; i < iAnzAbteile - 1; i++)
    {
        for(int k = i+1; k < iAnzAbteile; k++)
            {
                if(iX[iIndex[k]] < iX[iIndex[i]])
                {
                    int iTemp = iIndex[k];
                    iIndex[k] = iIndex[i];
                    iIndex[i] = iTemp;
                }
            }
    }
    /*ENDE Abteile sortieren*/

    /*Trapeze zeichnen*/
    for(int i = 0; i < iAnzAbteile - 1; i++)
    {
        ZeichneTrapezSenkrecht(iX[iIndex[i]], iX[iIndex[i+1]], iYu[iIndex[i]], iYu[iIndex[i+1]], iYo[iIndex[i]], iYo[iIndex[i+1]]);
    }
    /*ENDE Trapeze zeichnen*/
    return;
}

void aruZeichner::ZeichneTrapezSenkrecht(int iMinX, int iMaxX, int iMinYu, int iMaxYu, int iMinYo, int iMaxYo)
{
    if(iMaxX == iMinX)return;
    int iStelle;
    for(int iTempX = iMinX; iTempX < iMaxX; iTempX++)
    {
        int iLocMinY = iMinYu + (iMaxYu - iMinYu) * (iTempX - iMinX) / (iMaxX - iMinX);
        int iLocMaxY = iMinYo + (iMaxYo - iMinYo) * (iTempX - iMinX) / (iMaxX - iMinX);
        for(int iTempY = iLocMinY; iTempY < iLocMaxY; iTempY++)
        {
            iStelle = (iTempX + iTempY * iBreite)* 3;
            if((iStelle >= 0)&&(iStelle < (iHoehe * iBreite * 3)))
            {
                chZeichenFlaeche[iStelle] = RGB_R;
                chZeichenFlaeche[iStelle + 1] = RGB_G;
                chZeichenFlaeche[iStelle + 2] = RGB_B;
            }
        }
    }
    return;
}

void aruZeichner::HintergrundZeichnen(unsigned char r, unsigned char g, unsigned char b)
{
    for(int i = 0; i < iBreite * iHoehe * 3; i += 3)
    {
        chZeichenFlaeche[i] = r;
        chZeichenFlaeche[i + 1] = g;
        chZeichenFlaeche[i + 2] = b;
    }
    return;
}

void aruZeichner::logSchreiben(const char* msg)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruZeichner.log";
    Logbuch.open(pfad ,std::ios_base::out|std::ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

void aruZeichner::logSchreiben(const float msg, int i)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruZeichner.log";
    Logbuch.open(pfad ,std::ios_base::out|std::ios_base::app);
    if(Logbuch.good())
    {
        Logbuch.setf( std::ios::fixed, std::ios::floatfield );
        Logbuch.precision(i);
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

void aruZeichner::logSchreiben(const void *msg)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruZeichner.log";
    Logbuch.open(pfad ,std::ios_base::out|std::ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}
