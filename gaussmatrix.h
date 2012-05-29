// headerfile für gausmatrix klasse
// dient zur numerischen lösung von gleichungssystemen mit
// variabler anzahl von unbekannten

#ifndef GMATRIX_H
#define GMATRIX_H

struct cfpNode { // eine stützstelle
    double f; // frequenz
    double p; // phase
};
    

class cGaussMatrix {
public:
    cGaussMatrix(int); // legt eine Matrix n'ter ordnung an
    ~cGaussMatrix(); 
    void setMatrix(int, cfpNode);
    void cmpKoeff();
    double getKoeff(int); 
private:
    int m_nord;
    double *m_pkx; // n werte
    double *m_pkoeff; // n koeffizienten
    double *m_pmatrix; // n*n werte (quadratische matrix)
};

#endif

