/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/


#ifndef _DB_HPP
#define _DB_HPP

#include<iostream>
#include<vector>
#include<map>

#include "Aposta.hpp"

using namespace std;

// Classe responsável pelo armazenamento de dados do sorteio.
class Database{
    public:

    Database();
    ~Database();

    // Cadastra uma nova aposta.
    void registrar(string n, string c, vector<int> v);

    // Adiciona uma aposta vencedora à lista de vencedores.
    void addVencedor(int id);


    // Verifica se o número sorteado não é repetido.
    bool contains(int i);

    // Salva o banco de dados no arquivo data.csv.
    void salvar();

    // Limpa o banco de dados.
    void clear();

    private:

    // Armazena as apostas e seus dados.
    vector<Aposta> table;

    // Armazena as apostas vencedoras.
    vector<Aposta> v;

    // Armazena os números sorteados.
    vector<int> numeros;
    
    // vector<int> fdbck;

    // Armazena cada número apostado e quantas vezes foi escolhido.
    vector<pair<int, int>> fdbck;

    // Variável auxiliar que armazena quantos números no intervalo [1,50] foram apostados.
    int n_apostados;

    // Número de rodadas do sorteio.
    int rd;

    // Numero de apostas do sorteio.
    int num_apostas;
    
    // Estrutura para checar a relação de nomes e cpf.
    map<string, string> nomes;

    friend class Interface;
};

#endif