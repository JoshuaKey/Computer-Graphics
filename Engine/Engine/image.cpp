#include "stdafx.h"
#include "image.h"

#include <fstream>

namespace Image
{
	const unsigned char* LoadBMP(const char* filePath, unsigned int& width, unsigned int& height,
		unsigned int& bitsPerColor)
	{
		char header[54];

		int compressionType;
		int imageSize;

		unsigned char* pixelData = nullptr;

		std::ifstream file(filePath, std::fstream::binary);
		if (file.is_open())
		{
			file.read(header, 54);
			if(header[0] != 'B' || header[1] != 'M') { return nullptr; } // Invalid file
			
			int pixelDataStart = header[10] + (header[11] << 8) + (header[12] << 16) + (header[13] << 24);
			width = header[18] + (header[19] << 8) + (header[20] << 16) + (header[21] << 24);
			height = header[22] + (header[23] << 8) + (header[24] << 16) + (header[25] << 24);
			bitsPerColor = header[28] + (header[29] << 8);
			compressionType = header[30] + (header[31] << 8) + (header[32] << 16) + (header[33] << 24);

			//imageSize = header[34] + (header[35] << 8) + (header[36] << 16) + (header[37] << 24);
			imageSize = ((width * bitsPerColor) / 32) * 4 * height;

			file.seekg(pixelDataStart);

			pixelData = new unsigned char[imageSize + 1]{'\0'};
			file.read(reinterpret_cast<char*>(pixelData), imageSize);

			int bytesPerColor = bitsPerColor / 8;
			for (int i = 0; i < imageSize; i += bytesPerColor)
			{
				auto temp = pixelData[i];
				pixelData[i] = pixelData[i + 2];
				pixelData[i + 2] = temp;
			}
			
		} else
		{
			std::cerr << "File at: " << filePath << "\nCould not be opened." << std::endl;
		}
		file.close();

		return pixelData;
	}
}