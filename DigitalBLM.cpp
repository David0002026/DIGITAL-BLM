#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

const int NUM_JUEGOS = 10;

/* ===================== PROTOTIPOS DE FUNCIONES ===================== */
void mostrarMenu();
void mostrarCatalogo(string juegos[], double precios[], int stock[], int n);
void agregarCarrito(string juegos[], double precios[], int stock[],
                     int carritoID[], int carritoCantidad[], int &carritoCount, int n);
double obtenerTotalCarrito(double precios[], int carritoID[], int carritoCantidad[], int carritoCount);
void calcularTotal(string juegos[], double precios[], int carritoID[], int carritoCantidad[], int carritoCount);
void modificarCarrito(string juegos[], int stock[], int carritoID[], int carritoCantidad[],
                       int &carritoCount, int n);
void vaciarCarrito(int stock[], int carritoID[], int carritoCantidad[], int &carritoCount, bool restaurarStock);
void generarFactura(string juegos[], double precios[], int carritoID[], int carritoCantidad[],
                     int carritoCount, string cajero, int numeroFactura, double totalFactura);
void registrarCompraDiaria(int numeroFactura, string cajero, double totalFactura);
void confirmarCompra(string juegos[], double precios[], int stock[],
                      int carritoID[], int carritoCantidad[], int &carritoCount,
                      string cajero, int &numeroFactura, double &totalGeneral);

/* ============================ PROGRAMA PRINCIPAL ============================ */
int main() {

    // Inicializar variables: subtotal, total
    double totalGeneral = 0;
    int numeroFactura = 1;
    int opcion;
    string cajero;

    // Declarar e inicializar los arreglos: juegos[], precios[], stock[], carrito[]
    string juegos[NUM_JUEGOS] = {
        "GTA", "Minecraft", "Batman Arkham Origins", "Human Fall Flat",
        "Ready or Not", "Forza Horizon 6", "Peak", "Mortal Kombat XL",
        "Dragon Ball X", "Doom"
    };

    double precios[NUM_JUEGOS] = {
        29.99, 19.99, 24.99, 14.99, 27.99, 39.99, 9.99, 22.99, 34.99, 17.99
    };

    int stock[NUM_JUEGOS] = {
        15, 20, 10, 25, 12, 8, 30, 10, 6, 18
    };

    // Arreglos del carrito (paralelos): guardan el ID del juego y la cantidad pedida
    int carritoID[NUM_JUEGOS];
    int carritoCantidad[NUM_JUEGOS];
    int carritoCount = 0;

    cout << fixed << setprecision(2);

    cout << "=====================================\n";
    cout << "   TIENDA DIGITAL DE VIDEOJUEGOS   \n";
    cout << "=====================================\n";
    cout << "Ingrese el nombre completo del cajero: ";
    getline(cin, cajero);

    // Mostrar menu / opcion == 7 (Salir)? ... Switch Opcion
    do {

        mostrarMenu();
        cin >> opcion;

        // ¿opcion esta en rango 1..7?
        if (opcion < 1 || opcion > 7) {

            cout << "\nError: Opcion no valida.\n";

        } else {

            switch (opcion) {

                case 1: // Ver catalogo de juegos
                    mostrarCatalogo(juegos, precios, stock, NUM_JUEGOS);
                    break;

                case 2: // Añadir a carrito
                    agregarCarrito(juegos, precios, stock, carritoID, carritoCantidad, carritoCount, NUM_JUEGOS);
                    break;

                case 3: // Calcular total
                    calcularTotal(juegos, precios, carritoID, carritoCantidad, carritoCount);
                    break;

                case 4: // Modificar carrito
                    modificarCarrito(juegos, stock, carritoID, carritoCantidad, carritoCount, NUM_JUEGOS);
                    break;

                case 5: // Vaciar carrito
                    {
                        char confirmar;
                        cout << "\n¿Confirmar cancelar compra? (s/n): ";
                        cin >> confirmar;

                        if (confirmar == 's' || confirmar == 'S') {
                            vaciarCarrito(stock, carritoID, carritoCantidad, carritoCount, true);
                            cout << "Carrito vaciado.\n";
                        } else {
                            cout << "Operacion cancelada.\n";
                        }
                    }
                    break;

                case 6: // Confirmar compra
                    confirmarCompra(juegos, precios, stock, carritoID, carritoCantidad, carritoCount,
                                     cajero, numeroFactura, totalGeneral);
                    break;

                case 7: // Salir
                    cout << "\nSaliendo del sistema...\n";
                    break;
            }
        }

    } while (opcion != 7);

    cout << "\n=====================================\n";
    cout << "TOTAL GENERAL DE VENTAS DEL DIA: $" << totalGeneral << endl;
    cout << "Cajero que atendio: " << cajero << endl;
    cout << "=====================================\n";

    return 0;
}

/* ============================ DEFINICION DE FUNCIONES ============================ */

// Muestra el menu principal
void mostrarMenu() {
    cout << "\n=========== MENU PRINCIPAL ===========\n";
    cout << "1. Ver catalogo de juegos\n";
    cout << "2. Anadir a carrito\n";
    cout << "3. Calcular total\n";
    cout << "4. Modificar carrito\n";
    cout << "5. Vaciar carrito\n";
    cout << "6. Confirmar compra\n";
    cout << "7. Salir\n";
    cout << "=======================================\n";
    cout << "Seleccione una opcion (1-7): ";
}

// Case 1: recorre los arreglos con un FOR y muestra el catalogo completo
void mostrarCatalogo(string juegos[], double precios[], int stock[], int n) {

    cout << "\n============ CATALOGO DE JUEGOS ============\n";
    cout << left << setw(4) << "ID" << setw(28) << "Juego"
         << setw(12) << "Precio" << "Stock" << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(4) << (i + 1)
             << setw(28) << juegos[i]
             << "$" << setw(11) << precios[i]
             << stock[i] << endl;
    }
    cout << "==============================================\n";
}

// Case 2: agrega juegos al carrito, validando stock, en un DO-WHILE
void agregarCarrito(string juegos[], double precios[], int stock[],
                     int carritoID[], int carritoCantidad[], int &carritoCount, int n) {

    char otroJuego;

    do {

        int idIngresado, cantidad;

        cout << "\nIngrese el ID del juego (1-" << n << "): ";
        cin >> idIngresado;

        int id = idIngresado - 1;

        if (id < 0 || id >= n) {

            cout << "Error: ID de juego no valido.\n";

        } else {

            cout << "Ingrese la cantidad de copias: ";
            cin >> cantidad;

            // ¿cantidad de copias suficiente para el pedido?
            if (cantidad > 0 && cantidad <= stock[id]) {

                // Poner el juego en el carrito y descontar del stock
                carritoID[carritoCount] = id;
                carritoCantidad[carritoCount] = cantidad;
                carritoCount++;

                stock[id] -= cantidad;

                cout << juegos[id] << " agregado al carrito (" << cantidad << " copia(s)).\n";

            } else {
                cout << "Error: juego fuera de stock.\n";
            }
        }

        cout << "¿Desea agregar otro juego? (s/n): ";
        cin >> otroJuego;

    } while (otroJuego == 's' || otroJuego == 'S');
}

// Suma el total del carrito (precio * cantidad de cada item)
double obtenerTotalCarrito(double precios[], int carritoID[], int carritoCantidad[], int carritoCount) {

    double total = 0;

    for (int i = 0; i < carritoCount; i++) {
        total += precios[carritoID[i]] * carritoCantidad[i];
    }

    return total;
}

// Case 3: calcula y muestra el total del carrito actual
void calcularTotal(string juegos[], double precios[], int carritoID[], int carritoCantidad[], int carritoCount) {

    if (carritoCount == 0) {

        cout << "\nEl carrito esta vacio.\n";

    } else {

        cout << "\n=========== RESUMEN DEL CARRITO ===========\n";

        for (int i = 0; i < carritoCount; i++) {
            int id = carritoID[i];
            cout << juegos[id] << " x" << carritoCantidad[i]
                 << " = $" << (precios[id] * carritoCantidad[i]) << endl;
        }

        double total = obtenerTotalCarrito(precios, carritoID, carritoCantidad, carritoCount);
        cout << "--------------------------------------------\n";
        cout << "TOTAL A PAGAR: $" << total << endl;
        cout << "=============================================\n";
    }
}

// Case 4: modifica la cantidad de un juego que ya esta en el carrito
void modificarCarrito(string juegos[], int stock[], int carritoID[], int carritoCantidad[],
                       int &carritoCount, int n) {

    if (carritoCount == 0) {

        cout << "\nEl carrito esta vacio, no hay nada que modificar.\n";

    } else {

        int idIngresado, nuevaCantidad;

        cout << "\nIngrese el ID del juego a modificar (1-" << n << "): ";
        cin >> idIngresado;

        int id = idIngresado - 1;
        int posicion = -1;

        // Buscar el juego dentro del carrito
        for (int i = 0; i < carritoCount; i++) {
            if (carritoID[i] == id) {
                posicion = i;
            }
        }

        if (posicion == -1) {

            cout << "Ese juego no se encuentra en el carrito.\n";

        } else {

            cout << "Ingrese la nueva cantidad de copias: ";
            cin >> nuevaCantidad;

            int disponible = stock[id] + carritoCantidad[posicion];

            // ¿cantidad de copias suficiente para el pedido?
            if (nuevaCantidad > 0 && nuevaCantidad <= disponible) {

                stock[id] = disponible - nuevaCantidad;
                carritoCantidad[posicion] = nuevaCantidad;

                cout << "Carrito y stock actualizados correctamente.\n";

            } else {
                cout << "Error: juego fuera de stock.\n";
            }
        }
    }
}

// Case 5 (y limpieza tras Case 6): vacia el carrito, restaurando stock si se cancela la compra
void vaciarCarrito(int stock[], int carritoID[], int carritoCantidad[], int &carritoCount, bool restaurarStock) {

    if (restaurarStock) {
        for (int i = 0; i < carritoCount; i++) {
            stock[carritoID[i]] += carritoCantidad[i];
        }
    }

    carritoCount = 0;
}

// Genera el archivo .txt individual de la factura (Case 6, paso "Generar la factura")
void generarFactura(string juegos[], double precios[], int carritoID[], int carritoCantidad[],
                     int carritoCount, string cajero, int numeroFactura, double totalFactura) {

    string nombreArchivo = "factura_" + to_string(numeroFactura) + ".txt";
    ofstream archivo(nombreArchivo);

    time_t ahora = time(0);
    string fecha = ctime(&ahora);

    archivo << "===========================================\n";
    archivo << "     FACTURA DE COMPRA - TIENDA DIGITAL     \n";
    archivo << "===========================================\n";
    archivo << "Factura N°: " << numeroFactura << endl;
    archivo << "Fecha: " << fecha;
    archivo << "Cajero: " << cajero << endl;
    archivo << "-------------------------------------------\n";
    archivo << fixed << setprecision(2);

    for (int i = 0; i < carritoCount; i++) {
        int id = carritoID[i];
        archivo << juegos[id] << " x" << carritoCantidad[i]
                << " = $" << (precios[id] * carritoCantidad[i]) << endl;
    }

    archivo << "-------------------------------------------\n";
    archivo << "TOTAL: $" << totalFactura << endl;
    archivo << "===========================================\n";

    archivo.close();
}

// Registra la venta en el archivo de control diario (Case 6, paso "Registrar compra")
void registrarCompraDiaria(int numeroFactura, string cajero, double totalFactura) {

    ofstream control("control_ventas_diarias.txt", ios::app);

    time_t ahora = time(0);
    string fecha = ctime(&ahora);
    fecha = fecha.substr(0, fecha.size() - 1); // quita el salto de linea de ctime

    control << fixed << setprecision(2);
    control << "Factura #" << numeroFactura
            << " | Fecha: " << fecha
            << " | Cajero: " << cajero
            << " | Total: $" << totalFactura << endl;

    control.close();
}

// Case 6: confirma la compra, genera factura, registra el control diario y vacia el carrito
void confirmarCompra(string juegos[], double precios[], int stock[],
                      int carritoID[], int carritoCantidad[], int &carritoCount,
                      string cajero, int &numeroFactura, double &totalGeneral) {

    if (carritoCount == 0) {

        cout << "\nEl carrito esta vacio, no se puede confirmar la compra.\n";

    } else {

        char confirmar;
        cout << "\n¿Confirmar compra? (s/n): ";
        cin >> confirmar;

        if (confirmar == 's' || confirmar == 'S') {

            double totalFactura = obtenerTotalCarrito(precios, carritoID, carritoCantidad, carritoCount);

            generarFactura(juegos, precios, carritoID, carritoCantidad, carritoCount,
                            cajero, numeroFactura, totalFactura);

            registrarCompraDiaria(numeroFactura, cajero, totalFactura);

            totalGeneral += totalFactura;

            cout << "Compra confirmada. Factura N° " << numeroFactura
                 << " generada (factura_" << numeroFactura << ".txt).\n";

            numeroFactura++;

            // Vaciar el carrito sin restaurar stock, ya que la venta se completo
            vaciarCarrito(stock, carritoID, carritoCantidad, carritoCount, false);

        } else {
            cout << "Compra no confirmada.\n";
        }
    }
}