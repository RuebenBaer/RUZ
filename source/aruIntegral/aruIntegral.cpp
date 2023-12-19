#include "aruIntegral.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <new>
#include "../RUZ/RUZObjekte.h"

aruIntegral::aruIntegral(double *Integral, double dStartX, double dStartY, double dEndeX, double dEndeY, double Aufloesung, Achse Projektion)
{    
	dAufloesung = Aufloesung;
    aProjektion = Projektion;
    if(dAufloesung)
    {
        iBreite = (dEndeX - dStartX)  / dAufloesung + 0.5;
        iHoehe = (dEndeY - dStartY)  / dAufloesung + 0.5;
        iOffsetBreite = dStartX / dAufloesung + 0.5;
        iOffsetHoehe = dStartY / dAufloesung + 0.5;
    }else{
        iHoehe = 0;
        iBreite = 0;
        iOffsetHoehe = 0;
        iOffsetBreite = 0;
    }
	std::cout<<"Hoehe * Breite = "<<iHoehe<<" * "<<iBreite<<" = "<<iHoehe * iBreite<<"\n"<<flush;
	dIntegral = Integral = NULL;
	try{
		dIntegral = Integral = new double[iHoehe*iBreite];
	}
	catch(std::bad_alloc &ba)
	{
		std::cerr<<"Bad Allocation: "<<ba.what()<<"\n";
		throw ba;
	}
	catch(...)
	{
		throw "KEIN Integral angelegt!\n";
	}

	if(dIntegral == NULL)
    {
		std::cerr<<"KEIN Integral angelegt!\n";
		throw "KEIN Integral angelegt!\n";
    }
    if(dIntegral)
    {
        IntegralNullen();
    }
}

aruIntegral::~aruIntegral()
{
    if(dIntegral)delete []dIntegral;
}

void aruIntegral::IntegriereFlaeche(Flaeche* obj)
{
    if(obj->HoleTyp() == RUZ_Dreieck)IntegriereFlaeche(static_cast<Dreieck*>(obj));
    else IntegriereFlaeche(static_cast<Viereck*>(obj));
    return;
}

void aruIntegral::IntegriereFlaeche(Dreieck *obj)
{
    int px[3], py[3];
    {
        Vektor vOrt;
        for(int i = 0; i < 3; i++)
        {
            vOrt = obj->HolePunkt(i)->HolePosition();
            px[i] = (int)(vOrt.GetKoordinaten((aProjektion + 1)%3) / dAufloesung + 0.5) - iOffsetBreite;
            py[i] = (int)(vOrt.GetKoordinaten((aProjektion + 2)%3) / dAufloesung + 0.5) - iOffsetHoehe;
        }
    }
    int iXLinks, iXMitte, iXRechts;
    int iYLinks, iYMitte, iYRechts;
    /*Eckpunkte von links nach rechts ordnen*/
    iXLinks = iXMitte = iXRechts = px[0];
    iYLinks = iYMitte = iYRechts = py[0];
    if(px[1] < iXLinks)
    {
        iXLinks = px[1];
        iYLinks = py[1];
    }
    else
    {
        iXMitte = iXRechts = px[1];
        iYMitte = iYRechts = py[1];
    }

    if(px[2] < iXLinks)
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
        iXLinks = px[2];
        iYLinks = py[2];
    }else{
        if(px[2] > iXRechts)
        {
            iXRechts = px[2];
            iYRechts = py[2];
        }
        else
        {
            iXMitte = px[2];
            iYMitte = py[2];
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
	ZeichneGeradesDreieck(iXLinks, iYLinks, iYLinks, iXMitte, iYoben, iYunten, obj);
    ZeichneGeradesDreieck(iXMitte, iYoben, iYunten, iXRechts, iYRechts, iYRechts, obj);
    /*ENDE Dreieck senkrecht in der Mitte durchschneiden*/
    return;
}

void aruIntegral::IntegriereFlaeche(Viereck *obj)
{
    int px[4], py[4];
    {
        Vektor vOrt;
        for(int i = 0; i < 4; i++)
        {
            vOrt = obj->HolePunkt(i)->HolePosition();
            px[i] = (int)(vOrt.GetKoordinaten((aProjektion + 1)%3) / dAufloesung) - iOffsetBreite;
            py[i] = (int)(vOrt.GetKoordinaten((aProjektion + 2)%3) / dAufloesung) - iOffsetHoehe;
        }
    }
    int iYo[4], iYu[4], iIndex[4];
    /*Abteile sortieren*/
    for(int i = 0; i < 4; i++)
    {
        iIndex[i] = i;
    }
    for(int i = 0; i < 4 - 1; i++)
    {
        for(int k = i+1; k < 4; k++)
        {
            if(px[iIndex[k]] < px[iIndex[i]])
            {
                int iTemp = iIndex[k];
                iIndex[k] = iIndex[i];
                iIndex[i] = iTemp;
            }
        }
    }
    /*ENDE Abteile sortieren*/
    iYo[0] = iYu[0] = py[iIndex[0]];
    iYo[3] = iYu[3] = py[iIndex[3]];
	
	iYo[2] = 0;

    if(iIndex[1] == (iIndex[2]+2)%4)
    {
        if(px[iIndex[2]] != px[iIndex[0]])
        {
            iYo[1] = iYu[1] = py[iIndex[0]] + (py[iIndex[2]] - py[iIndex[0]]) * (px[iIndex[1]] - px[iIndex[0]]) / (px[iIndex[2]] - px[iIndex[0]]);
        }else
        {
            logSchreiben("px2 == px0!\n");
            iYo[1] = iYu[1] = py[iIndex[0]];
        }
        if(px[iIndex[3]] != px[iIndex[1]])
        {
            iYo[2] = iYu[2] = py[iIndex[1]] + (py[iIndex[3]] - py[iIndex[1]]) * (px[iIndex[2]] - px[iIndex[1]]) / (px[iIndex[3]] - px[iIndex[1]]);
        }else
        {
            logSchreiben("px3 == px1!\n");
            iYo[2] = iYu[2] = py[iIndex[1]];
        }
    }else
    {
        if(px[iIndex[3]] != px[iIndex[0]])
        {
            iYo[1] = iYu[1] = py[iIndex[0]] + (py[iIndex[3]] - py[iIndex[0]]) * (px[iIndex[1]] - px[iIndex[0]]) / (px[iIndex[3]] - px[iIndex[0]]);
            iYo[2] = iYu[2] = py[iIndex[0]] + (py[iIndex[3]] - py[iIndex[0]]) * (px[iIndex[2]] - px[iIndex[0]]) / (px[iIndex[3]] - px[iIndex[0]]);
        }else
        {
            logSchreiben("px2 == px0!\n");
            iYo[1] = iYu[1] = py[iIndex[0]];
        }
    }

    if(py[iIndex[1]] > iYo[1])
    {
        iYo[1] = py[iIndex[1]];
    }else
    {
        iYu[1] = py[iIndex[1]];
    }

    if(py[iIndex[2]] > iYo[2])
    {
        iYo[2] = py[iIndex[2]];
    }
    else
    {
        iYu[2] = py[iIndex[2]];
    }

    for(int i = 0; i < 3; i++)
    {
        ZeichneGeradesDreieck(px[iIndex[i]], iYo[i], iYu[i], px[iIndex[i+1]], iYo[i+1], iYu[i+1], obj);
    }
    return;
}

void aruIntegral::ZeichneGeradesDreieck(int iXL, int iYLo, int iYLu, int iXR, int iYRo, int iYRu, Flaeche* obj)
{
    /*Abbruchkriterien - Flaeche ausserhalb*/
    if(iXL >= iBreite)return;
    if(iXR < 0)return;
    if((iYLu >= iHoehe)&&(iYRu >= iHoehe))return;
    if((iYLo < 0)&&(iYRo < 0))return;
    if(iXL == iXR)
    {
        ZeichneLinie(iXL, iYLo, iXL, iYLu, obj);
        return;
    }
    /*ENDE Abbruchkriterien - Flaeche ausserhalb*/

    /*Schnitte mit Raendern finden*/
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
    /*ENDE Schnitte mit Raendern finden*/

    /*Abteile sortieren*/
	int *iIndex;
	try{
		iIndex = new int[iAnzAbteile];
	}
    catch(std::bad_alloc &ba)
	{
		std::cout<<ba.what()<<" => aruIntegral::ZeichneGeradesDreieck: iIndex konnte nicht instanziert werden\n";
		return;
	}
	if(!iIndex)
	{
		std::cerr<<"aruIntegral::ZeichneGeradesDreieck: iIndex konnte nicht instanziert werden\n";
		return;
	}
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
    for(int i = 0; i < iAnzAbteile-1; i++)
    {
        ZeichneTrapezSenkrecht(iX[iIndex[i]]-1, iX[iIndex[i+1]]+1, iYu[iIndex[i]]-1, iYu[iIndex[i+1]]-1, iYo[iIndex[i]]+1, iYo[iIndex[i+1]]+1, obj);
    }
    delete []iIndex;
    /*ENDE Trapeze zeichnen*/
    return;
}

void aruIntegral::ZeichneTrapezSenkrecht(int iMinX, int iMaxX, int iMinYu, int iMaxYu, int iMinYo, int iMaxYo, Flaeche* obj)
{
	if(iMaxX == iMinX)return;
    unsigned long long int iStelle;
	
	if(iMinX < 0)iMinX = 0;
	if(iMaxX > iBreite)iMaxX = iBreite;
	
	double wert = 0;

    for(unsigned long long int iTempX = iMinX; iTempX < (unsigned long long int)iMaxX; iTempX++)
    {
        unsigned long long int iLocMinY = iMinYu + (iMaxYu - iMinYu) * (((float)iTempX - iMinX) / (float)(iMaxX - iMinX));
		if(iLocMinY < 0)iLocMinY = 0;
		
        unsigned long long int iLocMaxY = iMinYo + (iMaxYo - iMinYo) * (((float)iTempX - iMinX) / (float)(iMaxX - iMinX));
		if(iLocMaxY > (unsigned long long int)iHoehe)iLocMaxY = iHoehe;
        for(unsigned long long int iTempY = iLocMinY; iTempY < iLocMaxY; iTempY++)
        {
            iStelle = iTempX + iTempY * iBreite;
            {
                //wert = obj->OrdinateAufEbene((iTempX + iOffsetBreite)*dAufloesung, (iTempY + iOffsetHoehe)*dAufloesung, aProjektion);
				Vektor vkt;
				vkt.SetKoordinaten((aProjektion+1)%3, ((float)iTempX + iOffsetBreite)*dAufloesung);
				vkt.SetKoordinaten((aProjektion+2)%3, ((float)iTempY + iOffsetHoehe)*dAufloesung);
				vkt.SetKoordinaten(aProjektion%3, 0);
				if(obj->OrtAufFlaeche(vkt, aProjektion, true))
				{
					wert = vkt.GetKoordinaten(aProjektion%3);
				}else
				{
					continue;
				}
				if(iStelle >= (unsigned long long int)(iHoehe * iBreite))
				{
					continue;
				}
				if(iStelle < 0)
				{
					continue;
				}
                dIntegral[iStelle] = wert;
            }
        }
    }
    return;
}

void aruIntegral::ZeichneLinie(int p1x, int p1y, int p2x, int p2y, Flaeche* obj)
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
				/*ggf. gegen OrtAufFlaeche tauschen*/
                dIntegral[x + y * iBreite] = obj->OrdinateAufEbene((x + iOffsetBreite)*dAufloesung, (y + iOffsetHoehe)*dAufloesung, aProjektion);
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
				/*ggf. gegen OrtAufFlaeche tauschen*/
                dIntegral[x + y * iBreite] = obj->OrdinateAufEbene((x + iOffsetBreite)*dAufloesung, (y + iOffsetHoehe)*dAufloesung, aProjektion);
            }
        }
    }
    return;
}

void aruIntegral::ZeichneLinie(int p1x, int p1y, int p2x, int p2y, int p1z, int p2z)
{
	return;
}

void aruIntegral::IntegralNullen(void)
{
    for(int i = 0; i < iBreite * iHoehe; i ++)
    {
		dIntegral[i] = NAN;
    }
    return;
}

int aruIntegral::HoleHoehe(void)const
{
    return iHoehe;
}

int aruIntegral::HoleBreite(void)const
{
    return iBreite;
}

int aruIntegral::HoleOffsetHoehe(void)const
{
    return iOffsetHoehe;
}

int aruIntegral::HoleOffsetBreite(void)const
{
    return iOffsetBreite;
}

double* aruIntegral::HoleIntegral(void)
{
    return dIntegral;
}

void aruIntegral::logSchreiben(const char* msg)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruIntegral.log";
    Logbuch.open(pfad ,std::ios_base::out|std::ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

void aruIntegral::logSchreiben(const float msg, int i)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruIntegral.log";
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

void aruIntegral::logSchreiben(const void *msg)
{
    std::ofstream Logbuch;
    const char *pfad = "log/aruIntegral.log";
    Logbuch.open(pfad ,std::ios_base::out|std::ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}
