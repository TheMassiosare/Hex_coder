#include "Utils.h"
#pragma warning(disable : 4996)

int main(int argc, char** argv)
{
	/* TODO:
		* Design for Linux
		* Let the user select the targeted EEPROM (only 2764 for now)
		* Bullet proof input commands
	*/

	char filename[128] = "a";
	unsigned char default_val = 0xFF;
	unsigned char t_color = t_green;
	for (unsigned int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'o':
			case 'O':
				if (strlen(argv[++i]) >= 128)
				{
					printf("File name too long\n");
					return 1;
				}
				strcpy(filename, argv[i]);
				break;
			case 'd':
			case 'D':
				if (strlen(argv[++i]) > 2)
				{
					printf("Default hex value too large");
					return 1;
				}
				default_val = str_to_hex(argv[i], 2);
				break;
			case 'c':
			case 'C':
				i++;
				if (!strcmp(argv[i], "red"))
					t_color = t_red;
				else if (!strcmp(argv[i], "blue"))
					t_color = t_blue;
				else
					t_color = t_green;
				break;
			default:
				printf("Invalid input argument\n");
				return 1;
				break;
			}
		}
	}

	if (!Init_Console_Buffer())
	{
		printf("Unable to create application window\n");
		return 1;
	}

	unsigned short i;
	for (i = 0; i < 256; i++)
	{
		printf("%02X:  ", i);
		Change_Output_Color(t_color, 0);
		printf("%02X  ", default_val);
		Change_Output_Color(t_blue | t_green | t_red, 0); 
		if ((i & 0x0F) == 0x0F) printf("\n\n");
	}

	Change_Output_Color(t_color, 0);
	Set_Cursor_Pos(0, 0);
	while (Process_Input() == TRUE);
	
	unsigned char mem[256];
	if (!Mem_Fill(mem))
	{
		printf("Error while writting to memory\n");
		return 1;
	}

	if (!Return_To_Console())
	{
		printf("Unable to return no original console\n");
		return 1;
	}

	sprintf(filename, "%s.hex", filename);
	FILE* out = fopen(filename, "w");
	if (!out)
	{
		printf("Eror creando el archivo");
		return -1;
	}

	fprintf(out, ":020000040000FA");

	unsigned char checksum = 0;
	i = 0;
	while (i < 256)
	{
		if (i % 0x10 == 0)
		{
			printf("\n:1000%02X00 ", ((i / 0x10) * 0x10));
			fprintf(out, "\n:1000%02X00", ((i / 0x10) * 0x10));
			checksum = 0;
		}
		printf("%02X ", mem[i]);
		fprintf(out, "%02X", mem[i]);
		checksum += mem[i];
		if ((i & 0x0F) == 0x0F)
		{
			checksum += 16 + (i / 0x10) * 0x10;
			printf("%02X ", (unsigned char)(256 - checksum % 256));
			fprintf(out, "%02X", (unsigned char)(256 - checksum % 256));
		}
		i++;
	}

	fprintf(out, "\n:00000001FF");
	printf("\nFile saved\n");
	fclose(out);

	return 0;
}