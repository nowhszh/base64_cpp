#include "FormatData.h"
#include <iostream>
#include <fstream>


//打开路径中的文件
bool FormatData::LoadFile(const std::string& path)
{
	//open the file
	std::ifstream ifs;
	ifs.open(path, std::ios::binary|std::ios::in);
	if (!ifs.is_open()) 
		return false;

    m_data.clear();
    char buf;
    while (ifs.eof() == false)
    {
        ifs.read(&buf, 1);
        m_data.push_back(buf);
    }
    m_data.pop_back();

    ifs.close(); 
	return true;
}

//将编码后的数据保存到对象中
void FormatData::LoadCode(const std::string& code)
{
	m_data = code;
}

//保存对象中保存的数据长度
std::string  FormatData::encode()
{
    std::string result;     //转换后的最终结果
    uint32_t tmp = 0;       //每次转换后的结果储存
    char* four_Byte = (char*)&tmp;      //按字节操作tmp的指针
    //原始数据的头尾指针
    const char* data_begin = m_data.c_str();
    const char* data_end = m_data.c_str() + m_data.size();

    //1. 数据转换
    while (data_end - data_begin > 0)      //数据转换未完成时,循环
    {
        int count = 0;
        four_Byte[count++] = (char)((data_begin[0] & 0xfc) >> 2);     //转换出第1个字节
        if ( data_end - data_begin >= 3)       //剩余字节大于3时, 转换全部
        {
            four_Byte[count++] =(char)(((data_begin[0] & 0x03) << 4) + ((data_begin[1] & 0xf0) >> 4));
            four_Byte[count++] =(char)(((data_begin[1] & 0x0f) << 2) + ((data_begin[2] & 0xc0) >> 6));   
            four_Byte[count++] = (char)(data_begin[2] & 0x3f);
        }
        else if (data_end - data_begin == 2)        //剩余字节等于2时
        {
            four_Byte[count++] = (char)(((data_begin[0] & 0x03) << 4) + ((data_begin[1] & 0xf0) >> 4));       //转换出第2个字节的
            four_Byte[count++] = (char)((data_begin[1] & 0x0f) << 2);     //转换出第3个字节的
        }
        else        //否则剩余剩余字节等于1时
            four_Byte[count++] = (char)((data_begin[0] & 0x03) << 4);     //转换出第2个字节的

        //将转换好的数据与base64表建立关系并储存
        for(auto i = 0; i < count; ++i)
            result.push_back(EnCodeTables[(int)(four_Byte[i])]);
        //迭代
        data_begin += 3;
        tmp = 0;        //置空暂存区
    }

    //2. 根据数据的长度与3取模补充对应数量的"="
    for (auto i = m_data.size() % 3; i != 0 && i < 3; ++i)
        result.push_back('=');

    return result;
}

//对字符串进行base64的编解码
std::string FormatData::decode()
{
    std::string result;     //转换后的最终结果
    char tmp[3] = {0};        //每次转换后的结果储存
    //原始数据的头尾指针
    const char* data_begin = m_data.c_str();
    const char* data_end = m_data.c_str() + m_data.size();

    //1. 数据转换   (取4个字节数据转换成3个字节的数据)
    while (data_end - data_begin > 0)
    {
        int count = 0;
        tmp[count++] = (char)(((DeCodeTables[(int)data_begin[0]]) << 2) + ((DeCodeTables[(int)data_begin[1]]) >> 4));   //将第一个字节的数据取出
        if(data_begin[2] != '=')
        {
            tmp[count++] = (char)(((DeCodeTables[(int)data_begin[1]]) << 4) + ((DeCodeTables[(int)data_begin[2]]) >> 2));   //如果倒数第二个字节不是'=', 则取出第二个字节
            if(data_begin[3] != '=')
                tmp[count++] = (char)(((DeCodeTables[(int)data_begin[2]]) << 6) + DeCodeTables[(int)data_begin[3]]);    //如果第三个字节不是'=', 取出第三个字节
        }

        //储存在结果中
        result.append(tmp, count);
        data_begin += 4;
    }

    return result;
}
