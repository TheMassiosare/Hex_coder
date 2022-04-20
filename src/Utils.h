#include <stdio.h>
#define WIN_WIDTH 145
#define WIN_HEIGHT 32

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

int Set_Cursor_Pos(unsigned int x, unsigned int y);
//Changes the background and foreground color
int Change_Output_Color(unsigned int foreground, unsigned int background);

enum console_colors
{
	t_blue = FOREGROUND_BLUE,
	t_green = FOREGROUND_GREEN,
	t_red = FOREGROUND_RED,
	t_reset = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
	b_blue = BACKGROUND_BLUE,
	b_green = BACKGROUND_GREEN,
	b_red = BACKGROUND_RED,
	b_reset = 0
};

#elif __linux__

#include <sys/ioctl.h>  //Input/Output control 
#include <stdlib.h>
#include <termios.h>
#define Set_Cursor_Pos(x,y) printf("\033[%i;%iH", Clamp(y+1, 1, WIN_HEIGHT-1), Clamp(x+1, 1, WIN_WIDTH-3))
#define Create_terminal() printf("\033[?1049h\033[h")
#define Return_to_terminal() printf("\033[?1049l")
#define Get_window_size(p_wsize) ioctl(1, TIOCGWINSZ, p_wsize)
#define Set_window_size(p_wsize) ioctl(1, TIOCSWINSZ, p_wsize)
#define Change_Output_Color(foreground, background) printf("\033[%i;%im", foreground, background)

int getch();

enum console_colors
{
	t_blue = 34,
	t_green = 32,
	t_red = 31,
	t_reset = 0, 
	b_blue = 44,
	b_green = 42,
	b_red = 41,
	b_reset = 49
};

#endif

//Redirects IO to current console
void Redirect_IO();
//Shows working OS
void Show_OS();
//Creates a console for the app
int Init_Console_Buffer();
//Gives back controll to the std console
int Return_To_Console();
int Process_Input(char* mem);
int Clamp(int n, int min, int max);
unsigned int str_to_hex(const char* str, int size);
unsigned int str_to_dec(const char* str, int size);
int _pow(int b, unsigned int exp);
