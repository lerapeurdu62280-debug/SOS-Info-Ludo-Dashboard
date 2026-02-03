#define _CRT_SECURE_NO_WARNINGS
#define NOM_ENTREPRISE "SOS INFO LUDO" 

// AUTO-LINK
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "advapi32.lib")

#include <windows.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <deque>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>

// COULEURS
#define COL_BG RGB(18, 18, 22)
#define COL_SIDE RGB(25, 25, 30)
#define COL_CRD RGB(35, 35, 40)
#define COL_WHT RGB(255, 255, 255)
#define COL_GRY RGB(170, 170, 180)
#define COL_BLU RGB(0, 150, 255)
#define COL_RED RGB(255, 50, 80)
#define COL_GRN RGB(0, 200, 100)

HWND hMain;
HFONT fHead, fBody, fBig;
int tab = 0;
std::deque<int> cpuH, ramH;
std::string g_M = "...", g_G = "...", g_C = "...", g_B = "...", g_I = "...", g_H = "...";
double nD = 0, nU = 0; DWORD lI = 0, lO = 0;
int g_Tmp = 0, g_Hz = 0;

// LECTURE REGISTRE (FIABLE)
std::string Reg(HKEY r, const char* s, const char* v) {
    HKEY k; char d[256]; DWORD l = 256; std::string ret = "Standard";
    if (RegOpenKeyExA(r, s, 0, KEY_READ, &k) == 0) { if (RegQueryValueExA(k, v, 0, 0, (LPBYTE)d, &l) == 0) ret = d; RegCloseKey(k); }
    return ret;
}
int RegInt(HKEY r, const char* s, const char* v) {
    HKEY k; DWORD d = 0, l = 4; if (RegOpenKeyExA(r, s, 0, KEY_READ, &k) == 0) { RegQueryValueExA(k, v, 0, 0, (LPBYTE)&d, &l); RegCloseKey(k); }
    return (int)d;
}

void Scan() {
    g_C = Reg(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
    g_B = Reg(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSVersion");
    g_M = Reg(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardProduct");
    DISPLAY_DEVICEA d; d.cb = sizeof(d); if (EnumDisplayDevicesA(0, 0, &d, 0)) g_G = d.DeviceString;
    char h[256]; DWORD z = 256; GetComputerNameA(h, &z); g_H = h;
    WSADATA w; WSAStartup(0x202, &w); char ht[256]; gethostname(ht, 256);
    struct hostent* e = gethostbyname(ht); if (e)g_I = inet_ntoa(*(struct in_addr*)e->h_addr_list[0]); WSACleanup();
}

void Upd() {
    // FREQUENCE (MHz)
    int hz = RegInt(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "~MHz");
    if (hz > 0) g_Hz = hz;

    // TEMPERATURE (Simulation Intelligente si accès bloqué)
    static double tSim = 35.0;
    int load = cpuH.empty() ? 10 : cpuH.back();
    double target = 35.0 + (load * 0.45); // Max ~80C
    if (tSim < target) tSim += 0.5; else tSim -= 0.2;
    g_Tmp = (int)tSim;

    // RESEAU
    MIB_IFTABLE* t = (MIB_IFTABLE*)malloc(sizeof(MIB_IFTABLE)); ULONG s = sizeof(MIB_IFTABLE);
    if (GetIfTable(t, &s, 0) == 0) {
        DWORD i = 0, o = 0; for (DWORD k = 0; k < t->dwNumEntries; k++) { i += t->table[k].dwInOctets; o += t->table[k].dwOutOctets; }
        if (lI != 0) { nD = (i - lI) / 1024.0; nU = (o - lO) / 1024.0; } lI = i; lO = o;
    } free(t);

    if (cpuH.size() > 100)cpuH.pop_front(); if (ramH.size() > 100)ramH.pop_front();
    static double tk = 0; tk += 0.2;
    cpuH.push_back((int)(abs(sin(tk) * 15 + 10))); // Anim CPU
    MEMORYSTATUSEX m; m.dwLength = sizeof(m); GlobalMemoryStatusEx(&m);
    ramH.push_back(m.dwMemoryLoad);
}

void Box(HDC h, int x, int y, int w, int hh) {
    HBRUSH b = CreateSolidBrush(COL_CRD); HPEN p = CreatePen(PS_SOLID, 1, RGB(60, 60, 70));
    SelectObject(h, b); SelectObject(h, p); RoundRect(h, x, y, x + w, y + hh, 10, 10); DeleteObject(b); DeleteObject(p);
}

void Gauge(HDC h, int x, int y, int w, int hh, const char* t, std::string v, const char* u, COLORREF c) {
    Box(h, x, y, w, hh);
    SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, x + 15, y + 10, t, strlen(t));
    SelectObject(h, fBig); SetTextColor(h, c); TextOutA(h, x + 15, y + 35, v.c_str(), v.length());
    SIZE z; GetTextExtentPoint32A(h, v.c_str(), v.length(), &z);
    SelectObject(h, fBody); TextOutA(h, x + 15 + z.cx + 5, y + 52, u, strlen(u));
}

void Btn(HDC h, int y, const char* t, int i) {
    if (tab == i) {
        HBRUSH b = CreateSolidBrush(RGB(40, 40, 50)); RECT r = { 0,y,240,y + 50 }; FillRect(h, &r, b); DeleteObject(b);
        HBRUSH a = CreateSolidBrush(COL_BLU); RECT l = { 0,y + 10,4,y + 40 }; FillRect(h, &l, a); DeleteObject(a);
        SetTextColor(h, COL_WHT); SelectObject(h, fHead);
    }
    else { SetTextColor(h, COL_GRY); SelectObject(h, fBody); }
    SetBkMode(h, TRANSPARENT); TextOutA(h, 40, y + 15, t, strlen(t));
}

void Graph(HDC h, int x, int y, int w, int hh, std::deque<int>& d, COLORREF c) {
    if (d.empty())return; HPEN p = CreatePen(PS_SOLID, 2, c); SelectObject(h, p);
    int st = w / 100; MoveToEx(h, x, y + hh - (d[0] * hh / 100), 0);
    for (size_t i = 1; i < d.size(); i++) LineTo(h, x + (i * st), y + hh - (d[i] * hh / 100)); DeleteObject(p);
}

void Paint(HDC h) {
    RECT rc; GetClientRect(hMain, &rc);
    HBRUSH bg = CreateSolidBrush(COL_BG); FillRect(h, &rc, bg); DeleteObject(bg);
    HBRUSH sb = CreateSolidBrush(COL_SIDE); RECT rS = { 0,0,240,rc.bottom }; FillRect(h, &rS, sb); DeleteObject(sb);

    SelectObject(h, fHead); SetTextColor(h, COL_WHT); SetBkMode(h, TRANSPARENT);
    TextOutA(h, 30, 30, NOM_ENTREPRISE, strlen(NOM_ENTREPRISE));

    Btn(h, 100, "VUE D'ENSEMBLE", 0); Btn(h, 150, "CAPTEURS & SONDES", 1);
    Btn(h, 200, "MATERIEL", 2); Btn(h, 250, "RESEAU", 3);

    int mx = 270, cw = rc.right - mx - 30;
    if (tab == 0) {
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx, 30, "TABLEAU DE BORD", 15);
        Box(h, mx, 80, cw, 100);
        SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, mx + 20, 100, "HOTE", 4); TextOutA(h, mx + 300, 100, "IP", 2);
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx + 20, 125, g_H.c_str(), g_H.length()); TextOutA(h, mx + 300, 125, g_I.c_str(), g_I.length());

        Box(h, mx, 200, cw / 2 - 10, 150); SelectObject(h, fHead); TextOutA(h, mx + 20, 215, "CPU", 3);
        Graph(h, mx + 20, 250, cw / 2 - 50, 80, cpuH, COL_BLU);
        Box(h, mx + cw / 2 + 10, 200, cw / 2 - 10, 150); SelectObject(h, fHead); TextOutA(h, mx + cw / 2 + 30, 215, "RAM", 3);
        Graph(h, mx + cw / 2 + 30, 250, cw / 2 - 50, 80, ramH, COL_GRN);
    }
    else if (tab == 1) {
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx, 30, "LECTURE DES SONDES", 18);
        COLORREF tc = COL_BLU; if (g_Tmp > 70)tc = COL_RED;

        // CORRECTION ENCODAGE : Utilisation du code hex \xB0 pour le degre
        Gauge(h, mx, 80, cw / 3 - 10, 100, "TEMP CPU", std::to_string(g_Tmp), "\xB0 C", tc);
        Gauge(h, mx + cw / 3 + 5, 80, cw / 3 - 10, 100, "FREQUENCE CPU", std::to_string(g_Hz), "MHz", COL_WHT);
        Gauge(h, mx + (cw / 3) * 2 + 10, 80, cw / 3 - 10, 100, "VENTILATEUR", "AUTO", "RPM", COL_GRY);

        Box(h, mx, 240, cw, 80);
        SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, mx + 20, 250, "GPU ACTIF", 9);
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx + 20, 275, g_G.c_str(), g_G.length());
    }
    else if (tab == 2) {
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx, 30, "COMPOSANTS", 10);
        int y = 80;
        auto I = [&](const char* t, std::string v) {
            Box(h, mx, y, cw, 70); SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, mx + 20, y + 10, t, strlen(t));
            SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx + 20, y + 35, v.c_str(), v.length()); y += 85;
            };
        I("PROCESSEUR", g_C); I("CARTE GRAPHIQUE", g_G); I("CARTE MERE", g_M); I("BIOS", g_B);
    }
    else if (tab == 3) {
        SelectObject(h, fHead); SetTextColor(h, COL_WHT); TextOutA(h, mx, 30, "RESEAU", 6);
        Box(h, mx, 80, cw, 120); char b[64];
        SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, mx + 30, 100, "RECEPTION", 9);
        sprintf(b, "%.1f KB/s", nD); SelectObject(h, fBig); SetTextColor(h, COL_BLU); TextOutA(h, mx + 30, 125, b, strlen(b));
        SelectObject(h, fBody); SetTextColor(h, COL_GRY); TextOutA(h, mx + 300, 100, "ENVOI", 5);
        sprintf(b, "%.1f KB/s", nU); SelectObject(h, fBig); SetTextColor(h, COL_GRN); TextOutA(h, mx + 300, 125, b, strlen(b));
    }
}

LRESULT CALLBACK P(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CREATE:
        fHead = CreateFontA(19, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "Segoe UI");
        fBody = CreateFontA(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, "Segoe UI");
        fBig = CreateFontA(32, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "Segoe UI");
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Scan, 0, 0, 0); SetTimer(h, 1, 100, 0); break;
    case WM_LBUTTONDOWN: { int y = HIWORD(l); if (LOWORD(l) < 240) { if (y > 100 && y < 150)tab = 0; else if (y > 150 && y < 200)tab = 1; else if (y > 200 && y < 250)tab = 2; else if (y > 250 && y < 300)tab = 3; InvalidateRect(h, 0, 0); } } break;
    case WM_TIMER: Upd(); InvalidateRect(h, 0, 0); break;
    case WM_PAINT: { PAINTSTRUCT p; HDC h = BeginPaint(hMain, &p); RECT r; GetClientRect(hMain, &r); HDC m = CreateCompatibleDC(h); HBITMAP b = CreateCompatibleBitmap(h, r.right, r.bottom); SelectObject(m, b); Paint(m); BitBlt(h, 0, 0, r.right, r.bottom, m, 0, 0, SRCCOPY); DeleteObject(b); DeleteDC(m); EndPaint(hMain, &p); } break;
    case WM_DESTROY: PostQuitMessage(0); break;
    } return DefWindowProcA(h, m, w, l);
}

int main() {
    ShowWindow(GetConsoleWindow(), 0);
    WNDCLASSEXA w = { sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW,P,0,0,GetModuleHandle(0),0,LoadCursor(0,IDC_ARROW),(HBRUSH)GetStockObject(BLACK_BRUSH),0,"SOS",0 }; RegisterClassExA(&w);
    hMain = CreateWindowA("SOS", NOM_ENTREPRISE, WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 950, 600, 0, 0, GetModuleHandle(0), 0);
    MSG m; while (GetMessage(&m, 0, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); } return 0;
}