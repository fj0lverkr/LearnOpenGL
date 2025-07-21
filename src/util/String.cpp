#include "util/Text.hpp"

char *CharUtil::concat(const char *first, const char *second)
{
	std::cout << "Concatenating '" << first << "' to '" << second << std::endl;
	const int size = sizeof(first) + sizeof(second);
	char *result;
	result = new char[size];
	strcpy(result, first);
	strcat(result, second);
	return result;
}