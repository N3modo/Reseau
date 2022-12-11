/*************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO];
    bool tab_bool[16];
    int taille_message,timer,borneInf,curseur,tailleF;
    paquet_t tab[16],pack;

    //init
    for(int i=0;i<16;i++)tab_bool[i]=0;
    curseur=0;borneInf=0;tailleF=8;
    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_message);
    /* tant que l'émetteur a des données à envoyer */
    while(taille_message!=0 || borneInf != curseur)
    {
      if (dans_fenetre(borneInf,curseur,tailleF) && taille_message > 0)
      {
          for (int i=0; i<taille_message; i++)
          {
              tab[curseur].info[i] = message[i];
          }

        tab[curseur].num_seq=curseur;
        tab[curseur].lg_info=taille_message;
        tab[curseur].type=DATA;
        tab[curseur].somme_ctrl=genererControl(&tab[curseur]);

        vers_reseau(&tab[curseur]);
        printf("Dernier paquet envoyé : %d\n",curseur);
        depart_temporisateur(curseur,100);
        curseur=(curseur+1)%16;
        printf("curseur = %d\n",curseur);
        de_application(message,&taille_message);
      }else{
        timer=attendre();
        if (timer == -1)
        {
          de_reseau(&pack);
          printf("ACK recu = %d\n",pack.num_seq);
          if (verifierControl(&pack) && dans_fenetre(borneInf,pack.num_seq,tailleF))
          {
            arreter_temporisateur(pack.num_seq);
            tab_bool[pack.num_seq]=1;
            while(tab_bool[borneInf])
            {
              tab_bool[borneInf]=0;
              borneInf=(borneInf+1)%16;
            }
          }
        }else{
          depart_temporisateur(timer,100);
          vers_reseau(&tab[timer]);
        }
      }
    }
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
