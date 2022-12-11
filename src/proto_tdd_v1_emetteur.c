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
    int taille_buffer;
    unsigned char buffer[MAX_INFO];
    paquet_t pdata,pack;
    //fintest

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(buffer, &taille_buffer);
    /* tant que l'émetteur a des données à envoyer */
    while(taille_buffer!=0)
    {
      for (int i=0; i<taille_buffer; i++)
      {
          pdata.info[i] = buffer[i];
      }
      pdata.num_seq=0;
      pdata.lg_info = taille_buffer;
      pdata.type = DATA;
      pdata.somme_ctrl=genererControl(&pdata);
      pack.type=NACK;
      while (pack.type==NACK)
      {
          /* construction paquet */
          /* remise à la couche reseau */
          vers_reseau(&pdata);
          de_reseau(&pack);
          /* lecture des donnees suivantes de la couche application */
      }
      de_application(buffer, &taille_buffer);
    }
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
