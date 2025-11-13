/*
---------------------------------------------------------------
Programa: Sistema Interativo de Territórios - WAR Estruturado
Autor: [Joao Paulo]
Data: [13.11.2025]
Descrição:
    Este programa simula o cadastro e a interação entre territórios
    no estilo do jogo WAR. Ele utiliza structs, ponteiros e alocação
    dinâmica de memória. Após o cadastro dos territórios, o usuário
    pode simular ataques entre eles com resultados aleatórios.
---------------------------------------------------------------
*/

#include <stdio.h>    // Entrada e saída padrão
#include <stdlib.h>   // Funções malloc, free, rand, srand
#include <string.h>   // Manipulação de strings
#include <time.h>     // Controle de aleatoriedade (srand, time)

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
// Parâmetros: ponteiro para vetor de Territorio e quantidade n
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

        printf("\n");
    }
}

// -------------------------------------------------------------
// Função: exibirTerritorios
// Objetivo: exibir informações atualizadas de todos os territórios
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
// Parâmetros: ponteiros para atacante e defensor
// -------------------------------------------------------------
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Cada lado lança um "dado" entre 1 e 6
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n*** BATALHA ENTRE TERRITORIOS ***\n");
    printf("%s (%s) ataca %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: %s conquistou o territorio %s!\n", atacante->nome, defensor->nome);
        // O defensor muda de dono
        strcpy(defensor->cor, atacante->cor);
        // O atacante envia metade das tropas para o novo território
        defensor->tropas = atacante->tropas / 2;
        // O atacante perde metade das tropas enviadas
        atacante->tropas /= 2;
    } else {
        printf("Resultado: O ataque falhou! %s perdeu 1 tropa.\n", atacante->nome);
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// -------------------------------------------------------------
// Função: liberarMemoria
// Objetivo: liberar o espaço de memória alocado dinamicamente
// -------------------------------------------------------------
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}

// -------------------------------------------------------------
// Função principal (main)
// -------------------------------------------------------------
int main() {
    srand(time(NULL)); // Garante aleatoriedade nos dados de ataque

    int n;
    printf("Informe o numero de territorios a cadastrar: ");
    scanf("%d", &n);
    getchar(); // Limpa o buffer do teclado

    // Alocação dinâmica de memória para os territórios
    struct Territorio* mapa = (struct Territorio*) calloc(n, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    // Cadastro e exibição inicial
    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // ---------------------------------------------------------
    // Simulação de ataques
    // ---------------------------------------------------------
    char opcao;
    do {
        int indiceAtacante, indiceDefensor;

        printf("\n===== MENU DE ATAQUE =====\n");
        exibirTerritorios(mapa, n);
        printf("Escolha o numero do territorio atacante: ");
        scanf("%d", &indiceAtacante);
        printf("Escolha o numero do territorio defensor: ");
        scanf("%d", &indiceDefensor);

        // Validação de índices e cor
        if (indiceAtacante < 1 || indiceAtacante > n || indiceDefensor < 1 || indiceDefensor > n) {
            printf("Indices invalidos! Tente novamente.\n");
        } else if (strcmp(mapa[indiceAtacante - 1].cor, mapa[indiceDefensor - 1].cor) == 0) {
            printf("Um territorio nao pode atacar outro da mesma cor!\n");
        } else if (mapa[indiceAtacante - 1].tropas <= 0) {
            printf("O territorio atacante nao tem tropas suficientes!\n");
        } else {
            atacar(&mapa[indiceAtacante - 1], &mapa[indiceDefensor - 1]);
        }

        // Exibição após ataque
        exibirTerritorios(mapa, n);

        printf("Deseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opcao);
    } while (opcao == 's' || opcao == 'S');

    // ---------------------------------------------------------
    // Encerramento
    // ---------------------------------------------------------
    liberarMemoria(mapa);
    printf("\nFim do programa. Obrigado por jogar!\n");

    return 0;
}
