// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "tmlex.h"

#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void tmlex_lex(struct tmlex_token out_toks[], int *out_tok_cnt, int max_tok_cnt, const char *src, size_t src_len)
{
    int next_tok = 0;
    int line     = 1;
    
    for (const char *c = src; c < src + src_len; ++c)
    {
        if (strchr(" \t\r\f", *c))
            continue;
        else if (*c == '\n')
            ++line;
        else if (isdigit(*c)) // literal number
        {
            if (next_tok >= max_tok_cnt)
                UTIL_PRINT_ERROR("tmlex tried adding too many tokens to out_toks");
            
            char *lex_buf = malloc(128);
            memset(lex_buf, '\0', sizeof(lex_buf));

            for (; isdigit(*c) || *c == '.'; ++c)
                strncat(lex_buf, c, 1);

            float num = (float)atof(lex_buf);
            free(lex_buf);
            
            out_toks[next_tok] = (struct tmlex_token)
            {
                .type    = TMLEX_TOKEN_TYPE_LITERAL_NUMBER,
                .line    = line,
                .val.num = num,
            };
            ++next_tok;
        }
        else if (*c == '"') // literal string
        {
            if (next_tok >= max_tok_cnt)
                UTIL_PRINT_ERROR("tmlex tried adding too many tokens to out_toks");
            
            char *lex_buf = malloc(512);
            memset(lex_buf, '\0', sizeof(lex_buf));

            for (; *c != '"'; ++c)
                strncat(lex_buf, c, 1);

            char *str_buf = malloc(strlen(lex_buf) + 1);
            strcpy(str_buf, lex_buf);
            free(lex_buf);
            
            out_toks[next_tok] = (struct tmlex_token)
            {
                .type    = TMLEX_TOKEN_TYPE_LITERAL_STRING,
                .line    = line,
                .val.str = str_buf,
            };
            ++next_tok;
        }
        else if (isalpha(*c) || *c == '_') // identifier
        {
            if (next_tok >= max_tok_cnt)
                UTIL_PRINT_ERROR("tmlex tried adding too many tokens to out_toks");
            
            char *lex_buf = malloc(128);
            memset(lex_buf, '\0', sizeof(lex_buf));

            for (; isalnum(*c) || *c == '_'; ++c)
                strncat(lex_buf, c, 1);

            char *iden_buf = malloc(strlen(lex_buf) + 1);
            strcpy(iden_buf, lex_buf);
            free(lex_buf);

            out_toks[next_tok] = (struct tmlex_token)
            {
                .type     = TMLEX_TOKEN_TYPE_IDENTIFIER,
                .line     = line,
                .val.iden = iden_buf,
            };
            ++next_tok;
        }
        else
            UTIL_PRINT_ERRORF("tmlex encountered invalid character '%c' on line %d", *c, line);
    }
    
    *out_tok_cnt = next_tok <= 1 ? 0 : next_tok - 1;
}

void tmlex_print_tokens(const struct tmlex_token toks[], int tok_cnt)
{
    for (int i = 0; i < tok_cnt; ++i)
    {
        printf("[%d] ", i);
        switch (toks[i].type)
        {
        case TMLEX_TOKEN_TYPE_LITERAL_NUMBER:
            printf("litnum: %f\n", toks[i].val.num);
            break;
        case TMLEX_TOKEN_TYPE_LITERAL_STRING:
            printf("litstr: \"%s\"\n", toks[i].val.str);
            break;
        case TMLEX_TOKEN_TYPE_IDENTIFIER:
            printf("iden: %s\n", toks[i].val.iden);
            break;
        }
    }
}

void tmlex_free_tokens(struct tmlex_token toks[], int tok_cnt)
{
}
