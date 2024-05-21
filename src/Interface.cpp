/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/

#include "..\include\Interface.hpp"

#include<sstream>
#include<random>
#include<algorithm>
#include<queue>
#include "..\include\raylib.h"
#include<fstream>
#include<cmath>

Interface::Interface(){

}

// Envia informações para o banco de dados sobre uma aposta para cadastro. 
void Interface::registrar(string n, string c, vector<int> v){
    db.registrar(n, c, v);
}

// Se v = true, lista as apostas vencedoras e seus dados. Caso contrário, lista os dados de todas as apostas cadastradas.
// As variáveis "beg" e "end" são responsáveis divisão das apostadas listadas por página, pois cada página tem um limite.
// Por exemplo: Se o limite for 5, devemos ter (beg = 0, end = 5) para listar as 5 primeiras apostas na primeira página e 
// (beg = 6, end = 10 para a listagem da segunda.
void Interface::list(int beg, int end, bool v)
{   
    vector<Aposta> *ptr;

    if(v == true) ptr = &db.v;
    else ptr = &db.table;

    // Variáveis para auxiliar no design da página.
    const int ww = 500, wh = 400;
    int xo = 100, yo = 1.5*wh/10, passo = 25;

    // Título da coluna de IDs.
    DrawText("ID", ww/20, wh/30, 30, DARKPURPLE);

    // Escolhe o título da coluna dos números da aposta. Se estiver listando as apostas vencedoras, 
    // o título será "APOSTAS VENCEDORAS". Caso contrário, será "APOSTAS" (Com respeito aos números apostados).
    if(v == true) DrawText("APOSTAS\nVENCEDORAS", 3.9*ww/20, 1*wh/30, 18, DARKGREEN);
    else DrawText("APOSTAS", 4.3*ww/20, 1.3*wh/30, 20, DARKPURPLE);

    // Título da coluna de CPFs.
    DrawText("CPF", 10*ww/20, wh/30, 30, DARKPURPLE);

    // Título da coluna de NOMEs.
    DrawText("NOME", 15*ww/20, wh/30, 30, DARKPURPLE);

    // Desenha divisórias para design da página.
    DrawLine(0,1.35*wh/10, ww, 1.35*wh/10, BLACK);
    DrawLine(1.8*ww/10, 0, 1.8*ww/10, 1.35*wh/10, BLACK);
    DrawLine(4.5*ww/10, 0, 4.5*ww/10, 1.35*wh/10, BLACK);
    DrawLine(6.75*ww/10, 0, 6.75*ww/10, 1.35*wh/10, BLACK);

    // Percorre linearmente as apostas para a impressão.
    for(int i=beg;i<end;i++)
    {   
        // Caso i seja a última aposta, sair da função.

        if(i==db.num_apostas) return;

        if(i>=ptr->size()) return;

        // Imprime o ID da aposta.
        DrawText(
            TextFormat("%d", ptr->at(i).id),
            ww/20,
            yo,
            15,
            DARKPURPLE
        );

        // Imprime os NÚMEROS da aposta.
        for(int j=0;j<ptr->at(i).numeros.size();j++)
        {
            DrawText(
                TextFormat("%2d", ptr->at(i).numeros[j]),
                2*ww/10 + j*24,
                yo,
                15,
                BLACK
            );
        }

        // Imprime o CPF ligado à aposta.
        DrawText(
            TextFormat("%s", ptr->at(i).cpf.c_str()),
            4.75*ww/10,
            yo,
            15,
            BLACK
        );

        // Imprime o NOME do dono da aposta.
        DrawText(
            TextFormat("%s", ptr->at(i).nome.c_str()),
            7*ww/10,
            yo,
            15,
            BLACK
        );

        // Desenha divisórias para separar as apostas.
        DrawLine(0,yo + passo - 2, ww, yo + passo - 2, BLACK);
        DrawLine(1.8*ww/10, yo+passo-2, 1.8*ww/10, yo - 6, BLACK);
        DrawLine(4.5*ww/10, yo+passo-2, 4.5*ww/10, yo - 6, BLACK);
        DrawLine(6.75*ww/10, yo+passo-2, 6.75*ww/10, yo - 6, BLACK);

        yo += passo;
    }
}

// Gera um número aleatório no intervalo [1,50].
int Interface::random(){
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uid(1,50);
    return uid(rng);
}

// Sorteia um número, retornado falso caso seja repetido e true caso contrário.
bool Interface::sortear(){
    int x = random();
       if(db.contains(x)!=true){
        db.numeros.push_back(x);

        return true;
    }
    return false;
}

// Procura apostas vencedoras no banco de dados, retornado true se encontrar e false caso não existam vencedores.
bool Interface::apurar(){

    // A cada apuração completa-se uma rodada.

    // Vetor auxiliar que receberá os números de cada aposta e será ordenado.
    vector<int> aux;

    // Iteração por todas as apostas.
    for(auto a : db.table)
    {      
        aux = a.numeros;
        
        // Ordenação do vetor auxiliar com os números para aplicação da busca binária.
        sort(aux.begin(), aux.end());

        int acertos = 0;

        // Verifica quantos números cada aposta acertou do sorteio.
        for(int i=0;i<db.numeros.size();i++)
        {
            // Busca binária para ver se o número sorteado está nos números da aposta.
            if(binary_search(aux.begin(), aux.end(), db.numeros[i]) == true)
            {
                acertos++;
            }
        }   

        // Se 5 números estiverem corretos, a aposta é vencedora.
        if(acertos == 5)
        {
            // Registra que a aposta é vencedora no banco de dados.
            db.addVencedor(a.id);
        }
    }

    // Ordena as apostas em ordem alfabética dos seus respectivos donos.
    if(db.v.empty() == false) sort(db.v.begin(), db.v.end());

    // Se não tiverem apostas vencedoras, retorna falso e true caso tenha pelo menos 1 vencedor.
    return db.v.size() > 0;
}

// Imprime na página os números sorteados.
void Interface::sorteados()
{   
    // Caso q   ueira verificar o cenário em que alguem ganha, descomentar a linha abaixo.
    // db.numeros = {1,2,3,4,5,6,7,8,9,10};

    const int ww = 500, wh = 400;

    // Vetor para auxiliar no design da página. Será utilizado para desenhar quadrados no fundo dos números sorteados.
    vector<Rectangle> sq(30);
    
    // Cria os quadrados.
    for(int i=0;i<30;i++)
    {
        sq[i] = {0.1*ww+5, 0.7*wh-7, 30, 27};
    }

    int xo = ww*0.1, yo = 0.7*wh - 0.0625*wh, passo = 40;
    for(int i=0;i<db.numeros.size();i++)
    {   
        // Variável responsável para a escolha da linha em que o número será impresso. Estão limitados 10 números por linha.
        int j = i % 10;
        
        // ----- Desenho dos quadrados.
        DrawRectangleLines(
            sq[i].x + j*passo, 
            sq[i].y + (i/10)*passo - 0.0625*wh,
            sq[i].width,
            sq[i].height,
            BLACK
            );

        DrawRectangle(
            sq[i].x + j*passo +1,
            sq[i].y + (i/10) * passo + 1 - 0.0625*wh,
            sq[i].width - 2,
            sq[i].height - 2,
            WHITE
        );
        // ----------------------------

        // Impressão dos números sorteados.
        DrawText(
            TextFormat("%4d ", db.numeros[i]),
            xo,
            yo,
            15,
            DARKPURPLE
        );
        xo += passo;

        // Se não couberem mais números na linha, pular para linha de baixo.
        if(xo>=0.9*ww)
        {
            xo = ww*0.1;
            yo += passo;
        }
    }
}

// Retorna o número de apostas cadastradas no banco de dados.
int Interface::apostas()
{
    return db.num_apostas;
}

// Sinaliza para o banco de dados salvar os dados do sorteio em um arquivo .csv
void Interface::salvar()
{
   db.salvar();
}

// Imprime as estatísticas do sorteio na tela, utilizando a mesma ideia do método list() para limitar os dados por página.
// Retorna quantos números distintos foram escolhidos para auxiliar no design da página.
int Interface::feedback(int beg, int end)
{   
    // Variável que armazena a quantidade de números distintos foram escolhidos para aposta. Caso não tenha sido calculada
    // ainda, percorre-se linearmente o vetor, que é ordenado em ordem decrescente com respeito ao número de vezes que foi
    // escolhido.
    if(db.n_apostados == 0)
    {
        sort(db.fdbck.rbegin(), db.fdbck.rend());
        for(int i=0;i<db.fdbck.size();i++)
        {
            if(db.fdbck[i].first == 0) break;
            else db.n_apostados++;
        }  
    }

    const int ww = 500, wh = 400;
    int xo = ww/20, yo = wh/5 + 12, passo = 25;


    // Título: número apostado.
    DrawText("NÚMERO APOSTADO", xo, yo/2, 20, DARKPURPLE);

    // Título: quantidade de apostas que escolheram o número.
    DrawText("QTD. DE APOSTAS", 11*ww/20, yo/2, 20, DARKPURPLE);

    // Imprime na tela quantas rodadas aconteceram.
    DrawText(
        TextFormat("RODADAS: %d", db.rd),
        ww/3,
        10,
        25,
        DARKPURPLE
    );

    // Imprime os dados coletados.
    for(int i=beg;i<end && db.fdbck[i].first != 0;i++)
    {   
        // Número.
        DrawText(
            TextFormat("%d", db.fdbck[i].second),
            xo+2*ww/20,
            yo,
            20,
            BLACK
        );

        // Número de apostas que escolheram esse número.
        DrawText(
            TextFormat("%d", db.fdbck[i].first),
            ww/2 + 2*ww/20,
            yo,
            20,
            BLACK
        );
        yo += passo;
    }

    return db.n_apostados;
}

// Retorna o número de rodadas do sorteio.
int Interface::rodadas()
{
    return db.rd;
}

// Retorna quantas apostas vencedoras existem.
int Interface::vencedores()
{
    return db.v.size();
}

// Gera 5 números aleatórios caso o apostador deseje. Retorna uma string ao invés de um vetor de inteiros porque
// no arquivo main o cadastro é feito a partir de strings, e não apartir de strings e 1 vetor.
string Interface::surpresinha()
{   
    // Vetor para o armazenamento dos números gerados aleatoriamente.
    vector<int> v;
    string s;

    // Número aleatório gerado pelo método random.
    int x = random();
    v.push_back(x);

    for(int i=1;i<5;i++)
    {
        x = random();

        // Booleano para verificar se o número gerado não é repetido.
        bool tem = false;

        // Percorre linearmente o vetor para a verificação de repetição.
        for(int j=0;j<v.size();j++)
        {   
            // Repetição encontrada.
            if(v[j] == x)
            {
                tem = true;
                break;
            }
        }
        if(tem == true) i--;
        else v.push_back(x);
    }

    // Conversão de um vetor de inteiros para string.
    for(int i=0;i<5;i++)
    {   
        // Sendo n a quantidade de dígitos de um determinado número x, temos:
        // n = ceil(log10(x) + 1) para números que não são potências de 10 (x != 10^p, onde p é algum natural).
        // Para x = 1, temos que n = 0 e para 1 < n <= 10, temos que n = 1.
        // Para 10 < x <= 100 temos que n = 2.
        // Para o sorteio, temos x pertencendo ao intervalo I = [1,50]. Então é suficiente verificar se o número
        // de dígitos de x é menor que 2 e diferente de 10, pois terá 1 dígito. Caso contrário, só resta o número
        // ter 2 dígitos.

        if(ceil(log10(v[i])) < 2 && v[i] != 10)
        {
            s += (v[i] + '0');
        }
        else
        {
            s += (v[i]/10 + '0'); 
            s += (v[i]%10 + '0');
        }   
            s += ' '; 
    }

    s.pop_back();

    return s;
}

// Sinaliza para o banco de dados limpar suas informações.
void Interface::clear()
{   
    db.clear();
}

// Adiciona uma rodada.
void Interface::addRodada()
{
    db.rd++;
}

// Verifica se o nome condiz com o nome cadastrado no banco de dados.
bool Interface::check(string n, string c)
{   
    // Se o cpf não estiver cadastrado, sem problemas.
    if(db.nomes.find(c) == db.nomes.end())
    {
        return true;
    }

    // Caso esteja, verificar se os nomes são iguais.
    return db.nomes[c] == n;
}

// Percorre todas as apostas.
int Interface::count(string c)
{
    int x = 0;

    for(auto a : db.table)
    {
        if(a.cpf == c) x++;
    }
    return x;
}

void Interface::premio()
{
    const int ww = 500, wh = 400, passo = 30;
    int xo = ww/5, yo = 2*wh/10;
    int p = 1000, base = 50000;

    DrawText(
        "CPF", 
        ww/6, 
        wh/10, 
        20, 
        DARKPURPLE
    );

    DrawText(
        "PREMIO", 
        ww/2 + ww/8, 
        wh/10, 
        20, 
        DARKGREEN
    );

    // Horizontal
    DrawLine(
        ww/20-1,
        1.75*wh/10, 
        0.9*ww+1, 
        1.75*wh/10, 
        BLACK
    );
    DrawLine(
        ww/20-1, 
        30, 0.9*ww + 1, 
        30, 
        BLACK
    );

    // Vertical
    DrawLine(
        ww/2, 
        30, 
        ww/2, 
        2*wh/10, 
        BLACK
    );
    DrawLine(
        ww/20-1, 
        29, 
        ww/20-1, 
        1.75*wh/10, 
        BLACK
    );
    DrawLine(
        0.9*ww+1, 
        29, 
        0.9*ww+1, 
        1.75*wh/10, 
        BLACK
    );

    // Percorre o vetor de apostas para imprimir as informações.
    for(auto i : db.v)
    {
        DrawText(
            TextFormat("%s", i.cpf.c_str()),
            0.5*xo,
            yo,
            20, 
            BLACK
        );

        DrawText(
            TextFormat("%d", count(i.cpf) * p + base),
            ww/2 + ww/8,
            yo,
            20,
            BLACK
        );

        // Horizontal
        DrawLine(
            ww/20-1,
            yo + passo - 4, 
            0.9*ww+1, 
            yo + passo-4, 
            BLACK
        );
        
        // Vertical
        DrawLine(
            ww/2, 
            yo+passo-4, 
            ww/2, 
            yo - 10, 
            BLACK
        );
        DrawLine(
            ww/20-1, 
            yo+passo-4, 
            ww/20-1, 
            yo - 20, 
            BLACK
        );
        DrawLine(
            0.9*ww+1, 
            yo+passo-4, 
            0.9*ww+1, 
            yo - 10, 
            BLACK
        );
        yo += passo;
    }
}