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
    paquet_t paquet,pack,tab[16];
    bool tab_bool[16];
    //init
    for(int i=0;i<16;i++)tab_bool[i]=0;
    int tailleF=8;
    int fin = 0;
    int paquet_attendu=0;
    pack.type = ACK;
    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    pack.num_seq = 0;
    pack.lg_info = 0;
    pack.somme_ctrl = genererControl(&pack);

    /* tant que le récepteur reçoit des données */
    while (!fin) {
        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquet);
        printf("paquet recu = %d\n",paquet.num_seq);
        if (verifierControl(&paquet))
        {
          if(dans_fenetre(paquet_attendu,paquet.num_seq,tailleF))
          {
            tab_bool[paquet.num_seq]=1;
            tab[paquet.num_seq]=paquet;
            while(tab_bool[paquet_attendu])
            {
              tab_bool[paquet_attendu]=0;
              for (int i=0; i<tab[paquet_attendu].lg_info; i++)
              {
                  message[i] = tab[paquet_attendu].info[i];
              }
              fin = vers_application(message, tab[paquet_attendu].lg_info);
              paquet_attendu=(paquet_attendu+1)%16;
            }
          }
          pack.num_seq=paquet.num_seq;
          pack.somme_ctrl = genererControl(&pack);
          vers_reseau(&pack);
          printf("dernier paquet acquitter: %d\n",pack.num_seq);
        }
      }
    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
