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

    unsigned char message[MAX_INFO];
    paquet_t paquet;
    paquet_t pack;
    int fin = 0;
    int paquet_attendu=0;

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    pack.type = ACK;
    pack.num_seq = 15;
    pack.lg_info = 0;
    pack.somme_ctrl = genererControl(&pack);

    /* tant que le récepteur reçoit des données */
    while (!fin) {
        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquet);
        printf("paquet recu = %d\n",paquet.num_seq);
        if (verifierControl(&paquet))
        {
          if(paquet.num_seq==paquet_attendu)
          {
            for (int i=0; i<paquet.lg_info; i++) {
                message[i] = paquet.info[i];
            }
            fin = vers_application(message, paquet.lg_info);
            pack.num_seq = paquet.num_seq;
            pack.somme_ctrl = genererControl(&pack);
            paquet_attendu=(paquet_attendu+1)%16;
          }
          vers_reseau(&pack);
          printf("dernier paquet acquitter: %d\n",pack.num_seq);
        }
      }
    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
