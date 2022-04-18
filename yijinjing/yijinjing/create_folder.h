#pragma once
#pragma warning(disable : 4996)
//#define WIN32
#ifdef WIN32//Windows
#include <direct.h>
#include <io.h>
#else// Linux
#ifdef __APPLE__
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <iostream>
#include <string>

inline void create_folder(const std::string& dir_path) {
    if (access(dir_path.c_str(), 0) == -1) {//�жϸ��ļ����Ƿ����
#ifdef WIN32
        int flag = mkdir(dir_path.c_str());//Windows�����ļ���
#else
        int flag = mkdir(dir_path.c_str(), S_IRWXU);//Linux�����ļ���
#endif
        if (flag == 0) {//�����ɹ�
            std::cout << "Create directory successfully." << std::endl;
        } else {//����ʧ��
            std::cout << "Fail to create directory." << std::endl;
            throw std::exception();
        }
    } else {
        std::cout << "This directory already exists." << std::endl;
    }
}
