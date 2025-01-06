#include <stdio.h>

struct ssalloc{
    void *ptr;
    ssize_t *sref_count;
};

struct ssalloc smem_alloc(void* sdata) {
    struct ssalloc s;
    s.ptr = sdata;
    s.sref_count = malloc(sizeof(ssize_t));
    if (s.sref_count) *s.sref_count += 1;
    return s;
}

void* ssrealloc(struct ssalloc* sdata) {
    if (sdata && sdata->sref_count)
        *(sdata->sref_count)++;
}

void release(struct ssalloc* sdata) {
    if (sdata && sdata->sref_count){
        (*sdata->sref_count)--;
        if(*sdata->sref_count == 0){
            free(sdata->ptr);
            free(sdata->sref_count);
            sdata->ptr = NULL;
            sdata->sref_count = NULL;
        }
    }
}