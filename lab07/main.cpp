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

#define delta 0.01
#define rho 1.0
#define mi 1.0
#define n_x 200
#define n_y 90
#define i_1 50
#define j_1 55
#define IT_MAX 20000


double** make_and_init_tab(int size1, int size2)
{
	
	double **tab;
    tab= new double* [size1];

    for (int i = 0; i < size1; i++){
        tab[i] = new double[size2];
	}
	
	for ( int i = 0; i < size1; i++ ){
		for ( int j = 0; j < size2; j++ )
		{
			tab[i][j] = 0.;
		}
	}
	
    return tab;
}

void delete_tab(double** tab, int size1, int size2)
{
    for (int i = 0; i < size1; i++)
        delete[] tab[i];
	

    delete[] tab;
}

void WB_psi(double Q_in, double Q_out, double* y, double** psi)
{
	// brzeg A (wejscie)
	for( int j = j_1; j <= n_y; j++ )
		psi[0][j] = ( Q_in / (2 * mi) ) * ( pow(y[j], 3.) / 3. - ( pow(y[j], 2.) / 2. ) * ( y[j_1] + y[n_y] ) + y[j] * y[j_1] * y[n_y]);
	
	// brzeg C (wyjscie)
	for( int j = 0; j <= n_y; j++ )
		psi[n_x][j] = ( Q_out / (2 * mi) ) * ( pow(y[j], 3.) / 3. -  ( pow(y[j], 2.) / 2. ) * y[n_y] ) + ( Q_in * pow(y[j_1], 2.) * ( -y[j_1] + 3 * y[n_y] ) ) / ( 12 * mi );
	
	// brzeg B
	for( int i = 1; i < n_x; i++ )
		psi[i][n_y] = psi[0][n_y];
	
	// brzeg D
	for( int i = i_1; i < n_x; i++ )
		psi[i][0] = psi[0][j_1];
	
	// brzeg E
	for( int j = 1; j <= j_1; j++ )
		psi[i_1][j] = psi[0][j_1];
	
	// brzeg F
	for( int i = 1; i <= i_1; i++ )
		psi[i][j_1] = psi[0][j_1];
	
}



void WB_dzeta(double Q_in, double Q_out, double* y, double** psi, double** dzeta)
{
	// brzeg A (wejscie)
	for( int j = j_1; j <= n_y; j++ )
		dzeta[0][j] = ( Q_in / (2 * mi) ) * ( 2 * y[j] - y[j_1] - y[n_y] );
	
	// brzeg C (wyjscie)
	for( int j = 0; j <= n_y; j++ )
		dzeta[n_x][j] = ( Q_out / (2 * mi) ) * ( 2 * y[j] - y[n_y] );
	
	// brzeg B
	for( int i = 1; i < n_x; i++ )
		dzeta[i][n_y] = ( 2 / pow(delta, 2.) ) * ( psi[i][n_y-1] - psi[i][n_y] );
	
	// brzeg D
	for( int i = i_1; i < n_x; i++ )
		dzeta[i][0] = ( 2 / pow(delta, 2.) ) * ( psi[i][1] - psi[i][0] );
	
	// brzeg E
	for( int j = 1; j < j_1; j++ )
		dzeta[i_1][j] = ( 2 / pow(delta, 2.) ) * ( psi[i_1+1][j] - psi[i_1][j] );
	
	// brzeg F
	for( int i = 1; i <= i_1; i++ )
		dzeta[i][j_1] = ( 2 / pow(delta, 2.) ) * ( psi[i][j_1+1] - psi[i][j_1] );
	
	// wierzchołek E/F
	dzeta[i_1][j_1] = 0.5 * ( dzeta[i_1-1][j_1] + dzeta[i_1][j_1-1] );
	
}


bool brzeg(int i, int j)
{
	return ((i == i_1)&&(j <= j_1)) || ((j == j_1)&&(i <= i_1)) || ( j == 0 ) || (  i == 0 ) || ( i == n_x ) || ( j == n_y );
}



void relaxation_NavierStokes( double Q_in )
{
	std::string fn = "Q=" + std::to_string(Q_in) + ".dat";
	FILE *file_Q = fopen(&fn[0], "w");
	
	double Q_out;
	int omega;
	double** psi, ** dzeta;
	double** u;
	double** v;
	double* y = new double [n_y + 1];

	
	psi = make_and_init_tab( n_x + 1, n_y + 1);
	dzeta = make_and_init_tab( n_x + 1, n_y + 1);
	u = make_and_init_tab( n_x + 1, n_y + 1);
	v = make_and_init_tab( n_x + 1, n_y + 1);
	
	for (int i = 0; i <= n_y; i++){
        y[i] =  delta * i;
	}
	
	Q_out = Q_in * ( ( pow(y[n_y], 3.) - pow(y[j_1], 3.) - 3 * y[j_1] * pow(y[n_y], 2.) + 3 * pow(y[j_1], 2.) * y[n_y] ) / pow(y[n_y], 3.) );

	
	// ustalamy WB psi:
	WB_psi( Q_in, Q_out, y, psi );
	
	// for(int i=0; i < n_x+1; i++){
		// for(int j=0; j<n_y+1; j++){
			// printf("[%d][%d] = %g\n",i,i,psi[i][j]);
		// }
		// printf("\n");
	// }
	
	for( int it = 1; it <= IT_MAX; it++)
	{
		if( it < 2000 ){
			omega = 0;
		}else{
			omega = 1;
		}
		
		for( int i = 1; i < n_x; i ++){
			for ( int j = 1; j < n_y; j ++)
			{
				
				if(!brzeg(i,j))
				{
					psi[i][j] = 0.25 * ( psi[i+1][j] + psi[i-1][j] + psi[i][j+1] + psi[i][j-1] - pow(delta, 2.) * dzeta[i][j] );
					
					dzeta[i][j] = 0.25 * ( dzeta[i+1][j] + dzeta[i-1][j] + dzeta[i][j+1] + dzeta[i][j-1] ) - omega * ( rho / ( 16. * mi ) ) * ( ( psi[i][j+1] - psi[i][j-1] )*( dzeta[i+1][j] - dzeta[i-1][j] ) - ( psi[i+1][j] - psi[i-1][j] )*( dzeta[i][j+1] - dzeta[i][j-1] ) ) ;
					
					u[i][j] =  ( psi[i][j + 1] - psi[i][j - 1] ) / ( 2 * delta );
                    v[i][j] = -( psi[i + 1][j] - psi[i - 1][j] ) / ( 2 * delta );
				}
			}
		}
		
		// kontrola WB dzeta:
		WB_dzeta( Q_in, Q_out, y, psi, dzeta );

		// kontrola błędu
		double gamma = 0.;
		int j_2 = j_1 + 2;
		for (int i = 1; i < n_x; i++)
            gamma += ( psi[i + 1][j_2] + psi[i - 1][j_2] + psi[i][j_2 + 1] + psi[i][j_2 - 1] - 4 * psi[i][j_2] - pow(delta, 2.) * dzeta[i][j_2] );
		printf("it = %d \t%g\n", it, gamma);
		
	}
	
	for (int i = 1; i < n_x; i++)
    {
        for (int j = 1; j < n_y; j++)
            if (i <= i_1 && j <= j_1){
               fprintf(file_Q, "%f\t %f\t %g\t %g\t %g\t %g\n", i * delta, j * delta, psi[i_1][j_1], dzeta[i_1][j_1], u[i][j], v[i][j]);
            }
			else
                fprintf(file_Q, "%f\t %f\t %g\t %g\t %g\t %g\n", i * delta, j * delta, psi[i][j], dzeta[i][j], u[i][j], v[i][j]);

        fprintf(file_Q, "\n");
    }
	
	
	
	delete(psi);
	delete(dzeta);
	delete(u);
	delete(v);
	delete[] y;
	
	fclose(file_Q);
}


int main()
{
	double Q1 = -1000.;
	double Q2 = -4000.;
	double Q3 =  4000.;
	
	relaxation_NavierStokes(Q1);
	relaxation_NavierStokes(Q2);
	relaxation_NavierStokes(Q3);

}