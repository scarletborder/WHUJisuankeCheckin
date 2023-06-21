/*
 * @Date: 2023-06-20 09:57:28
 * @LastEditors: scarletborder baishuibeef@gmail.com
 * @LastEditTime: 2023-06-21 09:23:09
 * @FilePath: \undefinede:\code\C\study_c\project\WHUJisuankeCheckin\main.cpp
 */
#include "md5.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
std::string IntToString(int n)
{
    std::string ret;
    while (n > 0) {
        char tmp = (n % 10) + '0';
        ret.insert(ret.begin(), tmp);
        n /= 10;
    }
    if (ret.size() <= 1) {
        ret.insert(ret.begin(), '0');
    }
    return ret;
}

unsigned char* getMd5Sum(std::string scpp)
{
    char hexbuf[128] = { 0 };
    strcpy(hexbuf, scpp.c_str());

    int read_len;
    int i;
    char temp[8] = { 0 };
    unsigned char digest[16]; // 存放结果
    // char hexbuf[128] = "12334567";
    unsigned char decrypt[16] = { 0 };
    unsigned char* decrypt32 = new unsigned char[64];

    MD5_CTX md5c;

    MD5Init(&md5c); // 初始化
    read_len = strlen(hexbuf);
    MD5Update(&md5c, (unsigned char*)hexbuf, read_len);

    MD5Final(&md5c, decrypt);
    strcpy((char*)decrypt32, "");

    for (i = 0; i < 16; i++) {
        sprintf(temp, "%02x", decrypt[i]);
        strcat((char*)decrypt32, temp);
    }
    printf("md5:%s\n", decrypt32);
    return decrypt32;
}

int main()
{
    std::cout << "In Windows, please ensure this program and file\"checkin.dat\" are in your local repo \n"
              << std::endl;
    std::ifstream afile;
    afile.open("checkin.dat", std::ios::in);
    std::string Stu_Nu;
    afile >> Stu_Nu;
    afile.close();
    if (Stu_Nu.empty() == true) {
        // 真空
        std::cout << "Initializing: Please enter your student's numero" << std::endl;
        std::cin >> Stu_Nu;
        std::cout << "Please enter your lab's numero" << std::endl;
        std::string labNu;
        std::cin >> labNu;
        Stu_Nu += labNu;
        std::cout << "==Your student's numero plus lab's numero" << Stu_Nu << ",if it is wrong,delete the file\"checkin.dat\" in content==\n\n"
                  << std::endl;

        std::ofstream catIgnore;
        catIgnore.open(".gitignore", std::ios::app | std::ios::out);
        catIgnore << "\n*.exe\ncheckin.dat";
    } else {
        std::cout << "==Your student's numero plus lab's numero" << Stu_Nu << ",if it is wrong,delete the file\"checkin.dat\" in content==\n\n"
                  << std::endl;
    }
    // 首先获得当前时间并转化为时间戳
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::string Date_Str = IntToString(1900 + ltm->tm_year) + "-" + IntToString(1 + ltm->tm_mon) + "-" + IntToString(ltm->tm_mday);

    // 打开当前时间戳对应的文件
    std::ofstream newfile;
    newfile.open(Date_Str, std::ios::out);

    // 判断没有写入文件，则输入当天签到码并写入
    std::string pwd;
    std::cout << "Please enter password today" << std::endl;
    std::cin >> pwd;
    std::string real_stu_nu = std::string(Stu_Nu.begin(), Stu_Nu.begin() + 13);
    std::string real_lab_nu = std::string(Stu_Nu.begin() + 13, Stu_Nu.end());

    std::string premd5 = real_stu_nu + pwd + real_lab_nu;
    newfile << getMd5Sum(premd5);
    std::cout << "create file \"" << Date_Str << " complete, please wait the operator of git" << std::endl;
    newfile.close();
    // 之后的git操作
    system("git add .");
    std::cout << "git add complete" << std::endl;
    std::string cmd2 = "git commit -m \"Add: " + Date_Str + "\"";
    system(cmd2.c_str());
    std::cout << "git commit complete" << std::endl;
    system("git push");
    std::cout << "git push complete" << std::endl;
    std::ofstream ifile;
    ifile.open("checkin.dat", std::ios::out);
    ifile << Stu_Nu;
    ifile.close();

    return 0;
}
