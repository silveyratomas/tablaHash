#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nombre_apellido[50];
    char dni[9];
    char sexo;
    float estatura;
    float peso;
} Persona;

void generarNombresApellidos(char nombre_apellido[50]) {
    char nombres[10][15] = {"Juan", "Ana", "Pedro", "Maria", "Carlos", "Laura", "Luis", "Isabel", "Javier", "Elena"};
    char apellidos[10][15] = {"Gomez", "Lopez", "Perez", "Rodriguez", "Fernandez", "Gonzalez", "Martinez", "Sanchez", "Diaz", "Romero"};

    strcpy(nombre_apellido, nombres[rand() % 10]);
    strcat(nombre_apellido, " ");
    strcat(nombre_apellido, apellidos[rand() % 10]);
}

void generarDNIUnico(char usedDNIs[1000][9], char dni[9]) {
    do {
        // Generar un DNI entre 10000000 y 99999999
        sprintf(dni, "%08d", rand() % 100000000);
    } while (usado(usedDNIs, dni));

    marcarComoUsado(usedDNIs, dni);
}

int usado(char usedDNIs[1000][9], const char dni[9]) {
    for (int i = 0; i < 1000; ++i) {
        if (strcmp(usedDNIs[i], dni) == 0) {
            return 1;  // Ya usado
        }
    }
    return 0;  // No usado
}

void marcarComoUsado(char usedDNIs[1000][9], const char dni[9]) {
    for (int i = 0; i < 1000; ++i) {
        if (usedDNIs[i][0] == '\0') {
            strcpy(usedDNIs[i], dni);
            break;
        }
    }
}

int funcionHash(const char *dni) {
    return atoi(dni) % 15;
}

void imprimirRegistrosPorGrupo(const Persona registros[1000], const int grupoPorIndice[1000], int grupo) {
    printf("Grupo %d:\n", grupo);
    for (int i = 0; i < 1000; ++i) {
        if (grupoPorIndice[i] + 1 == grupo) {
            printf("Registro %d: Nombre: %s, DNI: %s, Sexo: %c, Estatura: %.2f, Peso: %.2f\n",
                i + 1, registros[i].nombre_apellido, registros[i].dni,
                registros[i].sexo, registros[i].estatura, registros[i].peso);
        }
    }
    printf("\n");
}

void imprimirTodosRegistrosPorGrupos(const Persona registros[1000], const int grupoPorIndice[1000]) {
    for (int grupo = 1; grupo <= 15; ++grupo) {
        imprimirRegistrosPorGrupo(registros, grupoPorIndice, grupo);
    }
}

int main() {
    srand(time(NULL));

    Persona registros[1000];
    int grupoPorIndice[1000];
    char usedDNIs[1000][9];

    for (int i = 0; i < 1000; ++i) {
        generarDNIUnico(usedDNIs, registros[i].dni);
        generarNombresApellidos(registros[i].nombre_apellido);
        registros[i].sexo = (rand() % 2 == 0) ? 'M' : 'F';
        registros[i].estatura = (float)(rand() % 30 + 150) / 100;
        registros[i].peso = (float)(rand() % 5000 + 5000) / 100;
        grupoPorIndice[i] = funcionHash(registros[i].dni);
    }

    imprimirTodosRegistrosPorGrupos(registros, grupoPorIndice);

    char dniConsulta[9];
    printf("Ingrese un DNI para consultar a qué grupo pertenece y ver los datos: ");
    scanf("%8s", dniConsulta);

    int indiceConsulta = -1;
    for (int i = 0; i < 1000; ++i) {
        if (strcmp(registros[i].dni, dniConsulta) == 0) {
            indiceConsulta = i;
            break;
        }
    }

    if (indiceConsulta != -1) {
        printf("\nConsulta para el DNI %s:\n", dniConsulta);
        printf("Pertenece al Grupo %d\n", grupoPorIndice[indiceConsulta] + 1);
        printf("Valor hash del DNI: %d\n", funcionHash(dniConsulta));
        printf("Nombre: %s, Sexo: %c, Estatura: %.2f, Peso: %.2f\n",
            registros[indiceConsulta].nombre_apellido, registros[indiceConsulta].sexo,
            registros[indiceConsulta].estatura, registros[indiceConsulta].peso);

        FILE *archivo = fopen("consulta_dni.bin", "wb");
        if (archivo != NULL) {
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
