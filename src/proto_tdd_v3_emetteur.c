/*************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO];
    int borneInf,curseur,tailleF,temp,timer,taille_message,temp2,compteur;
    paquet_t tab[16];
    paquet_t pack;
    //init
    curseur=0;borneInf=0;tailleF=15;temp=0;compteur=50;
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
        if (borneInf==curseur) depart_temporisateur(temp,100);
        curseur=(curseur+1)%16;

        printf("curseur = %d\n",curseur);
        de_application(message,&taille_message);
        compteur=50;
      }else{
        timer=attendre();
        if (timer == -1)
        {
          de_reseau(&pack);
          printf("ACK recu = %d\n",pack.num_seq);
          if (verifierControl(&pack) && dans_fenetre(borneInf,pack.num_seq,tailleF))
          {
            borneInf=(pack.num_seq+1)%16;
            printf("nouvelle borne inf = %d\n",borneInf);
            arreter_temporisateur(temp);
            if (borneInf!=curseur) depart_temporisateur(temp,100);
          }
        }else{
          temp2=borneInf;
          depart_temporisateur(temp,100);
          while(temp2!=curseur)
          {
            vers_reseau(&tab[temp2]);
            temp2=(temp2+1)%16;
          }
          compteur--;
          printf("compteur: %d\n",compteur);
          if (compteur==0) return 1;
        }
      }
    }
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
