#pragma once
#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <fstream>
#include <sstream>
#include <codecvt>

//
// Base class to open and read configuration files of the format attribute-name: value
//
class ConfigReader
{
	private:
		std::ifstream _reader;	

		template<class T>
		T readAttributeImpl(std::string name, std::true_type);
		template<class T>
		T readAttributeImpl(std::string name, std::false_type);
	protected:
		bool openFile(std::string fileName);
		template<class T>
		T readAttribute(std::string name);
	public:
		ConfigReader() { }
		~ConfigReader() { }
};

//
// Accepts the type of the attribute value as the template parameter and the name of the
// attribute to be read as the function parameter. Calls the actual function implementation
// based on whether the template parameter is a string or an integral type.
//
template<class T>
T ConfigReader::readAttribute(std::string name)
{
	return readAttributeImpl<T>(name, std::is_integral<T>());
}

//
// Implementation of readAttribute function for integral types. Searches configuration file
// for an attribute with the specified name, reads in the value, converts it to the type
// specified, and returns the converted value.
//
template<class T>
T ConfigReader::readAttributeImpl(std::string name, std::true_type)
{
	// If no attribute name is passed in, return NULL
	if (!name.empty())
	{
		std::string currentLine;

		// Read next line in config file
		while (getline(ConfigReader::_reader, currentLine))
		{
			int delimiterPos = currentLine.find(':');	
			std::string attributeName = currentLine.substr(0, delimiterPos);

			// If attribute name of current line = specified attribute name...
			if (attributeName == name)
			{
				std::string attributeValue = currentLine.substr(delimiterPos + 2);	// Parse for attribute value

				// Convert to specified type and return
				std::istringstream converter(attributeValue);
				T convertedValue;
				converter >> convertedValue;

				return convertedValue;
			}
		}
	}

	return NULL;
}

//
// Implementation of readAttribute function for strings. Searches configuration file for
// an attribute with the specified name, reads in the value, and returns it as a string.
//
template<class T>
T ConfigReader::readAttributeImpl(std::string name, std::false_type)
{
	// If no attribute name is passed in, return NULL
	if (!name.empty())
	{
		std::string currentLine;

		// Read next line in config file
		while (getline(ConfigReader::_reader, currentLine))
		{
			int delimiterPos = currentLine.find(':');
			std::string attributeName = currentLine.substr(0, delimiterPos);

			// If attribute name of current line = specified attribute name...
			if (attributeName == name)
			{
				std::string lineAsString = currentLine.substr(delimiterPos + 2);	// Parse for attribute value and return		

				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.from_bytes(lineAsString);
			}
		}
	}

	return NULL;
}

#endif