//
//  transfer.cpp
//  6050_assn2
//
//  Created by 张子筠 on 10/29/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#include "transfer.h"
#include <math.h>
#define PI acos(-1.0)
PointTransfer::PointTransfer(){
    
}

void PointTransfer::translation(vector3f *vertex, bool right, bool left, bool up, bool down, float t, int size) {
    
    if(right || left) {
        float translateMatrix[4][4] = {{1, 0, 0, t}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)translateMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }
    if(up || down) {
        float translateMatrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, t}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)translateMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }

}

void PointTransfer::pointTranslation(vector3f *vertex, float x, float y, float z) {
    
}

void PointTransfer::rotation(vector3f *vertex,bool vertical, bool horizontal, float angle, int size) {
    float cosAngle = cos(angle * PI / 180.0 );
    float sinAngle = sin(angle * PI / 180.0);
    
    float fixX = vertex[0].x;
    float fixY = vertex[0].y;
    float fixZ = vertex[0].z;
    float fixHoriMatrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, -fixY}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    float reFixHoriMatrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, fixY}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    
    float fixVerMatrix[4][4] = {{1, 0, 0, -fixX}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    float reFixVerMatrix[4][4] = {{1, 0, 0, fixX}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    
    if(horizontal) {
        float translatMatrix[4][4] = {{1, 0, 0, 0}, {0, cosAngle, -sinAngle, 0}, {0, sinAngle, cosAngle, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)fixHoriMatrix, vertexMatrx);
            matrixMultiply((float **)translatMatrix, vertexMatrx);
            matrixMultiply((float **)reFixHoriMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }else {
        float translatMatrix[4][4] = {{cosAngle, 0, sinAngle, 0}, {0, 1, 0, 0}, {-sinAngle, 0, cosAngle, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)fixVerMatrix, vertexMatrx);
            matrixMultiply((float **)translatMatrix, vertexMatrx);
            matrixMultiply((float **)reFixVerMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }
}

void PointTransfer::scale(vector3f *vertex, bool bigger, bool smaller, float time, int size) {
    float fixX = vertex[0].x;
    float fixY = vertex[0].y;
    float fixZ = vertex[0].z;
    float fixMatrix[4][4] = {{1, 0, 0, -fixX}, {0, 1, 0, -fixY}, {0, 0, 1, -fixZ}, {0, 0, 0, 1}};
    float reFixMatrix[4][4] = {{1, 0, 0, fixX}, {0, 1, 0, fixY}, {0, 0, 1, fixZ}, {0, 0, 0, 1}};
    
    if(bigger) {
        float translatMatrix[4][4] = {{1 + time, 0, 0, 0}, {0, 1 + time, 0, 0}, {0, 0 , 1 + time, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)fixMatrix, vertexMatrx);
            matrixMultiply((float **)translatMatrix, vertexMatrx);
            matrixMultiply((float **)reFixMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }else if(smaller){
        float translatMatrix[4][4] = {{1 - time, 0, 0, 0}, {0, 1 - time, 0, 0}, {0, 0 , 1 - time, 0}, {0, 0, 0, 1}};
        for(int i = 0; i < size; i++) {
            float vertexMatrx[4] = {vertex[i].x, vertex[i].y, vertex[3].z, 1};
            matrixMultiply((float **)fixMatrix, vertexMatrx);
            matrixMultiply((float **)translatMatrix, vertexMatrx);
            matrixMultiply((float **)reFixMatrix, vertexMatrx);
            vertex[i].x = vertexMatrx[0];
            vertex[i].y = vertexMatrx[1];
            vertex[i].z = vertexMatrx[2];
        }
    }
}

void PointTransfer::matrixMultiply(float **matrix1, float *matrix2) {
    float result[4];
    for(int i = 0; i < 4; i++) {
        float a = (float)*((float *) matrix1 + i * 4 + 0) * matrix2[0];
        float b = (float)*((float *) matrix1 + i * 4 + 1) * matrix2[1];
        float c = (float)*((float *) matrix1 + i * 4 + 2) * matrix2[2];
        float d = (float)*((float *) matrix1 + i * 4 + 3) * matrix2[3];
        result[i] = a + b + c + d;
    }
    for(int i = 0; i < 4; i++) {
        matrix2[i] = result[i];
    }
    
}
