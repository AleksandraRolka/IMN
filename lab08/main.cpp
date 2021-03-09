/* 
 * KOMENDY : 
 *
 * make all - kompiluje program, uruchamia go tworząc odpowiednie pliki .dat oraz uruchamia skrypt gnuplota, który tworzy wykresy
 * make clean - usuwa plik wykonywalny 'main' oraz pliki z roszerzeniami .o, .dat, .png, .gif
 *
 */
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <cmath>


// parametry
#define delta 0.01
#define sigma (10 * delta)
#define n_x 400
#define n_y 90
#define i_1 200
#define i_2 210
#define j_1 50
#define x_A 0.45
#define y_A 0.45

#define M_PI 3.14159265358979323846



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

void delete_tab(double** tab)
{
    for (int i = 0; i <= n_x; i++)
        delete[] tab[i];
	

    delete[] tab;
}




void solve_AD(double D, int IT_MAX)
{	
	std::string fn1 = "u__D=" + std::to_string(D) + ".dat";
	std::string fn2 = "c__D=" + std::to_string(D) + ".dat";
	std::string fn3 = "x_sr__D=" + std::to_string(D) + ".dat";
	std::string fn4 = "vx_vy__D=" + std::to_string(D) + ".dat";
	FILE *file_u = fopen(&fn1[0], "w");
	FILE *file_c = fopen(&fn2[0], "w");
	FILE *file_x = fopen(&fn3[0], "w");
	FILE *file_vx_vy = fopen(&fn4[0], "w");
	
	
	
/////////////////////////////////////// utworzenie tablic ///////////////////////////////////////

	double** u_0, **u_1, ** psi, **v_x, **v_y;

	u_0 = make_and_init_tab( n_x + 1, n_y + 1);
	u_1 = make_and_init_tab( n_x + 1, n_y + 1);
	psi = make_and_init_tab( n_x + 1, n_y + 1);
	v_x = make_and_init_tab( n_x + 1, n_y + 1);
	v_y = make_and_init_tab( n_x + 1, n_y + 1);	
///////////////////////////////// wczytanie funkcji strumienia /////////////////////////////////

	std::ifstream file; 
	file.open("psi.txt"); 
	
	if(file.fail())
	{
		printf("Coś poszło nie tak przy otwieraniu pliku\n");
	}
	else{
		int ii, jj;
		for (int i = 0; i <= n_x; i++){
			for (int j = 0; j <= n_y; j++){
				file >> ii >> jj >> psi[i][j];
			}
		}
	}
////////////////////////////////// wyznaczenie pola predkosci //////////////////////////////////

	// poza brzegami i zastawką
	for (int i = 1; i < n_x; i++){
		for (int j = 1; j < n_y; j++){
			v_x[i][j] =  ( psi[i][j+1] - psi[i][j-1] ) / ( 2 * delta );
			v_y[i][j] = -( psi[i+1][j] - psi[i-1][j] ) / ( 2 * delta );
		}
	}
	// na zastawce
	for (int i = i_1; i <= i_2; i++){
		for (int j = 0; j <= j_1; j++){
			v_x[i][j] = 0.;
			v_y[i][j] = 0.;
		}
	}
	// na dolnym i górnym brzegu
	for (int i = 1; i < n_x; i++){
		v_x[i][0] = 0.;
		v_y[i][0] = 0.;
		v_x[i][n_y] = 0.;
		v_y[i][n_y] = 0.;
	}
	// na lewym i prawym brzegu
	for (int j = 0; j <= n_y; j++){
		v_x[0][j] = v_x[1][j];
		v_x[n_x][j] = v_x[n_x-1][j];
		v_y[0][j] = v_y[1][j];
		v_y[n_x][j] = v_y[n_x-1][j];
	}
	
	for (int i = 0; i <= n_x; i++){
		for (int j = 0; j <= n_y; j++){
			fprintf(file_vx_vy, "%g\t%g\t%g\t%g\n",i * delta,j * delta,v_x[i][j], v_y[i][j]);
		}
		fprintf(file_vx_vy, "\n");
	}
/////////////////////////////////// wyznaczenie v_max i delta_t ///////////////////////////////////
	
	double delta_t;
	//double max_modul = sqrt( pow( v_x[0][0] ,2. ) + pow( v_y[0][0] ,2. ) );
	double max_modul = 0.;
	double temp;
	double c, x_sr;
	
	for (int i = 0; i <= n_x; i++){
		for (int j = 0; j <= n_y; j++){
			temp = sqrt( pow( v_x[i][j] ,2. ) + pow( v_y[i][j] ,2. ) );
			if (temp > max_modul){
				max_modul = temp;
			}
		}
	}
	
	delta_t = delta / ( 4 * max_modul );
	// printf("max_modul = %g\n", max_modul );
	
///////////////////////////////////// inicjalizacja gęstości  /////////////////////////////////////

	for (int i = 0; i <= n_x; i++){
		for (int j = 0; j <= n_y; j++){
			u_0[i][j] = ( 1. / ( 2 * M_PI * pow( sigma ,2. ) ) ) * exp( -( pow( i * delta - x_A, 2. ) + pow( j * delta - y_A, 2. ) ) / ( 2. * pow( sigma ,2. ) ) );
		}
	}
	
	int Tk = IT_MAX / 20;
	int kk = 1;
	
	for (int it =1 ; it <= IT_MAX; it++)
	{
		
		// inicjalizacja kolejnego kroku
		for (int i = 0; i <= n_x; i++){
			for (int j = 0; j <= n_y; j++){
				u_1[i][j] = u_0[i][j];
			}
		}
		
		for (int k =1 ; k <= 20; k++)
		{
			for (int i = 0; i <= n_x; i++){
				for (int j = 1; j < n_y; j++){
					
					if(  i < i_1 || i > i_2 || j > j_1 ) 
					{
					
						if( i == 0 )
						{
							u_1[i][j] = ( 1. / ( 1. + ( 2 * D * delta_t ) / pow( delta, 2. ) ) ) * 
										( u_0[i][j] - 0.5 * delta_t * v_x[i][j] * 
										( ( u_0[i+1][j] - u_0[n_x][j] ) / ( 2 * delta ) + ( u_1[i+1][j] - u_1[n_x][j] ) / ( 2 * delta ) ) 
										- 0.5 * delta_t * v_y[i][j] * 
										( ( u_0[i][j+1] - u_0[i][j-1] ) / ( 2 * delta ) + ( u_1[i][j+1] - u_1[i][j-1] ) / ( 2 * delta ) )
										+ 0.5 * delta_t * D * 
										( ( u_0[i+1][j] + u_0[n_x][j] + u_0[i][j+1] + u_0[i][j-1] - 4. * u_0[i][j] ) / pow( delta, 2. ) 
										+ ( ( u_1[i+1][j] + u_1[n_x][j] + u_1[i][j+1] + u_1[i][j-1] ) / pow( delta, 2. ) ) ) );
						}
						else if ( i == n_x )
						{
							u_1[i][j] = ( 1. / ( 1. + ( 2 * D * delta_t ) / pow( delta, 2. ) ) ) * 
										( u_0[i][j] - 0.5 * delta_t * v_x[i][j] * 
										( ( u_0[0][j] - u_0[i-1][j] ) / ( 2 * delta ) + ( u_1[0][j] - u_1[i-1][j] ) / ( 2 * delta ) ) 
										- 0.5 * delta_t * v_y[i][j] * 
										( ( u_0[i][j+1] - u_0[i][j-1] ) / ( 2 * delta ) + ( u_1[i][j+1] - u_1[i][j-1] ) / (2 * delta ) ) 
										+ 0.5 * delta_t * D * 
										( ( u_0[0][j] + u_0[i-1][j] + u_0[i][j+1] + u_0[i][j-1] - 4. * u_0[i][j] ) / pow(delta, 2. ) 
										+ ( ( u_1[0][j] + u_1[i-1][j] + u_1[i][j+1] + u_1[i][j-1] ) / pow( delta, 2. ) ) ) );
						
					
						}
						else 
						{
							u_1[i][j] = ( 1. / ( 1. + ( 2 * D * delta_t ) / pow( delta, 2. ) ) ) * 
									( u_0[i][j] - 0.5 * delta_t * v_x[i][j] * 
									( ( u_0[i+1][j] - u_0[i-1][j] ) / ( 2 * delta ) + ( u_1[i+1][j] - u_1[i-1][j] ) / ( 2 * delta ) ) 
									- 0.5 * delta_t * v_y[i][j] * 
									( ( u_0[i][j+1] - u_0[i][j-1] ) / ( 2 * delta ) + ( u_1[i][j+1] - u_1[i][j-1] ) / (2 * delta ) ) 
									+ 0.5 * delta_t * D * 
									( ( u_0[i+1][j] + u_0[i-1][j] + u_0[i][j+1] + u_0[i][j-1] - 4. * u_0[i][j] ) / pow(delta, 2. ) 
									+ ( ( u_1[i+1][j] + u_1[i-1][j] + u_1[i][j+1] + u_1[i][j-1] ) / pow( delta, 2. ) ) ) );
						}
					}
				}
			}
			
			
			
			
			
		}
		// zachowanie rozwiazania do nastepnego wywolania
		for (int i = 0; i <= n_x; i++){
			for (int j = 0; j <= n_y; j++){
				u_0[i][j] = u_1[i][j];
			}
		}
		// wyznaczenie i zapisanie do pliku: c oraz x_sr		
		c = 0.;
		x_sr = 0.;
		for (int i = 0; i <= n_x; i++){
			for (int j = 0; j <= n_y; j++){
				c += u_0[i][j];
				x_sr += i * delta * u_0[i][j];
			}
		}
		c *= pow( delta,2. );
		x_sr *= pow( delta,2. );
		
		fprintf(file_c, "%g \t %g\n", it * delta_t, c);
		fprintf(file_x, "%g \t %g\n", it * delta_t, x_sr);
		
		if(it == (Tk * kk))
		{
			for (int i = 0; i <= n_x; i++){
				for (int j = 0; j <= n_y; j++){
					fprintf(file_u, "%d\t%d\t%g\n", i , j, u_1[i][j]);
				}
				fprintf(file_u, "\n");
			}
			kk++;
			fprintf(file_u, "\n");
		}
		
	}
	
	// for (int i = 0; i <= n_x; i++){
			// for (int j = 0; j <= n_y; j++){
				// fprintf(file_u, "%d\t%d\t%g\n", i , j, u_1[i][j]);
			// }
		// fprintf(file_u, "\n");
	// }

	delete_tab(u_0);
	delete_tab(u_1);
	delete_tab(psi);
	delete_tab(v_x);
	delete_tab(v_y);
	
	fclose(file_u);
	fclose(file_c);
	fclose(file_x);
	fclose(file_vx_vy);
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	double D = 0.;
	int IT_MAX = 10000;
	
	solve_AD(D,IT_MAX);
	D = 0.1;
	solve_AD(D,IT_MAX);

}