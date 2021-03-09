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


double analytical( double t, double lambda )
{
	
	return exp( lambda * t );
}

void explicit_Euler_method( double lambda, double y_0, double t_beg, double t_end, double dt )
{
	std::string filename = "explicit_Euler_method_" + std::to_string(dt) + ".dat";
	FILE *file = fopen(&filename[0], "w");
	
	double y_curr = y_0;
	double y_prev;
	
	for (double t = t_beg; t <= t_end; t = t + dt )
	{
		fprintf( file, "%g  \t %g  \t %g  \t %g\n", t, y_curr, analytical( t, lambda ), fabs( y_curr - analytical( t, lambda ) ) );
		y_prev = y_curr;
		y_curr = y_prev + dt * lambda * y_prev;
	}
	
	fclose(file);
}

void explicit_RK2_method( double lambda, double y_0, double t_beg, double t_end, double dt )
{
	std::string filename = "explicit_RK2_method_" + std::to_string(dt) + ".dat";
	FILE *file = fopen(&filename[0], "w");
	
	double y_curr = y_0;
	double y_prev, k1, k2;
	
	for (double t = t_beg; t <= t_end; t = t + dt )
	{
		fprintf( file, "%g  \t %g  \t %g  \t %g\n", t, y_curr, analytical( t, lambda ), fabs( y_curr - analytical( t, lambda ) ) );
		y_prev = y_curr;
		k1 = lambda * y_prev;
		k2 = lambda * ( y_prev + ( dt * k1 ) );
		y_curr = y_prev + ( dt / 2 ) * ( k1 + k2 );
	}
	
	fclose(file);	
}

void explicit_RK4_method( double lambda, double y_0, double t_beg, double t_end, double dt )
{
	std::string filename = "explicit_RK4_method_" + std::to_string(dt) + ".dat";
	FILE *file = fopen(&filename[0], "w");
	
	double y_curr = y_0;
	double y_prev, k1, k2, k3, k4;
	
	for (double t = t_beg; t <= t_end; t = t + dt )
	{
		fprintf( file, "%g  \t %g  \t %g  \t %g\n", t, y_curr, analytical( t, lambda ), fabs( y_curr - analytical( t, lambda ) ) );
		y_prev = y_curr;
		k1 = lambda * y_prev;
		k2 = lambda * ( y_prev + ( dt * k1 / 2 ) );
		k3 = lambda * ( y_prev + ( dt * k2 / 2 ) );
		k4 = lambda * ( y_prev + ( dt * k3 ) );
		y_curr = y_prev + ( dt / 6 ) * ( k1 + 2 * k2 + 2 * k3 + k4 );
	}
	
	fclose(file);	
}


double V_t( double w_V, double t)
{
	return 10 * sin( w_V * t );
}

void RLC( double R, double L, double C,  double t0, double dt, double Q_0, double I_0, double factor)
{
	double w_0 = 1 / sqrt( L * C );
	double T_0 = 2 * M_PI / w_0;
	double w_V = factor * w_0;
	double k1_Q, k2_Q, k3_Q, k4_Q, k1_I, k2_I, k3_I, k4_I;
	double	Q = Q_0, I = I_0;
	
	std::string filename = "RLC_Q_I_factor_" + std::to_string(factor) + ".dat";
	FILE *file = fopen(&filename[0], "w");
	
	
	for (double t = t0; t <= (4 * T_0 ); t = t + dt )
	{
		k1_Q = I;
		k1_I = V_t( w_V, t) / L - ( 1 / ( L * C ) ) * Q - ( R / L ) * I;
		k2_Q = I + ( dt / 2 ) * k1_I;
		k2_I = V_t( w_V, t + (dt / 2) ) / L - ( 1 / ( L * C ) ) * ( Q + ( dt / 2 ) * k1_Q ) - ( R / L ) * ( I + ( dt / 2 ) * k1_I );
		k3_Q = I + ( dt / 2 ) * k2_I;
		k3_I = V_t( w_V, t + (dt / 2) ) / L - ( 1 / ( L * C ) ) * ( Q + ( dt / 2 ) * k2_Q ) - ( R / L ) * ( I + ( dt / 2 ) * k2_I );
		k4_Q = I + dt * k3_I;
		k4_I = V_t( w_V, t + dt ) / L - ( 1 / ( L * C ) ) * ( Q + dt * k3_Q ) - ( R / L ) * ( I + dt * k3_I ); 
	
		Q = Q + ( dt / 6 ) * ( k1_Q + 2 * k2_Q + 2 * k3_Q + k4_Q );
		I = I + ( dt / 6 ) * ( k1_I + 2 * k2_I + 2 * k3_I + k4_I );

		fprintf( file, "%g  \t %g  \t %g\n", t, I, Q );
	}
	
	fclose(file);	
}


int main()
{
	
	explicit_Euler_method( -1., 1, 0., 5., 0.01);
	explicit_Euler_method( -1., 1, 0., 5., 0.1);
	explicit_Euler_method( -1., 1, 0., 5., 1);
	
	explicit_RK2_method( -1., 1, 0., 5., 0.01);
	explicit_RK2_method( -1., 1, 0., 5., 0.1);
	explicit_RK2_method( -1., 1, 0., 5., 1);

	explicit_RK4_method( -1., 1, 0., 5., 0.01);
	explicit_RK4_method( -1., 1, 0., 5., 0.1);
	explicit_RK4_method( -1., 1, 0., 5., 1);


	double R = 100, L = 0.1, C = 0.001, t_0 = 0., dt = pow( 10., -4. ), Q_0 = 0., I_0 = 0.;

	RLC( R, L, C, t_0, dt, Q_0, I_0, 0.5 );
	RLC( R, L, C, t_0, dt, Q_0, I_0, 0.8 );
	RLC( R, L, C, t_0, dt, Q_0, I_0, 1.0 );
	RLC( R, L, C, t_0, dt, Q_0, I_0, 1.2 );
	
    return 0;
}