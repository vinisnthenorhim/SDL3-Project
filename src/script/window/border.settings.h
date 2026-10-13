#include <SDL3/SDL.h>


#ifdef _WIN32

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

constexpr int TITLE_H = 32, BTN_W = 46, RESIZE_B = 8;
static WNDPROC g_oldProc = nullptr;

LRESULT CALLBACK CustomProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_NCCALCSIZE:
        if (wp) {  // removes the native title bar and border
            auto* p = (NCCALCSIZE_PARAMS*)lp;
            if (IsZoomed(hwnd)) {
                // maximized windows overhang the screen edge, so pull them in
                int f = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
                p->rgrc[0].left += f;  p->rgrc[0].right  -= f;
                p->rgrc[0].top  += f;  p->rgrc[0].bottom -= f;
            }
            return 0;
        }
        break;

    case WM_NCHITTEST: {
        POINT pt{ GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
        ScreenToClient(hwnd, &pt);
        RECT rc; GetClientRect(hwnd, &rc);

        if (!IsZoomed(hwnd)) {
            bool l = pt.x < RESIZE_B, r = pt.x >= rc.right - RESIZE_B;
            bool t = pt.y < RESIZE_B, b = pt.y >= rc.bottom - RESIZE_B;
            if (t && l) return HTTOPLEFT;    if (t && r) return HTTOPRIGHT;
            if (b && l) return HTBOTTOMLEFT; if (b && r) return HTBOTTOMRIGHT;
            if (l) return HTLEFT;  if (r) return HTRIGHT;
            if (t) return HTTOP;   if (b) return HTBOTTOM;
        }
        if (pt.y < TITLE_H) {
            // leave the button area as client so SDL gets the mouse events
            if (pt.x >= rc.right - BTN_W * 3) return HTCLIENT;
            return HTCAPTION;  // drag + double-click maximize + Aero Snap
        }
        return HTCLIENT;
    }
    }
    return CallWindowProc(g_oldProc, hwnd, msg, wp, lp);
}

void installCustomFrame(SDL_Window* win) {
  HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(win),
                  SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
  if(!hwnd) return;

  g_oldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CustomProc);

  MARGINS m{ 0, 0, 1, 0 };            // brings back the drop shadow
  DwmExtendFrameIntoClientArea(hwnd, &m);
  COLORREF border = RGB(60, 60, 60);   
  DwmSetWindowAttribute(hwnd, 34 , &border, sizeof(border));
  SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
      SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}
void borderFocus(SDL_Window* win, const SDL_Event* event)
{
  HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(win),
                  SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
  if(!hwnd) return;
  if (event->type == SDL_EVENT_WINDOW_FOCUS_GAINED)
  {
    COLORREF border = RGB(0, 120, 215);   
    DwmSetWindowAttribute(hwnd, 34 , &border, sizeof(border));
  }

  if (event->type == SDL_EVENT_WINDOW_FOCUS_LOST)
  {
    COLORREF border = RGB(60, 60, 60);   
    DwmSetWindowAttribute(hwnd, 34 , &border, sizeof(border));
  }

}

void maximizeOrRestore(SDL_Window* win)
{
  HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(win),
                  SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

  if (!hwnd) return;

  WINDOWPLACEMENT placement{};
  placement.length = sizeof(placement);

  if (!GetWindowPlacement(hwnd, &placement)) return;

  WPARAM command = placement.showCmd == SW_SHOWMAXIMIZED ? SC_RESTORE : SC_MAXIMIZE;

  SendMessage(hwnd, WM_SYSCOMMAND, command, 0);
}
#endif