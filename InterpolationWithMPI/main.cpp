//
//  main.cpp
//  InterpolationWithMPI
//
//  Created by Mahmut Erkul on 08/04/16.
//  Copyright © 2016 Mahmut Erkul. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include<fstream>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[])
{
    int rank,nproc;
    int number=0,tmp, p,i;
    float distance=0.0, agirlik=0.0, deger=0.0;
    float bolge_x[100];
    float bolge_y[100];
    float bolge_yuks[100];
    float bolge_sic[100];
    float ger_sic[100];
    float mevcut_x, mevcut_y,mevcut_yuks,mevcut_sic, sonuc = 0.0,toplam = 0.0;
    ifstream file;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    
    
    file.open("/Users/Mahmut/Downloads/test_dosyasi.txt");
    for (int k=0; file; k++)
    {
        file>> bolge_x[number] >> bolge_y[number] >> bolge_yuks[number] >> bolge_sic[number];
        number++;
    }
    number--;
    
    /*
     cout<< number;
     for (int k=0; k<number; k++)
     {
     cout<< bolge_x[k] <<" "<< bolge_y[k] <<" "<< bolge_yuks[k] <<" "<< bolge_sic[k]<<endl;;
     }
     
     */
    
    cout<<"Lütfen istenilen bolgenin x konumunu giriniz:";
    cin>>mevcut_x;
    cout<<std::endl<<"Lütfen istenilen bolgenin y konumunu giriniz:";
    cin>>mevcut_y;
    cout<<std::endl<<"Lütfen istenilen bolgenin yuksekligini giriniz:";
    cin>>mevcut_yuks;
geri:
    cout<<std::endl<<"Lütfen p degerini giriniz:";
    cin>>p;
    if (p<1) {
        cout<<endl<<"Yanlis giris yaptiniz. Bir daha deneyin!";
        goto geri;
    }
    
    cout<<nproc<<endl;
    if (rank==0) {
        for (int i=0; i<nproc; i++) {
            
            MPI_Send(&bolge_x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&bolge_y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&bolge_yuks, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&bolge_sic, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    
    else{
        MPI_Recv(&bolge_x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&bolge_y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&bolge_yuks, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&bolge_sic, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i=0; i<number; i++) {
            tmp= bolge_yuks[i]/100;
            ger_sic[i]= bolge_sic[i]+ (tmp*0.5);
            
            distance= sqrt(pow(mevcut_x - bolge_x[i], 2) + pow(mevcut_y - bolge_y[i], 2));
            agirlik= 1/pow(distance, p);
            deger= agirlik* ger_sic[i];
            
        }
    }
    if (rank==0) {
        MPI_Recv(&agirlik, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&deger, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sonuc += deger;
        toplam+= agirlik;
    }
    else{
        MPI_Send(&agirlik, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&deger, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    
    /*
     
     for (int i=0; i<number; i++) {
     tmp= bolge_yuks[i]/100;
     ger_sic[i]= bolge_sic[i]+ (tmp*0.5);
     
     distance= sqrt(pow(mevcut_x - bolge_x[i], 2) + pow(mevcut_y - bolge_y[i], 2));
     agirlik= 1/pow(distance, p);
     deger= agirlik* ger_sic[i];
     sonuc += deger;
     toplam+= agirlik;
     }
     
     */
    cout<<toplam<<endl<<sonuc<<endl;
    mevcut_sic= sonuc/toplam;
    cout<<mevcut_sic<<endl;
    tmp=mevcut_yuks/100;
    mevcut_sic= mevcut_sic-(tmp*0.5);
    cout<<mevcut_sic<<endl;
    cout<<fixed <<setprecision(1)<< mevcut_sic<<endl;
    //    cout<<mevcut_sic<<endl;
    MPI_Finalize();
    file.close();
    return 0;
}

