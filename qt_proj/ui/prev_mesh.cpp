#include "prev_mesh.h"


string UI_UtilMesh(string name)
{
    string str = "mesh:\n";

    std::ifstream ifs;
    ifs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        string nname = name.substr(10);
        std::string path = getResPath(nname);
        ifs.open(path, std::ifstream::binary | std::ios::in);
        uint inds = 0,verts = 0, type = 0x0;
        ifs.seekg(0, ios::beg);
        short lod = 1;
        ifs.read((char*)&lod, sizeof(lod));
        ifs.read((char*)(&verts), sizeof(uint));
        ifs.read((char*)(&type), sizeof(uint));
        ifs.read((char*)(&inds), sizeof(uint));


        str+=" lod:"+std::to_string(lod)+"\n";
        str+= " vert:"+std::to_string(verts)+"\n";
        str+= " inds:"+std::to_string(inds)+"\n";

        string types = " types: ";
        if(type & Vt_Pos3)
        {
            types += "pos3 ";
        }
        if(type & Vt_Pos2)
        {
            types += "pos2 ";
        }
        if(type & Vt_UV)
        {
            types += "uv ";
        }
        if(type & Vt_UV2)
        {
            types += "uv2 ";
        }
        if(type & Vt_UV3)
        {
            types += "uv3 ";
        }
        if(type & Vt_UV4)
        {
            types += "uv4 ";
        }
        if(type & Vt_Normal)
        {
            types += "normal ";
        }
        if(type & Vt_TAN)
        {
            types += "tan ";
        }
        if(type & Vt_BIT)
        {
            types += "bitan ";
        }
        if(type & Vt_Color)
        {
            types += "color ";
        }
        if(type & Vt_Skin)
        {
            types += "skin ";
        }
        str+= types+"\n";
        ifs.close();
    } catch (std::ifstream::failure e)
    {
        std::cerr<<"read mesh error "<<name<<std::endl;
    }

    return str;
}
