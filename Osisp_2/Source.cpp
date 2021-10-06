#include <windows.h>
#include <string>
#include <comdef.h>
#include <vector>
using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int windowW;
int windowH;
int columnW, rowH;
int countOfColumn, countOfRow;
vector <vector <const wchar_t*> > ivector = {{L"1",L"2",L"3",L"4"},
               {L"5",L"6",L"7",L"8"},
               {L"9",L"10",L"11",L"12"}};
const wchar_t* buf[3][4] = { {L"Hello,world",L"Hello,world",L"programming",L"planet"},{L"word",L"earth",L"don't",L"know"},
                            {L"tree",L"project",L"reality",L"window"} };

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Window Class";
    WNDCLASS windowClass = { };

    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = CLASS_NAME;
    int size = ivector.size();
    countOfRow = ivector[0].size()-1;
    countOfColumn = size;

    RegisterClass(&windowClass);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Osisp_2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 1;
    }
  
    ShowWindow(hwnd, nCmdShow);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    windowW= clientRect.right;
    windowH = clientRect.bottom;
    columnW = windowW / countOfColumn;
   
    rowH = windowH / (countOfRow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    free(&clientRect);
    return 0;
}

void fillWindow(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    columnW = windowW / countOfColumn;
    rowH = windowH / countOfRow;
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, pen);
    HFONT font = CreateFont(rowH, columnW/14, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, VARIABLE_PITCH, NULL);
    SelectObject(hdc, font);
    for (int i = 0; i < countOfColumn; i++) {
        for (int j = 0; j < countOfRow; j++) {

            RECT cell;
            SetRect(&cell, columnW * j, rowH * i, columnW * (j + 1), rowH * (i + 1));
            SetBkMode(hdc, TRANSPARENT);
            Rectangle(hdc, columnW * j, rowH * i, columnW * (j + 1), rowH * (i + 1));

            DrawText(hdc, buf[i][j], -1, &cell, DT_CENTER);
            SetBkMode(hdc, 10);
        }
    }
    DeleteObject(font);
    DeleteObject(pen);
    EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        exit(0);
        return 0;
    }
    case WM_PAINT:
    {
        fillWindow(hwnd, uMsg, wParam, lParam);
        return 0;
    }
    case WM_SIZE: 
    {
        windowW = LOWORD(lParam);
        windowH = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}