#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

typedef struct data{
float x;
float y;
float theta;
}Data;

void messageUDP(char *hote,char *service,Data message,int taille){

	struct addrinfo precisions,*resultat;
	int statut;
	int s;

	/* Creation de l'adresse de socket */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	statut=getaddrinfo(hote,service,&precisions,&resultat);
	if(statut<0){ perror("messageUDPgenerique.getaddrinfo"); exit(EXIT_FAILURE); }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("messageUDPgenerique.socket"); exit(EXIT_FAILURE); }

	/* Option sur la socket */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&vrai,sizeof(vrai))<0){
		perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
		exit(-1);
	}

	/* Envoi du message */
	int nboctets=sendto(s,&message,taille,0,resultat->ai_addr,resultat->ai_addrlen);
	if(nboctets<0){ perror("messageUDPgenerique.sento"); exit(EXIT_FAILURE); }

	/* Liberation de la structure d'informations */
	freeaddrinfo(resultat);

	/* Fermeture de la socket d'envoi */
	close(s);
}

int main(int argc,char *argv[]){

	if(argc!=2){
		fprintf(stderr,"Syntaxe : %s <serveur>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	char *hote=argv[1];
	char *service="5000";
	Data message;
	message.x=10.1010;
	message.y=12.1010;
	message.theta=24.1010;

	messageUDP(hote,service,(Data)message,sizeof(message));
	return 0;

}

