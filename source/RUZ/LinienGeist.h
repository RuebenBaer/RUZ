#ifndef __LINIENGEIST_H
#define __LINIENGEIST_H

typedef struct lg {
	void *p0, *p1;
	double laenge;
	double x0, x1, y0, y1;
} LinienGeist;

extern void LG_setzeWerte(LinienGeist *lg, void *p0, void *p1, double x0, double y0, double x1, double y1);
extern int SchneidenSich (LinienGeist *lg0, LinienGeist *lg1);

#endif //__LINIENGEIST_H