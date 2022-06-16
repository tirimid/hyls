// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "tmlex.h"

#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static float lex_literal_number(const char **c)
{
    static char lex_buf[1024];
    memset(lex_buf, '\0', sizeof(lex_buf));

    return (float)atof(lex_buf);
}

static char *lex_literal_string(const char **c)
{
    static char lex_buf[1024];
    memset(lex_buf, '\0', sizeof(lex_buf));

    return lex_buf;
}

void tmlex_lex(struct tmlex_token out_toks[], int *out_tok_cnt, int max_tok_cnt, const char *src, size_t src_len)
{
    int next_tok = 0;
    int line     = 1;
    
    for (const char *c = src; c < src + src_len; ++c)
    {
        if (*c == '\n')
            ++line;
        else if (isdigit(*c))
        {
            out_toks[next_tok] = (struct tmlex_token)
            {
                .type    = TMLEX_TOKEN_TYPE_LITERAL_NUMBER,
                .line    = line,
                .val.num = lex_literal_number(&c),
            };
            ++next_tok;
        }
        else if (isalpha(*c) || *c == '_')
        {
        }
        else
            UTIL_PRINT_ERRORF("tmlex encountered invalid character '%c' on line %d", *c, line);
    }
    
    *out_tok_cnt = next_tok <= 1 ? 0 : next_tok - 1;
}

void tmlex_print_toks(const struct tmlex_token toks[], int tok_cnt)
{
}

void tmlex_free_toks(struct tmlex_token toks[], int tok_cnt)
{
}
