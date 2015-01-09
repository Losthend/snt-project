//Definicion de metodos de deteccion de colisiones

void collisionManagementX(Object* obj, Ogre::SceneNode* player){
	 int objType = obj->m_objType;
	 switch ( objType )
      {
		 //en caso de ser del tipo 3, es decir, trampa de pinchos en X
         case 3:
			//reseteamos el personaje al punto inicial			 			
			//preferiria usar estaforma, pero como da problemas con la camara por ahora, usaremos la manual)
			//node->setPosition(node->getInitialPosition());			
			player->setPosition(0.0, 50.0, 0.0);
			gCamera->setPosition(0.0, 50.0, 450.0);
            break;
	 }
}

void collisionManagementY(Object* obj, Ogre::SceneNode* player){
	 int objType = obj->m_objType;
	 switch ( objType )
      {
		  
		 //en caso de ser del tipo 3, es decir, trampa de pinchos en X
         case 3:
			 //eliminamos la entidad del nodo
			 obj->m_node->detachObject(obj->m_entity);
			 //buscamos el nodo en el vector de objetos y lo borramos
			 for(int i = 0; i < vObjects.size(); i++)
				if(vObjects[i] == obj)
					vObjects.erase(vObjects.begin()+i);
            break;
	 }
}