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
#include "mgmres.h"
#include "mgmres.c"


// parametry  
#define delta 0.1

double rho_1(double x, double y, double x_max, double y_max, double sigma)
{
	return  exp( -pow( x - 0.25 * x_max , 2. ) / pow( sigma , 2. ) - pow( y - 0.5 * y_max, 2. ) / pow( sigma, 2.) );
}
double rho_2(double x, double y, double x_max, double y_max, double sigma)
{
	return -exp( -pow( x - 0.75 * x_max , 2. ) / pow( sigma , 2. ) - pow( y - 0.5 * y_max, 2. ) / pow( sigma, 2.) );
}
double rho(double x, double y, double x_max, double y_max, double sigma)
{
	return rho_1(x, y, x_max, y_max, sigma) + rho_2(x, y, x_max, y_max, sigma);
}


void solve_poisson(int n_x, int n_y, double eps_1, double eps_2, double V_1, double V_2, double V_3, double V_4, bool rhoNull)
{
	std::string r ="";
	if(rhoNull){
		r += "_rhoNull";
	}
	
	std::string f_a_name = "a____n_x=" + std::to_string(n_x) + "_n_y=" + std::to_string(n_y) + "_eps_1=" + std::to_string(eps_1) + "_eps_2=" + std::to_string(eps_2) + r + ".dat";
	
	std::string f_b_name = "b____n_x=" + std::to_string(n_x) + "_n_y=" + std::to_string(n_y) + "_eps_1=" + std::to_string(eps_1) + "_eps_2=" + std::to_string(eps_2) + r + ".dat";
	
	std::string f_V_name = "V____n_x=" + std::to_string(n_x) + "_n_y=" + std::to_string(n_y) + "_eps_1=" + std::to_string(eps_1) + "_eps_2=" + std::to_string(eps_2) + r + ".dat";
	
	
	// FILE *file_a = fopen(&f_a_name[0], "w");
	// FILE *file_b = fopen(&f_b_name[0], "w");
	FILE *file_V = fopen(&f_V_name[0], "w");
	
	
	int N = ( n_x + 1 ) * ( n_y + 1 );
	
	double* a = new double [5 * N];
	int* ia = new int [N+1];
	int* ja = new int [5 * N];
	double* b = new double [N];
	double* V = new double [N];
	double* eps_l = new double [N + n_x + 1];
	
	int k = -1;
	int brzeg, i, j, nz_num;
	double Vb;
	double x, y;
	double x_max = delta * n_x;
	double y_max = delta * n_y;
	double sigma = x_max * 0.1;
	
	int itr_max = 500;
	int mr = 500;
	double tol_abs= pow( 10., -8. );
    double tol_rel= pow( 10., -8. );	
	
	
	
	for( int i = 0; i < N + 1; i++){
		ia[i] = -1;
	}
	
	for( int l = 0; l < N + n_x + 1; l++)
	{
		j = floor( l / (n_x + 1) );
        i = l - j * (n_x + 1);
		
		eps_l[l] = (i <= n_x / 2.) ? eps_1 : eps_2;
		//printf("eps[%d] = %g\n", l, eps_l[l]);
	}

	
	for( int l = 0; l < N; l++)	{
		
		
		j = floor( l / (n_x + 1) );
        i = l - j * (n_x + 1);
		
		brzeg = 0;
		Vb = 0.;
		
		
		if( i == 0 ){		// lewy brzeg
			brzeg = 1;
			Vb = V_1;
		}
		if( j == n_y ){		// gorny brzeg
			brzeg = 1;
			Vb = V_2;
		}
		if( i == n_x ){		// prawy brzeg
			brzeg = 1;
			Vb = V_3;
		}
		if( j == 0 ){		// dolny brzeg
			brzeg = 1;
			Vb = V_4;
		}
		x = i * delta;
		y = j * delta;
		
		
		// wypelniamy wektor wyrazow wolnych
		if (rhoNull){
			b[l] = 0.0;
		}else{		// jesli w srodku jest gestosc
			b[l] = - rho( x, y, x_max, y_max, sigma );
		}
		
		if( brzeg == 1 ){		// wymuszamy wartosc potencjalu na brzegu
			b[l] = Vb;
		}
		
		// wypełniamy elementy macierzy A
		ia[l] = -1; // wskaźnik dla pierwszego el . w wierszu
		
		
		// lewa skrajna przekatna
		if( l - n_x - 1 >= 0 && brzeg == 0 ){
			k++;
			if ( ia[l] < 0 ){ 
				ia[l] = k;
				}
			a[k]= eps_l[l] / pow( delta, 2. );
			ja[k]= l - n_x - 1;
		}
		
		// poddiagonala
		if( l - 1 >= 0 && brzeg == 0 ){
			k++;
			if( ia[l] < 0){
				ia[l] = k;
			}
			a[k] = eps_l[l] / pow( delta, 2. );
			ja[k] = l - 1;
		}
	
		// diagonala
		k ++;
		if ( ia[l] < 0 ){
			ia[l] = k;
		}
		if( brzeg == 0){
			a[k] = -( 2 * eps_l[l] + eps_l[l + 1] + eps_l[l + n_x + 1] ) / pow( delta, 2. );
		}else{
			a[k] = 1;
		}
		ja[k] = l;
		
		// naddiagonala
		if( l < N && brzeg == 0 ){
			k++;
			a[k] = eps_l[l+1] / pow( delta, 2. );
			ja[k] = l + 1;
		}

		// prawa skrajna przekątna
		if( l < ( N - n_x - 1) && brzeg == 0 ){
			k++;
			a[k] = eps_l[l + n_x + 1] / pow( delta, 2. );
			ja[k] = l + n_x + 1;
		}
		
	}
	
	nz_num = k + 1;
	ia[N] = nz_num;
	
	pmgmres_ilu_cr( N, nz_num, ia, ja, a, V, b, itr_max, mr, tol_abs, tol_rel );
	
	for( int l = 0; l < N; l++ )
	{
		j = floor( l / (n_x + 1) );
        i = l - j * (n_x + 1);
		
		// fprintf(file_a, "%d\t%d\t%d\t%g\n", l, i, j, a[l]);
		// fprintf(file_b, "%d\t%d\t%d\t%g\n", l, i, j, b[l]);
		
        fprintf(file_V, "%d\t%g\t%g\t%g\n", l, i * delta, j * delta, V[l]);
		if ( i == n_x )
			fprintf(file_V, "\n");
	}
	

	delete(eps_l);
	delete(V);
	delete(b);
	delete(ja);
	delete(ia);
	delete(a);
	
	// fclose(file_a);
	// fclose(file_b);
	fclose(file_V);
	
}




int main()
{

	int n_x, n_y;
	double eps_1, eps_2;
	double V_1, V_2, V_3, V_4;
	bool rhoNull;
	
	
	
	n_x = 4;
	n_y = 4;
	eps_1 = 1;
	eps_2 = 1;
	V_1 =  10;
	V_3 =  10;
	V_2 = -10;
	V_4 = -10;
	rhoNull = true;
	
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	
	n_x = 50;
	n_y = 50;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	n_x = 100;
	n_y = 100;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	n_x = 200;
	n_y = 200;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	
	
	n_x = 100;
	n_y = 100;
	V_1 = 0.;
	V_2 = 0.;
	V_3 = 0.;
	V_4 = 0.;
	rhoNull = false;
	

	eps_1 = 1;
	eps_2 = 1;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	eps_1 = 1;
	eps_2 = 2;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);
	
	eps_1 = 1;
	eps_2 = 10;
	solve_poisson( n_x, n_y, eps_1, eps_2, V_1, V_2, V_3, V_4, rhoNull);

}