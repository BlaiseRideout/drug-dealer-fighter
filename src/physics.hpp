#ifndef MASS_H
#define MASS_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Body;
class World;

class Mass {
	public:
		Mass(std::shared_ptr<Body> b, glm::vec2 pos);
		Mass(std::shared_ptr<Body> b, glm::vec2 pos, bool isHand);

		void update();

		std::shared_ptr<Body> b;
		glm::vec2 pos;
		glm::vec2 ppos;
		bool isHand;
};

class Link {
	public:
		Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, bool collidable);
		Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, float strength, bool collidable);
		Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, float strength, float length, bool collidable);


		void update();
		void correct(std::shared_ptr<Link> l);

		std::shared_ptr<Mass> mass1;
		std::shared_ptr<Mass> mass2;
		float strength;
		float length;
		bool collidable;
};

class Body {
	public:
		Body(World const &w, std::function<void()> onCollide);
		Body(World const &w);

		void update();
		void correct(std::shared_ptr<Body> b);

		std::vector<std::shared_ptr<Mass>> masses;
		std::vector<std::shared_ptr<Link>> links;
		World const &w;
		std::function<void()> onCollide;

};

class World {
	public:
		World(float g);

		void addBody(std::shared_ptr<Body> b);
		void update();

		float g;
		static float width;
		static float height;
		std::vector<std::shared_ptr<Body>> bodies;
};

#endif