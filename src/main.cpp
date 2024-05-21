/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/

#include "..\include\raylib.h"
#include "..\include\Interface.hpp"
#include<iostream>
#include<string>
#include<queue>
#include<algorithm>
#include<cmath>

using namespace std;

#define max_letras 20
#define max_apostas 5

// Width e Height da janela do programa.
const int ww = 500, wh = 400;
// const int ww = 1366, wh = 768;

// Cria um intermediário entra a parte interativa com o banco de dados.
Interface interface;

int num_apostas = 5;    

// Variável auxiliar que ao terminar de sortear os primeiros 5 números, estará em cinco e será utilizada
// para limitar o sorteio a 26 rodadas.
int rds = 0;

// v : guarda o valor booleano retornado pela apuração de apostas.
// SORTEOU: flag para indicar se os primeiros 5 números já foram sorteados ou não.
bool v = false, SORTEOU = false;

// Erros previstos.
bool ERRO_NAME = false;
bool ERRO_CPF = false;
bool ERRO_NUMBERS = false;

// Caixas de texto para a página de cadastro.
Rectangle box[6] = {
        {3*ww/8+1,wh/6+1,1.15*ww/2-2,30-2},
        {3*ww/8+1,2*wh/6+1,1.15*ww/2-2,30-2},
        
        
        {3*ww/8+1,3*wh/6+1,1*ww/2-2,30-2},
        
        
        {0.5*ww+1,0.8*wh+1,wh/3-2,50-2},
        {0.4*ww,0.4*wh,0.2*ww,0.2*wh},
        {3*ww/8+1+ww/2-2, 3*wh/6+1, 0.15*ww/2-2, 30-2}
};

// Indica em qual página o usuário está nas partes de listagem e feedback.
int pg = 1;

// Limite de dados por página.
int limite = 12; // 4
int limite_end = 6;
int limite_feedback = 9;
int limite_premio = 7;

// Estados possíveis (Telas).
enum SCREEN {MENU, REGISTER, START, LIST, SAVE, SORTEIO, APURACAO, CONFIRMACAO1, END1, END2, STATS, PREMIACAO};

// Auxílio para localização de onde o usuário está digitando.
enum BOX {NAME, CPF, NUMBERS, IDLE, SUBMIT};

// Tela atual.
SCREEN sc = START;

// Caixa de texto atual.
BOX bx = IDLE;

// Input buffers do usário.
vector<string> input(3);

// Vetor auxiliar para guardar as apostas no cadastro.
vector<int> apostas;

// Desenha o menu principal na tela.
void drawMenu(){
    DrawText("MENU", ww/3, 5, 40, DARKPURPLE);
    DrawText("(+) REGISTRAR APOSTA", 25, wh/6 + 10, 20, DARKPURPLE);
    DrawText("(L) LISTAR APOSTAS", 25, 2*wh/6 + 10, 20, DARKPURPLE);
    DrawText("(F) FINALIZAR E REALIZAR SORTEIO", 25, 3*wh/6 + 10, 20, DARKPURPLE);
    DrawText("(S) SALVAR", 25, 4*wh/6 + 10, 20, DARKPURPLE);
}

// Desenha a página de cadastro.
void drawReg(){
    DrawText(
        "NOME: ", 
        25, 
        wh/6, 
        30, 
        DARKPURPLE
    );
    DrawRectangleLines(
        3*ww/8, 
        wh/6, 
        1.15*ww/2, 
        30, 
        BLACK
    );
    DrawRectangleRec(box[0], WHITE);
    DrawText(
        input[0].c_str(), 
        box[0].x+5, 
        box[0].
        y+10, 
        15, 
        BLACK
    );
    
    DrawText(
        "CPF: ", 
        25, 
        2*wh/6, 
        30, 
        DARKPURPLE
    );
    DrawRectangleLines(
        3*ww/8, 
        2*wh/6, 
        1.15*ww/2, 
        30, 
        BLACK
    );
    DrawRectangleRec(box[1], WHITE);
    DrawText(
        input[1].c_str(), 
        box[1].x+5, 
        box[1].y+10, 
        15, 
        BLACK
    );

    DrawText(
        "NUMEROS: ", 
        25, 
        3*wh/6, 
        30, 
        DARKPURPLE
    );
    DrawRectangleLines(
        3*ww/8, 
        3*wh/6, 
        1*ww/2, 
        30, 
        BLACK
    );
    DrawRectangleRec(box[2], WHITE);
    DrawText(
        input[2].c_str(), 
        box[2].x+5, 
        box[2].y+10, 
        15, 
        BLACK
    );

    
    DrawRectangleLines(
        box[5].x-1, 
        box[5].y-1, 
        box[5].width+2, 
        box[5].height+2, 
        BLACK
    );
    DrawRectangleRec(box[5], GRAY);
    DrawText(
        "?", 
        box[5].x + 2*box[5].width/5, 
        box[5].y + box[5].height/4, 
        15, 
        WHITE
    );
    DrawRectangleLines(
        0.5*ww, 
        0.8*wh, 
        wh/3, 
        50, 
        BLACK
    );
    DrawRectangleRec(box[3], WHITE);

    DrawText(
        "SUBMIT", 
        0.55*ww, 
        0.8*wh+20, 
        20, 
        GREEN
    );

    DrawText(
        "VOLTAR (E)", 
        ww/7, 
        0.85*wh, 
        20, 
        DARKPURPLE
    );

    // Caso ocorra erros, uma caixa vermelha aparecerá em volta de qual dado não foi possível ser cadastrado.
    if(ERRO_NAME != false) DrawRectangleLines(15, wh/6-5, 1.15*ww/2 + 2.9*ww/8, 40, RED);
    if(ERRO_CPF != false) DrawRectangleLines(15,2*wh/6-5, 1.15*ww/2 + 2.9*ww/8, 40, RED);
    if(ERRO_NUMBERS != false) DrawRectangleLines(15,3*wh/6-5, 1.15*ww/2 + 2.9*ww/8, 40, RED);
}

// Verifica em qual caixa de texto o usuário quer digitar.
void CheckBox()
{
    if(CheckCollisionPointRec(GetMousePosition(), box[0]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bx = NAME;
    else if(CheckCollisionPointRec(GetMousePosition(), box[1]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bx = CPF;
    else if(CheckCollisionPointRec(GetMousePosition(), box[2]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bx = NUMBERS;
    else if(CheckCollisionPointRec(GetMousePosition(), box[3]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bx = SUBMIT;
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bx = IDLE;
}

// Verifica se o usuário está digitando.
bool type(int i)
{
    if(
        (bx == NAME && i == 0) ||
        (bx == CPF  && i == 1) || 
        (bx == NUMBERS && i == 2)
    ) return true;

    return false;
}

// Recebe o input do usuário, salva no respectivo input_buffer e escreve na janela.
void getInput(int i)
{   
    if(type(i))
    {   
        // Lê a tecla que o usuário pressionou.
        int key = GetCharPressed();

        if(key>=32 && key<=125 && input[i].length()<max_letras)
        {
            input[i] += (char) key;
        }
        // Apaga
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            if(input[i].empty() == false) input[i].pop_back();
        }
        // Muda a caixa de texto .
        if(IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_ENTER))
        {
            switch(bx)
            {
                case NAME:
                {
                    bx = CPF;
                    break;
                }
                case CPF:
                {
                    bx = NUMBERS;
                    break;
                }
                case IDLE:
                {
                    bx = SUBMIT;
                    break;
                }
                default:
                    bx = IDLE;
            }
        }
    }
}

// Converte a string recebida do usuário para um vetor de inteiros que representa os números escolhidos para apostar.
bool input_to_apostas()
{   
    // Remove os espaços em branco no final do input de números.
    while(input[2].back() == ' ')
    {
        input[2].pop_back();
    }

    // Caso o número de espaços seja diferente de 4, entende-se que o usuário não apostou 5 números.
    int x = count(input[2].begin(), input[2].end(), ' ');
    if(x != 4) return false;

    // Fila auxiliar para guardar os dígitos do número apostado.
    queue<int> q;
    char c;

    // Inteiro auxiliar para a conversão do input do usuário para um inteiro
    int num = 0;

    for(int i=0;i<input[2].length();i++)
    {
        c = input[2][i];

        if(c>='0' && c<='9') q.push(c-'0');
        else if(c != ' ') 
        {
            cout << "Dif espaço" << endl;
            return false;
        }
        // Se o número tiver mais que 2 dígitos, automaticamente está fora do intervalo [1,50] de apostas.
        if(q.size()>2){
            cout << "q.size() > 2" << endl;
            return false;
        }

        // Os espaços separam as apostas. Caso um espaço seja encontrado, adicione o número apostado ao vetor auxiliar(apostas).
        // Caso seja o último número, também adicionar ao vetor de apostas.
        if(i==input[2].length()-1 || c==' ')
        {   
            // Por conta da estrutura de dados escolhida, os dígitos que "saem" primeiro da fila são os mais significativos. 
            // A estratégia escolhida foi:
            // -- Pegar o dígito (q.front())
            // -- Salvar no inteiro auxiliar
            // -- Seguir para o próximo dígito (q.pop())
            while(q.empty() == false)
            {
                //  Sendo "num" definido como o número apostado pelo usuário e "di" os seus dígitos(i pode ser interpretado como a posição do dígito na fila), 
                // existem 2 casos previstos:
                // 1°) Entrou 1 número na fila => o número apostado é de 1 dígito => num = d0 * 10^(1-1) = d0
                // 2°) Entraram 2 números na fila => o número apostado é de 2 dígitos => num = d0 * 10^(2-1) + d1 * 10^(1-1) = 10*d0 + d1
                num += (pow(10,q.size()-1) * q.front());
                q.pop();
            }

            // Verificação se a aposta do usuário está contida no intervalo [1,50] do enunciado.
            if(num<1 || num >50)
            {
                return false;
            }
            else 
            {   
                apostas.push_back(num);
            }
            num = 0;
        }
    }
    return true;
}

// Verifica se algum erro previsto foi encontrado
bool validate()
{   
    // Caso o nome do apostador não seja dado, um erro é detectado.
    if(input[0].length() == 0 || interface.check(input[0], input[1]) == false) ERRO_NAME = true;
    else ERRO_NAME = false;

    // No Brasil, um CPF tem 11 dígitos e espera-se uma entrada apenas com os números do cpf (sem '.' e '-'). Caso não tenha
    // 11 dígitos, a caixa de texto será limpa.
    if(input[1].length() != 11)
    {
        input[1].clear();
        bx = CPF;
        ERRO_CPF = true;
    }
    else ERRO_CPF = false;

    // Limpa o vetor auxiliar antes da tentativa de conversão.
    apostas.clear();

    // Caso a aposta do usuário seja inválida, um erro é detectado e caixa de texto é limpa.
    if(input_to_apostas() == false)
    {
        input[2].clear();
        bx = NUMBERS;
        ERRO_NUMBERS = true;
    }
    else ERRO_NUMBERS = false;

    // Se não há erros, os dados serão cadastrados.
    if(ERRO_CPF == false && ERRO_NUMBERS == false && ERRO_NAME == false)
    {
        bx = IDLE;
        return true;
    }
    return false;
}

// Lista as apostas registradas.
void drawList()
{
    // Aqui fica melhor um exemplo:
    // Sendo o limite = 5, na página 1 serão listadas as apostas de 0 à 4, na 2 de 5 à 9. Ou seja,
    // na página n serão listadas as aposta de (pg-1) * limite à pg*limite. Note que esses índices
    // são baseados para a iteração em um vetor de n elementos com os índices de 0 à n-1.
    interface.list((pg-1)*limite, pg*limite, false);
    DrawText(
        "VOLTAR (E)", 
        wh/10, 
        0.93*wh, 
        20, 
        DARKPURPLE
    );
    DrawText(
        TextFormat("%d", pg),
        19*ww/20,
        0.93*wh,
        20,
        DARKPURPLE
    );
}

// Desenha a página de sorteio e apuração. Nesse estado o operador decide se deseja prosseguir com a fase de apuração ou não.
void drawConf1()
{
    interface.sorteados();

    // -------- Design da página.
    DrawText(
        "DESEJA PROSSEGUIR?",
        ww/10,
        wh/8,
        25,
        DARKPURPLE
    );

    // Botão de prosseguir com a apuração.
    Rectangle r = {0.4*ww + 50,wh/8 + 80,120,40};

    DrawRectangleRec(r, WHITE);
    DrawRectangleLines(
        r.x-1,
        r.y-1,
        r.width+2,
        r.height+2,
        BLACK
    );

    DrawText(
        "SIM",
        r.x+r.width/3,
        r.y+r.height/4,
        20,
        GREEN
    );

    DrawText(
        "VOLTAR (E)",
        ww/7,
        r.y+r.height/4,
        18,
        DARKPURPLE
    );

    DrawText(
        "NÚMEROS SORTEADOS:",
        ww/10,
        0.5*wh,
        20,
        DARKPURPLE
    );
    // --------------------------

    if(((CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {   
        // Se o número de rodadas do sorteio é menor que 25, pode-se sortear mais um número.
        rds++;
        interface.addRodada();

        if(interface.rodadas()< 26)
        {
            v = interface.apurar();

            // Caso existam apostas vencedoras, ir para o estado END1.
            if(v == true) sc = END1;
            else
            {
                // Sorteia um número até encontrar um que não seja repetido.
                while(interface.sortear() == false);
            }
        }
        // Caso não existem apostas vencedoras e já se passaram 26 rodadas, ir para o estado END2.
        else sc = END2;
    }
}

// Limpa o banco de dados e reinicializa as variáveis para o possível começo de uma nova edição.
void endEdition()
{   
    // Renicia os dados para uma nova edição.
    interface.clear();
    bx = IDLE;
    sc = START;
    rds = 0;
    v = false;
    SORTEOU = false;
    pg = 1;
}

// Desenha a página de vencedores.
void drawEnd1()
{   
    // Lista as apostas vencedoras em ordem alfabética dos seus donos.
    interface.list((pg-1)*limite_end, pg*limite_end, true);

    // -------- Design da página.
    DrawText(
        TextFormat("%d", pg),
        19*ww/20,
        0.93*wh,
        20,
        DARKPURPLE
    );
    DrawText(
        "ESTATISTICAS (S)", 
        wh/10, 
        0.93*wh, 
        20, 
        DARKBLUE
    );
    
    // Botão de finalizar edição.
    Rectangle r = {2.7*wh/4, 0.93*wh, ww/3, 22};

    DrawRectangleRec(r, WHITE);
    DrawRectangleLines(
        r.x-1, 
        r.y-1, 
        r.width+2, 
        r.height+2, 
        BLACK
    );
    DrawText(
        "PREMIADOS", 
        r.x + 2*r.width/8, 
        r.y + 6, 
        15, 
        BLACK
    );
    // --------------------------

    // Caso o operador clique no botão PREMIADOS, a lista dos vencedores com seus 
    // respectivos prêmios é mostrada.
    if(CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        sc = PREMIACAO;
    }
}

// Desenha a página do cenário em que não existam vencedores e já se passaram 26 rodadas.
void drawEnd2()
{
    // --------- Design da página
    DrawText(
        "SEM VENCEDORES...",
        ww/10,
        1.5*wh/8,
        25,
        RED
    );

    // -- Botão de finalizar edição.
    Rectangle r = {ww/2 + 50,wh/8 + 80,120,40};

    DrawRectangleRec(r, WHITE);
    DrawRectangleLines(
        r.x-1,
        r.y-1,
        r.width+2,
        r.height+2,
        BLACK
    );

    DrawText(
        "CONCLUIR",
        r.x+r.width*0.1,
        r.y+r.height/4,
        20,
        DARKPURPLE
    );
    // -----------------------------

    DrawText(
            "ESTATISTICAS(S)",
            ww/8,
            wh/8+wh/5+5,
            20,
            DARKBLUE
        );

    DrawText(
        "NÚMEROS SORTEADOS:",
        ww/10,
        // 4.5*wh/8,
        0.5*wh,
        20,
        DARKPURPLE
    );
    // --------------------------

    interface.sorteados();

    // Caso o operador clique no botão de finalizar, terminar o sorteio.
    if(CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        endEdition();
    }
}

// Desenha a página inicial do sorteio.
void drawStart()
{
    // Botão de iniciar sorteio.
    Rectangle r = {ww/5, wh/3, 3*ww/5, 35};
    DrawRectangleRec(r, WHITE);
    DrawRectangleLines(
        r.x-1, 
        r.y-1, 
        r.width+2, 
        r.height+2, 
        BLACK
    );
    
    DrawText(
        "INICIAR NOVA EDIÇÃO", 
        r.x + r.width/8, 
        r.y + 6, 
        20, 
        DARKPURPLE
    );

    // Caso o botão iniciar seja clicado, começar uma nova edição do sorteio.
    if((CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_KP_ENTER)|| IsKeyPressed(KEY_ENTER))
    {
        sc = MENU;
    }
}

// Desenha a página reponsável pela listagem dos vencedores e seus respectivos prêmios.
void drawPremiacao()
{   
    const int ww = 500, wh = 400;
    // DrawText("PREMIADOS", ww/3, wh/10, 25, DARKPURPLE);
    interface.premio();

    // Desenha o botão de finalizar.
    Rectangle r = {2.7*wh/4, 0.93*wh, ww/3, 22};
    DrawRectangleRec(r, WHITE);
    DrawRectangleLines(
        r.x-1, 
        r.y-1, 
        r.width+2, 
        r.height+2, 
        BLACK
    );
    DrawText(
        "FINALIZAR",
        r.x + 2*r.width/7, 
        r.y + 6, 15, 
        BLACK
    );
    
    // Seta para a esquerda(<-) : Volta uma página.
    if(IsKeyPressed(KEY_LEFT))
    {
        if(pg>1) pg--;
    }
    // Seta para a direita(->) : Avança 1 página.
    if(IsKeyPressed(KEY_RIGHT))
    {   
        // O máximo de páginas é dado pelo arredondamento de (número de vencedores)/(limite por página).
        if(pg<ceil((float)(interface.vencedores())/limite_premio)) pg++;
    }

    // Desenha o número da página.
    DrawText(
        TextFormat("%d", pg),
        19*ww/20,
        0.93*wh,
        20,
        DARKPURPLE
    );

    // Verifica se o usuário clicou para finalizar a edição do sorteio.
    if(CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        endEdition();
    }
}

// main...
int main()
{   
    // Cria uma janela.
    InitWindow(ww, wh, "Sorteador");

    while (!WindowShouldClose())
    { 

        if(sc == REGISTER)
        {   
            CheckBox();
            switch(bx)
            {
                // Se estiver na caixa de texto de nome, o usuário está digitando o nome.
                case NAME:
                {   
                    getInput(0);
                    break;
                }
                // Se estiver na caixa de cpf, o usuário está digitando o cpf.
                case CPF:
                {
                    getInput(1);
                    break;
                }
                // Se estiver na caixa de números, o usuário está digitando os números escolhidos para apostar.
                case NUMBERS:
                {
                    getInput(2);
                    break;    
                }
                // Envia os dados para validação.
                case SUBMIT:
                {   
                    if(CheckCollisionPointRec(GetMousePosition(), box[3]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        bx = SUBMIT;
                        
                        if(validate())
                        {   
                            // Caso os dados sejam válidos, cadastrar no banco de dados.
                            interface.registrar(input[0], input[1], apostas);

                            // Limpa os inputs digitados pelo operador.
                            for(int i=0;i<input.size();i++) input[i].clear();
                        }

                        // Apóos a validação, preparar para cadastrar outra pessoa começando pelo nome.
                        bx = NAME;
                        break;
                    }
                }
                case IDLE:
                {
                    // Não fazer nada. Nesse estado é possível voltar para o menu principal.
                }
            }
        }

        BeginDrawing();
            // Cor de fundo.
            ClearBackground(LIGHTGRAY);

            switch(sc)
            {   
                // Menu principal.
                case MENU:
                {
                    drawMenu();

                    // Caso os números não tenham sidos sorteados ainda e o usuário apertar a tecla "+", 
                    // ir para a página de cadastro.
                    if(IsKeyPressed(KEY_KP_ADD) && SORTEOU == false) 
                    {
                        sc = REGISTER;
                        bx = NAME;
                    }
                    // L: listar as apostas e seus dados.
                    if(IsKeyPressed(KEY_L)) sc = LIST;
                    // S: salvar os dados das apostas.
                    if(IsKeyPressed(KEY_S)) sc = SAVE;
                    // F: Finalizar etapa de cadastros e ir para as fases de sorteio e apuração. Depois de pressionado, novas
                    // apostas não são permitidas.
                    if(IsKeyPressed(KEY_F)) sc = CONFIRMACAO1;
                    break;
                }

                // Página de cadastro de apostas.
                case REGISTER:
                {
                    drawReg();
                    // E: Voltar para o menu principal.
                    if(IsKeyPressed(KEY_E) && bx == IDLE) 
                    { 
                        sc = MENU;
                        pg = 1;
                        bx = NAME;
                    }
                    // Caso clicado o botão cinza na com o símbolo de "?", 5 números são gerados aleatoriamente para
                    // o usuário.
                    if(CheckCollisionPointRec(GetMousePosition(), box[5]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {   
                        input[2] = interface.surpresinha();
                    }
                    break;
                }
                // Página de listagem de apostas.
                case LIST:
                {   
                    drawList();
                    // E: Volta para o menu principal.
                    if(IsKeyPressed(KEY_E)) 
                    {
                        sc = MENU;
                        pg = 1;
                    }
                    // Seta para a esquerda(<-) : Volta 1 página.
                    if(IsKeyPressed(KEY_LEFT))
                    {
                        if(pg>1) pg--;
                    }
                    // Seta para a direita(->) : Avança 1 página.
                    if(IsKeyPressed(KEY_RIGHT))
                    {   
                        // O máximo de páginas é dado pelo arredondamento de (número de apostas)/(limite por página).
                        if(pg<ceil((float)(interface.apostas())/limite)) pg++;
                    }
                    break;
                }
                // Estado de apuração e sorteio de novos números.
                case CONFIRMACAO1:
                {   
                    // Caso nenhum número tenha sido sorteado, sortear os 5 números iniciais.
                    if(SORTEOU == false)
                    {   
                        // Sortear 5 números distintos.
                        for(int i=0;i<num_apostas;i++)
                        {
                            if(interface.sortear() == false) i--;
                        }
                        SORTEOU = true;
                        rds++;
                    }
                    drawConf1();

                    // E: Voltar para o menu principal.
                    if(IsKeyPressed(KEY_E))
                    {
                        sc = MENU;
                    }

                    break;
                }
                // Cenário em que há pelo menos um vencedor.
                case END1:
                {
                    drawEnd1();
                    
                    DrawText(
                        "NÚMEROS SORTEADOS:",
                        ww/10,
                        4.5*wh/8,
                        20,
                        DARKPURPLE
                    );
                    interface.sorteados();

                    // (<-) : Volta uma página.
                    if(IsKeyPressed(KEY_LEFT))
                    {
                        if(pg>1) pg--;
                    }
                    // (->) : Avança uma página.
                    if(IsKeyPressed(KEY_RIGHT))
                    {   
                        // O máximo de páginas é o arredondamento para cima de (n° de vencedores)/(limite por página).
                        if(pg<ceil((float)(interface.vencedores())/limite_end)) pg++;
                    }
                    // S: Mostra as estatísticas do sorteio.
                    if(IsKeyPressed(KEY_S))
                    {
                        sc = STATS;
                        pg = 1;
                    }
                    break;
                }
                // Página das estatísticas do sorteio.
                case STATS:
                {   
                    // Auxiliar para a formatação da página.
                    int n_apostados = interface.feedback((pg-1)*limite_feedback, pg*limite_feedback);

                    DrawText(
                        TextFormat("%d", pg),
                        19*ww/20,
                        0.93*wh,
                        20,
                        DARKPURPLE
                    );

                    DrawText("VOLTAR (E)", wh/10, 0.93*wh, 20, DARKPURPLE);
                    DrawText(
                        TextFormat("VENCEDORES: %d", interface.vencedores()),
                        4*ww/10 + ww/15,
                        0.93*wh,
                        20,
                        DARKGREEN
                    );

                    // (<-): Volta a página.
                    if(IsKeyPressed(KEY_LEFT))
                    {
                        if(pg>1) pg--;
                    }
                    // (->): Avança a página. Novamente, o número de dados por página é o arredondamento para cima de
                    // (auxiliar)/(limite por página).
                    if(IsKeyPressed(KEY_RIGHT))
                    {
                        if(pg<ceil((float)(n_apostados)/limite_feedback)) pg++;
                    }
                    // E: Volta para a lista de vencedores ou para a página final do sorteio.
                    if(IsKeyPressed(KEY_E))
                    {
                        if(v == true) sc = END1;
                        else sc = END2;
                        pg = 1;
                    }
                    break;
                }
                // Cenário em que não há apostas vencedoras.
                case END2:
                {   
                    drawEnd2();
                    if(IsKeyPressed(KEY_S))
                    {
                        sc = STATS;
                    }
                    break;
                }
                // Página inicial.
                case START:
                {
                    drawStart();
                    break;
                }
                // Nesse estado nenhuma página é desenhada. Os dados apenas são salvos no arquivo ".csv".
                case SAVE:
                {
                    interface.salvar();
                    sc = MENU;
                    break;
                }
                // Estado em que são mostradas as premiações para os vencedores.
                case PREMIACAO:
                {
                    drawPremiacao();
                    break;
                }
                default:
                {
                    drawMenu();
                }
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
