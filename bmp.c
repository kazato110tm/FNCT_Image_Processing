/*****************************************************************************/
/*                                                                           */
/*     bmp.c: bmp �t�@�C�������̃��C�u����                                   */
/*                                                                           */
/*             Kazutoshi Ando (Shizuoka Univ.)                               */
/*                                                                           */
/*                  Ver. 2004.11.30                                          */
/*                      WriteBmp: �w�b�_���̌������C��.                    */
/*                  Ver. 2004.11.26                                          */
/*                      Diminish:  ����(?)���[�`����ǉ�.                    */
/*                      PrintBmpInfo: bmp�t�@�C���ȊO�̃t�@�C����ǂ񂾂Ƃ�  */
/*                               ��, �G���[���o�͂���悤�ɕύX.             */
/*                  Ver. 2004.08.20                                          */
/*                      ReadBmp: 24bit�F��bmp�t�@�C���ȊO�̃t�@�C����        */
/*                               �ǂ񂾂Ƃ���, �G���[���o�͂���悤�ɕύX.   */
/*                      PrintBmpInfo: ����, �����𑜓x���o�͂���悤�ɕύX.  */
/*                      WriteBmp: �w�b�_���̌������C��.                    */
/*                  Ver. 2004.08.18                                          */
/*                        Gray ��ǉ�.                                       */
/*                  Ver. 2004.08.17                                          */
/*                        4byte ���E�ɍ��킹�邽�߂̌v�Z��������,            */
/*                        Rotate90, Shrink, Mosaic ��ǉ�.                   */
/*                        �G���[���b�Z�[�W�̏o�͐��W���G���[�o�͂ɕύX.    */
/*                  Ver. 2003.11.04                                          */
/*                                                                           */
/*****************************************************************************/
#include "bmp.h"

/* 
   �֐���: ReadBmp 
   ����  : char *filename, img *imgp
   �Ԃ�l: void
   ����  : bmp�`���̃t�@�C�� filename ���J����, ���̉摜�f�[�^��
           2�����z�� imgp->data �Ɋi�[����. ������, �w�b�_����ǂݍ��܂ꂽ
           �摜�̕��ƍ������O���[�o���ϐ� Bmp_width ��Bmp_height �ɃZ�b�g����. 
*/
void ReadBmp(char *filename, img *imgp) {
  int i,j;
  int Real_width;
  FILE *Bmp_Fp=fopen(filename,"rb");  /* �o�C�i�����[�h�ǂݍ��ݗp�ɃI�[�v��  */
  unsigned char *Bmp_Data;           /* �摜�f�[�^��1�s���i�[               */

  if(Bmp_Fp==NULL){
    fprintf(stderr,"Error: file %s couldn\'t open for read!.\n",filename);
    exit(1);
  }

  /* �w�b�_�ǂݍ��� */
  fread(Bmp_headbuf,sizeof(unsigned char),HEADERSIZE,Bmp_Fp);
        
  memcpy(&Bmp_type,Bmp_headbuf,sizeof(Bmp_type));
  if (strncmp(Bmp_type,"BM",2)!=0) {
    fprintf(stderr,"Error: %s is not a bmp file.\n",filename);
    exit(1);
  }
  
  memcpy(&imgp->width,Bmp_headbuf+18,sizeof(Bmp_width));
  memcpy(&imgp->height,Bmp_headbuf+22,sizeof(Bmp_height));
  memcpy(&Bmp_color,Bmp_headbuf+28,sizeof(Bmp_color));
  if (Bmp_color!=24) {
    fprintf(stderr,"Error: Bmp_color = %d is not implemented in this program.\n",Bmp_color);
    exit(1);
  }
  
  if (imgp->width > MAXWIDTH) {
    fprintf(stderr,"Error: Bmp_width = %d > %d = MAXWIDTH!\n",Bmp_width,MAXWIDTH);
    exit(1);
  }

  if (imgp->height > MAXHEIGHT) {
    fprintf(stderr,"Error: Bmp_height = %d > %d = MAXHEIGHT!\n",Bmp_height,MAXHEIGHT);
    exit(1);
  }
    
  Real_width = imgp->width*3 + imgp->width%4; /* 4byte ���E�ɂ��킹�邽�߂Ɏ��ۂ̕��̌v�Z */

 /* �z��̈�̓��I�m��. ���s�����ꍇ�̓G���[���b�Z�[�W���o�͂��ďI�� */
 if((Bmp_Data = (unsigned char *)calloc(Real_width,sizeof(unsigned char)))==NULL) {
   fprintf(stderr,"Error: Memory allocation failed for Bmp_Data!\n");
   exit(1);
 }           
 
  /* �摜�f�[�^�ǂݍ��� */
  for(i=0;i<imgp->height;i++) {
    fread(Bmp_Data,1,Real_width,Bmp_Fp);
    for (j=0;j<imgp->width;j++) {
      imgp->data[imgp->height-i-1][j].b = Bmp_Data[j*3];
      imgp->data[imgp->height-i-1][j].g = Bmp_Data[j*3+1];
      imgp->data[imgp->height-i-1][j].r = Bmp_Data[j*3+2];
    }
  }

  /* ���I�Ɋm�ۂ����z��̈�̉�� */
  free(Bmp_Data);

  /* �t�@�C���N���[�Y */
  fclose(Bmp_Fp); 
}

/* 
   �֐���: WriteBmp 
   ����  : char *filename, img *tp
   �Ԃ�l: void
   ����  : 2�����z�� tp->data �̓��e���摜�f�[�^�Ƃ���, 24�r�b�g
           bmp�`���̃t�@�C�� filename �ɏ����o��. 
*/
void WriteBmp(char *filename, img *tp) {

  int i,j;
  int Real_width;
  FILE *Out_Fp = fopen(filename,"wb");  /* �t�@�C���I�[�v�� */  
  unsigned char *Bmp_Data;     /* �摜�f�[�^��1�s���i�[               */
  
  if(Out_Fp==NULL){
    fprintf(stderr,"Error: file %s couldn\'t open for write!\n",filename);
    exit(1);
  }

  Bmp_color=24;
  Bmp_header_size=HEADERSIZE;
  Bmp_info_header_size=40;
  Bmp_planes=1;

  Real_width = tp->width*3 + tp->width%4;  /* 4byte ���E�ɂ��킹�邽�߂Ɏ��ۂ̕��̌v�Z */

  /* �z��̈�̓��I�m��. ���s�����ꍇ�̓G���[���b�Z�[�W���o�͂��ďI�� */
  if((Bmp_Data = (unsigned char *)calloc(Real_width,sizeof(unsigned char)))==NULL) {
   fprintf(stderr,"Error: Memory allocation failed for Bmp_Data!\n");
   exit(1);
 }

  /* �w�b�_���̏��� */
  Bmp_xppm=Bmp_yppm=0;
  Bmp_image_size = tp->height*Real_width;
  Bmp_size       = Bmp_image_size + HEADERSIZE;
  Bmp_headbuf[0]='B'; Bmp_headbuf[1]='M';
  memcpy(Bmp_headbuf+2,&Bmp_size,sizeof(Bmp_size));
  Bmp_headbuf[6]=Bmp_headbuf[7]=Bmp_headbuf[8]=Bmp_headbuf[9]=0;
  memcpy(Bmp_headbuf+10,&Bmp_header_size,sizeof(Bmp_header_size));
  Bmp_headbuf[11]=Bmp_headbuf[12]=Bmp_headbuf[13]=0;
  memcpy(Bmp_headbuf+14,&Bmp_info_header_size,sizeof(Bmp_info_header_size)); 
  Bmp_headbuf[15]=Bmp_headbuf[16]=Bmp_headbuf[17]=0;
  memcpy(Bmp_headbuf+18,&tp->width,sizeof(Bmp_width));
  memcpy(Bmp_headbuf+22,&tp->height,sizeof(Bmp_height));
  memcpy(Bmp_headbuf+26,&Bmp_planes,sizeof(Bmp_planes));
  memcpy(Bmp_headbuf+28,&Bmp_color,sizeof(Bmp_color));
  memcpy(Bmp_headbuf+34,&Bmp_image_size,sizeof(Bmp_image_size));
  memcpy(Bmp_headbuf+38,&Bmp_xppm,sizeof(Bmp_xppm));
  memcpy(Bmp_headbuf+42,&Bmp_yppm,sizeof(Bmp_yppm));
  Bmp_headbuf[46]=Bmp_headbuf[47]=Bmp_headbuf[48]=Bmp_headbuf[49]=0;
  Bmp_headbuf[50]=Bmp_headbuf[51]=Bmp_headbuf[52]=Bmp_headbuf[53]=0;
  
  /* �w�b�_��񏑂��o�� */
  fwrite(Bmp_headbuf,sizeof(unsigned char),HEADERSIZE,Out_Fp); 

  /* �摜�f�[�^�����o�� */
  for (i=0;i<tp->height;i++) {
    for (j=0;j<tp->width;j++) {
      Bmp_Data[j*3]   = tp->data[tp->height-i-1][j].b;
      Bmp_Data[j*3+1] = tp->data[tp->height-i-1][j].g;
      Bmp_Data[j*3+2] = tp->data[tp->height-i-1][j].r;
    }
    for (j=tp->width*3;j<Real_width;j++) {
      Bmp_Data[j]=0;
    }
    fwrite(Bmp_Data,sizeof(unsigned char),Real_width,Out_Fp);
  }

  /* ���I�Ɋm�ۂ����z��̈�̉�� */
  free(Bmp_Data);

  /* �t�@�C���N���[�Y */
  fclose(Out_Fp);
}

/* 
   �֐���: PrintBmpInfo
   ����  : char *filename
   �Ԃ�l: void
   ����  : �����Ƃ��ė^������t�@�C���������� bmp �`���̉摜�t�@�C��
           �̑�������ʂɏo�͂���. 
*/
void PrintBmpInfo(char *filename) {

  FILE *Bmp_Fp=fopen(filename,"rb");        /* �o�C�i�����[�h�ǂݍ��ݗp�ɃI�[�v��  */
  if(Bmp_Fp==NULL){
    fprintf(stderr,"Error: file %s couldn\'t open for write!\n",filename);
    exit(1);
  }

  fread(Bmp_headbuf,sizeof(unsigned char),HEADERSIZE,Bmp_Fp);
        
  memcpy(&Bmp_type,Bmp_headbuf,sizeof(Bmp_type));
  if (strncmp(Bmp_type,"BM",2)!=0) {
    fprintf(stderr,"Error: %s is not a bmp file.\n",filename);
    exit(1);
  }
  memcpy(&Bmp_size,Bmp_headbuf+2,sizeof(Bmp_size));
  memcpy(&Bmp_width,Bmp_headbuf+18,sizeof(Bmp_width));
  memcpy(&Bmp_height,Bmp_headbuf+22,sizeof(Bmp_height));
  memcpy(&Bmp_color,Bmp_headbuf+28,sizeof(Bmp_color));
  memcpy(&Bmp_comp,Bmp_headbuf+30,sizeof(Bmp_comp));
  memcpy(&Bmp_image_size,Bmp_headbuf+34,sizeof(Bmp_size));
  memcpy(&Bmp_xppm,Bmp_headbuf+38,sizeof(Bmp_xppm));
  memcpy(&Bmp_yppm,Bmp_headbuf+42,sizeof(Bmp_yppm));


  printf("�t�@�C����       = %s \n",filename);
  printf("�t�@�C���^�C�v   = %c%c \n",Bmp_type[0],Bmp_type[1]);
  printf("�t�@�C���T�C�Y   = %d (byte)\n",Bmp_size);
  printf("��               = %d (pixel)\n",Bmp_width);
  printf("����             = %d (pixel)\n",Bmp_height);
  printf("�F               = %d (bit)\n",Bmp_color);
  printf("���k             = %d\n",Bmp_comp);
  printf("�摜�����̃T�C�Y = %d (byte)\n",Bmp_image_size);
  printf("�����𑜓x       = %d (ppm)\n",Bmp_xppm);
  printf("�����𑜓x       = %d (ppm)\n",Bmp_yppm);

  fclose(Bmp_Fp);
}

/* 
   �֐���: HMirror 
   ����  : img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�𐅕������̋��f���Ƃ������̂�
           2�����z�� sp->data �Ɋi�[����. 
*/
void HMirror(img* sp, img *tp) {
  int i,j;
  long k;
  for(i=0;i<tp->height;i++) 
    for(j=0;j<tp->width;j++) 
      sp->data[tp->height-i-1][j]=tp->data[i][j];
  sp->height=tp->height;
  sp->width=tp->width;
}

/* 
   �֐���: VMirror 
   ����  : img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�𐂒������̋��f���Ƃ������̂�
           2�����z�� sp->data �Ɋi�[����. 
*/
void VMirror(img *sp, img *tp) {
  int i,j;
  long k;
  for(i=0;i<tp->height;i++) 
    for(j=0;j<tp->width;j++) 
      sp->data[i][tp->width-j-1]=tp->data[i][j];
  sp->height=tp->height;
  sp->width=tp->width;
}


/* 
   �֐���: Rotate90
   ����  : int a, img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�� 90�~a �x���v���ɉ�]�������̂�
           2�����z�� sp->data �Ɋi�[����. �����, �摜�̕��ƍ�����, 
           sp->height �� sp->width �ɐݒ�. 
*/
void Rotate90(int a, img *sp, img *tp) {
  int i,j;
  if ((a%4)==0) {
    for(i=0;i<tp->height;i++) 
      for(j=0;j<tp->width;j++) 
        sp->data[i][j]=tp->data[i][j];
    sp->width=tp->width;
    sp->height=tp->height;
  } else if ((a%4)==1) {
    for(i=0;i<tp->height;i++) 
      for(j=0;j<tp->width;j++) 
        sp->data[j][tp->height-i-1]=tp->data[i][j];
    sp->height=tp->width;
    sp->width=tp->height;
  } else if ((a%4)==2) {
    for(i=0;i<tp->height;i++) 
      for(j=0;j<tp->width;j++) 
        sp->data[tp->height-i-1][tp->width-j-1]=tp->data[i][j];
    sp->width=tp->width;
    sp->height=tp->height;
  } else {
    for(i=0;i<tp->height;i++) 
      for(j=0;j<tp->width;j++) 
        sp->data[tp->width-j-1][i]=tp->data[i][j];
    sp->height=tp->width;
    sp->width=tp->height;
  }
}


/* 
   �֐���: Shrink
   ����  : int a, img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�̕��ƍ����� 1/a �{�����摜��
           2�����z�� sp->data �Ɋi�[����. 
*/
void Shrink(int a, img *sp, img *tp) {
  int i,j,k,l,w,h,count;
  unsigned long tmp_r,tmp_g,tmp_b;

  sp->height=tp->height/a + ((tp->height%a == 0)?0:1);
  sp->width=tp->width/a + ((tp->width%a == 0)?0:1);

  for(i=0;i<sp->height;i++) {
    h = (a*(i+1)<=tp->height)?a:tp->height-a*i;
    for(j=0;j<sp->width;j++) {
      w = (a*(j+1)<=tp->width)?a:tp->width-a*j;
      tmp_r =0; tmp_g=0; tmp_b =0; count=0;
      for(k=0;k<h;k++)
        for(l=0;l<w;l++) {
          tmp_r+=tp->data[i*a+k][j*a+l].r;
          tmp_g+=tp->data[i*a+k][j*a+l].g;
          tmp_b+=tp->data[i*a+k][j*a+l].b;
          count++;
        }
      sp->data[i][j].r = (unsigned char)(tmp_r/count);
      sp->data[i][j].g = (unsigned char)(tmp_g/count);
      sp->data[i][j].b = (unsigned char)(tmp_b/count);
    }
  }
}

/* 
   �֐���: Mosaic
   ����  : int a, img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�Ƀ��U�C�N���������摜��
           2�����z�� sp->data �Ɋi�[����. ���U�C�N�̑傫����
           a�~a �ł���. 
*/
void Mosaic(int a, img *sp, img *tp) {
  int i,j,k,l,w,h,t_height,t_width,count;
  unsigned long tmp_r,tmp_g,tmp_b;

  sp->height=tp->height;
  sp->width=tp->width;

  t_height=tp->height/a + ((tp->height%a == 0)?0:1);
  t_width=tp->width/a + ((tp->width%a == 0)?0:1);

  for(i=0;i<t_height;i++) {
    h = (a*(i+1)<=tp->height)?a:tp->height-a*i;
    for(j=0;j<t_width;j++) {
      w = (a*(j+1)<=tp->width)?a:tp->width-a*j;
      tmp_r =0; tmp_g=0; tmp_b =0; count=0;
      for(k=0;k<h;k++)
        for(l=0;l<w;l++) {
          tmp_r+=tp->data[i*a+k][j*a+l].r;
          tmp_g+=tp->data[i*a+k][j*a+l].g;
          tmp_b+=tp->data[i*a+k][j*a+l].b;
          count++;
        }
      tmp_r = (unsigned char)(tmp_r/count);
      tmp_g = (unsigned char)(tmp_g/count);
      tmp_b = (unsigned char)(tmp_b/count);
      for(k=0;k<h;k++)
        for(l=0;l<w;l++) {
          sp->data[i*a+k][j*a+l].r = tmp_r;
          sp->data[i*a+k][j*a+l].g = tmp_g;
          sp->data[i*a+k][j*a+l].b = tmp_b;
        }
    }
  }

}

/* 
   �֐���: Gray
   ����  : img *sp, img *tp
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜���O���C�X�P�[���ϊ�����, 
           2�����z�� sp->data �Ɋi�[����. 
*/
void Gray(img *sp, img *tp) {
  int i,j;
  unsigned char tmp;

  sp->height=tp->height;
  sp->width=tp->width;

  for(i=0;i<sp->height;i++) {
    for(j=0;j<sp->width;j++) {
      tmp=(unsigned char)((tp->data[i][j].r+tp->data[i][j].g+tp->data[i][j].b)/3);
      sp->data[i][j].r = sp->data[i][j].g = sp->data[i][j].b = tmp;
    }
  }

}

/* 
   �֐���: Diminish
   ����  : img *sp, img *tp, unsigned char x
   �Ԃ�l: void 
   ����  : 2�����z�� tp->data �̉摜�������� (�eRGB�̋P�x�ɂ��� 
           2^x �ƃ}�X�N�����) ��, 2�����z�� sp->data �Ɋi�[����. 
*/
void Diminish(img *sp, img *tp, unsigned char x) {
  int i,j;
  unsigned char y;
  
  x = x % 8;
  y = 255 << x;

  sp->height=tp->height;
  sp->width=tp->width;
  
  for(i=0;i<sp->height;i++) {
    for(j=0;j<sp->width;j++) {
      sp->data[i][j].r = tp->data[i][j].r & y;
      sp->data[i][j].g = tp->data[i][j].g & y;
      sp->data[i][j].b = tp->data[i][j].b & y;
    }
  }
}
