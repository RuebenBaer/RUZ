//TODO (Ansi#1#): Leere Aufgabe

#include "RUZVerwaltung.h"
#include <time.h>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>

#define PI 3.14159265358979

using namespace std;


RUZ_Layer::RUZ_Layer(const char* name)
{
    strncpy(m_name, name, 255);

    m_punktLst = new Liste<Punkt>;
    m_linienLst = new Liste<Linie>;
    m_flaechenLst = new Liste<Flaeche>;
    m_strichLst = new Liste<Strich>;
    m_bogenLst = new Liste<Bogen>;
    m_kreisLst = new Liste<Kreis>;
    m_fngPktLst = new Liste<Fangpunkt>;
    m_hoehenMarkenLst = new Liste<HoehenMarke>;
    m_gefaelleMarkenLst = new Liste<GefaelleMarke>;
    m_fehlerMarkierung = new Liste<Vektor>;

    m_aktualisierungsFlaechen = new Liste<Flaeche>;

    m_projektionsRichtung = z;

    sichtbar = true;
    //gefaelleRasterAktiv = false;
    /*Log starten*/
	ofstream Logbuch;
}

RUZ_Layer::~RUZ_Layer()
{
    m_hoehenMarkenLst->ListeLoeschen("RUZ_Layer");/*Alle Hoehenmarken löschen*/
    m_gefaelleMarkenLst->ListeLoeschen("RUZ_Layer");/*Alle Gefällemarken löschen*/
    m_strichLst->ListeLoeschen("RUZ_Layer");/*Alle Striche löschen*/
    m_bogenLst->ListeLoeschen("RUZ_Layer");/*Alle Bögen löschen*/
    m_kreisLst->ListeLoeschen("RUZ_Layer");/*Alle Kreise löschen*/
    m_fngPktLst->ListeLoeschen("RUZ_Layer");/*Alle Fangpunkte löschen*/
    m_punktLst->ListeLoeschen("RUZ_Layer");/*Punkte werden gelöscht und alle angehängten Objekte*/
    m_fehlerMarkierung->ListeLoeschen("RUZ_Layer");
    /*Die Listen selber löschen*/
    delete m_punktLst;
    delete m_linienLst;
    delete m_flaechenLst;
    delete m_strichLst;
    delete m_bogenLst;
    delete m_kreisLst;
    delete m_fngPktLst;
    delete m_hoehenMarkenLst;
    delete m_gefaelleMarkenLst;
    delete m_aktualisierungsFlaechen;
    delete m_fehlerMarkierung;
    logSchreiben("Layer ");logSchreiben(this->HoleName());logSchreiben(" geloescht\n");
}

void RUZ_Layer::Benennen(const char* name)
{
    strncpy(m_name, name, 255);
    return;
}

void RUZ_Layer::thPunkteVernetzen(thread_info_vernetzen *thInf, Liste<Punkt>* t_pktLst)
{
    Punkt *von, *nach;
    Linie *strich1, *strich2;
    Listenelement<Linie> *LE_strich1, *LE_strich2;
	
	for(Linie *laeufer = m_linienLst->GetErstesElement(); laeufer != NULL; laeufer = m_linienLst->GetNaechstesElement())
    {
        laeufer->SetzeGeschuetzt(true);
		thInf->InkrVorhandeneLinie();
    }
	
    /*jeden Punkt mit jedem verbinden*/
    Liste<Punkt>* pktSammlung;
    if(t_pktLst->GetListenGroesse() != 0)
    {
        pktSammlung = t_pktLst;
    }else{
        pktSammlung = m_punktLst;
    }
	thInf->SetzeStatus(0);
    LoescheDoppeltePunkte(pktSammlung, 4);
	thInf->SetzeStatus(1);
    for(von = pktSammlung->GetErstesElement(); von != NULL; von = pktSammlung->GetNachfolger(von))
    {
        if(von->HoleLayer() != this)
        {
            pktSammlung->Entfernen(von);
        }
		if(thInf->BeendenAngefragt())//Abbruch angefragt
		{
			thInf->BeendigungFeststellen();
			return;
		}
    }
	
	/* Verbundene Punkte finden*/
	Liste<ObjektPaar> punktePaare;
	for(von = pktSammlung->GetErstesElement(); von != NULL; von = pktSammlung->GetNachfolger(von))
    {
        for(nach = pktSammlung->GetNachfolger(von); nach != NULL; nach = pktSammlung->GetNachfolger(nach))
        {
            if (SindDiagonalEndpunkte(von, nach)) {
				punktePaare.Hinzufuegen(new ObjektPaar(von, nach));
				continue;
			}
            Linie *verbindungsLinie = Verbunden(von, nach);
            if(verbindungsLinie != NULL)//prüfen, ob Punkte bereits verbunden sind
            {
				punktePaare.Hinzufuegen(new ObjektPaar(von, nach));
                continue;
            }
		}
	}
	
	thInf->SetzeStatus(2);
	bool verbunden;
    for (von = pktSammlung->GetErstesElement(); von != NULL; von = pktSammlung->GetNachfolger(von)) {
        for (nach = pktSammlung->GetNachfolger(von); nach != NULL; nach = pktSammlung->GetNachfolger(nach)) {
			verbunden = false;
            for (ObjektPaar *OP = punktePaare.GetErstesElement(); OP != NULL; OP = punktePaare.GetNaechstesElement()) {
				if ((OP->objReferenz == von && OP->objBezugsObj == nach) || (OP->objReferenz == nach && OP->objBezugsObj == von)) {
					verbunden = true;
					break;
				}
			}
			if (verbunden != true) {
				Linie::NeueLinie(von, nach, false);
				thInf->InkrNeueLinie();
			}
			if(thInf->BeendenAngefragt())//Abbruch angefragt
			{
				thInf->BeendigungFeststellen();
				return;
			}
        }
    }
	punktePaare.ListeLoeschen("");
	
	thInf->SetzeStatus(3);
    LinienNachLaengeSortieren();
    /*von kurzen Linien geschnittene Linien loeschen*/
	thInf->SetzeStatus(4);
	unsigned long long int aktLinieNr = 0;
    for(LE_strich1 = m_linienLst->GetErstesListenelement(); LE_strich1 != NULL;)
    {
		thInf->BearbeiteteLinie(aktLinieNr++);
        strich1 = LE_strich1->GetElement();
        for(LE_strich2 = LE_strich1->GetNachfolger(); LE_strich2 != NULL;)
        {
            strich2 = LE_strich2->GetElement();
            if(strich1->schneidet(strich2, z))
            {
                Listenelement<Linie> *temp;
                temp = LE_strich2;
                LE_strich2 = LE_strich2->GetNachfolger();
                m_linienLst->Entfernen(temp);
                delete strich2;
            }else{
                LE_strich2 = LE_strich2->GetNachfolger();
            }
			if(thInf->BeendenAngefragt())//Abbruch angefragt
			{
				thInf->BeendigungFeststellen();
				return;
			}
        }
        LE_strich1 = LE_strich1->GetNachfolger();
    }
    /*ENDE von kurzen Linien geschnittene Linien loeschen*/
	
    thInf->BeendigungFeststellen();/*Beendet modalen Dialog*/
    return;
}

RUZ_Layer* RUZ_Layer::Kopieren(char* name)
{
    RUZ_Layer* neuLayer = new RUZ_Layer(name);
    if(neuLayer == NULL)
    {
        return NULL;
    }

    Punkt* n_pkt;
    int i = 0;
    for(Listenelement<Punkt>* t_LE_pkt = m_punktLst->GetErstesListenelement(); t_LE_pkt != NULL; t_LE_pkt = t_LE_pkt->GetNachfolger())
    {
        Vektor t_ort = t_LE_pkt->GetElement()->HolePosition();
        n_pkt = new Punkt(t_ort.x(), t_ort.y(), t_ort.z(), neuLayer);
        if(n_pkt)
        {
            neuLayer->Wert(n_pkt, (double)i);
            t_LE_pkt->Wert((double)i);
            n_pkt->SetzeName(t_LE_pkt->GetElement()->HoleName());
            i++;
        }
    }

    Linie *n_ln;
    Punkt *n_anfang, *n_ende;
    double a_anfang, a_ende;
    i = 0;
    for(Listenelement<Linie>* a_LE_ln = m_linienLst->GetErstesListenelement(); a_LE_ln != NULL; a_LE_ln = a_LE_ln->GetNachfolger())
    {
        a_anfang = m_punktLst->Wert(a_LE_ln->GetElement()->HolePunkt(0));
        a_ende = m_punktLst->Wert(a_LE_ln->GetElement()->HolePunkt(1));

        Liste<Punkt> *n_pkt_lst = neuLayer->HolePunkte();
        n_anfang = n_ende = NULL;
        for(Listenelement<Punkt> *t_LE_pkt = n_pkt_lst->GetErstesListenelement(); t_LE_pkt != NULL; t_LE_pkt = t_LE_pkt->GetNachfolger())
        {
            if(t_LE_pkt->Wert() == a_anfang)
            {
                n_anfang = t_LE_pkt->GetElement();
            }
            if(t_LE_pkt->Wert() == a_ende)
            {
                n_ende = t_LE_pkt->GetElement();
            }
            if(n_anfang && n_ende)
            {
                n_ln = NULL;
                n_ln = Linie::NeueLinie(n_anfang, n_ende);
                if(n_ln)
                {
                    neuLayer->Wert(n_ln, (double)i);
                    a_LE_ln->Wert((double)i);
                    i++;
                }
                break;
            }
        }
    }

    Linie *neu_ln[4];
    double a_ln[4];
    for(Listenelement<Flaeche> *a_LE_fl = m_flaechenLst->GetErstesListenelement(); a_LE_fl != NULL; a_LE_fl = a_LE_fl->GetNachfolger())
    {
        for(int k = 0; k<4; k++)
        {
            neu_ln[k] = NULL;
        }
        Liste<Linie> *n_ln_lst = neuLayer->HoleLinien();
        if(a_LE_fl->GetElement()->HoleTyp() == RUZ_Dreieck)
        {
            for(int k = 0; k < 3; k++)
            {
                a_ln[k] = m_linienLst->Wert(a_LE_fl->GetElement()->HoleLinie(k));
            }
            for(Listenelement<Linie> *t_LE_ln = n_ln_lst->GetErstesListenelement(); t_LE_ln != NULL; t_LE_ln = t_LE_ln->GetNachfolger())
            {
                for(int k = 0; k < 3; k++)
                {
                    if(t_LE_ln->Wert() == a_ln[k])
                    {
                        neu_ln[k] = t_LE_ln->GetElement();
                    }
                }
                if(neu_ln[0] && neu_ln[1] && neu_ln[2])
                {
                    Dreieck::NeuesDreieck(neu_ln[0], neu_ln[1], neu_ln[2]);
                    break;
                }
            }
        }else
        if(a_LE_fl->GetElement()->HoleTyp() == RUZ_Viereck)
        {
            for(int k = 0; k < 4; k++)
            {
                a_ln[k] = m_linienLst->Wert(a_LE_fl->GetElement()->HoleLinie(k));
            }
            for(Listenelement<Linie> *t_LE_ln = n_ln_lst->GetErstesListenelement(); t_LE_ln != NULL; t_LE_ln = t_LE_ln->GetNachfolger())
            {
                for(int k = 0; k < 4; k++)
                {
                    if(t_LE_ln->Wert() == a_ln[k])
                    {
                        neu_ln[k] = t_LE_ln->GetElement();
                    }
                }
                if(neu_ln[0] && neu_ln[1] && neu_ln[2] && neu_ln[3])
                {
                    Viereck::NeuesViereck(neu_ln[0], neu_ln[1], neu_ln[2], neu_ln[3]);
                    break;
                }
            }
        }
    }

    for(HoehenMarke *a_HM = m_hoehenMarkenLst->GetErstesElement(); a_HM != NULL; a_HM = m_hoehenMarkenLst->GetNaechstesElement())
    {
        new HoehenMarke(a_HM->HolePosition(), neuLayer);
    }
    return neuLayer;
}

void RUZ_Layer::LinienNachLaengeSortieren()
{
    Listenelement<Linie> *laeufer = m_linienLst->GetErstesListenelement();

    while(laeufer != NULL)
    {
        if(laeufer->GetElement()->IstGeschuetzt())
        {
			laeufer->Wert(-1/(laeufer->GetElement()->ProjLaenge(z)));
            //laeufer->Wert(-1.0);
        }else{
            laeufer->Wert(laeufer->GetElement()->ProjLaenge(z));
        }
        laeufer = laeufer->GetNachfolger();
    };
    m_linienLst->ListeNachWertSortieren();
    return;
}

#ifdef DREICK_SUCHE_LINIE
void RUZ_Layer::DreieckeFinden(void)
{
  Punkt *p0, *p1, *p2;
  Liste<Linie> *ln_Sammlung0, *ln_Sammlung1;
  for(Linie *ln_laeufer = m_linienLst->GetErstesElement(); ln_laeufer != NULL; ln_laeufer = m_linienLst->GetNaechstesElement())
    ln_laeufer->SetzeBesucht('n');//alle Linien als noch nicht besucht kennzeichnen

  for(Linie *ln_laeufer = m_linienLst->GetErstesElement(); ln_laeufer != NULL; ln_laeufer = m_linienLst->GetNaechstesElement())
  {
    p0 = ln_laeufer->HolePunkt(0);
    ln_Sammlung0 = p0->HoleLinien();

    p1 = ln_laeufer->HolePunkt(1);
    ln_Sammlung1 = p1->HoleLinien();

    for(Linie* ln_p0 = ln_Sammlung0->GetErstesElement(); ln_p0 != NULL; ln_p0 = ln_Sammlung0->GetNaechstesElement())
    {
      if(ln_p0 == ln_laeufer)continue;
      for(Linie* ln_p1 = ln_Sammlung1->GetErstesElement(); ln_p1 != NULL; ln_p1 = ln_Sammlung1->GetNaechstesElement())
      {
        if(ln_p1 == ln_laeufer)continue;
        /*HIER WEITER: ACHTUNG BAUSTELLE*/
      }
    }
  }
}
#else
void RUZ_Layer::DreieckeFinden(void)
{
    Punkt *p_laeufer, *p2, *p3;
    Vektor dp2, dp3;
    Linie *li_laeufer;
    Liste<Linie> *li_sammlung, *li_sammlung_2;
    Listenelement<Linie> *LE_naechster, *LE_laeufer;
    double kreuzInZ;

    /*Linien nach Orientierung sortieren*/
    for(p_laeufer = m_punktLst->GetErstesElement(); p_laeufer != NULL; p_laeufer = m_punktLst->GetNaechstesElement())
    {
        li_sammlung = p_laeufer->HoleLinien();
        p_laeufer->SetzeBesucht('n');

        //Listenelement<Linie> *max_o, *max_u, *min_o, *min_u;
        //max_o = max_u = min_o = min_u = NULL;

        for(LE_laeufer = li_sammlung->GetErstesListenelement(); LE_laeufer != NULL; LE_laeufer = LE_laeufer->GetNachfolger())
        {
            li_laeufer = LE_laeufer->GetElement();
            p2 = li_laeufer->HolePunkt(0);
            if(p2 == p_laeufer) p2 = li_laeufer->HolePunkt(1);

            double cosinus = (p2->HolePosition().x() - p_laeufer->HolePosition().x()) /
                            sqrt(pow((p2->HolePosition().x() - p_laeufer->HolePosition().x()), 2) + pow((p2->HolePosition().y() - p_laeufer->HolePosition().y()), 2));

            double richtung = p2->HolePosition().y() - p_laeufer->HolePosition().y();
            if(richtung > 0)/*Linie liegt oberhalb der x-Achse*/
            {
                cosinus = cosinus - 1.0;
            }else/*Linie liegt unterhalb der x-Achse*/
            {
                cosinus = 1.0 - cosinus;
            }
            LE_laeufer->Wert(cosinus);
        }
        li_sammlung->ListeNachWertSortieren();
    }
    /*ENDE Linien nach Orientierung sortieren*/

    /*Verbindungslinien suchen*/
    Linie *testLinie;
    for(p_laeufer = m_punktLst->GetErstesElement(); p_laeufer != NULL; p_laeufer = m_punktLst->GetNaechstesElement())
    {
        li_sammlung = p_laeufer->HoleLinien();
        if(li_sammlung->GetListenGroesse() < 2)
        {
            continue;//wenn um Punkt nur eine Linie vorhanden -> zum nächsten Punkt gehen
        }
        Listenelement<Linie> *LE_erster = li_sammlung->GetErstesListenelement();
        for(LE_laeufer = LE_erster; LE_laeufer != NULL; LE_laeufer = LE_laeufer->GetNachfolger())
        {
            if((li_sammlung->GetListenGroesse() == 2)&&(LE_laeufer != LE_erster)&&(LE_laeufer->GetElement()->HoleFlaechenZahl()))
            {
                /*Verhindert, dass um einen Punkt mit zwei Linien zwei Dreiecke erzeugt werden*/
                continue;
            }
            LE_naechster = LE_laeufer->GetNachfolger();
            if(LE_naechster == NULL) LE_naechster = LE_erster;/*Letzte Linie mit der ersten pruefen*/

            p2 = LE_laeufer->GetElement()->HolePunkt(0);
            if(p2 == p_laeufer)p2 = LE_laeufer->GetElement()->HolePunkt(1);

            p3 = LE_naechster->GetElement()->HolePunkt(0);
            if(p3 == p_laeufer)p3 = LE_naechster->GetElement()->HolePunkt(1);

            /*Prüfen, ob eingeschlossener Winkel größer 180° ist*/
            dp2 = p2->HolePosition() - p_laeufer->HolePosition();
            dp3 = p3->HolePosition() - p_laeufer->HolePosition();
            kreuzInZ = dp2.x() * dp3.y() - dp2.y() * dp3.x();
            if(kreuzInZ > 0)
            {
                continue;
            }
            /*ENDE Prüfen, ob eingeschlossener Winkel größer 180° ist*/

            if((p2->HoleBesucht()=='n')&&(p3->HoleBesucht()=='n'))
            {
                li_sammlung_2 = p2->HoleLinien();
                for(Listenelement<Linie> * LE_innerer_laeufer = li_sammlung_2->GetErstesListenelement();
                    LE_innerer_laeufer != NULL; LE_innerer_laeufer = LE_innerer_laeufer->GetNachfolger())
                {
                    testLinie = LE_innerer_laeufer->GetElement();
                    if(((testLinie->HolePunkt(0) == p2)&&(testLinie->HolePunkt(1) == p3)) ||
                       ((testLinie->HolePunkt(1) == p2)&&(testLinie->HolePunkt(0) == p3)))
                    {
                        Dreieck* tempDreieck = Dreieck::NeuesDreieck((LE_laeufer->GetElement()), (LE_naechster->GetElement()), testLinie);
                        if(tempDreieck != NULL)
                        {
                          logSchreiben("Dreieck gefunden und erzeugt\n");
                          if(FlaecheVorhanden(tempDreieck))
                          {
                              delete tempDreieck;
                              logSchreiben("Dreieck gab es schon: gelöscht\n");
                          }
                        }
                    }
                }
            }
        }
        p_laeufer->SetzeBesucht('j');
    }
    //ElementlisteAusgeben();
    return;
}
#endif

void RUZ_Layer::ViereckeFinden(void)
{
    Linie *aktLinie;
    Linie *vergleichLinie[4];
    Flaeche *anliegerFlaeche[2];

    for(Listenelement<Linie> *li_LiEl_laeufer = m_linienLst->GetErstesListenelement(); li_LiEl_laeufer != NULL; li_LiEl_laeufer = li_LiEl_laeufer->GetNachfolger())
    {
        li_LiEl_laeufer->Wert(-1.0);
        aktLinie = li_LiEl_laeufer->GetElement();
        if(aktLinie == NULL)continue;
        /*Linien am Rand suchen*/

        if(aktLinie->IstGeschuetzt())
        {
            aktLinie->SetzeKantenklasse(gesperrt);
            continue;
        }
        if(aktLinie->HoleFlaechenZahl() < 2)
        {
            aktLinie->SetzeKantenklasse(BE);
            continue;
        }

        anliegerFlaeche[0] = aktLinie->HoleFlaechen()->GetErstesElement();
        anliegerFlaeche[1] = aktLinie->HoleFlaechen()->GetNaechstesElement();
        if((anliegerFlaeche[0] == NULL)||(anliegerFlaeche[1] == NULL))
        {
            aktLinie->SetzeKantenklasse(BE);
            continue;
        }
        if((anliegerFlaeche[0]->HoleTyp() == RUZ_Viereck)||(anliegerFlaeche[0]->HoleTyp() == RUZ_Viereck))
        {
            aktLinie->SetzeKantenklasse(BE);
            continue;
        }

        aktLinie->SetzeKantenklasse(RE);

        /*Abweichung der Innenwinkel(summen) aneinanderliegender Dreiecke von 90° bestimmen*/
        for (int i = 0; i < 2; i++)
        {
            for(int k = 0; k < 3; k++)
            {
                if(anliegerFlaeche[i]->HoleLinie(k) == aktLinie)
                {
                    vergleichLinie[2*i] = anliegerFlaeche[i]->HoleLinie((k+1)%3);
                    vergleichLinie[2*i+1] = anliegerFlaeche[i]->HoleLinie((k+2)%3);
                    if((vergleichLinie[2*i]->HolePunkt(0) != aktLinie->HolePunkt(i))&&(vergleichLinie[2*i]->HolePunkt(1) != aktLinie->HolePunkt(i)))
                    {
                        Linie *tempLinie = vergleichLinie[2*i];
                        vergleichLinie[2*i] = vergleichLinie[2*i+1];
                        vergleichLinie[2*i+1] = tempLinie;
                    }
                }
            }
        }
        double alpha, beta, gamma, delta, a, b, c;

        /*alpha*/
        a = aktLinie->ProjLaenge(z);
        b = vergleichLinie[0]->ProjLaenge(z);
        c = vergleichLinie[1]->ProjLaenge(z);
        alpha = acos((a*a+b*b-c*c)/(2*a*b));
        b = vergleichLinie[3]->ProjLaenge(z);
        c = vergleichLinie[2]->ProjLaenge(z);
        alpha += acos((a*a+b*b-c*c)/(2*a*b));
        if(alpha >= PI)continue;/*Winkel > 180° => ueberstumpfer Innenwinkel fuer Quad unbrauchbar*/
        /*ENDE alpha*/
        /*beta*/
        b = vergleichLinie[1]->ProjLaenge(z);
        c = vergleichLinie[0]->ProjLaenge(z);
        beta = acos((a*a+b*b-c*c)/(2*a*b));
        b = vergleichLinie[2]->ProjLaenge(z);
        c = vergleichLinie[3]->ProjLaenge(z);
        beta += acos((a*a+b*b-c*c)/(2*a*b));
        if(beta >= PI)continue;/*Winkel > 180° => ueberstumpfer Innenwinkel fuer Quad unbrauchbar*/
        /*ENDE beta*/
        /*gamma*/
        c = aktLinie->ProjLaenge(z);
        a = vergleichLinie[0]->ProjLaenge(z);
        b = vergleichLinie[1]->ProjLaenge(z);
        gamma = acos((a*a+b*b-c*c)/(2*a*b));
        /*ENDE gamma*/
        /*delta*/
        a = vergleichLinie[2]->ProjLaenge(z);
        b = vergleichLinie[3]->ProjLaenge(z);
        delta = acos((a*a+b*b-c*c)/(2*a*b));
        /*ENDE delta*/
        double sigmaQuadrat = (pow(alpha-PI/2, 2) + pow(beta-PI/2, 2) + pow(gamma-PI/2, 2) + pow(delta-PI/2, 2)) / 4;
        li_LiEl_laeufer->Wert(sigmaQuadrat);
        /*ENDE Abweichung der Innenwinkel(summen) aneinanderliegender Dreiecke von 90° bestimmen*/
    }
    m_linienLst->ListeNachWertSortieren();

    for(Listenelement<Linie> *li_LiEl_laeufer = m_linienLst->GetErstesListenelement(); li_LiEl_laeufer != NULL; li_LiEl_laeufer = li_LiEl_laeufer->GetNachfolger())
    {
        if(li_LiEl_laeufer->Wert() < 0)continue;
        aktLinie = li_LiEl_laeufer->GetElement();
        if(aktLinie == NULL)
        {
            continue;
        }
        if(aktLinie->HoleKantenklasse() == BE)continue;

        Liste<Flaeche>* t_FlLst = aktLinie->HoleFlaechen();
        anliegerFlaeche[0] = t_FlLst->GetErstesElement();
        anliegerFlaeche[1] = t_FlLst->GetNaechstesElement();
        if((anliegerFlaeche[0] == NULL)||(anliegerFlaeche[1] == NULL))
        {
            continue;
        }

        for (int i = 0; i < 2; i++)
        {
            for(int k = 0; k < 3; k++)
            {
                if(anliegerFlaeche[i]->HoleLinie(k) == aktLinie)
                {
                    vergleichLinie[2*i] = anliegerFlaeche[i]->HoleLinie((k+1)%3);
                    vergleichLinie[2*i+1] = anliegerFlaeche[i]->HoleLinie((k+2)%3);
                }
            }
        }
        aktLinie->SetzeKantenklasse(DE);
        vergleichLinie[0]->SetzeKantenklasse(BE);
        vergleichLinie[1]->SetzeKantenklasse(BE);
        vergleichLinie[2]->SetzeKantenklasse(BE);
        vergleichLinie[3]->SetzeKantenklasse(BE);

        if((anliegerFlaeche[0]->HoleTyp() == RUZ_Dreieck)&&(anliegerFlaeche[1]->HoleTyp() == RUZ_Dreieck))
        {
            Viereck::NeuesViereck((Dreieck*)anliegerFlaeche[0], (Dreieck*)anliegerFlaeche[1]);
        }else{
            continue;
        }
    }
    return;
}

void RUZ_Layer::SetzeSichtbarkeit(bool sbk)
{
    sichtbar = sbk;
    return;
}

void RUZ_Layer::Hinzufuegen(Punkt* obj)
{
    m_punktLst->ExklusivHinzufuegen(obj);
    obj->SetzeLayer(this);
	return;
}

void RUZ_Layer::Wert(Punkt* obj, double wert)
{
    for(Listenelement<Punkt> *t_LE_pkt = m_punktLst->GetErstesListenelement(); t_LE_pkt != NULL; t_LE_pkt = t_LE_pkt->GetNachfolger())
    {
        if(t_LE_pkt->GetElement() == obj)
        {
            t_LE_pkt->Wert(wert);
            break;
        }
    }
    return;
}

void RUZ_Layer::Hinzufuegen(Linie* obj, bool exklusiv)
{
    if (exklusiv) {
		m_linienLst->ExklusivHinzufuegen(obj);
		return;
	}
	m_linienLst->Hinzufuegen(obj);
    return;
}

void RUZ_Layer::Wert(Linie* obj, double wert)
{
    for(Listenelement<Linie> *t_LE_ln = m_linienLst->GetErstesListenelement(); t_LE_ln != NULL; t_LE_ln = t_LE_ln->GetNachfolger())
    {
        if(t_LE_ln->GetElement() == obj)
        {
            t_LE_ln->Wert(wert);
            break;
        }
    }
    return;
}

void RUZ_Layer::Hinzufuegen(Flaeche* obj)
{
    m_flaechenLst->ExklusivHinzufuegen(obj);
    GeaenderteFlaecheHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(HoehenMarke* obj)
{
    m_hoehenMarkenLst->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(GefaelleMarke* obj)
{
    m_gefaelleMarkenLst->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(Strich* obj)
{
    m_strichLst->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(Bogen* obj)
{
    m_bogenLst->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(Kreis* obj)
{
    m_kreisLst->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::Hinzufuegen(Fangpunkt* obj)
{
    m_fngPktLst->ExklusivHinzufuegen(obj);
    return;
}
void RUZ_Layer::Hinzufuegen(Vektor* Fehler)
{
    m_fehlerMarkierung->ExklusivHinzufuegen(Fehler);
    return;
}

void RUZ_Layer::Entfernen(Punkt* obj)
{
    m_punktLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Linie* obj)
{
    m_linienLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Flaeche* obj)
{
    m_aktualisierungsFlaechen->Entfernen(obj);
    m_flaechenLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Strich* obj)
{
    m_strichLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Bogen* obj)
{
    m_bogenLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Kreis* obj)
{
    m_kreisLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(Fangpunkt* obj)
{
    m_fngPktLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(HoehenMarke* obj)
{
    m_hoehenMarkenLst->Entfernen(obj);
    return;
}

void RUZ_Layer::Entfernen(GefaelleMarke* obj)
{
    m_gefaelleMarkenLst->Entfernen(obj);
    return;
}

void RUZ_Layer::FehlerEntfernen(void)
{
    m_fehlerMarkierung->ListeLoeschen("FehlerEntfernen");
    return;
}

void RUZ_Layer::EntferneTieferes(Achse projektion)
{
    Listenelement<Punkt> *pkt_LE_laeufer;
    Punkt *pkt_laeufer;

    Vektor tempOrt;
    for(pkt_LE_laeufer = m_punktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL;)
    {
        pkt_laeufer = pkt_LE_laeufer->GetElement();
        pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();/*Achtung: kann NULL sein*/
        for(Flaeche* aktFl = m_flaechenLst->GetErstesElement(); aktFl != NULL; aktFl = m_flaechenLst->GetNaechstesElement())
        {
            if(aktFl->IstEckPunkt(pkt_laeufer))
            {
                continue;
            }

            tempOrt = pkt_laeufer->HolePosition();

            if(aktFl->OrtAufFlaeche(tempOrt, projektion))
            {
                if(tempOrt.GetKoordinaten(projektion) > pkt_laeufer->HolePosition().GetKoordinaten(projektion))
                {
                    delete pkt_laeufer;
                    break;
                }
            }
        }
    }
    return;
}

void RUZ_Layer::EntferneHoeheres(Achse projektion)
{
    Listenelement<Punkt> *pkt_LE_laeufer;
    Punkt *pkt_laeufer;

    Vektor tempOrt;
    for(pkt_LE_laeufer = m_punktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL;)
    {
        pkt_laeufer = pkt_LE_laeufer->GetElement();
        pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger();/*Achtung: kann NULL sein*/
        for(Flaeche* aktFl = m_flaechenLst->GetErstesElement(); aktFl != NULL; aktFl = m_flaechenLst->GetNaechstesElement())
        {
            if(aktFl->IstEckPunkt(pkt_laeufer))
            {
                continue;
            }

            tempOrt = pkt_laeufer->HolePosition();

            if(aktFl->OrtAufFlaeche(tempOrt, projektion))
            {
                if(tempOrt.GetKoordinaten(projektion) < pkt_laeufer->HolePosition().GetKoordinaten(projektion))
                {
                    delete pkt_laeufer;
                    break;
                }
            }
        }
    }
    return;
}

void RUZ_Layer::LoescheDoppeltePunkte(int genauigkeit)
{
    LoescheDoppeltePunkte(m_punktLst, genauigkeit);
    return;
}

void RUZ_Layer::LoescheDoppeltePunkte(Liste<Punkt>* pktLst, int genauigkeit)
{
    Listenelement<Punkt> *pkt_LE_laeufer, *pkt_LE_naechster;
    Punkt *pkt_laeufer, *pkt_naechster;
    double genauigkeitPot = pow(10, -(genauigkeit));

    for(pkt_LE_laeufer = pktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL; pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger())
    {
        pkt_LE_laeufer->Wert(pkt_LE_laeufer->GetElement()->HolePosition().x());
    }
    pktLst->ListeNachWertSortieren();

    for(pkt_LE_laeufer = pktLst->GetErstesListenelement(); pkt_LE_laeufer != NULL; pkt_LE_laeufer = pkt_LE_laeufer->GetNachfolger())
    {
        pkt_laeufer = pkt_LE_laeufer->GetElement();
        for(pkt_LE_naechster = pkt_LE_laeufer->GetNachfolger(); pkt_LE_naechster != NULL; pkt_LE_naechster = pkt_LE_naechster->GetNachfolger())
        {
            pkt_naechster = pkt_LE_naechster->GetElement();
            if(pkt_naechster)
            {
                if(abs(pkt_laeufer->HolePosition().x() - pkt_naechster->HolePosition().x()) < genauigkeitPot)
                {
                    if(abs(pkt_laeufer->HolePosition().y() - pkt_naechster->HolePosition().y()) < genauigkeitPot)
                    {
                        if(abs(pkt_laeufer->HolePosition().z() - pkt_naechster->HolePosition().z()) < genauigkeitPot)
                        {
                            pkt_LE_naechster = pkt_LE_naechster->GetVorgaenger();
                            pktLst->Entfernen(pkt_naechster);
                            pkt_naechster->ErsetzenDurch(pkt_laeufer);
                        }
                    }
                }else{
                    break;
                }
            }
        }
    }
    return;
}

void RUZ_Layer::ViereckeTeilen(Liste<Flaeche>* flLst)
{
    Liste<Flaeche>* t_flLst;
    flLst == NULL ? t_flLst = m_flaechenLst : t_flLst = flLst;

    for(Flaeche* aktFl = t_flLst->GetErstesElement(); aktFl; aktFl = t_flLst->GetNaechstesElement())
    {
        if(aktFl->HoleTyp() == RUZ_Viereck)
        {
            (static_cast<Viereck*>(aktFl))->Teilen();
        }
    }
    return;
}

void RUZ_Layer::VorhandeneLinienSchuetzen(bool geschuetzt)
{
    for(Linie *tempLinie = m_linienLst->GetErstesElement(); tempLinie != NULL; tempLinie = m_linienLst->GetNaechstesElement())
    {
        tempLinie->SetzeGeschuetzt(geschuetzt);
    }
    return;
}

void RUZ_Layer::LinienAusStrichen(double genauigkeit, Achse proj_Richtung)
{
    Punkt *p1, *p2;
    for(Strich* t_strich = m_strichLst->GetErstesElement(); t_strich != NULL; t_strich = m_strichLst->GetNaechstesElement())
    {
        p1 = p2 = NULL;
        for(Punkt* t_pkt = m_punktLst->GetErstesElement(); t_pkt != NULL; t_pkt = m_punktLst->GetNaechstesElement())
        {
            if(t_pkt->IstNahebei(t_strich->Xa(), t_strich->Ya(), genauigkeit, proj_Richtung))p1 = t_pkt;
            if(t_pkt->IstNahebei(t_strich->Xe(), t_strich->Ye(), genauigkeit, proj_Richtung))p2 = t_pkt;
        }
        if((p1!=NULL)&&(p2!=NULL))
        {
           Linie::NeueLinie(p1, p2);
           delete t_strich;
        }
    }
    return;
}

void RUZ_Layer::RandAbschneiden(RUZ_Layer* andererLayer, int genauigkeit)
{
    /*Alle Randlinien des einen Layers mit den Flächen des anderen verschneiden*/
    Vektor pos;

    Liste<Linie>* lnLst[2];
    lnLst[0] = this->HoleLinien();
    lnLst[1] = andererLayer->HoleLinien();

    Liste<Linie>* schnittFuehrung[2];
    schnittFuehrung[0] = new Liste<Linie>;
    schnittFuehrung[1] = new Liste<Linie>;

    RUZ_Layer* schnittLayer[2];
    schnittLayer[0] = new RUZ_Layer("Hilfslayer0");
    schnittLayer[1] = new RUZ_Layer("Hilfslayer1");

    for(int i = 0; i < 2; i++)
    {
        for(Linie* tempLn = lnLst[i]->GetErstesElement(); tempLn; tempLn = lnLst[i]->GetNaechstesElement())
        {
            if(tempLn->HoleFlaechen()->GetListenGroesse() <= 1)
            {
                Linie::NeueLinie(new Punkt(tempLn->HolePunkt(0)->HolePosition(), schnittLayer[i]),
                                 new Punkt(tempLn->HolePunkt(1)->HolePosition(), schnittLayer[i]));
            }
        }
        schnittLayer[i]->LoescheDoppeltePunkte(genauigkeit);
        /*!!!!*/lnLst[i] = schnittLayer[i]->HoleLinien();
        for(Linie* aktLn = lnLst[i]->GetErstesElement(); aktLn; aktLn = lnLst[i]->GetNaechstesElement())
        {
            schnittFuehrung[i]->Hinzufuegen(aktLn);
        }
    }
    thread_info_verschnitt thInf(this, andererLayer);
    EntlangLinienSchneiden(schnittFuehrung[1], &thInf);
    for(int i = 0; i < 2; i++)
    {
        delete schnittFuehrung[i];
        delete schnittLayer[i];
    }
    LoescheDoppeltePunkte(genauigkeit);
    /*ENDE Alle Randlinien des einen Layers mit den Flächen des anderen verschneiden*/
    return;
}

void RUZ_Layer::LoescheFreiliegendeFlaechen(RUZ_Layer* andererLayer)/*Löscht Flächen, deren Schwerpunkt nicht innerhalb von Flächen in andererLayer liegen*/
{
    /*Alle Flächen löschen, die nicht über oder unter einer Fläche des anderen Layes liegen*/
    Liste<Flaeche>* flLst_eins = andererLayer->HoleFlaechen();

    for(Flaeche* aktFl = m_flaechenLst->GetErstesElement(); aktFl; aktFl = m_flaechenLst->GetNaechstesElement())
    {
        bool innerhalb = false;
        Vektor schwerPkt = aktFl->Schwerpunkt();
        for(Flaeche* t_fl = flLst_eins->GetErstesElement(); t_fl; t_fl = flLst_eins->GetNaechstesElement())
        {
            if(t_fl->IstInnerhalb(schwerPkt.x(), schwerPkt.y(), z))
            {
                innerhalb = true;
                break;
            }
        }
        if(!innerhalb)delete aktFl;
    }
    for(Linie* ln_laeufer = m_linienLst->GetErstesElement(); ln_laeufer; ln_laeufer = m_linienLst->GetNaechstesElement())
    {
        if(ln_laeufer->HoleFlaechen()->GetListenGroesse() == 0)delete ln_laeufer;
    }
    for(Punkt* t_pkt = m_punktLst->GetErstesElement(); t_pkt; t_pkt = m_punktLst->GetNaechstesElement())
    {
        if(t_pkt->HoleLinien()->GetListenGroesse() == 0)delete t_pkt;
    }
    /*ENDE Alle Flächen löschen, die nicht über oder unter einer Fläche des anderen Layes liegen*/
}

void RUZ_Layer::EntlangLinienSchneiden(Liste<Linie>* schnittLinien, thread_info_verschnitt *thInf)
{
    thInf->SetzeGesamtzahl(schnittLinien->GetListenGroesse());
    thInf->NulleBearbeitet();
    /*Alle Linien der Liste mit den Flächen des Layers verschneiden*/
    Punkt *teilPunkt;
    Vektor pos;

    for(Linie* tempLn = m_linienLst->GetErstesElement(); tempLn; tempLn = m_linienLst->GetNaechstesElement())
    {
        tempLn->SetzeBesucht('n');
    }
    for(Flaeche* tempFl = m_flaechenLst->GetErstesElement(); tempFl; tempFl = m_flaechenLst->GetNaechstesElement())
    {
        tempFl->SetzeBesucht('n');
    }

    for(Linie* schneideLinie = schnittLinien->GetErstesElement(); schneideLinie; schneideLinie = schnittLinien->GetNaechstesElement())
    {
      /*Thread Kontrollpunkt*/
      thInf->InkrementBearbeitet();
      if(thInf->BeendenAngefragt())
      {
        thInf->BeendigungFeststellen();
        return;
      }
      for(int k = 0; k < 2; k++)//Alle Endpunkte der Randlinien teilen die Flächen über oder unter Ihnen auf dem anderen Layer
      {
        pos = schneideLinie->HolePunkt(k)->HolePosition();
        teilPunkt = new Punkt(pos, this);
        if (teilPunkt == NULL)continue;
        for(Flaeche* fl_laeufer = m_flaechenLst->GetErstesElement(); fl_laeufer; fl_laeufer = m_flaechenLst->GetNaechstesElement())
        {
            /*Thread Kontrollpunkt*/
            if(thInf->BeendenAngefragt())
            {
              thInf->BeendigungFeststellen();
              return;
            }
            if(fl_laeufer->HoleBesucht() == 'j')
            {
              continue;
            }
            fl_laeufer->Teilen(teilPunkt, z, true);
        }
        if(teilPunkt->HoleLinien()->GetListenGroesse() == 0)delete teilPunkt;//Wenn am Punkt keine Linien hängen, hat er auch keine Fläche geteilt -> löschen
        for(Flaeche* fl_laeufer = m_flaechenLst->GetErstesElement(); fl_laeufer; fl_laeufer = m_flaechenLst->GetNaechstesElement())
        {
            fl_laeufer->SetzeBesucht('n');
        }
      }

      for(Linie* ln_laeufer = m_linienLst->GetErstesElement(); ln_laeufer; ln_laeufer = m_linienLst->GetNaechstesElement())
      {
        if(ln_laeufer->HoleBesucht() == 'j')
        {
          continue;
        }
        /*Thread Kontrollpunkt*/
        if(thInf->BeendenAngefragt())
        {
          thInf->BeendigungFeststellen();
          return;
        }
        if(ln_laeufer->schneidet(schneideLinie, pos, z))//Alle Randlinien werden mit den Randlinien des anderen Layers verschnitten
        {
          teilPunkt = new Punkt(pos, this);
          teilPunkt->SetzeBesucht('j');
          ln_laeufer->Teilen(teilPunkt, z, true);
          if(teilPunkt->HoleLinien()->GetListenGroesse() == 0)delete teilPunkt;//Wenn das Teilen der Linie nicht erfolgreich war, Punkt wieder löschen
        }
      }
      for(Linie* tempLn = m_linienLst->GetErstesElement(); tempLn; tempLn = m_linienLst->GetNaechstesElement())
      {
        tempLn->SetzeBesucht('n');
      }
      for(Flaeche* tempFl = m_flaechenLst->GetErstesElement(); tempFl; tempFl = m_flaechenLst->GetNaechstesElement())
      {
        tempFl->SetzeBesucht('n');
      }
    }
    return;
}

void RUZ_Layer::Verschneiden(RUZ_Layer *andererLayer, thread_info_verschnitt *thInf, int *genauigkeit)
{
    RUZ_Layer *layer1, *layer2;
    layer1 = this;
    layer2 = andererLayer;

    RUZ_Layer *randLayer1, *randLayer2, *aktRandLayer;
    RUZ_Layer *hilfsLayer;
    thInf->HoleLayer(&hilfsLayer, &randLayer1, &randLayer2, &layer1, &layer2);
    logSchreiben("RUZ_Layer::Verschneiden\nhilfsLayer: %p\nrandLayer1: %p\nrandLayer2: %p\n", hilfsLayer, randLayer1, randLayer2);

    Liste<Flaeche> *flLst1, *flLst2;

    /*Alle Vierecke in Dreiecke teilen*/
    layer1->ViereckeTeilen();
    layer2->ViereckeTeilen();

    flLst1 = layer1->HoleFlaechen();
    flLst2 = layer2->HoleFlaechen();

    SchnittlinienFinden(flLst1, flLst2, hilfsLayer, thInf);/*Hier passiert viel - hieraus auch einen Thread machen*/
    if(thInf->IstBeendet())return;
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }
    logSchreiben("Schnittlinien gefunden (%d Stueck)", hilfsLayer->HoleLinien()->GetListenGroesse());

    Liste<Linie> *lnLst;
    for(int i = 0; i < 2; i++)
    {
        (i == 0) ? (lnLst = layer1->HoleLinien()) : (lnLst = layer2->HoleLinien());
        (i == 0) ? (aktRandLayer = randLayer1) : (aktRandLayer = randLayer2);
        for(Linie *aktLn = lnLst->GetErstesElement(); aktLn; aktLn = lnLst->GetNaechstesElement())
        {
            if(aktLn->HoleFlaechen()->GetListenGroesse() <= 1)
            {
                Linie::NeueLinie(new Punkt(aktLn->HolePunkt(0)->HolePosition(), aktRandLayer), new Punkt(aktLn->HolePunkt(1)->HolePosition(), aktRandLayer));
            }
        }
        aktRandLayer->LoescheDoppeltePunkte(*genauigkeit);
    }

    hilfsLayer->LoescheDoppeltePunkte(*genauigkeit);

    thInf->SetzeStatus(0);
    layer1->EntlangLinienSchneiden(hilfsLayer->HoleLinien(), thInf);
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }
    layer1->LoescheDoppeltePunkte(*genauigkeit);

    thInf->SetzeStatus(1);
    layer1->EntlangLinienSchneiden(randLayer2->HoleLinien(), thInf);
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }
    layer1->LoescheDoppeltePunkte(*genauigkeit);

    thInf->SetzeStatus(2);
    layer2->EntlangLinienSchneiden(hilfsLayer->HoleLinien(), thInf);
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }
    layer2->LoescheDoppeltePunkte(*genauigkeit);

    thInf->SetzeStatus(3);
    layer2->EntlangLinienSchneiden(randLayer1->HoleLinien(), thInf);
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }
    layer2->LoescheDoppeltePunkte(*genauigkeit);

    thInf->SetzeStatus(4);
    if(thInf->IstBeendet())return;
    if(thInf->BeendenAngefragt())
    {
      thInf->BeendigungFeststellen();
      return;
    }

    layer2->LoescheFreiliegendeFlaechen(layer1);
    layer1->LoescheFreiliegendeFlaechen(layer2);

    thInf->BeendigungFeststellen();/*Beendet modalen Dialog*/
    return;
}

void RUZ_Layer::VerschneideFlaechen(Liste<Flaeche>* flLst, int genauigkeit)
{
    Liste<Flaeche>* t_flLst;
    flLst == NULL ? t_flLst = m_flaechenLst : t_flLst = flLst;

    RUZ_Layer *hilfsLayer = new RUZ_Layer("Hilfslayer");

    /*Alle Vierecke in Dreiecke teilen*/
    ViereckeTeilen(t_flLst);

    thread_info_verschnitt thInf(this, flLst->GetErstesElement()->HoleLayer());
    SchnittlinienFinden(t_flLst, t_flLst, hilfsLayer, &thInf);/*Hier passiert viel*/

    hilfsLayer->LoescheDoppeltePunkte(genauigkeit);

    if(t_flLst != m_flaechenLst)t_flLst->ListeLeeren("");
    EntlangLinienSchneiden(hilfsLayer->HoleLinien(), &thInf);
    LoescheDoppeltePunkte(genauigkeit);

    delete hilfsLayer;

    return;
}

void RUZ_Layer::SchnittlinienFinden(Liste<Flaeche>* lst_1, Liste<Flaeche>* lst_2, RUZ_Layer* zwSp, thread_info_verschnitt *thInf)
{
    Liste<Flaeche> *flLst1, *flLst2;
    RUZ_Layer* hilfsLayer;

    flLst1 = lst_1;
    flLst2 = lst_2;
    hilfsLayer = zwSp;

    int eckZahl1, eckZahl2, pktZaehler;
    Punkt *t_pkt[2];
    Vektor tempOrt1, tempOrt2;

    Flaeche *aktFl1, *aktFl2;

    /*Schnittlinien finden*/
    for(Listenelement<Flaeche> *akt_LE_Fl1 = flLst1->GetErstesListenelement(); akt_LE_Fl1; akt_LE_Fl1 = akt_LE_Fl1->GetNachfolger())
    {
      if(thInf->BeendenAngefragt())//Abbruch angefragt
      {
        thInf->BeendigungFeststellen();
        return;
      }
      aktFl1  = akt_LE_Fl1->GetElement();
      aktFl1->HoleTyp() == RUZ_Dreieck ? eckZahl1=3 : eckZahl1=4;
      for(Listenelement<Flaeche> *akt_LE_Fl2 = flLst2->GetErstesListenelement(); akt_LE_Fl2; akt_LE_Fl2 = akt_LE_Fl2->GetNachfolger())
      {
        if(thInf->BeendenAngefragt())//Abbruch angefragt
        {
          thInf->BeendigungFeststellen();
          return;
        }
          aktFl2 = akt_LE_Fl2->GetElement();
          if(aktFl1 == aktFl2)continue;
          if(aktFl1->TeiltLinieMit(aktFl2))continue;
          t_pkt[0] = t_pkt[1] = NULL;
          pktZaehler = 0;
          for(int i = 0; i < eckZahl1; i++)
          {
              if(aktFl2->DurchstossPunkt(aktFl1->HoleLinie(i), tempOrt1, tempOrt2))
              {
                  t_pkt[pktZaehler] = new Punkt(tempOrt1, hilfsLayer);
                  pktZaehler++;
              }
              if(pktZaehler > 1)break;
          }
          if(pktZaehler > 1)
          {
              Linie::NeueLinie(t_pkt[0], t_pkt[1]);
              continue;
          }

          aktFl2->HoleTyp() == RUZ_Dreieck ? eckZahl2=3 : eckZahl2=4;
          for(int i = 0; i < eckZahl2; i++)
          {
              if(aktFl1->DurchstossPunkt(aktFl2->HoleLinie(i), tempOrt1, tempOrt2))
              {
                  t_pkt[pktZaehler] = new Punkt(tempOrt1, hilfsLayer);
                  pktZaehler++;
                  if(pktZaehler > 1)
                  {
                      if((t_pkt[0])&&(t_pkt[1]))
                      {
                          if(t_pkt[0]->HolePosition() == t_pkt[1]->HolePosition())
                          {
                              pktZaehler--;
                          }
                      }
                  }
              }
              if(pktZaehler > 1)break;
          }
          if(pktZaehler > 1)
          {
              Linie::NeueLinie(t_pkt[0], t_pkt[1]);
          }
      }
  }
  /*ENDE Schnittlinien finden*/
  return;
}

Liste<Punkt>* RUZ_Layer::HolePunkte(void) const
{
    return m_punktLst;
}

Liste<Linie>* RUZ_Layer::HoleLinien(void) const
{
    return m_linienLst;
}

Liste<Flaeche>* RUZ_Layer::HoleFlaechen(void) const
{
    return m_flaechenLst;
}

Liste<HoehenMarke>* RUZ_Layer::HoleHoehenMarken(void) const
{
    return m_hoehenMarkenLst;
}

Liste<GefaelleMarke>* RUZ_Layer::HoleGefaelleMarken(void) const
{
    return m_gefaelleMarkenLst;
}

Liste<Vektor>* RUZ_Layer::HoleFehlermarken(void) const
{
    return m_fehlerMarkierung;
}

Liste<Strich>* RUZ_Layer::HoleStriche(void) const
{
    return m_strichLst;
}

Liste<Kreis>* RUZ_Layer::HoleKreise(void) const
{
    return m_kreisLst;
}

Liste<Fangpunkt>* RUZ_Layer::HoleFangpunkte(void) const
{
    return m_fngPktLst;
}

Liste<Bogen>* RUZ_Layer::HoleBoegen(void) const
{
    return m_bogenLst;
}

Linie* RUZ_Layer::Verbunden(Punkt *von, Punkt *nach)
{
    Liste<Linie> *vonLinien = von->HoleLinien();
    for(Linie *tempLinie = vonLinien->GetErstesElement(); tempLinie != NULL; tempLinie = vonLinien->GetNaechstesElement())
    {
        if(tempLinie->HolePunkt(0) == nach)return tempLinie;
        if(tempLinie->HolePunkt(1) == nach)return tempLinie;
    }
    return NULL;
}

bool RUZ_Layer::SindDiagonalEndpunkte(Punkt *von, Punkt *nach)
{
    for(Flaeche* tempFlaeche = m_flaechenLst->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = m_flaechenLst->GetNaechstesElement())
    {
        if(tempFlaeche->HoleTyp() == RUZ_Viereck)
        {
            for(int i = 0; i < 4; i++)
            {
                if(tempFlaeche->HolePunkt(i) == von)
                {
                    if(tempFlaeche->HolePunkt((i+2)%4) == nach)return true;
                }
            }
        }
    }
    return false;
}

bool RUZ_Layer::LinieDoppeltVorhanden(Linie *testLinie)
{
    for(Linie *tempLinie = m_linienLst->GetErstesElement(); tempLinie != NULL; tempLinie = m_linienLst->GetNaechstesElement())
    {
        if(*testLinie == *tempLinie)
        {
            if(testLinie != tempLinie)return true;
        }
    }
    return false;
}

Linie* RUZ_Layer::HoleLinie(Punkt* p1, Punkt* p2)
{
    Linie *tempLinie;
    for(tempLinie = m_linienLst->GetErstesElement(); tempLinie != NULL; tempLinie = m_linienLst->GetNaechstesElement())
    {
        for(int i = 0; i < 2; i++)
        {
            if((tempLinie->HolePunkt(i) == p1)&&(tempLinie->HolePunkt((i+1)%2) == p2))return tempLinie;
        }
    }
    tempLinie = Linie::NeueLinie(p1, p2);
    return tempLinie;
}

bool RUZ_Layer::PunktDoppeltVorhanden(Punkt *testPunkt)
{
    for(Punkt *tempPunkt = m_punktLst->GetErstesElement(); tempPunkt != NULL; tempPunkt = m_punktLst->GetNaechstesElement())
    {
        if(*testPunkt == *tempPunkt)
        {
            if(testPunkt != tempPunkt)
            return true;
        }
    }
    return false;
}

bool RUZ_Layer::FlaecheVorhanden(Flaeche *testFlaeche)
{
    for(Flaeche *tempFlaeche = m_flaechenLst->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = m_flaechenLst->GetNaechstesElement())
    {
        if(testFlaeche != tempFlaeche)/*Wenn die Instanzen nicht die gleichen sind*/
        {
            if(*testFlaeche == *tempFlaeche)return true;
        }
    }
    return false;
}

void RUZ_Layer::GeaenderteFlaecheHinzufuegen(Flaeche* obj)
{
    m_aktualisierungsFlaechen->ExklusivHinzufuegen(obj);
    return;
}

void RUZ_Layer::FlaechenAktualisieren(bool hlAktualisieren)
{
    if(m_aktualisierungsFlaechen->GetListenGroesse() > 0)
    {
        for(GefaelleMarke* tempGM = m_gefaelleMarkenLst->GetErstesElement(); tempGM != NULL; tempGM = m_gefaelleMarkenLst->GetNaechstesElement())
        {
            tempGM->LagePruefen();
        }
    }
    for(Flaeche* tempFlaeche = m_aktualisierungsFlaechen->GetErstesElement(); tempFlaeche != NULL; tempFlaeche = m_aktualisierungsFlaechen->GetNaechstesElement())
    {
        tempFlaeche->NormaleBerechnen();
        tempFlaeche->HoehenMarkenAktualisieren();
        if(hlAktualisieren)
        {
            tempFlaeche->HoehenlinienFinden();
        }
        m_aktualisierungsFlaechen->Entfernen(tempFlaeche);
    }
    for(HoehenMarke* tempHM = m_hoehenMarkenLst->GetErstesElement(); tempHM != NULL; tempHM = m_hoehenMarkenLst->GetNaechstesElement())
    {
        if(!(tempHM->IstInFlaeche()))
        {
            tempHM->Verschieben(Vektor(0, 0, 0));
        }
    }
    return;
}

void RUZ_Layer::SetzeHLParameter(double hoehenSchritt, double suchRadius, double starthoehe, Achse projektion)
{
    m_hoehenSchritt = hoehenSchritt;
    m_suchRadius = suchRadius;
    m_startHoehe = starthoehe;
    m_projektionsRichtung = projektion;
    return;
}

void RUZ_Layer::SetzeProjektion(Achse projektion)
{
    m_projektionsRichtung = (Achse)(projektion % 3);
    return;
}

void RUZ_Layer::LoescheFangpunkte(void)
{
    m_fngPktLst->ListeLoeschen("RUZ_Layer::LoescheFangpunkte");
    return;
}

bool RUZ_Layer::AusdehnungFinden(double &min_x, double &min_y, double &max_x, double &max_y, double &min_z, double &max_z)
{
    bool bRueckgabe = false;
    max_x = max_y = min_x = min_y = min_z = max_z = 0.0;
    Punkt *tempPkt;
    HoehenMarke *tempHM;
    Kreis *tempKr;
    double tempRadius;
    Fangpunkt *tempFP;

    Achse aktProjX = (Achse)((m_projektionsRichtung + 1) % 3);
    Achse aktProjY = (Achse)((m_projektionsRichtung + 2) % 3);
    Achse aktProjZ = (Achse)((m_projektionsRichtung) % 3);

    tempPkt = HolePunkte()->GetErstesElement();
    tempHM = HoleHoehenMarken()->GetErstesElement();
    tempKr = HoleKreise()->GetErstesElement();
    tempFP = HoleFangpunkte()->GetErstesElement();
    if(tempPkt != NULL)
    {
        max_x = min_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
        max_y = min_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
        max_z = min_z = tempPkt->HolePosition().GetKoordinaten(aktProjZ);
        bRueckgabe = true;
    }
    if(tempHM != NULL)
    {
        max_x = min_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
        max_y = min_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
        max_z = min_z = tempHM->HolePosition().GetKoordinaten(aktProjZ);
        bRueckgabe = true;
    }
    if(tempKr != NULL)
    {
        tempRadius = tempKr->HoleRadius();
        max_x = tempKr->HolePosition().GetKoordinaten(aktProjX) + tempRadius;
        min_x = tempKr->HolePosition().GetKoordinaten(aktProjX) - tempRadius;
        max_y = tempKr->HolePosition().GetKoordinaten(aktProjY) + tempRadius;
        min_y = tempKr->HolePosition().GetKoordinaten(aktProjY) - tempRadius;
        max_z = tempKr->HolePosition().GetKoordinaten(aktProjZ) + tempRadius;
        min_z = tempKr->HolePosition().GetKoordinaten(aktProjZ) - tempRadius;
        bRueckgabe = true;
    }
    if(tempFP != NULL)
    {
        max_x = min_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
        max_y = min_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
        max_z = min_z = tempFP->HolePosition().GetKoordinaten(aktProjZ);
        bRueckgabe = true;
    }

    for(tempPkt = HolePunkte()->GetErstesElement(); tempPkt != NULL; tempPkt = HolePunkte()->GetNaechstesElement())
    {
        if(tempPkt->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
        if(tempPkt->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempPkt->HolePosition().GetKoordinaten(aktProjX);
        if(tempPkt->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
        if(tempPkt->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempPkt->HolePosition().GetKoordinaten(aktProjY);
        if(tempPkt->HolePosition().GetKoordinaten(aktProjZ) < min_z)min_z = tempPkt->HolePosition().GetKoordinaten(aktProjZ);
        if(tempPkt->HolePosition().GetKoordinaten(aktProjZ) > max_z)max_z = tempPkt->HolePosition().GetKoordinaten(aktProjZ);
    }
    for(tempHM = HoleHoehenMarken()->GetErstesElement(); tempHM != NULL; tempHM = HoleHoehenMarken()->GetNaechstesElement())
    {
        if(tempHM->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
        if(tempHM->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempHM->HolePosition().GetKoordinaten(aktProjX);
        if(tempHM->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
        if(tempHM->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempHM->HolePosition().GetKoordinaten(aktProjY);
        if(tempHM->HolePosition().GetKoordinaten(aktProjZ) < min_z)min_z = tempHM->HolePosition().GetKoordinaten(aktProjZ);
        if(tempHM->HolePosition().GetKoordinaten(aktProjZ) > max_z)max_z = tempHM->HolePosition().GetKoordinaten(aktProjZ);
    }
    for(tempKr = HoleKreise()->GetErstesElement(); tempKr != NULL; tempKr = HoleKreise()->GetNaechstesElement())
    {
        tempRadius = tempKr->HoleRadius();
        if((tempKr->HolePosition().GetKoordinaten(aktProjX) - tempRadius) < min_x)min_x = tempKr->HolePosition().GetKoordinaten(aktProjX) - tempRadius;
        if((tempKr->HolePosition().GetKoordinaten(aktProjX) + tempRadius) > max_x)max_x = tempKr->HolePosition().GetKoordinaten(aktProjX) + tempRadius;
        if((tempKr->HolePosition().GetKoordinaten(aktProjY) - tempRadius) < min_y)min_y = tempKr->HolePosition().GetKoordinaten(aktProjY) - tempRadius;
        if((tempKr->HolePosition().GetKoordinaten(aktProjY) + tempRadius) > max_y)max_y = tempKr->HolePosition().GetKoordinaten(aktProjY) + tempRadius;
        if((tempKr->HolePosition().GetKoordinaten(aktProjZ) - tempRadius) < min_z)min_z = tempKr->HolePosition().GetKoordinaten(aktProjZ) - tempRadius;
        if((tempKr->HolePosition().GetKoordinaten(aktProjZ) + tempRadius) > max_z)max_z = tempKr->HolePosition().GetKoordinaten(aktProjZ) + tempRadius;
    }
    for(tempFP = HoleFangpunkte()->GetErstesElement(); tempFP != NULL; tempFP = HoleFangpunkte()->GetNaechstesElement())
    {
        if(tempFP->HolePosition().GetKoordinaten(aktProjX) < min_x)min_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
        if(tempFP->HolePosition().GetKoordinaten(aktProjX) > max_x)max_x = tempFP->HolePosition().GetKoordinaten(aktProjX);
        if(tempFP->HolePosition().GetKoordinaten(aktProjY) < min_y)min_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
        if(tempFP->HolePosition().GetKoordinaten(aktProjY) > max_y)max_y = tempFP->HolePosition().GetKoordinaten(aktProjY);
        if(tempFP->HolePosition().GetKoordinaten(aktProjZ) < min_z)min_z = tempFP->HolePosition().GetKoordinaten(aktProjZ);
        if(tempFP->HolePosition().GetKoordinaten(aktProjZ) > max_z)max_z = tempFP->HolePosition().GetKoordinaten(aktProjZ);
    }
    return bRueckgabe;
}

void RUZ_Layer::GefaelleRasterAnlegen(double rasGr, double minimum, double optimum, double maximum)
{
    if(rasGr == 0)return;

    gefaelleRasterAktiv = true;
    dGefMinimum = minimum;
    dGefOptimum = optimum;
    dGefMaximum = maximum;

    double min_x, min_y, max_x, max_y, min_z, max_z;
    if(AusdehnungFinden(min_x, min_y, max_x, max_y, min_z, max_z))
    {
        m_gefaelleMarkenLst->ListeLoeschen("");
        /*Runden*/
        min_x = ceil(min_x/rasGr) * rasGr;
        min_y = ceil(min_y/rasGr) * rasGr;
        max_x = ceil(max_x/rasGr) * rasGr;
        max_y = ceil(max_y/rasGr) * rasGr;

        double dLaeufer_x = min_x;
        Flaeche* flBisherigeFlaeche = NULL;
        do
        {
            double dLaeufer_y = min_y;
            do
            {
                GefaelleMarke* gmNeu = new GefaelleMarke(dGefMinimum, dGefOptimum, dGefMaximum, dLaeufer_x, dLaeufer_y, this, flBisherigeFlaeche);
                if(gmNeu)flBisherigeFlaeche = gmNeu->HoleFlaeche();
                dLaeufer_y += rasGr;
            }while(dLaeufer_y < max_y);
            dLaeufer_x += rasGr;
        }while(dLaeufer_x < max_x);
    }
    return;
}

void RUZ_Layer::GefaelleRasterAnpassen(void)
{
    return;
}

void RUZ_Layer::GefaelleRasterLoeschen(void)
{
    logSchreiben("GefaelleRasterLoeschen...\n");
    m_gefaelleMarkenLst->ListeLoeschen("GefaelleRasterLoeschen");
    logSchreiben("GefaelleRasterLoeschen...erledigt\n");
    gefaelleRasterAktiv = false;
    return;
}

void RUZ_Layer::logSchreiben(const char* msg, ...)
{
	#ifndef __OHNE_WIN_
    FILE *Logbuch;
    const char *pfad = "log/Debug.log";
    Logbuch = fopen(pfad, "a");
    va_list args;
    va_start (args, msg);
    vfprintf (Logbuch, msg, args);
    va_end (args);
    fclose(Logbuch);
	#endif
    return;
}

void RUZ_Layer::ElementlisteAusgeben(void)
{
    logSchreiben("\nLayer: ");
    logSchreiben(HoleName());
    logSchreiben("\nPunktListe\n");
    for(Punkt *laeufer = m_punktLst->GetErstesElement(); laeufer != NULL; laeufer =  m_punktLst->GetNaechstesElement())
    {
        logSchreiben("%p: %0.5f / %0.5f / %0.5f\n", laeufer, laeufer->HolePosition().x(), laeufer->HolePosition().y(), laeufer->HolePosition().z(), 5);
    }
    logSchreiben("\nLinienListe\n");
    for(Linie *laeufer = m_linienLst->GetErstesElement(); laeufer != NULL; laeufer =  m_linienLst->GetNaechstesElement())
    {
        logSchreiben("%p: %p / %p", laeufer, laeufer->HolePunkt(0), laeufer->HolePunkt(1));
    }

    logSchreiben("\nFlächenListe\n");
    for(Flaeche *laeufer = m_flaechenLst->GetErstesElement(); laeufer != NULL; laeufer =  m_flaechenLst->GetNaechstesElement())
    {
        logSchreiben("%p: ", laeufer);
        if(laeufer->HoleTyp() == RUZ_Dreieck)
        {
            logSchreiben("(Dreieck)\nP: %p / %p / %p\nL: %p / %p / %p\n",
                         laeufer->HolePunkt(0), laeufer->HolePunkt(1), laeufer->HolePunkt(2),
                         laeufer->HoleLinie(0), laeufer->HoleLinie(1), laeufer->HoleLinie(2));
        }else{

            logSchreiben("(Viereck)\nP: %p / %p / %p / %p\nL: %p / %p / %p / %p\n",
                         laeufer->HolePunkt(0), laeufer->HolePunkt(1), laeufer->HolePunkt(2), laeufer->HolePunkt(3),
                         laeufer->HoleLinie(0), laeufer->HoleLinie(1), laeufer->HoleLinie(2), laeufer->HoleLinie(3));
        }
        logSchreiben("\n");
    }
    return;
}

void RUZ_Layer::UngeschuetzteLinienLoeschen(void)
{
	for(Linie *laeufer = m_linienLst->GetErstesElement(); laeufer != NULL; laeufer = m_linienLst->GetNaechstesElement())
    {
        if(!laeufer->IstGeschuetzt())
		{
			delete laeufer;
		}
    }
	return;
}
