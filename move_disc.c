#include  "bmp.h"

img tmpIn,tmpgray,tmpOut;

#define range 51

void  Threshold(int i, int j, int th )	//閾値判定
{
    if( th < tmpgray.data[i][j].r ){
        tmpOut.data[i][j].r = 255 ;		//データ挿入_red
        tmpOut.data[i][j].g = 255 ;		//データ挿入_green
        tmpOut.data[i][j].b = 255 ;		//データ挿入_blue
    }else{
        tmpOut.data[i][j].r = 0 ;		//データ挿入_red
        tmpOut.data[i][j].g = 0 ;		//データ挿入_green
        tmpOut.data[i][j].b = 0 ;		//データ挿入_blue
    }
    return;
}

void Count_gray(int x , int y , int *gray){    //範囲内の各画素値の画素数をカウント
    int i , j;
    for( i = 0 ; i < 256 ; i++ )            //カウントリセット
        gray[ i ] = 0;
    for( i = x - range/2 ; i < x + range/2 ; i++ ){		//カウント
        for( j = y -range/2 ; j < y + range/2 ; j++ ){
            if( i >= 0 && i <= tmpIn.height-1 &&  j >= 0 && j <= tmpIn.width-1 )
                gray[ tmpgray.data[i][j].r ] ++;
        }
    }
	return;
}

int discriment( int *gray ){        //範囲内の分散の最大を導出
    int i , j, th ;
    double w1 , w2 , sum1 , sum2 , m1 , m2 ;
    double var;
    double max[2] = {0};
    
    
    for( th = 1 ; th < 255 ; th++ ){
        w1 = 0;  w2 = 0;
        sum1 = 0;  sum2 = 0;
        
        for( i = 0 ; i < th ; i++ ){	//閾値までの画素数と平均
            w1 += gray[i];
            sum1 += i * gray[i];
        }
        if( w1 != 0 )
            m1 = (double)sum1 / w1;
        else
            continue;
        
        
        for( i = th ; i < 256 ; i++ ){	//閾値以降の画素数と平均�
            w2 += gray[i];
            sum2 += i * gray[i];
        }
        if( w2 != 0 )
            m2 = (double)sum2 / w2;
        else
            continue;
        
        var = w1 * w2 * ( m1 - m2 )*( m1 - m2 );	//分散の最大を探す
        
        if( max[1] < var ){
            max[0] = th;
            max[1] = var;
        }
    }
    return (int)max[0];
}


void main()
{
    int i , j ;
    int gray[256] = {0};
    int threshold;
    
    FILE *f;
    
    char *filename="text.bmp";
    
    ReadBmp(filename,&tmpIn);
    
    
    printf("%d %d\n",tmpIn.height,tmpIn.width);
    
    Gray(&tmpgray,&tmpIn); //グレイで画素値の統一�
	
	tmpOut = tmpgray;
    
    for( i = 0 ; i <= tmpIn.height-1 ; i++ ){
        for( j = 0 ; j <= tmpIn.width-1 ; j++ ){
            Count_gray( i , j , gray );    //範囲内の各画素値の画素数カウント
            threshold = discriment(  gray );    //分散が最大の時の閾値
            Threshold( i , j , threshold );       //閾値による2値化�
        }
		printf("%d\n",i);
    }
    
    WriteBmp("move_disc.bmp",&tmpOut);
    PrintBmpInfo("move_disc.bmp");
    
    return;
}
