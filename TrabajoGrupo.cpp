/*
* Juego Master Mind C++. 
* El juego genera una clave de colores en forma de letras may. en el rango A-F. 
* Durante el juego se mostraran mensajes con el numero de muertos y heridos. 
* 1 muerto es un caracter acertada en una posicion determinada. 
* 1 herido es un caracter acertado que no esta en la posicion correcta. 
* Para ganar debes acertar todas las posiciones. 
* 
* Se han añadido al juego las siguientes características: 
* - Selector de nivel de dificultad para tener mas o menos intentos. (facil(15), medio(10), dificil(5))
* - Comprobacion de la entrada para caracteres repetidos.
* - Comprobacion del rango en la entrada para ver si metemos caracteres de la A a la F.
* - Pantalla de presentacion con instrucciones y selector de nivel. 
* 
* ================================================ 
* Autores: uo252040, uo250732, uo252137, uo250711 
* ================================================
*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <string>
using namespace std;

const int N_MAX_INTENTOS = 15; 
const int N_COLORES = 6;
const int N_COLORES_CLAVE = 4;
const int CARACTER_INICIAL = 'A';
const string LICENCIA = "Trabajo grupo FCyR";
const int CARACTER_FINAL = CARACTER_INICIAL + N_COLORES;
const int FACTOR_INICIAL = 20;
const int FACTOR_DIFICULTAD = 5;


char Clave[N_COLORES_CLAVE + 1] = "ABCD";

extern "C" int calculaNMuertosASM(char propuesta[], char clave[], int nElementos);

/*
* Lee la licencia del programa y comprueba q sea valida.
* La licencia debe ser como máximo de 20 caracteres.
* autor: uo252040
*/
bool leeLicencia()
{
	char licencia[20];
	cout << "Introduce la Licencia para Jugar: " << endl;
	cin.getline(licencia, 20);
	return (strlen(licencia) <= 20 && licencia == LICENCIA);
}

/*
* Función pseudo-aleatoria para generar la clave a partir de valor[]
*/
void generaClave(char valor[], int nValores, int minimo, int maximo)
{
	int aleatorio = 0;
	int generados = 0;
	int i = 0;
	bool repetido = false;

	while (generados != nValores)
	{
		// Genera un número aleatorio entre máximo y mínimo
		// minimo = codigo ASCII del primer caracter valido
		// maximo = codigo ASCII del ultimo caracter valido
		aleatorio = (int)(((double)rand() / (RAND_MAX + 1)) * (maximo - minimo) + minimo);


		// Se comprueba que el caracter no ha sido generado antes
		repetido = false;
		i = 0;
		while ((!repetido) && (i < generados))
		{
			repetido = (aleatorio == valor[i]);
			i++;
		}


		// Si el caracter no está repetido, se añade como válido a la clave
		if (!repetido)
		{
			valor[generados] = (char)aleatorio;
			generados++;
		}
	}

	// Hay que añadir el terminador de cadenas a la clave generada. Tiene que estar en la última posición del array.
	valor[nValores] = '\0';
}


/*
* Comprueba que los datos introducidos están comprendidos en el rango ASCII (CARACTER_INICIAL, CARACTER_FINAL)
* autor: uo250732
*/
bool compruebaRangoCaracteres(char propuesta[], int nElementos)
{
	int i;
	int incorrecto = 0; 
	for (i = 0; i < nElementos; i++)
	{
		if (!(propuesta[i] >= CARACTER_INICIAL && propuesta[i] < CARACTER_FINAL)){
			incorrecto++;
		}		
	}
	return (incorrecto == 0); 
}


/*
* Comprueba que los datos introducidos no contengan ningun caracter repetido.
* autor: uo250732
*/
bool compruebaCaracteresRepetidos(char propuesta[], int nElementos){
	int i, j;
	for (i = 0; i < nElementos; i++)
	{
		for (j = i + 1; j < nElementos; j++)
		{
			if (propuesta[i] == propuesta[j])
			{
				return false;
			}
		}
	}
	return true;
}

/*
* Calcula y devuelve el numero de muertos o posiciones exactas acertadas
* autor: uo252040
*/
int calculaNMuertos(char propuesta[], char solucion[], int nElementos){
	int nMuertos = 0;
	for (int i = 0; i < nElementos; i++){
		if (propuesta[i] == solucion[i])
			nMuertos++;
	}
	return nMuertos;
}

/*
* Calcula y devuelve el numero de heridos o posiciones no exactas acertadas
* autor: uo252137
*/
int calculaNHeridos(char propuesta[], char solucion[], int nElementos)
{
	int nHeridos = 0;
	for (int i = 0; i < nElementos; i++)
	{
		if (propuesta[0] == solucion[i] || propuesta[1] == solucion[i] || propuesta[2] == solucion[i] || propuesta[3] == solucion[i])
		{
			if (propuesta[i] != solucion[i])
			{
				nHeridos++;
			}
		}
	}
	return nHeridos;
}


/*
* Metodo que imprime por pantalla el resultado de la jugada actual, es decir, los muertos y los heridos conseguidos con la solucion propuesta
* autor: uo252137
*
*/
void comentarJugada(char propuesta[], char solucion[], int nElementos){
	int numeroMuertos = calculaNMuertosASM(propuesta, solucion, nElementos);
	int numeroHeridos = calculaNHeridos(propuesta, solucion, nElementos);

	if (numeroMuertos == N_COLORES_CLAVE)
	{
		cout << "HAS GANADO LA PARTIDA :)" << endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		cout << "Hay " << numeroMuertos << " muertos y " << numeroHeridos << " heridos." << endl;
	}

	cout << endl;
}

/*
* Aplicación principal main().
* Comprueba licencia y condiciones. Permite hasta un numero maximo de N_MAX_INTENTOS intentos.
* autor: uo252137
*/
int main()
{
	cout << "MASTER MIND" << endl;
	cout << endl;

	// Comprobar licencia
	if (leeLicencia() == false)
	{
		printf("Error en licencia\n");
		exit(EXIT_FAILURE);
	}

	cout << endl;

	cout << "Intrucciones del Juego " << endl;
	cout << "El objetivo de este juego es muy simple, solo tienes que descifrar la clave" << endl;
	cout << "formada por cuatro letras." << endl;
	cout << endl;

	cout << "OJO" << endl;
	cout << "- Ninguna letra puede estar repetida en la clave." << endl;
	cout << "- Las letras deben estar en el rango [A-F]." << endl;
	cout << "- La longitud de la clave tiene que ser de 4 letras mayusculas." << endl;
	cout << endl;

	cout << "Nivel de Dificultad" << endl;
	cout << "1 - Facil" << endl;
	cout << "2 - Medio" << endl;
	cout << "3 - Dificil" << endl;
	cout << endl;

	// preguntamos por la dificultad. 
	int dificultad = 0; 
	while (true)
	{
		cin >> dificultad;
		// solo admitimos niveles en el rango [1,3]
		if ((cin) && (dificultad > 0) && (dificultad < 4))
			break;

		cin.clear();
		cin.ignore(1000, '\n'); // ignorar lo que no sea un numero
		cout << "Nivel no valido, escoge un valor entre 1 y 3" << endl;
	}
	
	// Comienza el juego 
	int nMuertos = 0;
	int nHeridos = 0;
	int intentos_nivel = 0;
	int nIntentos = 0;
	int contador_intentos = 0;
	char intento[N_COLORES_CLAVE + 1];	

	srand((unsigned int)time(NULL));

	// Generar clave
	generaClave(Clave, N_COLORES_CLAVE, CARACTER_INICIAL, CARACTER_FINAL);

	// nivel de dificultad, intentos permitidos segun dificultad. 
	intentos_nivel = FACTOR_INICIAL - (dificultad * FACTOR_DIFICULTAD);
	cout << "Dispones de " << intentos_nivel << " intentos en la jugada" << endl;
	// Jugadas
	while (contador_intentos != intentos_nivel)
	{
		contador_intentos++;

		cout << "Introduce Intento: ";
		cin >> intento ;

		// Se comprueba la validez de la solucion propuesta, contando como intento tanto si es valida como si no lo es
		// independientemente de que la respuesta sea correcta o no.
		cout << "Intento Numero " << contador_intentos << endl;

		if (compruebaCaracteresRepetidos(intento, N_COLORES_CLAVE) == false)
		{
			cout << "No se permiten valores REPETIDOS en la jugada" << endl;
		}
		else if (compruebaRangoCaracteres(intento, N_COLORES_CLAVE) == false)
		{
			cout << "No se permiten valores FUERA DEL RANGO en la jugada" << endl;
		}
		else
		{
			comentarJugada(intento, Clave, N_COLORES_CLAVE);
		}
	}

	// Juego perdido
	if (contador_intentos == intentos_nivel){
		cout << "HAS PERDIDO :(" << endl;
		cout << endl;
		cout << "La clave era " << Clave << endl;
	}
	return 0;
}