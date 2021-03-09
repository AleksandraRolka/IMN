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
#include <vector>


// parametry  
#define eps 1.0
#define delta 0.1
#define n_x 150
#define n_y 100
#define V_1 10.
#define V_2 0.
#define x_max delta * n_x
#define y_max delta * n_y
#define sigma_x 0.1 * x_max
#define sigma_y 0.1 * y_max
#define TOL pow( 10., -8. )


double rho(double x, double y)
{
	double rho_1 =  2 * exp( ( -pow( x - 0.35 * x_max, 2. ) / pow( sigma_x, 2. ) ) - ( pow( y - 0.25 * y_max, 2. ) / pow( sigma_y, 2. ) ) );
	double rho_2 = -2 * exp( ( -pow( x - 0.65 * x_max, 2. ) / pow( sigma_x, 2. ) ) - ( pow( y - 0.25 * y_max, 2. ) / pow( sigma_y, 2. ) ) );
	double rho_3 =    - exp( ( -pow( x - 0.35 * x_max, 2. ) / pow( sigma_x, 2. ) ) - ( pow( y - 0.75 * y_max, 2. ) / pow( sigma_y, 2. ) ) );
	double rho_4 =      exp( ( -pow( x - 0.65 * x_max, 2. ) / pow( sigma_x, 2. ) ) - ( pow( y - 0.75 * y_max, 2. ) / pow( sigma_y, 2. ) ) );

	return rho_1 + rho_2 + rho_3 + rho_4;
}

double count_S(double** V, double** rho)
{
	double s = 0.;
	
	for ( int i = 0; i < n_x; i++ ){
		for ( int j = 0; j < n_y; j++ )
		{
			s += pow( delta, 2. ) * ( 0.5 * pow( ( V[i+1][j] - V[i][j] ) / delta, 2. ) + 0.5 * pow( ( V[i][j+1] - V[i][j] ) / delta, 2. ) -  rho[i][j] * V[i][j] );
		}
	}
	return s;
}

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
double** make_rho_tab()
{
	double **rho;
    rho = new double* [n_x];

    for (int i = 0; i < n_x; i++){
        rho[i] = new double[n_y];
	}
	
    return rho;
}


void delete_tab(double** V)
{
    for (int i = 0; i <= n_x; i++)
        delete[] V[i];
	

    delete[] V;
}
void delete_rho_tab(double** V)
{
    for (int i = 0; i < n_x; i++)
        delete[] V[i];
	

    delete[] V;
}

void global_relaxation(double omega_G)
{
	std::string filename_S = "global_relaxation_S_" + std::to_string(omega_G) + ".dat";
	FILE *file_S = fopen(&filename_S[0], "w");
	
	std::string filename_V = "global_relaxation_V_" + std::to_string(omega_G) + ".dat";
	FILE *file_V = fopen(&filename_V[0], "w");
	
	std::string filename_err = "global_relaxation_err_" + std::to_string(omega_G) + ".dat";
	FILE *file_err = fopen(&filename_err[0], "w");
	
	
	double **V_s, **V_n, **rho_tab;
    V_s = make_and_init_V_tab();
    V_n = make_and_init_V_tab();
    rho_tab = make_rho_tab();
	int it = 0;
	
	for ( int i = 0; i < n_x; i++ ){
		for ( int j = 0; j < n_y; j++ ) {
			rho_tab[i][j] = rho( i * delta , j * delta );
		}
	}
	
	for (int i = 0; i <= n_x ; i++ )
	{
		V_s[i][0] = V_1;
		V_n[i][0] = V_1;
		V_s[i][n_y] = V_2;
		V_n[i][n_y] = V_2;
	}
	
	double S_prev = 0.;
	double S = count_S(V_s, rho_tab);
	
	while(true)
	{
		S_prev = S;
		
		for ( int i = 1; i < n_x; i++ ){
			for ( int j = 1; j < n_y; j++ )
			{
				V_n[i][j] = 0.25 * ( V_s[i+1][j] + V_s[i-1][j] + V_s[i][j+1] + V_s[i][j-1] + ( pow( delta, 2. ) / eps ) * rho_tab[i][j] );
			}
		}
		
		for (int j = 0; j < n_y; j++){
            V_n[0][j] = V_n[1][j];
            V_n[n_x][j] = V_n[n_x - 1][j];
        }
		
		for ( int i = 0; i <= n_x; i++ ){
			for ( int j = 0; j <= n_y; j++ )
			{
				V_s[i][j] = ( 1.0 - omega_G ) * V_s[i][j] + omega_G * V_n[i][j];
			}
		}
		
		
		S = count_S(V_n, rho_tab);
		
		fprintf(file_S, "%d \t%g\n", it, S);
		
		if( fabs( (S - S_prev ) / S_prev ) < TOL )
		{
			break;
		}
		
		it++;
	}
	
	
	for ( int i = 0; i <= n_x; i++ ){
		for ( int j = 0; j <= n_y; j++ )
		{
			fprintf(file_V, "%d\t%d\t%g\n", i, j, V_n[i][j]);
		}
		fprintf(file_V, "\n");
	}
	
	double solution_err;
	
	for ( int i = 1; i < n_x; i++ ){
		for ( int j = 1; j < n_y; j++ )
		{
			//solution_err = pow( delta, 2. ) * V_n[i][j] + rho_tab[i][j] / eps; 
			solution_err = ( V_n[i + 1][j] - 2 * V_n[i][j] + V_n[i - 1][j] ) / ( delta * delta ) + ( V_n[i][j + 1] - 2 * V_n[i][j] + V_n[i][j - 1] ) / ( delta * delta ) + rho_tab[i][j] / eps;
			
			fprintf(file_err, "%g\t%g\t%g\n", i * delta, j * delta, solution_err);
		}
		fprintf(file_err, "\n");
	}
	
	delete_tab(V_s);
	delete_tab(V_n);
	delete_rho_tab(rho_tab);
	
	fclose(file_S);
	fclose(file_V);
	fclose(file_err);
}

void local_relaxation(double omega_L)
{
	std::string filename_S = "local_relaxation_S_" + std::to_string(omega_L) + ".dat";
	FILE *file_S = fopen(&filename_S[0], "w");
	
	
	double **V, **rho_tab;
    V = make_and_init_V_tab();
    rho_tab = make_rho_tab();
	int it = 0;
	
	
	for ( int i = 0; i < n_x; i++ ){
		for ( int j = 0; j < n_y; j++ )
		{
			rho_tab[i][j] = rho( i * delta, j * delta );
		}
	}
	
	for (int i = 0; i <= n_x; i++ )
	{
		V[i][0] = V_1;
		V[i][n_y] = V_2;
	}
	
	double S_prev = 0.;
	double S = count_S(V, rho_tab);
		
	while(true)
	{
		S_prev = S;
		
		for ( int i = 1; i < n_x; i++ ){
			for ( int j = 1; j < n_y; j++ )
			{
				V[i][j] = ( 1. - omega_L ) * V[i][j] + ( omega_L / 4. ) * ( V[i+1][j] + V[i-1][j] + V[i][j+1] + V[i][j-1] + ( pow( delta, 2. ) / eps ) * rho_tab[i][j] );
			}
		}
		
		for (int j = 1; j < n_y; j++){
            V[0][j] = V[1][j];
            V[n_x][j] = V[n_x - 1][j];
        }
		
		S = count_S(V, rho_tab);
		
		fprintf(file_S, "%d \t%g\n", it, S);
		
		if( fabs( (S - S_prev ) / S_prev ) < TOL )
		{
			break;
		}
		
		it++;
	}
	
	delete_tab(V);
	delete_rho_tab(rho_tab);
	
	fclose(file_S);
}

int main()
{
    global_relaxation(0.6);
    global_relaxation(1.0);
	
    local_relaxation(1.0);
    local_relaxation(1.4);
    local_relaxation(1.8);
    local_relaxation(1.9);
	
	return 0;
}
