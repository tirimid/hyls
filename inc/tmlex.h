// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef TMLEX_H__
#define TMLEX_H__

#include <stddef.h>

enum tmlex_token_type
{
    TMLEX_TOKEN_TYPE_COLON,       // :
    TMLEX_TOKEN_TYPE_SEMICOLON,   // ;
    TMLEX_TOKEN_TYPE_EQUALS,      // =
    TMLEX_TOKEN_TYPE_BRACE_LEFT,  // {
    TMLEX_TOKEN_TYPE_BRACE_RIGHT, // }

    TMLEX_TOKEN_TYPE_KEYWORD_CATEGORY, // category
    TMLEX_TOKEN_TYPE_KEYWORD_LIST,     // list
    TMLEX_TOKEN_TYPE_KEYWORD_NUM,      // num
    TMLEX_TOKEN_TYPE_KEYWORD_STR,      // str

    TMLEX_TOKEN_TYPE_LITERAL_STRING,
    TMLEX_TOKEN_TYPE_LITERAL_NUMBER,
    TMLEX_TOKEN_TYPE_IDENTIFIER,
};

struct tmlex_token
{
    enum tmlex_token_type type;
    int line;
    
    union
    {
        char *text;
        float num;
    } val;
};

void tmlex_lex(struct tmlex_token out_toks[], int *out_tok_cnt, int max_tok_cnt, const char *src, size_t src_len);
void tmlex_print_tokens(const struct tmlex_token toks[], int tok_cnt);
void tmlex_free_tokens(struct tmlex_token toks[], int tok_cnt);

#endif
