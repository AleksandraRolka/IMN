/* 
 * KOMENDY : 
 *
 * make all - kompiluje program, uruchamia go tworząc odpowiednie pliki .dat oraz uruchamia skrypt gnuplota, który tworzy wykresy
 * make clean - usuwa plik wykonywalny 'main' oraz pliki z roszerzeniami .o, .dat, .png
 *
 */
#include <iostream>
#include <cmath>
#include <string>


// parametry
#define n_x 150
#define n_t 1000
#define delta 0.1
#define delta_t 0.05
#define x_A 7.5
#define sigma 0.5



double* make1Dtab(int size)
{
	
	double *tab;
    tab= new double [size];
	
	for ( int i = 0; i < size; i++ )
		tab[i] = 0.;
	
    return tab;
}


double a_F( double x, double t, double xF)
{
	double delta_Kroneckera;
	
	if( x == xF ){
		delta_Kroneckera = 1.;
	}
	else{
		delta_Kroneckera = 0.;
	}
	
	return cos( 50. * t / ( delta_t * n_t ) ) * delta_Kroneckera;
}


void wave_equation(double alfa, double beta)
{
	
	std::string fn1 = "u_alfa_" + std::to_string(alfa) + "_beta_" + std::to_string(beta) + ".dat";
	std::string fn2 = "E_alfa_" + std::to_string(alfa) + "_beta_" + std::to_string(beta) + ".dat";
	
	FILE *file_u = fopen(&fn1[0], "w");
	FILE *file_E = fopen(&fn2[0], "w");
	
	
	
	double* u0 = make1Dtab( n_x + 1 );
	double* u = make1Dtab( n_x + 1 );
	double* v = make1Dtab( n_x + 1 );
	double* vp = make1Dtab( n_x + 1 );
	double* a = make1Dtab( n_x + 1 );
	double sum, E;
	
	// sztywne warunki brzegowe u, V
	u[0] = 0;
	u[n_x] = 0;
	v[0] = 0;
	v[n_x] = 0;
	
	// warunki początkowe u, v
	for ( int i = 1; i < n_x; i++)
	{
		u[i] = exp( - pow( (i * delta) - x_A, 2.) / ( 2. * pow( sigma, 2.) ));
		v[i] = 0.;
	}
	
	// zachowaj poprzedni wynik
    for ( int i = 0; i < n_x; i++ )
    {
        u0[i] = u[i];
    }
	
	// inicjalizacja a
	for ( int i = 1; i < n_x; i++ )
        {
            a[i] = ( u[i+1] - 2 * u[i] + u[i-1] ) / pow( delta, 2. ) - beta * ( u[i] - u0[i] ) / delta_t + alfa * a_F( delta * i, 0, 2.5);
        }
	
	
	for ( int t = 1; t <= n_t; t++ )
	{
		sum = 0.;
		E = 0.;
		
		for ( int i = 0; i < n_x; i++ )
			vp[i] = v[i] + delta_t / 2. * a[i];
		
		for ( int i = 0; i < n_x; i++ )
			u0[i] = u[i];
	
		for ( int i = 0; i < n_x; i++ )
			u[i] = u[i] + delta_t * vp[i];
		
		for ( int i = 1; i < n_x; i++ )
			a[i] = ( u[i+1] - 2 * u[i] + u[i-1] ) / pow( delta, 2. ) - beta * ( u[i] - u0[i] ) / delta_t + alfa * a_F( delta * i, delta_t * t, 2.5);
		
		for ( int i = 0; i < n_x; i++ )
			v[i] = vp[i] + delta_t / 2. * a[i];
		
		for ( int i = 1; i < n_x; i++ )
			sum += pow( v[i], 2. ) + pow( ( u[i+1] - u[i - 1] ) / ( 2. * delta ), 2. );
		
		E = ( delta / 4. ) * ( pow( ( u[1] - u[0] ) / delta, 2. ) + pow( ( u[n_x] - u[n_x-1] ) / delta, 2. ) ) + ( delta / 2. ) * sum;
		
		// zapis do plików	
		fprintf(file_E, "%g\t%g\n", delta_t * t, E);
		
		for ( int i = 0; i < n_x; i++ )
			fprintf(file_u, "%g\t%g\t%g\n", delta_t * t, delta * i, u[i]);
		fprintf(file_u, "\n");
		
	}
	
	
	delete[] u0;
	delete[] u;
	delete[] v;
	delete[] vp;
	delete[] a;
	
	fclose(file_u);
	fclose(file_E);
	
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	double alfa = 0.;
	double beta = 0.;
	
	wave_equation( alfa, beta );
	
	beta = 0.1;
	wave_equation( alfa, beta );
	
	beta = 1.;
	wave_equation( alfa, beta );
	
	alfa = 1.;
	wave_equation( alfa, beta );

}