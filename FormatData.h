#pragma once
#include <string>


//对数据进行base64编解码的类
class FormatData
{
public:
	FormatData() = default;
	~FormatData() = default;
	//将文件读取到对象中
	bool LoadFile(const std::string& path);
	//将编码后的数据保存到对象中
	void LoadCode(const std::string& code);
	//对字符串进行base64的编解码
	std::string encode();
	std::string decode(); 

	//保存对象中保存的数据长度
	inline size_t size() { return m_data.size(); }
private:
	std::string m_data;
    const char EnCodeTables[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char DeCodeTables[124] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,      //0-9
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,      //10-19
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,      //20-29
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,      //30-39
        0,  0,  0,  62, 0,  0,  0,  63, 52, 53,     //40-49
        54, 55, 56, 57, 58, 59, 60, 61, 0,  0,      //50-59
        0,  0,  0,  0,  0,  0,  1,  2,  3,  4,      //60-69
        5,  6,  7,  8,  9,  10, 11, 12, 13, 14,     //70-79
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24,     //80-89
        25, 0,  0,  0,  0,  0,   0, 26, 27, 28,      //90-99
        29, 30, 31, 32, 33, 34, 35, 36, 37, 38,     //100-109
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48,     //110-119
        49, 50, 51, 0                               //120-123   
    };
};

