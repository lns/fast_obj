#include <cstdio>
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"
#include "fast_obj_write.h"

class FastObj
{
public:
    fastObjMesh * ptr;

    FastObj() : ptr(nullptr) {}
    FastObj(const char * path) {
        ptr = fast_obj_read(path);
    }

    virtual ~FastObj() {
        if(nullptr != ptr)
            fast_obj_destroy(ptr);
        ptr = nullptr;
    }

    void info() {
        printf("position_count: %lu\n", ptr->position_count);
        printf("texcoord_count: %lu\n", ptr->texcoord_count);
        printf("normal_count:   %lu\n", ptr->normal_count);
        printf("face_count: %lu\n",     ptr->face_count);
        printf("material_count: %lu\n", ptr->material_count);
        printf("group_count: %lu\n",    ptr->group_count);
        for(int i=0; i<10; i++) {
            printf("%lu ", ptr->face_vertices[i]);
        }
        printf("\n");
        for(int i=0; i<10; i++) {
            printf("(%lu %lu %lu) ",
                ptr->indices[i].p, ptr->indices[i].t, ptr->indices[i].n);
        }
        printf("\n");
    }

    void write(const char * path) {
        FILE * fout = fopen(path, "wb"); // todo: failure check
        fast_obj_write(
            ptr->position_count,
            ptr->positions,
            ptr->texcoord_count,
            ptr->texcoords,
            ptr->face_count,
            ptr->face_vertices,
            (unsigned int*)ptr->indices,
            fout);
        fclose(fout);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, " Usage: %s obj_path\n", argv[0]);
        return 0;
    }
    FastObj obj(argv[1]);
    obj.info();
    obj.write("tmpout.obj");
    return 0;
}
