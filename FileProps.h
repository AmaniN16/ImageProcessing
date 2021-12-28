#pragma once
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;
struct Header {
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
};
class FileProps{
private:
	unsigned char blueP;
	unsigned char greenP;
	unsigned char redP;
public:
	FileProps();
	FileProps(unsigned char blue, unsigned char green, unsigned char red);
	unsigned char getBlue();
	unsigned char getGreen();
	unsigned char getRed();
};

class CurrImage {
private:
	Header currHeader;
	vector<FileProps> imageProps;
	string inPathing;
	string outPathing;
public:
	CurrImage(string inPath, Header daHeader, vector<FileProps> itsProps, string outPath);
	Header getHeader();
	vector<FileProps> getImageProps();
	string getInPath();
	string getOutPath();
};

