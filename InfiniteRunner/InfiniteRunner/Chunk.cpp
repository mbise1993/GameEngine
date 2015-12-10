#include "Chunk.h"
#include "ChunkSystem.h"
#include "OGL3DObject.h"

#include "LineBox.h"

Chunk::Chunk( ChunkSettings * settings, int XCoord, int ZCoord )
{
	// Set dimension
	this->dimensions = settings->chunkDimensions;

	// Set coord
	this->coordinate.x = XCoord;
	this->coordinate.z = ZCoord;

	// Calculate the center position
	this->calculatePosition();

	// Default occupant to null
	this->occupant = nullptr;
	this->generator = nullptr;
}

Chunk::~Chunk()
{}

void Chunk::regenChunk( int ZCoord )
{
	this->coordinate.z = ZCoord;
	this->calculatePosition();

	if ( this->generator != nullptr )
	{
		generator->populateChunk( this );
	}
}

void Chunk::render()
{
	if ( this->occupant != nullptr )
	{
		this->occupant->render( this->getRenderOrientation() );

		// TODO: Test code for line boxes
		//this->occupant->lineBox->render( this->getRenderOrientation() );
	}
}

void Chunk::setOccupant( OGL3DObject * occupant, float xOffset, float yOffset, float zOffset )
{
	this->occupant = occupant;
	this->occupantOffset.setPosition( xOffset, yOffset, zOffset );
}

void Chunk::setGenerator( ChunkGenerator * generator )
{
	this->generator = generator;
	this->regenChunk( this->coordinate.z );
}

void Chunk::updateXCoord( int XCoord )
{
	this->coordinate.x = XCoord;
	this->calculatePosition();
}

bool Chunk::checkOccupantCollision( OGL3DObject * entity )
{
	if ( this->occupant == nullptr ) return false;

	// Set the occupants bounding box
	this->occupant->boundingBox.referenceFrame.orientation = this->getRenderOrientation();
	this->occupant->boundingBox.applyTransform();

	this->occupant->clearCollisions();
	if ( this->occupant->checkForCollision( entity ) )
	{
		this->occupant = nullptr;
		return true;
	}


	return false;
}

void Chunk::setDebugPlane( OGL3DObject * plane )
{
	this->setOccupant( plane, 0.0f, 0.0f, 0.0f );
}

void Chunk::calculatePosition()
{
	this->chunkFrame.setPosition( this->coordinate.x * this->dimensions, 0, this->coordinate.z * this->dimensions );
}

inline glm::mat4 Chunk::getRenderOrientation()
{
	return this->chunkFrame.orientation * this->occupantOffset.orientation * this->occupant->referenceFrame.orientation;
}
