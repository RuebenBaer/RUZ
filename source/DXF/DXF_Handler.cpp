#include "DXF_Handler.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>
#include "..\RUZ\RUZVerwaltung.h"
#include "..\RUZ\RUZObjekte.h"

#define PI 3.14159265358979

using namespace std;

double aru_strtod(char* str)
{
    int iNachkomma = 0;
    bool bZahlBegonnen = false;
    bool bNegativ = false;
    double dVorkommaFkt = 10;
    double dErgebnis = NAN;
    double dSummand;
    while(*str!='\0')
    {
        if(*str=='-')
        {
            bNegativ = true;
        }
        if((*str=='.')||(*str==','))
        {
            DXF_Handler::logSchreiben("Komma gefunden\n");
            if(iNachkomma > 0)
            {
                return NAN;
            }
            iNachkomma = 1;
            dVorkommaFkt = 1;
        }else
        if((*str<'0')||(*str>'9'))
        {
            if(bZahlBegonnen)
            {
                DXF_Handler::logSchreiben("Zeichen in Zahl - ABBRUCH\n");
                return NAN;
            }
        }else
        {
            if(bZahlBegonnen == false)
            {
                bZahlBegonnen = true;
                dErgebnis = *str - '0';
            }
            else
            {
                dSummand = (*str - '0') * pow(0.1, iNachkomma);
                dErgebnis *= dVorkommaFkt;
                dErgebnis += dSummand;

                iNachkomma += (iNachkomma > 0);
                if(iNachkomma > 15)
                {
                    return (dErgebnis * (1-2*bNegativ));
                }
            }
        }
        str++;
    };
    return (dErgebnis * (1-2*bNegativ));
}

void DXF_Handler::logSchreiben(const char* msg, ...)
{
    FILE *Logbuch;
    const char *pfad = "log/DXF_Handler.log";
    Logbuch = fopen(pfad, "a");
    va_list args;
    va_start (args, msg);
    vfprintf (Logbuch, msg, args);
    va_end (args);
    fclose(Logbuch);
    return;
}

/*Char_Speicher*/
Char_Speicher::Char_Speicher(const char* str)
{
    strncpy(m_inhalt, str, m_char_MAX-1);
    m_inhalt[m_char_MAX-1] = '\0';
}
/*ENDE Char_Speicher*/

/*DXF_Import*/
DXF_Import::DXF_Import(char* DatName, char* XKenn, char* YKenn, char* ZKenn, char* SW)
{
    strcpy(Kennung_X, XKenn);
    strcpy(Kennung_Y, YKenn);
    strcpy(Kennung_Z, ZKenn);
    strcpy(Schluesselwort, SW);
    strcpy(DateiName, DatName);

    layerLst = NULL;
    namensLst = NULL;
    einzelLayer = NULL;
}

DXF_Import::~DXF_Import()
{
    if(namensLst)delete namensLst;
}

Liste<Char_Speicher>* DXF_Import::HoleLayerNamen(void)
{
    namensLst = new Liste<Char_Speicher>;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("Sucht Layernamen in %s...\n", DateiName);

    if(Datei.good())
    {
        Char_Speicher* str;
        SucheEntitieSection();

        while(!Datei.eof())
        {
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.getline(zeile, m_char_MAX-1);
            while(Datei.peek()=='\n')Datei.ignore();
            if(strcmp(zeile, "  8")==0)
            {/*Layername*/
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.get(zeile, m_char_MAX-1);
                while (Datei.peek()=='\n')Datei.ignore();
                bool vorhanden = false;
                for(Char_Speicher* laeufer = namensLst->GetErstesElement(); laeufer != NULL; laeufer = namensLst->GetNaechstesElement())
                {
                    if(strcmp(laeufer->HoleInhalt(), zeile)==0)
                    {
                        vorhanden = true;
                        break;
                    }
                }
                if(!vorhanden)
                {
                    str = new Char_Speicher(zeile);
                    namensLst->Hinzufuegen(str);
                }
            }
            if(strcmp(zeile, "ENDSEC")==0)break;
            if(Datei.fail())break;
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
    }
    return namensLst;
}

bool DXF_Import::EinlesenPunkte(Liste<RUZ_Layer>* layLst, Liste<Char_Speicher>* namLst)
{
    layerLst = layLst;
    namensLst = namLst;
    einzelLayer = NULL;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("Sucht Punkte in %s...\n", DateiName);

    if(Datei.good())
    {
        SucheEntitieSection();

        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        while(!Datei.eof())
        {
            if(strcmp(zeile, "  0")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, Schluesselwort)==0)/*Punkt gefunden*/
                {
                    LesePunkt();
                }
                if(strcmp(zeile, "ENDSEC")==0)break;
            }else{
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
            }
            if(Datei.fail())
            {
                Datei.close();
                if(Datei.bad())
                {
                    DXF_Handler::logSchreiben("badbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }
                else
                {
                    DXF_Handler::logSchreiben("failbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }

                return false;
            }
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
        return false;
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
        return true;
    }
    return true;
}

bool DXF_Import::EinlesenPunkteKanten(Liste<RUZ_Layer>* layLst, Liste<Char_Speicher>* namLst)
{
    layerLst = layLst;
    namensLst = namLst;
    einzelLayer = NULL;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("Sucht Punkte und Kanten in %s...\n", DateiName);

    if(Datei.good())
    {
        SucheEntitieSection();

        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        while(!Datei.eof())
        {
            if(strcmp(zeile, "  0")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, Schluesselwort)==0)/*Punkt gefunden*/
                {
                    LesePunkt();
                }
                if(strcmp(zeile, "LINE")==0)/*Linie gefunden*/
                {
                    LeseStrich();
                }
                if(strcmp(zeile, "POLYLINE")==0)/*Polylinie gefunden*/
                {
                    LesePolyLinie();
                }
                if(strcmp(zeile, "3DFACE")==0)/*Flaeche gefunden*/
                {
                    LeseFlaeche();
                }
                if(strcmp(zeile, "ENDSEC")==0)break;
            }else{
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
            }
            if(Datei.fail())
            {
                Datei.close();
                if(Datei.bad())
                {
                    DXF_Handler::logSchreiben("badbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }
                else
                {
                    DXF_Handler::logSchreiben("failbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }

                return false;
            }
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
        return false;
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
        return true;
    }
    return true;
}

bool DXF_Import::EinlesenPunkte(RUZ_Layer* layer, Liste<Char_Speicher>* namLst)
{
    layerLst = NULL;
    namensLst = namLst;
    einzelLayer = layer;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("Sucht Punkte in %s ohne Liste...", DateiName);

    if(Datei.good())
    {
        SucheEntitieSection();

        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        while(!Datei.eof())
        {
            if(strcmp(zeile, "  0")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, Schluesselwort)==0)/*Punkt gefunden*/
                {
                    LesePunkt();
                }
                if(strcmp(zeile, "ENDSEC")==0)break;
            }else{
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
            }
            if(Datei.fail())
            {
                Datei.close();
                if(Datei.bad())
                {
                    DXF_Handler::logSchreiben("badbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }
                else
                {
                    DXF_Handler::logSchreiben("failbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }

                return false;
            }
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
        return false;
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
        return true;
    }
    return true;
}

bool DXF_Import::EinlesenPunkteKanten(RUZ_Layer* layer, Liste<Char_Speicher>* namLst)
{
    layerLst = NULL;
    namensLst = namLst;
    einzelLayer = layer;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("Sucht Punkte und Kanten in %s ohne Liste...", DateiName);

    if(Datei.good())
    {
        SucheEntitieSection();

        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        while(!Datei.eof())
        {
            if(strcmp(zeile, "  0")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, Schluesselwort)==0)/*Punkt gefunden*/
                {
                    LesePunkt();
                }
                if(strcmp(zeile, "LINE")==0)/*Linie gefunden*/
                {
                    LeseStrich();
                }
                if(strcmp(zeile, "POLYLINE")==0)/*Polylinie gefunden*/
                {
                    LesePolyLinie();
                }
                if(strcmp(zeile, "3DFACE")==0)/*Flaeche gefunden*/
                {
                    LeseFlaeche();
                }
                if(strcmp(zeile, "ENDSEC")==0)break;
            }else{
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
            }
            if(Datei.fail())
            {
                Datei.close();
                if(Datei.bad())
                {
                    DXF_Handler::logSchreiben("badbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }
                else
                {
                    DXF_Handler::logSchreiben("failbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }

                return false;
            }
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
        return false;
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
        return true;
    }
    return true;
}

bool DXF_Import::EinlesenHintergrund(RUZ_Layer* hintergrundLayer)
{
    layerLst = NULL;
    namensLst = NULL;
    einzelLayer = hintergrundLayer;

    Datei.open(DateiName, ios_base::in);
    DXF_Handler::logSchreiben("%s konnte ge�ffnet werden\n", DateiName);

    if(Datei.good())
    {
        SucheEntitieSection();

        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        while(!Datei.eof())
        {
            if(strcmp(zeile, "  0")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, "LINE")==0)/*Linie gefunden*/
                {
                    LeseStrich();
                }
                if(strcmp(zeile, "POLYLINE")==0)/*Polylinie gefunden*/
                {
                    LesePolyLinie();
                }
                if(strcmp(zeile, "ARC")==0)/*Bogen gefunden*/
                {
                    LeseBogen();
                }
                if(strcmp(zeile, "ENDSEC")==0)
                {
                    break;
                }
            }else{
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
            }
            if(Datei.fail())
            {
                Datei.close();
                if(Datei.bad())
                {
                    DXF_Handler::logSchreiben("badbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }
                else
                {
                    DXF_Handler::logSchreiben("failbit beim Lesen der Datei\nEinlesen unvollst�ndig\n");
                }

                return false;
            }
        }
    }
    Datei.close();
    if(Datei.is_open())
    {
        DXF_Handler::logSchreiben("Datei NICHT geschlossen\n");
        return false;
    }else{
        DXF_Handler::logSchreiben("Datei geschlossen\n");
        return true;
    }
    return true;
}

void DXF_Import::SucheEntitieSection(void)
{
    while(!Datei.eof())
    {
        //while(Datei.peek()==' ')Datei.ignore();
        Datei.getline(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        if(strcmp(zeile, "  0")==0)
        {
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.getline(zeile, m_char_MAX-1);
            while(Datei.peek()=='\n')Datei.ignore();
            if(strcmp(zeile, "SECTION")==0)
            {
                //while(Datei.peek()==' ')Datei.ignore();
                Datei.getline(zeile, m_char_MAX-1);
                while(Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, "  2")==0)
                {
                    //while(Datei.peek()==' ')Datei.ignore();
                    Datei.getline(zeile, m_char_MAX-1);
                    while(Datei.peek()=='\n')Datei.ignore();
                    if(strcmp(zeile, "ENTITIES")==0)
                    {
                        return;
                    }
                }
            }
        }
        if(Datei.fail())break;
    }
    return;
}

void DXF_Import::LesePunkt(void)
{
    x[0]=y[0]=z[0]='0';/*Wert auf 0 setzen*/
    x[1]=y[1]=z[1]='\0';/*Endkennung c-string*/
    strcpy(aktLayer, "STANDARD");/*Punkte ohne Layerangabe werden zum Standardlayer hinzugef�gt*/
    double dX, dY, dZ;

    /*Suche nach den Punkt-Kennungen*/
    while(!Datei.eof())
    {
        //while(Datei.peek()==' ')Datei.ignore();
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        /*zeile auf Kennungen pruefen (x, y, z)*/
        if(strcmp(zeile, Kennung_X)==0)
        {/*x fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(x, zeile, m_char_MAX-1);
            x[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, Kennung_Y)==0)
        {/*y fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(y, zeile, m_char_MAX-1);
            y[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, Kennung_Z)==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(z, zeile, m_char_MAX-1);
            z[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  8")==0)
        {/*Layername*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(aktLayer, zeile, m_char_MAX-1);
            aktLayer[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  0")==0)
        {
            RUZ_Layer* t_layer = NULL;
            if(einzelLayer != NULL)
            {
                if(LayerAufListe(aktLayer))
                {
                    t_layer = einzelLayer;
                }
            }else{
                t_layer = HoleLayer(aktLayer);
            }
            if(t_layer!=NULL)
            {
                dX = aru_strtod(x);
                dY = aru_strtod(y);
                dZ = aru_strtod(z);
                if(isnan(dX)||isnan(dY)||isnan(dZ))return;
                new Punkt(dX, -dY, dZ, t_layer);
            }
            return;
        }
        if(Datei.fail())break;
    }
    return;
}

void DXF_Import::LeseStrich(void)
{
    x[0]=y[0]=xe[0]=ye[0]='0';/*Wert auf 0 setzen*/
    x[1]=y[1]=xe[1]=ye[1]='\0';/*Endkennung c-string*/
    strcpy(aktLayer, "STANDARD");/*Striche ohne Layerangabe werden zum Standardlayer hinzugef�gt*/

    /*Suche nach den LINE-Kennungen*/
    while(!Datei.eof())
    {
        //while(Datei.peek()==' ')Datei.ignore();
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        /*zeile auf Kennungen pruefen (x, y, xe, ye)*/
        if(strcmp(zeile, " 10")==0)
        {/*x fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(x, zeile, m_char_MAX-1);
            x[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 20")==0)
        {/*y fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(y, zeile, m_char_MAX-1);
            y[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 11")==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(xe, zeile, m_char_MAX-1);
            xe[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 21")==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(ye, zeile, m_char_MAX-1);
            ye[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  8")==0)
        {/*Layername*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(aktLayer, zeile, m_char_MAX-1);
            aktLayer[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  0")==0)
        {
            RUZ_Layer* t_layer = NULL;
            if(einzelLayer != NULL)
            {
                if(LayerAufListe(aktLayer))
                {
                    t_layer = einzelLayer;
                }
            }else{
                t_layer = HoleLayer(aktLayer);
            }
            if(t_layer!=NULL)new Strich((double)atof(x), -(double)atof(y), (double)atof(xe), -(double)atof(ye), t_layer);
            return;
        }
        if(Datei.fail())break;
    }
    return;
}

void DXF_Import::LesePolyLinie(void)
{
    double dXs, dYs, dX0, dY0, dX1, dY1;
    dXs = dYs = dX0 = dY0 = dX1 = dY1 = 0.0;

    bool bGeschlossen = false;
    bool bErsterVertex = true;

    x0[0]=y0[0]=x1[0]=y1[0]=xe[0]=ye[0]='0';/*Wert auf 0 setzen*/
    x0[1]=y0[1]=x1[1]=y1[1]=xe[1]=ye[1]='\0';/*Endkennung c-string*/
    strcpy(aktLayer, "STANDARD");/*Striche ohne Layerangabe werden zum Standardlayer hinzugef�gt*/

    /*Suche nach den POLYLINE-Kennungen*/

    while(!Datei.eof())
    {
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        /*zeile auf Kennungen pruefen (x, y, xe, ye)*/
        if(strcmp(zeile, "  0")==0)
        {
            while(!Datei.eof())
            {
                Datei.get(zeile, m_char_MAX-1);
                while (Datei.peek()=='\n')Datei.ignore();
                if(strcmp(zeile, "VERTEX")==0)
                {
                    LeseVertex(dX1, dY1);
                    if(bErsterVertex)
                    {
                        bErsterVertex = false;
                        dXs = dX0 = dX1;
                        dYs = dY0 = dY1;
                    }else
                    {
                        RUZ_Layer* t_layer = NULL;
                        if(einzelLayer != NULL)
                        {
                            if(LayerAufListe(aktLayer))
                            {
                                t_layer = einzelLayer;
                            }
                        }else{
                            t_layer = HoleLayer(aktLayer);
                        }
                        if(t_layer!=NULL)new Strich(dX0, -dY0, dX1, -dY1, t_layer);

                        dX0 = dX1;
                        dY0 = dY1;
                        continue;
                    }
                }
                if(strcmp(zeile, "SEQEND")==0)
                {
                    if(bGeschlossen)
                    {
                        RUZ_Layer* t_layer = NULL;
                        if(einzelLayer != NULL)
                        {
                            if(LayerAufListe(aktLayer))
                            {
                                t_layer = einzelLayer;
                            }
                        }else{
                            t_layer = HoleLayer(aktLayer);
                        }
                        if(t_layer!=NULL)new Strich(dX1, -dY1, dXs, -dYs, t_layer);
                    }
                    return;
                }
            }
        }
        if(strcmp(zeile, "  8")==0)
        {/*Layername*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(aktLayer, zeile, m_char_MAX-1);
            aktLayer[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 70")==0)
        {/*Polyline Flag*/
            int iFlag = 0;
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            iFlag = atol(zeile);
            if(iFlag & 0x00000001)
            {
                bGeschlossen = true;
            }
        }
        if(Datei.fail())break;
    }
    return;
}

void DXF_Import::LeseVertex(double& _x, double& _y)
{
    x[0]=y[0]='0';/*Wert auf 0 setzen*/
    x[1]=y[1]='\0';/*Endkennung c-string*/
    while(!Datei.eof())
    {
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        if(strcmp(zeile, " 10")==0)
        {
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            _x = atof(zeile);
        }
        if(strcmp(zeile, " 20")==0)
        {
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            _y = atof(zeile);
        }
        if(strcmp(zeile, "  0")==0)break;
        if(Datei.fail())break;
    }
    return;
}


void DXF_Import::LeseFlaeche(void)
{
    x0[0]=y0[0]=z0[0]=x1[0]=y1[0]=z1[0]=x2[0]=y2[0]=z2[0]='0';/*Wert auf 0 setzen*/
    x0[1]=y0[1]=z0[1]=x1[1]=y1[1]=z1[1]=x2[1]=y2[1]=z2[1]='\0';/*Endkennung c-string*/
    strcpy(aktLayer, "STANDARD");/*Punkte ohne Layerangabe werden zum Standardlayer hinzugef�gt*/

    /*Suche nach den 3DFACE-Kennungen*/
    while(!Datei.eof())
    {
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        /*zeile auf Kennungen pruefen (x, y, xe, ye)*/
        if(strcmp(zeile, " 10")==0)
        {/*x fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(x0, zeile, m_char_MAX-1);
            x0[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 20")==0)
        {/*y fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(y0, zeile, m_char_MAX-1);
            y0[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 30")==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(z0, zeile, m_char_MAX-1);
            z0[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 11")==0)
        {/*x fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(x1, zeile, m_char_MAX-1);
            x1[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 21")==0)
        {/*y fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(y1, zeile, m_char_MAX-1);
            y1[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 31")==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(z1, zeile, m_char_MAX-1);
            z1[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 12")==0)
        {/*x fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(x2, zeile, m_char_MAX-1);
            x2[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 22")==0)
        {/*y fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(y2, zeile, m_char_MAX-1);
            y2[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 32")==0)
        {/*z fuellen und als gefuellt kennzeichnen*/
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(z2, zeile, m_char_MAX-1);
            z2[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  8")==0)
        {/*Layername*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(aktLayer, zeile, m_char_MAX-1);
            aktLayer[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  0")==0)
        {
            RUZ_Layer* t_layer = NULL;
            if(einzelLayer != NULL)
            {
                if(LayerAufListe(aktLayer))
                {
                    t_layer = einzelLayer;
                }
            }else{
                t_layer = HoleLayer(aktLayer);
            }
            if(t_layer!=NULL)
            {
                Punkt* p0 = new Punkt((double)atof(x0), -(double)atof(y0), (double)atof(z0), t_layer);
                Punkt *p1 = new Punkt((double)atof(x1), -(double)atof(y1), (double)atof(z1), t_layer);
                Punkt *p2 = new Punkt((double)atof(x2), -(double)atof(y2), (double)atof(z2), t_layer);
                Linie *l0 = Linie::NeueLinie(p0, p1);
                Linie *l1 = Linie::NeueLinie(p1, p2);
                Linie *l2 = Linie::NeueLinie(p2, p0);
                Dreieck::NeuesDreieck(l0, l1, l2);
            }
            return;
        }
        if(Datei.fail())break;
    }
    return;
}

void DXF_Import::LeseBogen(void)
{
    radius[0]=alphaA[0]=alphaE[0]=xm[0]=ym[0]='0';/*Wert auf 0 setzen*/
    radius[1]=alphaA[1]=alphaE[1]=xm[1]=ym[1]='\0';/*Endkennung c-string*/
    strcpy(aktLayer, "STANDARD");/*Punkte ohne Layerangabe werden zum Standardlayer hinzugef�gt*/

    /*Suche nach den ARC-Kennungen*/
    while(!Datei.eof())
    {
        //while(Datei.peek()==' ')Datei.ignore();
        Datei.get(zeile, m_char_MAX-1);
        while(Datei.peek()=='\n')Datei.ignore();
        /*zeile auf Kennungen pruefen (x, y, xe, ye)*/
        if(strcmp(zeile, " 10")==0)
        {/*xm fuellen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(xm, zeile, m_char_MAX-1);
            xm[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 20")==0)
        {/*ym fuellen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(ym, zeile, m_char_MAX-1);
            ym[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 40")==0)
        {/*radius fuellen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, m_char_MAX-1);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(radius, zeile, m_char_MAX-1);
            radius[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 50")==0)
        {/*alphaA fuellen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, sizeof zeile);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(alphaA, zeile, m_char_MAX-1);
            alphaA[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, " 51")==0)
        {/*alphaE fuellen*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, sizeof zeile);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(alphaE, zeile, m_char_MAX-1);
            alphaE[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  8")==0)
        {/*Layername*/
            //while(Datei.peek()==' ')Datei.ignore();
            Datei.get(zeile, sizeof zeile);
            while (Datei.peek()=='\n')Datei.ignore();
            strncpy(aktLayer, zeile, m_char_MAX-1);
            aktLayer[m_char_MAX-1] = '\0';
        }
        if(strcmp(zeile, "  0")==0)
        {
            double d_radius, d_alphaA, d_alphaE, d_xm, d_ym, d_xa, d_ya, d_xe, d_ye;
            d_xm = (double)atof(xm);
            d_ym = (double)atof(ym);
            d_radius = (double)atof(radius);
            d_alphaA = (double)atof(alphaA)*PI/180;
            d_alphaE = (double)atof(alphaE)*PI/180;

            d_xa = d_xm + d_radius * cos(d_alphaA);
            d_ya = d_ym + d_radius * sin(d_alphaA);
            d_xe = d_xm + d_radius * cos(d_alphaE);
            d_ye = d_ym + d_radius * sin(d_alphaE);

            RUZ_Layer* t_layer = NULL;
            if(einzelLayer != NULL)
            {
                if(LayerAufListe(aktLayer))
                {
                    t_layer = einzelLayer;
                }
            }else{
                t_layer = HoleLayer(aktLayer);
            }
            if(t_layer!=NULL)new Bogen(d_xa, -d_ya, d_xe, -d_ye, d_xm, -d_ym, t_layer);
            return;
        }
        if(Datei.fail())break;
    }
    return;
}

RUZ_Layer* DXF_Import::HoleLayer(const char* layerName)
{
    if(!LayerAufListe(layerName))
    {
        DXF_Handler::logSchreiben("Layername nicht gefunden\n");
        return NULL;
    }
    if(layerLst == NULL)return NULL;
    RUZ_Layer* layerLaeufer;
    for(layerLaeufer = layerLst->GetErstesElement(); layerLaeufer != NULL; layerLaeufer = layerLst->GetNaechstesElement())
    {
        if(strcmp(layerLaeufer->HoleName(), layerName)==0)
        {
            return layerLaeufer;
        }
    }
    layerLaeufer = new RUZ_Layer(layerName);
    layerLst->Hinzufuegen(layerLaeufer);
    return layerLaeufer;
}

bool DXF_Import::LayerAufListe(const char* layerName)
{
    if(namensLst == NULL)return true;
    Char_Speicher* namensLaeufer;
    char laeuferName[m_char_MAX];
    for(namensLaeufer = namensLst->GetErstesElement(); namensLaeufer != NULL; namensLaeufer = namensLst->GetNaechstesElement())
    {
        strncpy(laeuferName, namensLaeufer->HoleInhalt(), m_char_MAX);
        laeuferName[m_char_MAX-1] = '\0';
        if(strcmp(laeuferName, layerName)==0)return true;
    }
    return false;
}
/*ENDE DXF_Import*/

/*DXF_Export*/
DXF_Export::DXF_Export(Liste<RUZ_Layer> *layerListe, char* DatName, int genauigkeit, unsigned int cSichtFlags)
{
    iHmGenauigkeit = genauigkeit;
    if(iHmGenauigkeit < 0)iHmGenauigkeit = 0;
    double dHmKommaVerschub = pow(10, iHmGenauigkeit);

    strcpy(DateiName, DatName);

    ofstream Ausgabe;
    Ausgabe.open(DateiName,ios_base::out|ios_base::trunc);
    if(Ausgabe.good())
    {
        Ausgabe.precision(15);
        ifstream blockVorlage;
        blockVorlage.open("BLOCKS.vlg", ios_base::in);
        if(blockVorlage.good())
        {
            char zeile[256];
            while(!blockVorlage.eof())
            {
                blockVorlage.get(zeile, 255);
                Ausgabe<<zeile<<"\n";
                blockVorlage.ignore(1);
            }
        }
        Ausgabe<<"  0\nSECTION\n  2\nENTITIES\n";/*DXF-Header schreiben*/
        Ausgabe.setf( ios::fixed, ios::floatfield );

        Vektor tOrt;
        Punkt *aktPunkt;
        HoehenMarke *aktHM;
        Linie *aktLinie;
        Flaeche *aktFlaeche;
        RUZ_Hoehenlinie *aktHL;
        Strich *aktStrich;
        Bogen *aktBogen;
        int anzEcken;

        int iFarbNr = 12;
        /*Alle Layer*/
        for(RUZ_Layer *aktLayer = layerListe->GetErstesElement(); aktLayer != NULL; aktLayer = layerListe->GetNaechstesElement())
        {
            if(!aktLayer->IstSichtbar())continue;

            if(cSichtFlags & (1<<0))
            {
              Liste<Punkt> *pktLst = aktLayer->HolePunkte();
              for(aktPunkt = pktLst->GetErstesElement(); aktPunkt != NULL; aktPunkt = pktLst->GetNaechstesElement())
              {
                  tOrt = aktPunkt->HolePosition();
                  Ausgabe<<"  0\nPOINT\n 10\n"<<tOrt.x()<<"\n 20\n"<<-tOrt.y()
                                  <<"\n 30\n"<<tOrt.z()<<"\n  8\n"<<aktLayer->HoleName()<<"\n";
              }
            }

            if(cSichtFlags & (1<<1))
            {
              Liste<Linie> *lnLst = aktLayer->HoleLinien();
              for(aktLinie = lnLst->GetErstesElement(); aktLinie != NULL; aktLinie = lnLst->GetNaechstesElement())
              {
                  Ausgabe<<"  0\nLINE\n";

                  for(int i = 0; i < 2; i++)
                  {
                      aktPunkt = aktLinie->HolePunkt(i);
                      tOrt = aktPunkt->HolePosition();
                      Ausgabe<<" 1"<<i<<"\n"<<tOrt.x()<<"\n 2"<<i<<"\n"<<-tOrt.y()
                              <<"\n 3"<<i<<"\n"<<tOrt.z()<<"\n";
                  }

                  Ausgabe<<"  8\n"<<aktLayer->HoleName()<<"\n";
              }
            }

            if(cSichtFlags & (1<<2))
            {
              Liste<Flaeche> *flLst = aktLayer->HoleFlaechen();
              for(aktFlaeche = flLst->GetErstesElement(); aktFlaeche != NULL; aktFlaeche = flLst->GetNaechstesElement())
              {
                Ausgabe<<"  0\n3DFACE\n";
                if(aktFlaeche->HoleTyp() == RUZ_Dreieck)
                {
                    anzEcken = 3;
                }else
                if(aktFlaeche->HoleTyp() == RUZ_Viereck)
                {
                    anzEcken = 4;
                }else{
                    continue;
                }

                for(int i = 0; i < 4; i++)
                {
                    aktPunkt = aktFlaeche->HolePunkt(i % anzEcken);
                    tOrt = aktPunkt->HolePosition();
                    Ausgabe<<" 1"<<i<<"\n"<<tOrt.x()<<"\n 2"<<i<<"\n"<<-tOrt.y()
                            <<"\n 3"<<i<<"\n"<<tOrt.z()<<"\n 62\n"<<iFarbNr<<"\n";
                }
                Ausgabe<<"  8\n"<<aktLayer->HoleName()<<"\n";

                if(cSichtFlags & (1<<3))
                {
                  Liste<RUZ_Hoehenlinie> *hlLst = aktFlaeche->HoleHL();
                  for(aktHL = hlLst->GetErstesElement(); aktHL != NULL; aktHL = hlLst->GetNaechstesElement())
                  {
                      Ausgabe<<"  0\nLINE\n";

                      for(int i = 0; i < 2; i++)
                      {
                          Ausgabe<<" 1"<<i<<"\n"<<aktHL->x(i)<<"\n 2"<<i<<"\n"<<-aktHL->y(i)
                                  <<"\n 3"<<i<<"\n"<<round(aktHL->h()*dHmKommaVerschub)/dHmKommaVerschub<<"\n";
                      }
                      Ausgabe.precision(iHmGenauigkeit);
                      Ausgabe<<"  8\nHL"<<round(aktHL->h()*dHmKommaVerschub)/dHmKommaVerschub<<"_"<<aktLayer->HoleName()<<"\n";
                      Ausgabe.precision(15);
                  }
                }
              }
            }

            if(cSichtFlags & (1<<4))
            {
              Liste<Strich> *strLst = aktLayer->HoleStriche();
              for(aktStrich = strLst->GetErstesElement(); aktStrich != NULL; aktStrich = strLst->GetNaechstesElement())
              {
                  Ausgabe<<"  0\nLINE\n";

                  Ausgabe<<" 10"<<"\n"<<aktStrich->Xa()
                          <<"\n 20"<<"\n"<<-aktStrich->Ya()
                          <<"\n 30"<<"\n"<<0.0<<"\n";
                  Ausgabe<<" 11"<<"\n"<<aktStrich->Xe()
                          <<"\n 21"<<"\n"<<-aktStrich->Ye()
                          <<"\n 31"<<"\n"<<0.0<<"\n";
                  Ausgabe<<"  8\n"<<aktLayer->HoleName()<<"\n";
              }
            }

            if(cSichtFlags & (1<<5))
            {
              Liste<Bogen> *bgLst = aktLayer->HoleBoegen();
              for(aktBogen = bgLst->GetErstesElement(); aktBogen != NULL; aktBogen = bgLst->GetNaechstesElement())
              {
                  Ausgabe<<"  0\nARC\n";

                  Ausgabe<<" 10"<<"\n"<<aktBogen->Xm()
                          <<"\n 20"<<"\n"<<-aktBogen->Ym()
                          <<"\n 30"<<"\n"<<0.0<<"\n";

                  double t_radius = sqrt(pow(aktBogen->Xa()-aktBogen->Xm(), 2)+pow(aktBogen->Ya()-aktBogen->Ym(), 2));
                  Ausgabe<<" 40"<<"\n"<<t_radius<<"\n";

                  double t_alpha = atan(-(aktBogen->Ya()-aktBogen->Ym())/(aktBogen->Xa()-aktBogen->Xm()))*180/PI;
                  if(acos((aktBogen->Xa()-aktBogen->Xm()) > 0))t_alpha += 180.0;
                  Ausgabe<<" 50"<<"\n"<<t_alpha<<"\n";

                  t_alpha = atan(-(aktBogen->Ye()-aktBogen->Ym())/(aktBogen->Xe()-aktBogen->Xm()))*180/PI;
                  if(acos((aktBogen->Xe()-aktBogen->Xm()) > 0))t_alpha += 180.0;
                  Ausgabe<<" 51"<<"\n"<<t_alpha<<"\n";

                  Ausgabe<<"  8\n"<<aktLayer->HoleName()<<"\n";
              }
            }

            if(cSichtFlags & (1<<6))
            {
              Liste<HoehenMarke> *hmLst = aktLayer->HoleHoehenMarken();
              for(aktHM = hmLst->GetErstesElement(); aktHM != NULL; aktHM = hmLst->GetNaechstesElement())
              {
                  tOrt = aktHM->HolePosition();
                  Ausgabe<<"  0\nTEXT\n 10\n"<<tOrt.x()<<"\n 20\n"<<-tOrt.y()<<"\n  1\n";
                  if(aktHM->IstInFlaeche())
                  {
                      Ausgabe.precision(iHmGenauigkeit);
                      Ausgabe<<round(tOrt.z()*dHmKommaVerschub)/dHmKommaVerschub;
                      Ausgabe.precision(15);
                  }else{
                      Ausgabe<<"#NiF";
                  }
                  Ausgabe<<"\n 40\n"<<0.1<<"\n  8\n"<<aktLayer->HoleName()<<"\n";
                  Ausgabe<<"  0\nINSERT\n  2\nHoehenkote\n 10\n"<<tOrt.x()<<"\n 20\n"<<-tOrt.y()<<"\n 30\n";
                  if(aktHM->IstInFlaeche())
                  {
                      Ausgabe.precision(iHmGenauigkeit);
                      Ausgabe<<round(tOrt.z()*dHmKommaVerschub)/dHmKommaVerschub;
                      Ausgabe.precision(15);
                  }else{
                      Ausgabe<<0.0;
                  }
                  Ausgabe<<"\n 41\n0.2\n42\n0.2\n43\n0.2"<<"\n  8\n"<<aktLayer->HoleName()<<"\n";
              }
            }
            iFarbNr += 90;
            if(iFarbNr > 255)iFarbNr -= 200;
        }
        Ausgabe<<"  0\nENDSEC\n  0\nEOF\n";
        Ausgabe.close();
    }
}
/*ENDE DXF_Export*/
