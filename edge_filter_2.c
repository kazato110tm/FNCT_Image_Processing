//先鋭化
#include "bmp.c"
#include <stdlib.h>

img tmp1,tmp2;
#define filter_size 3 
int filter[filter_size][filter_size] = { { -1 , -1 , -1 }
									    ,{ -1 , 9 , -1 }
										,{ -1 , -1 , -1 }};
									
void Edge_filter(int i, int j )	//先鋭化
{
	int NewPixcel_r = 0;
	int NewPixcel_g = 0;
	int NewPixcel_b = 0;
	int m , n ;
	for( m = -1 ; m <= 1 ; m++ ){
		for( n = -1 ; n <= 1 ; n++ ){
			if( i+m >= 0 && j+n >= 0 && i+m < tmp1.height && j+n < tmp1.width ){
				NewPixcel_r += tmp1.data[ i+m ][ j+n ].r * filter[ m+1 ][ n+1 ];
				NewPixcel_g += tmp1.data[ i+m ][ j+n ].g * filter[ m+1 ][ n+1 ];
				NewPixcel_b += tmp1.data[ i+m ][ j+n ].b * filter[ m+1 ][ n+1 ];
			}
		}
	}
	tmp2.data[i][j].r = abs( NewPixcel_r )  ;		//データ挿入_red
	tmp2.data[i][j].g = abs( NewPixcel_g )  ;		//データ挿入_green
	tmp2.data[i][j].b = abs( NewPixcel_b )  ;		//データ挿入_blue
	return;
}

void main()
{

	int i,j;

	FILE *f;

	char *filename="noise_text.bmp";

	ReadBmp(filename,&tmp1);


	printf("%d %d\n",tmp1.height,tmp1.width);

	tmp2=tmp1;     /* 元の画像ファイルのすべての画像データをコピーする */
  
	for (i=0; i<= tmp2.height-1; ++i) {
		for (j=0; j<= tmp2.width-1; ++j) {
			Edge_filter( i , j );		
	  	}
	}
	WriteBmp("edge_filter_2.bmp",&tmp2);
	PrintBmpInfo("edge_filter_2.bmp");
	
	return;
}
