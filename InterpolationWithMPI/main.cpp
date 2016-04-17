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
#include <vector>
using namespace std;
/*
 get square of the distance between two points 
 arguments will be changed to two point objects
 after refactoring
 
 */
int getDistanceSquared(int x1, int y1, int z1, int t1, int x2, int y2, int z2, int t2){
    ofstream test;
    test.open("/Users/Mahmut/Downloads/test.txt");
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int result = pow(abs(x1-x2),2)+pow(abs(y1-y2),2)+pow(abs(z1-z2),2)+pow(abs(t1-t2),2);
    test << "rank = " << rank << "result = " << result << endl;
    test.close();
    return result;
}

int main(int argc, char* argv[])
{
    
    int distances[100][100];
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
    
    MPI_Init(NULL, NULL);
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
    cout<<std::endl<<"Lütfen p degerini giriniz:";
    cin>>p;
//    if (p<1) {
//        cout<<endl<<"Yanlis giris yaptiniz. Bir daha deneyin!";
//        goto geri;
//    }
    
    // buffer for the coordinates of the current point
    int x_pos=0;
    int y_pos=0;
    int z_pos=0;
    int t_pos=0;
    
    
    // buffer for the coordinates of other points
    
    int x_posArray[100];
    int y_posArray[100];
    int z_posArray[100];
    int t_posArray[100];
    
    
    // display buffer
    
    int display[100][100];
    
    
    
    if (rank==0) {
        for (int j = 0; j<ceil(number/2); j++) {
            if(i != j){
                
                x_pos = bolge_x[j];
                y_pos = bolge_y[j];
                z_pos = bolge_yuks[j];
                t_pos = bolge_sic[j];
                
                // send the coordinates of the point to all processes
                MPI_Send(&x_pos, 1, MPI_INT, j%nproc, 0, MPI_COMM_WORLD);
                MPI_Send(&y_pos, 1, MPI_INT, j%nproc, 0, MPI_COMM_WORLD);
                MPI_Send(&z_pos, 1, MPI_INT, j%nproc, 0, MPI_COMM_WORLD);
                MPI_Send(&t_pos, 1, MPI_INT, j%nproc, 0, MPI_COMM_WORLD);
                
                
                for (int i = 0; i<ceil(number/2); i++) {
                    
                    x_posArray[i] = bolge_x[i];
                    y_posArray[i] = bolge_y[i];
                    z_posArray[i] = bolge_yuks[i];
                    t_posArray[i] = bolge_sic[i];
                    
                    // distribute the coordinates to all processes
                    MPI_Send(&x_posArray[i], 1, MPI_INT, i%nproc, 0, MPI_COMM_WORLD);
                    MPI_Send(&y_posArray[i], 1, MPI_INT, i%nproc, 0, MPI_COMM_WORLD);
                    MPI_Send(&z_posArray[i], 1, MPI_INT, i%nproc, 0, MPI_COMM_WORLD);
                    MPI_Send(&t_posArray[i], 1, MPI_INT, i%nproc, 0, MPI_COMM_WORLD);
                    
                }
            }
        }
    }
    else{
        
        cout<<"else"<<endl;
        for (int j=0; j<ceil(number/2); j++) {
            MPI_Recv(&x_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&y_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&z_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&t_pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            for (int i =0; i<ceil(number/2); i++) {
                MPI_Recv(&x_posArray[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&y_posArray[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&z_posArray[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&t_posArray[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                distances[j][i] = getDistanceSquared(x_pos,y_pos,z_pos,t_pos,x_posArray[i],y_posArray[i],z_posArray[i],t_posArray[i]);
                
                display[j][i] = distances[j][i];
                MPI_Send(&display[j][i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                
            }
            
            
        }
        
    }
    
    if (rank==0) {
        
        for (int i = 0; i < ceil(number/2); i++) {
            for (int j = 0; j<ceil(number/2); j++) {
                MPI_Recv(&display[i][j], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                cout<<display[i][j]<<'\t';
            }
       
        }
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

