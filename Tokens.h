/*
Integrantes: Raphael Iniesta Reis
RA: 10396285
*/

#ifndef Tokens_h
#define Tokens_h

typedef enum {

    declaracao_token,
    
    expressao_token,
    
    // Struct nulo (início da lista)
    null_token,
 
// --------------------------------------------------
    
    // Operandos:
    operando_token,
    // Número
    numero_token,
    
    // Nome de variável
    id_token, 

    
    // Lógicos
    or_token, // ||
    and_token, // &&
    not_token, // !
    true_token,
    false_token,
    
// --------------------------------------------------
    
    // Operadores matemáticos
    plus_token, // +
    minus_token, // -
    div_token, // /
    times_token, // *
    
// --------------------------------------------------
    
    // Comentários
    comentario_linha_token, // //
    comentario_multilinha_token, // /* */
    
// --------------------------------------------------
    
    // Tipo
    int_token, // int
    bool_token, // bool
    if_token, // if { ... }
    else_token, // else { ... }
    main_token, // main() { ... }
    write_int_token, // printf()
    read_int_token, // scanf()
    void_token, // void <nome>() { ... }
    while_token, // while(condição) { ... }
    type_specifier_token,
    
// --------------------------------------------------
    
    // Atribuição
    atribui_token, // =

// --------------------------------------------------
    
    // Blocos
    abre_par_token, // (
    fecha_par_token, // )
    abre_chaves_token, // {
    fecha_chaves_token, // }
    
// --------------------------------------------------
    
    // Relacional
    menor_token, // <
    maior_token, // >
    menor_igual_token, // <=
    maior_igual_token, // >=
    igual_token, // ==
    diferente_token, // !=
    
// --------------------------------------------------
    
    // Delimitadores
//    new_line_token, // \n
    retorno_carro_token, // \r
    tab_token, // \t
    vazio_token, // \0
    space_token, // ' '
    
// --------------------------------------------------
        
    // Delimitadores
    virgula_token,
    ponto_virgula_token,
    dois_pontos_token,
    
// --------------------------------------------------
    
    error_token, // Erro
    
// --------------------------------------------------
    
    char_token, // Identificar se é uma char
    return_token
    
} TokenTypes;

void print_token(TokenTypes token) {
    char *tokens[48] = {"declaracao", "expressao", "null", "operando", "numero", "id", "OR", "AND", "NOT", "true", "false", "SOMA", "SUBTRAÇÃO", "DIVISÃO", "MULTIPLICAÇÃO", "comentário em linha", "comentário multilinha", "int", "bool", "if", "else", "main", "writeint", "readint", "void", "while", "type_specifier", "atribui", "abre_par", "fecha_par", "abre_chaves", "fecha_chaves", "menor", "maior", "menor_igual", "maior_igual", "igual", "diferente", "retorno_carro_token", "tab_token", "vazio_token", "space_token", "virgula_token", "ponto_vírgula", "dois_pontos", "error_token", "char", "return"};

    printf("%s", tokens[token]);
}

#endif