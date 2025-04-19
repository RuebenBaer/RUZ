#include "LinienGeist.h"
#include <math.h>
#include <stdio.h>

int SchneidenSich (LinienGeist *lg0, LinienGeist *lg1)
{
	if (lg0->p0 == lg1->p0 || lg0->p0 == lg1->p1 || lg0->p1 == lg1->p0 || lg0->p1 == lg1->p1)
		return 0;
	double kreuz0, kreuz1;
	kreuz0 = (lg0->x1 - lg0->x0) * (lg1->y0 - lg0->y0) - (lg0->y1 - lg0->y0) * (lg1->x0 - lg0->x0);
	kreuz1 = (lg0->x1 - lg0->x0) * (lg1->y1 - lg0->y0) - (lg0->y1 - lg0->y0) * (lg1->x1 - lg0->x0);
	if (!(kreuz0 * kreuz1 < 0))
		return 0;
	kreuz0 = (lg1->x1 - lg1->x0) * (lg0->y0 - lg1->y0) - (lg1->y1 - lg1->y0) * (lg0->x0 - lg1->x0);
	kreuz1 = (lg1->x1 - lg1->x0) * (lg0->y1 - lg1->y0) - (lg1->y1 - lg1->y0) * (lg0->x1 - lg1->x0);
	if (!(kreuz0 * kreuz1 < 0))
		return 0;
	return 1;
}

void LG_setzeWerte(LinienGeist *lg, void *p0, void *p1, double x0, double y0, double x1, double y1)
{
	lg->p0 = p0;
	lg->p1 = p1;
	lg->x0 = x0;
	lg->y0 = y0;
	lg->x1 = x1;
	lg->y1 = y1;
	lg->laenge = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
	return;
}