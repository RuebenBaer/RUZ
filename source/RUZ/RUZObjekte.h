/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul der Zeichnungsobjekte

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef __RUZObjekte_
#define __RUZObjekte_

#include "..\Vektor\Vektor.h"
#include "..\Liste\Verkettete_Liste.h"
//#include <windows.h>//cross-plattform dadurch kaputt

class Linie;
class Flaeche;
class Dreieck;
class Viereck;
class Punkt;
class HoehenMarke;
class GefaelleMarke;
class RUZ_Objekt;
class Strich;
class Bogen;
class Kreis;
class Fangpunkt;


/*Objekte nicht von RUZ_Objekte abgeleitet*/
struct RelativLage
{
    double lambda, mue;
    RelativLage(double t_lambda, double t_mue):lambda(t_lambda), mue(t_mue){}
};

struct Hoehenpunkt
{
    double x, y, z;
    Hoehenpunkt(const Vektor& ort):x(ort.x()), y(ort.y()), z(ort.z()){}
};

struct ObjektPaar
{
    RUZ_Objekt *objReferenz, *objBezugsObj;
    ObjektPaar(RUZ_Objekt* a, RUZ_Objekt* b):objReferenz(a), objBezugsObj(b){}
};

struct LinienFlaeche {
	Linie *ln;						/* Grundlinie 													*/
	Punkt *p_neu[2] = {NULL, NULL};	/* neue Eckpunkte der Flaeche p0 -> p_neu[0]; p1 -> p_neu[1]	*/
	Vektor n, extR;					/* Flaechennormale, Extrusionsrichtung							*/
};

class RUZ_Hoehenlinie
{
public:
    RUZ_Hoehenlinie(double xa, double ya, double xe, double ye, double h);
    ~RUZ_Hoehenlinie();
    double x(int);
    double y(int);
    double h(void);
private:
    double x_a, y_a, x_e, y_e, hoehe;
};

#include <iostream>
#include <fstream>

enum Achse {x = 0, y, z};
enum KantenKlasse{unklassifiziert = 0, BE, DE, RE, gesperrt};
/*boundary edge, diagonal edge, corner edge, regular edge, special regaular edge*/
enum RUZ_Typ{RUZ_Punkt = 0, RUZ_Linie, RUZ_Dreieck, RUZ_Viereck, RUZ_HoehenMarke, RUZ_GefaelleMarke, RUZ_Kreis, RUZ_Fangpunkt};
#include "..\RUZ\RUZVerwaltung.h"

class Strich
{
protected:
    Strich(double xa, double ya, double xe, double ye);
    RUZ_Layer *m_layer;
public:
    Strich(double xa, double ya, double xe, double ye, RUZ_Layer *layer);
    ~Strich();
    double Xa(void);
    double Ya(void);
    double Xe(void);
    double Ye(void);
    void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);
private:
    double m_AnfangX, m_AnfangY, m_EndeX, m_EndeY;
};

class Bogen:public Strich
{
public:
    Bogen(double xa, double ya, double xe, double ye, double xm, double ym, RUZ_Layer *layer);
    ~Bogen();
    double Xm(void);
    double Ym(void);
    void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);
private:
    double m_MitteX, m_MitteY;
};
/*ENDE Objekte nicht von RUZ_Objekte abgeleitet*/

class RUZ_Objekt
{
public:
    RUZ_Objekt();
    virtual ~RUZ_Objekt();
    virtual RUZ_Typ HoleTyp(void) const = 0;
    virtual RUZ_Layer* HoleLayer(void) const = 0;
    virtual void Verschieben(const Vektor&) = 0;
    virtual bool HolePosition(Vektor&) = 0;
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL) = 0;
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ) = 0;
    char HoleBesucht(void){return besucht;};
    void SetzeBesucht(char a){besucht = a; return;};
protected:
    int QuadratischeGleichung(double a, double b, double c, double& lsg1, double& lsg2);
    /*Logbuch*/
    void logSchreiben(const char* msg, ...);
    /*ENDE Logbuch*/
    char besucht;
};

class Punkt : public RUZ_Objekt
{
private:
    Vektor pOrt;
    Liste<Linie> *adjLinie;
    void Aufraeumen(void);
    float m_farbe[3];
    char m_name[64];
    RUZ_Layer *m_layer;
    void AenderungAnLayerMitteilen(void);
public:
    Punkt();
    Punkt(const Punkt&);
    Punkt(const Vektor&, RUZ_Layer*);
    Punkt(double, double, double, RUZ_Layer*);
    Punkt(double*, RUZ_Layer*);
	
	Linie* Verbunden(Punkt* pkt);

    virtual ~Punkt();
    virtual RUZ_Layer* HoleLayer(void) const;
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_Punkt;};
    virtual void Verschieben(const Vektor&);
    virtual bool HolePosition(Vektor&);
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);

    bool operator==(const Punkt&) const;
    Punkt& operator=(const Punkt&);
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);
    Vektor HolePosition(void) const;
    void SetzeLayer(RUZ_Layer*);
    void SetzeFarbe(float, float, float);
    float* HoleFarbe(void);
    char* HoleName(void){return m_name;};
    void SetzeName(const char* n);
    /*Zugehörige Linien*/
    void Hinzufuegen(Linie*);
    void Entfernen(Linie*);
    Liste<Linie>* HoleLinien(void) const;

    void Positionieren(const Vektor&);

    bool IstNahebei(double, double, double, Achse);
    void ErsetzenDurch(Punkt*);

    void ImLogAusgeben(void);
};

class Linie : public RUZ_Objekt
{
private:
    Punkt *p[2];
    Linie *teilLinie[2];
    Liste<Flaeche> *adjFlaeche;
    int flaechenZaehler;
    void Aufraeumen(void);
    bool geschuetzt;
    /*Linienklassen zur Quadrilateration*/
    KantenKlasse kantenTyp;
    int farbe[3];

    Linie(Punkt*, Punkt*);

public:
    static Linie* NeueLinie(Punkt*, Punkt*);
    Linie();
    Linie(const Linie&);
    Linie& operator=(const Linie&);
    bool operator==(const Linie&);
    Punkt* HolePunkt(int) const;
    virtual ~Linie();

    virtual RUZ_Layer* HoleLayer(void) const;
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_Linie;};
    virtual void Verschieben(const Vektor&);
    virtual bool HolePosition(Vektor&);
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ){return;};
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);

    bool HoleFarbe(unsigned char&, unsigned char&, unsigned char&) const;
    void SetzeFarbe(unsigned char, unsigned char, unsigned char);
    void LoescheFarbe(void);
    Vektor HoleRichtung(void) const;
    void SetzePunkte(Punkt*, Punkt*);
    void SetzePunkt(int, Punkt*);
    bool Teilen(Punkt*, Achse, bool);
    bool LotFussPunkt(Vektor&, Achse, bool);
    void ZwangsTeilung(Punkt*);
    void ErsetzenDurch(Linie*);
    double Laenge(void)const;
    double ProjLaenge(Achse)const;
    bool IstNahebei(double, double, double, Achse);
    bool IstAufLinie(const Vektor&, Achse);
    bool IstEndPunkt(Punkt *);
    /*bool schneidet(Linie&);//3D schnitt*/
    bool schneidet(double, double, double, double, Achse);/*Verbindung zweier Koordinaten, Projektion entlang Achse x=0, y, z*/
    bool schneidet(Linie*, Achse);/*Linie, Projektion entlang Achse x=0, y, z*/
    bool schneidet(Linie*, Vektor&, Achse, bool virtuell = false);/*Linie, Projektion entlang Achse x=0, y, z - füllt den Vektor mit den Schnittkoordinaten*/
    bool schneidet(double, double, double, double, Vektor&, Achse, bool virtuell = false);/*Verbindung zweier Koordinaten, Projektion entlang Achse x=0, y, z - füllt den Vektor mit den Schnittkoordinaten*/
    /*Zugehörige Flächen*/
    void Hinzufuegen(Flaeche*);
    void Entfernen(Flaeche*);
    Liste<Flaeche>* HoleFlaechen(void)const;
    /*zusätzliche Eigenschaften*/
    bool IstGeschuetzt(void)const{return geschuetzt;};
    void SetzeGeschuetzt(bool a){geschuetzt = a; return;};
    int HoleFlaechenZahl(void)const{return flaechenZaehler;};
    void SetzeKantenklasse(KantenKlasse);
    KantenKlasse HoleKantenklasse(void)const;
    double HoleGefaelle(Achse) const;

    void HoehenPunkteFinden(Liste<Hoehenpunkt>* pktLst, double hoehenSchritt, double suchRadius, double startHoehe);

    Vektor PunktBeiLambda(double lambda);

    int Extrudieren(double resGefaelle, Vektor v_extrRichtung, Achse prjRichtung = z, bool gefaelleBehalten = false);
    int Extrudieren(double resGefaelle, Vektor v_extrRichtung, double extrAbstand, Achse prjRichtung = z, bool gefaelleBehalten = false);

    Linie* Parallele(Vektor vRichtung, Achse prjRichtung = z);
    Linie* Parallele(Vektor vRichtung, double dAbstand, Achse prjRichtung = z);
};


class Flaeche : public RUZ_Objekt
{
protected:
    Vektor m_normale;
protected:
    Liste<RUZ_Hoehenlinie>* adjHL;
    Liste<HoehenMarke>* adjHoehenMarken;
    Liste<GefaelleMarke>* adjGefaelleMarken;
    double m_hoehenSchritt, m_suchRadius, m_startHoehe;
    Achse m_projektionsRichtung;
    void LoescheDoppeltePunkte(Liste<Hoehenpunkt>* pktLst);
    int farbe[3];
	double min[3], max[3];
public:
    Flaeche();
    virtual ~Flaeche();
    virtual RUZ_Layer* HoleLayer(void) const = 0;
    virtual RUZ_Typ HoleTyp(void) const = 0;
    bool HoleFarbe(unsigned char&, unsigned char&, unsigned char&);
    void SetzeFarbe(unsigned char, unsigned char, unsigned char);
    void LoescheFarbe(void);
    Vektor HoleNormale(void);
    virtual Linie* HoleLinie(int)const = 0;
    virtual Punkt* HolePunkt(int)const = 0;
    virtual double FlaechenInhalt(Achse) = 0;
    virtual double Volumen(Achse) = 0;
    virtual double Seitenverhaeltnis(void) = 0;
    virtual RUZ_Objekt* IstInnerhalb(double, double, Achse) = 0;
    virtual RUZ_Objekt* IstInnerhalb(Punkt*) = 0;
    virtual RUZ_Objekt* IstInnerhalb(Vektor) = 0;
    virtual bool OrtAufFlaeche(Vektor& vkt, Achse achse, bool innenPruefung = 1) = 0;
    virtual bool Gefaelle(Vektor& vOrt, Vektor& vGefaelle, Achse aProjRichtung) = 0;
    virtual void Teilen(Punkt*, Achse, bool) = 0;
    virtual void ZwangsTeilung(Punkt*) = 0;
    virtual void DoHoehenlinienFinden(void) = 0;
    virtual void NormaleBerechnen(void) = 0;
    virtual Vektor Schwerpunkt(void) = 0;
    virtual bool DurchstossPunkt(Linie* durchstossLn, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt = false) = 0;
    virtual bool OrdinateAufEbene(Punkt*, Achse) = 0;
    virtual double OrdinateAufEbene(double, double, Achse) = 0;
    virtual bool IstEckPunkt(Punkt*) = 0;
    virtual void ErsetzeLinie(Linie*, Linie*) = 0;
    virtual void EckenNeuErmitteln(void) = 0;
	virtual void MinMax(void) = 0;

    virtual bool operator==(const Flaeche&) = 0;

    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ){return;};

    Liste<RUZ_Hoehenlinie>* HoleHL(void);
    void SetzeHoehenlinie(double, double, double, double, double);
    void HoehenlinienFinden();
    void Hinzufuegen(HoehenMarke*);
    void Entfernen(HoehenMarke*);
    void Hinzufuegen(GefaelleMarke*);
    void Entfernen(GefaelleMarke*);
    void HoehenMarkenAktualisieren(void);
    bool TeiltLinieMit(Flaeche*);
};

class Dreieck : public Flaeche
{
private:
    Linie *l[3];
    Punkt *p[3];
    Dreieck();
    Dreieck(Punkt**, Punkt**, Punkt**, Linie*, Linie*, Linie*);

    void ListeAusgeben(Liste<Hoehenpunkt>*);

public:
    static Dreieck* NeuesDreieck(Linie*, Linie*, Linie*);
    Dreieck(const Dreieck&);
    Dreieck& operator=(const Dreieck&);
    virtual ~Dreieck();
    virtual RUZ_Layer* HoleLayer(void) const;
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_Dreieck;};
    virtual void Verschieben(const Vektor&);
    virtual bool HolePosition(Vektor& vkt){return false;};
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);

    virtual Linie* HoleLinie(int) const;
    virtual Punkt* HolePunkt(int) const;
    virtual double FlaechenInhalt(Achse);
    virtual double Volumen(Achse);
    virtual double Seitenverhaeltnis(void);
    virtual RUZ_Objekt* IstInnerhalb(double x, double y, Achse a);
    virtual RUZ_Objekt* IstInnerhalb(Punkt*);
    virtual RUZ_Objekt* IstInnerhalb(Vektor);
    virtual bool OrtAufFlaeche(Vektor& vkt, Achse achse, bool innenPruefung = 1);
    virtual bool Gefaelle(Vektor&, Vektor&, Achse);
    virtual void Teilen(Punkt*, Achse, bool);
    virtual void ZwangsTeilung(Punkt*);
    virtual void DoHoehenlinienFinden(void);
    virtual void NormaleBerechnen(void);
    virtual Vektor Schwerpunkt(void);
    virtual bool DurchstossPunkt(Linie* durchstossLn, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt = false);
    virtual bool OrdinateAufEbene(Punkt*, Achse);
    virtual double OrdinateAufEbene(double, double, Achse);
    virtual bool IstEckPunkt(Punkt*);
    virtual void ErsetzeLinie(Linie*, Linie*);
    virtual void EckenNeuErmitteln(void);
	virtual void MinMax(void);

    Vektor MitteGegenueber(Punkt*);
    bool operator==(const Flaeche&);

    void ImLogAusgeben(void);
};

class Viereck : public Flaeche /*konkave Vierecke!*/
{
private:
    Linie *l[4];
    Punkt *p[4];
    Vektor m_p10, m_p30, m_p0123;
    int farbe[3];
    Viereck();
    Viereck(Punkt** p1, Punkt** p2, Punkt** p3, Punkt** p4, Linie** l1, Linie** l2, Linie** l3, Linie** l4);

    void HilfsVektorenBerechnen(void);
    void ImLogAusgeben(void);
public:
    static Viereck* NeuesViereck(Dreieck*, Dreieck*);/*loescht die Dreiecke*/
    static Viereck* NeuesViereck(Linie*, Linie*, Linie*, Linie*);
    Viereck(const Viereck&);
    Viereck& operator=(const Viereck&);
    virtual ~Viereck();
    virtual RUZ_Layer* HoleLayer(void) const;
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_Viereck;};
    virtual void Verschieben(const Vektor&);
    virtual bool HolePosition(Vektor& vkt){return false;};
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);

    virtual Linie* HoleLinie(int) const;
    virtual Punkt* HolePunkt(int) const;
    virtual double FlaechenInhalt(Achse){return 0;};
    virtual double Volumen(Achse){return 0;};
    virtual double Seitenverhaeltnis(void){return -1.0;};
    virtual RUZ_Objekt* IstInnerhalb(double, double, Achse);
    virtual RUZ_Objekt* IstInnerhalb(Punkt *pkt){return NULL;};
    virtual RUZ_Objekt* IstInnerhalb(Vektor pkt){return NULL;};
    virtual bool OrtAufFlaeche(Vektor& vkt, Achse achse = z, bool innenPruefung = 1);
    virtual bool Gefaelle(Vektor&, Vektor&, Achse);
    void HoleNormale(Vektor&, Vektor&, Achse);
    virtual bool OrdinateAufEbene(Punkt*, Achse);
    virtual double OrdinateAufEbene(double, double, Achse);
    virtual void Teilen(Punkt*, Achse, bool);
    void Teilen(void);
    virtual void ZwangsTeilung(Punkt*);
    virtual void DoHoehenlinienFinden(void);
    virtual void NormaleBerechnen(void);
    virtual Vektor Schwerpunkt(void);
    virtual bool DurchstossPunkt(Linie* durchstossLn, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt = false);
    virtual bool DurchstossPunkt(Vektor v_Ln_start, Vektor v_ln_ende, Vektor& v_rueckgabe1, Vektor& v_rueckgabe2, bool aussenErlaubt = false);
    virtual bool IstEckPunkt(Punkt*);
    virtual void ErsetzeLinie(Linie*, Linie*);/*Verwendung zur Vereinigung von Objekten*/
    virtual void EckenNeuErmitteln(void);
	virtual void MinMax(void);
private:
    bool LageAufFlaeche(Vektor& vOrt, RelativLage& rl_Lage_1, RelativLage& rl_Lage_2, Achse projRichtung = z);
    void EinzelfeldHoehenlinien(Linie*, Linie*, Linie*, Linie*);
public:
    bool operator==(const Flaeche&);
};

class HoehenMarke : public RUZ_Objekt
{
public:
    HoehenMarke(const Vektor&, RUZ_Layer*);
    HoehenMarke(double, double, RUZ_Layer*);
    HoehenMarke(double*, RUZ_Layer*);
    virtual ~HoehenMarke();
    virtual RUZ_Layer* HoleLayer(void) const {return m_layer;};
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_HoehenMarke;};
    virtual void Verschieben(const Vektor&);
    virtual void LagePruefen(void);
    virtual bool HolePosition(Vektor&);
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);

    void Positionieren(const Vektor&);
    bool IstInFlaeche(void);
    void FlaecheNullen(void);
    bool IstNahebei(double, double, double, Achse);
    Vektor HolePosition(void) const;
protected:
    Vektor vOrt;
    Flaeche *m_flaeche;
    RUZ_Layer* m_layer;
    char m_hoehe[18];

    void FlaecheFinden(void);
    void NeuBerechnen(void);
};

class GefaelleMarke : public RUZ_Objekt
{
public:
    GefaelleMarke(float, float, float, const Vektor&, RUZ_Layer*, Flaeche* flTemp = NULL);
    GefaelleMarke(float, float, float, double, double, RUZ_Layer*, Flaeche* flTemp = NULL);
    GefaelleMarke(float, float, float, double*, RUZ_Layer*, Flaeche* flTemp = NULL);
    virtual ~GefaelleMarke();
    virtual RUZ_Layer* HoleLayer(void) const {return m_layer;};
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_GefaelleMarke;};
    virtual void LagePruefen(void);

    /*Von RUZ_Objekt geerbte, aber nicht verwendete Fktn*/
    virtual void Verschieben(const Vektor& vkt){return;};
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL){return NULL;};
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ){return;};
    /*ENDE Von RUZ_Objekt geerbte, aber nicht verwendete Fktn*/

    virtual bool HolePosition(Vektor& vkt);
    Vektor HolePosition(void) const;
    Flaeche* HoleFlaeche(void) const {return m_flaeche;};
    bool IstInFlaeche(void);
    void FlaecheNullen(void);
    void SetzeMinOptMax(float, float, float);
    unsigned char *HoleFarbe(void);
    Vektor HoleGefaelle(void);
private:
    Vektor vOrt;
    Vektor vGefaelle;
    Flaeche *m_flaeche;
    RUZ_Layer* m_layer;

    float fMinimum, fOptimum, fMaximum;
    unsigned char cFarbe[3];

    void FlaecheFinden(void);
    void NeuBerechnen(void);
};

class Kreis : public RUZ_Objekt
{
private:
    Vektor vOrt;
    double dRadius;
    RUZ_Layer* m_layer;
    Liste<Vektor>* m_Fangpunkte;
public:
    Kreis();
    Kreis(const Vektor&, double, RUZ_Layer*);
    Kreis(const Kreis&);
    virtual ~Kreis();
    virtual RUZ_Typ HoleTyp(void) const;
    virtual RUZ_Layer* HoleLayer(void) const;
    virtual void Verschieben(const Vektor&);
    virtual bool HolePosition(Vektor&);
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);

    Kreis& operator=(const Kreis&);
    void Positionieren(const Vektor&);

    Vektor HolePosition(void) const;
    void SetzeRadius(double);
    double HoleRadius(void) const;
    Liste<Vektor>* HoleFangpunkte(void);
    bool FindeSchnittpunkte(Linie*, Achse);
    bool FindeSchnittpunkte(Kreis*, Achse);
    void LoescheFangpunkte(void);

    int IstNahebei(double, double, double, Achse); /*0 - false | 1 - bei Mittelpunkt | 2 - bei Rand*/
};

class Fangpunkt : public RUZ_Objekt
{
private:
    Vektor vOrt;
    RUZ_Layer* m_layer;
public:
    Fangpunkt(const Vektor&, RUZ_Layer*);
    Fangpunkt(double, double, double, RUZ_Layer*);
    Fangpunkt(double*, RUZ_Layer*);
    virtual ~Fangpunkt();
    virtual RUZ_Layer* HoleLayer(void) const {return m_layer;};
    virtual RUZ_Typ HoleTyp(void) const {return RUZ_Fangpunkt;};
    virtual void Verschieben(const Vektor& vkt){return;};
    virtual bool HolePosition(Vektor&);
    virtual RUZ_Objekt* Kopieren(Liste<RUZ_Objekt>* lObjQuelle, Liste<ObjektPaar>* lBereitsKopiert, RUZ_Layer* zielLayer = NULL);
    virtual void Skalieren(const Vektor&, double fktX, double fktY, double fktZ);
    void Positionieren(const Vektor&);

    bool IstNahebei(double, double, double, Achse);
    Vektor HolePosition(void) const {return vOrt;};
};

/*Logbuch*/
void logSchreiben(const char* msg, ...);
/*ENDE Logbuch*/
bool LinienzugGeschlossen(Punkt**, Punkt**, Punkt**, Linie*, Linie*, Linie*);

void LinienExtrudieren(LinienFlaeche lnFl[], int groesse, double reGef, double zielOrdinate, Achse projAchse, Vektor &richtungsPunkt);
bool LinienNormale(LinienFlaeche &lnFl, Vektor &richtungsPunkt, double resGefaelle, Achse prjRichtung);

#endif // __RUZObjekte_
