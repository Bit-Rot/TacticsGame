#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "TacticsErrors.h"
#include "SyncUtil.h"
#include "Landscape.h"
#include "EMRequestHandler.h"
#include <set>

typedef std::set<Entity*> typeEntitySet;
typedef typeEntitySet::iterator typeEntityIterator;

/**
 *	The <pre>EntityManager</pre> is in charge of keeping track of existing entities and adding, updating, and removing
 *	entities from play.  The <pre>EntityManager</pre> provides a series of 'request' methods which correspond to private
 *	functionality of the <pre>EntityManager</pre> class.  Each of these function passes the request to the
 *	<pre>EntityManager</pre>'s <pre>EMRequestHandler</pre> to either pass the request along the network or to complete the
 *	request altogether.  See <pre>EMRequestHandler</pre> for more details.
 */
class EntityManager
{
public:
	static EntityManager* getInstance();
	static void destroyInstance();

	//Public methods
	void update(float dt);
	ReturnCode getEntityOnGridPosition(int iX, int iY, std::set<Entity*> &pobjEntitySet);

	//Model-Modification Request Methods.
	virtual ReturnCode requestAddEntity(Entity* pobjEntity);
	virtual ReturnCode requestRemoveEntity(Entity* pobjEntity);
	virtual ReturnCode requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY);

	//Getters/Setters
	void setLandscape(Landscape* pobjLandscape){m_pobjLandscape = pobjLandscape;}
	void setRequestHandler(EMRequestHandler* pobjRequestHandler){m_pobjRequestHandler = pobjRequestHandler;}
	void setMessageHandler(EMRequestHandler* pobjMessageHandler){m_pobjMessageHandler = pobjMessageHandler;}
	EMRequestHandler* getMessageHandler(){return m_pobjMessageHandler;}

	/**
	 *	@brief Retrieve a pointer to an entity by its unique id.
	 *	
	 *	@param [IN] uiEntityId - the unique id of the entity to retrieve
	 *	@param [OUT] pobjEntity - the pointer to the output entity
	 *	@return RC_OK if successful, RC_ERR_GENERAL if the entity is not found
	 */
	ReturnCode EntityManager::getEntityById(UINT uiEntityId, Entity *& pobjEntity);

protected:
	//Aggregate properties
	Landscape* m_pobjLandscape;

	//Owned properties
	typeEntitySet m_pobjEntitySet;
	CriticalSectionEx CS;
	EMRequestHandler* m_pobjRequestHandler;
	EMRequestHandler* m_pobjMessageHandler;

	//Protected methods
	ReturnCode addEntity(Entity* pobjEntity);
	ReturnCode removeEntity(Entity* pobjEntity);
	ReturnCode changeEntityPosition(Entity* pobjEntity, int iX, int iY);

private:
	//Singleton stuffs
	EntityManager();
	virtual ~EntityManager();
	static EntityManager* m_instance;

//TODO: I don't like this reliance on preprocessor macros, but I don't see any better way of implementing this.
// We have a few goals in our design of the EntityManager - EMRequestHandler relationship.
// 1. We want the client and server to share core EntityManager logic.
// 2. All direct Model-Modification methods should be obscured from outside classes, and thus private
// 3. Client and Server should define different behavior which wraps Model-Modification methods.
// 
// Given this, we attempted to create ClientEntityManager and ServerEntityManager subclasses which would define
// a 'request' interface separately, satisfying all three above requirements.  However, since we implement the 
// singleton design pattern with the EntityManager, we rely on the static getInstance() method to access it from,
// for example, the Entity classes.  Given that the getInstance() method is static, we cannot define it as virtual,
// and we cannot call any other virtual (and thus non-static) member function, and so we have no way to allow the 
// Client and Server entities to access the ClientEntityManager and ServerEntityManager, respectively, without 
// defining separate Client/Server entities, using preprocessor macros in the Client/Server entities anyway, or 
// granting the client project access to server implementations or vice-versa.
// 
// And so we veered toward a RequestHandler implementation which would allow the client and server to define their
// own 'request' functionality (defined in the ClientEMRequesthandler and ServerEMRequestHandler classes, respectively)
// while keeping the core 'Model-Modifying' methods private and shared between client and server.  Since we implement
// request handlers as properties of the EntityManager, however, we must declare them friends of the EntityManager class
// in order to allow them access to the EntityManager's private Model-Modifying functionality.  Since the ClientEMRequestHandler
// is only accessible in the client project, and the ServerEMRequestHandler in the server project, we must use #defines
// to prevent compiler errors.  
// 
// It's a bit gross, yes, but it's the best thing we could think of at the time of implementation.
#if defined (SERVEREMREQUESTHANDLER_H)
	friend class ServerEMRequestHandler;
#else if defined (CLIENTEMREQUESTHANDLER_H)
	friend class ClientEMRequestHandler;
	friend class EMMessageHandler;
#endif

};

#endif //#ifndef ENTITYMANAGER_H