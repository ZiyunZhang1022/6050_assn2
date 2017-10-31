//
//  transfer.hpp
//  6050_assn2
//
//  Created by 张子筠 on 10/29/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#ifndef transfer_hpp
#define transfer_hpp

#include <stdio.h>
#include "main.h"

class PointTransfer{
public:
    PointTransfer();
    
    void translation(vector3f *vertex, bool right, bool left, bool up, bool down, float t, int size);
    
    void rotation(vector3f *vertex, bool vertical, bool horizontal, float angle, int size);
    
    void scale(vector3f *vertex, bool bigger, bool smaller, float time, int size);
private:
    void matrixMultiply(float **matrix1, float *matrix2);

    void pointTranslation(vector3f *vertex, float x, float y, float z);
};

#endif /* transfer_hpp */
