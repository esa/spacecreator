#ifndef INCONSISTENT_MODEL_EXCEPTION
#define INCONSISTENT_MODEL_EXCEPTION

#include <exception>

class InconsistentModelException : public std::exception
{
public:
	const char * what() const throw()
    {
    	return "Inconsistent Model Exception";
    }
};

#endif
