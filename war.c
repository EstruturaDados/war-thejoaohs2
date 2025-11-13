/*
WAR Estruturado - Nível Novato, Aventureiro e Mestre (versão integrada)
Autor: [Joao Paulo]
Data: [13.11.2025]
Descrição:
    Programa que reúne os três níveis do desafio:
      - Novato: struct Territorio, cadastro e exibição.
      - Aventureiro: alocação dinâmica, ataques entre territórios usando ponteiros.
      - Mestre: sistema de missões estratégicas com sorteio, armazenamento dinâmico e verificação.
    O código utiliza fgets+sscanf para leitura segura e evita fflush(stdin).
    Comentários explicativos inclusos para facilitar manutenção e entendimento.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ----------------------------
   Definições e constantes
   ---------------------------- */
#define MAX_NOME 30
#define MAX_COR  15
#define TAM_MISSAO 200

/* Estrutura que representa um território. (Nível Novato) */
typedef struct {
    char nome[MAX_NOME];   // nome do território
    char cor[MAX_COR];     // cor do exército dono do território
    int tropas;            // quantidade de tropas naquele território
} Territorio;

/* ----------------------------
   Funções utilitárias de I/O
   ---------------------------- */

/* Leitura segura de linha: usa fgets e remove o '\n' final, se existir */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

/* Leitura segura de inteiro: pede repetidamente até obter um inteiro válido */
int lerInteiro(const char *prompt) {
    char linha[128];
    int valor;
    while (1) {
        printf("%s", prompt);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return 0; // EOF ou erro -> retornar 0
        }
        if (sscanf(linha, "%d", &valor) == 1) {
            return valor;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
}

/* ----------------------------
   Nível Novato: cadastro e exibição
   ---------------------------- */

/*
  cadastrarTerritoriosFixo:
  - Exemplo simples para o nível Novato (opcional).
  - Aqui mantemos a versão segura usando fgets.
*/
void cadastrarTerritoriosFixo(Territorio mapa[], int n) {
    printf("=== CADASTRO (NIVEL NOVATO) ===\n");
    for (int i = 0; i < n; ++i) {
        printf("Territorio %d\n", i + 1);
        printf("Digite o nome do territorio: ");
        lerLinha(mapa[i].nome, MAX_NOME);

        printf("Digite a cor do exercito: ");
        lerLinha(mapa[i].cor, MAX_COR);

        mapa[i].tropas = lerInteiro("Digite a quantidade de tropas: ");
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;

        printf("\n");
    }
}

/* Exibição clara dos territórios (usada em todos os níveis) */
void exibirTerritorios(const Territorio *mapa, int n) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < n; ++i) {
        printf("[%2d] Nome: %-20s | Cor: %-8s | Tropas: %3d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("======================\n");
}

/* ----------------------------
   Nível Aventureiro: ataques e alocação dinâmica
   ---------------------------- */

/*
  atacar:
  - Simula um ataque entre dois territórios (ponteiros).
  - Usa rand() para gerar um "dado" de 1 a 6 para atacante e defensor.
  - Se atacante vencer -> defensor muda de cor para a do atacante e
    recebe metade das tropas do atacante (metade arredondada para baixo).
    O atacante fica com o restante.
  - Se perder -> atacante perde 1 tropa (não fica negativo).
  - Todas as alterações são feitas via ponteiros.
*/
void atacar(Territorio *atacante, Territorio *defensor) {
    if (atacante == NULL || defensor == NULL) return;

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\n--- BATALHA ---\n");
    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("Resultado: atacante venceu!\n");
        // transferir propriedade e tropas
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas = atacante->tropas - tropasTransferidas;
    } else {
        printf("Resultado: defensor resistiu. Atacante perde 1 tropa.\n");
        atacante->tropas = (atacante->tropas > 0) ? atacante->tropas - 1 : 0;
    }
    printf("--------------\n");
}

/* ----------------------------
   Nível Mestre: missões estratégicas
   ---------------------------- */

/*
  atribuirMissao:
  - Sorteia (rand) uma das missoes e copia para destino (ponteiro char*).
  - Destino deve apontar para memória válida (malloc) com espaço suficiente.
*/
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    if (destino == NULL || missoes == NULL || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/*
  verificarMissao:
  - Lógica simples para verificação de uma missão (pode ser expandida).
  - Parâmetros:
      missao: string com a descrição da missão
      mapa: ponteiro para vetor de Territorio
      tamanho: quantos territórios no mapa
      corJogador: cor que representa o jogador (ex: "azul" ou "vermelha")
  - Retorna 1 se cumprida, 0 caso contrário.
  - A verificação atual suporta as seguintes lógicas (string contém):
      - "Conquistar X" -> verifica se jogador tem >= X territórios
      - "Eliminar ... vermelha" -> verifica se nao existe mais territorios de cor "vermelha"
      - "Dominar metade" -> verifica se possui >= metade dos territórios
      - "Conquistar 2 territorios do inimigo azul" -> verifica se conquistou 2 territórios cuja cor anterior era "azul" (simplificação)
      - "20 tropas" -> verifica se possui ao menos 20 tropas em algum território
*/
int verificarMissao(const char *missao, Territorio *mapa, int tamanho, const char *corJogador) {
    if (missao == NULL || mapa == NULL || corJogador == NULL) return 0;

    int contJogador = 0;
    int existeVermelha = 0;
    int totalTropasMaior20 = 0;

    for (int i = 0; i < tamanho; ++i) {
        if (strcmp(mapa[i].cor, corJogador) == 0) contJogador++;
        if (strcmp(mapa[i].cor, "vermelha") == 0) existeVermelha = 1;
        if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 20) totalTropasMaior20 = 1;
    }

    // Missao: "Conquistar 3 territorios seguidos" -> checar >=3 territorios
    if (strstr(missao, "Conquistar") != NULL) {
        // tenta extrair um numero na string
        int x = 0;
        const char *p = missao;
        while (*p) {
            if (sscanf(p, "%d", &x) == 1) break;
            p++;
        }
        if (x > 0 && contJogador >= x) return 1;
        // se nao achou numero, talvez seja 'Conquistar 2 territorios do inimigo azul' -> checar >=2 de cor diferente
        if (strstr(missao, "inimigo azul") != NULL) {
            // simplificação: verificar se existem ao menos 2 territorios com cor "azul" que nao sejam do jogador
            int contAzulInimigo = 0;
            for (int i = 0; i < tamanho; ++i) {
                if (strcmp(mapa[i].cor, "azul") == 0 && strcmp(corJogador, "azul") != 0) contAzulInimigo++;
            }
            if (contAzulInimigo >= 2) return 1;
        }
    }

    // Missao: "Eliminar todas as tropas da cor vermelha"
    if (strstr(missao, "Eliminar") != NULL && strstr(missao, "vermelha") != NULL) {
        if (!existeVermelha) return 1;
    }

    // Missao: "Dominar metade do mapa"
    if (strstr(missao, "Dominar metade") != NULL) {
        if (contJogador >= (tamanho + 1) / 2) return 1; // >= metade arredondando para cima
    }

    // Missao: "Ter pelo menos 20 tropas em um territorio"
    if (strstr(missao, "20 tropas") != NULL) {
        if (totalTropasMaior20) return 1;
    }

    return 0; // por padrão, nao cumpriu
}

/* ----------------------------
   Gerenciamento de memória e utilitários finais
   ---------------------------- */

/* liberarMemoria:
   libera mapa e missoes (ponteiros alocados dinamicamente)
*/
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2) {
    if (mapa != NULL) free(mapa);
    if (missao1 != NULL) free(missao1);
    if (missao2 != NULL) free(missao2);
}

/* ----------------------------
   Função principal (main)
   ---------------------------- */
int main(void) {
    srand((unsigned int) time(NULL)); // garante aleatoriedade para rand()

    printf("=== WAR Estruturado (Novato | Aventureiro | Mestre) ===\n\n");

    /* Escolha do número de territórios (dinâmico) */
    int n = 0;
    while (n <= 0) {
        n = lerInteiro("Informe o numero de territorios a cadastrar (ex: 5): ");
        if (n <= 0) printf("Informe um numero maior que zero.\n");
    }

    /* Alocação dinâmica do mapa (Nível Aventureiro) */
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: memoria insuficiente para alocar o mapa.\n");
        return 1;
    }

    /* Cadastro: usamos a rotina segura (fgets) (Nível Novato adaptado) */
    cadastrarTerritoriosFixo(mapa, n);

    /* Exibição inicial do mapa */
    exibirTerritorios(mapa, n);

    /* -------------------------
       MISSÕES (Nível Mestre)
       - Criamos um vetor de 5 missoes como pedido
       - As missões serão alocadas dinamicamente por jogador
       ------------------------- */
    char *missoesPadrao[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar metade do mapa",
        "Conquistar 2 territorios do inimigo azul",
        "Ter pelo menos 20 tropas em um territorio"
    };
    int totalMissoes = sizeof(missoesPadrao) / sizeof(missoesPadrao[0]);

    /* Alocação dinâmica para missões dos dois jogadores */
    char *missaoJogador1 = (char *) malloc(TAM_MISSAO * sizeof(char));
    char *missaoJogador2 = (char *) malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
        printf("Erro: memoria insuficiente para alocar missoes.\n");
        liberarMemoria(mapa, missaoJogador1, missaoJogador2);
        return 1;
    }

    /* Atribuir missões aleatórias (cada jogador recebe uma) */
    atribuirMissao(missaoJogador1, missoesPadrao, totalMissoes);
    atribuirMissao(missaoJogador2, missoesPadrao, totalMissoes);

    /* Exibir missões aos jogadores apenas uma vez (interface intuitiva) */
    printf("\n--- Missoes Atribuidas ---\n");
    printf("Jogador 1 (cor azul) : %s\n", missaoJogador1);
    printf("Jogador 2 (cor vermelha) : %s\n", missaoJogador2);
    printf("--------------------------\n");

    /* -------------------------
       LOOP principal: permitirá ataques e verificações de missão
       - Validações: indices, atques contra mesma cor, tropas suficientes
       - Usa ponteiros em atacar()
       ------------------------- */
    int vencedor = 0;
    char continuar[8] = "s";

    while ((continuar[0] == 's' || continuar[0] == 'S') && vencedor == 0) {
        exibirTerritorios(mapa, n);

        printf("\n--- MENU DE ATAQUE ---\n");
        int ia = lerInteiro("Escolha o numero do territorio atacante: ");
        int id = lerInteiro("Escolha o numero do territorio defensor: ");

        /* Validacoes de indices */
        if (ia < 1 || ia > n || id < 1 || id > n) {
            printf("Indices invalidos! Escolha valores entre 1 e %d.\n", n);
            printf("Deseja tentar novamente? (s/n): ");
            lerLinha(continuar, sizeof(continuar));
            continue;
        }

        Territorio *atac = &mapa[ia - 1];
        Territorio *defe = &mapa[id - 1];

        /* Nao permitir atacar o mesmo territorio ou atacar mesmo time */
        if (ia == id) {
            printf("Um territorio nao pode atacar a si mesmo.\n");
            printf("Deseja tentar novamente? (s/n): ");
            lerLinha(continuar, sizeof(continuar));
            continue;
        }
        if (strcmp(atac->cor, defe->cor) == 0) {
            printf("Nao e permitido atacar um territorio da mesma cor.\n");
            printf("Deseja tentar novamente? (s/n): ");
            lerLinha(continuar, sizeof(continuar));
            continue;
        }
        if (atac->tropas <= 0) {
            printf("Territorio atacante nao possui tropas suficientes para atacar.\n");
            printf("Deseja tentar novamente? (s/n): ");
            lerLinha(continuar, sizeof(continuar));
            continue;
        }

        /* Executa o ataque (modifica via ponteiros) */
        atacar(atac, defe);

        /* Mostra o estado apos ataque */
        exibirTerritorios(mapa, n);

        /* Verifica missões silenciosamente após cada turno (Nível Mestre) */
        if (verificarMissao(missaoJogador1, mapa, n, "azul")) {
            printf("\n>>> Jogador 1 (azul) cumpriu sua missao: %s <<<\n", missaoJogador1);
            vencedor = 1;
            break;
        }
        if (verificarMissao(missaoJogador2, mapa, n, "vermelha")) {
            printf("\n>>> Jogador 2 (vermelha) cumpriu sua missao: %s <<<\n", missaoJogador2);
            vencedor = 2;
            break;
        }

        /* Pergunta se deseja continuar jogando */
        printf("Deseja realizar outro ataque? (s/n): ");
        lerLinha(continuar, sizeof(continuar));
        if (continuar[0] == '\0') strcpy(continuar, "n");
    }

    /* Resultado final */
    if (vencedor == 1) printf("\n*** FIM DE JOGO: Jogador 1 (azul) venceu! ***\n");
    else if (vencedor == 2) printf("\n*** FIM DE JOGO: Jogador 2 (vermelha) venceu! ***\n");
    else printf("\nJogo encerrado sem vencedor.\n");

    /* Limpeza de memoria alocada dinamicamente */
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    printf("Obrigado por jogar! (Programa encerrando)\n");
    return 0;
}
