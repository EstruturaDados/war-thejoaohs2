/*
---------------------------------------------------------------
Programa: WAR Estruturado - Missões Estratégicas (Versão Final)
Autor: [Joao Paulo]
Data: [13.11.2025]
Descrição:
    Este programa é a versão completa do sistema WAR estruturado.
    Ele permite o cadastro dinâmico de territórios, simulação de ataques,
    e agora inclui a atribuição e verificação de missões estratégicas
    para cada jogador. O código utiliza structs, ponteiros, alocação
    dinâmica e modularização de funções, simulando um jogo de estratégia.
---------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------------------------------------------------
// Definição da estrutura "Territorio"
// -------------------------------------------------------------
struct Territorio {
    char nome[30];   // Nome do território
    char cor[10];    // Cor do exército
    int tropas;      // Quantidade de tropas
};

// -------------------------------------------------------------
// Função: cadastrarTerritorios
// Objetivo: permitir ao usuário cadastrar todos os territórios
// -------------------------------------------------------------
void cadastrarTerritorios(struct Territorio* mapa, int n) {
    printf("===== CADASTRO DE TERRITORIOS =====\n\n");
    for (int i = 0; i < n; i++) {
        printf("Territorio %d\n", i + 1);
        fflush(stdin);

        printf("Digite o nome do territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exercito: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();

        printf("\n");
    }
}

// -------------------------------------------------------------
// Função: exibirTerritorios
// Objetivo: exibir as informações dos territórios cadastrados
// -------------------------------------------------------------
void exibirTerritorios(struct Territorio* mapa, int n) {
    printf("\n===== DADOS DOS TERRITORIOS =====\n\n");
    for (int i = 0; i < n; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exercito: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
        printf("-----------------------------------------\n");
    }
}

// -------------------------------------------------------------
// Função: atacar
// Objetivo: simular um ataque entre dois territórios usando rand()
// -------------------------------------------------------------
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n*** BATALHA ENTRE TERRITORIOS ***\n");
    printf("%s (%s) ataca %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: %s conquistou o territorio %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("Resultado: O ataque falhou! %s perdeu 1 tropa.\n", atacante->nome);
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// -------------------------------------------------------------
// Função: atribuirMissao
// Objetivo: sortear uma missão aleatória e atribuí-la ao jogador
// -------------------------------------------------------------
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// -------------------------------------------------------------
// Função: verificarMissao
// Objetivo: verificar se o jogador cumpriu sua missão
// -------------------------------------------------------------
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* corJogador) {
    /*
        A lógica abaixo é simplificada:
        - Se a missão contiver "Conquistar", verifica se o jogador
          possui pelo menos 3 territórios.
        - Se contiver "Eliminar", verifica se não há territórios
          com a cor "vermelha".
    */
    int territoriosJogador = 0;
    int existeVermelho = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0)
            territoriosJogador++;
        if (strcmp(mapa[i].cor, "vermelha") == 0)
            existeVermelho = 1;
    }

    if (strstr(missao, "Conquistar") != NULL && territoriosJogador >= 3)
        return 1;
    if (strstr(missao, "Eliminar") != NULL && !existeVermelho)
        return 1;

    return 0;
}

// -------------------------------------------------------------
// Função: liberarMemoria
// Objetivo: liberar memória dos territórios e missões
// -------------------------------------------------------------
void liberarMemoria(struct Territorio* mapa, char* missao1, char* missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
    printf("\nMemoria liberada com sucesso.\n");
}

// -------------------------------------------------------------
// Função principal (main)
// -------------------------------------------------------------
int main() {
    srand(time(NULL));

    int n;
    printf("Informe o numero de territorios a cadastrar: ");
    scanf("%d", &n);
    getchar();

    // Alocação dinâmica do vetor de territórios
    struct Territorio* mapa = (struct Territorio*) calloc(n, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // ---------------------------------------------------------
    // Criação do vetor de missões pré-definidas
    // ---------------------------------------------------------
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar metade do mapa",
        "Conquistar 2 territorios do inimigo azul",
        "Ter pelo menos 20 tropas em um territorio"
    };
    int totalMissoes = 5;

    // ---------------------------------------------------------
    // Atribuição dinâmica de missões para dois jogadores
    // ---------------------------------------------------------
    char* missaoJogador1 = (char*) malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*) malloc(100 * sizeof(char));
    if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
        printf("Erro ao alocar memoria para as missoes!\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\n===== MISSOES ATRIBUIDAS =====\n");
    printf("Jogador 1 (cor azul): %s\n", missaoJogador1);
    printf("Jogador 2 (cor vermelha): %s\n", missaoJogador2);

    // ---------------------------------------------------------
    // Simulação de ataques e verificação das missões
    // ---------------------------------------------------------
    char opcao;
    int vencedor = 0;
    do {
        int indiceAtacante, indiceDefensor;
        printf("\n===== MENU DE ATAQUE =====\n");
        exibirTerritorios(mapa, n);

        printf("Escolha o numero do territorio atacante: ");
        scanf("%d", &indiceAtacante);
        printf("Escolha o numero do territorio defensor: ");
        scanf("%d", &indiceDefensor);

        if (indiceAtacante < 1 || indiceAtacante > n || indiceDefensor < 1 || indiceDefensor > n) {
            printf("Indices invalidos! Tente novamente.\n");
        } else if (strcmp(mapa[indiceAtacante - 1].cor, mapa[indiceDefensor - 1].cor) == 0) {
            printf("Um territorio nao pode atacar outro da mesma cor!\n");
        } else {
            atacar(&mapa[indiceAtacante - 1], &mapa[indiceDefensor - 1]);
        }

        exibirTerritorios(mapa, n);

        // Verificação de missão cumprida para cada jogador
        if (verificarMissao(missaoJogador1, mapa, n, "azul")) {
            printf("\n>>> Jogador 1 (azul) cumpriu sua missao: %s <<<\n", missaoJogador1);
            vencedor = 1;
        }
        if (verificarMissao(missaoJogador2, mapa, n, "vermelha")) {
            printf("\n>>> Jogador 2 (vermelha) cumpriu sua missao: %s <<<\n", missaoJogador2);
            vencedor = 2;
        }

        if (vencedor != 0)
            break;

        printf("Deseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opcao);
    } while (opcao == 's' || opcao == 'S');

    if (vencedor == 1)
        printf("\n*** FIM DE JOGO: Jogador 1 venceu! ***\n");
    else if (vencedor == 2)
        printf("\n*** FIM DE JOGO: Jogador 2 venceu! ***\n");
    else
        printf("\nJogo encerrado sem vencedor.\n");

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    return 0;
}
