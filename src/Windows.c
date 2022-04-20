#if defined(_WIN32) || defined(_WIN64)
#include "Utils.h"

HANDLE std_handle, active_handle;
CONSOLE_SCREEN_BUFFER_INFOEX std_info;

void Show_OS() { printf("WIN32 or WIN64\n"); }

//Creates a new console window big enough for the aplication
//Returns FALSE if failed, TRUE if successful
BOOL Init_Console_Buffer()
{
	std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleScreenBufferInfoEx(std_handle, &std_info))
	{
		printf("Unable to retrieve std console information\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}

	HANDLE mem_handle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,		//Read/Write access 
		FILE_SHARE_READ | FILE_SHARE_WRITE,	//Shared? 
		NULL,								//Inheritable by child?
		CONSOLE_TEXTMODE_BUFFER,			//Type of console 
		NULL								//NULL by default
	);

	if (mem_handle == INVALID_HANDLE_VALUE || std_handle == INVALID_HANDLE_VALUE)
	{
		printf("Error while creating console\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}

	COORD new_buffer_size =
	{
		WIN_WIDTH < std_info.dwSize.X ? std_info.dwSize.X : WIN_WIDTH,
		WIN_HEIGHT < std_info.dwSize.Y ? std_info.dwSize.Y : WIN_HEIGHT
	};
	if (!SetConsoleScreenBufferSize(mem_handle, new_buffer_size))
	{
		printf("Error while modifying buffer size\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	std_info.dwSize = new_buffer_size;

	if (!SetConsoleActiveScreenBuffer(mem_handle))
	{
		printf("Error while setting another active buffer\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	active_handle = mem_handle;

	SMALL_RECT window_size = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };
	if (!SetConsoleWindowInfo(mem_handle, TRUE, &window_size))
	{
		printf("Error while resizing the console\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}

	Redirect_IO();
	SetConsoleCursorPosition(active_handle, (COORD) { 0, 0 });

	return TRUE;
}

BOOL Change_Output_Color(unsigned int foreground, unsigned int background)
{
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console_handle == INVALID_HANDLE_VALUE)
	{
		printf("Unable to get std handle in Change_Output_Color\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	return SetConsoleTextAttribute(console_handle, foreground | background);
}

BOOL Return_To_Console()
{
	if (!CloseHandle(active_handle))
	{
		printf("Unable to close the active handle\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	if (!SetConsoleScreenBufferInfoEx(std_handle, &std_info))
	{
		printf("Error while setting screen buffer extended info\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	if (!SetConsoleActiveScreenBuffer(std_handle))
	{
		printf("Error while returning to std console\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	active_handle = std_handle;
	Redirect_IO();
	return TRUE;
}

BOOL Process_Input(char* mem)
{
	BOOL loop = TRUE;
	while(loop)
	{
		INPUT_RECORD console_input;
		LPDWORD events_read;
		if (!ReadConsoleInput(
			GetStdHandle(STD_INPUT_HANDLE),
			&console_input,
			1,
			&events_read))
		{
			printf("Unable to read from console\n");
			printf("Error: %lu\n", GetLastError());
		}

		if (console_input.Event.KeyEvent.bKeyDown)
		{
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			if (!GetConsoleScreenBufferInfo(active_handle, &buffer_info))
			{
				printf("Unable to read console information\n");
				printf("Error: %lu\n", GetLastError());
				return FALSE;
			}
			COORD new_pos = buffer_info.dwCursorPosition;
			switch (console_input.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_ESCAPE:
				if (!Return_To_Console())
				{
					printf("Unable to return no original console\n");
					return 1;
				}
				exit(1);
			case VK_RETURN:
				loop = FALSE;
			case VK_RIGHT:
				(buffer_info.dwCursorPosition.X - 5) % 9 == 0 ? new_pos.X++ : (new_pos.X += 8);
				if (new_pos.X > 141)
				{
					new_pos = (COORD){ 5, new_pos.Y + 2 * (buffer_info.dwCursorPosition.Y < 30) };
				}
				Set_Cursor_Pos(new_pos.X, new_pos.Y);
				break;
			case VK_LEFT:
				(buffer_info.dwCursorPosition.X - 6) % 9 == 0 ? new_pos.X-- : (new_pos.X -= 9);

				if (buffer_info.dwCursorPosition.X <= 5)
				{
					new_pos = (COORD){ 140, new_pos.Y - 2 * (buffer_info.dwCursorPosition.Y > 0) };
				}
				Set_Cursor_Pos(new_pos.X, new_pos.Y);
				break;
			case VK_UP:
				new_pos.Y > 0 ? (new_pos.Y -= 2) : (new_pos.Y = 30);
				Set_Cursor_Pos(new_pos.X, new_pos.Y);
				break;
			case VK_DOWN:
				new_pos.Y < 30 ? (new_pos.Y += 2) : (new_pos.Y = 0);
				Set_Cursor_Pos(new_pos.X, new_pos.Y);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				printf("%c", console_input.Event.KeyEvent.uChar.AsciiChar);
				(buffer_info.dwCursorPosition.X - 5) % 9 == 0 ? new_pos.X++ : (new_pos.X += 8);
				if (new_pos.X > 141)
				{
					new_pos = (COORD){ 5, new_pos.Y + 2 * (buffer_info.dwCursorPosition.Y < 30) };			
				}
				Set_Cursor_Pos(new_pos.X, new_pos.Y);
				break;
			default:
				break;
			}
		}
	}

	unsigned char pos = 0;
	CHAR_INFO char_buffer[2];
	SMALL_RECT area_to_read;
	for (unsigned int y = 0; y < 31; y+=2)
	{
		for (unsigned int x = 5; x < 142; x += 9)
		{
			area_to_read = (SMALL_RECT){ x, y, x+1, y };
			if(!ReadConsoleOutput(active_handle,	//Console Handle
				char_buffer,						//Address of the buffer
				(COORD) {2, 1},						//Size of buffer {cols, rows}
				(COORD) {0, 0},						//Coords of upper-left char to read
				&area_to_read						//Size of area to read
			))
			{
				printf("Unable to read console information\n");
				printf("Error: %lu\n", GetLastError());
				return FALSE;
			}
			char str[] = { char_buffer[0].Char.AsciiChar,  char_buffer[1].Char.AsciiChar };
			mem[pos++] = str_to_hex(str, 2);
		}
	}
	return TRUE;
}

BOOL Set_Cursor_Pos(unsigned int x, unsigned int y)
{
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	if (!GetConsoleScreenBufferInfo(active_handle, &buffer_info))
	{
		printf("Unable to read console information\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
	if (!SetConsoleCursorPosition(
		active_handle,
		(COORD) {
		Clamp(x, 5, WIN_WIDTH - 3), Clamp(y, 0, WIN_HEIGHT - 2)
	}))
	{
		printf("Unable to set cursor position\n");
		printf("Error: %lu\n", GetLastError());
		return FALSE;
	}
		return TRUE;
}

#endif

