#include "..\source\RUZ\RUZObjekte.h"

int main(int argc, char **argv)
{
	RUZ_Layer lay("Test");
	Punkt *p1, *p2, *p3;
	p1 = new Punkt (15.7, 18.3, 3.21, &lay);
	p2 = new Punkt (16.9, 35.8, -1.05, &lay);
	p3 = new Punkt (25.4, 33.7, 0.0, &lay);
	Linie* ln1 = Linie::NeueLinie(p1, p2);
	Linie* ln2 = Linie::NeueLinie(p3, p2);
	
	LinienFlaeche lnFl[2];
	
	lnFl[0].ln = ln1;
	lnFl[1].ln = ln2;
	
	Vektor vkt(20.0, 20.0, 0.0);
	
	LinienExtrudieren(lnFl, 2, 1.0, 1.0, z, vkt);
	
	printf("Anzahl Flaechen: %d\n", lay.HoleFlaechen()->GetListenGroesse());
	
	for (int i = 0; i < 2; i++) {
		printf("Normale %d: %g | %g | %g\n", i, lnFl[i].n.x(), lnFl[i].n.y(), lnFl[i].n.z());
	}
	
	delete ln1;
	delete ln2;
	delete p1;
	delete p2;
	delete p3;
	return 0;
}