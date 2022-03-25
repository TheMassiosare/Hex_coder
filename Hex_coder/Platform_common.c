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

unsigned int str_to_hex(char* arr, int size)
{
	unsigned int n = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		if (arr[i] <= '9')
		{
			n += ((arr[i] - '0') << 4 * (size - i - 1));
		}
		else if (arr[i] <= 'F')
		{
			n += ((arr[i] - 'A' + 10) << 4 * (size - i - 1));
		}
		else
		{
			n += ((arr[i] - 'a' + 10) << 4 * (size - i - 1));
		}
	}
	return n;
}
