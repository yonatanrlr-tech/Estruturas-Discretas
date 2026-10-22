#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
using namespace std;

const string DIGITOS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int valorDigito(char c) {
    c = toupper(static_cast<unsigned char>(c));
    size_t pos = DIGITOS.find(c);
    if (pos == string::npos) return -1;
    return static_cast<int>(pos);
}

bool sonCeros(const vector<int>& v) {
    return all_of(v.begin(), v.end(),
                  [](int d) { return d == 0; });
}

string convertirEntero(vector<int> numero, int n, int m) {
    if (sonCeros(numero)) return "0";

    string resultado;

    while (!numero.empty()) {
        vector<int> cociente;
        int residuo = 0;

        for (int d : numero) {
            int valor = residuo * n + d;
            int q = valor / m;
            residuo = valor % m;

            if (!cociente.empty() || q != 0)
                cociente.push_back(q);
        }

        resultado += DIGITOS[residuo];
        numero = cociente;
    }

    reverse(resultado.begin(), resultado.end());
    return resultado;
}

string convertirDecimal(vector<int> fraccion,
                        int n, int m, int precision) {
    string resultado;

    for (int paso = 0;
         paso < precision && !sonCeros(fraccion);
         paso++) {

        int acarreo = 0;

        for (int i = (int)fraccion.size() - 1;
             i >= 0; i--) {

            int valor = fraccion[i] * m + acarreo;
            fraccion[i] = valor % n;
            acarreo = valor / n;
        }

        resultado += DIGITOS[acarreo];
    }

    if (!sonCeros(fraccion))
        resultado += "...";

    return resultado;
}

string convertirBase(string numero, int n, int m,
                     int precision) {

    if (n < 2 || n > 36 || m < 2 || m > 36)
        throw invalid_argument("Bases invalidas (2-36).");

    if (precision < 0 || precision > 1000)
        throw invalid_argument("Precision invalida.");

    bool negativo = false;
    bool decimal = false;
    bool hayDigitos = false;

    vector<int> entero, fraccion;
    size_t inicio = 0;

    if (!numero.empty() &&
        (numero[0] == '-' || numero[0] == '+')) {
        negativo = numero[0] == '-';
        inicio = 1;
    }

    for (size_t i = inicio; i < numero.size(); i++) {
        char c = numero[i];

        if (c == '.' || c == ',') {
            if (decimal)
                throw invalid_argument(
                    "Dos separadores decimales.");
            decimal = true;
            continue;
        }

        int d = valorDigito(c);

        if (d < 0 || d >= n)
            throw invalid_argument(
                "Digito no valido para la base.");

        hayDigitos = true;

        if (decimal)
            fraccion.push_back(d);
        else
            entero.push_back(d);
    }

    if (!hayDigitos)
        throw invalid_argument("Numero vacio.");

    string resultado = convertirEntero(entero, n, m);
    string decimales =
        convertirDecimal(fraccion, n, m, precision);

    if (!decimales.empty())
        resultado += "." + decimales;

    if (negativo &&
        !(sonCeros(entero) && sonCeros(fraccion)))
        resultado = "-" + resultado;

    return resultado;
}

int main() {
    int n, m, precision;
    string numero;

    cout << "=== CONVERSOR DE BASES ===\n";

    cout << "Base de origen (N): ";
    cin >> n;

    cout << "Base de destino (M): ";
    cin >> m;

    cout << "Numero a convertir: ";
    cin >> numero;

    cout << "Cantidad de decimales: ";
    cin >> precision;

    if (!cin) {
        cout << "Datos invalidos.\n";
        return 1;
    }

    try {
        cout << "\nResultado: "
             << convertirBase(numero, n, m, precision)
             << " (base " << m << ")\n";
    }
    catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}