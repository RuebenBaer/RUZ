#ifndef __LINIENGEIST_H
#define __LINIENGEIST_H

typedef struct lg {
	void *p0, *p1;
	double laenge;
	int geloescht;
	double x0, x1, y0, y1;
} LinienGeist;

extern int SchneidenSich (LinienGeist lg0, LinienGeist lg1);

#endif //__LINIENGEIST_H