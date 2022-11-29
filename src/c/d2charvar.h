/* 11/22/22/d2charvar.h */

#ifndef __D2CHARVAR_H__
    #define __D2CHARVAR_H__

    #ifdef __cplusplus
        extern "C" {
    #endif

    char **d2charvar(size_t dim1,size_t *dim2);
    void free_d2charvar(char **array);

    #ifdef __cplusplus
        }//extern
    #endif
#endif
