//���ω��t�B���^
#include "bmp.c"

img tmp1,tmp2;
#define filter_size 5 
int filter[filter_size][filter_size] = { { 1 , 1 , 1 , 1 , 1 }
										,{ 1 , 1 , 1 , 1 , 1 }
										,{ 1 , 1 , 1 , 1 , 1 }
										,{ 1 , 1 , 1 , 1 , 1 }
										,{ 1 , 1 , 1 , 1 , 1 }};
									
void Ave_filter(int i, int j )	//���ω��֐�
{
	int NewPixcel_r = 0;
	int NewPixcel_g = 0;
	int NewPixcel_b = 0;
	int counter = 0;		//�s�N�Z���J�E���g
	int m , n ;
	for( m = -2 ; m <= 2 ; m++ ){
		for( n = -2 ; n <= 2 ; n++ ){
			if( i+m >= 0 && j+n >= 0 && i+m < tmp1.height && j+n < tmp1.width ){
				NewPixcel_r += tmp1.data[ i+m ][ j+n ].r * filter[ m+2 ][ n+2 ];
				NewPixcel_g += tmp1.data[ i+m ][ j+n ].g * filter[ m+2 ][ n+2 ];
				NewPixcel_b += tmp1.data[ i+m ][ j+n ].b * filter[ m+2 ][ n+2 ];
				counter++;
			}
		}
	}
	tmp2.data[i][j].r = NewPixcel_r / counter ;		//�f�[�^�}��_red
	tmp2.data[i][j].g = NewPixcel_g / counter ;		//�f�[�^�}��_green
	tmp2.data[i][j].b = NewPixcel_b / counter ;		//�f�[�^�}��_blue
	return;
}

void main()
{

	int i,j;

	FILE *f;

	char *filename="noise_text.bmp";

	ReadBmp(filename,&tmp1);


	printf("%d %d\n",tmp1.height,tmp1.width);

	tmp2=tmp1;     /* ���̉摜�t�@�C���̂��ׂẲ摜�f�[�^���R�s�[���� */
  
	for (i=0; i<= tmp2.height-1; ++i) {
		for (j=0; j<= tmp2.width-1; ++j) {
			Ave_filter( i , j );		//���ω�
	  	}
	}
	WriteBmp("ave_filter_5_5.bmp",&tmp2);
	PrintBmpInfo("ave_filter_5_5.bmp");
	
	return;
}