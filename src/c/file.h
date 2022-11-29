/* 9/20/22 file.h */
#ifndef __FILE_H__
    #define __FILE_H__

    #include <vector>
    #include <string>

    class file {
        public:

            //char **files,**identify;
            //int *nfileseach;
            //size_t nfiles,nstrings,max_length,*strlen_files,*strlen_identify;
            //START221029
            std::vector<std::string> files;
            std::vector<int> nfileseach;

            //file();
            //~file();
            //START221029
            file(){};
            virtual ~file(){};

            int read_files(int nfiles,char **argv);
            int get_files(int num_region_files,char **argv);

            //START221029
            //int init_FS(size_t tnfiles,int nfiles);
            //int read_conc(int nfile,char **file);
        };
#endif
