/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/
#ifndef _APOSTA_HPP
#define _APOSTA_HPP

#include<iostream>
#include<vector>

using namespace std;

class Aposta{
    public:
    
    Aposta(string n="", string c="", int i=0);

    bool operator<(Aposta &a);

    private:

    // ID da aposta que é gerado automaticamente pelo banco de dados.
    int id;

    // CPF e NOME do apostador.
    string cpf, nome;

    // Numeros da aposta.
    vector<int> numeros;

    friend class Database;
    friend class Interface;
};

#endif