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
        int index = 2;
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
        int index = 3;
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

void fast_obj_write_f(
    unsigned int  face_count,
    unsigned int* face_vertices,
    unsigned int* indices,
    FILE * fout)
{
    // write faces as "f v/vt v/vt ..."
    char buf[256] = "f ";
    unsigned int offset = 0;
    for(unsigned int i=0; i<face_count; i++) {
        int index = 2;
        for(unsigned int j=0; j<face_vertices[i]; j++) {
            // v
            itoa(indices[3*(offset+j)+0], buf+index, 10);
            index += strlen(buf+index);
            buf[index++] = '/';
            // vt
            itoa(indices[3*(offset+j)+1], buf+index, 10);
            index += strlen(buf+index);
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
    unsigned int  face_count,
    unsigned int* face_vertices,
    unsigned int* indices,
    FILE * fout)
{
    const char info[256] = "# This file is generated by fast_obj_writer.\n\n\0";
    fwrite(info, 1, strlen(info), fout);
    // write positions as "v"
    fast_obj_write_v(position_count, positions, fout);
    // write texcoord as "vt"
    fast_obj_write_vt(texcoord_count, texcoords, fout);
    // write faces as "f"
    fast_obj_write_f(face_count, face_vertices, indices, fout);
    //
    return;
}

#endif //FAST_OBJ_IMPLEMENTATION
