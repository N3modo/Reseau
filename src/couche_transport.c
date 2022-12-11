#include <stdio.h>
#include "couche_transport.h"
#include "services_reseau.h"

/* ************************************** */
/* Fonctions utilitaires couche transport */
/* ************************************** */

/*
 * Ajouter ici vos fonctions utilitaires
 * (generer_controle, verifier_controle...)
 */

/* ======================================================================= */
/* =================== Fenêtre d'anticipation ============================ */
/* ======================================================================= */

/*--------------------------------------*/
/* Fonction d'inclusion dans la fenetre */
/*--------------------------------------*/
int dans_fenetre(unsigned int inf, unsigned int pointeur, int taille) {

    unsigned int sup = (inf+taille-1) % SEQ_NUM_SIZE;

    return
        /* inf <= pointeur <= sup */
        ( inf <= sup && pointeur >= inf && pointeur <= sup ) ||
        /* sup < inf <= pointeur */
        ( sup < inf && pointeur >= inf) ||
        /* pointeur <= sup < inf */
        ( sup < inf && pointeur <= sup);
}

uint8_t genererControl(paquet_t *pdata){
  uint8_t somme_ctrl=0;
  somme_ctrl=somme_ctrl ^ pdata->type;
  somme_ctrl=somme_ctrl ^ pdata->num_seq;
  somme_ctrl=somme_ctrl ^ pdata->lg_info;
  for (int i=0;i<pdata->lg_info;i++){
    somme_ctrl=somme_ctrl ^ pdata->info[i];
  }
  printf("somme_ctrl : %d\n",somme_ctrl);
  return somme_ctrl;
}

bool verifierControl(paquet_t *pdata){
  uint8_t somme_ctrl = genererControl(pdata);
  printf("%d\n",pdata->somme_ctrl);
  return somme_ctrl==pdata->somme_ctrl;
}
