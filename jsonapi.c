
/* disable warnings about old C89 functions in MSVC */
#if !defined(_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif
#if defined(_MSC_VER)
#pragma warning (push)
/* disable warning about single line comments in system headers */
#pragma warning (disable : 4001)
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

#ifdef ENABLE_LOCALES
#include <locale.h>
#endif

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
#ifdef __GNUC__
#pragma GCC visibility pop
#endif

#include "cJSON.h"


CJSON_PUBLIC(int) tolower(int c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return c + ('a' - 'A');
	return c;
}
 
CJSON_PUBLIC(int) toupper(int c)
{
	if ((c >= 'a') && (c <= 'z'))
		return c + ('A' - 'a');
	return c;
}

extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
extern double strtod(const char *str,char **endptr);
double strtod(const char *str,char **endptr) 
{ 
	const char* p = str;
	double  value = 0.L;
	int sign  = 0;
	double direct = 10;
	double  factor;
	int i = 0;
	while ( *str==' ' )
		p++;

	if(*p == '-' || *p == '+')
		sign = *p++;
	while ( *p<='9'&&*p>='0' ) {
		value *= 10;
		value = value + (*p - '0');
		p++;
	}

	if ( *p == '.' ) 
	{
		factor = 1;
		p++;
		while ( *p<='9'&&*p>='0' ) 
		{
			factor *= 0.1;
			value  += (*p++ - '0') * factor;
		}
	}
   if(*p=='e'||*p=='E')
   {    
		p++;
		switch(*p)
		{
			case '-':
				direct=0.1;
				p++;
				break;
			case '+':
				p++;
			default:
				break;
		}
		if(*p<='9'&&*p>='0') {
			char *end;
			long unsigned int loop = simple_strtoul(p, &end, 8);
			for(i=0;i<loop;i++) {
				value *= direct;
			}
			p = end;
		}
	}
	*endptr = (char *) p;
	return (sign == '-' ? -value : value);
}