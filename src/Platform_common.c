#include "Utils.h"
void Redirect_IO()
{
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONERR$", "w", stderr);
}

int Clamp(int n, int min, int max)
{
	return n < min ? min : n > max ? max : n;
}

unsigned int str_to_hex(const char* str, int size)
{
	unsigned int n = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		if (str[i] <= '9')
		{
			n += ((str[i] - '0') << 4 * (size - i - 1));
		}
		else if (str[i] <= 'F')
		{
			n += ((str[i] - 'A' + 10) << 4 * (size - i - 1));
		}
		else
		{
			n += ((str[i] - 'a' + 10) << 4 * (size - i - 1));
		}
	}
	return n;
}

unsigned int str_to_dec(const char* str, int size)
{
	unsigned int n = 0;
	for(unsigned int i = 0; i < size; i++)
	{
		n += (str[i]-'0')*_pow(10, size-i-1);
	}
	return n;
}

int _pow(int b, unsigned int exp)
{
    int tmp;
    if( exp == 0)
        return 1;
    tmp = _pow(b, exp / 2);
    if (exp % 2 == 0)
        return tmp * tmp;
    else
        return b * tmp * tmp;
}
