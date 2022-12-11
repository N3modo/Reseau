/*************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
    //test
    unsigned char buffer[MAX_INFO];
    paquet_t pdata;
    int fin = 0;
    //fintest
    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");
    /* tant que le récepteur reçoit des données */
    while (!fin) {
        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&pdata);
        if (verifierControl(&pdata)==0)
        {
          printf("***********ERREUR***********\n\n");
          //vers_application(&);
          pdata.type=NACK;
        }else{
          /* extraction des donnees du paquet recu */
          for (int i=0; i<pdata.lg_info; i++) {
              buffer[i] = pdata.info[i];
          }
          /* remise des données à la couche application */
          pdata.type=ACK;
          fin = vers_application(buffer, pdata.lg_info);
        }
        vers_reseau(&pdata);
    }

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
