/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/

#include "..\include\Aposta.hpp"

Aposta::Aposta(string n, string c, int i) : nome(n), cpf(c), id(i) {}

// Para ordenação das apostas em ordem alfabética dos apostadores.
bool Aposta::operator<(Aposta &a)
{
    return (this->nome<a.nome);
}