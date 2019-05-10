//
//  transf.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/9/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef transf_h
#define transf_h

#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

using namespace std;
using namespace glm;


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

void prt_mat(mat4 mat)
{
    cout.setf(ios::fixed);
    cout<<setprecision(2);
    cout<<mat[0][0]<<"\t"<<mat[0][1]<<"\t"<<mat[0][2]<<"\t"<<mat[0][3]<<endl;
    cout<<mat[1][0]<<"\t"<<mat[1][1]<<"\t"<<mat[1][2]<<"\t"<<mat[1][3]<<endl;
    cout<<mat[2][0]<<"\t"<<mat[2][1]<<"\t"<<mat[2][2]<<"\t"<<mat[2][3]<<endl;
    cout<<mat[3][0]<<"\t"<<mat[3][1]<<"\t"<<mat[3][2]<<"\t"<<mat[3][3]<<endl<<endl;
}

void prt_vec(vec3* vec)
{
    cout<<vec->x<<" "<<vec->y<<" "<<vec->z<<endl<<endl;
}

void prt_vec(vec4* vec)
{
    cout<<vec->x<<" "<<vec->y<<" "<<vec->z<<" "<<vec->w<<endl<<endl;
}

void test()
{
    vec4 vec(1.0f,1.0f,0.0f,1.0f);
    prt_vec(&vec);
    mat4 trans;
    prt_mat(trans);
    trans= mat4(1.0f);
    prt_mat(trans);
    trans= translate(trans,vec3(1.0f, 1.0f, 0.0f));
    prt_mat(trans);
    vec = trans * vec;
    prt_vec(&vec);
    
    mat4 m4 = ortho(0, 800, 0, 640, 0, 100);
    prt_mat(m4);
    
    m4 = perspective(radians(45.0f), 1920.0f/1080.0f, 2.0f, 10.0f);
    prt_mat(m4);
}


#endif /* transf_h */
