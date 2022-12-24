#ifndef CARGS_PARSER_H
#define CARGS_PARSER_H

#include <fw.h>


// -o="arg" --
// =           | ->            hyphen
//  =          | -> none    -> reserved
//   =         | ->            equals
//    =        | ->            quots
//     ===     | -> none    -> literal
//        =    | ->            quots
//         =   | ->            space
//          == | -> hyphen  -> termatator
enum TokenKind : int8_t
{
    TK_Invalid      = -1,
    TK_None         = 0,
    TK_Space        = 1,
    TK_Hyphen       = 2,
    TK_Reserved     = 3,
    TK_Equals       = 4,
    TK_Quots        = 5,
    TK_Literal      = 6,
    TK_Terminator   = 7
};

struct Token
{
    TokenKind   kind;
    int32_t     iStart;
    int32_t     iEnd;
};

void CreateToken(Token* token, TokenKind kind, int32_t iStart, int32_t iEnd)
{
    memset(token, 0, sizeof(Token));
    token->kind = kind;
    token->iStart = iStart;
    token->iEnd = iEnd;
}

struct TokenNode
{
    Token pToken;
    TokenNode* pNext;
};

void CreateTokenNode(TokenNode* node, Token* token)
{
    memset(node, 0, sizeof(TokenNode));
    node->pToken = *token;
    node->pNext = NULL;
}

int32_t scan(char* pszCmd, size_t cXCh, Token** arrOutTkn)
{
    size_t strlen = strnlen(pszCmd, cXCh);
    
    TokenKind* arrTknKind = (TokenKind*) malloc(strlen * sizeof(char));

    size_t tknlen = 0;
    for (size_t i = 0; i < strlen; ++i)
    {
        switch (pszCmd[i])
        {
            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                arrTknKind[i] = TK_Space;
                break;
            case '-':
                arrTknKind[i] = TK_Hyphen;
                break;
            case '=':
                arrTknKind[i] = TK_Equals;
                break;          
            case '"':
                arrTknKind[i] = TK_Quots;
                break;
            default:
                arrTknKind[i] = TK_None;
                break;
        }
    }

    size_t cTkn;
    for (size_t i = 0; i < strlen; ++i)
    {
        switch (arrTknKind[i])
        {
            case TK_Hyphen:
                if (i < strlen - 1 && arrTknKind[i + 1] == TK_Hyphen)
                {
                    arrTknKind[i + 0] = TK_Terminator;
                    arrTknKind[i + 1] = TK_Terminator;
                    i++;
                }
                break;

            case TK_Equals:
            case TK_Quots:
                break;

            case TK_Space:
            case TK_None:
                {
                    TokenKind kind = arrTknKind[i];
                    for (; i < strlen && arrTknKind[i] == kind; ++i) {}
                    i--;
                }
                break;
        }
        cTkn++;
    }

    Token* arrTkn = (Token*) malloc(cTkn * sizeof(Token));
    for (size_t i = 0, iTkn = 0; i < strlen; ++i)
    {
        switch (arrTknKind[i])
        {
            case TK_Terminator:
                CreateToken(&arrTkn[iTkn], TK_Terminator, i, i + 1);
                i++;
                iTkn++;
                break;

            case TK_Hyphen:
            case TK_Equals:
            case TK_Quots:
                {
                    TokenKind kind = arrTknKind[i];
                    CreateToken(&arrTkn[iTkn], kind, i, i);
                    iTkn++;
                }
                break;

            case TK_Space:
            case TK_None:
                {
                    size_t s = i;
                    TokenKind kind = arrTknKind[i];
                    for (; i < strlen && arrTknKind[i] == kind; ++i) {}
                    i--;
                    CreateToken(&arrTkn[iTkn], kind, s, i);
                    iTkn++;
                }
                break;
        }
    }

    free(arrTknKind);

    *arrOutTkn = arrTkn;
}

#endif