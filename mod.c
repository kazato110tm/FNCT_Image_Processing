/* bmp�`���̉摜�t�@�C������o�͂���֐����i�[����t�@�C�� */
#include  "bmp.h"

/* �摜�f�[�^���i�[����\���̕ϐ����`����  */
img tmpIn,tmpOut,tmpgray;


int Mode(int x , int y , int height , int width){
	int i,j;
	int gray[256] = {0};
	int max1[2] = {0};
	int max2[2] = {0};
	int min[2];
	int dot[2];
	//�q�X�g�O�����̃J�E���g
	for( i = x ; i < x+height ; i++ ){
		for( j = y ; j < y+width ; j++ ){
			gray[ tmpOut.data[x+i][y+j].r ] ++;
		}
	}
	//�ő�l����ю��ɑ傫���R�̌���
	for( i = 1 ; i < 255 ; i++ ){
		if(gray[i] > gray[i-1] && gray[i] > gray[i+1]){
			if( gray[i] > max1[1] ){
				max2[0] = max1[0];
				max2[1] = max1[1];
				max1[0] = i;
				max1[1] = gray[i];
			}else if( gray[i] > max2[1] ){
				max2[0] = i;
				max2[1] = gray[i];
			}
		}
	}
	
	printf("%d : %d\n%d : %d\n",max1[0],max1[1],max2[0],max2[1]);	
	//max1,max2�̉�f�l�召����
	if(max1[0] < max2[0]){
		dot[0] = max1[0];
		dot[1] = max2[0];
	}else{
		dot[0] = max2[0];
		dot[1] = max1[0];
	}
		
	min[1] = max1[1];
	
	//�R�̊Ԃ�臒l����
	for( i =dot[0] ; i < dot[1] ; i++ ){
		if(min[1] > gray[i]){
			min[0] = i;
			min[1] = gray[i];
		}
	}
	printf("\n min %d : %d\n",min[0],min[1]);

	return min[0];
}


void Threshold(int i, int j, int th )	//臒l����
{
	if( th < tmpgray.data[i][j].r ){
		tmpgray.data[i][j].r = 255 ;		//�f�[�^�}��_red
		tmpgray.data[i][j].g = 255 ;		//�f�[�^�}��_green
		tmpgray.data[i][j].b = 255 ;		//�f�[�^�}��_blue
	}else{
		tmpgray.data[i][j].r = 0 ;		//�f�[�^�}��_red
		tmpgray.data[i][j].g = 0 ;		//�f�[�^�}��_green
		tmpgray.data[i][j].b = 0 ;		//�f�[�^�}��_blue
	}
	return;
}

void main()
{

	int i,j;
	int x,y,th,width,height;
	FILE *f_in;

	char *filename="text.bmp";

	ReadBmp(filename,&tmpIn);
	
	Gray(&tmpOut,&tmpIn); //�O���C�ŉ�f�l�̓���
    
	scanf("%d %d %d %d",&x,&y,&height,&width);	//臒l�������W
	
	th = Mode(x,y,height,width);	//臒l���
	
	tmpgray = tmpOut;
	
	for (i = x; i < x + height; ++i) {
		for (j = y; j < y + width; ++j) {
			Threshold(i,j,th);
	  	}
	}
  
	WriteBmp("mod.bmp",&tmpgray);
	PrintBmpInfo("mod.bmp");
	return;
}
