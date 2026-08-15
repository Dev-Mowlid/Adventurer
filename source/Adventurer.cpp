#include <windows.h>
#include <stdint.h>
#include <xinput.h>

#define global_variable static
#define Internal static

// global_variable int yofsite;


#define X_INPUT_GET_STATE(name) DWORD name(DWORD dwUserIndex,XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStab)
{
    return(0);
}
global_variable x_input_get_state *xinputgetstate_ = XInputGetStateStab;
#define XInputGetState xinputgetstate_

#define X_INPUT_SET_STATE(name) DWORD name(DWORD dwUserIndex, XINPUT_VIBRATION  *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStab)
{
    return(0);
}
global_variable x_input_set_state *xinputsetstate_ = XInputSetStateStab;
#define XInputSetState xinputsetstate_

global_variable bool Running;
struct windimension
{
    int width;
    int height;
};
struct winbuffer
{
    int width;
    int height;
    void *memory;
    BITMAPINFO info;
};
global_variable winbuffer global_buffer;

Internal windimension getwindimension(HWND window);
LRESULT CALLBACK win32wnhndle(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
Internal void win32resizedibsection(winbuffer *buffer, int width, int height);
Internal void win32updatewindow(HDC devcontext,winbuffer *buffer, int x, int y, int width, int height);
Internal void populatebitmap(winbuffer *buffer, int xofsite, int yofsite);
Internal void loadXinput(void);



int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE hprevinstant, LPSTR cmdline, int cmdshow)
{
    WNDCLASSA wnclass = {};
    win32resizedibsection(&global_buffer,1000,620);
    loadXinput();
    wnclass.style = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW;
    wnclass.lpfnWndProc = win32wnhndle;
    wnclass.lpszClassName = "handmadehero";
    wnclass.hInstance = Instance;

    if (RegisterClassA(&wnclass))
    {
        HWND window = CreateWindowExA(
            0,
            wnclass.lpszClassName,
            "handmadehero",
            WS_VISIBLE|WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
            0,0,Instance,0

        );
        if (window)
        {
            HDC devcontext = GetDC(window);
            int xofsite = 0;
            int yofsite = 0;
            Running = true;
            while(Running)
            {
                MSG message = {};
                while(PeekMessageA(&message,0,0,0,PM_REMOVE))
                {
                    if (message.message == WM_QUIT)
                    {
                        Running = false;
                    }
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }

                for (DWORD controllerindex; controllerindex < XUSER_MAX_COUNT; controllerindex++)
                {
                    XINPUT_STATE cntrlstate;
                    if (XInputGetState(controllerindex,&cntrlstate) == ERROR_SUCCESS)
                    {
                        XINPUT_GAMEPAD *pad = &cntrlstate.Gamepad;
                
                        bool up = pad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
                        bool DOWN = pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
                        bool LEFT = pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
                        bool RIGHT = pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
                        bool START = pad->wButtons & XINPUT_GAMEPAD_START;
                        bool BACK = pad->wButtons & XINPUT_GAMEPAD_BACK;
                        bool LEFT_SHOULDER = pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
                        bool RIGHT_SHOULDER = pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
                        bool AButton = pad->wButtons & XINPUT_GAMEPAD_A;
                        bool BButton = pad->wButtons & XINPUT_GAMEPAD_B;
                        bool XButton = pad->wButtons & XINPUT_GAMEPAD_X;
                        bool YButton = pad->wButtons & XINPUT_GAMEPAD_Y;

                        int16_t lstickx = pad->sThumbLX;
                        int16_t lsticky = pad->sThumbLY;
                    }
                    else
                    {
                        //controller not connected
                    }
                }

                windimension dimension = getwindimension(window);
                populatebitmap(&global_buffer,xofsite,yofsite);
                win32updatewindow(devcontext,&global_buffer,0,0, dimension.width, dimension.height);
                ++xofsite;
                ++yofsite;

            }
        }
    }
    return(0);
}

LRESULT CALLBACK win32wnhndle(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (message)
    {

    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_KEYDOWN:
    {
        uint32_t vkcode = wparam;
        if (vkcode == 'W')
        {
            // yofsite += 20;
        }
    }break;
    case WM_SIZE:
    {
    }break;
    case WM_CLOSE:
    {
        Running = false;
    }break;
    case WM_DESTROY:
    {
        Running = false;
    }break;
    case WM_ACTIVATEAPP:
    {

    }break;
    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        HDC devcontext = BeginPaint(window,&paint);
        windimension dimension = getwindimension(window);
        win32updatewindow(devcontext,&global_buffer,0,0,dimension.width,dimension.height);
        EndPaint(window,&paint);
    }break;
    
    default:
    {
        return DefWindowProcA(window,message, wparam,lparam);
    }break;
        
    }return (result);
}

Internal void win32resizedibsection(winbuffer *buffer, int width, int height)
{
    if (buffer->memory)
    {
        VirtualFree(buffer->memory,0,MEM_RELEASE);
    }
    buffer->width = width;
    buffer->height = height;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = width;
    buffer->info.bmiHeader.biHeight = -height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    buffer->info.bmiHeader.biBitCount = 32;

    int memorysize = buffer->width * buffer->height *4;

    buffer->memory = VirtualAlloc(0,memorysize,MEM_COMMIT,PAGE_READWRITE);
   

}
Internal void win32updatewindow(HDC devcontext, winbuffer *buffer, int x, int y, int width, int height)
{

    StretchDIBits(devcontext,
        0,0,width,height,
        0,0,buffer->width,buffer->height, 
        buffer->memory,&buffer->info, 
        DIB_RGB_COLORS,SRCCOPY);
}

Internal void populatebitmap(winbuffer *buffer, int x_ofsite, int y_ofsite)
{
    int pitch = buffer->width * 4;
    uint8_t *row = (uint8_t *)buffer->memory;    
    for (int y = 0; y<buffer->height; y++)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int x =0; x < buffer->width; x++)
        {
            uint8_t blue = (x + x_ofsite);
            uint8_t green = (y + y_ofsite);
            *pixel++ = ((green << 8)|blue);
        }
        row += pitch;
    }
}

Internal windimension getwindimension(HWND window)
{
    RECT rect;
    GetClientRect(window, &rect);
    windimension dimension;
    dimension.width = rect.right - rect.left;
    dimension.height = rect.bottom - rect.top;
    return(dimension);
}

Internal void loadXinput(void)
{
    HMODULE Xinputlibrary = LoadLibraryA("xinput1_3.dll");
    if (Xinputlibrary)
    {
        XInputGetState = (x_input_get_state *)GetProcAddress(Xinputlibrary,"XInputGetState");
        XInputSetState = (x_input_set_state *)GetProcAddress(Xinputlibrary, "XInputSetState");
    }
}
    