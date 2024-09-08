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

void aruIntegral::thIntegriereFlaeche(Flaeche* obj, thread_info_integral *thInf)
{
	if(obj->HoleTyp() == RUZ_Dreieck)thIntegriereFlaeche(static_cast<Dreieck*>(obj), thInf);
	else thIntegriereFlaeche(static_cast<Viereck*>(obj), thInf);
	return;
}

void aruIntegral::thIntegriereFlaeche(Dreieck *obj, thread_info_integral *thInf)
{
	int x, y;
	thInf->SetVars(&x, &y, 0, 0, 0, 0);
	
	int px[3], py[3];
	unsigned long long int iStelle;

	Vektor vOrt;
	for(int i = 0; i < 3; i++)
	{
		vOrt = obj->HolePunkt(i)->HolePosition();
		px[i] = (int)(vOrt.GetKoordinaten((aProjektion + 1)%3) / dAufloesung + 0.5) - iOffsetBreite;
		py[i] = (int)(vOrt.GetKoordinaten((aProjektion + 2)%3) / dAufloesung + 0.5) - iOffsetHoehe;
	}

	int minX, maxX, minY, maxY;
	minX = maxX = px[0];
	minY = maxY = py[0];
	for(int i = 1; i < 3; i++)
	{
		if(px[i] < minX)minX = px[i];
		if(px[i] > maxX)maxX = px[i];
		if(py[i] < minY)minY = py[i];
		if(py[i] > maxY)maxY = py[i];
	}
	if(--minX < 0)minX = 0;
	if(++maxX > iBreite-1)maxX = iBreite-1;
	if(--minY < 0)minY = 0;
	if(++maxY > iHoehe-1)maxY = iHoehe-1;

	Vektor vkt;
	double wert;

	thInf->SetVars(&x, &y, minX, maxX, minY, maxY);
	
	for(x = minX; x < maxX+1; x++)
	{
		if(thInf->BeendenAngefragt())//Abbruch angefragt
		{
			thInf->BeendigungFeststellen();
			return;
		}
		for(y = minY; y < maxY+1; y++)
		{			
			if(thInf->BeendenAngefragt())//Abbruch angefragt
			{
				thInf->BeendigungFeststellen();
				return;
			}
			iStelle = x + y * iBreite;
			vkt.SetKoordinaten((aProjektion+1)%3, ((float)x + iOffsetBreite)*dAufloesung);
			vkt.SetKoordinaten((aProjektion+2)%3, ((float)y + iOffsetHoehe)*dAufloesung);
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
	return;
}

void aruIntegral::thIntegriereFlaeche(Viereck *obj, thread_info_integral *thInf)
{
	int x, y;
	thInf->SetVars(&x, &y, 0, 0, 0, 0);
	
	int px[4], py[4];
	unsigned long long int iStelle;
	
	Vektor vOrt;
	for(int i = 0; i < 4; i++)
	{
		vOrt = obj->HolePunkt(i)->HolePosition();
		px[i] = (int)(vOrt.GetKoordinaten((aProjektion + 1)%3) / dAufloesung + 0.5) - iOffsetBreite;
		py[i] = (int)(vOrt.GetKoordinaten((aProjektion + 2)%3) / dAufloesung + 0.5) - iOffsetHoehe;
	}

	int minX, maxX, minY, maxY;
	minX = maxX = px[0];
	minY = maxY = py[0];
	for(int i = 1; i < 4; i++)
	{
		if(px[i] < minX)minX = px[i];
		if(px[i] > maxX)maxX = px[i];
		if(py[i] < minY)minY = py[i];
		if(py[i] > maxY)maxY = py[i];
	}
	if(--minX < 0)minX = 0;
	if(++maxX > iBreite-1)maxX = iBreite-1;
	if(--minY < 0)minY = 0;
	if(++maxY > iHoehe-1)maxY = iHoehe-1;

	Vektor vkt;
	double wert;

	thInf->SetVars(&x, &y, minX, maxX, minY, maxY);
	
	for(x = minX; x < maxX+1; x++)
	{
		if(thInf->BeendenAngefragt())//Abbruch angefragt
		{
			thInf->BeendigungFeststellen();
			return;
		}
		for(y = minY; y < maxY+1; y++)
		{
			if(thInf->BeendenAngefragt())//Abbruch angefragt
			{
				thInf->BeendigungFeststellen();
				return;
			}
			iStelle = x + y * iBreite;
			vkt.SetKoordinaten((aProjektion+1)%3, ((float)x + iOffsetBreite)*dAufloesung);
			vkt.SetKoordinaten((aProjektion+2)%3, ((float)y + iOffsetHoehe)*dAufloesung);
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
