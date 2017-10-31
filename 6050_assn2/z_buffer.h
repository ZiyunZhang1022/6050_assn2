//
//  z_buffer.hpp
//  6050_assn2
//
//  Created by 张子筠 on 10/27/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#ifndef z_buffer_h
#define z_buffer_h

#include <stdio.h>
#include <assert.h>
#include <list>
#include "main.h"
#include "Obj.h"

typedef float datatype;
typedef float colortype;
enum Direction{LEFT,RIGHT};

#define winY 500
#define winX 500


struct Color
{
    colortype r,g,b;
};
struct PolygonTable
{
    datatype a,b,c,z;
    unsigned int id;
    int dy;
    Color col;
    
};

typedef PolygonTable ClassPolTable;
typedef PolygonTable ActPolTable;
struct ClassSideTable
{
    datatype x,dx;
    int dy;
    unsigned int id;
    
};

struct ActSideTable
{
    datatype xl,dxl,xr,dxr;
    int dyl,dyr;
    datatype zl,dzx,dzy;
    unsigned int id;
};
struct xyPos
{
    float x,y;
};
typedef Color  PixelColor;

class scanning_zbuffer
{
public:
    scanning_zbuffer();
    
    //建立分类的多边形表和边表
    //create polygon table and side table
    void CreateClassTable();
    
    //initial frame_buffer and z-buffer
    void initBuffer();
    
    //add new polygon which is associated with current scan line to the active side table
    //同时把该多边形在xy平面上的投影和扫描线相交的边加入到活化边表中
    void addPolTable(int);
    
    ////但对于凹多边形，任何时刻都有可能需要把边加入活化边表中，故每次更新前都进行检查
    void addSideTable(Direction dir,int curY,list<ActSideTable>::iterator it);
    
    //活化边表的更新
    void updateActSideTable(int);
    
    //活化多边形表的更新
    void updateActPolTable();
    //清空分类边表和分类面表
    void clearAll();
    //从.obj文件载入模型数据
    void loadModel();
    xyPos vertexProjection(vector3f v);
    //
    void coordUpdate(unsigned char key);
    
    int getActPolNum(){return m_listActPolTable.size();}
    
    ObjFloat m_color_buffer[1680][3];
    
    model getModel(){
        return m_3DModel;
    }
    
private:
    CLoadObj m_Loadobj;//obj file loading
    
    model m_3DModel;
    
    //分类多边形表
    vector<ClassPolTable> m_listClassPolTable[1050];
    //分类边表
    vector<ClassSideTable> m_listClassSideTable[1050];
    
    //活化多边形表
    list<ActPolTable> m_listActPolTable;
    //活化边表
    list<ActSideTable> m_listActSideTable;
    
    int m_num_of_polygon;
    int m_num_of_vertex;
    
    
    datatype m_depth_buffer[winX];
    
    
};

#endif