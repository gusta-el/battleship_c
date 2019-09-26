
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

//0 o Mira
//1 ~ Água
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

int score(int x[10][10]){
	int score = 0;
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			if(x[i][j] == 2){
				score++;
			}
		}
	}
	return score;
}

int findAbscissa(char abscissa){
	char abscissas[10] = "ABCDEFGHIJ";
	for(int i = 0; i < 10; i++){
		if(abscissas[i] == abscissa){
			return i;
			}
	}
}

void build(int type, int campo[10][10], char coordenadas[2]){
	campo[findAbscissa(coordenadas[0])][coordenadas[1]-'0'-1] = type;
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

struct sockaddr_in local;
struct sockaddr_in remoto;

int main ()
{
	char ip[50];
	printf("Digite o IP do Servidor: ");
    scanf("%s",ip);

drawBattleShip();

	int myBoard[10][10];
	int currentBoard[10][10];
	int enemyBoard[10][10];
	char value[2];
	int bomba = 0;
	
	char coordenadas[2];
	
	int gameOver = 0;
	int xY;
	int shipType = 1;
	int water = 0;
	int ship = 2;
	int position;


//==========================CONFIGURACOES===========================//
//==========================CONFIGURACOES===========================//
    int client;
    int slen = sizeof(remoto);
    int i = 0;

    int sockfd = socket(AF_INET,SOCK_STREAM, 0);
    char mensagemServidor[4096];
    char buffer[4096] = "\nMensagem enviada ao servidor!";
    char mensagemCliente[4096];

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    } else {
		printf("\n");
		printf("=======================================================\n");
		printf("============AGUARDE UM JOGADOR SE CONECTAR=============\n");
		printf("...\n");
	}

    local.sin_family    = AF_INET;
    local.sin_port      = htons(PORTA);
    local.sin_addr.s_addr      = inet_addr(ip);
    memset(local.sin_zero, 0x0, 8);
    bind(sockfd, (struct sockaddr*) &local, sizeof(local));
    listen(sockfd, 1);

//==========================CONFIGURACOES===========================//
//==========================CONFIGURACOES===========================//

    client = accept(sockfd, (struct sockaddr*) &remoto, &slen);

//==========================INICIO DO JOGO===========================//
//==========================INICIO DO JOGO===========================//

	printf("================JOGADOR ENCONTRADO=====================\n");
	printf("==================MONTE SEU MAPA=======================\n");

	cleanBoard(myBoard);
	cleanBoard(enemyBoard);
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


    char nome[100];
	char nomeCliente[100];
    printf("Digite seu nome: ");
    scanf("%s",nome);
    int primeiraVez = 0;

    strcpy(buffer, nome);

    if(send(client,buffer, strlen(buffer), 0)){

	while(gameOver != 1){
		memset(buffer, 0x0, LEN);
		if(recv(client, mensagemCliente, LEN,0) > 0){
			if(primeiraVez == 0){
				printf("======%s SE CONECTOU PARA JOGAR CONTRA VOCÊ!====\n",mensagemCliente);
				strcpy(nomeCliente,mensagemCliente);
				printf("===========MAY THE SHIP BE WITH YOU!============\n");
				printf("Digite a coordenada para atacar %s: ", nomeCliente);
				scanf("%s",mensagemServidor);
				primeiraVez++;
				strcpy(nomeCliente,mensagemCliente);

				send(client,mensagemServidor, strlen(mensagemServidor), 0);
			} else {

				strncpy(value, mensagemCliente, 2);

                printf("Valor %s: ", value);
                if(strcmp(value,"BB") == 0){
                    printf("==============BOMBA! VOCÊ ACERTOU!===============\n");
					build(ship, enemyBoard, mensagemServidor);
					buildBoard(enemyBoard);
					printf("Digite outra coordenada para atacar %s: ", nomeCliente);
					strcpy(mensagemServidor,"");
					scanf("%s",mensagemServidor);
					send(client,mensagemServidor, strlen(mensagemServidor), 0);

                } else if(strcmp(value,"AA") == 0){
				
					build(water, enemyBoard, value);
					buildBoard(enemyBoard);
					printf("==============ÁGUA! AGUARDE A VEZ DO JOGADOR!===============\n");
					strcpy(mensagemServidor,"OK");
					send(client,mensagemServidor, strlen(mensagemServidor), 0);
					
				} else if(strcmp(value,"OK") == 0){
				
					printf("Digite a coordenada para atacar %s: ", nomeCliente);
					scanf("%s",mensagemServidor);
					send(client,mensagemServidor, strlen(mensagemServidor), 0);					
					
				} else {

				printf("%s atirou em %s \n", nomeCliente, value);
				
				int validar = bomb(myBoard, value);
				if(validar == 1){
					printf("=======BOMBA! UM DE SEUS NAVIOS FOI ATINGIDO!=====\n");
					build(ship,currentBoard, value);
					buildBoard(currentBoard);

					if(score(currentBoard) >= 4){
						strcpy(mensagemServidor,"GO");
						send(client,mensagemServidor, strlen(mensagemServidor), 0);
							printf("=============GAME OVER===============\n");
							printf("=============VOCÊ PERDEU=============\n");
							buildBoard(enemyBoard);
							printf("Pontuação %s: %d \n", nome, score(enemyBoard));
							printf("Pontuação %s: %d \n", nomeCliente, score(currentBoard));
						break;
					} else {
						printf("======================BOMBA!======================\n");
					}
				} else {
					printf("======================ÁGUA!======================\n");
					build(water, currentBoard, value);
					buildBoard(currentBoard);					
					printf("======================ÁGUA!======================\n");
					strcpy(mensagemServidor,"AA");
					send(client,mensagemServidor, strlen(mensagemServidor), 0);
				}

				}

			}
	
		}
	}

//==========================FIM DO JOGO===========================//
//==========================FIM DO JOGO===========================//
	return 0;
}


}