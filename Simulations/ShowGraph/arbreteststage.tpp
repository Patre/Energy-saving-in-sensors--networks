/**
 * \brief Constructeur de l'arbre.
 * \param une chaine de cararctaire(le pere suppreme de l'arbre).
 **/
template<typename T>
ArbreStage<T>::ArbreStage()
{
    this->values = Nullptr(arbre<T>);
}

/**
 * \brief Constructeur de l'arbre.
 * \param une chaine de cararctaire(le pere suppreme de l'arbre).
 **/

template<typename T>
ArbreStage<T>::ArbreStage(T pere)
{
    this->values = Nullptr(arbre<T>);
    ajouterElement(&this->values,pere);
}



/**
 * \brief permet de detruire l'arbre.
 **/

template<typename T>
ArbreStage<T>::~ArbreStage()
{
    this->freeArbre(&this->values);
}

/**
 * \brief Ajouter un element fils dans le sous arbre pere.
 * \param pere c'est la chaine qui represente le  sous arbre qui va contenir le fils.
 * \param fils c'est l'element qui va etre ajouter.
 * \return le nombre d'occurance ajouter ou 0 si le pere n'existe pas.
 **/

template<typename T>
int ArbreStage<T>::ajouterFils(T pere, T fils)
{
    return ajouterFils(this->values,pere,fils);
}



/**
 * \brief Ajouter un element fils dans le sous arbre pere.
 * \param encours c'est le sous arbre qui va contenir le val.
 * \param val c'est l'element qui va etre ajouter.
 **/
template<typename T>
void ArbreStage<T>::ajouterElement(arbre<T> **encours,T val)
{
    *encours = new arbre<T>;
    (*encours)->element=val;
    (*encours)->sizeFils=0;
    int i=FILS_MAX-1;
    while(i--)  (*encours)->fils[i]=Nullptr(arbre<T>);
}

/**
 * \brief Ajouter un element fils dans le sous arbre pere.
 * \param encours c'est l'arbre Supreme.
 * \param pere c'est la chaine qui represente le  sous arbre qui va contenir le fils.
 * \param fils c'est l'element qui va etre ajouter.
 * \return le nombre d'occurance ajouter ou 0 si le pere n'existe pas.
 **/

template<typename T>
int ArbreStage<T>::ajouterFils(arbre<T> *encours,T pere,T fils)
{
    if(!this->estVide() && encours->element==pere)
    {
        ajouterElement(&encours->fils[encours->sizeFils],fils);
        encours->sizeFils++;
        return 1;
    }

    else if(!this->estVide())
    {
        int ret=0;
        int i=encours->sizeFils;
        while(i--)
            ret+=ajouterFils(encours->fils[i],pere,fils);
        return ret;
    }

    else
    {
        std::cout << "il faut ajouter le pere supreme pour l'ajouter "<<std::endl;
        return 0;
    }
}

/**
 * \brief Affichage de l'arbre.
 **/
template<typename T>
void ArbreStage<T>::afficher()
{
    this->afficher(this->values);
}

/**
 * \brief affichage Recuresive .
 * \param encours l'arbre a afficher.
 **/
template<typename T>
void ArbreStage<T>::afficher(arbre<T> *encours)
{
    if(!this->estVide())
    {
        std::cout<<"element "<< encours->element<<" : {";
        int i=encours->sizeFils;
        while(i--)      std::cout<<encours->fils[i]->element<<",";
        std::cout<<"}\n";

        i=encours->sizeFils;
        while(i--)      afficher(encours->fils[i]);
    }
    else
        std::cout << "l'arbre est Vide";
}


/**
 * \brief permet de librere l'espace memoire de l'arbre
 * \param l'arbre a librer
 **/
template<typename T>
void ArbreStage<T>::freeArbre(arbre<T> **encours)
{
    if(!this->estVide())
    {
        if((*encours)->sizeFils==0)
        {
            delete(*encours);
            *encours = Nullptr(arbre<T>);
            return;
        }
        else
        {
            int i=0;
            i=(*encours)->sizeFils;
            while(i--)      freeArbre(&(*encours)->fils[i]);

            delete(*encours);
            *encours=Nullptr(arbre<T>);
        }
    }
}

/**
 * \brief Verfier si l'arbre est vide ou pas
 * \return  true si l'arbre est vide, false sinon
 **/
template<typename T>
bool ArbreStage<T>::estVide()
{
    if(this->values == Nullptr(arbre<T>))  return true;
    else return false;
}

/**
 * \brief peremet d'ajouter le pere supreme de l'arbre
 * \param pere est la chaine a ajouter
 * \return  true si l'arbre est vide, false sinon
 **/
template<typename T>
bool ArbreStage<T>::ajouterPereSupereme(T pere)
{
    if(this->estVide())
    {
        ajouterElement(&this->values,pere);
        return true;
    }
    return false;
}

