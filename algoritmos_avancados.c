#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura de uma sala
typedef struct Sala {
  char nome[50];
  struct Sala *esquerda;
  struct Sala *direita;
} Sala;

/**
 * criarSala
 * Cria uma sala com nome fornecido e ponteiros para esquerda/direita nulos.
 */
Sala *criarSala(const char *nome) {
  Sala *novaSala = (Sala *)malloc(sizeof(Sala));
  if (novaSala == NULL) {
    printf("Erro ao alocar memória!\n");
    exit(1);
  }
  strcpy(novaSala->nome, nome);
  novaSala->esquerda = NULL;
  novaSala->direita = NULL;
  return novaSala;
}

/**
 * explorarSalas
 * Permite ao jogador navegar pela mansão a partir da sala atual.
 */
void explorarSalas(Sala *atual) {
  if (atual == NULL)
    return;

  printf("\nVocê está na sala: %s\n", atual->nome);

  // Se a sala não possui filhos, é um nó-folha
  if (atual->esquerda == NULL && atual->direita == NULL) {
    printf("Esta sala não possui mais caminhos. Fim da exploração!\n");
    return;
  }

  char escolha;
  do {
    printf("Escolha o caminho: (e) Esquerda, (d) Direita, (s) Sair: ");
    scanf(" %c", &escolha);

    if (escolha == 'e' && atual->esquerda != NULL) {
      explorarSalas(atual->esquerda);
      break;
    } else if (escolha == 'd' && atual->direita != NULL) {
      explorarSalas(atual->direita);
      break;
    } else if (escolha == 's') {
      printf("Saindo da exploração...\n");
      break;
    } else {
      printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
    }
  } while (1);
}

int main() {
  // Montagem do mapa da mansão
  Sala *hall = criarSala("Hall de Entrada");
  Sala *cozinha = criarSala("Cozinha");
  Sala *salaEstar = criarSala("Sala de Estar");
  Sala *jardim = criarSala("Jardim");
  Sala *biblioteca = criarSala("Biblioteca");
  Sala *quarto = criarSala("Quarto");

  // Definindo caminhos (árvore binária)
  hall->esquerda = cozinha;
  hall->direita = salaEstar;
  cozinha->esquerda = jardim;
  salaEstar->direita = biblioteca;
  salaEstar->esquerda = quarto;

  // Início da exploração
  explorarSalas(hall);

  // Liberação de memória
  free(jardim);
  free(cozinha);
  free(biblioteca);
  free(quarto);
  free(salaEstar);
  free(hall);

  return 0;
}
