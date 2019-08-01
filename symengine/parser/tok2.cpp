/* Generated by re2c 1.1.1 on Wed Jul 31 22:35:22 2019 */
#line 1 "tok2.re"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <istream>
#include <fstream>
#include <memory>

#include "parser.h"


enum num_t {
    // Unrecognized token. This is caused by a syntax error, unless there is a
    // bug in the tokenizer rules.
    ERR_UNKNOWN_TOKEN,

    // Null character \x00 encountered in the input file (we use it to
    // terminate). With a different SIZE, this Null character might be
    // mis-recognized as the END token. Solution: ensure the file does not
    // contain the null character.
    ERR_NULL,

    // The buffer input_t::buf is not big enough to hold `need` chars. The
    // `need <= YYMAXFILL`, where `YYMAXFILL` is the longest (sequence) of
    // tokens needed to be in the buffer simultaneously.
    // Solution: Increase SIZE.
    // Note: Whenever the buffer fills, the data (all the loaded tokens in the
    // buffer) gets moved in it and we start over. To ensure this happens
    // rarely, make SIZE large enough, for example 64*1024 (64 KB). The amount
    // of data to move is typically low, so this size should be large enough
    // not to be noticeable. One can run a benchmark study what size stops
    // being noticeable for a large file.
    ERR_BUF,

    // We reached the end of input.
    END,
    WS, OPERATOR, POW, LE, EQ, GE, IDENTIFIER, NUMERIC,
    IMPLICIT_MUL};

#define YYMAXFILL 3

static const size_t SIZE = 64*1024;

size_t sread(char *ptr, size_t size, size_t count, std::istream &stream)
{
    stream.read(ptr, count);
    return stream.gcount();
}

struct input_t {
    unsigned char buf[SIZE + YYMAXFILL];
    unsigned char *lim;
    unsigned char *cur;
    unsigned char *mar;
    unsigned char *tok;
    bool eof;

    std::istream &file;

    input_t(std::istream &f)
        : buf()
        , lim(buf + SIZE)
        , cur(lim)
        , mar(lim)
        , tok(lim)
        , eof(false)
        , file(f)
    {}
    bool fill(size_t need)
    {
        if (eof) {
            return false;
        }
        const size_t free = tok - buf;
        if (free < need) {
            return false;
        }
        memmove(buf, tok, lim - tok);
        lim -= free;
        cur -= free;
        mar -= free;
        tok -= free;
        lim += sread((char*)lim, 1, free, file);
        if (lim < buf + SIZE) {
            eof = true;
            memset(lim, 0, YYMAXFILL);
            lim += YYMAXFILL;
        }
        return true;
    }
};

static num_t lex(input_t &in)
{
    in.tok = in.cur;
    
#line 101 "tok2.cpp"
{
	char yych;
	if ((in.lim - in.cur) < 2) if (!in.fill(2)) return ERR_BUF;
	yych = *in.cur;
	switch (yych) {
	case 0x00:	goto yy2;
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\f':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '\'':
	case ':':
	case ';':
	case '?':
	case '[':
	case '\\':
	case ']':
	case '`':
	case '{':
	case '}':
	case 0x7F:	goto yy4;
	case '\t':
	case '\n':
	case '\v':
	case '\r':
	case ' ':	goto yy6;
	case '&':
	case '(':
	case ')':
	case '+':
	case ',':
	case '-':
	case '/':
	case '^':
	case '|':
	case '~':	goto yy9;
	case '*':	goto yy11;
	case '.':	goto yy12;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy13;
	case '<':	goto yy16;
	case '=':	goto yy17;
	case '>':	goto yy18;
	case '@':	goto yy19;
	default:	goto yy21;
	}
yy2:
	++in.cur;
#line 122 "tok2.re"
	{
                if (in.lim - in.tok == YYMAXFILL) {
                    return END;
                } else {
                    return ERR_NULL;
                }
            }
#line 194 "tok2.cpp"
yy4:
	++in.cur;
yy5:
#line 121 "tok2.re"
	{ return ERR_UNKNOWN_TOKEN; }
#line 200 "tok2.cpp"
yy6:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return ERR_BUF;
	yych = *in.cur;
	switch (yych) {
	case '\t':
	case '\n':
	case '\v':
	case '\r':
	case ' ':	goto yy6;
	default:	goto yy8;
	}
yy8:
#line 129 "tok2.re"
	{ return WS; }
#line 216 "tok2.cpp"
yy9:
	++in.cur;
yy10:
#line 131 "tok2.re"
	{ return OPERATOR; }
#line 222 "tok2.cpp"
yy11:
	yych = *++in.cur;
	switch (yych) {
	case '*':	goto yy19;
	default:	goto yy10;
	}
yy12:
	yych = *++in.cur;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy24;
	default:	goto yy5;
	}
yy13:
	++in.cur;
	if ((in.lim - in.cur) < 3) if (!in.fill(3)) return ERR_BUF;
	yych = *in.cur;
	switch (yych) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case ' ':
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '/':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '`':
	case '{':
	case '|':
	case '}':
	case '~':
	case 0x7F:	goto yy15;
	case '.':	goto yy26;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy13;
	case 'E':
	case 'e':	goto yy30;
	default:	goto yy27;
	}
yy15:
#line 137 "tok2.re"
	{ return NUMERIC; }
#line 331 "tok2.cpp"
yy16:
	yych = *++in.cur;
	switch (yych) {
	case '=':	goto yy31;
	default:	goto yy10;
	}
yy17:
	yych = *++in.cur;
	switch (yych) {
	case '=':	goto yy33;
	default:	goto yy5;
	}
yy18:
	yych = *++in.cur;
	switch (yych) {
	case '=':	goto yy35;
	default:	goto yy10;
	}
yy19:
	++in.cur;
#line 132 "tok2.re"
	{ return POW; }
#line 354 "tok2.cpp"
yy21:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return ERR_BUF;
	yych = *in.cur;
	switch (yych) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case ' ':
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '`':
	case '{':
	case '|':
	case '}':
	case '~':
	case 0x7F:	goto yy23;
	default:	goto yy21;
	}
yy23:
#line 136 "tok2.re"
	{ return IDENTIFIER; }
#line 430 "tok2.cpp"
yy24:
	++in.cur;
	if ((in.lim - in.cur) < 3) if (!in.fill(3)) return ERR_BUF;
	yych = *in.cur;
yy25:
	switch (yych) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case ' ':
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '`':
	case '{':
	case '|':
	case '}':
	case '~':
	case 0x7F:	goto yy15;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy24;
	case 'E':
	case 'e':	goto yy30;
	default:	goto yy27;
	}
yy26:
	yych = *++in.cur;
	switch (yych) {
	case 'E':
	case 'e':	goto yy27;
	default:	goto yy25;
	}
yy27:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return ERR_BUF;
	yych = *in.cur;
yy28:
	switch (yych) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case ' ':
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '`':
	case '{':
	case '|':
	case '}':
	case '~':
	case 0x7F:	goto yy29;
	default:	goto yy27;
	}
yy29:
#line 138 "tok2.re"
	{ return IMPLICIT_MUL; }
#line 599 "tok2.cpp"
yy30:
	yych = *(in.mar = ++in.cur);
	switch (yych) {
	case '+':
	case '-':	goto yy37;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy39;
	default:	goto yy28;
	}
yy31:
	++in.cur;
#line 133 "tok2.re"
	{ return LE; }
#line 621 "tok2.cpp"
yy33:
	++in.cur;
#line 135 "tok2.re"
	{ return EQ; }
#line 626 "tok2.cpp"
yy35:
	++in.cur;
#line 134 "tok2.re"
	{ return GE; }
#line 631 "tok2.cpp"
yy37:
	yych = *++in.cur;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy39;
	default:	goto yy38;
	}
yy38:
	in.cur = in.mar;
	goto yy29;
yy39:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return ERR_BUF;
	yych = *in.cur;
	switch (yych) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case 0x0E:
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
	case 0x1F:
	case ' ':
	case '!':
	case '"':
	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '`':
	case '{':
	case '|':
	case '}':
	case '~':
	case 0x7F:	goto yy15;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy39;
	default:	goto yy27;
	}
}
#line 139 "tok2.re"

}

std::unique_ptr<input_t> in;
using SymEngine::Parser;
SymEngine::ParserBase::STYPE__ *dval;

void yy_scan_stream(std::istream &stream)
{
    in = std::make_unique<input_t>(stream);
}

int yylex()
{
    for (;;) {
        num_t t = lex(*in);
        switch (t) {
            case END: return 0;
            case ERR_BUF: printf("ERR BUF.\n"); return 0;
            case ERR_NULL: printf("ERR NULL.\n"); return 0;
            case ERR_UNKNOWN_TOKEN: printf("ERR unknown token\n"); return 0;
            case WS: break;
            case OPERATOR:
                return in->tok[0];
            case POW: return Parser::POW;
            case LE: return Parser::LE;
            case EQ: return Parser::EQ;
            case GE: return Parser::GE;
            case IDENTIFIER:
                *dval = std::string((char*)in->tok, in->cur-in->tok);
                return Parser::IDENTIFIER;
            case NUMERIC:
                *dval = std::string((char*)in->tok, in->cur-in->tok);
                return Parser::NUMERIC;
            case IMPLICIT_MUL:
                *dval = std::string((char*)in->tok, in->cur-in->tok);
                return Parser::IMPLICIT_MUL;
        }
    }
}
