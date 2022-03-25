#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
enum console_colors
{
	t_blue = FOREGROUND_BLUE,
	t_green = FOREGROUND_GREEN,
	t_red = FOREGROUND_RED,
	t_intense = FOREGROUND_INTENSITY,
	b_blue = BACKGROUND_BLUE,
	b_green = BACKGROUND_GREEN,
	b_red = BACKGROUND_RED,
	b_intense = BACKGROUND_INTENSITY
};
#elif __linux__
//include Linux library
#endif

//Redirects IO to current console
void Redirect_IO();
//Shows working OS
void Show_OS();
//Creates a console for the app
int Init_Console_Buffer();
//Changes the background and foreground color
int Change_Output_Color(unsigned int foreground, unsigned int background);
//Gives back controll to the std console
int Return_To_Console();
int Process_Input();
int Set_Cursor_Pos(unsigned int x, unsigned int y);
int Clamp(int n, int min, int max);
int Mem_Fill(unsigned char* mem);
unsigned int str_to_hex(char* arr, int size);

