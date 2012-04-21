#ifndef ARBRETESTSTAGE_H
#define ARBRETESTSTAGE_H
#include <iostream>
#include <string.h>

#define Nullptr(type) (type *)0
#define FILS_MAX  20

//Le Type d'arbre
template <typename T>
struct arbre
{
    T element;
    int sizeFils;
    arbre *fils[FILS_MAX];
};


//la Class
template <typename T>
class ArbreStage
{
public :
    //functions
    ArbreStage(T pereSupreme);
    ArbreStage();
    ~ArbreStage();

    bool estVide();
    bool ajouterPereSupereme(T pere);
    int ajouterFils(T pere,T fils);
    void afficher();

private:
    //a cause des appelles recursives
    void ajouterElement(arbre<T> **encours,T val);
    int ajouterFils(arbre<T> *encours,T pere,T fils);
    void afficher(arbre<T> *encours);
    void freeArbre(arbre<T> **encours);

    //variables
    arbre<T> *values;

};

#include "arbreteststage.tpp"

#endif // ARBRETESTSTAGE_H
