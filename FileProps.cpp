#include "FileProps.h"
FileProps::FileProps() {
	blueP = 0;
	greenP = 0;
	redP = 0;
}

FileProps::FileProps(unsigned char blue, unsigned char green, unsigned char red) {
	blueP = blue;
	greenP = green;
	redP = red;
}

unsigned char FileProps::getBlue() {
	return blueP;
}

unsigned char FileProps::getGreen() {
	return greenP;
}

unsigned char FileProps::getRed() {
	return redP;
}


CurrImage::CurrImage(string inPath, Header daHeader, vector<FileProps> itsProps, string outPath){
	currHeader = daHeader;
	inPathing = inPath;
	outPathing = outPath;
	for (unsigned int i = 0; i < itsProps.size(); i++) {
		imageProps.push_back(itsProps[i]);
	}
}

Header CurrImage::getHeader() {
	return currHeader;
}

vector<FileProps> CurrImage::getImageProps() {
	return imageProps;
}

string CurrImage::getInPath() {
	return inPathing;
}

string CurrImage::getOutPath() {
	return outPathing;
}