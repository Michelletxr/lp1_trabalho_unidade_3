/**
 * @file marketplace.cpp
 * @author Isaac Franco (isaacfranco@imd.ufrn.br)
 * @version 0.1
 * @date 2022-01-27
 * 
 * @brief Marketplace em C++
 * 
 */
#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "utils.h"
#include <random>
#include <ctime>

using namespace std;

class Usuario {
    public:
    int id; // número incremental
    string email;
    string nome;
    string senha_hash; // Senha em hash
};

class Produto {
    public:
    int id; // número incremental
    string nome;
    float preco;
    int quantidade;
};

class Loja {
    public:
    int id; // número incremental
    string nome;
    Usuario proprietario;
    vector<Produto> produtos;
};

class Venda {
    public:
    int id; // número incremental
    int comprador_id; // Id do Usuário que fez a compra
    int loja_id; // Id da Loja que o produto foi comprado
    int produto_id; // Id do produto comprado
    int quantidade; // Quantos produtos foram comprados
    float preco_unitario; // Qual era o preço do produto no momento da venda
};

template<typename T>

class Marketplace {
    private:
    map<string, Usuario> usuarios; // Chave: email, Valor: Usuario
    map<string, int> acessos_liberados; // Chave: token_de_acesso, Valor: id_do_usuario
    map<int, Loja> lojas; // Chave: id da loja, Valor: Loja
    vector<Venda> vendas;
    vector<int> id_list;

    Usuario* verificaToken(string token){

        auto user_encontrado =  acessos_liberados.find(token);
    
       
       Usuario* user{nullptr};
        

        if (user_encontrado!=acessos_liberados.end())
        {
           
           
            for (auto u : this->usuarios)
            {

                if(u.second.id == user_encontrado->second)
                {
                  
                   user = new Usuario;
                    user->email = u.second.email;
                    user->id = u.second.id;
                    user->nome = u.second.nome;
                    break;
                     
                 }
            }
            
        }


        return user;
        
    }


    bool verificaLoja(string nome_)
    {
        for (auto l : this->lojas)
        {
            if(l.second.nome == nome_){ return true;} //retorna verdadeiro se achar loja com nome
        }
        
        return false; //retorna falso se não achar loja com nome
    }

    bool verificaProprietario(Usuario proprietario, Loja loja){ return proprietario.id == loja.proprietario.id; }

    Produto* buscar_produto_id(int id_produto){

        Produto* produto_encontrado{nullptr};

        for (auto loja : this->lojas)
        {
            for (auto produto : loja.second.produtos)
            {
                if(produto.id==id_produto)
                {
                    produto_encontrado = &produto;
                    return produto_encontrado;
                }
            } 
        }
        return produto_encontrado;
    }

    Loja* buscar_loja_idProduto(int id_produto)
    {
        

        Loja* loja_encontrada{nullptr};

        for (auto loja : this->lojas)
        {
            for (auto produto : loja.second.produtos)
            {
                if(produto.id==id_produto)
                {  
                    loja_encontrada = new Loja;
                    loja_encontrada->id = loja.second.id;
                    loja_encontrada->nome = loja.second.nome;
                    loja_encontrada->produtos = loja.second.produtos;
                    loja_encontrada->proprietario = loja.second.proprietario;
                    return loja_encontrada;
                }
            } 
        }
        return loja_encontrada;
    }


    int gerarId(){
         
        unsigned long int id{0};
        int max = this->usuarios.size()*10;
        std::srand(std::time(nullptr));
        std::vector<int>::iterator it;

        do
        {
            max+=1;
            id = rand() % max;
            it = std::find(id_list.begin(),id_list.end(), id);

        }while(it!=id_list.end());

        id_list.push_back(id);
        return id;
    }

    void atualiza_estoque(int loja_id, int remove_estoque, int produto_id) {
            
        auto loja = this->lojas.find(loja_id);
        vector<Produto>* produto = &loja->second.produtos;
            auto pos{produto->begin()};
            for(int i =0; i<produto->size(); i++)
            {
                pos++;
                if(produto->at(i).id==produto_id)
                {
                    auto item = produto->at(i);
                    produto->erase(pos);
                    item.quantidade-= remove_estoque;
                    produto->push_back(item);
                }
                    
            }    
        }
        


  

    // TODO Separa a implementação em .h e .cpp do Marketplace
    public:

        //construtor
        Marketplace(){}

        //destrutor
        ~Marketplace(){}

        /**
         * Cadastra um usuário no marketplace, retornando true ou false se o cadastro foi realizado com sucesso.
         * O e-mail deve ser único
         * @param nome Nome do usuário
         * @param email Email do usuário
         * @param senha Senha do usuário. Deve ser armazenada em forma criptografada.
         * @return True se o cadastro foi realizado com sucesso, false caso contrário.
         */
        bool me_cadastrar(string nome, string email, string senha) {
            // TODO(opcional) Implementar
            // Buscando usuário com e-mail no cadastro
            auto it = usuarios.find(email);
            // Se não existir, cria um novo usuário
            if (it == usuarios.end()) {
                Usuario novo_usuario;
                novo_usuario.id = this->gerarId(); //podemos fazer assim pois não existe remoção
                novo_usuario.email = email;
                novo_usuario.nome = nome;
                novo_usuario.senha_hash = geraHash(senha);
                usuarios.insert(make_pair(email, novo_usuario));
                return true;
            }
            return false;
        }

        /**
         * Tenta logar o usuário com esse e-mail / senha.
         * Caso bem sucessido o login, deve gerar aleatoriamente um token de acesso
         * e o par <token, usuario_id> deve ser armazenado em "acessos_liberados".
         * @param email Email do usuário
         * @param senha Senha do usuário.
         * @return  token de acesso caso o login seja bem sucedido. Caso contrário, retornar "invalid"
         */
        string login(string email, string senha) {
            // TODO(opcional) Implementar
            // Buscando usuário com e-mail no cadastro
            auto it = usuarios.find(email);
            // Se não existir, retorna "invalid"
            if (it == usuarios.end()) {
                return "invalid";
            }
            // Se existir, verifica se a senha está correta
            string senha_hash = geraHash(senha);
            if (it->second.senha_hash == senha_hash) {
                // Se estiver correta, gera um token de acesso
                string token_de_acesso = genRandomString(32);
                // Armazena o token de acesso e o id do usuário
                acessos_liberados.insert(make_pair(token_de_acesso, it->second.id));
                return token_de_acesso;
            }
            return "invalid";
        }

        /**
         * Cria uma loja no marketplace com o nome especificado para o usuário que tem
         * um acesso com esse token.
         * @param token Token de acesso
         * @param nome Nome da loja
         * @return O id da loja, ou -1 caso o token não exista em acessos_liberados ou
         * uma loja com esse nome já exista no marketplace
         */
        int criar_loja(string token, string nome) {

            //verifica token de acesso
            auto userValido = this->verificaToken(token);

            if (userValido != nullptr)
            {
               if(!verificaLoja(nome)) //verifica se já existe loja com esse nome
               {
                   Loja novaLoja;
                   novaLoja.id = this->gerarId();
                   novaLoja.nome = nome;
                   novaLoja.proprietario = *userValido;
                   lojas.insert(make_pair(novaLoja.id, novaLoja));
                   delete userValido; //libera o espaço de memoria alocado
                   
                   cout<< "Loja " << novaLoja.nome << " foi criada com sucesso! \n";
                   return novaLoja.id;
               }
            }

            //se token for valido e retornar o usuario, então cria a loja
            return -1;
        }

        /**
         * Adicionando produtos em uma loja(pelo id) de um usuário(pelo token).
         * Não é permitido adicionar um produto em um loja caso seu proprietário não seja o usuário do token passado
         * A quantidade de um produto inserido é 0 (zero)
         * 
         * @return Um id do produto adicionado para ser usado em outras operações
         */
        int adicionar_produto(string token, int loja_id, string nome, float preco) {
            
            auto userValido = this->verificaToken(token);
            auto loja = this->lojas.find(loja_id);
            if(loja==this->lojas.end()){return -1;}
            if(verificaProprietario(*userValido, loja->second))
            {
                Produto newProduto;
                newProduto.id = this->gerarId(); //gerar ids
                newProduto.nome = nome;
                newProduto.preco = preco;
                newProduto.quantidade=0;
                loja->second.produtos.push_back(newProduto);

                delete userValido; //libera a memoria

                cout<< "Produto " << newProduto.nome << " foi criado com sucesso! \n";
                return newProduto.id;
            }
            return -1;
        }

        /**
         * Adiciona uma quantidade em um produto em uma loja(pelo id) de um usuário(pelo token).
         * 
         * @param token Token de acesso
         * @param loja_id Id da loja
         * @param produto_id Id do produto
         * @param quantidade Quantidade a ser adicionada
         * @return retornar novo estoque
         */
        int adicionar_estoque(string token, int loja_id, int produto_id, int quantidade) {
            
            auto userValido = this->verificaToken(token);
            auto loja = this->lojas.find(loja_id);
            
            if(verificaProprietario(*userValido, loja->second))
            {
                vector<Produto>* produto = &loja->second.produtos;
                auto pos{produto->begin()};
                for(int i =0; i<produto->size(); i++)
                {
                    pos++;
                    if(produto->at(i).id==produto_id)
                    {
                        auto item = produto->at(i);
                        produto->erase(pos);
                        item.quantidade+=quantidade;
                        produto->push_back(item);

                        cout<< "Produto " << item.nome << " foi adicionado ao estoque com sucesso! \n";
                        return item.quantidade;
                        
                    }
                    
                }    
            }
            return -1;
        }

        /**
         * Muda um produto da loja com o id loja_origem_id para loja_destino_id
         * Garantir que:
         *  - loja_origem_id e loja_destino_id são do usuário
         *  - O produto está originalmente na loja_origem
         *  - loja_origem_id != loja_destino_id
         * 
         * @param token Token de acesso
         * @param loja_origem_id Id da loja de origem
         * @param loja_destino_id Id da loja de destino
         * @param produto_id Id do produto
         * @return True se a operação foi bem sucedida, false caso contrário
         */
        bool transferir_produto(string token, int loja_origem_id, int loja_destino_id, int produto_id) {

            auto userValido = this->verificaToken(token);
            auto loja_origem = this->lojas.find(loja_origem_id);
            auto loja_destino = this->lojas.find(loja_destino_id);

            //verifica se são lojas diferentes e se o usuario é valido
           if(loja_origem_id!=loja_destino_id && userValido!=nullptr)
            {
                //verifica se as duas lojas são do msm prorpietario
                if(verificaProprietario(*userValido, loja_origem->second) && verificaProprietario(*userValido, loja_destino->second))
                {
                    vector<Produto> *vetor1 = &loja_origem->second.produtos;
                    vector<Produto> *vetor2 = &loja_destino->second.produtos;
                   
                    for (auto i = vetor1->begin(); i != vetor1->end(); i++)
                    {
                        
                        if(i->id=produto_id)
                        {
                            vetor2->push_back(*i);
                            vetor1->erase(i);

                            delete userValido; //libera memoria
                            
                            cout<< "Produto " << i->nome << " foi transferido da loja "
                            << loja_origem->second.nome
                            << " para loja "<<loja_destino->second.nome
                            <<" com sucesso! \n";
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        /**
         * Lista de produtos do marketplace que tem a string nome_parcial no nome
         * 
         * @param nome_parcial String que deve aparecer no nome do produto
         * @return Lista de produtos que tem a string nome_parcial no nome
         */
        vector<Produto> buscar_produtos(string nome_parcial) {
            vector<Produto> encontrados;

            for (auto loja : this->lojas)
            {
                for (auto p : loja.second.produtos)
                {
                   
                    if(p.nome.find(nome_parcial)!=string::npos)
                    {
                        encontrados.push_back(p);
                    }
                }
                
            }
            
            if(encontrados.empty()){cout<< "Não foram encontrados produtos com esse nome!\n";}
            return encontrados;
        }

        /**
         * Lista de produtos de uma loja específica do marketplace que tem a string nome_parcial no nome
         * 
         * @param nome_parcial String que deve aparecer no nome do produto
         * @param loja_id Id da loja
         * @return Lista de produtos que tem a string nome_parcial no nome e que pertencem a loja especificada
         */
        vector<Produto> buscar_produtos(string nome_parcial, int loja_id) {
            vector<Produto> encontrados;

            auto loja = this->lojas.find(loja_id);

            if(loja!=this->lojas.end())
            {

                for (auto p : loja->second.produtos)
                {
                    if(p.nome.find(nome_parcial)!=string::npos){encontrados.push_back(p);}
                }
            }

            
            if(encontrados.empty()){cout<< "Não foi possível encontrar o produto em questão!\n";}
            return encontrados;
        }

        /**
         * Lista de lojas do marketplace que tem a string nome_parcial no nome
         * 
         * @param nome_parcial String que deve aparecer no nome da loja
         * @return Lista de lojas que tem a string nome_parcial no nome
         */
        vector<Loja> buscar_lojas(string nome_parcial) 
        {
            vector<Loja> encontradas;
            for (auto loja : this->lojas)
            {
                if(loja.second.nome.find(nome_parcial)!=std::string::npos){encontradas.push_back(loja.second);}
            }
            
            if(encontradas.empty()){cout<< "Não foram encontradas lojas com esse nome!\n";}
            return encontradas;
        }

        /**
         * Lista de lojas do marketplace
         * 
         * @return Lista de lojas do marketplace
         */
        vector<Loja> listar_lojas() {
            
            vector<Loja> encontradas;
            for (auto loja : this->lojas)
            {
                encontradas.push_back(loja.second);
            }
            
            if(encontradas.empty()){cout<< "Não existem lojas cadastradas!";}
            return encontradas;
        }

        /**
         * Cria uma nova Venda para o usuário com acesso com esse token,
         * para o produto especificado, para a loja desse produto e com a quantidade especificada.
         * 
         * @param token Token de acesso
         * @param produto_id Id do produto
         * @param quantidade Quantidade a ser vendida
         * @return Id da venda criada ou -1 caso não seja possível criar a venda
         */
        int comprar_produto(string token, int produto_id, int quantidade) 
        {
            Venda venda;
            venda.id= -1;
            auto userValido = this->verificaToken(token);
            auto produto = buscar_produto_id(produto_id);
            if(produto!=nullptr && userValido!=nullptr)
            {
                if(produto->quantidade<quantidade){ return -1;} // n existe estoque para essa compra

                venda.comprador_id = userValido->id;
                venda.produto_id = produto_id;
                venda.id = this->gerarId(); //melhorar o id da venda
                venda.quantidade = quantidade;
                venda.preco_unitario = produto->preco; 

                //conferindo o id da loja ao qual o produto pertence
                auto loja = buscar_loja_idProduto(produto_id);
                if(loja!=nullptr){
                    venda.loja_id = loja->id;
                }else{
                    venda.loja_id = this->gerarId();
                }

                this->vendas.push_back(venda);
                
                atualiza_estoque(loja->id, quantidade, produto_id);//atualizar o estoque

                cout<< "Usuário: "<<userValido->nome << " realizou a compra com sucesso!"<<endl;
                delete userValido;
                delete loja;

            
                
                
            }
            
            return venda.id;
        }


        // Métodos de debug (adicionar a vontade)
        void show_usuarios() {
            for (auto it = usuarios.begin(); it != usuarios.end(); it++) {
                cout <<"Nome: " << it->first
                << " || Email: " << it->second.email
                << " || id: " << it->second.id
                << " || senha: " << it->second.senha_hash<< endl;
                
            }
        }
        void show_tokens() {
            for (auto it = acessos_liberados.begin(); it != acessos_liberados.end(); it++) {
                cout << "Token: "<<it->first << " || id: " << it->second << endl;
            }
        }

        void show_produtos() {
            for(auto h= lojas.begin(); h!=lojas.end(); h++){
                cout<<" Loja:  "<< h->second.nome << " - Proprietário: "<< h->second.proprietario.nome <<endl;
                for (auto it = h->second.produtos.begin(); it != h->second.produtos.end(); it++) {
                cout << "nome: "<< it->nome << " || preço: " << it->preco  << " || quantidade "<< it->quantidade <<endl;
                }
            }
        }

        void show_lojas() {
            for (auto it = lojas.begin(); it != lojas.end(); it++) {
                cout <<"Nome da loja: "<< it->second.nome << " id: " << it->second.id << endl;
            }
        }

         void show_vendas() {
            for (auto it = vendas.begin(); it != vendas.end(); it++) {
                cout <<"id da loja: "<< it->loja_id 
                << " || id do comprador: " << it->comprador_id 
                << " || id do produto: " << it->produto_id
                << " || id da venda: " << it->id
                << " || Quantidade: " << it->quantidade
                << endl;
            }
        }
};

#endif 

