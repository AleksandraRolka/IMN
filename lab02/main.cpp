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
#include <cstring>

double alfa( double beta, int N, double gamma)
{
	return beta * N - gamma;
}

void Picard_method(double u_0, double dt, double t_max, double TOL, int N, double beta, double gamma)
{
		
	FILE *file = fopen("Picard_method.dat", "w");
	
	double curr_u_mi = u_0;
	double prev_u_mi, prev_u = u_0;
	int mi;
	
	for (double t = 0.; t <= t_max; t = t + dt)
	{
		mi = 0;
		prev_u_mi = 0.;
		while ( fabs( curr_u_mi - prev_u_mi ) >= TOL && mi < 20 )
		{
			prev_u_mi = curr_u_mi;
			curr_u_mi = prev_u + ( dt / 2. ) * ( ( alfa( beta, N, gamma ) * prev_u - beta * pow( prev_u, 2.) ) + ( alfa( beta, N, gamma ) * prev_u_mi - beta * pow( prev_u_mi, 2.) ) );
		}
		prev_u = curr_u_mi;
		fprintf( file, "%g \t%g \t%g\n", t, curr_u_mi, (double)N - curr_u_mi);
	}
	
	fclose(file);
}


void Newton_method(double u_0, double dt, double t_max, double TOL, int N, double beta, double gamma)
{
	
	FILE *file = fopen("Newton_method.dat", "w");
	
	double curr_u_mi = u_0;
	double prev_u_mi, prev_u = u_0;
	int mi;
	
	for (double t = 0.; t <= t_max; t = t + dt)
	{
		mi = 0;
		prev_u_mi = 0.;
		while ( fabs( curr_u_mi - prev_u_mi ) >= TOL && mi < 20 )
		{
			prev_u_mi = curr_u_mi;
			curr_u_mi = prev_u_mi - ( prev_u_mi - prev_u - ( dt / 2. ) * ( alfa( beta, N, gamma ) * prev_u - beta * pow( prev_u, 2. ) + alfa( beta, N, gamma ) * prev_u_mi - beta * pow( prev_u_mi, 2. ) ) ) / ( 1. - ( dt / 2 ) * ( alfa( beta, N, gamma ) - 2 * beta * prev_u_mi ) );
		}
		prev_u = curr_u_mi;
		fprintf( file, "%g \t%g \t%g\n", t, curr_u_mi, (double)N - curr_u_mi);
	}

	fclose(file);
}


double func(double beta, int N, double gamma, double u)
{
	return ( beta * N - gamma ) * u - beta * pow( u, 2. );
}
// func() niezależne od t, dlatego c1,c2 są niepotrzebne

void RK2_method(double u_0, double dt, double t_max, double TOL, int N, double beta, double gamma)
{
	
	FILE *file = fopen("RK2_method.dat", "w");
	
	double a11 = 0.25,
		   a12 = 0.25 - sqrt(3) / 6,
		   a21 = 0.25 + sqrt(3) / 6,
		   a22 = 0.25,
		   
		   b1 = 0.5, b2 = 0.5;
		   
	double dU1, dU2, F1, F2, m11, m12, m21, m22;
	
	double curr_U1_mi = u_0, curr_U2_mi = u_0, prev_U1_mi, prev_U2_mi, prev_u, curr_u = u_0;
	int mi;
		   
	
	
	for (double t = 0.; t <= t_max; t = t + dt)
	{
		mi = 0;
		
		prev_U1_mi = prev_u;
		prev_U2_mi = prev_u;
		prev_u = curr_u;
		
		while ( mi < 20 )
		{
			++mi;	
			
			m11 = 1 - dt * a11 * ( alfa( beta, N, gamma ) - 2 * beta * prev_U1_mi );
			m12 =   - dt * a12 * ( alfa( beta, N, gamma ) - 2 * beta * prev_U2_mi );
			m21 =   - dt * a21 * ( alfa( beta, N, gamma ) - 2 * beta * prev_U1_mi );
			m22 = 1 - dt * a22 * ( alfa( beta, N, gamma ) - 2 * beta * prev_U2_mi );
		
			F1 = prev_U1_mi - prev_u - dt * ( a11 * ( alfa( beta, N, gamma ) * prev_U1_mi - beta * pow( prev_U2_mi, 2. ) ) + a12 * ( alfa( beta, N, gamma ) * prev_U2_mi - beta * pow( prev_U2_mi, 2.) ) );
			F2 = prev_U2_mi - prev_u - dt * ( a21 * ( alfa( beta, N, gamma ) * prev_U1_mi - beta * pow( prev_U2_mi, 2. ) ) + a22 * ( alfa( beta, N, gamma ) * prev_U2_mi - beta * pow( prev_U2_mi, 2.) ) );
		
			dU1 = ( F2 * m12 - F1 * m22 ) / ( m11 * m22 - m12 * m21 );
            dU2 = ( F1 * m21 - F2 * m11 ) / ( m11 * m22 - m12 * m21 );
			
			prev_U1_mi = curr_U1_mi;
			prev_U2_mi = curr_U2_mi;
			
			curr_U1_mi = prev_U1_mi +  dU1;
            curr_U2_mi = prev_U2_mi +  dU2;
			
			curr_u = prev_u + dt * ( b1 * func( beta, N, gamma, curr_U1_mi ) + b2 * func( beta, N, gamma, curr_U2_mi ) );
			
			if(fabs( curr_u - prev_u ) < TOL)
				break;
		}
		fprintf( file, "%g \t%g \t%g\n", t, curr_u, (double)N - curr_u);
	}
	
	fclose(file);
	
}

int main()
{

	int N = 500;
	double beta = 0.001, gamma = 0.1, t_max = 100, dt = 0.1, u_0 = 1, TOL = pow(10,-6);
	
	
	Picard_method(u_0, dt, t_max, TOL, N, beta, gamma);
	Newton_method(u_0, dt, t_max, TOL, N, beta, gamma);
	RK2_method(u_0, dt, t_max, TOL, N, beta, gamma);

    
	return 0;
}
