//
//  z_buffer.cpp
//  6050_assn2
//
//  Created by 张子筠 on 10/27/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#include "z_buffer.h"
#include "main.h"

#include <math.h>


using namespace std;

#define MINDEPTH -10000.0f
int half_winY = winY / 2;
int half_winX = winX / 2;

float rotate=30.0f/180.0f*3.1415926;
float bigY=0,bigX=0;

scanning_zbuffer::scanning_zbuffer() {
    loadModel();
    m_num_of_polygon=m_3DModel.faceCount;
    m_num_of_vertex=m_3DModel.vertexCount;
    //coordUpdate();
    //CreateClassTable();
}

void scanning_zbuffer::clearAll()
{
    for(int i = 0;i < winY; i++)
    {
        m_listClassPolTable[i].clear();
        m_listClassSideTable[i].clear();
    }
    m_listActPolTable.clear();
    m_listActSideTable.clear();
}

void scanning_zbuffer::loadModel() {
    m_Loadobj.ImportObj(&m_3DModel, FILE_NAME);
   
}

xyPos scanning_zbuffer::vertexProjection(vector3f v) {
    xyPos xy;
    xy.x=v.x;///bigX*half_winX*0.8;
    xy.y=v.y;///bigY*half_winY*0.8;
    return xy;
}

void scanning_zbuffer::addPolTable(int curY) {
    vector<ClassPolTable> listclassPolTable=m_listClassPolTable[curY];
    vector<ClassSideTable> listclassSideTable=m_listClassSideTable[curY];
    for(unsigned int i=0;i<listclassPolTable.size();i++)
    {
        //投影相交
        
        int id=listclassPolTable[i].id;
        ActSideTable sideTable={0};
        int k1=-1,k2=-1;
        for (int j=0;j<listclassSideTable.size();j++)
        {
            if(listclassSideTable[j].id==id&&k1==-1)
            {
                //x1=listclassSideTable[j].x;
                k1=j;
                continue;
            }
            if(listclassSideTable[j].id==id&&k2==-1&&k1!=-1)
            {
                k2=j;
                break;
            }
        }
        //如果多边形在视平面上的投影为一条线
        //会导致只找到一条边,这个多边形忽略
        //
        if(k1==-1||k2==-1)
            continue;
        //x坐标肯定是相等的
        if(listclassSideTable[k1].x<listclassSideTable[k2].x||
           (listclassSideTable[k1].x==listclassSideTable[k2].x&&listclassSideTable[k1].dx<listclassSideTable[k2].dx))
        {
            sideTable.xl=listclassSideTable[k1].x;
            sideTable.dxl=listclassSideTable[k1].dx;
            sideTable.dyl=listclassSideTable[k1].dy;
            
            sideTable.xr=listclassSideTable[k2].x;
            sideTable.dxr=listclassSideTable[k2].dx;
            sideTable.dyr=listclassSideTable[k2].dy;
            
            
        }
        else
        {
            sideTable.xl=listclassSideTable[k2].x;
            sideTable.dxl=listclassSideTable[k2].dx;
            sideTable.dyl=listclassSideTable[k2].dy;
            
            sideTable.xr=listclassSideTable[k1].x;
            sideTable.dxr=listclassSideTable[k1].dx;
            sideTable.dyr=listclassSideTable[k1].dy;
        }
        //左交点处多边形所在平面的深度值
        sideTable.id=id;
        //??????
        sideTable.zl=listclassPolTable[i].z;
        sideTable.dzx=-listclassPolTable[i].a/listclassPolTable[i].c;
        sideTable.dzy=listclassPolTable[i].b/listclassPolTable[i].c;
        //consider if c==0;
        m_listActSideTable.push_back(sideTable);
        m_listActPolTable.push_back(listclassPolTable[i]);
        
    }
    
    
}

void scanning_zbuffer::addSideTable(Direction dir,int curY,list<ActSideTable>::iterator it) {
    vector<ClassSideTable>::iterator i;// listclassSideTable=m_listClassSideTable[curY];
    int x;
    if(dir==LEFT)x=(*it).xl;
    else
        x=(*it).xr;
    for (i=m_listClassSideTable[curY].begin();i!=m_listClassSideTable[curY].end();i++)
    {
        if((*i).id==(*it).id&&int((*i).x)==x)//由于精度误差，只需近似相等即可
            break;
        
    }
    if(dir==LEFT)
    {
        (*it).dxl=(*i).dx;
        (*it).dyl=(*i).dy;
    }
    else
    {
        (*it).dxr=(*i).dx;
        (*it).dyr=(*i).dy;
    }
}

//直接分类为投影相交
void scanning_zbuffer::CreateClassTable() {
    face* pface=m_3DModel.faceVertex;
    vector3f* pVertex=m_3DModel.vertex;
    int numvertex=m_3DModel.vertexCount;
    int xx,yy;
    for (int i=0;i<m_3DModel.vertexCount;i++)
    {
        xx=abs(pVertex[i].x);
        yy=abs(pVertex[i].y);
        if(xx>bigX)bigX=xx;
        if(xx>bigY)bigY=yy;
    }
    ClassPolTable polTable;
    ClassSideTable sideTable;
    int vndex;//用来标记法向索引
    int vdex;
    
    for(int i=0;i<m_num_of_polygon;i++)
    {
        polTable.id=i;
        //计算平面参数ax+by+cz+d=0;
        /*
         vndex=pface[i].normalIndex[0]-1;//先暂时用某个顶点的法向作为多边形的法向
         polTable.a=pNormal[vndex].x;
         polTable.b=pNormal[vndex].y;
         polTable.c=pNormal[vndex].z;
         
         
         polTable.a/=polTable.c;
         polTable.b/=polTable.c;
         polTable.c=1;
         cout<<polTable.a<<" "<<polTable.b<<" "<<polTable.c<<endl;
         //
         */
        vdex=pface[i].vertexIndex[0]-1;
        assert(vdex<numvertex);
        
        float vx1=pVertex[vdex].x,vy1=pVertex[vdex].y,vz1=pVertex[vdex].z;
        vdex=pface[i].vertexIndex[1]-1;
        float vx2=pVertex[vdex].x,vy2=pVertex[vdex].y,vz2=pVertex[vdex].z;
        vdex=pface[i].vertexIndex[2]-1;
        float vx3=pVertex[vdex].x,vy3=pVertex[vdex].y,vz3=pVertex[vdex].z;
        //polTable.c=1;
        //polTable.b=((vz2-vz1)*(vx1-vx3)-(vz3-vz1)*(vx1-vx2))/((vy1-vy2)*(vx1-vx3)-(vy1-vy3)*(vx1-vx2));
        //polTable.a=(vz2-vz1-polTable.b*(vy1-vy2))/(vx1-vx2);
        
        polTable.a=(vy1-vy2)*(vz1-vz3)-(vy1-vy3)*(vz1-vz2);
        polTable.b=(vz1-vz2)*(vx1-vx3)-(vx1-vx2)*(vz1-vz3);
        polTable.c=(vx1-vx2)*(vy1-vy3)-(vx1-vx3)*(vy1-vy2);
        
        //polTable.a/=polTable.c;
        //polTable.b/=polTable.c;
        //polTable.c=1;
        
        //cout<<polTable.a<<" "<<polTable.b<<" "<<polTable.c<<endl;
        //int nx=pNormal[vndex].x;
        //int ny=pNormal[vndex].y;
        //int nz=pNormal[vndex].z;
        //cout<<nx<<" "<<ny<<" "<<nz<<endl;
        
        
        
        
        //如果投影为线段，不载入
        if(polTable.c==0)continue;
        //color
        //vndex=pface[i].normalIndex[0]-1;
        //float lightadd=-pNormal[vndex].x+pNormal[vndex].y-pNormal[vndex].z;
        //lightadd=lightadd>0?lightadd:0;
        
        
        float minY,maxY;//注意坐标的类型int or float
        int vdex1,vdex2,zvdex;
        float x1,x2;
        int y1,y2;
        int pos;
        vdex1=pface[i].vertexIndex[0]-1;
        xyPos xy=vertexProjection(pVertex[vdex1]);
        y1=xy.y;
        x1=xy.x;
        
        minY=maxY=y1;zvdex=vdex1;
        for(int j=1;j<=pface[i].num;j++)
        {
            if(j==pface[i].num)
                vdex2=pface[i].vertexIndex[0]-1;//必须考虑第一个点是否与最后一个点连接上
            else
                vdex2=pface[i].vertexIndex[j]-1;
            
            xy=vertexProjection(pVertex[vdex2]);
            y2=xy.y;
            x2=xy.x;
            
            if(y2>maxY){maxY=y2;zvdex=vdex2;}
            if(y2==maxY&&x2<vertexProjection(pVertex[zvdex]).x)zvdex=vdex2;
            if(y2<minY)minY=y2;
            if(y1>y2)
            {
                sideTable.x=x1;
                //if y1-y2==0
                sideTable.dx=(x2-x1)/(y1-y2);//dx=-1/k
                sideTable.dy=y1-y2;//精度问题
                sideTable.id=i;
                pos=int(y1);
                if(pos>=0&&pos<winY)
                    m_listClassSideTable[pos].push_back(sideTable);//考虑y1为负值！！！！！
            }
            else if(y2>y1)
            {
                sideTable.x=x2;
                sideTable.dx=(x2-x1)/(y1-y2);//dx=-1/k
                sideTable.dy=y2-y1;
                sideTable.id=i;
                pos=int(y2);
                if(pos>=0&&pos<winY)
                    m_listClassSideTable[pos].push_back(sideTable);
                
            }
            //if y1==y2 边将不被载入分类边表
            x1=x2;y1=y2;
        }
        
        polTable.dy=maxY-minY;
        polTable.z=pVertex[zvdex].z;
        int index = 0;
        pos=int(maxY);
        if(pos>=0&&pos<winY)
            m_listClassPolTable[pos].push_back(polTable);
        
    }
    
}

void scanning_zbuffer::initBuffer()
{
    
    memset(m_color_buffer,1.0,sizeof(ObjFloat)*4*winX);
    //memset(m_depth_buffer,-100.0,sizeof(datatype)*winX);
    for(int i=0;i<winX;i++)
    {
        m_depth_buffer[i]=MINDEPTH;
    }
    
}

void scanning_zbuffer::updateActPolTable()
{
    list<ActPolTable>::iterator it = m_listActPolTable.begin();
    while (it!=m_listActPolTable.end())
    {
        if ((*it).dy==0)
        {
            it=m_listActPolTable.erase(it);
        }
        else
        {
            (*it).dy--;
            it++;
        }
        
        
        
    }
}

void scanning_zbuffer::updateActSideTable(int curY)
{
    list<ActSideTable>::iterator it;
    //处理活化边表中的每一对边
    if(curY==241)
    {
        curY=241;
    }
    int m=m_listActSideTable.size();
    for(it=m_listActSideTable.begin();it!=m_listActSideTable.end();)//
    {
        float xl,xr;
        datatype z,dzx;
        unsigned int id;
        face* pface=m_3DModel.faceVertex;
        id=(*it).id;
        xl=(*it).xl;xr=(*it).xr;
        z=(*it).zl;dzx=(*it).dzx;
        //int tag=0;
        for (int x=(int)(xl+0.5);x<=(int)(xr+0.5);x++)
        {
            
            if(z > m_depth_buffer[x])
            {
                    m_depth_buffer[x]=z;
                    m_color_buffer[x][0]=pface[id].col[0];
                    m_color_buffer[x][1]=pface[id].col[1];
                    m_color_buffer[x][2]=pface[id].col[2];
                
                
            }
            z+=dzx;
            
        }
        --(*it).dyl;--(*it).dyr;
        (*it).xl+=(*it).dxl;
        (*it).xr+=(*it).dxr;
        (*it).zl+=(*it).dzx*(*it).dxl+(*it).dzy;/////???
        if((*it).dyl==0||(*it).dyr==0)
        {
            if((*it).dyl==0&&(*it).dyr==0)
            {
                it=m_listActSideTable.erase(it);//
                
                continue;;
            }
            if((*it).dyl==0)
            {
                addSideTable(LEFT,curY-1,it);//为下一次扫描线添加新边；curY-1
            }
            else 
            {
                addSideTable(RIGHT,curY-1,it);
            }			
        }
        /*
         (*it).xl+=(*it).dxl;
         (*it).xr+=(*it).dxr;
         (*it).zl+=(*it).dzx*(*it).dxl+(*it).dzy;
         */
        it++;
    }
    //modify Active polygon table;
    updateActPolTable();
    
    
}

