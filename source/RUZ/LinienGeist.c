#include "LinienGeist.h"

int SchneidenSich (LinienGeist lg0, LinienGeist lg1)
{
	if (lg0.p0 == lg1.p0 || lg0.p0 == lg1.p1 || lg0.p1 == lg1.p0 || lg0.p1 == lg1.p1)
		return 0;
	double kreuz0, kreuz1;
	kreuz0 = (lg0.x1 - lg0.x0) * (lg1.y0 - lg0.y0) - (lg0.y1 - lg0.y0) * (lg1.x0 - lg0.x0);
	kreuz1 = (lg0.x1 - lg0.x0) * (lg1.y1 - lg0.y0) - (lg0.y1 - lg0.y0) * (lg1.x1 - lg0.x0);
	if (!(kreuz0 * kreuz1 < 0))
		return 0;
	kreuz0 = (lg1.x1 - lg1.x0) * (lg0.y0 - lg1.y0) - (lg1.y1 - lg1.y0) * (lg0.x0 - lg1.x0);
	kreuz1 = (lg1.x1 - lg1.x0) * (lg0.y1 - lg1.y0) - (lg1.y1 - lg1.y0) * (lg0.x1 - lg1.x0);
	if (!(kreuz0 * kreuz1 < 0))
		return 0;
	return 1;
}