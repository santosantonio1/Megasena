/*
            EXAME TÉCNICO - DELL IT ACADEMY 

    Autor: Antônio Vinicius Corrêa dos Santos
    Github: @santosantonio1
    Versão: 21/03/2024

*/
#ifndef _INTERFACE_HPP
#define _INTERFACE_HPP

#include "Db.hpp"

// A classe interface é o intermediário entre a parte gráfica e o banco de dados construído para o problema
class Interface{
	public:
    
	Interface();

    // Envia para o banco de dados NOME, CPF e NÚMEROS da aposta para o cadastro.
    void registrar(string n, string c, vector<int> v); 

    // Se v = true, apenas as apostas vencedoras são listadas. Caso contrário,
    // todas as apostas são listadas. Na listagem todos os dados com respeito
    // a cada aposta são mostrados para o operador.
    void list(int beg, int end, bool v);

    // Sorteia um número não repetido.
    bool sortear(); 

    // Verifica se há algum vencedor.
    bool apurar();

    // Método responsável pela impressão dos números sorteados na tela.
    void sorteados();

    // Retorna o número de apostas cadastradas no banco de dados.
    int apostas(); 

    // Sinaliza para o banco de dados deletar seus dados.
    void clear();

    // Gera 5 números aleatórios em formato de string.
    string surpresinha();

    // Retorna o número de vencedores do sorteio.
    int vencedores();

    // Retorna o número de rodadas do sorteio.
    int rodadas();
    
    // Salva o banco de dados em um arquivo data.csv.
    void salvar(); 

    // Compartilha as estatísticas do sorteio com o operador, que são:
    //      - Quantas rodadas aconteceram.
    //      - Listagem de todos os números apostados e quantas apostas
    //      cada um recebeu.
    // Retorna um auxiliar para o design da página.
    int feedback(int beg, int end);

    // Método auxiliar para verificação de um nome ligado a um cpf.
    bool check(string n, string c);

    // Atualiza o número de rodadas.
    void addRodada();

    // Método para contar quantas apostas um apostador fez.
    int count(string c);

    // Lista os vencedores e seus respectivos prêmios.
	void premio();
    
    private:

    // Classe com os dados do sorteio.    
    Database db;

    // Gerador de números aleatórios para sortear os números.
    int random();
};

#endif