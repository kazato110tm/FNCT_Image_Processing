//���f�B�A���t�B���^
#include "bmp.c"


img tmp1,tmp2;
#define filter_size 9 

// -- ��r�p�̊֐� cmp --
int cmp( const void *p, const void *q ) {
    return *(int*)q - *(int*)p;
}

									
void Median_filter(int i, int j )	//���f�B�A���֐�
{
	int count = 0;		//�s�N�Z���J�E���g
	int m , n ;
	int data_r[filter_size] = {0};
	int data_g[filter_size] = {0};
	int data_b[filter_size] = {0};
	for( m = -1 ; m <= 1 ; m++ ){
		for( n = -1 ; n <= 1 ; n++ ){
			if( i+m >= 0 && j+n >= 0 && i+m < tmp1.height && j+n < tmp1.width ){
				data_r[count] = tmp1.data[ i+m ][ j+n ].r;
				data_g[count] = tmp1.data[ i+m ][ j+n ].g;
				data_b[count] = tmp1.data[ i+m ][ j+n ].b;
				count++;
			}
		}
	}
	
	qsort( (void*)data_r , count, sizeof(int), cmp );
	qsort( (void*)data_g , count, sizeof(int), cmp );
	qsort( (void*)data_b , count, sizeof(int), cmp );
	
	tmp2.data[i][j].r = data_r[ count/2 ] ;		//�f�[�^�}��_red
	tmp2.data[i][j].g = data_g[ count/2 ] ;		//�f�[�^�}��_green
	tmp2.data[i][j].b = data_b[ count/2 ] ;		//�f�[�^�}��_blue
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
			Median_filter( i , j );		//���f�B�A���֐�
	  	}
	}
	WriteBmp("median.bmp",&tmp2);
	PrintBmpInfo("median.bmp");
	
	return;
}
