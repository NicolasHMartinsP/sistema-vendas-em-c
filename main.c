#include <stdio.h> // biblioteca para usar funções basicas
 #include <string.h> // biblioteca para usar strcspn e getchar, para diminuir o cache no buffer e evitar erros
#include <math.h>    // biblioteca para calcular juros compostos com pow() 
#include <time.h>    // biblioteca para colocar a data no final da compra

// struct serve para armazenar nome e CPF
struct Cliente {
    char nome[40];
    char cpf[15];
};

int main() {
    struct Cliente cliente; 
    int opcao, quantidade;
    float subtotal = 0.0;
    // variável com preços dos 6 produtos
    float preco[6] = {109.90, 44.90, 69.90, 134.00, 99.90, 164.90};
    int clienteCadastrado = 0; //  para controle de cadastro
    float descontoCadastro = 0.0; // variável para aplicar desconto extra se cadastrado

    // Variavel para armazenar quantidade comprada de cada produto
    int qtdProdutos[6] = {0,0,0,0,0,0};

    // Loop principal e menu para seleção de produtos e quantidade
    do {
        printf("\n===== MENU DE PRODUTOS =====\n");
        // Exibe o menu dos produtos
        for (int i=0; i<6; i++) {
            printf("%d - Produto %d - R$ %.4f\n", i+1, i+1, preco[i]);
        }
        printf("7 - Finalizar compra\n");
        printf("8 - Cancelar compra\n");
        printf("Escolha um produto (1-8): ");
        scanf("%d", &opcao);

        //  switch para controlar opções do menu
        switch(opcao) {
            // Para produtos de 1 a 6
            case 1: case 2: case 3: case 4: case 5: case 6:
                printf("Quantidade: ");
                scanf("%d", &quantidade);
                if (quantidade > 0) {
                    // Calcula subtotal atual somando o preço multiplicado pela quantidade
                    subtotal += preco[opcao-1] * quantidade;
                    // Atualiza a variavel de quantidades para a nota depois de comprar
                    qtdProdutos[opcao-1] += quantidade;

                    // Exibe subtotal parcial após a compra deste produto
                    printf("Subtotal atual: R$ %.4f\n", subtotal);

                    // Pergunta se o cliente deseja continuar comprando
                    printf("Deseja continuar comprando? (1=Sim / 0=Não): ");
                    scanf("%d", &opcao);
                    if (opcao == 1)
                        opcao = 0; // volta para o menu de produtos
                    else
                        opcao = 7; // finaliza a compra
                }
                break;

            // Finalizar compra
            case 7:
                if (subtotal == 0) {
                    printf("Nenhum produto selecionado.\n");
                    opcao = 0; // força voltar para o menu se nada comprado
                }
                break;

            // caso queira cancelar a compra
            case 8:
                printf("Compra cancelada.\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
                opcao = 0; // se digitar uma opção errada, volta para o menu
                break;
        }

    } while (opcao != 7); // loop até o usuário finalizar

    // Pergunta se o cliente já é cadastrado no clube
    printf("\nVocê já possui cadastro no clube? (1=Sim / 0=Não): ");
    scanf("%d", &clienteCadastrado);

    if (!clienteCadastrado) {
        // Se não cadastrado, oferece opção de cadastrar
        printf("Deseja se cadastrar? (1=Sim / 0=Não): ");
        scanf("%d", &clienteCadastrado);

        if (clienteCadastrado == 1) {
            printf("Digite seu nome completo: ");
            getchar(); // limpar buffer do teclado antes de usar fgets para evitar erros
            fgets(cliente.nome, sizeof(cliente.nome), stdin);
            // Remove o '\n' que fgets deixa no final da string
            cliente.nome[strcspn(cliente.nome, "\n")] = 0;

            printf("Digite seu CPF (formato 000.000.000-00): ");
            fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
            cliente.cpf[strcspn(cliente.cpf, "\n")] = 0;

            printf("Cadastro realizado com sucesso!\n");
            // Aplica desconto de 5% para cliente cadastrado
            descontoCadastro = 0.05;
        }
    } else {
        // Se já cadastrado, aplica desconto direto
        descontoCadastro = 0.05;
    }

    // Pergunta forma de pagamento
    int formaPagamento, parcelas = 1;
    float totalFinal = subtotal;

    printf("\n===== FORMA DE PAGAMENTO =====\n");
    printf("1 - Pix (5%% de desconto)\n");
    printf("2 - Cartão de débito (5%% de desconto)\n");
    printf("3 - Cartão de crédito (2%% de juros compostos ao mês)\n");
    printf("Escolha uma forma de pagamento (1-3): ");
    scanf("%d", &formaPagamento);

    // Aplica desconto ou juros conforme forma de pagamento
    switch (formaPagamento) {
        case 1:
        case 2:
            // 5% de desconto para pix ou débito
            totalFinal = subtotal * 0.95;
            break;

        case 3:
            // Cartão de crédito - pergunta número de parcelas
            printf("Quantas parcelas? ");
            scanf("%d", &parcelas);
            if (parcelas < 1) parcelas = 1;
            // Aplica juros compostos: valor * (1 + 0.02)^parcelas
            totalFinal = subtotal * pow(1.02, parcelas);
            break;

        default:
            // Se opção inválida, considera débito com 5% de desconto
            printf("Opção inválida. Considerando pagamento em débito.\n");
            totalFinal = subtotal * 0.95;
            formaPagamento = 2;
            break;
    }

    // Aplica desconto adicional de 5% se cliente cadastrado
    if (clienteCadastrado) {
        printf("Desconto adicional de cliente cadastrado aplicado (5%%).\n");
        totalFinal *= 0.95;
    }

    // aqui é para imprimir a nota fiscal da compra
    printf("\n===== Nota Fiscal =====\n");
    printf("Produto\t\tQuantidade\tPreço Unit.\n");
    for (int i = 0; i < 6; i++) {
        if (qtdProdutos[i] > 0) {
            printf("Produto %d\t%d\t\tR$ %.4f\n", i+1, qtdProdutos[i], preco[i]);
        }
    }
    printf("-------------------------------\n");
    printf("Subtotal: R$ %.4f\n", subtotal);
    if (clienteCadastrado) printf("Desconto cadastro: 5%%\n");
    if (formaPagamento == 1 || formaPagamento == 2) {
        printf("Desconto forma de pagamento: 10%%\n");
    } else if (formaPagamento == 3) {
        printf("Juros forma de pagamento: 2%% a.m. compostos\n");
        printf("Parcelas: %d\n", parcelas);
    }
    printf("Valor total a pagar: R$ %.4f\n", totalFinal);

    // se o cliente escolheu credito
    if (formaPagamento == 3) {
        printf("Valor da parcela: R$ %.4f\n", totalFinal / parcelas);
    }

    // pega a data na hora da compra e coloca na nota 
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\nData e hora da compra: %02d/%02d/%04d %02d:%02d:%02d\n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

    printf("\nValeu por fortalecer o corre, cole na goma quando quiser.\n");

    return 0;
}
