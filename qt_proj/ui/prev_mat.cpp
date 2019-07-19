#include "prev_mat.h"
#include "../engine/std/material.h"


string readtex(ifstream& f)
{
    string str = "";
    int ext = 0;
    f.read((char*)&ext,sizeof(int));
    if(ext > 0)
    {
        readstring(f, str);
    }
    return str;
}

string UI_UtilMat(string name)
{
    string str = "mat:\n";
    std::ifstream ifs;
    ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        string nname = name.substr(10);
        std::string path = getResPath(nname);
        ifs.open(path, std::ifstream::binary | std::ios::in);
        str += " diffuse_texture:" +readtex(ifs)+"\n";
        str += " normal_texure:" +readtex(ifs)+"\n";
        str += " specul_texture:" +readtex(ifs)+"\n";
        str += " ambient_texture:" +readtex(ifs)+"\n";
        std::cout<<str<<std::endl;
        ifs.close();
    } catch (std::ifstream::failure e)
    {
        std::cerr<<"read material error "<<name<<std::endl;
    }
    return str;
}



