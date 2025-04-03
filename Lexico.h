/*
Integrantes: Raphael Iniesta Reis
RA: 10396285
*/

#ifndef __LEXICO_H__
#define __LEXICO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tokens.h"

char *read_file(FILE *file);
void program(LinkedList *list, char **buffer, int *lineCount);
void compound_stmt(LinkedList *list, char **buffer, int *lineCount);
void var_decl(LinkedList *list, char **buffer, int *lineCount);
void type_specifier(LinkedList *list, char **buffer, int *lineCount);
void var_decl_list(LinkedList *list, char **buffer, int *lineCount);
void variable_id(LinkedList *list, char **buffer, int *lineCount);
void stmt(LinkedList *list, char **buffer, int *lineCount);
void assig_stmt(LinkedList *list, char **buffer, int *lineCount);
void cond_stmt(LinkedList *list, char **buffer, int *lineCount, TokenTypes token);
void while_stmt(LinkedList *list, char **buffer, int *lineCount);
void expr(LinkedList *list, char **buffer, int *lineCount);
void conjunction(LinkedList *list, char **buffer, int *lineCount);
void comparison(LinkedList *list, char **buffer, int *lineCount);
void relation(LinkedList *list, char **buffer, int *lineCount);
void sum(LinkedList *list, char **buffer, int *lineCount);
void term(LinkedList *list, char **buffer, int *lineCount);
void factor(LinkedList *list, char **buffer, int *lineCount);

void Analisador_Lexico(LinkedList *list, char **buffer, int *lineCount) {
    program(list, buffer, lineCount);
    print_list(list);
}

// Faz a leitura do arquivo e retorna um array de chars.
char *read_file(FILE *file) {
    char *buffer = 0;
    long length;

    if(file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);

        if(buffer) {
            fread(buffer, 1, length, file);
        }

        fclose(file);
    }

    return buffer;
}

// Vai consumir todos os comentários quando houver um
void consume_comment(LinkedList *list, char **buffer, int *lineCount) {
    if(**buffer == '/') {
        (*buffer)++;
        if(**buffer == '/') { // Verifica se é um comentário de uma linha
        insert_node(list, new_node(comentario_linha_token, *lineCount, "", ""));

            while(**buffer != '\n') {
                (*buffer)++;
            }
            (*lineCount)++;
            (*buffer)++; // Consome o '\n'
        }

        if(**buffer == '*') {
            (*buffer)++; // Consome *
            int loop = 1;
            while(loop == 1) {
                if(**buffer == '\n') {
                    (*lineCount)++;
                    (*buffer)++;
                }

                // Verifica o final do comentário de multiplas linhas
                if(**buffer == '*') {
                    insert_node(list, new_node(comentario_multilinha_token, *lineCount, "", ""));

                    (*buffer)++;
                    if(**buffer == '/') {
                        (*buffer)++;
                        break;
                    }
                }
        
                (*buffer)++;
            }
        }
    }
    
    if(**buffer == '/') {
        consume_comment(list, buffer, lineCount);
    }
}

void consume_blank_space(LinkedList *list, char **buffer, int *lineCount) {
    if(isspace(**buffer) == 1) {
        if(**buffer == '\n') (*lineCount)++;
        (*buffer)++;
        consume_blank_space(list, buffer, lineCount);
        return;
    } else if(**buffer == '/') {
        consume_comment(list, buffer, lineCount);
    }
    return;
}

void error(TokenTypes expected, TokenTypes received) {
    printf("Erro sintático, esperado[");
    print_token(expected);
    printf("] encontrado[");
    print_token(received);
    printf("]\n");
    exit(1);
}

// Retorna o token identificado e também insere na lista encadeada
TokenTypes getToken(LinkedList *list, char **buffer, int *lineCount) {

    consume_blank_space(list, buffer, lineCount);

    TokenTypes tokens[10] = {int_token, char_token, write_int_token, read_int_token, if_token, else_token, while_token, abre_par_token, fecha_par_token};
    char *tokensStr[10] = {"int", "char", "writeint", "readint", "if", "else", "while", "(", ")"};
    char type[10] = "\0";
    int counter = 0;
    int loop = 0;

    while(isalnum(**buffer) || **buffer == '_') {
        type[counter] = (**buffer);
        counter++;
        (*buffer)++;
    }

    for(int i = 0; i < 9; i++) {
        if((strcmp(type, tokensStr[i])) == 0) return tokens[i];
        loop++;
    }

    if(loop == 9) {
        if(isdigit(type[0])) { // Se for um dígito, deve ser um hexadecimal -> 0xfff
            insert_node(list, new_node(int_token, *lineCount, type, ""));
            
            return int_token;
        } else if(isalpha(type[0])) {
            int counter2 = 0;
            
            while(type[counter2] != '\0') counter2++;

            if(counter2 == 1) { // Verifica se é um char
                insert_node(list, new_node(char_token, *lineCount, *buffer, ""));
                (*buffer)++; // Consome o char
                return char_token;
            }

            insert_node(list, new_node(id_token, *lineCount, "", type));
            return id_token;
        }
    }

    return null_token;
}

void getID(LinkedList *list, char **buffer, int *lineCount) {
    char id[10] = "\0";
    int counter = 0;

    while(isalnum(**buffer) || **buffer == '_') {
        id[counter] = (**buffer);
        (*buffer)++;
        counter++;
    }

    insert_node(list, new_node(id_token, *lineCount, "", id));
}

void program(LinkedList *list, char **buffer, int *lineCount) {
    consume_comment(list, buffer, lineCount); // Consome quaisquer comentários que possam estar no começo do arquivo

    // Consome quaisquer linhas em branco antes do programa principal
    consume_blank_space(list, buffer, lineCount);

    while(**buffer != '{') {
        char token[5] = "\0";
        char *types[4] = {"void", "main", "(", ")"};
        TokenTypes token_type[4] = {void_token, main_token, abre_par_token, fecha_par_token};
        int count = 0;
        if(isalpha(**buffer)) {
            while(isalpha(**buffer)) {
                token[count] = **buffer;
                count++;
                (*buffer)++;
            }

            for(int i = 0; i < 4; i++) {
                if(strcmp(token, types[i]) == 0) {
                    insert_node(list, new_node(token_type[i], *lineCount, "", ""));
                }
            }

        } else if(**buffer == '(') {
            insert_node(list, new_node(abre_par_token, *lineCount, "", ""));
            (*buffer)++;
        } else if(**buffer == ')') {
            insert_node(list, new_node(fecha_par_token, *lineCount, "", ""));
            (*buffer)++;
        }

        // Consome quaisquer espaços em branco
        while(**buffer == ' ') {
            (*buffer)++;
        }
    }

    insert_node(list, new_node(abre_chaves_token, *lineCount, "", ""));
    compound_stmt(list, buffer, lineCount);
}

void compound_stmt(LinkedList *list, char **buffer, int *lineCount) {
    if(**buffer == '{') (*buffer)++;
    consume_blank_space(list, buffer, lineCount);

    var_decl(list, buffer, lineCount);

    consume_blank_space(list, buffer, lineCount);

    while(**buffer != '\0') {
        stmt(list, buffer, lineCount);
        if(**buffer == '}') {
            insert_node(list, new_node(fecha_chaves_token, *lineCount, "", ""));
            // (*buffer)++;
            break;
        }
    }

    if(**buffer == '\0') {
        error(fecha_chaves_token, getToken(list, buffer, lineCount));
    }
    (*buffer)++; // Consome }
    return;
}

void var_decl(LinkedList *list, char **buffer, int *lineCount) {

    type_specifier(list, buffer, lineCount);
    var_decl_list(list, buffer, lineCount);

    return;
}

void type_specifier(LinkedList *list, char **buffer, int *lineCount) {
    char type[6] = "\0";
    int counter = 0;

    while(**buffer != ' ') {
        type[counter] = (**buffer);
        (*buffer)++;
        counter++;
    }

    if(strcmp(type, "int") == 0) {
        insert_node(list, new_node(int_token, *lineCount, "", ""));
        (*buffer)++; // Consome ' '
        return;
    } else if(strcmp(type, "char") == 0) {
        insert_node(list, new_node(char_token, *lineCount, "", ""));
        (*buffer)++;
        return;
    }

    error(type_specifier_token, getToken(list, buffer, lineCount));

}

void var_decl_list(LinkedList *list, char **buffer, int *lineCount) {
    while(!isalpha(**buffer)) (*buffer)++;

    while(**buffer != ';') {
        variable_id(list, buffer, lineCount);
    }

    if(**buffer == ';') {
        (*buffer)++; // Consome ;
        insert_node(list, new_node(ponto_virgula_token, *lineCount, "", ""));
        return;
    }
}

void variable_id(LinkedList *list, char **buffer, int *lineCount) {

    consume_blank_space(list, buffer, lineCount);

    char id[100] = "";
    int counter = 0;

    while(isalnum(**buffer) || **buffer == '_') {
        id[counter] = (**buffer);
        counter++;
        (*buffer)++;
    }

    insert_node(list, new_node(id_token, *lineCount, "", id));

    if(**buffer == ',') {
        (*buffer)++; // Consome a vírgula
        variable_id(list, buffer, lineCount);
    }

    if(**buffer == '=') {
        expr(list, buffer, lineCount);
    } else if(**buffer == ';') return;

    /*
    TODO: Handle ERROR
    */

    if(**buffer == '\n') {
        error(ponto_virgula_token, getToken(list, buffer, lineCount));
    }
}

void expr(LinkedList *list, char **buffer, int *lineCount) {
    
    consume_blank_space(list, buffer, lineCount);
    
    conjunction(list, buffer, lineCount);
    
    consume_blank_space(list, buffer, lineCount);

    while(**buffer == '|') {
        (*buffer)++; // Consome o primeiro |
        if(**buffer != '|') error(or_token, getToken(list, buffer, lineCount));
        (*buffer)++;

        consume_blank_space(list, buffer, lineCount);
        
        conjunction(list, buffer, lineCount);

        consume_blank_space(list, buffer, lineCount);
    }
}

void conjunction(LinkedList *list, char **buffer, int *lineCount) {
    consume_blank_space(list, buffer, lineCount);
    comparison(list, buffer, lineCount);
    consume_blank_space(list, buffer, lineCount);

    if(**buffer == '&') {
        (*buffer)++; // Consome &
        
        if(**buffer != '&') error(and_token, getToken(list, buffer, lineCount)); // Erro lexico
        (*buffer)++; // Consome outro &
        consume_blank_space(list, buffer, lineCount);

        conjunction(list, buffer, lineCount);
    }
}

void comparison(LinkedList *list, char **buffer, int *lineCount) {
    sum(list, buffer, lineCount);

    if(**buffer != '&' || **buffer != '<' || **buffer != '>' || **buffer != '=' || **buffer != '!') {
        relation(list, buffer, lineCount);
        sum(list, buffer, lineCount);
    }
}

void sum(LinkedList *list, char **buffer, int *lineCount) {
    term(list, buffer, lineCount);

    while(**buffer == '+' || **buffer == '-') {
        if(**buffer == '+') {
            insert_node(list, new_node(plus_token, *lineCount, "", ""));
            term(list, buffer, lineCount);
        } else if(**buffer == '-') {
            insert_node(list, new_node(minus_token, *lineCount, "", ""));
            term(list, buffer, lineCount);
        }
    }
}

void term(LinkedList *list, char **buffer, int *lineCount) {
    factor(list, buffer, lineCount);

    while(**buffer == '*' || **buffer == '/') {
        if(**buffer == '*') {
            insert_node(list, new_node(times_token, *lineCount, "", ""));
            factor(list, buffer, lineCount);
        } else if(**buffer == '/') {
            insert_node(list, new_node(div_token, *lineCount, "", ""));
            factor(list, buffer, lineCount);
        }
    }
}

void factor(LinkedList *list, char **buffer, int *lineCount) {

    consume_blank_space(list, buffer, lineCount);

    TokenTypes token = getToken(list, buffer, lineCount);

    if(token == abre_par_token) {
        consume_blank_space(list, buffer, lineCount);
        expr(list, buffer, lineCount);
        consume_blank_space(list, buffer, lineCount);

        return;
    }

    
    /*
    TODO: Handle ERROR
    */
   return;
}

void stmt(LinkedList *list, char **buffer, int *lineCount) {

    consume_blank_space(list, buffer, lineCount);

    TokenTypes token = getToken(list, buffer, lineCount);
    
    switch(token) {
        case write_int_token:
            insert_node(list, new_node(write_int_token, *lineCount, "", ""));
            consume_blank_space(list, buffer, lineCount);

            if(**buffer != '(') error(abre_par_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(abre_par_token, *lineCount, "", ""));
            (*buffer)++;

            expr(list, buffer, lineCount);

            if(**buffer != ')') error(fecha_par_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(fecha_par_token, *lineCount, "", ""));
            (*buffer)++;

            if(**buffer != ';') error(ponto_virgula_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(ponto_virgula_token, *lineCount, "", ""));
            (*buffer)++;
            return;
            break;
        case read_int_token:
            insert_node(list, new_node(read_int_token, *lineCount, "", ""));

            if(**buffer != '(') error(abre_par_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(abre_par_token, *lineCount, "", ""));
            (*buffer)++;

            getID(list, buffer, lineCount);

            if(**buffer != ')') error(fecha_par_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(fecha_par_token, *lineCount, "", ""));
            (*buffer)++;

            if(**buffer != ';') error(ponto_virgula_token, getToken(list, buffer, lineCount));
            insert_node(list, new_node(ponto_virgula_token, *lineCount, "", ""));
            (*buffer)++; // Consome ;

            return;
            break;

        case else_token:
        case if_token:
            cond_stmt(list, buffer, lineCount, token);
            return;
            break;

        case id_token:
            assig_stmt(list, buffer, lineCount);
            return;
            break;

        case while_token:
            while_stmt(list, buffer, lineCount);
            return;
            break;

        default:
            break;
    }

    return;
}

void assig_stmt(LinkedList *list, char **buffer, int *lineCount) {
    consume_blank_space(list, buffer, lineCount);
    
    if(**buffer != '=') error(atribui_token, getToken(list, buffer, lineCount));
    insert_node(list, new_node(atribui_token, *lineCount, "", ""));
    (*buffer)++; // Consome = 
    
    
    consume_blank_space(list, buffer, lineCount);
    expr(list, buffer, lineCount);
    
    if(**buffer != ';') error(ponto_virgula_token, getToken(list, buffer, lineCount));
    insert_node(list, new_node(ponto_virgula_token, *lineCount, "", ""));
    return;
}

void cond_stmt(LinkedList *list, char **buffer, int *lineCount, TokenTypes token) {
    
    if(token == if_token) {
        consume_blank_space(list, buffer, lineCount);
        insert_node(list, new_node(if_token, *lineCount, "", ""));
        
        if(**buffer != '(') error(abre_par_token, getToken(list, buffer, lineCount));
        insert_node(list, new_node(abre_par_token, *lineCount, "", ""));
        (*buffer)++;
        
        while(**buffer != 41) {
            expr(list, buffer, lineCount);
            consume_blank_space(list, buffer, lineCount);
        }
        
        insert_node(list, new_node(fecha_par_token, *lineCount, "", ""));
        
        (*buffer)++; // Consome )
        consume_blank_space(list, buffer, lineCount);
        
        stmt(list, buffer, lineCount);
        (*buffer)++; // Consome ;

        return;
    } else {
        insert_node(list, new_node(else_token, *lineCount, "", ""));
        consume_blank_space(list, buffer, lineCount);
        stmt(list, buffer, lineCount);
        if(**buffer == ';') (*buffer)++;
        return;
    }

}

void while_stmt(LinkedList *list, char **buffer, int *lineCount) {
    consume_blank_space(list, buffer, lineCount);

    if(**buffer != '(') error(abre_par_token, getToken(list, buffer, lineCount));
    (*buffer)++; // Consome (
    consume_blank_space(list, buffer, lineCount);

    expr(list, buffer, lineCount);

    consume_blank_space(list, buffer, lineCount);

    if(**buffer != ')') error(fecha_par_token, getToken(list, buffer, lineCount));
    (*buffer)++; // Consome )

    stmt(list, buffer, lineCount);
}

void relation(LinkedList *list, char **buffer, int *lineCount) {
    consume_blank_space(list, buffer, lineCount);
    if(**buffer == ' ') (*buffer)++;
    if(**buffer == '<') {
        (*buffer)++;
        if(**buffer == '=') {
            insert_node(list, new_node(menor_igual_token, *lineCount, "", ""));
            return;
        }
        insert_node(list, new_node(menor_token, *lineCount, "", ""));
        return;
    } else if(**buffer == '>') {
        (*buffer)++;
        if(**buffer == '=') {
            insert_node(list, new_node(maior_igual_token, *lineCount, "", ""));
            return;
        }
        insert_node(list, new_node(maior_token, *lineCount, "", ""));
        return;
    } else if(**buffer == '=') {
        (*buffer)++;
        if(**buffer != '=') error(igual_token, getToken(list, buffer, lineCount));
        insert_node(list, new_node(igual_token, *lineCount, "", ""));
        return;
    } else if(**buffer == '!') {
        (*buffer)++;
        if(**buffer != '=') error(diferente_token, getToken(list, buffer, lineCount));
        insert_node(list, new_node(diferente_token, *lineCount, "", ""));
        return;
    }
}

#endif