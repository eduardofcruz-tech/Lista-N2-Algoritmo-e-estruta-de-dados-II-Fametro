#include <stdio.h>
#include <stdlib.h>

// Recebe n e cria dinamicamente um vetor de n elementos
int* cria_vetor(int n) {
  int* vetor = (int*)malloc(n * sizeof(int));

  if (vetor == NULL) {
    printf("Erro: Falha na alocacao de memoria para o vetor.\n");
  }

  return vetor;  // Retorna o ponteiro para a área alocada
}

// Recebe o ponteiro do vetor e n, e imprime os elementos
void imprime_vetor(const int* vetor, int n) {
  if (vetor == NULL) {
    printf("Erro: O vetor e NULL.\n");
    return;
  }

  printf("\nElementos do vetor:\n");
  for (int i = 0; i < n; i++) {
    printf("Elemento[%d]: %d\n", i, vetor[i]);
  }
}

// Recebe um ponteiro para um vetor e libera esta área de memória
void libera_vetor(int* vetor) {
  if (vetor != NULL) {
    free(vetor);
    printf("\nMemoria do vetor liberada com sucesso.\n");
  } else {
    printf("\nAviso: Tentativa de liberar um ponteiro NULL.\n");
  }
}

int main() {
  int n;
  int* meu_vetor = NULL;

  // Lê o valor n
  printf("Digite o numero de elementos (n) para o vetor: ");
  if (scanf("%d", &n) != 1 || n <= 0) {
    printf(
        "Entrada invalida ou tamanho menor que 1. O programa sera "
        "encerrado.\n");
    return 1;
  }

  //Chama a função para criar o vetor
  meu_vetor = cria_vetor(n);

  if (meu_vetor == NULL) {
    return 1;
  }

  // Lê os n elementos do vetor
  printf("\nDigite os %d elementos do vetor:\n", n);
  for (int i = 0; i < n; i++) {
    printf("Elemento[%d]: ", i);
    if (scanf("%d", &meu_vetor[i]) != 1) {
      printf("Erro de leitura. Encerrando o programa.\n");
      libera_vetor(meu_vetor);
      return 1;
    }
  }

  // Chama a função de impressão
  imprime_vetor(meu_vetor, n);

  // Libera a memória alocada
  libera_vetor(meu_vetor);

  return 0;
}