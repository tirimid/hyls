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

    // extract all tokens, keywords are not processed here
    for (const char *c = src; c < src + src_len; ++c)
    {
        if (strchr(" \t\r\f\0", *c))
            continue;
        else if (*c == '\n')
            ++line;
        else if (strchr(":;={}", *c)) // special characters
        {
            enum tmlex_token_type tok_type;
            switch (*c)
            {
            case ':':
                tok_type = TMLEX_TOKEN_TYPE_COLON;
                break;
            case ';':
                tok_type = TMLEX_TOKEN_TYPE_SEMICOLON;
                break;
            case '=':
                tok_type = TMLEX_TOKEN_TYPE_EQUALS;
                break;
            case '{':
                tok_type = TMLEX_TOKEN_TYPE_BRACE_LEFT;
                break;
            case '}':
                tok_type = TMLEX_TOKEN_TYPE_BRACE_RIGHT;
                break;
            }

            out_toks[next_tok] = (struct tmlex_token)
            {
                .type = tok_type,
                .line = line,
            };
            ++next_tok;
        }
        else if (isdigit(*c)) // literal number
        {
            if (next_tok >= max_tok_cnt)
                UTIL_PRINT_ERROR("tmlex tried adding too many tokens to out_toks");
            
            char *lex_buf = malloc(128);
            memset(lex_buf, '\0', sizeof(lex_buf));

            for (; isdigit(*c) || *c == '.'; ++c)
                strncat(lex_buf, c, 1);
            --c; // on next loop iteration, a character is skipped if this is not done

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

            ++c; // not doing this will instantly terminate the string
            for (; *c != '"'; ++c)
                strncat(lex_buf, c, 1);
            // not decrementing c here, closing quote should be skipped

            char *str_buf = malloc(strlen(lex_buf) + 1);
            strcpy(str_buf, lex_buf);
            free(lex_buf);
            
            out_toks[next_tok] = (struct tmlex_token)
            {
                .type     = TMLEX_TOKEN_TYPE_LITERAL_STRING,
                .line     = line,
                .val.text = str_buf,
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
            --c; // on next loop iteration, a character is skipped if this is not done

            char *iden_buf = malloc(strlen(lex_buf) + 1);
            strcpy(iden_buf, lex_buf);
            free(lex_buf);

            out_toks[next_tok] = (struct tmlex_token)
            {
                .type     = TMLEX_TOKEN_TYPE_IDENTIFIER,
                .line     = line,
                .val.text = iden_buf,
            };
            ++next_tok;
        }
        else
            UTIL_PRINT_ERRORF("tmlex encountered invalid character '%c' on line %d", *c, line);
    }

    // process keywords (which are currently stored as identifiers)
    for (int i = 0; i < next_tok; ++i)
    {
        struct tmlex_token *tok = &out_toks[i];
        if (tok->type == TMLEX_TOKEN_TYPE_IDENTIFIER)
        {
            if (strcmp(tok->val.text, "category") == 0)
            {
                free(tok->val.text);
                tok->type = TMLEX_TOKEN_TYPE_KEYWORD_CATEGORY;
            }
            else if (strcmp(tok->val.text, "list") == 0)
            {
                free(tok->val.text);
                tok->type = TMLEX_TOKEN_TYPE_KEYWORD_LIST;
            }
            else if (strcmp(tok->val.text, "num") == 0)
            {
                free(tok->val.text);
                tok->type = TMLEX_TOKEN_TYPE_KEYWORD_NUM;
            }
            else if (strcmp(tok->val.text, "str") == 0)
            {
                free(tok->val.text);
                tok->type = TMLEX_TOKEN_TYPE_KEYWORD_STR;
            }
        }
    }
    
    *out_tok_cnt = next_tok;
}

void tmlex_print_tokens(const struct tmlex_token toks[], int tok_cnt)
{
    for (int i = 0; i < tok_cnt; ++i)
    {
        printf("[%d] ", i);
        switch (toks[i].type)
        {
        case TMLEX_TOKEN_TYPE_COLON:
            printf("colon (:)\n");
            break;
        case TMLEX_TOKEN_TYPE_SEMICOLON:
            printf("semicolon (;)\n");
            break;
        case TMLEX_TOKEN_TYPE_EQUALS:
            printf("equals (=)\n");
            break;
        case TMLEX_TOKEN_TYPE_BRACE_LEFT:
            printf("lbrace ({)\n");
            break;
        case TMLEX_TOKEN_TYPE_BRACE_RIGHT:
            printf("rbrace (})\n");
            break;

        case TMLEX_TOKEN_TYPE_KEYWORD_CATEGORY:
            printf("kwcat\n");
            break;
        case TMLEX_TOKEN_TYPE_KEYWORD_LIST:
            printf("kwlist\n");
            break;
        case TMLEX_TOKEN_TYPE_KEYWORD_NUM:
            printf("kwnum\n");
            break;
        case TMLEX_TOKEN_TYPE_KEYWORD_STR:
            printf("kwstr\n");
            break;
            
        case TMLEX_TOKEN_TYPE_LITERAL_NUMBER:
            printf("litnum: %f\n", toks[i].val.num);
            break;
        case TMLEX_TOKEN_TYPE_LITERAL_STRING:
            printf("litstr: \"%s\"\n", toks[i].val.text);
            break;
        case TMLEX_TOKEN_TYPE_IDENTIFIER:
            printf("iden: %s\n", toks[i].val.text);
            break;
        }
    }
}

void tmlex_free_tokens(struct tmlex_token toks[], int tok_cnt)
{
    for (int i = 0; i < tok_cnt; ++i)
    {
        switch (toks[i].type)
        {
        case TMLEX_TOKEN_TYPE_LITERAL_STRING:
        case TMLEX_TOKEN_TYPE_IDENTIFIER:
            free(toks[i].val.text);
            break;
        }
    }
}
