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
    //test
    int taille_message,waiting;
    int paquet_attendu=0;
    unsigned char message[MAX_INFO];
    paquet_t pdata,pack;
    //fintest

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_message);
    /* tant que l'émetteur a des données à envoyer */
    while(taille_message!=0)
    {
      //construction paquet
      for (int i=0; i<taille_message; i++)
      {
          pdata.info[i] = message[i];
      }
      pdata.num_seq=paquet_attendu;
      pdata.lg_info = taille_message;
      pdata.type = DATA;
      pdata.somme_ctrl=genererControl(&pdata);

      //envoie paquet
      vers_reseau(&pdata);
      depart_temporisateur(1,100);
      waiting=attendre();
      int compteur=50;
      while (waiting != -1)
      {
          /* construction paquet */
          /* remise à la couche reseau */
          vers_reseau(&pdata);
          depart_temporisateur(1,100);
          waiting=attendre();
          compteur--;
          if (compteur==0)
          {
            return 1;
          }
          /* lecture des donnees suivantes de la couche application */
      }
      paquet_attendu++;
      if(paquet_attendu==256)
      {
        paquet_attendu=0;
      }
      de_reseau(&pack);
      arreter_temporisateur(1);
      de_application(message, &taille_message);
    }
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
