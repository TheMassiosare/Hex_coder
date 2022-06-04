#include "Utils.h"
#include <string.h>
#pragma warning(disable : 4996)

int main(int argc, char** argv)
{
	/* TODO:
		* Let the user select the targeted EEPROM (only 2764 for now)
	*/

	char filename[128] = "a";
	unsigned char default_val = 0xFF;
	unsigned char t_color = t_green;
	for (unsigned int i = 1; i < argc; i++)
	{
		if(strlen(argv[i]) < 2 || (i + 2 > argc))
		{
			Change_Output_Color(t_red, b_reset);
			printf("Invalid input argument: ");
			Change_Output_Color(t_reset, b_reset);
			printf("missing specification\n");
			return 1;
		}

		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'o':
			case 'O':
				if (strlen(argv[++i]) >= 128)
				{
					Change_Output_Color(t_red, b_reset);
					printf("Input error: ");
					Change_Output_Color(t_reset, b_reset);
					printf("File name too long\n");
					return 1;
				}
				strcpy(filename, argv[i]);
				break;
			case 'd':
			case 'D':
				if (strlen(argv[++i]) > 3 || str_to_dec(argv[i], strlen(argv[i])) > 255)
				{
					Change_Output_Color(t_red, b_reset);
					printf("Input error: ");
					Change_Output_Color(t_reset, b_reset);
					printf("default decimal value too large\n");
					return 1;
				}
				default_val = (unsigned char)str_to_dec(argv[i], strlen(argv[i]));
				break;
			case 'h':
			case 'H':
				if (strlen(argv[++i]) > 2)
				{
					Change_Output_Color(t_red, b_reset);
					printf("Input error: ");
					Change_Output_Color(t_reset, b_reset);
					printf("default hex value too large\n");
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
				Change_Output_Color(t_red, b_reset);
				printf("Invalid input argument: ");
				Change_Output_Color(t_reset, b_reset);
				printf("unknown command\n");
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
		Change_Output_Color(t_color, b_reset);
		printf("%02X  ", default_val);
		Change_Output_Color(t_reset, b_reset); 
		if ((i & 0x0F) == 0x0F) printf("\n\n");
	}
	Change_Output_Color(t_color, b_reset);
	Set_Cursor_Pos(5, 0);

	unsigned char mem[256];
	for(i = 0; i < 256; i++)
		mem[i] = default_val;
	
	if (!Process_Input(mem))
	{
		Return_To_Console();
		printf("Unable to process input\n");
		printf("No files were generated\n");
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