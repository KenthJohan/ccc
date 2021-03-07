/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once


enum csc_tok_c_type
{
	CSC_TOK_C_VOID = 256,
	CSC_TOK_C_CONST,
	CSC_TOK_C_INT,
	CSC_TOK_C_IDENTIFIER,
	CSC_TOK_C_LITERAL_INTEGER,
	CSC_TOK_C_IF
};


char const * tok_type_tostr (int t)
{
	switch (t)
	{
	case '\0': return "<EOF>";
	case '\r':  return "<CR>";
	case '\n': return "<LF>";
	case '{': return "{";
	case '}': return "}";
	case '(': return "(";
	case ')': return ")";
	case '*': return "*";
	case ':': return ":";
	case ',': return ",";
	case '?': return "?";
	case '+': return "+";
	case '-': return "-";
	case '/': return "/";
	case '<': return "<";
	case '>': return ">";
	case '|': return "|";
	case '^': return "^";
	case CSC_TOK_C_VOID: return "VOID";
	case CSC_TOK_C_CONST: return "CONST";
	case CSC_TOK_C_IDENTIFIER: return "IDENTIFIER";
	case CSC_TOK_C_INT: return "INT";
	case CSC_TOK_C_LITERAL_INTEGER: return "LITERAL_INTEGER";
	}
	return 0;
}
