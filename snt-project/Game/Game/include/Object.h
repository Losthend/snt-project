#ifndef __Object_h_
#define __Object_h_

//Clase para creacion de objectos estaticos (paredes, suelo, etc)
class Object 
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor
    Object(std::string id, int objType);
    virtual ~Object(void);
	bool update(Ogre::Vector3 vDistance);
	void gravity();
	
	//Velocidad del juego (duracion de cada frame)
	double FPS;

	//Nombre del nodo y de la entidad
	std::string m_entName;
	std::string m_nodeName;

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* m_entity;
	Ogre::SceneNode* m_node;	

	//Tipo de objeto
	int m_objType; 

	//Vector de movimiento/direccion
	Ogre::Vector3 m_direction;

	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	double m_gravity;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Object_h_

//---------------------------------------------------------------------------