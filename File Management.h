/*
	File Management.h
	Implements methods for reading and writing text to files
*/

#pragma once

#include <string>
#include <vector>
#include <fstream>


class FileManagement {

public:

	/*
		Returns a list of files which have previously been saved and can be opened

		@return Vector of strings of file names which can be opened
	*/
	static std::vector<std::string> GetList() {
		// get the list from the "saved_files.txt" file
		std::vector<std::string> output;
		std::string line = "";
		std::ifstream inFile;
		inFile.open("saved_files.txt");
		if (inFile.is_open())
		{
			// read the lines of "saved_files.txt" and add the file names to the output vector
			while (getline(inFile, line))
			{
				output.push_back(line);
			}
		}
		if (output.size() == 0) {
			std::ofstream myfile;
			std::string fullName = "saved_files.txt";
			myfile.open(fullName.c_str());
			myfile << "";
			myfile.close();
		}
		return output;
	}


	/*
		Checks if a file name corresponds to a file which can be opened

		@param fileName
		@return True if the given file can be opened
	*/
	static bool CheckExists(std::string fileName) {
		// retrieve the list of openable files
		std::vector<std::string> files = GetList();
		// check the given file name is in the list
		for (int i = 0; i < files.size(); i++) {
			if (files[i] == fileName) {
				return true;
			}
		}
		return false;
	}


	/*
		Get the text content of a file with the given name
	*/
	static std::string ReadFile(std::string fileName) {
		// first make sure we can open this file
		if (!CheckExists(fileName)) {
			return "";
		}
		// now open it
		std::string contents = "";
		std::string line = "";
		std::ifstream inFile;
		inFile.open((fileName + ".dti").c_str());
		if (inFile.is_open())
		{
			// read the contents line by line and add them to the output string
			while (getline(inFile, line))
			{
				if (contents != "") {
					contents += "\n";
				}
				contents += line;
			}
		}
		return contents;
	}


	/*
		Writes text to the file name provided

		@param fileName - The file name to write to
		@param content - The text to write
	*/
	static void WriteFile(std::string fileName, std::string content) {
		if (!CheckExists(fileName)) {
			// update the "saved_files.txt" adding this file name to the list if it's not already there
			std::vector<std::string> files = GetList();
			files.push_back(fileName);
			std::string fileString = "";
			for (int i = 0; i < files.size(); i++) {
				if (fileString != "") {
					fileString += "\n";
				}
				fileString += files[i];
			}
			std::ofstream fileListOutput;
			std::string name = "saved_files.txt";
			fileListOutput.open(name.c_str());
			fileListOutput << fileString;
			fileListOutput.close();
		}
		// write the content to the file itself
		std::ofstream myfile;
		std::string fullName = fileName + ".dti";
		myfile.open(fullName.c_str());
		myfile << content;
		myfile.close();
	}


	/*
		Deletes the file with the given name

		@param fileName - The file name of the file to delete
	*/
	static void DeleteFile(std::string fileName) {
		if (CheckExists(fileName)) {
			// if the file exists, remove it from "saved_files.txt"
			WriteFile(fileName, "");
			std::vector<std::string> list = GetList();
			std::string listText = "";
			for (int i = 0; i < list.size(); i++) {
				if (list[i] != fileName) {
					if (listText != "") {
						listText += "\n";
					}
					listText += list[i];
				}
			}
			// write empty string "" to the file
			// this does not delete it but it is the best we can do to remain crossplatform using no additional libraries
			std::ofstream fileListOutput;
			std::string name = "saved_files.txt";
			fileListOutput.open(name.c_str());
			fileListOutput << listText;
			fileListOutput.close();
		}
	}

};