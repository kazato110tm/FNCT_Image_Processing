#include  "bmp.h"

img tmpIn,tmpgray,tmpOut;

#define range 51

void  Threshold(int i, int j, int th )	//臒l����
{
    if( th < tmpgray.data[i][j].r ){
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

void Count_gray(int x , int y , int *gray){    //�͈͓��̊e��f�l�̉�f�����J�E���g
    int i , j;
    for( i = 0 ; i < 256 ; i++ )            //�J�E���g���Z�b�g
        gray[ i ] = 0;
    for( i = x - range/2 ; i < x + range/2 ; i++ ){		//�J�E���g
        for( j = y -range/2 ; j < y + range/2 ; j++ ){
            if( i >= 0 && i <= tmpIn.height-1 &&  j >= 0 && j <= tmpIn.width-1 )
                gray[ tmpgray.data[i][j].r ] ++;
        }
    }
	return;
}

int discriment( int *gray ){        //�͈͓��̕��U�̍ő�𓱏o
    int i , j, th ;
    double w1 , w2 , sum1 , sum2 , m1 , m2 ;
    double var;
    double max[2] = {0};
    
    
    for( th = 1 ; th < 255 ; th++ ){
        w1 = 0;  w2 = 0;
        sum1 = 0;  sum2 = 0;
        
        for( i = 0 ; i < th ; i++ ){	//臒l�܂ł̉�f���ƕ���
            w1 += gray[i];
            sum1 += i * gray[i];
        }
        if( w1 != 0 )
            m1 = (double)sum1 / w1;
        else
            continue;
        
        
        for( i = th ; i < 256 ; i++ ){	//臒l�ȍ~�̉�f���ƕ��χ
            w2 += gray[i];
            sum2 += i * gray[i];
        }
        if( w2 != 0 )
            m2 = (double)sum2 / w2;
        else
            continue;
        
        var = w1 * w2 * ( m1 - m2 )*( m1 - m2 );	//���U�̍ő��T��
        
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
    
    Gray(&tmpgray,&tmpIn); //�O���C�ŉ�f�l�̓���
	
	tmpOut = tmpgray;
    
    for( i = 0 ; i <= tmpIn.height-1 ; i++ ){
        for( j = 0 ; j <= tmpIn.width-1 ; j++ ){
            Count_gray( i , j , gray );    //�͈͓��̊e��f�l�̉�f���J�E���g
            threshold = discriment(  gray );    //���U���ő�̎���臒l
            Threshold( i , j , threshold );       //臒l�ɂ��2�l���
        }
		printf("%d\n",i);
    }
    
    WriteBmp("move_disc.bmp",&tmpOut);
    PrintBmpInfo("move_disc.bmp");
    
    return;
}
