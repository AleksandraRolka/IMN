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
#include <utility>


// parametry startowe 
#define x_0 0.01
#define v_0 0.
#define dt_0 1.
#define S 0.75
#define t_max 40
#define alfa 5.0
#define p 2
#define eps pow( 10, (-10.) )



double g( double x, double v)
{
	return alfa * ( 1. - pow( x, 2. ) ) * v - x;
}

double F( double xn, double xn1, double vn, double vn1, double dt)
{
	return xn1 - xn - ( dt / 2. ) * ( vn + vn1 );
}

double G( double xn, double xn1, double vn, double vn1, double dt)
{
	return vn1 - vn - ( dt / 2. ) * ( g( xn, vn ) + g( xn1, vn1) );
}



double a11()
{
	return 1.;
}

double a12( double dt )
{
	return  - dt / 2.;
}

double a21( double xn1, double vn1, double dt )
{
	return -( dt / 2. ) * ( (-2) * alfa * xn1 * vn1 - 1. );
}

double a22( double xn1, double dt )
{
	return 1. - ( dt / 2. ) * alfa * ( 1. - pow( xn1, 2.) );
}



std::pair<double,double> trapezoidal_method( double xn, double vn, double dt)
{
	
	double dx, dv;
	double xn1 = xn;
	double vn1 = vn;
	while ( true )
	{
		dx = ( - F(xn, xn1, vn, vn1, dt) * a22(xn1, dt) -  ( - G(xn, xn1, vn, vn1, dt) ) * a12(dt)  )      		   / (  a11() * a22(xn1, dt) - a12(dt) * a21(xn1, vn1, dt)  );
		dv = ( a11() * ( - G(xn, xn1, vn, vn1, dt) )	-  a21( xn1, vn1, dt ) * ( - F(xn, xn1, vn, vn1, dt) ) )  /  (  a11() * a22(xn1, dt) - a12(dt) * a21(xn1, vn1, dt)  );
				
		xn1 = xn1 + dx;
		vn1 = vn1 + dv;
		
		if (fabs(dx) < eps && fabs(dv) < eps)
			break;
	}
	
	return std::pair<double, double>( xn1, vn1 );
}



std::pair<double,double> RK2_method( double xn, double vn, double dt)
{
	double k1_x = vn;
	double k1_v = g( xn, vn );
	
	double k2_x = vn + dt * k1_v;
	double k2_v = g( xn + dt * k1_x, vn + dt * k1_v );
	
	xn = xn + ( dt / 2. ) * ( k1_x + k2_x );
	vn = vn + ( dt / 2. ) * ( k1_v + k2_v );
	
	return std::pair<double, double>( xn, vn );
}



double max( double num1, double num2)
{
	return num1 > num2 ? num1 : num2;
}


void time_step_control(double TOL, FILE* file, std::pair<double,double> (* method)( double xn, double vn, double dt) )
{
	double t = 0.;
	double dt = dt_0;
	double xn = x_0;
	double vn = v_0;
	double E_x = 0., E_v = 0.;
	double C_x = 0., C_v = 0.;
	
	std::pair<double,double> two_dt_steps( xn, vn );
	std::pair<double,double> one_doubled_dt_step( xn, vn );
	
	do{
		
		two_dt_steps = method( xn, vn, dt );
		two_dt_steps = method( two_dt_steps.first, two_dt_steps.second, dt );
		
		one_doubled_dt_step = method( xn, vn, 2 * dt );
		
		C_x = one_doubled_dt_step.first  - two_dt_steps.first;
		C_v = one_doubled_dt_step.second - two_dt_steps.second;
		
		E_x = C_x / ( pow( 2., p ) - 1. );
		E_v = C_v / ( pow( 2., p ) - 1. );
		
		if ( max( fabs(E_x), fabs(E_v) ) < TOL )
		{
			t = t + 2 * dt;
			xn = two_dt_steps.first;
			vn = two_dt_steps.second;
			fprintf(file, " %g  \t%g  \t%g  \t%g\n", t, dt, xn, vn);
		}
		
		dt = pow( ( S * TOL ) / max( fabs(E_x), fabs(E_v) ), ( 1. / ( p + 1. ) ) ) * dt;
		
	} while ( t < t_max);
}



int main()
{
	
	double TOL[]  = { pow(10, (-2.)),pow(10, (-5.)) };
	
	FILE* file_1_TOL_1 = fopen("Metoda_RK2_TOL_1.dat", "w");
	FILE* file_1_TOL_2 = fopen("Metoda_RK2_TOL_2.dat", "w");
	FILE* file_2_TOL_1 = fopen("Metoda_trapezow_TOL_1.dat", "w");
	FILE* file_2_TOL_2 = fopen("Metoda_trapezow_TOL_2.dat", "w");
	
	
	time_step_control( TOL[0], file_1_TOL_1, &RK2_method );
	time_step_control( TOL[1], file_1_TOL_2, &RK2_method );
	
	time_step_control( TOL[0], file_2_TOL_1, &trapezoidal_method );
	time_step_control( TOL[1], file_2_TOL_2, &trapezoidal_method );
	
	
	fclose(file_1_TOL_1);
	fclose(file_1_TOL_2);
	fclose(file_2_TOL_1);
	fclose(file_2_TOL_2);
    
	return 0;
}
