#ifndef DEMO_H
#define DEMO_H

#include "game.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include "model.hpp"

#include <bullet/btBulletDynamicsCommon.h>


class Demo : public Game {
	public:
		Demo();
		virtual ~Demo();
	protected:
		virtual void update();
		virtual void draw();
	private:
		void initGraphics();
		void initPhysics();

		Texture t;
        ShaderProgram p;
        //Buffer inds;
        //VAO vao;
        Model m;
        FPCamera c;
        
        glm::vec3 boxPosition;

        btBroadphaseInterface* broadphase;

        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;

        btSequentialImpulseConstraintSolver* solver;

        btDiscreteDynamicsWorld* dynamicsWorld;

        btCollisionShape* groundShape;
        btCollisionShape* fallShape;

        btDefaultMotionState* groundMotionState;
        btRigidBody* groundRigidBody;

		btDefaultMotionState* fallMotionState;
        btRigidBody* fallRigidBody;
};


#endif