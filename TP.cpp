/*
 *  FirstProgram.cpp
 *  
 *
 *  Created by Olivier Strauss on 10/10/16.
 *  Copyright 2016 LIRMM. All rights reserved.
 *
 */

#include "CImg.h"
#include "string.h"

using namespace cimg_library;

// Calcul de la multiplication C = A.B au sens des matrices.
// C est NlinxNcol, A est NlinxNcom, B est NcomxNcol
// Le pointeurs doivent etre alloues correctement avant l'appel
int MatMult(double *A, double *B, double *C, int Nlin, int Ncom, int Ncol)
{
	int lin, col, k ;
	double *ptA, *ptB, *ptC ;
	
	ptC = C ;
	for(lin = 0 ; lin<Nlin ; lin++)
	{
		for(col = 0 ; col<Ncol ; col++, ptC++)
		{
			ptA = A + (lin*Ncom) ;
			ptB = B + col ;
			(*ptC) = 0.0 ;
			for(k = 0 ; k<Ncom ; k++)
			{
				(*ptC) += (*ptA) * (*ptB) ;
				ptA++ ; 
				ptB+= Ncol ; 
			}
		}
	}
	return 1 ;
}

// Donne des 2 points d'intersection entre la droite
// de parametre L et le carre de dimension Dx, Dy
int Intersection( double L[3], int Dx, int Dy, int x_inter[2], int y_inter[2] )
{
	double a=L[0], b=L[1], c=L[2] ;
	double x[4], y[4] ;
	int nb_points_ok = 0, n ;
	x[0] = 0 ;
	x[1] = Dx-1 ;
	y[2] = 0 ;
	y[3] = Dy-1 ;
	
	if(fabs(L[0])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		b = -L[1]/L[0] ;
		c = -L[2]/L[0] ;
		x[2] = b * y[2] + c ;
		x[3] = b * y[3] + c ;
	}
	else 
	{
		x[2] = -Dx ;
		x[3] = -Dx ;
	}

	if(fabs(L[1])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		a = -L[0]/L[1] ;
		c = -L[2]/L[1] ;
		y[0] = a * x[0] + c ;
		y[1] = a * x[1] + c ;
	}
	else 
	{
		y[0] = -Dy ;
		y[1] = -Dy ;
	}
	
	for(n=0 ; n<4 ; n++)
	{
		if( (x[n]>=0.0) && (y[n]>=0.0) && (x[n]<=Dx) && (y[n]<=Dy) && (nb_points_ok<2) )
		{
			x_inter[nb_points_ok] = (int)floor(x[n]+0.5) ;
			y_inter[nb_points_ok] = (int)floor(y[n]+0.5) ;
			nb_points_ok ++ ;
		}
	}
	
	if(nb_points_ok==2) return 1 ; 
	else return 0 ;	
}


void printMatrice(CImg <double>::iterator it, int nLine, int nCol){
	//	CImg <double>::iterator it= res.begin() ;
	for(int lin=0 ; lin<nLine ; lin++)
	{
		for(int col=0 ; col<nCol ; col++, it++)
		{
			printf("[%g]",(*it)) ;
		}
		printf("\n") ;
	}
}



int main(int argc, char *argv[])
{
	int nombre_de_points = 8, n=2 ;

/*
point G n : 0 138 255 
point D n : 0 158 256 
point G n : 1 157 316 
point D n : 1 172 318 

point G n : 0 231 145 
point D n : 0 211 148 
point G n : 1 158 59 
point D n : 1 126 63 

point G n : 0 129 445 
point D n : 0 131 446 
point G n : 1 55 280 
point D n : 1 35 283 

point G n : 0 137 343 
point D n : 0 147 343 
point G n : 1 180 91 
point D n : 1 157 93

*/

	int xd[nombre_de_points] = {158,172      , 211, 126   , 131, 35,    147, 157};
	int yd[nombre_de_points] = {256, 318     , 148, 63    , 446, 283,   343, 93};

	int xg[nombre_de_points] = {138, 157    ,231, 158     , 129, 55,    137, 180};
	int yg[nombre_de_points] = {255, 316    ,145, 59      , 445, 280,   343, 91  };
	char droite_gaughe = 'd' ;






	if(argc<2) 
	{
		printf("\nCe programme a deux arguments d'appel qui sont les deux images droite et gauche\n") ;
		printf("Il faut le lancer de la forme ./Stereo ImageDroite.tif ImageGauche.tif\n") ;
		return 0 ;
	}
	
	// Chargement des deux images dont les noms ont été passés au programme principal
	CImg<unsigned char> imageD(argv[1]),imageG(argv[2]) ;
	// Definition des couleurs de trace (rouge, vert, bleu)
	const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
	// Creation des objets d'affichage
	CImgDisplay Droite_disp(imageD,"Image droite"), Gauche_disp(imageG,"Image gauche");
	



	// Definition d'une matrice de 5 lignes et 3 colonnes (et un plan) en double precision
	// remplie au depart de 0
	CImg <double> A(8,8,1,1,0) ; 

	CImg <double>::iterator it2 ; // defiition d'un iterateur (permet d'avoir le premier element de la matrice)
	int NlinA, NcolA, NlinB, NcolB, NlinC, NcolC, lin, col;
	
	NlinA = A.height() ;
	NcolA = A.width() ;
	
	// Remplissage de la matrice A avec des valeurs aleatoires entre 0 et 10
	//matrice_A.rand(0,10) ;
	
	// Affichage de la matrice A
	it2 = A.begin() ;
	for(lin=0 ; lin<NlinA ; lin++)
	{
		A[lin*NcolA] = xg[lin]*xd[lin];
		A[lin*NcolA+1] = xg[lin]*yd[lin];
		A[lin*NcolA+2] = xg[lin];
		A[lin*NcolA+3] = yg[lin]*xd[lin];
		A[lin*NcolA+4] = yg[lin]*yd[lin];
		A[lin*NcolA+5] = yg[lin];
		A[lin*NcolA+6] = xd[lin];
		A[lin*NcolA+7] = yd[lin];


		for(int  col=0 ; col<NcolA; col++, it2++)
		{
			
			printf("[%g]",(*it2)) ;
			//printf("%i, ", matrice[lin*Ncol+col]);
		}
		printf("\n") ;
	}


	
	// Selection de nombre_de_points paires de points (en commencant par l'image droite
/*	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed() && n<nombre_de_points) 
	{
		switch (droite_gaughe) 
		{
			case 'd' :
  			Gauche_disp.set_title("%s","Image gauche");
			Droite_disp.set_title("%s","Cliquez ici") ;
			Droite_disp.wait();
				if (Droite_disp.button() && Droite_disp.mouse_y()>=0) 
				{
					yd[n] = Droite_disp.mouse_y();
					xd[n] = Droite_disp.mouse_x();
					imageD.draw_circle(xd[n],yd[n],3,red,1.0,1).display(Droite_disp);
					droite_gaughe = 'g' ;
					
				} break ;
			case 'g' :
		  Droite_disp.set_title("%s","Image droite") ;
				Gauche_disp.set_title("%s","Cliquez ici");
				Gauche_disp.wait();
				if (Gauche_disp.button() && Gauche_disp.mouse_y()>=0) 
				{
					yg[n] = Gauche_disp.mouse_y();
					xg[n] = Gauche_disp.mouse_x();
					imageG.draw_circle(xg[n],yg[n],3,blue,1.0,1).display(Gauche_disp);
					droite_gaughe = 'd' ; 

					n++ ;
				} break ;
			default : break;
		}		
	}*/
	
	// Affichage de tous les points en vert
	for(n=0 ; n<nombre_de_points ; n++) 
	{
		imageD.draw_circle(xd[n],yd[n],3,green,1.0,1).display(Droite_disp);
		imageG.draw_circle(xg[n],yg[n],3,green,1.0,1).display(Gauche_disp);
		printf("point G n : %i %d %d \n",n, xg[n],yg[n]);
		printf("point D n : %i %d %d \n",n, xd[n],yd[n]);

		
	}
	
	// Selection de deux points dans l'image droite et affichage de la droite passant par ces deux points
/*	n=0 ;
	double delta, L[3] ;
	int x_inter[2], y_inter[2] ;
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed() && n<2) 
	{
		Droite_disp.set_title("%s","Cliquez deux points") ;
		Droite_disp.wait();
		if (Droite_disp.button() && Droite_disp.mouse_y()>=0) 
		{
			yd[n] = Droite_disp.mouse_y();
			xd[n] = Droite_disp.mouse_x();
			imageD.draw_circle(xd[n],yd[n],3,red,1.0,1).display(Droite_disp);
			n = n+1 ;
		}			
	}
	delta = (double)xd[0]*(double)yd[1] - (double)xd[1]*(double)yd[0] ;
	imageD.draw_line(xd[0],yd[0],xd[1],yd[1],green).display(Droite_disp);

	L[0] = (double)( yd[1] - yd[0] ) ;
	L[1] = (double)( xd[0] - xd[1] ) ;
	L[2] = (double)xd[1]*(double)yd[0] - (double)xd[0]*(double)yd[1] ;		
			
	n = Intersection(L, imageD.width(), imageD.height(), x_inter, y_inter ) ;
	if(n)	imageD.draw_line(x_inter[0],y_inter[0],x_inter[1],y_inter[1],red).display(Droite_disp);
	*/







 // Partie calcul sur les matrices
 
	// Definition d'une matrice de 5 lignes et 3 colonnes (et un plan) en double precision
	// remplie au depart de 0
	/*
	CImg <double> matrice_A(5,3,1,1,0) ; 
	CImg <double>::iterator it ; // defiition d'un iterateur (permet d'avoir le premier element de la matrice)
//	int lin, col, NlinA, NcolA, NlinB, NcolB, NlinC, NcolC;
	
	NlinA = matrice_A.height() ;
	NcolA = matrice_A.width() ;
	
	// Remplissage de la matrice A avec des valeurs aleatoires entre 0 et 10
	matrice_A.rand(0,10) ;
	
	// Affichage de la matrice A
	it = matrice_A.begin() ;
	for( lin=0 ; lin<NlinA ; lin++)
	{
		for( col=0 ; col<NcolA ; col++, it++)
		{
			printf("[%g]",(*it)) ;
		}
		printf("\n") ;
	}
	*/

	CImg <double> B(1,8,1,1,-1) ; 
	NlinB = B.height() ;
 	NcolB = B.width() ;
 	CImg <double> f(1,8,1,1,-1) ; 

	CImg <double> C(NlinA,NcolA,1,1,0) ;
 	NlinC = C.height() ;
 	NcolC = C.width() ;
	printf("\n matrice result A \n");
	printMatrice(A.begin(),  A.height(), A.width()  );
/*	CImg <double> At =	A.transpose();
	printf("\n matrice result At \n");
	printMatrice(At.begin(), At.height(), At.width());*/

			printf("\n matrice result B \n");
			printMatrice(B.begin(), B.height(), B.width());

//CImg <double> res(1,8,1,1,0) ;

	//f = (At *A)-1 * At * B
	//res = At.mul(A).transpose().mul(At).mul(B);
	//CImg <double> res = A.pseudoinvert().mul(B);
/*

	//C = (At * A)
	MatMult((double *)At.begin(), (double *)A.begin(),(double *)C.begin(), NlinC, NcolA, NcolC) ;
	
			printf("\nmatrice result C \n");
			printMatrice(C.begin(),   C.height(), C.width());
			//C = (At * A)-1
		//*C = C.invert();
		//	printf("\nmatrice result C-1 \n");
		//	printMatrice(C.begin(),   C.height(), C.width());
	//D = (At * A)-1 * At
	CImg <double> D(8,8,1,1,0) ;
	MatMult((double *)C.invert().begin(), (double *)At.begin(), (double *)D.begin(), NlinA, NcolA, NcolB) ;
	
			printf("\nmatrice result D \n");
			printMatrice(D.begin(),   D.height(), D.width());
			printf("\n matrice result B \n");
			printMatrice(B.begin(), B.height(), B.width());


	//res = (At * A)-1 * At * B
	*/
	MatMult((double *)A.pseudoinvert().begin(), (double *)B.begin(), (double *)f.begin(), NlinA, NcolA, NcolB) ;

	printf("\n matrice result f \n");
	printMatrice(f.begin(), f.height(), f.width());


	CImg <double> F(3,3,1,1,0);
	CImg <double>::iterator it_f = f.begin() ; 
//	CImg <double>::iterator it_F = F.begin() ; 
	for( lin=0 ; lin<3 ; lin++)
	{
		for( col=0 ; col<3 ; col++, it_f++)
		{	if(lin*3+col==8)
				F[lin*3+col]=1;
			else
			F[lin*3+col]=f[lin*3+col];

		}
		
	}
	
	printf("\n matrice result F \n");
	printMatrice(F.begin(), F.height(), F.width());




	n=0 ;
	double delta, L[3] ;
	int x_click[2], y_click[2] ;
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed() /*&& n<2*/) 
	{
		Droite_disp.set_title("%s","Cliquez un point point") ;
		Droite_disp.wait();
		if (Droite_disp.button() && Droite_disp.mouse_y()>=0) 
		{

			CImg <double> Md(3,1,1,1,0);
			CImg <double> Mg(3,1,1,1,0);
			CImg <double> Mg_vec(3,1,1,1,0);

			Md[0] = Droite_disp.mouse_y();
			Md[1] = Droite_disp.mouse_x();
			Md[2] = 1;

			//MatMult((double *)Md.begin(), (double *)F.begin(), (double *)Mg.begin(), 1, 3, 3);
			MatMult((double *)F.invert().begin(), (double *)Md.begin(), (double *)Mg_vec.begin(), 3, 3, 1);

				printf("\n matrice result Mg \n");
				printMatrice(Mg_vec.begin(), Mg_vec.height(), Mg_vec.width());


			//MatMult((double *)F.invert().begin(), (double *)Md.invert.begin(), (double *)Mg_vec.begin(), 1, 3, 3);


			imageD.draw_circle(Md[1],Md[0],3,red,1.0,1).display(Droite_disp);
		//	imageG.draw_circle(Mg[1],Mg[0],3,red,1.0,1).display(Gauche_disp);
		//	imageG.draw_line(Mg[1],Mg[0],Mg[1]*Mg_vec[1],Mg[0]*Mg_vec[0],red).display(Gauche_disp);
			//n = n+1 ;
			int x_inter[2], y_inter[2] ;
			n = Intersection(Mg_vec, imageD.width(), imageD.height(), x_inter, y_inter ) ;
		if(n)	imageG.draw_line(x_inter[0],y_inter[0],x_inter[1],y_inter[1],red).display(Gauche_disp);
		}
	}
	//delta = (double)xd[0]*(double)yd[1] - (double)xd[1]*(double)yd[0] ;
//	imageD.draw_line(xd[0],yd[0],xd[1],yd[1],green).display(Droite_disp);










	// Attente de la fermeture d'une des images pour arrêter le programme
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed()) ;
	
	return 0;
}



