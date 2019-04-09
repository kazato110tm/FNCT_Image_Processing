
/* bmp�`���̉摜�t�@�C������o�͂���֐����i�[����t�@�C�� */
#include  "bmp.h"

img tmpIn,tmpOut;

void Threshold(int i, int j, int th )	//臒l����
{
	if( th < tmpOut.data[i][j].r ){
		tmpOut.data[i][j].r = 255 ;		//�f�[�^�}��_red
		tmpOut.data[i][j].g = 255 ;		//�f�[�^�}��_green
		tmpOut.data[i][j].b = 255 ;		//�f�[�^�}��_blue
	}else{
		tmpOut.data[i][j].r = 0 ;		//�f�[�^�}��_red
		tmpOut.data[i][j].g = 0 ;		//�f�[�^�}��_green
		tmpOut.data[i][j].b = 0 ;		//�f�[�^�}��_blue
	}
	return;
}

void main()
{
	int i,j,th;
	int gray[256] = {0};
	double w1,w2,sum1,sum2;
	double m1,m2;
	double var;
	double max[2] = {0};
	int threshold;

	FILE *f;

	char *filename="text.bmp";

	ReadBmp(filename,&tmpIn);


	printf("%d %d\n",tmpIn.height,tmpIn.width);

	tmpOut = tmpIn;     /* ���̉摜�t�@�C���̂��ׂẲ摜�f�[�^���R�s�[���� */
	
	Gray(&tmpOut,&tmpIn); //�O���C�ŉ�f�l�̓���
	
	for( i = 0 ; i < tmpIn.height ; i++ ){		//�J�E���g
		for( j = 0 ; j < tmpIn.width ; j++ ){
			gray[ tmpOut.data[i][j].r ] ++;
		}
	}
	

	for( th = 1 ; th < 255 ; th++ ){
		w1 = 0;  w2 = 0;
		sum1 = 0;  sum2 = 0;
		
		for( i = 0 ; i < th ; i++ ){	//臒l�܂ł̉�f���ƕ���
			w1 += gray[i];
			sum1 += i*gray[i];
		}
		if( w1 != 0 )
			m1 = (double)sum1 / w1;
		else
			continue;

		
		for( i = th ; i < 256 ; i++ ){	//臒l�ȍ~�̉�f���ƕ���
			w2 += gray[i];
			sum2 += i*gray[i];
		}
		if( w2 != 0 )
			m2 = (double)sum2 / w2;
		else
			continue;
		
		var = w1*w2 * ( m1 - m2 )*( m1 - m2 );	//���U�̍ő�l��T��
		
		printf("%d : %lf\n",th,var);
		
		if( max[1] < var ){
			max[0] = th;
			max[1] = var;
			//printf("%lf : %lf\n",max[0],max[1]);
		}
	}
	threshold = (int)max[0];
	printf("%d\n",threshold);
	
	for( i = 0 ; i < tmpIn.height ; i++ ){		//臒l�ɂ�锒��
		for( j = 0 ; j < tmpIn.width ; j++ ){
			Threshold(i,j,threshold);
	  	}
	}
  
  
	WriteBmp("discriment.bmp",&tmpOut);
	PrintBmpInfo("discriment.bmp");
	
	return;
}
