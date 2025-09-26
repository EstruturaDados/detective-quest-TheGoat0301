#include <ctype.h> // Para tolower
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10 // Tamanho da tabela hash

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

typedef struct HashEntry {
  char pista[100];
  char suspeito[50];
  struct HashEntry *next;
} HashEntry;

HashEntry *hashTable[TABLE_SIZE];

// Função hash simples para strings
unsigned int hash(const char *str) {
  unsigned int h = 0;
  while (*str) {
    h += (unsigned char)*str++;
  }
  return h % TABLE_SIZE;
}

// Inicializa a tabela hash
void initHash() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashTable[i] = NULL;
  }
}

// Insere associação pista/suspeito na tabela hash
void inserirNaHash(const char *pista, const char *suspeito) {
  unsigned int index = hash(pista);
  HashEntry *novo = (HashEntry *)malloc(sizeof(HashEntry));
  if (!novo) {
    printf("Erro ao alocar memória para hash!\n");
    exit(1);
  }
  strcpy(novo->pista, pista);
  strcpy(novo->suspeito, suspeito);
  novo->next = hashTable[index];
  hashTable[index] = novo;
}

// Consulta o suspeito correspondente a uma pista
char *encontrarSuspeito(const char *pista) {
  unsigned int index = hash(pista);
  HashEntry *entry = hashTable[index];
  while (entry) {
    if (strcmp(entry->pista, pista) == 0) {
      return entry->suspeito;
    }
    entry = entry->next;
  }
  return NULL;
}

// Libera memória da tabela hash
void liberarHash() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    HashEntry *entry = hashTable[i];
    while (entry) {
      HashEntry *temp = entry;
      entry = entry->next;
      free(temp);
    }
  }
}

// Cria dinamicamente um cômodo
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

// Insere a pista coletada na árvore de pistas
PistaNode *inserirPista(PistaNode *raiz, const char *pista) {
  if (!raiz) {
    PistaNode *novoNode = (PistaNode *)malloc(sizeof(PistaNode));
    if (!novoNode) {
      printf("Erro ao alocar memória para pista!\n");
      exit(1);
    }
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

// Exibe pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
  if (!raiz)
    return;
  exibirPistas(raiz->esquerda);
  printf("- %s\n", raiz->pista);
  exibirPistas(raiz->direita);
}

// Conta quantas pistas apontam para um suspeito específico (recursivo na BST)
int contarPistasParaSuspeito(PistaNode *raiz, const char *acusado) {
  if (!raiz)
    return 0;
  int count = contarPistasParaSuspeito(raiz->esquerda, acusado);
  char *sus = encontrarSuspeito(raiz->pista);
  if (sus && strcmp(sus, acusado) == 0) {
    count++;
  }
  count += contarPistasParaSuspeito(raiz->direita, acusado);
  return count;
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

// Navega pela árvore e ativa o sistema de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
  if (!atual)
    return;

  // Exibe a sala atual e a pista, se houver
  printf("\nVocê está na sala: %s\n", atual->nome);
  if (strlen(atual->pista) > 0) {
    printf("Você encontrou uma pista: %s\n", atual->pista);
    *arvorePistas = inserirPista(*arvorePistas, atual->pista);
  }

  char escolha;
  while (1) {
    printf("Escolha o caminho: (e) Esquerda, (d) Direita, (s) Sair: ");
    if (scanf(" %c", &escolha) != 1) {
      printf("Erro na leitura. Tente novamente.\n");
      while (getchar() != '\n')
        ; // Limpa buffer em caso de erro
      continue;
    }
    while (getchar() != '\n')
      ; // Limpa buffer após leitura bem-sucedida

    escolha = tolower(escolha);

    if (escolha == 'e') {
      if (atual->esquerda) {
        explorarSalasComPistas(atual->esquerda, arvorePistas);
        // Após explorar a esquerda, volta para a sala atual
        printf("\nVocê voltou para a sala: %s\n", atual->nome);
      } else {
        printf("Caminho inexistente à esquerda!\n");
      }
    } else if (escolha == 'd') {
      if (atual->direita) {
        explorarSalasComPistas(atual->direita, arvorePistas);
        // Após explorar a direita, volta para a sala atual
        printf("\nVocê voltou para a sala: %s\n", atual->nome);
      } else {
        printf("Caminho inexistente à direita!\n");
      }
    } else if (escolha == 's') {
      printf("Saindo da exploração...\n");
      return;
    } else {
      printf("Opção inválida. Digite e, d ou s.\n");
    }
  }
}

// Conduz à fase de julgamento final
void verificarSuspeitoFinal(PistaNode *arvorePistas) {
  char acusado[50];
  printf("\n=== Todas as pistas coletadas em ordem alfabética ===\n");
  exibirPistas(arvorePistas);

  printf("\nAcuse um suspeito (ex: Mordomo, Jardineiro, Cozinheiro, Dona da "
         "Casa, Bibliotecario): ");
  scanf("%49s", acusado); // Limita para evitar overflow
  while (getchar() != '\n')
    ; // Limpa buffer

  int count = contarPistasParaSuspeito(arvorePistas, acusado);
  if (count >= 2) {
    printf(
        "Parabéns! Há %d pistas apontando para %s. O culpado foi descoberto!\n",
        count, acusado);
  } else {
    printf("Provas insuficientes! Apenas %d pista(s) apontam para %s. O "
           "mistério continua...\n",
           count, acusado);
  }
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

  // Inicializa e popula a tabela hash com associações fixas
  initHash();
  inserirNaHash("Mapa da mansão encontrado", "Dona da Casa");
  inserirNaHash("Pegadas suspeitas na cozinha", "Cozinheiro");
  inserirNaHash("Flor com manchas estranhas", "Jardineiro");
  inserirNaHash("Livro antigo sobre mistérios", "Bibliotecario");
  inserirNaHash("Diário do mordomo", "Mordomo");

  PistaNode *arvorePistas = NULL;

  printf("Bem-vindo ao Detective Quest! Explore a mansão e colete pistas.\n");
  explorarSalasComPistas(hall, &arvorePistas);

  verificarSuspeitoFinal(arvorePistas);

  // Liberação de memória
  liberarSalas(hall);
  liberarPistas(arvorePistas);
  liberarHash();

  return 0;
}