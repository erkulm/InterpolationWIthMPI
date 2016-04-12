//
//  point.cpp
//  InterpolationWithMPI
//
//  Created by Mahmut Erkul on 13/04/16.
//  Copyright © 2016 Mahmut Erkul. All rights reserved.
//

#include "point.hpp"

class Point {
private:
    double x_pos;
    double y_pos;
    double z_pos;
    double temperature;
    
public:
    Point(){
        x_pos = NULL;
        y_pos = NULL;
        z_pos = NULL;
        temperature = NULL;
    }
    
    void setX(double x){
        this->x_pos = x;
    }
    
    double getX(){
        return this->x_pos;
    }
    
    
    void setY(double y){
        this->y_pos = y;
    }
    
    double getZ(){
        return this->z_pos;
    }
    
    void setZ(double z){
        this->z_pos = z;
    }
    
    double getTemperature(){
        return this->temperature;
    }
};