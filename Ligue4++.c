#include <stdio.h>
#include <stdlib.h>

#define LINHAS 6
#define COLUNAS 7

// Definição de constantes para os jogadores e tokens
#define VAZIO 0
#define JOGADOR_1 1
#define JOGADOR_2 2

typedef struct {
    int tipo;       
    char simbolo;   
    int dono;       
} Posicao;

Posicao tabuleiro[LINHAS][COLUNAS];

//Funções Auxiliares

// Inicializa o tabuleiro definindo todas as posições como vazias
void inicializarTabuleiro() {
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            tabuleiro[i][j].dono = VAZIO;
            tabuleiro[i][j].tipo = VAZIO;
            tabuleiro[i][j].simbolo = '.';
        }
    }
}

// Imprime o tabuleiro na tela
void imprimirTabuleiro() {
    // Limpa a tela
    system("cls || clear"); 
    
    printf("\n === LIGUE 4++ (Etapa 1) === \n\n");
    
    // Imprime o cabeçalho das colunas
    printf("  1   2   3   4   5   6   7\n");
    printf("-----------------------------\n");
    
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        printf("|");
        for (j = 0; j < COLUNAS; j++) {
            printf(" %c |", tabuleiro[i][j].simbolo);
        }
        printf("\n-----------------------------\n");
    }
    printf("\n");
}

// Verifica se houve vitória (4 em sequência: horizontal, vertical ou diagonal),
int verificarVitoria(int jogador) {
    int i, j;
    // Varre o tabuleiro procurando sequências
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            if (tabuleiro[i][j].dono == jogador) {
                // Horizontal
                if (j + 3 < COLUNAS &&
                    tabuleiro[i][j+1].dono == jogador &&
                    tabuleiro[i][j+2].dono == jogador &&
                    tabuleiro[i][j+3].dono == jogador)
                    return 1;
                
                // Vertical
                if (i + 3 < LINHAS &&
                    tabuleiro[i+1][j].dono == jogador &&
                    tabuleiro[i+2][j].dono == jogador &&
                    tabuleiro[i+3][j].dono == jogador)
                    return 1;

                // Diagonal Principal (descendo para direita)
                if (i + 3 < LINHAS && j + 3 < COLUNAS &&
                    tabuleiro[i+1][j+1].dono == jogador &&
                    tabuleiro[i+2][j+2].dono == jogador &&
                    tabuleiro[i+3][j+3].dono == jogador)
                    return 1;

                // Diagonal Secundária (descendo para esquerda)
                if (i + 3 < LINHAS && j - 3 >= 0 &&
                    tabuleiro[i+1][j-1].dono == jogador &&
                    tabuleiro[i+2][j-2].dono == jogador &&
                    tabuleiro[i+3][j-3].dono == jogador)
                    return 1;
            }
        }
    }
    return 0;
}

// Função para inserir ficha comum com "gravidade"
int inserirFicha(int col, int jogador) {
    // Ajusta índice (usuário digita 1 a 7, matriz é 0 a 6)
    int j = col - 1;

    // Validação básica de limites
    if (j < 0 || j >= COLUNAS) {
        printf("Coluna invalida! Tente entre 1 e 7.\n");
        return 0; // Falha
    }

    // Lógica da Gravidade: percorre de baixo para cima (0)
    // A ficha ocupa a primeira posição livre encontrada
    int i;
    for (i = LINHAS - 1; i >= 0; i--) {
        if (tabuleiro[i][j].dono == VAZIO) {
            // Atualiza a struct da posição
            tabuleiro[i][j].dono = jogador;
            tabuleiro[i][j].tipo = 1; // Ficha Comum
            tabuleiro[i][j].simbolo = (jogador == JOGADOR_1) ? 'X' : 'O';
            return 1; // Sucesso
        }
    }

    printf("Coluna cheia! Escolha outra.\n");
    return 0; // Falha (coluna lotada)
}

// Loop principal da partida
void jogar() {
    inicializarTabuleiro();
    int turno = 1;
    int fimDeJogo = 0;
    int totalJogadas = 0;
    int maxJogadas = LINHAS * COLUNAS;

    // Loop do jogo
    while (fimDeJogo == 0 && totalJogadas < maxJogadas) {
        imprimirTabuleiro();

        // Define de quem é a vez (Ímpar = Jogador 1, Par = Jogador 2)
        int jogadorAtual = (turno % 2 != 0) ? JOGADOR_1 : JOGADOR_2;
        
        printf("Vez do Jogador %d (%c). Escolha a coluna (1-7): ", 
               jogadorAtual, (jogadorAtual == 1) ? 'X' : 'O');
        
        int coluna;
        scanf("%d", &coluna);

        // Tenta inserir a ficha. Se der certo, passa o turno.
        if (inserirFicha(coluna, jogadorAtual)) {
            // Verifica vitória após a jogada
            if (verificarVitoria(jogadorAtual)) {
                imprimirTabuleiro();
                printf("\nParabens! Jogador %d (%c) venceu!\n", 
                       jogadorAtual, (jogadorAtual == 1) ? 'X' : 'O');
                fimDeJogo = 1;
            }
            turno++;
            totalJogadas++;
        } else {
            // Se inserirFicha retornou 0, pede input novamente (turno não avança)
            printf("Pressione Enter para tentar novamente...");
            getchar(); getchar(); 
        }
    }

    if (!fimDeJogo) {
        imprimirTabuleiro();
        printf("\nO jogo terminou em EMPATE (Tabuleiro Cheio)!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar(); getchar();
}

// 4. Menu Principal,
int main() {
    int opcao = 0;

    do {
        system("cls || clear");
        printf("=== MENU LIGUE 4++ ===\n");
        printf("1. Iniciar Novo Jogo\n");
        printf("2. Hall dos Campeoes\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2:
                printf("\nFuncionalidade prevista para a Etapa 2.\n");
                printf("Pressione Enter para continuar...");
                getchar(); getchar();
                break;
            case 3:
                printf("\nSaindo do jogo... Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                getchar(); getchar();
        }
    } while (opcao != 3);

    return 0;
}
