#ifdef __linux__
#include "Utils.h"

struct coord {
    short x, y;
};

struct coord pos = {0, 0}; 

void Show_OS()
{
    printf("Linux");
}

int Init_Console_Buffer()
{
    Create_terminal();
    struct winsize size;
    Get_window_size(&size); 
    Set_Cursor_Pos(0,0);
    if(size.ws_col < WIN_WIDTH || size.ws_row < WIN_HEIGHT)
    {
        Return_To_Console();
        printf("Unable to start application\n");
		printf("Please resize your terminal such that it has at least %i columns and %i rows\n", WIN_WIDTH+1, WIN_HEIGHT+1);
        return 0;
    }    
    struct winsize new_size = (struct winsize){WIN_HEIGHT, WIN_WIDTH, size.ws_xpixel, size.ws_ypixel};
    Set_window_size(&new_size);
    pos = (struct coord){5, 0};
    return 1;
}

int Return_To_Console()
{
    Return_to_terminal();
    Change_Output_Color(t_reset, b_reset);
    return 1;
}

int Process_Input(char* mem)
{
    while(1)
    {
        char c = getch(); 
        switch (c)
        {
        case 27: /* ESC */
            Return_To_Console();
            printf("Process aborted\n");
            exit(1);
        case 10: /* ENTER */
            return 0;
        case 61: /* UP */
            pos.y > 0 ? (pos.y -= 2) : (pos.y = 30);
            Set_Cursor_Pos(pos.x, pos.y);
            break;
        case 62: /* DOWN */
            pos.y < 30 ? (pos.y += 2) : (pos.y = 0);
            Set_Cursor_Pos(pos.x, pos.y);
            break;
        case 63: /* RIGHT */
            (pos.x - 5) % 9 == 0 ? pos.x++ : (pos.x += 8);
            if (pos.x > 141)
            {
                pos = (struct coord){ 5, pos.y + 2 * (pos.y < 30) };
            }
            Set_Cursor_Pos(pos.x, pos.y);
            break;
        case 64: /* LEFT */
            (pos.x - 6) % 9 == 0 ? pos.x-- : (pos.x -= 8);
            if (pos.x < 5)
            {
                pos = (struct coord){ WIN_WIDTH-4, pos.y - 2 * (pos.y > 0) };
            }
            Set_Cursor_Pos(pos.x, pos.y);
            break;    
        default:
            if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                printf("%c", c);
                unsigned char index = (pos.x-5)/9 + pos.y/2*0x10;
                if((pos.x-5)%9)
                {
                    mem[index] &= 0xF0;
                    mem[index] |= str_to_hex(&c, 1);
                }
                else
                {
                    mem[index] &= 0x0F;
                    mem[index] |= str_to_hex(&c, 1) << 4;
                }
                (pos.x - 5) % 9 == 0 ? pos.x++ : (pos.x += 8);
                if (pos.x > 141)
                {
                    pos = (struct coord){ 5, pos.y + 2 * (pos.y < 30) };			
                }

                Set_Cursor_Pos(pos.x, pos.y);
            }
            break;
        }
    }

    return 1;
}

int getch()
{
    int c;
    //Save old terminal state
    struct termios og, new; 
    tcgetattr(0, &og);
    new = og;
    //Set new terminal state to non canon and no echo
    new.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(0, TCSANOW, &new);
    c = getchar();
    if(c == 27)
    {
        if(getchar() == 91) 
            c = getchar()-4;
    }
    //Restor terminal settings
    tcsetattr(0, TCSANOW, &og);
    return c;
}

#endif