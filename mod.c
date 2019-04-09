/* bmp形式の画像ファイルを入出力する関数を格納するファイル */
#include  "bmp.h"

/* 画像データを格納する構造体変数を定義する  */
img tmpIn,tmpOut,tmpgray;


int Mode(int x , int y , int height , int width){
	int i,j;
	int gray[256] = {0};
	int max1[2] = {0};
	int max2[2] = {0};
	int min[2];
	int dot[2];
	//ヒストグラムのカウント
	for( i = x ; i < x+height ; i++ ){
		for( j = y ; j < y+width ; j++ ){
			gray[ tmpOut.data[x+i][y+j].r ] ++;
		}
	}
	//最大値および次に大きい山の検索
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
	//max1,max2の画素値大小判定
	if(max1[0] < max2[0]){
		dot[0] = max1[0];
		dot[1] = max2[0];
	}else{
		dot[0] = max2[0];
		dot[1] = max1[0];
	}
		
	min[1] = max1[1];
	
	//山の間の閾値検索
	for( i =dot[0] ; i < dot[1] ; i++ ){
		if(min[1] > gray[i]){
			min[0] = i;
			min[1] = gray[i];
		}
	}
	printf("\n min %d : %d\n",min[0],min[1]);

	return min[0];
}


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
	int x,y,th,width,height;
	FILE *f_in;

	char *filename="text.bmp";

	ReadBmp(filename,&tmpIn);
	
	Gray(&tmpOut,&tmpIn); //グレイで画素値の統一
    
	scanf("%d %d %d %d",&x,&y,&height,&width);	//閾値を取る座標
	
	th = Mode(x,y,height,width);	//閾値取り
	
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
