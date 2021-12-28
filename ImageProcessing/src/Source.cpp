#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include "FileProps.h"
using namespace std;



void writeFile(string filePath, Header currHeader, vector<FileProps>& props) {
	
		ofstream file(filePath, ios_base::binary);

		//writing the header 
		file.write(&currHeader.idLength, sizeof(currHeader.idLength));
		file.write(&currHeader.colorMapType, sizeof(currHeader.colorMapType));
		file.write(&currHeader.dataTypeCode, sizeof(currHeader.dataTypeCode));
		file.write((char*)&currHeader.colorMapOrigin, sizeof(currHeader.colorMapOrigin));
		file.write((char*)&currHeader.colorMapLength, sizeof(currHeader.colorMapLength));
		file.write(&currHeader.colorMapDepth, sizeof(currHeader.colorMapDepth));
		file.write((char*)&currHeader.xOrigin, sizeof(currHeader.xOrigin));
		file.write((char*)&currHeader.yOrigin, sizeof(currHeader.yOrigin));
		file.write((char*)&currHeader.width, sizeof(currHeader.width));
		file.write((char*)&currHeader.height, sizeof(currHeader.height));
		file.write(&currHeader.bitsPerPixel, sizeof(currHeader.bitsPerPixel));
		file.write(&currHeader.imageDescriptor, sizeof(currHeader.imageDescriptor));

		int width = (int)currHeader.width;
		int height = (int)currHeader.height;

		//Writing the pixels
		int pixelSize = width * height;
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		
		for (unsigned int j = 0; j < pixelSize; j++) {
			blue = props[j].getBlue();
			green = props[j].getGreen();
			red = props[j].getRed();
			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}

}

void readFile(string filePath, vector<Header>& theseHeaders, vector<FileProps>& props, vector<CurrImage>& InputImages, string outPath) {
	ifstream file(filePath, ios_base::binary);

	//Reading the header 
	Header currHeader;
	file.read(&currHeader.idLength, sizeof(currHeader.idLength));
	file.read(&currHeader.colorMapType, sizeof(currHeader.colorMapType));
	file.read(&currHeader.dataTypeCode, sizeof(currHeader.dataTypeCode));
	file.read((char*)&currHeader.colorMapOrigin, sizeof(currHeader.colorMapOrigin));
	file.read((char*)&currHeader.colorMapLength, sizeof(currHeader.colorMapLength));
	file.read(&currHeader.colorMapDepth, sizeof(currHeader.colorMapDepth));
	file.read((char*)&currHeader.xOrigin, sizeof(currHeader.xOrigin));
	file.read((char*)&currHeader.yOrigin, sizeof(currHeader.yOrigin));
	file.read((char*)&currHeader.width, sizeof(currHeader.width));
	file.read((char*)&currHeader.height, sizeof(currHeader.height));
	file.read(&currHeader.bitsPerPixel, sizeof(currHeader.bitsPerPixel));
	file.read(&currHeader.imageDescriptor, sizeof(currHeader.imageDescriptor));
	int width = (int)currHeader.width;
	int height = (int)currHeader.height;
	theseHeaders.push_back(currHeader);

	//Print header values

	//Reading the pixels
	int pixelSize = width * height;
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	for (unsigned int i = 0; i < pixelSize; i++) {
		file.read((char*)&blue, sizeof(blue));
		file.read((char*)&green, sizeof(green));
		file.read((char*)&red, sizeof(red));
		FileProps currPixel(blue, green, red);
		props.push_back(currPixel);
	}
	CurrImage thisImage(filePath, currHeader, props, outPath);
	InputImages.push_back(thisImage);
}

void multiply(CurrImage image1, CurrImage image2, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> image2Props = image2.getImageProps();
	vector<FileProps> outputProps;
	unsigned char blue1;
	unsigned char green1;
	unsigned char red1;
	unsigned char blue2;
	unsigned char green2;
	unsigned char red2;
	float blue;
	float green;
	float red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		blue1 = image1Props[i].getBlue();
		blue2 = image2Props[i].getBlue();
		blue = (((float)blue1 * (float)blue2) / 255.0f) + 0.5f;

		green1 = image1Props[i].getGreen();
		green2 = image2Props[i].getGreen();
		green = (((float)green1 * (float)green2) / 255.0f) + 0.5f;

		red1 = image1Props[i].getRed();
		red2 = image2Props[i].getRed();
		red = (((float)red1 * (float)red2) / 255.0f) + 0.5;
		
		FileProps outputImageProp((unsigned char) blue, (unsigned char) green, (unsigned char) red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);
}

void multiplyVals(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green;
	unsigned char blue;
	unsigned char red;
	unsigned char red1;
	unsigned char blue1;
	float tempRed;
	float tempBlue;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		blue1 = image1Props[i].getBlue();
		red1 = image1Props[i].getRed();
		
		tempBlue = (float)blue1 * 0.0f;
		if (tempBlue > 255.0f) {
			tempBlue = 255.0f;
		}
		blue = (unsigned char)tempBlue;

		tempRed = (float)red1 * 4.0f;
		if (tempRed > 255.0f) {
			tempRed = 255.0f;
		}
		red = (unsigned char)tempRed;
		green = image1Props[i].getGreen();

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);

}

void subtract(CurrImage image1, CurrImage image2, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> image2Props = image2.getImageProps();
	vector<FileProps> outputProps;
	unsigned char blue1;
	unsigned char green1;
	unsigned char red1;
	unsigned char blue2;
	unsigned char green2;
	unsigned char red2;
	int blue;
	int green;
	int red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		blue1 = image1Props[i].getBlue();
		blue2 = image2Props[i].getBlue();
		blue = blue1 - blue2;
		if (blue < 0) {
			blue = 0;
		}
		green1 = image1Props[i].getGreen();
		green2 = image2Props[i].getGreen();
		green = green1 - green2;
		if (green < 0) {
			green = 0;
		}
		red1 = image1Props[i].getRed();		
		red2 = image2Props[i].getRed();
		red = red1 - red2;
		if (red < 0) {
			red = 0;
		}

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);


}

void add(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green1;
	unsigned char blue;
	int green;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		blue = image1Props[i].getBlue();
		red = image1Props[i].getRed();

		green1 = image1Props[i].getGreen();
		green = green1 + 200;
		if (green > 255) {
			green = 255;
		}

		FileProps outputImageProp(blue, (unsigned char)green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);
}

void screen(CurrImage image1, CurrImage image2, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> image2Props = image2.getImageProps();
	vector<FileProps> outputProps;
	unsigned char blue1;
	unsigned char green1;
	unsigned char red1;
	unsigned char blue2;
	unsigned char green2;
	unsigned char red2;
	float tempBlue;
	float tempGreen;
	float tempRed;
	unsigned char blue;
	unsigned char green;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		blue1 = image1Props[i].getBlue();
		blue2 = image2Props[i].getBlue();
		tempBlue = (1.0f-(1.0f-((float)blue1/255.0f))*(1-((float)blue2/255)));
		blue = (unsigned char)(tempBlue * 255.0f + 0.5f);
		if (blue < 0) {
			blue = 0;
		}
		else if (blue > 255) {
			blue = 255;
		}
		green1 = image1Props[i].getGreen();
		green2 = image2Props[i].getGreen();
		tempGreen = (1.0f - (1.0f - ((float)green1 / 255.0f)) * (1.0f - ((float)green2 / 255.0f)));
		green = (unsigned char)(tempGreen*255.0f + 0.5f);
		if (green < 0) {
			green = 0;
		}
		else if (green > 255) {
			green = 255;
		}
		red1 = image1Props[i].getRed();
		red2 = image2Props[i].getRed();
		tempRed = (1.0f - (1.0f - ((float)red1 / 255.0f)) * (1.0f - ((float)red2 / 255.0f)));
		red = (unsigned char)(tempRed*255 + 0.5);
		if (red < 0) {
			red = 0;
		}
		else if (red > 255) {
			red = 255;
		}

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);
}

void overlay(CurrImage image1, CurrImage image2, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> image2Props = image2.getImageProps();
	vector<FileProps> outputProps;
	unsigned char blue1;
	unsigned char green1;
	unsigned char red1;
	unsigned char blue2;
	unsigned char green2;
	unsigned char red2;
	float tempBlue;
	float tempGreen;
	float tempRed;
	unsigned char blue;
	unsigned char green;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		//Blue
		blue1 = image1Props[i].getBlue();
		blue2 = image2Props[i].getBlue();
		if (((float)blue2/255.0f) <= 0.5f) {
			tempBlue = (2.0f * ((float)blue1/255.0f) * ((float)blue2/255.0f));
			blue = (unsigned char)(tempBlue * 255.0f + 0.5f);
		}
		else {
			tempBlue = (1.0f - (2*(1.0f - ((float)blue1 / 255.0f)) * (1.0f - ((float)blue2 / 255.0f))));
			blue = (unsigned char)(tempBlue * 255.0f + 0.5f);
		}
		if (blue < 0) {
			blue = 0;
		}
		else if (blue > 255) {
			blue = 255;
		}
		//Green
		green1 = image1Props[i].getGreen();
		green2 = image2Props[i].getGreen(); 
		if (((float)green2 / 255.0f) <= 0.5f) {
			tempGreen = (2.0f * ((float)green1 / 255.0f) * ((float)green2 / 255.0f));
			green = (unsigned char)(tempGreen * 255.0f + 0.5f);
		}
		else {
			tempGreen = (1.0f - (2.0f * (1.0f - ((float)green1 / 255.0f)) * (1.0f - ((float)green2 / 255.0f))));
			green = (unsigned char)(tempGreen * 255.0f + 0.5f);
		}
		if (green < 0) {
			green = 0;
		}
		else if (green > 255) {
			green = 255;
		}
		//Green
		red1 = image1Props[i].getRed();
		red2 = image2Props[i].getRed(); 
		if (((float)red2 / 255.0f) <= 0.5f) {
			tempRed = (2.0f * ((float)red1 / 255.0f) * ((float)red2 / 255.0f));
			red = (unsigned char)(tempRed * 255.0f + 0.5f);
		}
		else {
			tempRed = (1.0f - (2.0f * (1.0f - ((float)red1 / 255.0f)) * (1.0f - ((float)red2 / 255.0f))));
			red = (unsigned char)(tempRed * 255.0f + 0.5f);
		}
		if (red < 0) {
			red = 0;
		}
		else if (red > 255) {
			red = 255;
		}

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);


}

void setRed(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green;
	unsigned char blue;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		red = image1Props[i].getRed();
		blue = image1Props[i].getRed();
		green = image1Props[i].getRed();

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);

}

void setGreen(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green;
	unsigned char blue;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		red = image1Props[i].getGreen();
		blue = image1Props[i].getGreen();
		green = image1Props[i].getGreen();

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);

}

void setBlue(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green;
	unsigned char blue;
	unsigned char red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		red = image1Props[i].getBlue();
		blue = image1Props[i].getBlue();
		green = image1Props[i].getBlue();

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);

}

void combine(CurrImage redCol, CurrImage greenCol, CurrImage blueCol, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> redProps = redCol.getImageProps();
	vector<FileProps> greenProps = greenCol.getImageProps();
	vector<FileProps> blueProps = blueCol.getImageProps();
	vector<FileProps> outputProps;
	unsigned char green;
	unsigned char blue;
	unsigned char red;

	for (unsigned int i = 0; i < redProps.size(); i++) {
		red = redProps[i].getRed();
		green = greenProps[i].getGreen();
		blue = blueProps[i].getBlue(); 
		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);

}

void flip(CurrImage image1, Header firstHeader, vector<CurrImage>& outputImages, string outPath) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> outputProps;
	int green;
	int blue;
	int red;

	for (unsigned int i = 0; i < image1Props.size(); i++) {
		red = image1Props[image1Props.size() - 1 - i].getRed();
		blue = image1Props[image1Props.size() - 1 - i].getBlue();
		green = image1Props[image1Props.size() - 1 - i].getGreen();

		FileProps outputImageProp(blue, green, red);
		outputProps.push_back(outputImageProp);
	}

	CurrImage outputI("", firstHeader, outputProps, outPath);
	outputImages.push_back(outputI);


}

void test(CurrImage image1, CurrImage image2, string testNum, vector<int>& successfulTests) {
	vector<FileProps> image1Props = image1.getImageProps();
	vector<FileProps> image2Props = image2.getImageProps();
	bool rEqual;
	bool gEqual;
	bool bEqual;
	
	for (unsigned int i = 0; i < image1Props.size(); i++) {
		if (image1Props[i].getRed() == image2Props[i].getRed()) {
			rEqual = true;
		}

		else {
			rEqual = false;
			cout << "Test #" << testNum << "....Failed" << endl;
			break;
		}

		if (image1Props[i].getGreen() == image2Props[i].getGreen()) {
			gEqual = true;
		}

		else {
			gEqual = false;
			cout << "Test #" << testNum << "....Failed" << endl;
			break;
		}

		if (image1Props[i].getBlue() == image2Props[i].getBlue()) {
			bEqual = true;
		}

		else {
			bEqual = false;
			cout << "Test #" << testNum << "....Failed" << endl;
			break;
		}
	}

	if (rEqual == true && gEqual == true && bEqual == true) {
		cout << "Test #" << testNum << "....Successful!" << endl;
		successfulTests.push_back(1);
	}

}


int main() {
	vector<FileProps> image;
	vector<CurrImage> inputImages;
	vector<CurrImage> outputImages;
	vector<CurrImage> exampleImages;
	vector<int> failedTests;
	vector<int> successfulTests;
	vector<Header> exHeaders;
	vector<Header> theseHeaders;
	string inPathing;
	string outPathing;

	//INPUTS
	{
		//Layer 1 input[0]
		inPathing = "input/layer1.tga";
		outPathing = "/output/layer1.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Pattern 1 input[1]
		inPathing = "input/pattern1.tga";
		outPathing = "/output/pattern1.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Layer 2 input[2]
		inPathing = "input/layer2.tga";
		outPathing = "/output/layer2.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//car input [3]
		inPathing = "input/car.tga";
		outPathing = "/output/car.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Pattern 2 input[4]
		inPathing = "input/pattern2.tga";
		outPathing = "/output/pattern2.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Text input[5]
		inPathing = "input/text.tga";
		outPathing = "/output/text.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Circles input[6]
		inPathing = "input/circles.tga";
		outPathing = "/output/circles.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Layer red input[7]
		inPathing = "input/layer_red.tga";
		outPathing = "/output/layer_red.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Layer green input[8]
		inPathing = "input/layer_green.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

		//Layer blue input[9]
		inPathing = "input/layer_blue.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();


		//Text2 input[10]
		inPathing = "input/text2.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, inputImages, outPathing);
		image.clear();

	}

	//Examples
	{
		//Part 1 ex[0]
		inPathing = "examples/EXAMPLE_part1.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 2
		inPathing = "examples/EXAMPLE_part2.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 3
		inPathing = "examples/EXAMPLE_part3.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 4
		inPathing = "examples/EXAMPLE_part4.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 5
		inPathing = "examples/EXAMPLE_part5.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 6
		inPathing = "examples/EXAMPLE_part6.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//Part 7
		inPathing = "examples/EXAMPLE_part7.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//8r
		inPathing = "examples/EXAMPLE_part8_r.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//8g
		inPathing = "examples/EXAMPLE_part8_g.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//8b
		inPathing = "examples/EXAMPLE_part8_b.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//9
		inPathing = "examples/EXAMPLE_part9.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

		//10
		inPathing = "examples/EXAMPLE_part10.tga";
		outPathing = "/output/expt1.tga";
		readFile(inPathing, theseHeaders, image, exampleImages, outPathing);
		image.clear();

	}



	//Multiply aka part 1
	{
		outPathing = "output/part1.tga";
		multiply(inputImages[0], inputImages[1], inputImages[0].getHeader(), outputImages, outPathing);
		Header header = outputImages[0].getHeader();
		image = outputImages[0].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[0], outputImages[0], "1", successfulTests);

	}

	//Subtract aka part 2
	{
		outPathing = "output/part2.tga";
		subtract(inputImages[3], inputImages[2], inputImages[2].getHeader(), outputImages, outPathing);
		Header header = outputImages[1].getHeader();
		image = outputImages[1].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[1], outputImages[1], "2", successfulTests);
	}

	//Screen aka part 3
	{
		outPathing = "output/part3.tga";
		multiply(inputImages[0], inputImages[4], inputImages[0].getHeader(), outputImages, outPathing); // creates output[2]
		screen(inputImages[5], outputImages[2], outputImages[2].getHeader(), outputImages, outPathing);
		Header header = outputImages[3].getHeader();
		image = outputImages[3].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[2], outputImages[3], "3", successfulTests);
	}

	//Part 4
	{
		outPathing = "output/part4.tga";
		multiply(inputImages[6], inputImages[2], inputImages[6].getHeader(), outputImages, outPathing); //Creates output[4]
		subtract(outputImages[4], inputImages[4], outputImages[4].getHeader(), outputImages, outPathing);
		Header header = outputImages[5].getHeader();
		image = outputImages[5].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[3], outputImages[5], "4", successfulTests);
	}

	//Part 5
	{
		outPathing = "output/part5.tga";
		overlay(inputImages[0], inputImages[1], inputImages[0].getHeader(), outputImages, outPathing);
		Header header = outputImages[6].getHeader();
		image = outputImages[6].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[4], outputImages[6], "5", successfulTests);
	}
	
	//Part 6
	{
		outPathing = "output/part6.tga";
		add(inputImages[3], inputImages[3].getHeader(), outputImages, outPathing);
		Header header = outputImages[7].getHeader();
		image = outputImages[7].getImageProps();
		writeFile(outPathing, header, image);

		test(exampleImages[5], outputImages[7], "6", successfulTests);
	}

	//Part 7
	{
		outPathing = "output/part7.tga";
		multiplyVals(inputImages[3], inputImages[3].getHeader(), outputImages, outPathing);
		Header header = outputImages[8].getHeader();
		image = outputImages[8].getImageProps();
		writeFile(outPathing, header, image);

		test(exampleImages[6], outputImages[8], "7", successfulTests);
		
	}

	//Part 8
	{
		//red
		outPathing = "output/part8_r.tga";
		setRed(inputImages[3], inputImages[3].getHeader(), outputImages, outPathing);
		Header header = outputImages[9].getHeader();
		image = outputImages[9].getImageProps();
		writeFile(outPathing, header, image);
		test(exampleImages[7], outputImages[9], "8", successfulTests);

		//green
		outPathing = "output/part8_g.tga";
		setGreen(inputImages[3], inputImages[3].getHeader(), outputImages, outPathing);
		Header header2 = outputImages[10].getHeader();
		image = outputImages[10].getImageProps();
		writeFile(outPathing, header2, image);
		test(exampleImages[8], outputImages[10], "9", successfulTests);

		//blue
		outPathing = "output/part8_b.tga";
		setBlue(inputImages[3], inputImages[3].getHeader(), outputImages, outPathing);
		Header header3 = outputImages[11].getHeader();
		image = outputImages[11].getImageProps();
		writeFile(outPathing, header2, image);
		test(exampleImages[9], outputImages[11], "10", successfulTests);
	}

	//Part 9
	{
		outPathing = "output/part9.tga";
		combine(inputImages[7], inputImages[8], inputImages[9], inputImages[7].getHeader(), outputImages, outPathing);
		Header header = outputImages[12].getHeader();
		image = outputImages[12].getImageProps();
		writeFile(outPathing, header, image);

		test(exampleImages[10], outputImages[12], "11", successfulTests);
	}

	//Part 10
	{
		outPathing = "output/part10.tga";
		flip(inputImages[10], inputImages[10].getHeader(), outputImages, outPathing);
		Header header = outputImages[13].getHeader();
		image = outputImages[13].getImageProps();
		writeFile(outPathing, header, image);

		test(exampleImages[11], outputImages[13], "12", successfulTests);
	}

	cout << "Test results: " << successfulTests.size() << " / 12" << endl;
}