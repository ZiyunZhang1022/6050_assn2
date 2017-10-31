//
//  Obj.cpp
//  6050_assn2
//
//  Created by 张子筠 on 10/27/17.
//  Copyright © 2017 ziyunz. All rights reserved.
//

#include "Obj.h"
#include <stdlib.h>

bool flag = true;
CLoadObj::CLoadObj()
{
    //m_pVertice.reserve(300000);
    //m_pCoord.reserve(300000);
    //m_pNormal.reserve(300000);
    //m_pFace.reserve(100000);
}

//将obj文件的信息读入指定的模型中
bool CLoadObj::ImportObj(model *myModel, char *strFileName) {
    char strMessage[255] = {0};				// 用于显示错误信息
    
    // 判断是否是一个合法的模型和文件类型
    if(!myModel || !strFileName) return false;
    
    // 以只读方式打开文件，返回文件指针
    m_FilePointer = fopen(strFileName, "r");
    
    // 判断文件指针是否正确
    if(!m_FilePointer) {
        // 如果文件指针不正确，则显示错误信息
        sprintf(strMessage, "Unable to find or open the file: %s", strFileName);
        //MessageBox(NULL, strMessage, "Error", MB_OK);
        return false;
    }
    
    // 读入文件信息
    ReadObjFile(myModel);
    
    //让模型绕固定点旋转，比如绕模型的一个Z坐标最小的点旋转，
    //则要求出最小Z坐标，然后把模型所有的Z坐标都平移一下，再旋转即可
    
    // 关闭打开的文件
    fclose(m_FilePointer);
    
    return true;
}

void CLoadObj::ReadObjFile(model *myModel) {
    char strLine[100]		= {0};
    char ch					= 0;
    
    while(!feof(m_FilePointer))
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        
        // 获得obj文件中的当前行的第一个字符
        ch = fgetc(m_FilePointer);
        
        switch(ch)
        {
            case 'v':
                //读入点的信息，要区分顶点 ("v")、法向量 ("vn")、纹理坐标 ("vt")
                ReadVertexInfo();
                break;
                
            case 'f':						//读入的是'f'
                //读入面的信息
                ReadFaceInfo();
                break;
            case 's':
                //if(flag) flag = false;
//                else flag = true;
            default:
                //略过该行的内容
                if (ch != '\n') fgets(strLine, 100, m_FilePointer);
                break;
        }
    }
    FillInObjectInfo(myModel);
}

void CLoadObj::ReadVertexInfo()
{
    static int numVertex;
    vector3f vNewVertex		= {0};
    char strLine[100]		= {0};
    char ch = 0;
    
    //读入第一个字符，判断读入的是否顶点/法向量/UV坐标
    ch = fgetc(m_FilePointer);
    
    //printf("%d\n", ++numVertex);
    
    switch(ch)
    {
        case ' ':					//如果是空格，则必是顶点("v")
            //读入顶点坐标，格式是"v x y z"
            fscanf(m_FilePointer, "%f %f %f", &vNewVertex.x, &vNewVertex.y, &vNewVertex.z);
            
            //读入该行中余下的内容，则文件指针到下一行
            fgets(strLine, 100, m_FilePointer);
            //添加一个新的顶点到顶点链表中
            m_pVertice.push_back(vNewVertex);
            
            break;
            
            
        default:
            fgets(strLine, 100, m_FilePointer);
            
            break;
    }
}

void CLoadObj::ReadFaceInfo()
{
    static int numFace;
    face newFace;
    newFace.num = 3;
    int vertex;
    
    char strLine[100]		= {0};
    char ch					= 0;
    
    
    fscanf(m_FilePointer, "%c", &ch);
    fgets(strLine, 100, m_FilePointer);
    //读入对象的顶点和法线坐标索引，格式“顶点1//法向1 顶点2//法向2 ……”
    sscanf(strLine,"%d/%d/%d %d/%d/%d %d/%d/%d ",
           &newFace.vertexIndex[0],&newFace.normalIndex[0],&newFace.coordIndex[0],
           &newFace.vertexIndex[1],&newFace.normalIndex[1],&newFace.coordIndex[1],
           &newFace.vertexIndex[2],&newFace.normalIndex[2],&newFace.coordIndex[2]);
    
    //添加一个新面到面链表中
    srand((unsigned)time(NULL));
    int vexCount = m_pVertice.size();
    if(flag) {
        newFace.col[0] = 0.5;
        newFace.col[1] = 0.1;
        newFace.col[2] = 0.2;
        flag = false;
        
    }else {
        newFace.col[0] = 0.3;
        newFace.col[1] = 0.4;
        newFace.col[2] = 0.6;
        flag = true;
    }
   
    m_pFace.push_back(newFace);
    
    //fgets(strLine, 100, m_FilePointer);
}

void CLoadObj::FillInObjectInfo(model *myModel)
{
    int textureOffset = 0, vertexOffset = 0;
    int i = 0, j;
    m_pVertice = fixCoordinate(m_pVertice);
    //获得顶点的数量、纹理坐标的数量、法线数量和面的数量
    myModel->vertexCount = m_pVertice.size();
    myModel->faceCount = m_pFace.size();
    
    //如果读入了点
    if(myModel->vertexCount) {
        
        //分配保存点的存储空间
        myModel->vertex = new vector3f [myModel->vertexCount];
    }
    
    //如果读入了面
    if(myModel->faceCount)
    {
        //分配保存面的存储空间
        myModel->faceVertex = new face [myModel->faceCount];
    }
    
    // 遍历对象中的所有点
    for(i = 0; i < myModel->vertexCount; i++)
    {
        // 将当前的顶点从临时链表中拷贝到模型链表中
        myModel->vertex[i] = m_pVertice[i];
    }
    
    
    // 遍历所有的面
    for(i = 0; i < myModel->faceCount; i++)
    {
        // 拷贝临时的面链表到模型链表中
        myModel->faceVertex[i] = m_pFace[i];
        
        /*
         pModel->pFace[i].num = m_pFace[i].num;
         strcpy(pModel->pFace[i].material, m_pFace[i].material);
         
         for (j=0;j<pModel->pFace[i].num;j++)
         {
         pModel->pFace[i].vertexIndex[j] = m_pFace[i].vertexIndex[j];
         pModel->pFace[i].coordIndex[j] = m_pFace[i].coordIndex[j];
         pModel->pFace[i].normalIndex[j] = m_pFace[i].normalIndex[j];
         
         //printf("f %d p %d: %d %d %d\n", i, j, pModel->pFace[i].vertexIndex[j], pModel->pFace[i].coordIndex[j], pModel->pFace[i].normalIndex[j]);
         }
         */
    }
    
    //由于OBJ文件中没有材质，因此将materialID设置为-1，必须手动设置材质
    
    
    //清除所有的临时链表
    m_pVertice.clear();
    m_pFace.clear();
    
    
    // 设置所有的布尔值为false
    m_bJustReadAFace = false;
}

vector<vector3f> CLoadObj:: fixCoordinate(vector<vector3f> vertexs) {
    float xSum = 0.0;
    float ySum = 0.0;
    float zSum = 0.0;
    int size = vertexs.size();
    for(int i = 0; i < vertexs.size(); i++) {
        xSum += vertexs.at(i).x;
        ySum += vertexs.at(i).y;
        zSum += vertexs.at(i).z;
    }
    
    float xAverage = xSum / size;
    float yAverage = ySum / size;
    float zAverage = zSum / size;
    
    vector<vector3f> newPoints1;
    
    for(int i = 0; i < size; i++) {
        vector3f newPoint;
        newPoint.x = vertexs.at(i).x - xAverage;
        newPoint.y = vertexs.at(i).y - yAverage;
        newPoint.z = vertexs.at(i).z - zAverage;
        newPoints1.push_back(newPoint);
    }
    
    return newPoints1;
}
