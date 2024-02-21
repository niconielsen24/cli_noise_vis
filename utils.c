#include <windows.h>

#ifdef _WIN32

    /**
    *check_console_window_resize_event()
    *
    *2022.10.05 - Created by Greg Spears and placed in the public domain.
    *           - Tested -- use at your own risk.
    *
    *Params: none
    *Returns: TRUE if the console window has changed size.  FALSE if not.
    *
    *USAGE: Best practice is to call the function repeatedly from your main application 
    *loop.   Preferably a place where the function can be called several times per second 
    */


    int check_console_window_resize_event(void){
    /* variables declared static hold their value between function calls.*/
    static int old_screen_w=0, old_screen_h=0;
    static HANDLE hConOut=NULL;
    int current_screen_w, current_screen_h;
    int window_resized = FALSE;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SECURITY_ATTRIBUTES sa;
    if(!hConOut)
    {
        /* First call -- get the window handle one time and save it*/
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        /* Using CreateFile we get the true console handle, avoiding any redirection.*/
        hConOut = CreateFile( TEXT("CONOUT$"),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            &sa, OPEN_EXISTING, (DWORD)0, (HANDLE)0 );
    }
    if(!hConOut) /* actually, this is a bad error, let your app handle the error as needed*/
        return FALSE;
    GetConsoleScreenBufferInfo( hConOut, &csbi );
    current_screen_w  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    current_screen_h  = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    if(!old_screen_w && !old_screen_h)
    {
        /* Execution comes here if this is first time this function is called.  
        ** Initialize the static variables and bail...*/
        old_screen_w = current_screen_w;
        old_screen_h = current_screen_h;
        return FALSE;
    }
    /* At last the real work of this function can be realized...*/
    if(current_screen_w != old_screen_w || current_screen_h != old_screen_h)
    {
        old_screen_w = current_screen_w;
        old_screen_h = current_screen_h;
        window_resized = TRUE;
    }
    return window_resized;
    }

#endif

#ifdef __linux__
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>

// these functions arent tested and i do not really know if they work correctly

bool get_key_state() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    int ch = getch();
    endwin();

    return ch == 27;
}

int check_console_window_resize_event(void){
    static struct winsize prev_size = {0, 0, 0, 0};
    struct winsize current_size;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &current_size);

    if (current_size.ws_row != prev_size.ws_row || current_size.ws_col != prev_size.ws_col) {
        prev_size = current_size;
        return true;
    }

    return false;
}
#endif

double map(double value, double fromMin, double fromMax, double toMin, double toMax) {
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

float clamp(float val) {
    return min(max(0.0, val),1.0);
}

