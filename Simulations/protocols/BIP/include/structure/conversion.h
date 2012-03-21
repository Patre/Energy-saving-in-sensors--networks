#ifndef DEF_CONVERSION
#define DEF_CONVERSION


void list_to_listC(listC **liste_connected,list *liste,int node);
void listC_to_list(list **liste,listC *liste_connected);

void arbre_to_list(list **liste,arbre *arbreV);
void arbre_to_list_sauf(list **liste,arbre *arbreV,int val);
void arbre_to_list_fils(list **liste,arbre *arbreV);

#endif


