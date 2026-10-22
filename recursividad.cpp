#include <windows.h>
#include <vector>

const int ANCHO = 750;
const int ALTO = 650;

class Figura {
private:
    std::vector<unsigned char> zona;

    void marcarCuadrado(int x, int y, int a) {
        for (int py = y - a; py <= y + a; py++) {
            for (int px = x - a; px <= x + a; px++) {
                if (px >= 0 && px < ANCHO &&
                    py >= 0 && py < ALTO) {
                    zona[py * ANCHO + px] = 1;
                }
            }
        }
    }

public:
    Figura() : zona(ANCHO * ALTO, 0) {}

    void F(int x, int y, int a) {
        if (a <= 7) {
            return;
        }

        marcarCuadrado(x, y, a);

        F(x - a, y - a, a / 2);
        F(x + a, y - a, a / 2);
        F(x - a, y + a, a / 2);
        F(x + a, y + a, a / 2);
    }

    void Dibujar(HDC hdc) {
        HPEN lapiz = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN anterior = (HPEN)SelectObject(hdc, lapiz);

        HBRUSH fondo =
            (HBRUSH)GetStockObject(HOLLOW_BRUSH);
        HBRUSH anteriorFondo =
            (HBRUSH)SelectObject(hdc, fondo);

        for (int y = 1; y < ALTO - 1; y++) {
            for (int x = 1; x < ANCHO - 1; x++) {
                int pos = y * ANCHO + x;

                if (zona[pos] == 0) {
                    continue;
                }

                if (zona[pos - 1] == 0 ||
                    zona[pos + 1] == 0 ||
                    zona[pos - ANCHO] == 0 ||
                    zona[pos + ANCHO] == 0) {

                    SetPixel(hdc, x, y, RGB(0, 0, 0));
                }
            }
        }

        SelectObject(hdc, anteriorFondo);
        SelectObject(hdc, anterior);
        DeleteObject(lapiz);
    }
};

LRESULT CALLBACK ProcedimientoVentana(
    HWND ventana, UINT mensaje,
    WPARAM wParam, LPARAM lParam) {

    switch (mensaje) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(ventana, &ps);

        FillRect(hdc, &ps.rcPaint,
                 (HBRUSH)GetStockObject(WHITE_BRUSH));

        Figura figura;

        figura.F(350, 300, 150);

        figura.Dibujar(hdc);

        EndPaint(ventana, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(ventana, mensaje, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE instancia, HINSTANCE anterior,
    LPSTR argumentos, int mostrar) {

    const char CLASE[] = "VentanaFigura";

    WNDCLASS wc = {};
    wc.lpfnWndProc = ProcedimientoVentana;
    wc.hInstance = instancia;
    wc.lpszClassName = CLASE;
    wc.hbrBackground =
        (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&wc);

    HWND ventana = CreateWindowEx(
        0, CLASE, "Figura recursiva",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        ANCHO, ALTO,
        NULL, NULL, instancia, NULL
    );

    if (ventana == NULL) {
        return 0;
    }

    ShowWindow(ventana, mostrar);
    UpdateWindow(ventana);

    MSG mensaje = {};

    while (GetMessage(&mensaje, NULL, 0, 0) > 0) {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

    return 0;
}
