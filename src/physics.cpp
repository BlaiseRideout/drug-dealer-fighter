#include "physics.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

inline float cross(glm::vec2 v1, glm::vec2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Mass::Mass(std::shared_ptr<Body> b, glm::vec2 pos) : Mass(b, pos, false) {
}

Mass::Mass(std::shared_ptr<Body> b, glm::vec2 pos, bool isHand) : b(b), pos(pos), ppos(pos), isHand(isHand) {
}

void Mass::update() {
	glm::vec2 vel = this->pos - this->ppos;
	vel.x *= 0.95;
	vel.y *= 0.95;

	this->ppos = pos;
	this->pos += vel;

	this->pos.y += this->b->w.g;
}

Link::Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, bool collidable) : Link(mass1, mass2, 1.0f, collidable) {
}

Link::Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, float strength, bool collidable) : Link(mass1, mass2, strength, abs(glm::length(mass1->pos - mass2->pos)), collidable) {
}

Link::Link(std::shared_ptr<Mass> mass1, std::shared_ptr<Mass> mass2, float strength, float length, bool collidable) : mass1(mass1), mass2(mass2), strength(strength), length(length), collidable(collidable) {
}

void Link::update() {
	glm::vec2 line = this->mass1->pos - this->mass2->pos;
	float offset = length - glm::length(line);
	line = glm::normalize(line);
	line.x *= offset * strength * .5;
	line.y *= offset * strength * .5;

	this->mass1->pos += line;
	this->mass2->pos -= line;
}

void Link::correct(std::shared_ptr<Link> l) {
	/*std::shared_ptr<Mass> hand;
	std::shared_ptr<Mass> notHand;
	bool isL = false;
	if(this->mass1->isHand) {
		hand = this->mass1;
		notHand = this->mass2;
	}
	else if(this->mass2->isHand) {
		hand = this->mass2;
		notHand = this->mass1;
	}
	else if(l->mass1->isHand) {
		hand = l->mass1;
		notHand = l->mass2;
		isL = true;
	}
	else if(l->mass2->isHand) {
		hand = l->mass2;
		notHand = l->mass1;
		isL = true;
	}
	else
		return; */

	glm::vec2 p = this->mass1->pos;
	glm::vec2 r = this->mass2->pos - p;

	glm::vec2 q = l->mass1->pos;
	glm::vec2 s = l->mass2->pos - q;

	if(abs(cross(r, s)) < 0.001f) {
		// the two lines are parallel
		/*if(abs(cross(q - p, r)) < 0.001f) {
			// the two lines are collinear
			float n = glm::dot(q - p, r);
			float m = glm::dot(p - q, s);
			if((0.001f <= n && n <= glm::dot(r, r)) || (0.001f <= m && m <= glm::dot(s, s))) {
				// the two lines are overlapping
			}
			else {
				// the two lines are collinear but disjoint
			}
		} */
	}
	else {
		float u = cross((q - p), r) / (cross(r, s));
		float t = cross((q - p), s) / (cross(r, s));
		if(0 <= t && t <= 1 && 0 <= u && u <= 1) {
			// the lines intersect
			glm::vec2 iPoint = p + t * r;
			float scale = 1.0f;

			std::vector<float> lengths({glm::length(iPoint - this->mass1->pos),
				glm::length(iPoint - this->mass2->pos),
				glm::length(iPoint - l->mass1->pos),
				glm::length(iPoint - l->mass2->pos)
			});

			int min = std::min_element(lengths.begin(), lengths.end()) - lengths.begin();

			if(min == 0) {
				if(this->mass1->isHand) {
					if(glm::length(this->mass1->pos - this->mass1->ppos) > 7.0f && l->mass1->b->onCollide) {
						l->mass1->b->onCollide();
						scale = 6.0f;
					}
				}

				l->mass1->pos += glm::normalize(this->mass1->pos - this->mass1->ppos) * scale;
				l->mass2->pos += glm::normalize(this->mass1->pos - this->mass1->ppos) * scale;

				glm::vec2 off = iPoint - this->mass1->pos;

				this->mass1->ppos = this->mass1->pos;
				this->mass1->pos += off;

				this->mass2->ppos = this->mass2->pos;
				this->mass2->pos += off;
			}
			else if(min == 1) {
				if(this->mass2->isHand) {
					if(glm::length(this->mass2->pos - this->mass2->ppos) > 7.0f && l->mass1->b->onCollide) {
						l->mass1->b->onCollide();
						scale = 6.0f;
					}
				}

				l->mass1->pos += glm::normalize(this->mass2->pos - this->mass2->ppos) * scale;
				l->mass2->pos += glm::normalize(this->mass2->pos - this->mass2->ppos) * scale;

				glm::vec2 off = iPoint - this->mass2->pos;

				this->mass1->ppos = this->mass1->pos;
				this->mass1->pos += off;

				this->mass2->ppos = this->mass2->pos;
				this->mass2->pos += off;
			}
			else if(min == 2) {
				if(l->mass1->isHand) {
					if(glm::length(l->mass1->pos - l->mass1->ppos) > 7.0f && this->mass1->b->onCollide) {
						this->mass1->b->onCollide();
						scale = 6.0f;
					}
				}

				this->mass1->pos += glm::normalize(l->mass1->pos - l->mass1->ppos) * scale;
				this->mass2->pos += glm::normalize(l->mass1->pos - l->mass1->ppos) * scale;
				
				glm::vec2 off =  iPoint - l->mass1->pos;

				l->mass1->ppos = l->mass1->pos;
				l->mass1->pos += off;

				l->mass2->ppos = l->mass2->pos;
				l->mass2->pos += off;
			}
			else if(min == 3) {
				if(l->mass2->isHand) {
					if(glm::length(l->mass2->pos - l->mass2->ppos) > 7.0f && this->mass1->b->onCollide) {
						this->mass1->b->onCollide();
						scale = 6.0f;
					}
				}

				this->mass1->pos += glm::normalize(l->mass1->pos - l->mass2->ppos) * scale;
				this->mass2->pos += glm::normalize(l->mass1->pos - l->mass2->ppos) * scale;
				
				glm::vec2 off = iPoint - l->mass2->pos;

				l->mass1->ppos = l->mass1->pos;
				l->mass1->pos += off;

				l->mass2->ppos = l->mass2->pos;
				l->mass2->pos += off;
			}


			/*if(isL) {
				if(glm::length(hand->pos - hand->ppos) > 7.0f && this->mass1->b->onCollide) {
					this->mass1->b->onCollide();
					scale = 6.0f;
				}

				this->mass1->pos += glm::normalize(hand->pos - notHand->pos) * scale;
				this->mass2->pos += glm::normalize(hand->pos - notHand->pos) * scale;
			}
			else {
				if(glm::length(hand->pos - hand->ppos) > 7.0f && l->mass1->b->onCollide) {
					l->mass1->b->onCollide();
					scale = 6.0f;
				}

				l->mass1->pos += glm::normalize(hand->pos - notHand->pos) * scale;
				l->mass2->pos += glm::normalize(hand->pos - notHand->pos) * scale;
			}

			hand->ppos = hand->pos;
			hand->pos = iPoint; */

		}
	}
}

Body::Body(World const &w) : w(w) {
}

void Body::update() {
	for(auto i = masses.begin(); i != masses.end(); ++i) {
		(*i)->update();

		glm::vec2 &pos((*i)->pos);

		if(pos.x > World::width)
			pos.x = World::width;
		else if(pos.x < 0.0f)
			pos.x = 0.0f;

		if(pos.y > World::height)
			pos.y = World::height;
		else if(pos.y < 0.0f)
			pos.y = 0.0f;
	}
		
	for(auto i = links.begin(); i != links.end(); ++i) {
		(*i)->update();
		(*i)->update(); // multiple phase solver
	}
}

void Body::correct(std::shared_ptr<Body> body) {
	for(auto i = this->links.begin(); i != this->links.end(); ++i) {
		if((*i)->collidable) {
			for(auto j = body->links.begin(); j != body->links.end(); ++j) {
				if((*j)->collidable)
					(*i)->correct(*j);
			}
		}
	}
}

World::World(float g) : g(g) {
}

void World::addBody(std::shared_ptr<Body> b) {
	bodies.push_back(b);
}

void World::update() {
	auto start = this->bodies.begin() + 1;
	for(auto i = this->bodies.begin(); i != this->bodies.end(); ++i) {
		for(auto j = start; j != this->bodies.end(); ++j) {
			(*i)->correct(*j);
		}
		(*i)->update();
		start++;
	}
}

float World::width = 1000.0f;
float World::height = World::width * (600.0f / 800.0f);