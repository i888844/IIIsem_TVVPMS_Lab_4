#include <iostream>
#include <cmath>
#include <windows.h>
#include <time.h>

#pragma warning(disable: 4996)

#define PI 3.1415926535
#define SKO 1
#define MX 0
#define EPS 0.00001

using namespace std;

double function(double x, double math_o, double s)
{
    double result = 1.0 / (s * powf(2.0 * PI, 0.5)) * expf(-1.0 * (powf(x - math_o, 2.0) / (2.0 * powf(s, 2.0))));
    return result;
}

double integral(double x_start, double x_end, double math_o, double s)
{
    double x = x_start;
    double delta = 0.0;
    double s_current = 0.0;
    double s_previous = 0.0;
    int intervals = 1;
    int i = 0;
    do
    {
        s_previous = s_current;
        s_current = 0.0;
        intervals *= 2;
        delta = (x_end - x_start) / intervals;
        for (i = 1, x = x_start; i <= intervals; i++, x += delta)
        {
            s_current += function(x, math_o, s);
        }
        s_current *= delta;
    } while (fabs(s_previous - s_current) > EPS);
    return s_current;
}

double minimum(double* array, int N)
{
    double minimum = *(array + 0);
    for (int i = 0; i < N; i++)
    {
	    if (minimum > *(array + i))
	    {
            minimum = *(array + i);
	    }
    }
    return minimum;
}

double maximum(double* array, int N)
{
    double maximum = *(array + 0);
    for (int i = 0; i < N; i++)
    {
        if (maximum < *(array + i))
        {
            maximum = *(array + i);
        }
    }
    return maximum;
}

double math_ozh(double* array, int N)
{
    double math_ozh = 0;
    for (int i = 0; i < N; i++)
    {
        math_ozh += *(array + i);
    }
    math_ozh /= N;
    return math_ozh;
}

double dispers_p(double* array, int N, double math_o, int p)
{
    double dispers_p = 0;
    for (int i = 0; i < N; i++)
    {
        dispers_p += powf((*(array + i)) - math_o, p);
    }
    dispers_p /= N;
    return dispers_p;
}

double disper_group(double** Tb, int intervals, double math_o, int p)
{
    double disper_group = 0;
    for (int i = 0; i < intervals; i++)
    {
        disper_group += powf((Tb[i][2] - math_o) * Tb[i][4], p);
    }
    return disper_group;
}

double math_ozh_group(double** Tb, int intervals)
{
    double math_ozh_group = 0;
    for (int i = 0; i < intervals; i++)
    {
        math_ozh_group += Tb[i][2] * Tb[i][4];
    }
    return math_ozh_group;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));
    int i = 0;
    int j = 0;
    int N = 0;
    double number = 0;
    double* array = new double[200];
    //double x_start = -3.0;
    //double x_end = 3.0;
    //double left_line = 0.0;
    //cube a(20, x_start, x_end);
    //a.output();
    //double delta = (x_end - x_start) / a.get_sides();
    //FILE* output = fopen("output.txt", "w");
    //if (output == nullptr)
    //{
    //    cout << "[Ошибка]: не удалось открыть файл для записи" << endl;
    //    return 1;
    //}
    //for (i = 0; i < N; i++)
    //{
    //    number = a.roll();
    //    left_line = x_start + (number - 1) * delta;
    //    array[i] = delta * (double)rand() / (double)RAND_MAX + left_line;
    //    cout << array[i] << "  ";
    //    fprintf(output, "%f ", array[i]);
    //}
    //fclose(output);

    FILE* input = fopen("output.txt", "r+");
	if (input == nullptr)
	{
        cout << "[Ошибка]: не удалось открыть файл для чтения" << endl;
        return 1;
	}
    i = 0;
    while(fscanf(input, "%lf", &number) == 1)
    {
    	    cout << number << "  ";
            array[i] = number;
            i++;
            N++;
    }
    fclose(input);

    double x_min = minimum(array, N);
    double x_max = maximum(array, N);
    double R = x_max - x_min;
    int intervals = (int)(1 + 3.32 * log10f(N));
    double h = R / intervals;
    double math_o = math_ozh(array, N);
    double disp = dispers_p(array, N, math_o, 2);
    double sig = powf(disp, 0.5);
    double asim = dispers_p(array, N, math_o, 3)/powf(sig, 3);
    double ek = dispers_p(array, N, math_o, 4) / powf(sig, 4) - 3;
    cout << "\n\nКол-во интервалов: " << intervals << endl;
    cout << "Максимальное: " << x_max << endl;
    cout << "Минимальное: " << x_min << endl;
    cout << "R: " << R << endl;
    cout << "h: " << h << endl;
    cout << "Мат. ожидание: " << math_o << endl;
    cout << "Дисперсия: " << disp << endl;
    cout << "СКО: " << sig << endl;
    cout << "Асимметрия: " << asim << endl;
    cout << "Эксцесс: " << ek << endl;
    double **Tb = new double * [N];
    for (i = 0; i < N; i++)
    {
        Tb[i] = new double [9];
        for (j = 0; j < 9; j++)
        {
            Tb[i][j] = 0.0;
        }
    }
    Tb[0][0] = x_min;
    Tb[0][1] = Tb[0][0] + h;
    for (i = 1; i < intervals + 1; i++)
    {
        Tb[i][0] = Tb[i - 1][1];
        Tb[i][1] = Tb[i][0] + h;
    }
    Tb[intervals - 1][1] = x_max;
    for (i = 0; i < N; i++)
    {
	    for (j = 0; j < intervals; j++)
	    {
		    if (array[i]<= Tb[j][1])
		    {
                Tb[j][2] += array[i];
                Tb[j][3] += 1;
                break;
		    }
	    }
    }
    double F = 0.0;
    for (i = 0; i < intervals; i++)
    {
        Tb[i][2] /= Tb[i][3];
        Tb[i][4] = Tb[i][3] / N;
        Tb[i][5] = Tb[i][4] / h;
        Tb[i][6] = Tb[i][4] + F;
        F = Tb[i][6];
    }
    double math_o_group = math_ozh_group(Tb, intervals);
    double disp_group = disper_group(Tb, intervals, math_o_group, 2);
    double sig_group = powf(disp_group, 0.5);
    double asim_group = disper_group(Tb, intervals, math_o_group,3)/powf(sig_group, 3);
    double ek_group = disper_group(Tb, intervals, math_o_group, 4) / powf(sig_group, 4) - 3;
    cout << "\nМат. ожидание групп.: " << math_o_group << endl;
    cout << "Дисперсия групп.: " << disp_group << endl;
    cout << "СКО групп.: " << sig_group << endl;
    cout << "Асимметрия групп.: " << asim_group << endl;
    cout << "Эксцесс групп.: " << ek_group << endl;
    double hi2 = 0.0;
    F = integral(x_min - 2 * h, x_min, MX, SKO) / 2;
    double lamda = 0.0;
    double max_rF = 0.0;
    for (i = 0; i < intervals; i++)
    {
        Tb[i][7] = integral(Tb[i][0], Tb[i][1], MX, SKO);
        hi2 += powf(Tb[i][4] - Tb[i][7], 2) / Tb[i][7];
        Tb[i][8] = Tb[i][7] + F;
        F = Tb[i][8];
        if (fabs(Tb[i][8] - Tb[i][6]) > max_rF)
        {
            max_rF = abs(Tb[i][8] - Tb[i][6]);
        }
    }
    hi2 *= N;
    lamda = max_rF * powf(N, 0.5);
    cout << "\n\nТаблица" << endl;
    cout << "+-----+--------------+---------------+----------------+---------+------------------+------------+---------------+---------------+-------------+" << endl;
    cout << "|  N  |   X_left_i   |   X_right_i   |   X_predst_i   |   N_i   |        P_i*      |    W_i*    |      F_i*     |      P_i      |     F_i     |" << endl;
    cout << "+-----+--------------+---------------+----------------+---------+------------------+------------+---------------+---------------+-------------+" << endl;
    for (i = 0; i < intervals; i++)
    {
        printf("%4i", i + 1);
	    for (j = 0; j < 9; j++)
	    {
            printf("%15.5lf", Tb[i][j]);
	    }
        cout << endl;
    }
    cout << "Признак Пирсона: " << hi2 << endl;
    cout << "Критерий Колмагорова: " << lamda << endl;
    delete[]array;
    return 0;
}