#include <windows.h>
#include <stdint.h>

#define global_variable static
#define Internal static

global_variable bool Running;
global_variable int clientwidth;
global_variable int clientheight;
global_variable void *bitmapmemory;
global_variable BITMAPINFO bitmapinfo;

LRESULT CALLBACK win32wnhndle(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
Internal void win32resizedibsection(int width, int height);
Internal void win32updatewindow(HDC devcontext, LPRECT rect, int x, int y, int width, int height);
Internal void populatebitmap(int xofsite, int yofsite);



int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE hprevinstant, LPSTR cmdline, int cmdshow)
{
    WNDCLASSA wnclass = {};
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

                populatebitmap(xofsite,yofsite);
                
                RECT rect;
                GetClientRect(window,&rect);
                HDC devcontext = GetDC(window);
                win32updatewindow(devcontext,&rect, 0,0, clientwidth, clientheight);
                ReleaseDC(window,devcontext);
                ++xofsite;
                ++yofsite;

            }
        }
    }
    return 0;
}

LRESULT CALLBACK win32wnhndle(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (message)
    {
    case WM_SIZE:
    {
        RECT clientrect;
        GetClientRect(window,&clientrect);
        int width = clientrect.right - clientrect.left;
        int height = clientrect.bottom - clientrect.top;
        clientheight = height;
        clientwidth = width;
        win32resizedibsection(width, height);
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
        RECT clientrect;
        GetClientRect(window,&clientrect);

        PAINTSTRUCT paint;
        HDC devcontext = BeginPaint(window,&paint);
        int x = paint.rcPaint.left;
        int y = paint.rcPaint.top;
        int width = paint.rcPaint.right - paint.rcPaint.left;
        int height = paint.rcPaint.bottom - paint.rcPaint.top;
        win32updatewindow(devcontext, &clientrect,x,y,width,height);
        EndPaint(window,&paint);
    }break;
    
    default:
    {
        return DefWindowProcA(window,message, wparam,lparam);
    }break;
        
    }return (result);
}

Internal void win32resizedibsection(int width, int height)
{
    if (bitmapmemory)
    {
        VirtualFree(bitmapmemory,0,MEM_RELEASE);
    }

    bitmapinfo.bmiHeader.biSize = sizeof(bitmapinfo.bmiHeader);
    bitmapinfo.bmiHeader.biWidth = width;
    bitmapinfo.bmiHeader.biHeight = -height;
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biCompression = BI_RGB;
    bitmapinfo.bmiHeader.biBitCount = 32;

    int memorysize = width * height *4;

    bitmapmemory = VirtualAlloc(0,memorysize,MEM_COMMIT,PAGE_READWRITE);
   

}
Internal void win32updatewindow(HDC devcontext, LPRECT rect, int x, int y, int width, int height)
{
    int btmpwidth = rect->right - rect->left;
    int btmpheigth = rect->bottom - rect->top;

    StretchDIBits(devcontext,
        0,0,clientwidth,clientheight,
        0,0,clientwidth,clientheight, 
        bitmapmemory,&bitmapinfo, 
        DIB_RGB_COLORS,SRCCOPY);
}

Internal void populatebitmap(int xofsite, int yofsite)
{
    int pitch = clientwidth*4;
    uint8_t *row = (uint8_t *)bitmapmemory;    
    for (int y = 0; y<clientheight; y++)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int x =0; x < clientwidth; x++)
        {
            uint8_t blue = (x + xofsite);
            uint8_t green = (y + yofsite);
            *pixel++ = ((green << 8)|blue);
        }
        row += pitch;
    }
}