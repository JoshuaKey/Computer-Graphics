
#include "stdafx.h"

#include "meshLoader.h"
#include "mesh.h"

#include <sstream>
#include <iostream>

#include <Windows.h>

using namespace glm;

namespace {
	std::vector<glm::vec3> posValues;
	std::vector<glm::vec2> uvValues;
	std::vector<glm::vec3> normValues;

	void parseVertex(char* line);
	void parseNormal(char* line);
	void parseTexture(char* line);
	void parseFace(char* line,
		std::vector<glm::vec3>& pos,
		std::vector<glm::vec2>& uv,
		std::vector<glm::vec3>& norm);

	// Returns a pointer to the next " " in this string
	char* getNextValue(char* curr) {
		auto length = strcspn(curr, " ");
		return (curr + length);
	}

	int parseBuffer(char* buffer,
		std::vector<glm::vec3>& pos,
		std::vector<glm::vec2>& uv,
		std::vector<glm::vec3>& norm) 
	{

		char *currStr = buffer, *endOfLine;

		while (currStr != nullptr) {
			// Get line
			int index = strcspn(currStr, "\n");

			// Check if line is partial
			if (!currStr[index]) { 
				return index; 
			}
			else { endOfLine = &currStr[index + 1]; }

			// check specific data
			if (!strncmp(currStr, "v ", 2u)) {
				parseVertex(currStr+2);
			} else if (!strncmp(currStr, "vn ", 3u)) {
				parseNormal(currStr+2);
			} else if (!strncmp(currStr, "vt ", 3u)) {
				parseTexture(currStr+2);
			} else if (!strncmp(currStr, "f ", 2u)) { // Face
				parseFace(currStr+2, pos, uv, norm);
			}
			currStr = endOfLine; // Beginning of next line
		}
		return 0;
	}
	void parseVertex(char* line) 
	{
		float x, y, z;

		x = strtof(line, &line);

		y = strtof(line, &line);

		z = strtof(line, nullptr);

		vec3 vertex(x, y, z);
		posValues.push_back(vertex);
	}
	void parseNormal(char* line) 
	{
		float x, y, z;

		x = strtof(line, &line);

		y = strtof(line, &line);

		z = strtof(line, nullptr);

		vec3 vertex(x, y, z);
		normValues.push_back(vertex);
	}
	void parseTexture(char* line) 
	{
		float x, y;

		x = strtof(line, &line);

		y = strtof(line, &line);

		vec2 vertex(x, y);
		uvValues.push_back(vertex);
	}

	void parseFace(char* line,
		std::vector<glm::vec3>& pos,
		std::vector<glm::vec2>& uv,
		std::vector<glm::vec3>& norm)
	{
		for (int i = 0; i < 3; i++) {

			long posInd = strtol(line, &line, 10)-1;
			pos.push_back(posValues[posInd]);
			if (*line == '/') {
				++line;
				if (*line != '/') {
					long uvInd = strtol(line, &line, 10)-1;
					uv.push_back(uvValues[uvInd]);
				}
				++line;
				long normInd = strtol(line, &line, 10)-1;
				norm.push_back(normValues[normInd]);
			}
		}
	}
}

bool MeshLoader::ParseModel(const wchar_t *filePath, Mesh& mesh)
{

	///*bool val = MeshLoader::ParseModel(filePath, mesh.pos, mesh.uv, mesh.norm);
	//mesh.verticeAmo = mesh.pos.size();
	//return val;*/
	return false;
}

bool MeshLoader::ParseModel(const wchar_t *filePath,
	std::vector<glm::vec3>& pos,
	std::vector<glm::vec2>& uv,
	std::vector<glm::vec3>& norm) 
{
	static const auto BUFFER_SIZE = 16*1024;
	posValues.clear();
	uvValues.clear();
	normValues.clear();

	auto file = CreateFile(filePath, FILE_READ_DATA, 0, NULL, OPEN_EXISTING, 
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (file != INVALID_HANDLE_VALUE) {
		char buffer[BUFFER_SIZE+1];
		buffer[BUFFER_SIZE] = 0;
		DWORD bytes_read;

		int offset = 0;
		int bytesToRead;
		do {
			// Determine how many I need to read
			bytesToRead = BUFFER_SIZE - offset;
			ReadFile(file, buffer + offset, bytesToRead , &bytes_read, NULL);

			// Parse
			offset = parseBuffer(buffer, pos, uv, norm);

			// Adjust buffer to account for partial line
			auto src = buffer + BUFFER_SIZE - offset;
			strcpy_s(buffer, src);
		} while (bytes_read == bytesToRead); // Check if end of file / error

		if (!strncmp(buffer, "v ", 2u)) {
			parseVertex(buffer+2);
		} else if (!strncmp(buffer, "vn ", 3u)) {
			parseNormal(buffer+2);
		} else if (!strncmp(buffer, "vt ", 3u)) {
			parseTexture(buffer+2);
		} else if (!strncmp(buffer, "f ", 2u)) { // Face
			parseFace(buffer+2, pos, uv, norm);
		}

		CloseHandle(file);
		return true;
	} else
	{
		return false;
	}
}

/*

Does not use Indexing!!!!!!!!!

*/
