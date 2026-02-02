#include <stdio.h>
#include <stdlib.h>
    
typedef struct {
    char simbolo; 
    int jogador; 
} Posicao;

void iniciarTabuleiro(Posicao tabuleiro[6][7]){
    for (int i=0; i<6; i++){
        for (int j=0; j<7; j++){
            tabuleiro[i][j].simbolo = ' ';
            tabuleiro[i][j].jogador = 0;
    
        }
    }
}
void imprimitTabuleiro(Posicao tabuleiro[6][6]){
    printf("\n");
    for (int i = 0; i < 6; i++) {
        printf("|");
        for (int j = 0; j < 7; j++) {
            printf(" %c |", tabuleiro[i][j].simbolo);
        }
        printf("\n");
    }

    printf("  1   2   3   4   5   6   7\n\n");
}

int inserirFicha(Posicao tabuleiro[6][7], int coluna, int jogador){
    for (int i=5; i>=0; i--){
        if (tabuleiro[i][coluna].jogador ==0 ){
            tabuleiro[i][coluna].jogador = jogador;
            tabuleiro[i][coluna].simbolo = (jogador == 1) ? 'X' : 'O';
            return 1;
        }
    }
    return 1;
}






int main(){

	int opcao;
   do {
	printf("Ligue 4++\n");
    printf("1. Inicia novo jogo\n");
    printf("2. Hall dos campeões\n");
    printf("3. Sair\n");
    printf("Escolha uma opção; ");
    scanf("%d", &opcao);

    if (opcao == 1){
        jogar();
    }else if (opcao == 2){
        printf("Sair\n");
    } else {
        printf("Opcao invalida.\n");
    }
    return 0;
   }
    /*switch(opcao)
    {
        case 1:
            printf("Iniciando novo jogo...\n");
            // Código para iniciar um novo jogo
            break;
        case 2:
            printf("Hall dos campeões...\n");
            // Código para mostrar o hall dos campeões
            break;
        case 3:
            printf("Saindo do jogo...\n");
            // Código para sair do jogo
            break;
        default:
            printf("Opção inválida!\n");
    }

    

    Posicao tabuleiro[6][7];

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            tabuleiro[i][j].simbolo = ' ';
            tabuleiro[i][j].jogador = 0;
        }   
    }

    printf("\n");


    int coluna;

    printf("Vez do jogador 1 (X): ");
    scanf("%d", &coluna);
    coluna--;

    if (coluna < 0 || coluna > 6) {
        printf("Coluna invalida!\n");
        return 0;
    }

    int linhainserida = -1;
    for (int i = 5; i >= 0; i--) {
        if (tabuleiro[i][coluna].jogador == 0) {
            tabuleiro[i][coluna].simbolo = 'X';
            tabuleiro[i][coluna].jogador = 1;
            linhainserida = i;
            break;
        }
    }

    if(linhainserida == -1) {
        printf("Essa coluna está cheia!\n");
        
    }
	return 0;
}*/
