#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 2000
#define LEN 4096

struct sockaddr_in remoto;

//0 o Mira
//1 @ Água
//2 # Barco

//0 Água
//1 Barco

void buildBoard(int x[10][10]){

	char abscissas[10] = "ABCDEFGHIJ";
	char ordenadas[10] = "1234567890";
	printf("  ");
	for(int l = 0; l < 10; l++){
    	printf(" %c ", ordenadas[l]);
	}
	printf("\n");
	for(int i = 0; i < 10; i++){
		printf("%c ", abscissas[i]);
		for(int j = 0; j < 10; j++){
			if(x[i][j] == 0){
				printf(" o ");
			}
			if(x[i][j] == 1){
				printf(" ~ ");
			}
			if(x[i][j] == 2){
				printf(" # ");
			}
		}
		printf("\n");
	}
}

void cleanBoard(int x[10][10]){
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			x[i][j] = 0;
		}
	}
}


int findAbscissa(char abscissa){
	char abscissas[10] = "ABCDEFGHIJ";
	for(int i = 0; i < 10; i++){
		if(abscissas[i] == abscissa){
			return i;
			}
	}
}

void build(int value, int campo[10][10], char coordenadas[2]){
	campo[findAbscissa(coordenadas[0])][coordenadas[1]-'0'-1] = value;
}

void buildOthersShips(int shipType, int xY, int campo[10][10], char coordenadas[2]){
	int x = findAbscissa(coordenadas[0]);
	int y = 0;
	if(coordenadas[1]-'0'-1 == -1){
		y = 9;
	} else {
		y = coordenadas[1]-'0'-1;
	}

	if(xY == 1){
		if(x + shipType > 9){
			for(int i = 0; i < shipType;i++){
				campo[x-i][y] = 2;
			}
		} else {
			for(int i = 0; i < shipType;i++){
				campo[x+i][y] = 2;
			}
		}
	} else {
		if(y + shipType < 9){
			for(int i = 0; i < shipType;i++){
				campo[x][y-i] = 2;
			}
		} else {
			for(int i = 0; i < shipType;i++){
				campo[x][y+i] = 2;
			}
		}
	}
}

int bomb(int campo[10][10], char coordenadas[2]){

	int x = findAbscissa(coordenadas[0]);
	int y = 0;

	if(coordenadas[1]-'0'-1 == -1){
		y = 9;
	} else {
		y = coordenadas[1]-'0'-1;
	}

	if(campo[x][y] == 2){
		return 1;
	} else {
		return 0;
	}
}

void drawBattleShip(){

	printf("=======================================================\n");
	printf("=======================================================\n");
	printf("______         _    _    _             _      _        \n");
	printf("| ___ \\       | |  | |  | |           | |    (_)       \n");
	printf("| |_/ /  __ _ | |_ | |_ | |  ___  ___ | |__   _  _ __  \n");
	printf("| ___ \\ / _` || __|| __|| | / _ \\/ __|| '_ \\ | || '_ \n");
	printf("| |_/ /| (_| || |_ | |_ | ||  __/\\__ \\| | | || || |_) |\n");
	printf("\\____/  \\__,_| \\__| \\__||_| \\___||___/|_| |_||_|| .__/\n");
	printf("                                                | |    \n");
	printf("                                                |_|    \n");
	printf("=======================================================\n");
	printf("======================================^================\n");
	printf("====================================='  '   ===========\n");
	printf("=====================================          ========\n");
	printf("======================|\\============          =========\n");
	printf("======================| \\============         =========\n");
	printf("======================|  \\=============================\n");
	printf("======================|   \\============================\n");
	printf("===================== |____\\===========================\n");
	printf("==============________|_________=======================\n");
	printf("~ ~ ~ ~ ~ ~ ~\\                 /~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
	printf(" ~ ~ ~ ~ ~ ~ ~\\               /~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ \n");
	printf("~ ~ ~ ~ ~ ~ ~ ~\\_____________/~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
	printf("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ \n");
	printf("\n");

}

int main(){

	char ip[50];
	printf("Digite o IP do Servidor: ");
    scanf("%s",ip);

drawBattleShip();

//==========================CONFIGURACOES===========================//
//==========================CONFIGURACOES===========================//

	int myBoard[10][10];
	int currentBoard[10][10];
	int enemyBoard[10][10];
	char value[2];
	int ship = 2;

	char coordenadas[2];
	int gameOver = 0;
	int xY;
	int shipType = 1;
	int position;

    int server;
    int slen = sizeof(remoto);
    int i = 0;

    int sockfd = socket(AF_INET,SOCK_STREAM, 0);
    char buffer[4096];
    char msg[4096];
    char msgServidor[4096];

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    } else {
        printf("=======================================================\n");
        printf("==================MONTE SEU MAPA=======================\n");
		printf("============CONECTANDO COM O SEU OPONENTE==============\n");
        printf("=====================AGUARDE===========================\n");
    }
   
    remoto.sin_family    = AF_INET;
    remoto.sin_port      = htons(PORTA);
    remoto.sin_addr.s_addr      = inet_addr(ip);
    memset(remoto.sin_zero, 0x0, 8);
    
//==========================CONFIGURACOES===========================//
//==========================CONFIGURACOES===========================//

    connect(sockfd, (struct sockaddr*) &remoto, slen);

//==========================INICIO DO JOGO===========================//
//==========================INICIO DO JOGO===========================//

    char nome[100];
    char nomeOponente[100];
    int primeiraVez = 0;

	cleanBoard(myBoard);
	cleanBoard(currentBoard);
	buildBoard(myBoard);

	//4 Submarinos
	printf("============POSICIONE 4 SUBMARINOS NO MAPA=============\n");
	for(int i = 1; i < 5; i++){
		printf("Escolha a posição do seu %dº submarino: ", i);
		scanf("%s",coordenadas);
		build(ship,myBoard, coordenadas);
	}

	//3 Cruzadores
	shipType = 2;
	printf("============POSICIONE 3 CRUZADORES NO MAPA=============\n");
	for(int i = 1; i < 4; i++){
		printf("Escolha a posição do seu %dº cruzadores: ", i);
		scanf("%s",coordenadas);
		printf("Escolha a posição vertical (1) ou horizontal (2) do seu %dº cruzadores: ", i);
		scanf("%d",&xY);
		buildOthersShips(shipType,xY,myBoard, coordenadas);
	}

	//2 Encouraçados
	shipType = 3;
	printf("============POSICIONE 2 ENCOURAÇADOS NO MAPA=============\n");
	for(int i = 1; i < 3; i++){
		printf("Escolha a posição do seu %dº encouraçados: ", i);
		scanf("%s",coordenadas);
		printf("Escolha a posição vertical (1) ou horizontal (2) do seu %dº encouraçados: ", i);
		scanf("%d",&xY);
		buildOthersShips(shipType,xY,myBoard, coordenadas);
	}

	//1 Porta Avião
	shipType = 4;
	printf("============POSICIONE O PORTA AVIÕES NO MAPA=============\n");
		printf("Escolha a posição do seu %dº porta-aviões: ", i);
		scanf("%s",coordenadas);
		printf("Escolha a posição vertical (1) ou horizontal (2) do seu %dº porta-aviões: ", i);
		scanf("%d",&xY);
		buildOthersShips(shipType,xY,myBoard, coordenadas);
	
	
	printf("==================SUA ESTRATÉGIA=======================\n");
	buildBoard(myBoard);
	printf("=======================================================\n");
    printf("=====================AGUARDE===========================\n");

	while(1){
		
	memset(buffer, 0x0, LEN);

	    if(recv(sockfd, buffer, LEN,0) > 0){
		
            if(primeiraVez == 0){
                strcpy(nomeOponente, buffer);
                printf("DIGITE SEU NOME: ");
                scanf("%s",nome);
                printf("====================%s x %s =======================\n",nome, nomeOponente);
                strcpy(buffer, nome);
				printf("===========MAY THE SHIP BE WITH YOU!============\n");
                printf("AGUARDE A JOGADA DO %s...\n",nomeOponente);
                primeiraVez++;
                send(sockfd,buffer, strlen(buffer), 0);
            } else {
				printf("Valor %s",buffer);
                //printf("Valor %s e %d: ",buffer, strcmp(buffer,"BB"));
				
                if(strcmp(buffer,"BB") == 0){
                    printf("==============BOMBA! VOCÊ ACERTOU!===============\n");
					build(ship, enemyBoard, value);
                    buildBoard(enemyBoard);
					printf("Digite outra coordenada para atacar %s: ", nomeOponente);
					scanf("%s",buffer);
                    
                    send(sockfd,buffer, strlen(buffer), 0);

                } else if (strcmp(buffer,"AA") == 0){
                    strcpy(buffer,"AA");
                    send(sockfd,buffer, strlen(buffer), 0);
                } else{

                printf("%s ATIROU EM %s \n", nomeOponente, buffer);

				int validar = bomb(myBoard, buffer);

				if(validar == 1){
					printf("======================BOMBA!======================\n");
						strcpy(buffer,"BB");
						send(sockfd,buffer, strlen(buffer), 0);
                } else {
					printf("======================ÁGUA!server======================\n");
						strcpy(buffer,"AA");
						send(sockfd,buffer, strlen(buffer), 0);
				}

            if(validar != 1){
                printf("Digite a coordenada para atacar %s: ", nomeOponente);
                scanf("%s",buffer);
				strcpy(value,buffer);
                send(sockfd,buffer, strlen(buffer), 0);
                }            
                }
            }
	    }
		
        strcpy(buffer, "");
        }

	printf("Fim do programa. Socket desligado");    
    return 0;

}


