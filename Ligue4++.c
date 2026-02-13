#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINHAS 6
#define COLUNAS 7

// Constantes
#define VAZIO 0
#define JOGADOR_1 1
#define JOGADOR_2 2

// Tipos de Ficha
#define COMUM 1
#define EXPLOSIVA 2
#define PORTAL 3

// Struct Jogador atualizada para a Etapa 2
typedef struct {
    char nome[21];
    int qtdComum;     // Começa com 21
    int qtdExplosiva; // Ganha +1 a cada 5 rodadas
    int qtdPortal;    // Ganha +1 a cada 5 rodadas
    int id;           // 1 ou 2
    int jogadas;      // Para o ranking
} Jogador;

// Struct Posicao (Mantida, com uso do campo 'tipo')
typedef struct {
    int tipo;
    char simbolo;
    int dono;
} Posicao;

// Variáveis Globais
Posicao tabuleiro[LINHAS][COLUNAS];
Jogador j1, j2; 

// Funções Básicas

void inicializarTabuleiro() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            tabuleiro[i][j].dono = VAZIO;
            tabuleiro[i][j].tipo = VAZIO;
            tabuleiro[i][j].simbolo = '.';
        }
    }
}

void imprimirTabuleiro() {
    system("cls || clear");
    printf("\n === LIGUE 4++ (Etapa 2) === \n");
    printf("  1   2   3   4   5   6   7\n");
    printf("-----------------------------\n");
    for (int i = 0; i < LINHAS; i++) {
        printf("|");
        for (int j = 0; j < COLUNAS; j++) {
            printf(" %c |", tabuleiro[i][j].simbolo);
        }
        printf("\n-----------------------------\n");
    }
    printf("\n");
}

int verificarVitoria(int jogador) {
    // Varre o tabuleiro procurando sequências de 4
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (tabuleiro[i][j].dono == jogador) {
                // Horizontal
                if (j + 3 < COLUNAS &&
                    tabuleiro[i][j+1].dono == jogador &&
                    tabuleiro[i][j+2].dono == jogador &&
                    tabuleiro[i][j+3].dono == jogador) return 1;
                // Vertical
                if (i + 3 < LINHAS &&
                    tabuleiro[i+1][j].dono == jogador &&
                    tabuleiro[i+2][j].dono == jogador &&
                    tabuleiro[i+3][j].dono == jogador) return 1;
                // Diagonal Principal
                if (i + 3 < LINHAS && j + 3 < COLUNAS &&
                    tabuleiro[i+1][j+1].dono == jogador &&
                    tabuleiro[i+2][j+2].dono == jogador &&
                    tabuleiro[i+3][j+3].dono == jogador) return 1;
                // Diagonal Secundária
                if (i + 3 < LINHAS && j - 3 >= 0 &&
                    tabuleiro[i+1][j-1].dono == jogador &&
                    tabuleiro[i+2][j-2].dono == jogador &&
                    tabuleiro[i+3][j-3].dono == jogador) return 1;
            }
        }
    }
    return 0;
}

// Função Recursiva de Explosão
void explodir(int l, int c) {
    if (l < 0 || l >= LINHAS || c < 0 || c >= COLUNAS) return;

    // Limpa a posição central
    tabuleiro[l][c].tipo = VAZIO;
    tabuleiro[l][c].dono = VAZIO;
    tabuleiro[l][c].simbolo = '.';

    // Percorre vizinhança 3x3
    for (int i = l - 1; i <= l + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS) {
                // Se encontrar OUTRA explosiva, RECURSÃO
                if (tabuleiro[i][j].tipo == EXPLOSIVA) {
                    explodir(i, j); 
                } else {
                    // Se for comum ou portal, apenas destrói
                    tabuleiro[i][j].tipo = VAZIO;
                    tabuleiro[i][j].dono = VAZIO;
                    tabuleiro[i][j].simbolo = '.';
                }
            }
        }
    }
}

// Função Inserir Modificada
// Retorna 1 se a jogada aconteceu, 0 se inválida
int realizarJogada(int col, int tipo, Jogador *jog) {
    int j = col - 1; // Ajuste índice
    if (j < 0 || j >= COLUNAS) {
        if(jog->id < 3) printf("Coluna invalida!\n"); // Só avisa se for humano
        return 0;
    }

    // -- LÓGICA DO PORTAL --
    if (tipo == PORTAL) {
        // Portal elimina a ficha de baixo e some
        // Procura a primeira ficha ocupada (de cima pra baixo)
        for (int i = 0; i < LINHAS; i++) {
            if (tabuleiro[i][j].dono != VAZIO) {
                tabuleiro[i][j].dono = VAZIO;
                tabuleiro[i][j].tipo = VAZIO;
                tabuleiro[i][j].simbolo = '.';
                jog->qtdPortal--;
                return 1; 
            }
        }
        // Se a coluna estava vazia, o portal some inutilmente
        jog->qtdPortal--;
        return 1;
    }

    // LÓGICA COMUM E EXPLOSIVA (Gravidade)
    for (int i = LINHAS - 1; i >= 0; i--) {
        if (tabuleiro[i][j].dono == VAZIO) {
            
            // Verifica o que tem EMBAIXO (para regra da mina)
            if (i + 1 < LINHAS) {
                Posicao *abaixo = &tabuleiro[i+1][j];
                
                // Se cair em cima de EXPLOSIVA INIMIGA -> BUM!
                if (abaixo->tipo == EXPLOSIVA && abaixo->dono != jog->id) {
                    explodir(i+1, j); 
                    // A ficha jogada também é gasta
                    if (tipo == COMUM) jog->qtdComum--;
                    else jog->qtdExplosiva--;
                    return 1;
                }
                
                // Se cair em cima de EXPLOSIVA AMIGA -> Desativa
                if (abaixo->tipo == EXPLOSIVA && abaixo->dono == jog->id) {
                    abaixo->tipo = COMUM; // Vira comum (não explode)
                }
            }

            // Inserção Normal
            tabuleiro[i][j].dono = jog->id;
            tabuleiro[i][j].tipo = tipo;
            
            // Define Símbolo
            if (tipo == EXPLOSIVA) tabuleiro[i][j].simbolo = (jog->id == 1) ? '*' : '+';
            else tabuleiro[i][j].simbolo = (jog->id == 1) ? 'X' : 'O';

            // Desconta Inventário
            if (tipo == COMUM) jog->qtdComum--;
            else jog->qtdExplosiva--;
            
            return 1;
        }
    }

    if(jog->id < 3) printf("Coluna cheia!\n");
    return 0;
}

// Hall da Fama (Arquivos)
void gerenciarHall(Jogador vencedor) {
    Jogador hall[3];
    // Zera o hall temporário
    for(int k=0; k<3; k++) hall[k].jogadas = 9999;

    FILE *arq = fopen("hall_campeoes.bin", "rb");
    if (arq != NULL) {
        fread(hall, sizeof(Jogador), 3, arq);
        fclose(arq);
    }

    // Verifica se entra no top 3
    int entrou = 0;
    for (int i = 0; i < 3; i++) {
        if (vencedor.jogadas < hall[i].jogadas) {
            // Desloca
            for (int k = 2; k > i; k--) hall[k] = hall[k-1];
            // Insere
            hall[i] = vencedor;
            entrou = 1;
            break;
        }
    }

    if (entrou) {
        printf("\nPARABENS! Voce entrou para o Hall dos Campeões!\n");
        arq = fopen("hall_campeoes.bin", "wb");
        fwrite(hall, sizeof(Jogador), 3, arq);
        fclose(arq);
    }
}

void exibirHall() {
    Jogador hall[3];
    FILE *arq = fopen("hall_campeoes.bin", "rb");
    if (arq == NULL) {
        printf("\nAinda não tem campeoes registrados.\n");
        return;
    }
    fread(hall, sizeof(Jogador), 3, arq);
    
    printf("\n=== HALL DOS CAMPEOES ===\n");
    for (int i = 0; i < 3; i++) {
        if (hall[i].jogadas < 9999)
            printf("%d. %s - %d jogadas\n", i+1, hall[i].nome, hall[i].jogadas);
    }
    fclose(arq);
    printf("\nPressione Enter...");
    getchar(); getchar();
}

// Jogo Principal
void jogar(int modo) {
    // Inicializa Jogadores
    j1.id = 1; j1.qtdComum = 21; j1.qtdExplosiva = 0; j1.qtdPortal = 0; j1.jogadas = 0;
    j2.id = 2; j2.qtdComum = 21; j2.qtdExplosiva = 0; j2.qtdPortal = 0; j2.jogadas = 0;

    // Configura Nomes
    if (modo == 1 || modo == 2) {
        printf("Nome Jogador 1: "); scanf(" %[^\n]", j1.nome);
    } else strcpy(j1.nome, "CPU 1");

    if (modo == 2) {
        printf("Nome Jogador 2: "); scanf(" %[^\n]", j2.nome);
    } else strcpy(j2.nome, "CPU 2");

    inicializarTabuleiro();
    int turno = 1; // Ímpar = J1, Par = J2
    int fim = 0;
    int rodadaGlobal = 0;

    while (!fim && rodadaGlobal < 42) { // Max 42 casas
        imprimirTabuleiro();

        // Distribuição de Fichas Especiais a cada 5 rodadas) 
        // Se turno > 1 e (turno-1) % 10 == 0 (5 rodadas de cada) -> ganham fichas
        if (turno > 1 && (turno-1) % 10 == 0) {
            j1.qtdExplosiva++; j1.qtdPortal++;
            j2.qtdExplosiva++; j2.qtdPortal++;
            printf("Rodada %d: Jogadores ganharam fichas especiais!\n", (turno-1)/2);
        }

        Jogador *atual = (turno % 2 != 0) ? &j1 : &j2;
        int ehHumano = 1;
        if (modo == 3) ehHumano = 0; // Automático
        if (modo == 1 && atual->id == 2) ehHumano = 0; // 1 Jogador (vs CPU)

        printf("Vez de %s (%c)\n", atual->nome, (atual->id==1)?'X':'O');
        printf("Inventario: [1]Comum:%d  [2]Explosiva:%d  [3]Portal:%d\n\n", 
               atual->qtdComum, atual->qtdExplosiva, atual->qtdPortal);

        int col, tipo;

        if (ehHumano) {
            printf("Escolha TIPO (1,2,3) e COLUNA (1-7): ");
            if (scanf("%d %d", &tipo, &col) != 2) {
                while(getchar()!='\n'); continue;
            }
        } else {
            // IA Simples (Aleatória)
            col = (rand() % 7) + 1;
            tipo = (rand() % 3) + 1; 
            // CPU tenta usar especial se tiver, senão usa comum
            if (tipo == EXPLOSIVA && atual->qtdExplosiva == 0) tipo = COMUM;
            if (tipo == PORTAL && atual->qtdPortal == 0) tipo = COMUM;
        }

        // Validação de Inventário
        int temFicha = 0;
        if (tipo == COMUM && atual->qtdComum > 0) temFicha = 1;
        if (tipo == EXPLOSIVA && atual->qtdExplosiva > 0) temFicha = 1;
        if (tipo == PORTAL && atual->qtdPortal > 0) temFicha = 1;

        if (!temFicha) {
            if(ehHumano) {
                printf("Voce não tem essa ficha!\n");
                getchar(); getchar();
            }
            continue;
        }

        // Tenta Jogar
        if (realizarJogada(col, tipo, atual)) {
            atual->jogadas++;
            rodadaGlobal++;
            
            if (verificarVitoria(atual->id)) {
                imprimirTabuleiro();
                printf("\nVITORIA DE %s!\n", atual->nome);
                gerenciarHall(*atual);
                fim = 1;
            } else {
                turno++;
            }
        }
    }

    if (!fim) printf("EMPATE!\n");
    printf("Pressione Enter para voltar...");
    getchar(); getchar();
}

// Menu (Atualizado)
int main() {
    srand(time(NULL)); // Aleatórios
    int opcao = 0;

    do {
        system("cls || clear");
        printf("=== MENU LIGUE 4++ (ETAPA 2) ===\n");
        printf("1. 1 Jogador (Vs CPU)\n");
        printf("2. 2 Jogadores\n");
        printf("3. Automatico (CPU vs CPU)\n");
        printf("4. Hall dos Campeoes\n");
        printf("5. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogar(1); break;
            case 2: jogar(2); break;
            case 3: jogar(3); break;
            case 4: exibirHall(); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 5);

    return 0;
}
