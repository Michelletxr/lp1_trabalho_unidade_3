
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "utils.h"
#include "marketplace.h"

int main() {
    
    Marketplace<void> marketplace;

    bool cadastro1_ok, cadastro2_ok, cadastro3_ok;

    cout<< "\n\n\t\t\t !--------------TESTES UNITÁRIOS----------!\n\n";

    cadastro1_ok = marketplace.me_cadastrar("João", "joao@gmail.com", "123456");
    testa(cadastro1_ok, "Cadastro de usuário 1");
    cadastro2_ok = marketplace.me_cadastrar("Maria", "maria@gmail.com", "654321");
    testa(cadastro2_ok, "Cadastro de usuário 2");
    cadastro3_ok = marketplace.me_cadastrar("Joana", "joana@gmail.com", "124035");
    testa(cadastro2_ok, "Cadastro de usuário 3");

    marketplace.show_usuarios();

    if ( !cadastro1_ok || !cadastro2_ok) {
        cout << "Cadastro de João ou Maria nao realizado" << endl;
        return -1;
    }

    string invalid_token = marketplace.login("inexistente@hotmail.com", "senha qualquer");
    testa(invalid_token == "invalid", "login de usuario inexistente");
    
    string joao_token = marketplace.login("joao@gmail.com", "123456");
    testa(joao_token != "invalid", "login de usuario valido");
    cout << "Token de acesso recebido para João: " << joao_token << endl;

    if (joao_token != "invalid") {
        cout << "Usuário logado com sucesso" << endl;

        // João cria duas lojas com o seu token de acesso
        int bodega_do_joao_id = marketplace.criar_loja(joao_token, "Bodega do João");
        testa(bodega_do_joao_id != -1, "Cadastro de loja");
        int acougue_do_joao_id = marketplace.criar_loja(joao_token, "Açougue do João");
        testa(acougue_do_joao_id != -1, "Cadastro de loja");
    
       
        // João adiciona produtos na loja e altera o seu estoque
        int leite_id = marketplace.adicionar_produto(joao_token, bodega_do_joao_id, "Leite em pó", 8.40);
        testa(leite_id != -1, "Cadastro de produto");
        marketplace.adicionar_estoque(joao_token, bodega_do_joao_id, leite_id, 10);
        
        
       
      int novo_estoque_leite = marketplace.adicionar_estoque(joao_token, bodega_do_joao_id, leite_id, 5);
      testa(novo_estoque_leite == 15, "Adicionando estoque");
       
        int arroz_id = marketplace.adicionar_produto(joao_token, bodega_do_joao_id, "Arroz", 3.50);
        marketplace.adicionar_estoque(joao_token, bodega_do_joao_id, arroz_id, 40);
        
        int coca_id = marketplace.adicionar_produto(joao_token, bodega_do_joao_id, "Coca cola 250ml", 2.40);
        marketplace.adicionar_estoque(joao_token, bodega_do_joao_id, coca_id, 15);
        
        // João adicionou um produto na loja que não deveria... Aff João...
        int picanha_id = marketplace.adicionar_produto(joao_token, bodega_do_joao_id, "Picanha Maturada", 58.40);
        marketplace.adicionar_estoque(joao_token, bodega_do_joao_id, picanha_id, 5);
        
        int pic_suina_id = marketplace.adicionar_produto(joao_token, acougue_do_joao_id, "Picanha Suína", 78.40);
        marketplace.adicionar_estoque(joao_token, acougue_do_joao_id, pic_suina_id, 8);
    
        // Transferindo um produto de uma loja para outrao (do mesmo usuário)
        marketplace.transferir_produto(joao_token, bodega_do_joao_id, acougue_do_joao_id, pic_suina_id);



          cout<< "\n\n\t\t\t !--------------JOANA CRIA LIVRARIA----------!\n\n";
        string joana_token = marketplace.login("joana@gmail.com", "124035");
        testa(joana_token != "invalid", "login de usuario valido");
        int livraria_id=  marketplace.criar_loja(joana_token, "livraria da joana");
        testa(livraria_id != -1, "Cadastro de loja");
        int livro1_id = marketplace.adicionar_produto(joana_token, livraria_id, "Dicionário", 100);
        testa(livro1_id != -1, "Cadastro de produto");
        marketplace.adicionar_estoque(joana_token, livraria_id, livro1_id, 40);
        int livro2_id = marketplace.adicionar_produto(joana_token, livraria_id, "Livro didático sobre java", 800);
        testa(livro2_id != -1, "Cadastro de produto");
        marketplace.adicionar_estoque(joana_token, livraria_id, livro2_id, 20);


        // Logar como Maria
        string maria_token = marketplace.login("maria@gmail.com", "654321");
       // marketplace.show_tokens(); // opcional. debug
        //marketplace.show_produtos();
        //marketplace.comprar_produto(maria_token, pic_suina_id, 2);

        // Maria buscando picanha:
        cout<< "\n\n\t\t\t !--------------MARIA COMPRA PRODUTOS----------!\n\n";
        vector<Produto> picanhas = marketplace.buscar_produtos("Picanha");
        
        // verificando se achou picanhas e comprando a primeira
        if (picanhas.size() > 0) {
            marketplace.comprar_produto(maria_token, picanhas[0].id, 1);
            cout<<"\n -Maria comprou "<< picanhas[0].nome<< endl;
        }


        // Maria buscando uma loja qualquer com o nome Bodega
        vector<Loja> bodegas = marketplace.buscar_lojas("Bodega");
        // vendo se existe alguma bodega, se existir, comprando o primeiro produto com o nome Coca
        if (bodegas.size() > 0) {
            vector<Produto> produtos = marketplace.buscar_produtos("Coca", bodegas[0].id);
            if (produtos.size() > 0) {
                marketplace.comprar_produto(maria_token, produtos[0].id, 1);
                cout<<"\n -Maria comprou "<< produtos[0].nome<< " na loja: "<<bodegas[0].nome << endl;
            }
        }

        vector<Produto> livro = marketplace.buscar_produtos("java");
        
        // verificando se achou livro e comprando
        if (livro.size() > 0) {
            marketplace.comprar_produto(maria_token, livro[0].id, 4);
            cout<<"\n -Maria comprou "<< livro[0].nome<< endl;
        }


        cout<< "\n\n\t\t\t !--------------TENTANDO TRANSFERIR PRODUTOS DE LOJAS COM PROPRIETÁRIOS DIFERENTES----------!\n\n";
        bool transferir = marketplace.transferir_produto(joao_token, bodega_do_joao_id, livraria_id, coca_id);
        testa(transferir, "Transferênia de produto");
        if(!transferir){ cout << "Não é possível realizar a transferência\n";}

        cout<< "\n\n\t\t\t !--------------TENTANDO BUSCAR PRODUTOS E LOJAS QUE NÃO EXISTEM----------!\n\n";
        vector<Produto> produto_invalido1 = marketplace.buscar_produtos("Computador");
        vector<Produto> produto_invalido2 = marketplace.buscar_produtos("Mouse", -1);
        vector<Loja> loja_invalida = marketplace.buscar_lojas("Magazine");

        cout<< "\n\n\t\t\t !--------------TENTANDO FAZER OPERAÇÕES DE CADASTRO E COMPRA COM TOKEN INVÁLIDO----------!\n\n";
        int loja_id=  marketplace.criar_loja("aleatório", "loja teste");
        testa(loja_id != -1, "Cadastro de loja");
        int produto_id = marketplace.adicionar_produto("aleatório", -1, "produto teste", 1);
        testa(produto_id != -1, "Cadastro de produto");
        int venda_id = marketplace.comprar_produto("aleatório", -1, 1);
        testa(venda_id != -1, "Compra de produto");
            




      
        // mostrando todas as lojas do marketplace
        cout<< "\n\n\t\t\t !--------------LISTA DE LOJAS DO MARKETPLACE COM SEUS PRODUTOS----------!\n\n";
        vector<Loja> lojas = marketplace.listar_lojas();
        for (Loja loja : lojas) {
            cout << loja.nome << endl;
            for (Produto produto : loja.produtos) {
                cout << "\t" << produto.nome << endl;
            }
        }

        cout<< "\n\n\t\t\t !--------------USUÁRIOS----------!\n\n";
        marketplace.show_usuarios(); // opcional. debug
        cout<< "\n\n\t\t\t !--------------PRODUTOS----------!\n\n";
        marketplace.show_produtos(); // opcional. debug
        cout<< "\n\n\t\t\t !--------------VENDAS----------!\n\n";
        marketplace.show_vendas();
        cout<< "\n\n\t\t\t !--------------LOJAS----------!\n\n";
        marketplace.show_lojas();
        cout<< "\n\t\t\t !--------------TOKENS----------!\n\n";
        marketplace.show_tokens();
        cout<<"\n\n";




    } else {
        cout << "Usuário não pode se logar" << endl;
    }
}


///ADICIONAR MAIS TESTES