#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definición de la estructura para almacenar la información de una persona
typedef struct {
    char nombre_apellido[50];
    int dni;
    char sexo;
    float estatura;
    float peso;
} Persona;

// Función para generar nombres y apellidos ficticios
void generarNombresApellidos(char nombre_apellido[50]) {
    // Arrays con nombres y apellidos predefinidos
    char nombres[10][15] = {"Juan", "Ana", "Pedro", "Maria", "Carlos", "Laura", "Luis", "Isabel", "Javier", "Elena"};
    char apellidos[10][15] = {"Gomez", "Lopez", "Perez", "Rodriguez", "Fernandez", "Gonzalez", "Martinez", "Sanchez", "Diaz", "Romero"};

    // Seleccionar aleatoriamente un nombre y un apellido y concatenarlos
    strcpy(nombre_apellido, nombres[rand() % 10]);
    strcat(nombre_apellido, " ");
    strcat(nombre_apellido, apellidos[rand() % 10]);
}

// Función para generar un DNI único
int generarDNIUnico(int *usedDNIs, int index) {
    int dni;
    do {
        // Generar un DNI entre 1000 y 9999
        dni = rand() % 9000 + 1000;
    } while (usedDNIs[dni - 1000] == 1); // Verificar si el DNI ya ha sido usado

    usedDNIs[dni - 1000] = 1; // Marcar el DNI como usado
    return dni;
}

// Función hash simple para reducir el DNI a un índice en la tabla hash
int funcionHash(int dni) {
    return dni % 15; // Reducir el DNI a un índice entre 0 y 14
}

int main() {
    // Inicializar el generador de números aleatorios
    srand(time(NULL));

    // Crear una estructura de datos para almacenar cada registro
    Persona registros[1000];
    int grupoPorIndice[1000]; // Array para almacenar el grupo al que pertenece cada persona
    int usedDNIs[9000] = {0}; // Array para rastrear los DNIs utilizados

    // Llenar la lista de registros con datos ficticios
    for (int i = 0; i < 1000; ++i) {
        // Generar un DNI único
        registros[i].dni = generarDNIUnico(usedDNIs, i);

        // Generar nombres y apellidos ficticios
        generarNombresApellidos(registros[i].nombre_apellido);

        // Generar valores aleatorios para sexo, estatura y peso
        registros[i].sexo = (rand() % 2 == 0) ? 'M' : 'F';
        registros[i].estatura = (float)(rand() % 30 + 150) / 100;  // Estatura entre 1.50 y 1.80
        registros[i].peso = (float)(rand() % 5000 + 5000) / 100;   // Peso entre 50 y 100 kg

        // Calcular el grupo usando la función hash
        grupoPorIndice[i] = funcionHash(registros[i].dni);
    }

    // Mostrar los registros por grupo en orden del 1 al 15
    for (int grupo = 1; grupo <= 15; ++grupo) {
        printf("Grupo %d:\n", grupo);
        for (int i = 0; i < 1000; ++i) {
            if (grupoPorIndice[i] + 1 == grupo) {
                printf("Registro %d: Nombre: %s, DNI: %d, Sexo: %c, Estatura: %.2f, Peso: %.2f\n",
                    i + 1, registros[i].nombre_apellido, registros[i].dni,
                    registros[i].sexo, registros[i].estatura, registros[i].peso);
            }
        }
        printf("\n");
    }

    // Permitir al usuario ingresar un DNI para consultar
    int dniConsulta;
    printf("Ingrese un DNI para consultar a qué grupo pertenece y ver los datos: ");
    scanf("%d", &dniConsulta);

    // Encontrar el índice correspondiente al DNI ingresado
    int indiceConsulta = -1;
    for (int i = 0; i < 1000; ++i) {
        if (registros[i].dni == dniConsulta) {
            indiceConsulta = i;
            break;
        }
    }

    // Mostrar el resultado de la consulta
    if (indiceConsulta != -1) {
        printf("\nConsulta para el DNI %d:\n", dniConsulta);
        printf("Pertenece al Grupo %d\n", grupoPorIndice[indiceConsulta] + 1);
        printf("Valor hash del DNI: %d\n", funcionHash(dniConsulta));
        printf("Nombre: %s, Sexo: %c, Estatura: %.2f, Peso: %.2f\n",
            registros[indiceConsulta].nombre_apellido, registros[indiceConsulta].sexo,
            registros[indiceConsulta].estatura, registros[indiceConsulta].peso);

        // Escribir la información en un archivo binario
        FILE *archivo = fopen("consulta_dni.bin", "wb");
        if (archivo != NULL) {
            // Escribir la información de la persona en el archivo binario
            fwrite(&registros[indiceConsulta], sizeof(Persona), 1, archivo);
            fclose(archivo);
            printf("\nLa información se ha guardado en consulta_dni.bin\n");
        } else {
            printf("\nError al abrir el archivo para escribir.\n");
        }
    } else {
        printf("\nNo se encontró un registro para el DNI ingresado.\n");
    }

    return 0;
}
