/*******************************************************************************
filename			: SCModel.cpp
author			: Jihwan Oh
email				: dkdkdkdudrn@gmail.com
date				: 2021.01.28

description:
    Codes for Shader class.

*******************************************************************************/
#include <glm/vector_relational.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SCModel.hpp"
/****************************************
    ModelLoader Class
*****************************************/
ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
    m_modelList.clear();
}

void ModelLoader::LoadOBJ(const char* obj_file_path_, const char* name_, bool normalize_)
{
    Model currModel;
    std::string objCode;
    std::ifstream file(obj_file_path_, std::ios::in);
    std::string line; 

    while (std::getline(file, line))
    {
        /*********************************************************/
        //* OBJ FILE FORMAT
        //
        //  v - vertices
        //  vt - texture coordinate
        //  f - there'll be more than 3 infos
        //      1. v v v - only contain vertices indices
        //      2. v/vt v/vt v/vt - contain vertice, texture indices
        //      3. v/vt/vn v/vt/vn v/vt/vn - vertice, texture, normal indices
        //  
        /*********************************************************/
        std::string keyWord = line.substr(0, 2);
        //check v for vertices
        if (keyWord == "v ")
        {
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            float x, y, z;
            v >> x; v >> y; v >> z;
            vert = glm::vec3(x, y, z);

            currModel.m_vertices.push_back(vert);
        }//v end

        //check for texture co-ordinate
        else if (keyWord == "vt")
        {
            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            float U, V;
            v >> U; v >> V;
            tex = glm::vec2(U, V);
            currModel.m_texture.push_back(tex);
        }//vt end
        
         //check for vertex normal
        else if (keyWord == "vn")
        {
            std::istringstream v(line.substr(3));
            glm::vec3 vn;
            float x, y, z;
            v >> x; v >> y; v >> z;
            vn = glm::vec3(x, y, z);

            currModel.m_vertexNormal.push_back(vn);
        }//vn end

        //check for faces
        else if (keyWord == "f ") 
        {
            int a, b, c;   // to store mesh index
            int A, B, C;  // to store texture index
            int x, y, z;    // to store vertex normal
            const char* chh = line.c_str();
            int matches = sscanf(chh, "f %i %i %i", &a, &b, &c); //here it read the line start with f and store the corresponding values in the variables

            if (matches == 3) // f v1 v2 v3 
            {
                // don't forget that indicies need to - 1
                currModel.m_indicies.push_back(--a);
                currModel.m_indicies.push_back(--b);
                currModel.m_indicies.push_back(--c);
            }
            else
            {
                matches = sscanf(chh, "f %i/%i %i/%i %i/%i", &a, &A, &b, &B, &c, &C);
                if (matches == 6) // f v1 / vt1 v2 / vt2 v3 / vt3
                {
                    // don't forget that indicies need to - 1
                    currModel.m_indicies.push_back(--a); 
                    currModel.m_indicies.push_back(--b); 
                    currModel.m_indicies.push_back(--c); 

                    currModel.m_texIndicies.push_back(--A);
                    currModel.m_texIndicies.push_back(--B);
                    currModel.m_texIndicies.push_back(--C);
                }
                else
                {
                    int matches = sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &a, &A, &x, &b, &B, &y, &c, &C, &z); //here it read the line start with f and store the corresponding values in the variables
                    if (matches == 9) // when f has vertex indices, texture indices
                    {
                        // don't forget that indicies need to - 1
                        currModel.m_indicies.push_back(--a); 
                        currModel.m_indicies.push_back(--b);
                        currModel.m_indicies.push_back(--c);

                        currModel.m_texIndicies.push_back(--A);
                        currModel.m_texIndicies.push_back(--B);
                        currModel.m_texIndicies.push_back(--C);

                        currModel.m_vnIndicies.push_back(--x);
                        currModel.m_vnIndicies.push_back(--y);
                        currModel.m_vnIndicies.push_back(--z);
                    }
                }
            }

        } // f end

    }

    int vertSize = static_cast<int>(currModel.m_vertices.size());
    int indSize = static_cast<int>(currModel.m_indicies.size());

    std::cout <<
        "Load '" << obj_file_path_ <<
        "'...Save as '" << name_ << "'>> vertices : " << currModel.m_vertices.size() <<
        ", indices : " << currModel.m_indicies.size() / 3;;

    if (normalize_)
    {
        std::cout <<", NORMALIZED"<< std::endl;
        // values for normalization
        float minX = 0;
        float maxX = 0;
        float minY = 0;
        float maxY = 0;
        float minZ = 0;
        float maxZ = 0;

        for (auto& vertValue : currModel.m_vertices)
        {
            if (minX >= vertValue.x)
            {
                minX = vertValue.x;
            }
            if (maxX <= vertValue.x)
            {
                maxX = vertValue.x;
            }

            if (minY >= vertValue.y)
            {
                minY = vertValue.y;
            }
            if (maxY <= vertValue.y)
            {
                maxY = vertValue.y;
            }

            if (minZ >= vertValue.z)
            {
                minZ = vertValue.z;
            }
            if (maxZ <= vertValue.z)
            {
                maxZ = vertValue.z;
            }
        }

        float xGap = abs(maxX - minX);
        float yGap = abs(maxY - minY);
        float zGap = abs(maxZ - minZ);

        float toScale = 1 / glm::max(xGap, glm::max(yGap, zGap));
        glm::vec3 toMove((minX + maxX) / 2.f,
            (minY + maxY) / 2.f,
            (minZ + maxZ) / 2.f);

        for (auto &vertValue : currModel.m_vertices)
        {
            vertValue -= toMove;
            vertValue *= toScale;
        }
    }// normalize end

    // Get vertex normal
    std::vector<std::vector<glm::vec3>> vertexNormals(currModel.m_vertices.size());

    
    for (int i = 0; i < indSize; i = i + 3)
    {
        glm::vec3 aPoint = currModel.m_vertices[currModel.m_indicies[i]];
        glm::vec3 bPoint = currModel.m_vertices[currModel.m_indicies[i + 1]];
        glm::vec3 cPoint = currModel.m_vertices[currModel.m_indicies[i + 2]];

        // Get two line from points
        glm::vec3 aLine = aPoint - bPoint;
        glm::vec3 bLine = cPoint - bPoint;

        glm::vec3 currNorm = glm::normalize(glm::cross(bLine, aLine));

        vertexNormals[currModel.m_indicies[i]].push_back(currNorm);
        vertexNormals[currModel.m_indicies[i + 1]].push_back(currNorm);
        vertexNormals[currModel.m_indicies[i + 2]].push_back(currNorm);
    }

    std::vector<glm::vec3> normalVector;
    for (auto& verNorm : vertexNormals)
    {
        glm::vec3 currNormal(0.0f);

        for (auto& eachNormal : verNorm)
        {
            currNormal += eachNormal;
        }
        
        normalVector.push_back(currNormal);
    }
    
    for (int i = 0; i < currModel.m_vnIndicies.size(); ++i)
    {
        glm::vec3 fileNorm = currModel.m_vertexNormal[currModel.m_vnIndicies[i]];
        glm::vec3 myNorm = normalVector[currModel.m_vnIndicies[i]];

        std::cout << "( " << fileNorm.x << ", " << fileNorm.y << ", " << fileNorm.z << ")== FVN" << std::endl;
        std::cout << "( " << fileNorm.x << ", " << fileNorm.y << ", " << fileNorm.z << ")== MVN" << std::endl;
    }
    currModel.m_vertexNormal = normalVector;

    m_modelList.emplace(name_, currModel);
}

Model* ModelLoader::GetModel(const char* name_)
{
    Model* currModel = &m_modelList.find(std::string(name_))->second;

    return currModel;
}
