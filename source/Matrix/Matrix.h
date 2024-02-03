#ifndef _Matrizenrechnung_
#define _Matrizenrechnung_

#include <cstdlib>
#include "../Vektor/Vektor.h"

class Matrix
{
	private:
	   int tDim_x;
	   double *tElement, *tInvElement;
       int tDim_y;
       void InhaltDrehen(void);
	public:
	   Matrix(int,int);
	   Matrix(const Vektor, const Vektor, const Vektor);/**3-dimensionale Matrix; Vektoren werden in die Spalten eingefuegt*/
	   Matrix(const Matrix&);
	   ~Matrix(void);
	   Matrix& operator=(const Matrix&);
	   bool MInverse(void);
	   double wert(int, int)const;
	   void wert(int, int, double);
	   double inv_wert(int, int)const;
	   void inv_wert(int, int, double);
	   void printScreen(void)const;
	   void EinheitsMatrix(void);
	   Matrix operator*(const Matrix&);
	   Vektor operator*(const Vektor&);
	   int Dimension(int)const;
};

#endif
