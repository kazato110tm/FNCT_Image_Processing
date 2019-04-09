/* bmp形式の画像ファイルを入出力する関数を格納するファイル */
#include  "bmp.h"

/* 画像データを格納する構造体変数を定義する  */
img tmpIn,tmpOut,tmpgray;


void Threshold(int i, int j, int th )	//閾値判定
{
	if( th < tmpgray.data[i][j].r ){
		tmpgray.data[i][j].r = 255 ;		//データ挿入_red
		tmpgray.data[i][j].g = 255 ;		//データ挿入_green
		tmpgray.data[i][j].b = 255 ;		//データ挿入_blue
	}else{
		tmpgray.data[i][j].r = 0 ;		//データ挿入_red
		tmpgray.data[i][j].g = 0 ;		//データ挿入_green
		tmpgray.data[i][j].b = 0 ;		//データ挿入_blue
	}
	return;
}

void main()
{

	int i,j;
	int gray[256] = {0};
	int max1[2] = {0};
	int max2[2] = {0};
	int dot[2];
	int min[2];
	FILE *f_in;

	char *filename="text.bmp";

	ReadBmp(filename,&tmpIn);
	
	Gray(&tmpOut,&tmpIn);
	
	for (i=0; i<=tmpOut.height-1; ++i) {
		for (j=0; j<=tmpOut.width-1; ++j) {
			gray[ tmpOut.data[i][j].r ] ++;
	  	}
	}
	
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
	
	
	
	if(max1[0] < max2[0]){
		dot[0] = max1[0];
		dot[1] = max2[0];
	}else{
		dot[0] = max2[0];
		dot[1] = max1[0];
	}
		
	min[1] = max1[1];
	for( i =dot[0] ; i < dot[1] ; i++ ){
		if(min[1] > gray[i]){
			min[0] = i;
			min[1] = gray[i];
		}
	}
	printf("\n min %d : %d\n",min[0],min[1]);
	
	if( (f_in = fopen("gray.csv","wt"))!=NULL){
		for( i = 0 ; i < 256 ; i++ ){
			fprintf(f_in,"%d , %d\n",i,gray[i]);
		}
	}
	
	tmpgray = tmpOut;
	
	for (i=0; i<=tmpgray.height-1; ++i) {
		for (j=0; j<=tmpgray.width-1; ++j) {
			Threshold(i,j,min[0]);
	  	}
	}
  
	WriteBmp("noise_textgray.bmp",&tmpgray);
	return;
}
