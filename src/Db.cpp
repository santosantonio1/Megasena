/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/

#include "..\include\Db.hpp"
#include<fstream>
#include<algorithm>

// Inicializa o banco de dados.
Database::Database()
{
    n_apostados = 0;
    rd = 0;
    num_apostas = 0;
    fdbck.resize(51);

    // Inicialização do vetor com todos o numéros possíveis contidos no intervalo para a aposta com 0 escolhas.
    // Armazena-se 51 números, mas o 0 será sempre descartado nas operações.
    for(int i=0;i<51;i++)
    {
        fdbck[i].first = 0;
        fdbck[i].second = i;
    }
}

// Limpa o banco de dados.
Database::~Database()
{
    clear();
}

// Responsável pelo cadastro de uma aposta.
void Database::registrar(string n, string c, vector<int> v)
{
    // Cadastra uma aposta e seu ID é gerado automaticamente pela fórmula num_apostas + 1000.
    table.push_back(Aposta(n,c,num_apostas+1000));
    table[num_apostas].numeros = v;
    num_apostas++;

    if(nomes.find(c) == nomes.end())
    {
        nomes[c] = string(n);
    }

    // Atualização do número de vezes que um número foi apostado.
    for(int i=0;i<5;i++) fdbck[v[i]].first++;

}

// Adiciona uma aposta vencedora à lista de apostas vencedoras.
void Database::addVencedor(int id)
{
    v.push_back(table[id-1000]);
}

// Percorre linearmente o vetor de números sorteados para detectar se o novo número sorteado é repetido.
bool Database::contains(int n){
    for(auto i : numeros){
        if(i==n) return true;
    }
    return false;
}

// Limpa o banco de dados.
void Database::clear()
{
    table.clear();
    v.clear();
    numeros.clear();
    fdbck.clear();
    n_apostados = 0;
    rd = 0;
    num_apostas = 0;
    nomes.clear();
    
    fdbck.resize(51);
    for(int i=0;i<51;i++)
    {
        fdbck[i].first = 0;
        fdbck[i].second = i;
    }
}

// Salva os dados do banco de dados em um arquivo .csv
void Database::salvar()
{
    //O arquivo é salvo no formato: 
    //      id;cpf;nome;números
    ofstream out;
    out.open(".\\data\\data.csv", ios::out);
    if(!out.good()) return;
    for(auto a : table)
    {
        out << a.id << ";" << a.cpf << ";" << a.nome << ";" << a.numeros[0];
        for(int i=0;i<a.numeros.size();i++) out << " " << a.numeros[i];
        out << endl;
    }
    out.close();
}
