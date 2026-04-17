// Desenvolvido por Alessandro Joao Brassanini
// Data: 19 de Abril de 2022

/*
Uso de matriz bidimensional (5x5) para armazenar o cenario.
Lacos de repeticao para imprimir o labirinto a cada rodada.
Condicionais para validar os movimentos e impedir atravessar paredes.
Leitura de teclado com scanf para capturar as direcoes.
Verificacao de vitoria quando o jogador chega na saida.

Regras:
P - posisao atual do jogador.
0 - caminho livre.
1 - parede (nao pode atravessar).
S - saida (objetivo do jogo).

*/


#include <stdio.h>   // Necessario para printf e scanf
#include <stdlib.h>  // Necessario para system (limpar tela em alguns sistemas)
#include <ctype.h>   // Necessario para toupper (converter tecla para maiuscula)
#include <windows.h>
#include <time.h>    // Necessario para srand e time
#include <conio.h>

#define N 10 // Tamanho da matriz do labirinto

void somMovimento(){
    Beep(800, 50);
}

void somVitoria() {
    Beep(523, 200);
    Beep(659, 200);
    Beep(784, 200);
    Beep(1046, 800);
}

void somParede() {
    Beep(250, 200);
}

void somComandoInvalido() {
    Beep(330, 200);
    Beep(220, 200);
}

void mudarCor(int cor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cor);
}

int existeCaminho(int labirinto[N][N], int x, int y, int visit[N][N]) {
    if (x < 0 || x >= N || y < 0 || y >= N) {
        return 0;
    }

    if (labirinto[x][y] == 1 || visit[x][y]) {
        return 0;
    }

    if (labirinto[x][y] == -1) {
        return 1;
    }

    visit[x][y] = 1;

    if (existeCaminho(labirinto, x - 1, y, visit)) return 1;
    if (existeCaminho(labirinto, x + 1, y, visit)) return 1;
    if (existeCaminho(labirinto, x, y - 1, visit)) return 1;
    if (existeCaminho(labirinto, x, y + 1, visit)) return 1;

    return 0;
}

void gerarLabirinto(int labirinto[N][N]) {
    int i, j;
    int visit[N][N];

    do {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                labirinto[i][j] = (rand() % 100 < 65) ? 1 : 0;
            }
        }

        labirinto[0][0] = 0;
        labirinto[N - 1][N - 1] = -1;

        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                visit[i][j] = 0;
            }
        }
    } while (!existeCaminho(labirinto, 0, 0, visit));
}

int main()
{
    // Matriz que representa o labirinto
    // 0 = caminho, 1 = parede, S = saida (marcada por -1 aqui)
    int labirinto[N][N];

    srand((unsigned)time(NULL));
    gerarLabirinto(labirinto);

    int x = 0, y = 0; // Posicao inicial do jogador (linha, coluna)
    char comando;     // Variavel para armazenar o comando do jogador
    int jogando = 1, contMovimento = 0, contBatidaParede = 0;  // Controle do loop principal do jogo


    // Loop principal do jogo
    while (jogando)
    {
        // Limpa a tela (funciona no Windows com "cls", em Linux/Mac com "clear")
        // Se estiver testando no Code::Blocks ou terminal, pode comentar a linha abaixo
        system("clear || cls");

        // Exibe o labirinto
        
        printf("Jogo do Labirinto 10x10\n");
        printf("Use W (cima), S (baixo), A (esquerda), D (direita)\n");
        printf("Objetivo: chegar na saida (O)\n\n");

        printf("Linha: %d | Coluna: %d\n", x, y);

        int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i == x && j == y)
                {
                    mudarCor(10); // Verde para o jogador
                    printf("@"); // Mostra jogador
                }
                else if (labirinto[i][j] == 1)
                {
                    mudarCor(12); // Vermelho para parede
                    printf("X "); // Mostra parede
                }
                else if (labirinto[i][j] == -1)
                {
                    mudarCor(14); // Amarelo para saida
                    printf("O "); // Mostra saida
                }
                else
                {
                    mudarCor(15); // Branco para caminho livre
                    printf(". "); // Mostra caminho livre
                }
            }
            printf("\n");
        }
        mudarCor(7); // Resto do texto em branco

        // Verifica se chegou a saida
        if (labirinto[x][y] == -1)
        {
            somVitoria();
            printf("\nParabens! Voce encontrou a saida!");
            printf("\nNumero de Movimentos: %d", contMovimento);
            printf("\nNumero de Batidas na Parede: %d", contBatidaParede);
            printf("\nPontuacao Final: %d\n\n", contMovimento - contBatidaParede);
            break; // Sai do jogo
        }

        // Solicita movimento do jogador
        printf("\nDigite seu movimento (W/A/S/D): ");
        comando = getch(); // Captura o comando sem precisar pressionar Enter
        comando = toupper(comando); // Converte para maiuscula para facilitar

        // Calcula nova posicao do jogador
        int novoX = x;
        int novoY = y;

        int comandoValido = 1;

        if (comando == 'W')
        {
            novoX--; // Move para cima
        }
        else if (comando == 'S')
        {
            novoX++; // Move para baixo
        }
        else if (comando == 'A')
        {
            novoY--; // Move para esquerda
        }
        else if (comando == 'D')
        {
            novoY++; // Move para direita
        }
        else
        {
            printf("Comando invalido!\n");
            somComandoInvalido();
            comandoValido = 0;
        }

        if (!comandoValido)
        {
            continue;
        }

        // Verifica se nova posicao e valida (dentro da matriz e nao e parede)
        if (novoX >= 0 && novoX < N && novoY >= 0 && novoY < N && labirinto[novoX][novoY] != 1)
        {
            somMovimento(); // Toca som de movimento
            contMovimento ++; // Incrementa contador de movimentos
            x = novoX;
            y = novoY; // Atualiza posicao do jogador
        }
        else
        {
            somParede();
            contBatidaParede ++;
            printf("Movimento invalido! Parede ou fora dos limites!\n");
        }
    }

    system("\npause"); // Pausa o console para ver a mensagem final
    return 0; // Fim do jogo
}
