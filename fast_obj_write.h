#include <stdlib.h>
#include <stdio.h>

void fast_obj_write(
    unsigned int  position_count,
    float*        positions,
    unsigned int  texcoord_count,
    float*        texcoords,
    unsigned int  face_count,
    unsigned int* face_vertices,
    unsigned int* indices,
    FILE * fout);

#ifdef FAST_OBJ_IMPLEMENTATION
#include <string.h>
#include "ryu/ryu.h"

void fast_obj_write_v(
    unsigned int  position_count,
    float*        positions,
    FILE * fout)
{
    // write positions as "v"
    char buf[64] = "v ";
    for(unsigned int i=1; i<position_count; i++) {
        int index = 2; // strlen("v ")
        // x
        index += f2s_buffered_n(positions[3*i+0], buf+index);
        buf[index++] = ' ';
        // y
        index += f2s_buffered_n(positions[3*i+1], buf+index);
        buf[index++] = ' ';
        // z
        index += f2s_buffered_n(positions[3*i+2], buf+index);
        buf[index++] = '\n';
        // write to file
        fwrite(buf, 1, index, fout);
    }
}

void fast_obj_write_vt(
    unsigned int  texcoord_count,
    float*        texcoords,
    FILE * fout)
{
    // write texcoords as "vt"
    char buf[64] = "vt ";
    for(unsigned int i=1; i<texcoord_count; i++) {
        int index = 3; // strlen("vt ")
        // u
        index += f2s_buffered_n(texcoords[2*i+0], buf+index);
        buf[index++] = ' ';
        // v
        index += f2s_buffered_n(texcoords[2*i+1], buf+index);
        buf[index++] = '\n';
        // write to file
        fwrite(buf, 1, index, fout);
    }
}


void fast_obj_write_vn(
    unsigned int  normal_count,
    float*        normals,
    FILE * fout)
{
    // write normals as "vn"
    char buf[64] = "vn ";
    for(unsigned int i=1; i<normal_count; i++) {
        int index = 3; // strlen("vn ")
        // x
        index += f2s_buffered_n(normals[3*i+0], buf+index);
        buf[index++] = ' ';
        // y
        index += f2s_buffered_n(normals[3*i+1], buf+index);
        buf[index++] = ' ';
        // z
        index += f2s_buffered_n(normals[3*i+2], buf+index);
        buf[index++] = '\n';
        // write to file
        fwrite(buf, 1, index, fout);
    }
}

void fast_obj_write_f(
    unsigned int  face_count,
    unsigned int* face_vertices,
    unsigned int* indices,
    FILE * fout)
{
    // Write faces as
    // case 1: "f v1 v2 ..."
    // case 2: "f v1/vt1 v2/vt2 ..."
    // case 3: "f v1/vt1/vn1 v2/vt2/vn2 ..."
    // case 4: "f v1//vn1 v2//vn2 ..."
    // See https://en.wikipedia.org/wiki/Wavefront_.obj_file
    char buf[256] = "f "; // warning: we do not support too much edges in a face
    unsigned int offset = 0;
    for(unsigned int i=0; i<face_count; i++) {
        int index = 2;
        for(unsigned int j=0; j<face_vertices[i]; j++) {
            // v
            itoa(indices[3*(offset+j)+0], buf+index, 10);
            index += strlen(buf+index);
            // vt
            if (indices[3*(offset+j)+1] > 0) {
                buf[index++] = '/';
                itoa(indices[3*(offset+j)+1], buf+index, 10);
                index += strlen(buf+index);
            }
            // vn
            if (indices[3*(offset+j)+2] > 0) {
                if(indices[3*(offset+j)+1] == 0) // we need two slashes
                    buf[index++] = '/';
                buf[index++] = '/';
                itoa(indices[3*(offset+j)+2], buf+index, 10);
                index += strlen(buf+index);
            }
            //
            buf[index++] = ' ';
        }
        buf[index-1] = '\n';
        fwrite(buf, 1, index, fout);
        offset += face_vertices[i];
    }
}

void fast_obj_write(
    unsigned int  position_count,
    float*        positions,
    unsigned int  texcoord_count,
    float*        texcoords,
    unsigned int  normal_count,
    float*        normals,
    unsigned int  face_count,
    unsigned int* face_vertices,
    unsigned int* indices,
    FILE * fout)
{
    const char info[256] = "# This file uses FP32 for real numbers.\n\n\0";
    fwrite(info, 1, strlen(info), fout);
    // write positions as "v"
    fast_obj_write_v(position_count, positions, fout);
    // write texcoord as "vt"
    fast_obj_write_vt(texcoord_count, texcoords, fout);
    // write normal as "vn"
    fast_obj_write_vn(normal_count, normals, fout);
    // write faces as "f"
    fast_obj_write_f(face_count, face_vertices, indices, fout);
    //
    return;
}

#endif //FAST_OBJ_IMPLEMENTATION
