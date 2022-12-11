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
    unsigned char message[MAX_INFO];
    paquet_t paquet;
    int fin = 0;
    int paquet_attendu=0;
    //fintest
    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");
    /* tant que le récepteur reçoit des données */
    while (!fin) {
        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquet);

        if (verifierControl(&paquet))
        {
          if(paquet.num_seq==paquet_attendu)
          {
            for (int i=0; i<paquet.lg_info; i++) {
                message[i] = paquet.info[i];
            }
            paquet_attendu++;
            if(paquet_attendu==256)
            {
              paquet_attendu=0;
            }
            fin = vers_application(message, paquet.lg_info);
            paquet.type=ACK;
          }
          vers_reseau(&paquet);
        }
      }
    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
