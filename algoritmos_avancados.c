#include <ctype.h> // Para tolower
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
  char nome[50];
  char pista[100];
  struct Sala *esquerda;
  struct Sala *direita;
} Sala;

typedef struct PistaNode {
  char pista[100];
  struct PistaNode *esquerda;
  struct PistaNode *direita;
} PistaNode;

// Cria uma nova sala
Sala *criarSala(const char *nome, const char *pista) {
  Sala *novaSala = (Sala *)malloc(sizeof(Sala));
  if (!novaSala) {
    printf("Erro ao alocar memória!\n");
    exit(1);
  }
  strcpy(novaSala->nome, nome);
  strcpy(novaSala->pista, pista);
  novaSala->esquerda = NULL;
  novaSala->direita = NULL;
  return novaSala;
}

// Insere pista na árvore BST
PistaNode *inserirPista(PistaNode *raiz, const char *pista) {
  if (!raiz) {
    PistaNode *novoNode = (PistaNode *)malloc(sizeof(PistaNode));
    strcpy(novoNode->pista, pista);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    return novoNode;
  }
  if (strcmp(pista, raiz->pista) < 0)
    raiz->esquerda = inserirPista(raiz->esquerda, pista);
  else if (strcmp(pista, raiz->pista) > 0)
    raiz->direita = inserirPista(raiz->direita, pista);
  return raiz;
}

// Exibe pistas em ordem
void exibirPistas(PistaNode *raiz) {
  if (!raiz)
    return;
  exibirPistas(raiz->esquerda);
  printf("- %s\n", raiz->pista);
  exibirPistas(raiz->direita);
}

// Libera memória da árvore de pistas
void liberarPistas(PistaNode *raiz) {
  if (!raiz)
    return;
  liberarPistas(raiz->esquerda);
  liberarPistas(raiz->direita);
  free(raiz);
}

// Libera memória das salas de forma recursiva
void liberarSalas(Sala *raiz) {
  if (!raiz)
    return;
  liberarSalas(raiz->esquerda);
  liberarSalas(raiz->direita);
  free(raiz);
}

// Explora salas e coleta pistas
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
  if (!atual)
    return;

  printf("\nVocê está na sala: %s\n", atual->nome);

  if (strlen(atual->pista) > 0) {
    printf("Você encontrou uma pista: %s\n", atual->pista);
    *arvorePistas = inserirPista(*arvorePistas, atual->pista);
  }

  char escolha;

  do {
    printf("Escolha o caminho: (e) Esquerda, (d) Direita, (s) Sair: ");
    if (scanf(" %c", &escolha) != 1) {
      printf("Erro na leitura.\n");
      return;
    }
    while (getchar() != '\n')
      ; // Limpa buffer

    escolha = tolower(escolha);

    if (escolha == 'e') {
      if (atual->esquerda)
        explorarSalasComPistas(atual->esquerda, arvorePistas);
      else
        printf("Caminho inexistente à esquerda!\n");
    } else if (escolha == 'd') {
      if (atual->direita)
        explorarSalasComPistas(atual->direita, arvorePistas);
      else
        printf("Caminho inexistente à direita!\n");
    } else if (escolha == 's') {
      printf("Saindo da exploração...\n");
      return;
    } else {
      printf("Opção inválida. Digite e, d ou s.\n");
    }
  } while (1);
}

int main() {
  // Criação das salas
  Sala *hall = criarSala("Hall de Entrada", "Mapa da mansão encontrado");
  Sala *cozinha = criarSala("Cozinha", "Pegadas suspeitas na cozinha");
  Sala *salaEstar = criarSala("Sala de Estar", "");
  Sala *jardim = criarSala("Jardim", "Flor com manchas estranhas");
  Sala *biblioteca = criarSala("Biblioteca", "Livro antigo sobre mistérios");
  Sala *quarto = criarSala("Quarto", "Diário do mordomo");

  // Conexão das salas
  hall->esquerda = cozinha;
  hall->direita = salaEstar;
  cozinha->esquerda = jardim;
  salaEstar->esquerda = quarto;
  salaEstar->direita = biblioteca;

  PistaNode *arvorePistas = NULL;

  explorarSalasComPistas(hall, &arvorePistas);

  printf("\n=== Todas as pistas coletadas em ordem alfabética ===\n");
  exibirPistas(arvorePistas);

  liberarSalas(hall);
  liberarPistas(arvorePistas);

  return 0;
}
