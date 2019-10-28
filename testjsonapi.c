
#include "stdio.h"

int tolower(int c);
int toupper(int c);
double strtod(const char *str,char **endptr);

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
int main (int argc, char **argv)
{
	int size = 0;
	char *str = argv[1];
	char *hex = argv[2];
	char *end;
	size = strlen(str);
	int i = 0;
	for(i=0;i<size;i++) {
		printf("%c", tolower(str[i]));
	}
	printf("\n");
	for(i=0;i<size;i++) {
		printf("%c", toupper(str[i]));
	}
	printf("\n");
	printf("%lf\n", strtod(str, &end));
	printf("%s\n", end);

	return 0;
}