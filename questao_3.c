#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicao da estrutura de um no da pilha (lista ligada)
typedef struct Node {
    double value;  // Valor double armazenado no no (operando)
    struct Node* next; // Ponteiro para o proximo no (abaixo)
} Node;

// Definicao da estrutura de controle da pilha
typedef struct Stack {
    Node* top; // Ponteiro para o elemento superior da pilha
} Stack;

// Prototipos das funcoes
Stack* createStack();
void push(Stack* stack, double value);
double pop(Stack* stack);
int isEmpty(Stack* stack);
void freeStack(Stack* stack);
double evaluateRPN(char* expression);
const char* get_base_name(const char* path);

// Variavel global para sinalizar e propagar erros para a funcao main
static int global_error_code = 0;

// 2. Funcoes da Pilha (LIFO - Last In, First Out)
// ===============================================================

// Aloca e inicializa uma nova pilha vazia.
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Erro de alocacao de memoria para a pilha");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL; // Topo nulo indica pilha vazia
    return stack;
}

// Verifica se a pilha esta vazia.
int isEmpty(Stack* stack) { return (stack == NULL || stack->top == NULL); }

// Adiciona um novo valor ao topo da pilha.
void push(Stack* stack, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro de alocacao de memoria para o No");
        freeStack(stack);
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->next = stack->top; // Novo no aponta para o antigo topo
    stack->top = newNode;      // O novo no se torna o topo
}

// Remove e retorna o valor do topo da pilha.
double pop(Stack* stack) {
    if (isEmpty(stack)) {
        global_error_code = -1; // Sinaliza erro de pilha vazia
        return 0.0;
    }

    Node* temp = stack->top;
    double poppedValue = temp->value;
    stack->top = temp->next; // Atualiza o topo para o proximo no
    free(temp);              // Libera o no removido

    return poppedValue;
}

// Libera toda a memoria alocada para a pilha (todos os nos e a estrutura Stack).
void freeStack(Stack* stack) {
    if (stack == NULL) return;

    Node* current = stack->top;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(stack);
}

// 3. Funcoes Utilitarias e Avaliador RPN
// ===============================================================

// Extrai o nome do executavel de um caminho completo (multiplataforma).
const char* get_base_name(const char* path) {
    const char* base = path;
    const char* p;

    // Procura o ultimo separador de diretorio ('/' ou '\')
    for (p = path; *p; p++) {
        if (*p == '/' || *p == '\\') {
            base = p + 1;
        }
    }
    return base;
}

// Avalia a expressao RPN
double evaluateRPN(char* expression) {
    Stack* s = createStack();
    // Cria uma copia da string, pois strtok a modifica
    char* str_copy = strdup(expression);
    char* token;
    double val1, val2, result;
    int operand_count = 0; // Contador para checagem de malformacao final

    if (str_copy == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para copia da string.\n");
        freeStack(s);
        global_error_code = -2;
        return 0.0;
    }

    // Processa a expressao token por token (delimitador: espaco)
    token = strtok(str_copy, " ");

    while (token != NULL) {
        // Verifica se e um operador de 1 caractere (+, -, *, /)
        if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            
            // Pop do segundo operando (val2)
            val2 = pop(s);
            if (global_error_code != 0) {
                fprintf(stderr,
                        "Erro: Expressao malformada (operandos insuficientes para o "
                        "operador '%c').\n",
                        token[0]);
                global_error_code = -3;
                break;
            }

            // Pop do primeiro operando (val1)
            val1 = pop(s);
            if (global_error_code != 0) {
                fprintf(stderr,
                        "Erro: Expressao malformada (operandos insuficientes para o "
                        "operador '%c').\n",
                        token[0]);
                global_error_code = -3;
                break;
            }

            operand_count -= 1; // Dois pops, um push (resultado) = saldo -1

            // Realiza a operacao
            switch (token[0]) {
                case '+':
                    result = val1 + val2;
                    break;
                case '-':
                    result = val1 - val2;
                    break;
                case '*':
                    result = val1 * val2;
                    break;
                case '/':
                    if (val2 == 0.0) {
                        fprintf(stderr, "Erro: Tentativa de divisao por zero.\n");
                        global_error_code = -4;
                        break;
                    }
                    result = val1 / val2;
                    break;
            }

            if (global_error_code != 0) break;

            push(s, result); // Empilha o resultado
        } else {
            // Token e um operando (numero)
            char* endptr;
            // Tenta converter a string para double (strtod e robusto)
            result = strtod(token, &endptr);

            // Checa se a conversao falhou ou se ha caracteres invalidos
            if (endptr == token || *endptr != '\0') {
                fprintf(stderr, "Erro: Token invalido ou expressao malformada: '%s'.\n",
                        token);
                global_error_code = -5;
                break;
            }

            push(s, result);
            operand_count++;
        }

        // Proximo token
        token = strtok(NULL, " ");
    }

    double final_result = 0.0;

    // Trata erros que ocorreram durante o loop
    if (global_error_code != 0) {
        free(str_copy);
        freeStack(s);
        return 0.0;
    }

    // A expressao bem formada deve deixar exatamente 1 valor na pilha.
    if (operand_count == 1 && !isEmpty(s)) {
        final_result = pop(s);
        // Garante que nao sobrou mais nada na pilha (excessao: mais operandos que operadores)
        if (!isEmpty(s)) {
            fprintf(stderr,
                    "Erro: Expressao malformada (operandos demais/operadores de "
                    "menos).\n");
            global_error_code = -3;
        }
    } else {
        // Expressao vazia ou malformada (ex: operador demais ou operandos demais)
        fprintf(
            stderr,
            "Erro: Expressao malformada (operadores de menos/operandos demais).\n");
        global_error_code = -3;
    }

    free(str_copy);
    freeStack(s);

    // Retorna o resultado ou 0.0 em caso de erro final
    return (global_error_code == 0) ? final_result : 0.0;
}

// 4. Programa Principal (main)
// ===============================================================

int main(int argc, char* argv[]) {
    // Exibe a mensagem de ajuda se o argumento RPN nao for fornecido
    if (argc != 2) {
        const char* program_name = get_base_name(argv[0]);

        printf("\n****************************************\n");
        fprintf(stderr, "      Copie e cole no terminal os exemplos:\n");
        printf("****************************************\n");
        fprintf(stderr, "1 ./%s \"10 5 +\"\n", program_name);
        fprintf(stderr, "2 ./%s \"5 1 2 + 4 * + 3 -\"\n", program_name);
        fprintf(stderr, "3 ./%s \"10 4 2 / +\"\n", program_name);
        fprintf(stderr, "4 ./%s \"10 5 + *\"\n", program_name);
        fprintf(stderr, "5 ./%s \"10 0 /\"\n", program_name);

        return EXIT_FAILURE;
    }

    char* expression = argv[1];
    global_error_code = 0;

    // Chama a funcao principal de avaliacao
    double result = evaluateRPN(expression);

    // Trata o resultado e o codigo de erro
    if (global_error_code == 0) {
        printf("Resultado: %.2f\n", result);
        return EXIT_SUCCESS;
    } else {
        // A mensagem de erro especifica ja foi impressa por evaluateRPN
        return EXIT_FAILURE;
    }
}