# battleship_c

## O jogo

Os soldados russos criaram o conceito do jogo durante a Primeira Guerra Mundial. Eles improvisavam um mapa quadriculado em um pedaço de papel, e ganhava quem acertasse as coordenadas dos veículos inimigos primeiro.
No século XX, o passatempo tornou-se popular entre os prisioneiros e soldados. Nos anos 30 apareceu a primeira versão comercial do jogo, ainda em papel, e se chamava Salvo. Durante a Segunda Guerra Mundial, foi lançado com o jogo no qual é conhecido até hoje: Battleship. E durante a Guerra Fria surgiu a primeira versão de tabuleiro, com peças encaixáveis na famosa maletinha de plástico.

## Sobre o programa

É possível executar o jogo de diversas formas em ambos sistemas operacionais: Windows e Linux.

### Como executar

1º: é necessário iniciar o programa _BatalhaServidor.exe_. Ele irá questionar qual o IP que você irá se conectar (ex: 127.0.0.1). Após isso, o jogo será iniciado e o mesmo ficará aguardando um jogador se conectar.

2º: é necessário iniciar o programa _BatalhaCliente.exe_. Ele irá questionar qual o IP do servidor que você irá se conectar (ex: 127.0.0.1). Após isso, o jogo será iniciado e o programa _BatalhaServidor.exe_ reconhecerá que entrou um jogador na _sala_ para competir.

### Sobre o código

O código foi escrito em C utilizando as bibliotecas abaixo:

- include <stdio.h>
- include <stdlib.h>
- include <string.h>
- include <errno.h>
- include <arpa/inet.h>
- include <sys/socket.h>
- include <sys/types.h>
- include <netinet/in.h>

#### Dados do socket

O socket foi criado a partir de um struct em C com os seguintes atributos/dados:

- remoto.sin_family: AF_NET                        -> é a família do IP a ser conectado (AF_NET refere-se ao IPv4 no caso).
- remoto.sin_port      = htons(PORTA);             -> é a porta de comunicação que o cliente se comunicará com o server (2000 no caso).
- remoto.sin_addr.s_addr      = inet_addr(ip);     -> é o IP do servidor propriamente dito (que o usuário digitará).

#### Métodos

Foram utilizados os seguintes métodos:

##### void buildBoard(int x[10][10])
Este método não possui retorno. Ele irá mapear os valores números presentes na matriz e irá converter na arte ASCII.

- //0 o Mira
- //1 ~ Água
- //2 # Barco

Além disso, ele desenha os eixos alfanúmericos para que o usuário visualize as coordenadas.

##### void cleanBoard(int x[10][10])
Este método não possui retorno. Ele irá criar/inicializar o tabuleiro (vazio, sem posições de navios). Após isso, o jogador que irá escolher as posições que deseja alocar seus navios.

##### int score(int x[10][10])
Este método realiza a pontuação atual de determinado jogo para verificar todos os navios já foram derrotados.

##### int findAbscissa(char abscissa)
Este método recupera o valor da coordenada 'letra' (ABCDEFGHIJ) e converte ela no índice correspondente da matriz no mapa.

##### void build(int type, int campo[10][10], char coordenadas[2])
Este método não possui retorno. Ele irá gravar no mapa de acordo com as coordenadas algum valor (0 para água ou 1 para barco).

##### void buildOthersShips(int shipType, int xY, int campo[10][10], char coordenadas[2])
Este método não possui retorno. Ele irá gravar os demais navios (exceto o submarino).
Os parametros são:
- shipType: o tamanho do navio;
- int xY: a posição que o jogador gostaria que ele estivesse (vertical ou horizontal).
- campo: o mapa do jogador

##### int bomb(int campo[10][10], char coordenadas[2])
Este método possui retorno. Ele irá verificar se determinada coordenada representa um navio.
0 - para água.
1 - caso seja bomba, ou seja, a coordenada corresponde ao barco.

##### void drawBattleShip()
Este método possui retorno. Ele é a primeira parte gráfica do jogo contemplando a seguinte Asci arte:













