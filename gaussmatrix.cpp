// gaussmatrix implementierung

#include "gaussmatrix.h"

cGaussMatrix::cGaussMatrix(int n) {
    m_nord = n;
    m_pkx = new double[n];
    m_pkoeff = new double[n];
    m_pmatrix = new double[n*n];
}


cGaussMatrix::~cGaussMatrix() {
    delete[] m_pkx;
    delete[] m_pkoeff;
    delete[] m_pmatrix;
}    


void cGaussMatrix::setMatrix(int z, cfpNode node) {
    m_pkx[z] = node.p;
    int i, aoffs;
    double fr = node.f; // startwert
    double ko = 1.0;
    aoffs = (z+1) * m_nord -1;
    for (i = 0; i < (m_nord-1); i++) { // yz = az*f^3 +bz*f^2 +cz*f^1 +dz*f^0
	ko *= fr; // potenzieren     
	m_pmatrix[aoffs -1 - i] = ko;
    }
    m_pmatrix[aoffs] = 1.0; // eine zeile in der matrix initialisiert
}


void cGaussMatrix::cmpKoeff() { // auflösen der matrix durch elimination und koeffizienten bestimmen
    int i,z,s; // zeile, spalte
    double fak;
    for (i = 1; i < m_nord; i++) {
	for (z = i; z < m_nord; z++)
	{
	    if (m_pmatrix[z*m_nord + i - 1] != 0.0)
		fak = -m_pmatrix[(i-1)*m_nord + i - 1] / m_pmatrix[z * m_nord + i - 1];
	    else
		fak = 0.0;
	    m_pkx[z] = fak * m_pkx[z] + m_pkx[i-1];
	    for (s = i-1; s < m_nord; s++) 
		m_pmatrix[z*m_nord + s] = fak * m_pmatrix[z*m_nord  + s] + m_pmatrix[(i-1)*m_nord +s]; 
	}  
    } // gauss'sche eliminiation erledigt
    
     for (i = 0; i < m_nord; i++) { // alle koeffizienten bestimmen
	z = m_nord -1 - i; // wir starten in der letzten zeile 
	m_pkoeff[z] = m_pkx[z];
	if (i > 0) { // wir hangeln uns hoch
	    for (s = z+1; s < m_nord; s++) 
		m_pkoeff[z] -= m_pmatrix[z*m_nord + s] * m_pkoeff[s];
	}
	if (m_pmatrix[z*m_nord + z] != 0.0) 
	    m_pkoeff[z] /= m_pmatrix[z*m_nord + z];
	else
	   m_pkoeff[z] = 0.0;
    }
}
    
     
double cGaussMatrix::getKoeff(int n) {
    return m_pkoeff[n];
}
