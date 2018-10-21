#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSV_NAME_LEN_MAX 100
#define BUFF_SIZE 500

void read_coeffs_from_csv(char* csv_name, float* coeffs, int n_coeffs);
int get_n_coeffs(char* csv_name);
void run(int n_coeffs, float* coeffs, float tolerance, int n_max, float a, float b);
float f_val(int n_coeffs, float* coeffs, float num);
int sign(float val);

int main(int argc, char **argv)
{
    int n_coeffs;
    char* csv_name;
    float tolerance;
    int n_max;
    float a;
    float b;

    /* Print number of arguments.*/
    printf("Number of arguments: %d\n", argc);

    if (argc < 6)
    {
        exit(0);
    }

    csv_name = (char*) malloc(strlen(argv[1]) * sizeof(char));
    strcpy(csv_name, argv[1]);
    tolerance = atof(argv[2]);
    n_max = atoi(argv[3]);
    a = atof(argv[4]);
    b = atof(argv[5]);

    n_coeffs = get_n_coeffs(csv_name);
    float* coeffs = (float*) malloc((n_coeffs) * sizeof(float));
    printf("N coeffs: %d\n", n_coeffs);

    read_coeffs_from_csv(csv_name, coeffs, n_coeffs);

    run(n_coeffs, coeffs, tolerance, n_max, a, b);
}

void run(int n_coeffs, float* coeffs, float tolerance, int n_max, float a, float b)
{
    printf("### IN RUN ###\n");
    printf("tolerance: %f\n", tolerance);
    printf("n_max: %d\n", n_max);
    printf("a: %f\n", a);
    printf("b: %f\n", b);

    int cond1;
    int cond2;

    // Check conditions
    if (a > b)
    {
        printf("a needs to be bigger than b, a:%f, b:%f", a, b);
        exit(0);
    }

    cond1 = ((f_val(n_coeffs, coeffs, a) < 1) && (f_val(n_coeffs, coeffs, b) < 0)); 
    cond2 = ((f_val(n_coeffs, coeffs, a) > 0) && (f_val(n_coeffs, coeffs, b) > 0)); 
    if ((cond1) || (cond2))
    {
        printf("f(a) and f(b) must have different signs, f(a):%f, f(b):%f",
               f_val(n_coeffs, coeffs, a),
               f_val(n_coeffs, coeffs, b));
        exit(0);
    }

    int N = 1;
    float c;
    while (N <= n_max)
    {
        c = (a + b)/2.0; // Calculate a new midpoint.
        if ((f_val(n_coeffs, coeffs, c) == 0) || ((b-a)/2 < tolerance)){
            printf("X = %f", c);
            exit(0);
        }
        N++;
        if (sign(f_val(n_coeffs, coeffs, c)) == sign(f_val(n_coeffs, coeffs, a)))
        {
            a = c;
        }
        else
        {
            b = c;
        }
        printf("N: %d, a: %f, b:%f, c:%f\n", N, a, b, c);
    }
    printf("Bisection method failed.");
}


float f_val(int n_coeffs, float* coeffs, float num)
{
    int i;
    float exp;
    float sum = coeffs[0];

    for (i = 1; i < n_coeffs; i++)
    {
        exp = (float) i;
        sum += coeffs[i] * powf(num, exp);
    }
    return sum;
}

int sign(float val)
{
   if (val > 0)
       return 1;
   if (val < 0)
       return -1;
   return 0;
}

int get_n_coeffs(char* csv_name)
{
    int i;
    int commas = 0;

    char* buffer = (char*) malloc(BUFF_SIZE * sizeof(char));

    FILE* f = fopen(csv_name, "r");
    fgets(buffer, BUFF_SIZE, f);

    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == ',')
        {
            commas++;
        }
    }

    return commas + 1;
}

void read_coeffs_from_csv(char* csv_name, float* coeffs, int n_coeffs)
{
    int i;
    int sub_str_len;
    
    char* sub_buffer = (char*) malloc(BUFF_SIZE * sizeof(char));
    char* buffer = (char*) malloc(BUFF_SIZE * sizeof(char));
    char delim[] = ",";


    FILE* f = fopen(csv_name, "r");
    fgets(buffer, BUFF_SIZE, f);
    if (buffer[strlen(buffer)-1] == '\n')
    {
        buffer[strlen(buffer)-1] = '\0';
    }

    char* top_ptr = strtok(buffer, delim);

    i = 0;
    while (top_ptr != NULL)
    {
        if (top_ptr[0] == '-')
        {
            sub_str_len = strlen(top_ptr);
            memcpy(sub_buffer, &top_ptr[1], sub_str_len-1);
            sub_buffer[sub_str_len] = '\0';
            coeffs[i] = (-1.0) * atof(sub_buffer);
        }
        else
        {
            coeffs[i] = atof(top_ptr);
        }
        i++;
        top_ptr = strtok(NULL, delim);
    }
}
