#pragma once
#ifndef BASE_OBJECT
#define BASE_OBJECT

#include "Publisher.h"
#include <string>

class Logger;

class BaseObject : public Publisher
{
protected:
	Logger* logger;

	std::string name;

public:
	BaseObject( Logger* logger = nullptr );

	virtual ~BaseObject();

	virtual void setLogger( Logger* logger ) { this->logger = logger; }

	virtual Logger* getLogger() { return this->logger; }

	virtual void setName( const std::string& name ) { this->name = name; }

	virtual const std::string& getName()  const { return this->name; }
};

#endif

