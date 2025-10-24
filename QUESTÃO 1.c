#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para a data (dia e mes)
typedef struct {
    int dia;
    int mes;
} Data;

// Definição da estrutura principal do Contato
typedef struct {
    char nome[40];
    char telefone[15];
    char celular[15];
    char email[45];
    Data aniversario;
} Contato;

// Definição do nó da lista encadeada (Elemento)
struct elemento {
    Contato info;
    struct elemento* prox; // Ponteiro para o próximo elemento
};
typedef struct elemento Elemento;

// Inicializa a lista como vazia (NULL)
Elemento* cria_agenda() { return NULL; }

// Insere um novo contato no final da lista
Elemento* insere_contato(Elemento* agenda, Contato novo) {
    // Aloca memória para o novo nó
    Elemento* novo_elemento = (Elemento*)malloc(sizeof(Elemento));
    if (novo_elemento == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return agenda;
    }
    novo_elemento->info = novo;
    novo_elemento->prox = NULL;

    // Caso 1: Lista vazia
    if (agenda == NULL) return novo_elemento;

    // Caso 2: Percorre a lista até o último elemento
    Elemento* atual = agenda;
    while (atual->prox != NULL) atual = atual->prox;

    // Liga o novo elemento no final
    atual->prox = novo_elemento;
    return agenda;
}

// Percorre a lista e imprime todos os contatos
void lista_contatos(Elemento* agenda) {
    if (agenda == NULL) {
        printf("Agenda vazia.\n");
        return;
    }

    Elemento* atual = agenda;
    int contador = 1;
    while (atual != NULL) {
        // Formato de impressão com preenchimento zero para dia/mes
        printf("\n--- Contato %d ---\n", contador++);
        printf(
            "Nome: %s\nTelefone: %s\nCelular: %s\nEmail: %s\nAniversario "
            "(dd/mm): %02d/%02d\n",
            atual->info.nome, atual->info.telefone, atual->info.celular,
            atual->info.email, atual->info.aniversario.dia,
            atual->info.aniversario.mes);
        atual = atual->prox;
    }
}

// Busca um contato pelo nome (retorna o ponteiro para o nó ou NULL)
Elemento* busca_contato(Elemento* agenda, char nome[]) {
    Elemento* atual = agenda;
    while (atual != NULL) {
        // Comparação de strings
        if (strcmp(atual->info.nome, nome) == 0) return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Remove um contato pelo nome
Elemento* remove_contato(Elemento* agenda, char nome[]) {
    Elemento* atual = agenda;
    Elemento* anterior = NULL;

    // Busca pelo contato e armazena o anterior
    while (atual != NULL && strcmp(atual->info.nome, nome) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Contato nao encontrado.\n");
        return agenda;
    }

    // Se anterior é NULL, o nó a ser removido é a cabeça da lista
    if (anterior == NULL)
        agenda = atual->prox;
    else
        anterior->prox = atual->prox; // Desvia o ponteiro

    free(atual); // Libera a memória do nó removido
    printf("Contato removido com sucesso.\n");
    return agenda;
}

// Permite a reentrada de dados para um contato existente
void atualiza_contato(Elemento* agenda, char nome[]) {
    Elemento* contato = busca_contato(agenda, nome);
    if (contato == NULL) {
        printf("Contato nao encontrado.\n");
        return;
    }

    printf("Contato encontrado. Digite os novos dados:\n");
    
    // Leitura e remoção do '\n' para strings (usando fgets e strcspn)
    printf("Nome: ");
    fgets(contato->info.nome, 41, stdin);
    contato->info.nome[strcspn(contato->info.nome, "\n")] = 0;
    
    // Repetição para os demais campos de string
    printf("Telefone: ");
    fgets(contato->info.telefone, 16, stdin);
    contato->info.telefone[strcspn(contato->info.telefone, "\n")] = 0;
    printf("Celular: ");
    fgets(contato->info.celular, 16, stdin);
    contato->info.celular[strcspn(contato->info.celular, "\n")] = 0;
    printf("Email: ");
    fgets(contato->info.email, 41, stdin);
    contato->info.email[strcspn(contato->info.email, "\n")] = 0;
    
    // Leitura de inteiros
    printf("Dia do aniversario: ");
    scanf("%d", &contato->info.aniversario.dia);
    printf("Mes do aniversario: ");
    scanf("%d", &contato->info.aniversario.mes);
    getchar(); // Limpa o buffer de entrada
    printf("Contato atualizado com sucesso.\n");
}

int main() {
    Elemento* agenda = cria_agenda();
    int opcao = 0;
    char nome[40];

    while (opcao != 6) {
        // Exibe o menu principal
        printf("         AGENDA TELEFONICA\n");
        printf("********************************\n");
        printf("1. Inserir Contato\n");
        printf("2. Listar Contatos\n");
        printf("3. Buscar Contato\n");
        printf("4. Editar Contato\n");
        printf("5. Remover Contato\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer
            opcao = 0;
            continue;
        }
        getchar(); // Limpa o '\n'

        if (opcao == 6) {
            printf("\nSaindo da Agenda. Adeus!\n");
            break;
        }

        switch (opcao) {
            case 1: {
                Contato novo;
                printf("\n--- Cadastro de Novo Contato ---\n");
                
                // Rotina de entrada de dados para novo contato
                printf("Nome: ");
                fgets(novo.nome, 41, stdin);
                novo.nome[strcspn(novo.nome, "\n")] = 0;
                printf("Telefone: ");
                fgets(novo.telefone, 16, stdin);
                novo.telefone[strcspn(novo.telefone, "\n")] = 0;
                printf("Celular: ");
                fgets(novo.celular, 16, stdin);
                novo.celular[strcspn(novo.celular, "\n")] = 0;
                printf("Email: ");
                fgets(novo.email, 41, stdin);
                novo.email[strcspn(novo.email, "\n")] = 0;
                printf("Dia do aniversario: ");
                scanf("%d", &novo.aniversario.dia);
                printf("Mes do aniversario (MM): ");
                scanf("%d", &novo.aniversario.mes);
                getchar();
                
                agenda = insere_contato(agenda, novo);
                printf("Contato '%s' cadastrado com sucesso.\n", novo.nome);
                break;
            }
            case 2:
                lista_contatos(agenda);
                break;
            case 3:
                printf("Digite o nome do contato a buscar: ");
                fgets(nome, 41, stdin);
                nome[strcspn(nome, "\n")] = 0;
                Elemento* encontrado = busca_contato(agenda, nome);
                // Exibe o resultado da busca
                if (encontrado) {
                    printf("\n--- Contato Encontrado ---\n");
                    printf(
                        "Nome: %s\nTelefone: %s\nCelular: %s\nEmail: %s\nAniversario "
                        "(dd/mm): %02d/%02d\n",
                        encontrado->info.nome, encontrado->info.telefone,
                        encontrado->info.celular, encontrado->info.email,
                        encontrado->info.aniversario.dia,
                        encontrado->info.aniversario.mes);
                } else
                    printf("Contato nao encontrado.\n");
                break;
            case 4:
                printf("Digite o nome do contato a editar: ");
                fgets(nome, 41, stdin);
                nome[strcspn(nome, "\n")] = 0;
                atualiza_contato(agenda, nome);
                break;
            case 5:
                printf("Digite o nome do contato a remover: ");
                fgets(nome, 41, stdin);
                nome[strcspn(nome, "\n")] = 0;
                agenda = remove_contato(agenda, nome);
                break;
            default:
                if (opcao != 0 && opcao != 6) {
                    printf("Opcao invalida. Por favor, escolha uma opcao entre 1 e 6.\n");
                }
        }
    }

    // Libera toda a memória da lista antes de finalizar o programa
    Elemento* atual = agenda;
    Elemento* proximo;
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    return 0;
}