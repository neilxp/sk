#include <iomanip>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <cstring>

#define DH_NUM_BYTES_PER_LINE	16
#define DH_HEX_POS	46	
#define DH_HEX_OFFSET  6
#define DH_HEX_COLUMN_SIZE				8

#define DH_SEPERATOR		' '
#define DH_UNPRINTABLE	'.'

//#define dumpHex(ptr, size) do{\
//	LOG4CXX_TRACE(gCommonLogger, "Dump Hex Value of "#ptr", length: " << size);\
//	__dumpHex((unsigned char * )(ptr), (size));}while(0);

static std::string dumpToHexString(const unsigned char* ptr, size_t len)
{
	if (ptr == NULL || len == 0)
	{
		return "NULL";
	}
	std::ostringstream overallOss, oss2;
	overallOss << "Dump hex value, size " << len << std::endl;
	overallOss << std::hex << std::uppercase << std::setw(6) << std::setfill('0') << int(0) << ": ";

	size_t offset = 0, pos = 0;
	for (size_t idx = 0; idx < len; idx++)
	{
		offset = idx % DH_NUM_BYTES_PER_LINE;
		if (offset == 0) pos = DH_HEX_OFFSET;

		const unsigned char& byte = ptr[idx];
		overallOss << std::setw(2) << std::setfill('0') << (unsigned) byte;
		pos += 2;

		std::isprint(byte) != 0 ? oss2 << byte : oss2 << DH_UNPRINTABLE;
		if (idx + 1 == len || offset + 1 == DH_NUM_BYTES_PER_LINE)
		{
			overallOss << std::setw(DH_HEX_POS - pos + 2 )
				<< std::setfill(DH_SEPERATOR) << "  " << oss2.str();
			oss2.str("");
			if (idx + 1 != len){
				overallOss << "\n";
				overallOss << std::hex << std::uppercase << std::setw(6) << std::setfill('0') << idx + 1 << ": ";
			}
		}
		else if (offset + 1 == DH_HEX_COLUMN_SIZE)
		{
			overallOss << std::setw(1) << std::setfill(DH_SEPERATOR) << "";
			pos += 1;
		}
		else if (offset % 2 == 1)
		{
			overallOss << std::setw(1) << std::setfill(DH_SEPERATOR) << "";
			pos += 1;
		}
	}
	return overallOss.str();
}

using namespace std;

int main(int argc, char** argv)
{
	char *data = "haorehuraoula\x23\x11\x99\xe2\xbb\xbeulrabeulrhalreuchalreuhlraheulraheluhaloeubarcxbalreobkrlcaok";
	cout << dumpToHexString((unsigned char*)data, strlen(data)) << "\n";
	data = "haorehuraoula\x23\x11\x99\xe2\xbb\xbeulrabeulrhalreuchalreuhlraheulraheluhaloeubarcxbalreobkrlcaok1";
	cout << dumpToHexString((unsigned char*)data, strlen(data)) << "\n";
	data = "haorehuraoula\x23\x11\x99\xe2\xbb\xbeulrabeulrhalreuchalreuhlraheulraheluhaloeubarcxbalreobkrlcaok12";
	cout << dumpToHexString((unsigned char*)data, strlen(data)) << "\n";
	data = "haorehuraoula\x23\x11\x99\xe2\xbb\xbeulrabeulrhalreuchalreuhlraheulraheluhaloeubarcxbalreobkrlcao";
	cout << dumpToHexString((unsigned char*)data, strlen(data)) << "\n";
	return 0;
}
