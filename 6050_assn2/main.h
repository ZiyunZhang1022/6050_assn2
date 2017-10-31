//
//  Main.h
//  6050_assn2
//
//  Created by 张子筠 on 10/27/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#ifndef main_h
#define main_h

#include <vector>

using namespace std;
typedef int32_t ObjBool;
typedef uint8_t ObjByte;
typedef uint16_t ObjWord;
typedef uint32_t ObjDword;
typedef int8_t ObjIntb;
typedef int16_t ObjIntw;
typedef int32_t ObjIntd;

typedef float ObjFloat;
typedef double ObjDouble;

#define FILE_NAME "/Users/zhangziyun/Documents/xcode/6050_assn2/6050_assn2/6050_assn2/simple2.obj"

class vector3f {
public: ObjFloat x, y, z;
};

class vector2f {
public: ObjFloat x, y;
};

struct face{
    int num;
    int vertexIndex[3];			//顶点索引
    int coordIndex[3];			//纹理坐标索引
    int normalIndex[3];			//法线索引
    float col[3];
};



struct model {
    vector3f *vertex;
    
    face *faceVertex;
    int vertexCount, textureCount, normalCount,faceCount;
};


#endif /* Main_h */
