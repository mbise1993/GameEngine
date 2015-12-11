#include "OGLObjectLoader.h"
#include "GameObjectManager.h"
#include "Room.h"
#include "OGLObjectReader.h"
#include "Lighting/LightSource.h"
#include "Corrider.h"
#include "Animation/RotateYBehavior.h"
#include "Cat.h"
#include "Animation/BackForthBehavior.h"
#include "Animation/PatrolBehavior.h"
#include "Guitarist.h"

OGLObjectLoader::OGLObjectLoader()
{

}

OGLObjectLoader::~OGLObjectLoader()
{

}

void OGLObjectLoader::loadObjects(GameObjectManager* gameObjectManager)
{
	OGLObjectReader* objectReader = new OGLObjectReader();
	OGLObject* oglObject;
	LightSource* lightSource;

	// Corridor connecting rooms
	oglObject = new Corrider("Hallway", 7.5f, 6.0f, 15.0f,
		{ 0.0f, 0.0f, 0.5f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f });
	oglObject->referenceFrame.setPosition(0.0f, 0.0f, 0.0f);
	gameObjectManager->addObject(oglObject->getName(), oglObject);

	//
	// Room 1 and all its objects
	//
	Room* room1 = new Room("Room1", 27.0f, 8.5f, 27.0f, 7.5f, 6.0f,
		{ 0.0f, 0.0f, 0.5f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f });
	room1->referenceFrame.setPosition(0.0f, 0.0f, -21.0f);
	gameObjectManager->addObject(room1->getName(), room1);

	// Table
	if (objectReader->readFile("../../Resources/Objects/Table.obj"))
	{
		oglObject = objectReader->getOGLObject("Table1");
		oglObject->referenceFrame.setPosition(10.5f, 0.0f, -13.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Chair
	if (objectReader->readFile("../../Resources/Objects/Chair.obj"))
	{
		oglObject = objectReader->getOGLObject("Chair1");
		oglObject->referenceFrame.setPosition(9.0f, 0.0f, -13.0f);
		oglObject->referenceFrame.rotateY(180.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Short lamp
	if (objectReader->readFile("../../Resources/Objects/ShortLamp.obj"))
	{
		oglObject = objectReader->getOGLObject("ShortLamp");
		oglObject->referenceFrame.setPosition(10.5f, 4.2f, -11.5f);
		oglObject->referenceFrame.rotateX(-90.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);

		// Light source
		lightSource = new LightSource();
		lightSource->setPosition(0.0f, 3.0f, 0.0f);
		lightSource->setIntensity(0.1f);
		oglObject->setLightSource(lightSource);
	}

	// Tall lamp
	if(objectReader->readFile("../../Resources/Objects/TallLamp.obj"))
	{
		oglObject = objectReader->getOGLObject("TallLamp1");
		oglObject->referenceFrame.setPosition(-11.0f, 1.5f, -32.0f);
		oglObject->referenceFrame.rotateX(-90.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);

		// Light source
		lightSource = new LightSource();
		lightSource->setPosition(0.0f, 6.0f, 0.0f);
		lightSource->setIntensity(0.2f);
		oglObject->setLightSource(lightSource);
	}

	// Bed
	if (objectReader->readFile("../../Resources/Objects/Bed.obj"))
	{
		oglObject = objectReader->getOGLObject("Bed");
		oglObject->referenceFrame.setPosition(-9.0f, -1.5f, -21.0f);
		oglObject->referenceFrame.rotateY(180.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Cat
	if(objectReader->readFile("../../Resources/Objects/Cat.obj"))
	{
		// Get VBOObjects used to create cat
		std::map<std::string, VBOObject*> vboObjects = objectReader->getVBOObjects();

		// Create body
		OGLObject* body = new OGL3DObject("CatBody");
		body->setArrayBufferType();
		body->addVBOObject(vboObjects["CatBody"]);
		// Create head
		OGLObject* head = new OGL3DObject("CatHead");
		head->setArrayBufferType();
		head->addVBOObject(vboObjects["CatHead"]);
		// Create ears
		OGLObject* ears = new OGL3DObject("CatEars");
		ears->setArrayBufferType();
		ears->addVBOObject(vboObjects["CatEars"]);
		// Create tail
		OGLObject* tail = new OGL3DObject("CatTail");
		tail->setArrayBufferType();
		tail->addVBOObject(vboObjects["CatTail"]);
		// Create front left leg
		OGLObject* frontLeftLeg = new OGL3DObject("CatLegFrontLeft");
		frontLeftLeg->setArrayBufferType();
		frontLeftLeg->addVBOObject(vboObjects["CatLegFrontLeft"]);
		// Create front right leg
		OGLObject* frontRightLeg = new OGL3DObject("CatLegFrontRight");
		frontRightLeg->setArrayBufferType();
		frontRightLeg->addVBOObject(vboObjects["CatLegFrontRight"]);
		// Create back left leg
		OGLObject* backLeftLeg = new OGL3DObject("CatLegBackLeft");
		backLeftLeg->setArrayBufferType();
		backLeftLeg->addVBOObject(vboObjects["CatLegBackLeft"]);
		// Create back right leg
		OGLObject* backRightLeg = new OGL3DObject("CatLegBackRight");
		backRightLeg->setArrayBufferType();
		backRightLeg->addVBOObject(vboObjects["CatLegBackRight"]);

		// Create cat
		oglObject = new Cat("Cat", 60.0f, body, head, ears, tail, frontLeftLeg, frontRightLeg, backLeftLeg, backRightLeg);
		oglObject->referenceFrame.setPosition(-4.0f, 1.0f, -26.0f);
		oglObject->setBehavior(new PatrolBehavior(10.0f, 3.0f, 90.0f, 90.0f));
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room1->addOwnedObject(oglObject->getName(), oglObject);
	}

	//
	// Room 2 and all its objects
	//
	Room* room2 = new Room("Room2", 27.0f, 8.5f, 27.0f, 7.5f, 6.0f,
		{ 0.0f, 0.0f, 0.5f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f });
	room2->referenceFrame.setPosition(0.0f, 0.0f, 21.0f);
	room2->referenceFrame.rotateY(180.0f);
	gameObjectManager->addObject(room2->getName(), room2);

	// Tall lamp
	if (objectReader->readFile("../../Resources/Objects/TallLamp.obj"))
	{
		oglObject = objectReader->getOGLObject("TallLamp2");
		oglObject->referenceFrame.setPosition(11.0f, 1.5f, 32.0f);
		oglObject->referenceFrame.rotateX(-90.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);

		// Light source
		lightSource = new LightSource();
		lightSource->setPosition(0.0f, 6.0f, 0.0f);
		lightSource->setIntensity(0.2f);
		lightSource->setState(LIGHT_OFF);
		oglObject->setLightSource(lightSource);
	}

	// Ceiling fan
	if (objectReader->readFile("../../Resources/Objects/CeilingFan.obj"))
	{
		oglObject = objectReader->getOGLObject("CeilingFan");
		oglObject->referenceFrame.setPosition(0.0f, 8.0f, 24.0f);
		oglObject->setBehavior(new RotateYBehavior(360.0f));
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);

		// Light source
		lightSource = new LightSource();
		lightSource->setPosition(0.0f, -1.0f, 0.0f);
		lightSource->setIntensity(0.2f);
		lightSource->setState(LIGHT_OFF);
		oglObject->setLightSource(lightSource);
	}

	// Table
	if (objectReader->readFile("../../Resources/Objects/Table.obj"))
	{
		oglObject = objectReader->getOGLObject("Table2");
		oglObject->referenceFrame.setPosition(10.5f, 0.0f, 21.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Stereo
	if (objectReader->readFile("../../Resources/Objects/Stereo.obj"))
	{
		oglObject = objectReader->getOGLObject("Stereo");
		oglObject->referenceFrame.setPosition(10.5f, 3.0f, 21.0f);
		oglObject->referenceFrame.rotateY(-180.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Chair
	if (objectReader->readFile("../../Resources/Objects/Chair.obj"))
	{
		oglObject = objectReader->getOGLObject("Chair2");
		oglObject->referenceFrame.setPosition(-9.0f, 0.0f, 25.0f);
		oglObject->referenceFrame.rotateY(-135.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Guitarist
	if (objectReader->readFile("../../Resources/Objects/Guitarist.obj"))
	{
		// Get VBOObjects used to create cat
		std::map<std::string, VBOObject*> vboObjects = objectReader->getVBOObjects();

		// Create body
		OGLObject* torso = new OGL3DObject("GuitaristTorso");
		torso->setArrayBufferType();
		torso->addVBOObject(vboObjects["GuitaristTorso"]);
		// Create legs
		OGLObject* legs = new OGL3DObject("GuitaristLegs");
		legs->setArrayBufferType();
		legs->addVBOObject(vboObjects["GuitaristLegs"]);
		// Create head
		OGLObject* head = new OGL3DObject("GuitaristHead");
		head->setArrayBufferType();
		head->addVBOObject(vboObjects["GuitaristHead"]);
		// Create left arm
		OGLObject* leftArm = new OGL3DObject("GuitaristLeftArm");
		leftArm->setArrayBufferType();
		leftArm->addVBOObject(vboObjects["GuitaristLeftArm"]);
		// Create right bicep
		OGLObject* rightBicep = new OGL3DObject("GuitaristRightBicep");
		rightBicep->setArrayBufferType();
		rightBicep->addVBOObject(vboObjects["GuitaristRightBicep"]);
		// Create right forearm
		OGLObject* rightForearm = new OGL3DObject("GuitaristRightForearm");
		rightForearm->setArrayBufferType();
		rightForearm->addVBOObject(vboObjects["GuitaristRightForearm"]);

		// Create guitarist
		oglObject = new Guitarist("Guitarist", 40.0f, torso, legs, head, leftArm, rightBicep, rightForearm);
		oglObject->referenceFrame.setPosition(-9.0f, 2.2f, 25.0f);
		oglObject->referenceFrame.rotateY(135.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);
	}

	// Guitar
	if (objectReader->readFile("../../Resources/Objects/Guitar.obj"))
	{
		oglObject = objectReader->getOGLObject("Guitar");
		oglObject->referenceFrame.setPosition(-8.0f, 3.2f, 24.8f);
		oglObject->referenceFrame.rotateY(135.0f);
		oglObject->referenceFrame.rotateZ(20.0f);
		gameObjectManager->addObject(oglObject->getName(), oglObject);
		room2->addOwnedObject(oglObject->getName(), oglObject);
	}
}