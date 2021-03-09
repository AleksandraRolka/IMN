/* 
 * KOMENDY : 
 *
 * make all - kompiluje program, uruchamia go tworząc odpowiednie pliki .dat oraz uruchamia skrypt gnuplota, który tworzy wykresy
 * make clean - usuwa plik wykonywalny 'main' oraz pliki z roszerzeniami .o, .dat, .png
 *
 */
#include <iostream>
#include <cstdio>
#include <cmath>


// parametry  
#define delta 0.2
#define n_x 128
#define n_y 128
#define TOL pow( 10., -8. )

const double x_max = delta * n_x;
const double y_max = delta * n_y;
const double sigma_x = 0.1 * x_max;
const double sigma_y = 0.1 * y_max;


int it = 0;

double** make_and_init_V_tab()
{
	double **V;
    V = new double* [n_x + 1];

    for (int i = 0; i <= n_x; i++){
        V[i] = new double[n_y + 1];
	}
	
	for ( int i = 0; i <= n_x; i++ ){
		for ( int j = 0; j <= n_y; j++ )
		{
			V[i][j] = 0.;
		}
	}
	
    return V;
}

void delete_tab(double** V)
{
    for (int i = 0; i <= n_x; i++)
        delete[] V[i];
	

    delete[] V;
}


double rho(double x, double y)
{
	return 0.1 * exp( -(pow( x - 0.5 * x_max , 2. ) / pow( sigma_x , 2. )) - (pow( y - 0.5 * y_max , 2. ) / pow( sigma_y , 2. )) );
}

double c(double p, int k)
{
	if( p == k/2. || p == -k/2.){
		return 0.5;
	}else 
		return 1.;
}

double c_a_b(double alfa, double beta, int k)
{
	return c( alfa, k ) * c( beta, k );
}

double rho_d(int i, int j, int k)
{	
	double rho_d_s = 0.;
	double rho_d_s1 = 0.;
	double rho_d_s2 = 0.;
	for ( double alfa = -k / 2.; alfa <= k / 2.; alfa += 1. ){
		for ( double beta = -k / 2.; beta <= k / 2.; beta += 1. )
		{
			rho_d_s1 += c_a_b(alfa,beta,k) * rho( alfa + i, beta  + j );
			rho_d_s2 += c_a_b(alfa,beta,k);
		}
	}
	rho_d_s = rho_d_s1 / rho_d_s2;
	
	return rho_d_s;
}


double count_S(double** V, int k)
{
	double s = 0.;
	
	for ( int i = 0; i <= n_x - k; i += k){
		for ( int j = 0; j <= n_y - k; j += k )
		{
		s +=  pow( k * delta, 2. ) * ( 0.5 * pow( ( V[i+k][j] - V[i][j] ) / ( 2. * k * delta ) + ( V[i+k][j+k] - V[i][j+k] ) / ( 2. * k * delta ) , 2. ) + 0.5 * pow( ( V[i][j+k] - V[i][j] ) / ( 2. * k * delta ) + ( V[i+k][j+k] - V[i+k][j] ) / ( 2. * k * delta ) , 2. )  - rho_d( i, j, k ) * V[i][j] );
		}
	}
	return s;
}


void relaxation(double ***V, int k)
{
	double S_it, S_it_prev;
	
	std::string filename_S = "S_k=" + std::to_string(k) + ".dat";
	FILE *file_S = fopen(&filename_S[0], "w");
	
	std::string filename_V = "V_k=" + std::to_string(k) + ".dat";
	FILE *file_V = fopen(&filename_V[0], "w");

	S_it = count_S( (*V), k );
	
	while(true)
	{
		it++;
		
		for ( int i = k; i <= n_x - k; i += k ){
			for ( int j = k; j <= n_y - k; j += k )
			{
				(*V)[i][j] = 0.25 * ( (*V)[i + k][j] + (*V)[i - k][j] + (*V)[i][j + k] + (*V)[i][j - k] + pow( delta * k , 2. ) * rho_d( i, j, k ) );

			}
		}
		
		S_it_prev = S_it;
		S_it = count_S( (*V), k );
		
		// wartości całki funkcjonalnej w funkcji numeru iteracji
		fprintf(file_S,"%d\t%g\n", it, S_it);
		
		// warunek stopu
		if( fabs( (S_it - S_it_prev ) / S_it_prev ) < TOL )
		{
			break;
		}
		
	}
	
	for ( int i = 0; i <= n_x; i += k ){
		for ( int j = 0; j <= n_y; j +=k )
		{
			// potencjał w węźle
			fprintf(file_V,"%d\t%d\t%g\n", i, j, (*V)[i][j]);
		}
		fprintf(file_V,"\n");
	}
	
	// zagęszczanie siatki
	if (k != 1)
    {
		for (int i = 0; i <= n_x - k; i += k){
			for (int j = 0; j <= n_y - k; j += k)
			{
				(*V)[i + k/2][j + k/2]   = 0.25 * ( (*V)[i][j] 		+  (*V)[i + k][j] 	+ (*V)[i][j + k] 	+ (*V)[i + k][j + k]);
				
				
				if ( i != (n_x - k) )
					(*V)[i + k][j + k/2] = 0.5  * ( (*V)[i + k][j] 	+  (*V)[i + k][j + k] );
				
				if ( j !=  (n_y - k) )
					(*V)[i + k/2][j + k] = 0.5  * ( (*V)[i][j + k] 	+  (*V)[i + k][j + k] );
				
				if ( j != 0 )
					(*V)[i + k/2][j] 	 = 0.5  * ( (*V)[i][j] 		+  (*V)[i + k][j] );
				
				if ( i !=  0 )
					(*V)[i][j + k/2] 	 = 0.5  * ( (*V)[i][j] 		+  (*V)[i][j + k] );
			}
		}
	}
	
	fclose(file_S);
	fclose(file_V);
}


int main()
{
	double **V = make_and_init_V_tab();
	
	// warunki brzegowe
	for (int i = 0; i <= n_x; i++)
    {
        V[i][n_y] = -sin( 2. * M_PI * delta * i / x_max );
        V[i][0]   =  sin( 2. * M_PI * delta * i  / x_max );
    }
	for (int i = 0; i <= n_y; i++)
    {
        V[0][i]   =  sin( M_PI * delta * i / y_max );
        V[n_x][i] =  sin( M_PI * delta * i / y_max );
    }
	
	
	int k[] = { 16, 8, 4, 2, 1 };
	
	for( int i = 0; i < 5; i++ )
		relaxation( &V, k[i] );
	
	
	delete_tab(V);
	
	return 0;
}