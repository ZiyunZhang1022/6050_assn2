//
//  Obj.hpp
//  6050_assn2
//
//  Created by Ziyun Zhang on 10/27/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#ifndef Obj_h
#define Obj_h

#include <stdio.h>
#include "main.h"

class CLoadObj {
public:
    CLoadObj();
    
    //read objFile to the model
    bool ImportObj(model *myModel, char *strFileName);
    
    //读入对象，在ImportObj()中将调用该函数
    void ReadObjFile(model *myModel);
    
    //读入顶点信息，在ReadObjFile()中调用该函数
    void ReadVertexInfo();
    
    //读入面信息，在ReadObjFile()中调用该函数
    void ReadFaceInfo();
    
    void FillInObjectInfo(model *myModel);
    
private:
    
    //the file to be read
    FILE *m_FilePointer;
    
    //vector of vertex
    vector<vector3f>  m_pVertice;
    
    //vector of face
    vector<face>  m_pFace;
    
    bool m_bJustReadAFace;
    
    vector<vector3f> fixCoordinate(vector<vector3f> vertexs);
    
    
};

#endif /* Obj_hpp */
