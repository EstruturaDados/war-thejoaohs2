/*
---------------------------------------------------------------
Programa: Cadastro e Exibição de Territórios
Autor: [Joao.Paulo]
Data: [13.11.2025]
Descrição:
    Este programa demonstra o uso de estruturas (structs) em C
    para armazenar e manipular informações relacionadas a territórios.
    Cada território possui nome, cor do exército e número de tropas.
    O usuário pode cadastrar 5 territórios, e ao final, o sistema
    exibe os dados registrados.
---------------------------------------------------------------
*/

#include <stdio.h>   // Biblioteca padrão para entrada e saída (printf, scanf, fgets)
#include <string.h>  // Biblioteca para manipulação de strings (função fgets, etc.)

// -------------------------------------------------------------
// Definição da estrutura "Territorio"
// -------------------------------------------------------------
// Esta struct agrupa três informações relacionadas a um território:
// 1. nome      -> nome do território
// 2. cor       -> cor do exército
// 3. tropas    -> quantidade de tropas no território
// -------------------------------------------------------------
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // ---------------------------------------------------------
    // Declaração de variáveis
    // ---------------------------------------------------------
    struct Territorio territorios[5]; // Vetor para armazenar 5 territórios
    int i; // Variável de controle do laço

    // ---------------------------------------------------------
    // Entrada de dados: cadastro dos territórios
    // ---------------------------------------------------------
    printf("===== CADASTRO DE TERRITORIOS =====\n\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d\n", i + 1);

        // Limpa o buffer do teclado (necessário ao usar fgets após scanf)
        fflush(stdin);

        // Entrada do nome do território
        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove o '\n' do final

        // Entrada da cor do exército
        printf("Digite a cor do exercito: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0'; // Remove o '\n' do final

        // Entrada da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n"); // Quebra de linha para melhor visualização
    }

    // ---------------------------------------------------------
    // Exibição dos dados cadastrados
    // ---------------------------------------------------------
    printf("===== DADOS DOS TERRITORIOS CADASTRADOS =====\n\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------------------\n");
    }

    // ---------------------------------------------------------
    // Fim do programa
    // ---------------------------------------------------------
    printf("Cadastro concluido com sucesso!\n");

    return 0;
}