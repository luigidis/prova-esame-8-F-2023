#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ERROR_LENGTH 1024
#define BUFFER_SIZE 1024

void readParameters(int argc, char *argv[]);
void error(char *error);
double ***createMatrix(char *filename, int *rows, int *cols);
void printMatrix(double ***matrix, int rows, int cols);
void freeMatrix(double **matrix, int rows);
void sparsify(double **matrix);
// collect();
// writeToFile();

int main(int argc, char *argv[])
{
    printf("Numeri di argomenti passati: %d\n", argc);

    for (int i = 0; i < argc; i++)
    {
        printf("Parametro passato n°%d = %s\n", i, argv[i]);
    }

    //In questo caso è come se avessi creato un alias di argv[1]
    char *inputFilename = argv[1];
    char *outputFilename = argv[0];
    int rows, cols;

    printf("============PUNTO A============\n");
    readParameters(argc, argv);
    printf("============PUNTO B============\n");
    double ***A = createMatrix(inputFilename, &rows, &cols);
    printMatrix(A, rows, cols);
}

void readParameters(int argc, char *argv[])
{
    char errorMessage[MAX_ERROR_LENGTH] = ""; // Buffer per concatenare la stringa

    if (argc != 3)
        error("Numero di parametri errato");

    for (int i = 1; i < argc; i++)
    {

        if (strlen(argv[i]) > 255)
        {
            char temp[100]; // Variabile temporanea per costruire il messaggio

            /*  snprintf è una funzione che formatta una stringa e la scrive in un buffer, evitando buffer overflow
                snprintf(char *str, size_t size, const char *format, ...)
                str è il buffer dove scrivere la stringa formattata
                size la dimensione massima del buffer
                format la stringa di formato (tipo printf)
                ... gli argomenti da inserire nella stringa
            */
            snprintf(temp, sizeof(temp), "Errore nel file n°:%d\t", i);

            /*
                strcat e strncat entrambe servono per la "string concatenation"
                la strncat è la versione più sicura, che evita di scrivere oltre alla memoria disponibile
                e quindi evita un overflow
                char *strcat(char *dest, const char *src);
                char *strncat(char *dest, const char *src, size_t n);
                - dest la stringa in cui concatenare (deve avere spazio sufficiente!)
                - src: la stringa da aggiungere
                - n numero massimo di caratteri da copiare
            */
            strncat(errorMessage, temp, MAX_ERROR_LENGTH - strlen(errorMessage) - 1);
        }
    }

    if (strlen(errorMessage) > 0)
    {
        error(errorMessage);
    }
    else
    {
        printf("Parametri validi!\n");
    }
        
    
}

void error(char *error)
{
    printf("[ERROR] %s\n", error);
    exit(1);
}

double ***createMatrix(char *filename, int *rows, int *cols)
{
    // Leggo il file
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        error("Impossibile aprire il file");
    }

    // Buffer per leggere il contenuto delle righe
    char line[BUFFER_SIZE];
    // Numero di righe
    int m = 0, n = 0;
    // Nel file assumo che ci sono un numero n di righe con un numero m di valori per riga tutti in virgola mobile
    while (fgets(line, BUFFER_SIZE, file))
    {
        m++; // Riga corretta
        if (m == 1)
        {
            // Conta il numero di colonne nella prima riga
            char *token = strtok(line, " ");
            while (token)
            {
                n++; // Aumento il numero di colonne
                // A strtok si usa per suddividere una stringa più grande in sottostringhe utilizzando un set di delimitatori, in questo caso lo spazio " "
                // La prima volta che si chiama strtok si passa la stringa da tokenizzare, poi se si vuole tokenizzare sempre la stessa scritta il primo parametro da inserire è NULL
                //  strtok(char *str,const char *delim);
                token = strtok(NULL, " ");
            }
        }
    }

    if (m == 0 || n == 0)
    {
        fclose(file);
        printf("Il file è vuoto o malformattato");
    }

    // Alloco spazio per la matrice per il corretto numero di righe
    double ***matrix = (double ***)malloc(m * sizeof(double **));
    if (!matrix)
    {
        fclose(file);
        error("Allocazione memoria fallita, per la matrice");
    }

    for (int i = 0; i < m; i++)
    {
        // Allocco il corretto spazio per ogni riga in base al numero di colonne ottenuto
        matrix[i] = (double **)malloc(n * sizeof(double *));
        if (!matrix[i])
        {
            for (int j = 0; j < i; j++)
                free(matrix[j]);
            free(matrix);
            fclose(file);
            error("Allocazione di memoria fallita, per le righe");
        }
    }

    // Torna all'inizio del file
    rewind(file);
    int row = 0;

    // Rileggiamo il file e creiamo la matrice
    while (fgets(line, sizeof(line), file))
    {
        int col = 0;
        char *token = strtok(line, " ");
        while (token)
        {
            double *value = (double*)malloc(sizeof(double));
            if (!value)
            {
                for (int i = 0; i < m ; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        if(matrix[i][j])
                            free(matrix[i][j]);
                    }
                    free(matrix[i]);
                }
                free(matrix);
                fclose(file);
                error("Allocazione memoria fallita per i valori");
            }
            *value = atof(token);
            matrix[row][col] = value;
            token = strtok(NULL, " ");
            col++;
        }
        row++;
    }

    // Chiudo lo stream del file
    fclose(file);
    printf("File Chiuso\n");
    // Mi conservo colonne e righe
    *rows = m;
    *cols = n;
    // Ritorno la matrice di puntatori a double
    return matrix;
}

void printMatrix(double ***matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        printf("Riga n°: %d\t", i);

        for (int j = 0; j < cols; j++)
        {
            printf("%f\t", *matrix[i][j]);
        }

        printf("\n");
    }
}

void freeMatrix(double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void sparsify(double ***matrix, int rows, int cols)
{  
    for (int i = 0; i < rows; i++)
    {
        
    }


}