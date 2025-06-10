//* Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//* Types and Structures
typedef enum
{
	ENTERO,
	VARCHAR,
} TipoDato;

typedef struct Campo
{
	char *nombre;
	int longitud;
	TipoDato tipo;
	struct Campo *siguiente;
} Campo;

typedef struct Registro
{
	void **datos;
	struct Registro *siguiente;
	struct Registro *anterior;
} Registro;

typedef struct Tabla
{
	char nombre[50];
	Campo *campos;
	Registro *registros;
	int numCampos;
	int numRegistros;
	struct Tabla *siguiente;
} Tabla;

typedef struct
{
	Tabla *tablas;
	int numTablas;
} BaseDatos;

//* Fuctions
// Get Fields
void agregarCampos(BaseDatos **base, Tabla *tabla)
{
	if (tabla == NULL)
	{
		printf("Tabla no encontrada.\n");
		return;
	}

	for (int i = 0; i < tabla->numCampos; i++)
	{
		tabla->campos[i].nombre = (char *)malloc(50 * sizeof(char));
		printf("Ingrese el nombre del campo %d: ", i + 1);
		scanf("%s", tabla->campos[i].nombre);

		printf("Ingrese la longitud del campo %d: ", i + 1);
		scanf("%d", &tabla->campos[i].longitud);

		int tipo;
		printf("Ingrese el tipo de dato del campo %d (0 para ENTERO, 1 para VARCHAR): ", i + 1);
		scanf("%d", &tipo);

		if (tipo < 0 || tipo > 1) // Validar tipo de dato (Opcional)
		{
			printf("Tipo de dato invalido. Asignando VARCHAR por defecto.\n");
			tipo = 1; // Asignar VARCHAR por defecto si el tipo es invalido
		}

		tabla->campos[i].tipo = (tipo == 0) ? ENTERO : VARCHAR;

		if (i < tabla->numCampos - 1)
		{
			tabla->campos[i].siguiente = &tabla->campos[i + 1];
		}
		else
		{
			tabla->campos[i].siguiente = NULL; // El ultimo campo no apunta a otro
		}
	}
}

// Add Table
void agregarTabla(BaseDatos **base, char *nombreTabla)
{
	if (*base == NULL)
	{
		*base = (BaseDatos *)malloc(sizeof(BaseDatos));
		(*base)->tablas = NULL;
		(*base)->numTablas = 0;
	}

	Tabla *nuevaTabla = (Tabla *)malloc(sizeof(Tabla));
	strcpy(nuevaTabla->nombre, nombreTabla);

	printf("\nIngrese el numero de campos para la tabla '%s': ", nombreTabla);
	int numCampos;
	scanf("%d", &numCampos);
	if (numCampos <= 0)
	{
		printf("El numero de campos debe ser mayor que 0.\n");
		free(nuevaTabla);
		return;
	}
	nuevaTabla->numCampos = numCampos;

	nuevaTabla->campos = (Campo *)malloc(numCampos * sizeof(Campo));
	agregarCampos(base, nuevaTabla);

	nuevaTabla->registros = NULL;
	nuevaTabla->numRegistros = 0;
	nuevaTabla->siguiente = (*base)->tablas;

	(*base)->tablas = nuevaTabla;
	(*base)->numTablas++;
};

// Search Table
Tabla *buscarTablaEnBd(BaseDatos *base, char *nombreTabla)
{
	if (base == NULL || base->tablas == NULL)
	{
		return NULL;
	}

	Tabla *tablaActual = base->tablas;
	while (tablaActual != NULL)
	{
		if (strcmp(tablaActual->nombre, nombreTabla) == 0)
		{
			return tablaActual;
		}
		tablaActual = tablaActual->siguiente;
	}
	return NULL;
}

// Show Fields
void mostrarCampos(Tabla *tabla)
{
	if (tabla == NULL)
	{
		printf("Tabla no encontrada.\n");
		return;
	}

	printf("Numero de campos de la tabla '%s': %d\n", tabla->nombre, tabla->numCampos);
	printf("Campos de la tabla '%s':\n", tabla->nombre);
	Campo *campoActual = tabla->campos;
	while (campoActual != NULL)
	{
		printf(" - %s (%d) ", campoActual->nombre, campoActual->longitud);
		if (campoActual->tipo == ENTERO)
		{
			printf("[ENTERO]\n");
		}
		else if (campoActual->tipo == VARCHAR)
		{
			printf("[VARCHAR]\n");
		}
		campoActual = campoActual->siguiente;
	}
	printf("\n");
}

// Show Tables
void mostrarTabla(BaseDatos *base, char *nombreTabla)
{
	Tabla *tabla = buscarTablaEnBd(base, nombreTabla);
	if (tabla == NULL)
	{
		printf("Tabla '%s' no encontrada.\n", nombreTabla);
		return;
	}

	printf("Tabla: %s\n", tabla->nombre);
	mostrarCampos(tabla);

	printf("Registros: %d\n", tabla->numRegistros);
	Registro *registroActual = tabla->registros;
	while (registroActual != NULL)
	{
		for (int i = 0; i < tabla->numCampos; i++)
		{
			if (registroActual->datos[i] != NULL)
				printf(" - %s: %s\n", tabla->campos[i].nombre, (char *)registroActual->datos[i]);
			else
				printf(" - %s: NULL\n", tabla->campos[i].nombre);
		}
		registroActual = registroActual->siguiente;
	}
};

// Add Record

// Show Records

//* Main
int main()
{
	// Variables
	BaseDatos *baseDatos = NULL;
	int opcion, continuar = 1;

	printf("Bienvenido al Gestor de Base de Datos\n");

	while (continuar)
	{

		printf("\nMenu:\n");
		printf("1. Agregar Tabla\n");
		printf("2. Mostrar Tabla\n");
		printf("3. Agregar Registro\n");
		printf("4. Mostrar Registros\n");
		printf("0. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion)
		{
			{
			case 1:
			{
				char nombreTabla[50];
				printf("\nIngrese el nombre de la tabla: ");
				scanf("%s", nombreTabla);
				agregarTabla(&baseDatos, nombreTabla);
				printf("Tabla '%s' agregada exitosamente.\n", nombreTabla);
			}
			break;

			case 2:
			{
				char nombreTabla[50];
				printf("\nIngrese el nombre de la tabla a mostrar: ");
				scanf("%s", nombreTabla);
				mostrarTabla(baseDatos, nombreTabla);
			}
			break;

			case 3:
			{
				printf("\nFuncionalidad de agregar registro no implementada.\n");
				// Aquí se implementaría la lógica para agregar un registro a una tabla.
			}
			break;

			case 4:
			{
				printf("\nFuncionalidad de mostrar registros no implementada.\n");
				// Aquí se implementaría la lógica para mostrar los registros de una tabla.
			}
			break;

			case 0:
				printf("\nSaliendo del gestor de base de datos.\n");
				continuar = 0;
				break;

			default:
				printf("Opcion no valida. Intente de nuevo.\n");
				break;
			}
		}
	}
	return 0;
}