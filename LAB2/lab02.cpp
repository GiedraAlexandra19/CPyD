#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Multiplicación de matrices clásica
void Clasica(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    int m = B[0].size();
    int p = B.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = 0;
            for (int k = 0; k < p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Multiplicación de matrices por bloques
void Bloques(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int blockSize) {
    int n = A.size();
    int m = B[0].size();
    int p = B.size();

    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < m; j += blockSize) {
            for (int k = 0; k < p; k += blockSize) {
                for (int x = i; x < min(i + blockSize, n); x++) {
                    for (int y = j; y < min(j + blockSize, m); y++) {
                        for (int z = k; z < min(k + blockSize, p); z++) {
                            C[x][y] += A[x][z] * B[z][y];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int n = 1000; // Tamaño de las matrices
    int blockSize = 64; // Tamaño del bloque para la multiplicación por bloques

    // Inicialización de matrices A, B y C
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 2));
    vector<vector<int>> C(n, vector<int>(n, 0));

    // Medir tiempo para la multiplicación clásica
    auto start = high_resolution_clock::now();
    Clasica(A, B, C);
    auto stop = high_resolution_clock::now();
    std::chrono::duration<double> duration = stop - start;

    cout << "Tiempo para multiplicacion clasica: " << duration.count() << " s" << endl;

    // Reinicializar matriz C
    C = vector<vector<int>>(n, vector<int>(n, 0));

    // Medir tiempo para la multiplicación por bloques
    auto startb = high_resolution_clock::now();
    Bloques(A, B, C, blockSize);
    auto stopb = high_resolution_clock::now();
     std::chrono::duration<double> durationb = stopb - startb;

    cout << "Tiempo para multiplicacion por bloques: " << durationb.count() << " s" << endl;

    return 0;
}