#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>

Player::Player(World &w, bool id) : w(w), body(new Body(w)), controller(id?1:0), textureShader(VertexShader("res/texture.vert"), FragmentShader("res/texture.frag")), pointShader(VertexShader("res/color.vert"), FragmentShader("res/color.frag")), alive(true), health(100.0f), id(id), leftStick(60, glm::vec2(0.0f)), rightStick(60, glm::vec2(0.0f)), curInput(0), /*color(glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f)) */ color(colors[rand() % 3]) {
	if(id)
		this->bodyTex = Texture("res/body.png");
	else
		this->bodyTex = Texture("res/body2.png");

	this->body->onCollide = std::bind(&Player::hurt, this);

	this->faceTex = Texture("res/face.png");
	this->armTex = Texture("res/arm.png");
	this->legTex = Texture("res/leg.png");
	this->barTex = Texture("res/bar.png");
	this->healthTex = Texture("res/health.png");
	this->blingTex = Texture("res/bling.png");
	this->backgroundTex = Texture("res/background.jpg");

	initPhysics(200.0f + id * 400.0f, 500.0f);
	initGraphics();
}

void Player::initPhysics(float x, float y) {
	////////////
	// MASSES //
	////////////

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -30.0f, y + 75.0f)))); // LL 0
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 30.0f, y + 75.0f))));  // LR 1
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 50.0f, y + -75.0f)))); // UR 2
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -50.0f, y + -75.0f))));// UL 3
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 0.0f, y + -100.0f)))); // neck 4

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -100.0f, y + -75.0f)))); // left elbow 5
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -150.0f, y + -75.0f), true))); // left hand 6

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 100.0f, y + -75.0f))));  // right elbow 7
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 150.0f, y + -75.0f), true)));  // right hand 8

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -50.0f, y + 125.0f)))); // left knee 9
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -50.0f, y + 175.0f)))); // left foot 10

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 50.0f, y + 125.0f))));  // right knee 11
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 50.0f, y + 175.0f))));  // right foot 12

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -20.0f, y + -120.0f)))); // head LL 13
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 20.0f, y + -120.0f))));  // head LR 14
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 21.0f, y + -150.0f)))); // head UR 15
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -21.0f, y + -150.0f))));// head UL 16

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x, y + -30.0f)))); // 17 bling
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x, y + -10.0f)))); // 18 bling2

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -25.0f, y + -70.0f))));// 19 chain left
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -17.0f, y + -50.0f))));// 20 chain left2
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + -10.0f, y + -40.0f))));// 21 chain left3

	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 25.0f, y + -70.0f))));// 22 chain right
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 17.0f, y + -50.0f))));// 23 chain right2
	this->body->masses.push_back(std::shared_ptr<Mass>(new Mass(this->body, glm::vec2(x + 10.0f, y + -40.0f))));// 24 chain right3




	///////////
	// LINKS //
	///////////

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[13], false))); // neck - head LL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[14], false))); // neck - head LR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[15], false))); // neck - head UR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[16], false))); // neck - head UL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[14], this->body->masses[15], false))); // head LR - head UR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[15], this->body->masses[16], false))); // head LL - head UR

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[0], this->body->masses[1], true))); // LL - LR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[1], this->body->masses[2], true))); // LR - UR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[2], this->body->masses[4], true))); // UR - neck
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[3], true))); // neck - UL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[3], this->body->masses[0], true))); // UL - LL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[3], this->body->masses[2], true))); // UL - UR

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[0], false))); // neck - LL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[1], false))); // neck - LR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[0], this->body->masses[2], false))); // LL - UR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[1], this->body->masses[3], false))); // LR - UL

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[13], this->body->masses[14], true))); // head LL - head LR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[15], this->body->masses[14], true))); // head LR - head UR
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[15], this->body->masses[16], true))); // head UR - head UL
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[13], this->body->masses[16], true))); // head UL - head LL

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[3], this->body->masses[5], true))); // left upper arm
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[5], this->body->masses[6], true))); // left lower arm

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[2], this->body->masses[7], true))); // right upper arm
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[7], this->body->masses[8], true))); // right lower arm

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[0], this->body->masses[9], false))); // left thigh
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[9], this->body->masses[10], false))); // left calf

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[1], this->body->masses[11], false))); // right thigh
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[11], this->body->masses[12], false))); // right calf

	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[3], this->body->masses[19], 1.0f, 15.0f, false))); // UL - chain left
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[19], 1.0f, 15.0f, false))); // neck - chain left
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[2], this->body->masses[22], 1.0f, 15.0f, false))); // UR - chain raight
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[4], this->body->masses[22], 1.0f, 15.0f, false))); // neck - chain right
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[20], this->body->masses[19], false))); // chain left2 - chain left
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[20], this->body->masses[21], false))); // chain left2 - chain left3
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[23], this->body->masses[22], false))); // chain right2 - chain right
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[23], this->body->masses[24], false))); // chain right2 - chain right3
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[17], this->body->masses[24], false))); // bling - chain right3
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[17], this->body->masses[21], false))); // bling - chain left3
	this->body->links.push_back(std::shared_ptr<Link>(new Link(this->body->masses[17], this->body->masses[18], false))); // bling - bling 2



	this->w.addBody(this->body);
}

void Player::initGraphics() {
	std::vector<glm::vec2> temp_vertices;
	temp_vertices.push_back(glm::vec2(-0.5f, -0.5f));
	temp_vertices.push_back(glm::vec2(0.5f, -0.5f));
	temp_vertices.push_back(glm::vec2(0.5f, 0.5f));
	temp_vertices.push_back(glm::vec2(-0.5f, 0.5f));
	texVao.setAttrib(this->textureShader, "vertexPosition", Buffer(temp_vertices), 2);

	std::vector<glm::vec2> temp_uvs;
	temp_uvs.push_back(glm::vec2(0.0f, 1.0f));
	temp_uvs.push_back(glm::vec2(1.0f, 1.0f));
	temp_uvs.push_back(glm::vec2(1.0f, 0.0f));
	temp_uvs.push_back(glm::vec2(0.0f, 0.0f));
	texVao.setAttrib(this->textureShader, "vertexUV", Buffer(temp_uvs), 2);

	std::vector<GLuint> temp_triangles;
	temp_triangles.push_back(2);
	temp_triangles.push_back(1);
	temp_triangles.push_back(0);
	temp_triangles.push_back(0);
	temp_triangles.push_back(3);
	temp_triangles.push_back(2);
	this->texIndices.loadData(temp_triangles);

	this->textureShader["M"] = glm::mat4(1.0f);
	this->textureShader["P"] = glm::ortho(0.0f, (float)World::width, (float)World::height, 0.0f, -0.1f, 10.0f);
	this->textureShader["color"] = glm::vec3(1.0f, 1.0f, 1.0f);

	this->pointShader["P"] = glm::ortho(0.0f, (float)World::width, (float)World::height, 0.0f, -0.1f, 10.0f);

  	this->pointShader["color"] = glm::vec4(216.0f / 255.0f, 184.0f / 255.0f, 36.0f / 255.0f, 1.0f);
	std::vector<glm::vec2> lines_data;
	for(auto link = this->body->links.begin(); link != this->body->links.end(); ++link) {
		lines_data.push_back((*link)->mass1->pos);
		lines_data.push_back((*link)->mass2->pos);
	}

	this->links.loadData(lines_data);
	this->vao.setAttrib(this->pointShader, "vertexPosition", this->links, 2);
}

void Player::draw() {
	this->textureShader.use();
	this->texVao.bind();

	if(!this->id) {
		this->textureShader["tex"] = this->backgroundTex;
		this->textureShader["M"] = glm::scale(glm::mat4(1.0f), glm::vec3(World::width, World::height, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

		this->texIndices.drawElements();
	}

	this->textureShader["color"] = this->color;

	glm::vec2 lshoulder = this->body->masses[3]->pos;
	glm::vec2 lelbow = this->body->masses[5]->pos;
	glm::vec2 lhand = this->body->masses[6]->pos;

	glm::vec2 an = lelbow - lshoulder;
	float angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->armTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(lshoulder.x, lshoulder.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(60.0f, 30.0f, 1.0f));

	this->texIndices.drawElements();

	an = lhand - lelbow;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->armTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(lelbow.x, lelbow.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.0f, 30.0f, 1.0f));

	this->texIndices.drawElements();

	glm::vec2 rshoulder = this->body->masses[2]->pos;
	glm::vec2 relbow = this->body->masses[7]->pos;
	glm::vec2 rhand = this->body->masses[8]->pos;

	an = relbow - rshoulder;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->armTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(rshoulder.x, rshoulder.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(60.0f, 30.0f, 1.0f));

	this->texIndices.drawElements();

	an = rhand - relbow;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->armTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(relbow.x, relbow.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.0f, 30.0f, 1.0f));

	this->texIndices.drawElements();	


	this->textureShader["color"] = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec2 rhip = this->body->masses[1]->pos;
	glm::vec2 rknee = this->body->masses[11]->pos;
	glm::vec2 rfoot = this->body->masses[12]->pos;

	an = rknee - rhip;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->legTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(rhip.x, rhip.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.0f, 20.0f, 1.0f));

	this->texIndices.drawElements();

	an = rfoot - rknee;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->legTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(rknee.x, rknee.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(80.0f, 15.0f, 1.0f));

	this->texIndices.drawElements();

	glm::vec2 lhip = this->body->masses[0]->pos;
	glm::vec2 lknee = this->body->masses[9]->pos;
	glm::vec2 lfoot = this->body->masses[10]->pos;

	an = lknee - lhip;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->legTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(lhip.x, lhip.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.0f, 20.0f, 1.0f));

	this->texIndices.drawElements();

	an = lfoot - lknee;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 180;

	this->textureShader["tex"] = this->legTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(lknee.x, lknee.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(80.0f, 15.0f, 1.0f));

	this->texIndices.drawElements();


	glm::vec2 bodyPos = this->body->masses[0]->pos;

	for(int i = 1; i < 5; ++i) {
		bodyPos += this->body->masses[i]->pos;
	}

	bodyPos /= 5.0f;

	an = bodyPos - this->body->masses[4]->pos;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 - 90;

	this->textureShader["tex"] = this->bodyTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(bodyPos.x, bodyPos.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(130.0f, 190.0f, 1.0f));

	this->texIndices.drawElements();


	glm::vec2 bling1 = this->body->masses[17]->pos;
	glm::vec2 bling2 = this->body->masses[18]->pos;

	an = bling2 - bling1;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 - 90;

	this->textureShader["tex"] = this->blingTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(bling1.x, bling1.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 80.0f, 1.0f));

	this->texIndices.drawElements();


	bodyPos = this->body->masses[13]->pos;
	for(int i = 14; i <= 16; ++i)
		bodyPos += this->body->masses[i]->pos;

	bodyPos /= 4;

	an = bodyPos - this->body->masses[4]->pos;
	angle = std::atan2(an.y, an.x) / 3.14 * 180 + 90;

	this->textureShader["tex"] = this->faceTex;
	this->textureShader["color"] = this->color;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(bodyPos.x, bodyPos.y + 30, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -20.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.0f, 80.0f, 1.0f));

	this->texIndices.drawElements();

	this->textureShader["color"] = glm::vec3(1.0f, 1.0f, 1.0f);

	this->textureShader["tex"] = this->barTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(id?World::width - 220.0f:20.0f, 10.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 20.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

	this->texIndices.drawElements();

	this->textureShader["tex"] = this->healthTex;
	this->textureShader["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(id?World::width - 220.0f:20.0f, 10.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(this->health * 2, 20.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

	this->texIndices.drawElements();


	std::vector<glm::vec2> lines_data;

	lines_data.push_back(this->body->masses[19]->pos);
	lines_data.push_back(this->body->masses[20]->pos);
	lines_data.push_back(this->body->masses[20]->pos);
	lines_data.push_back(this->body->masses[21]->pos);
	lines_data.push_back(this->body->masses[22]->pos);
	lines_data.push_back(this->body->masses[23]->pos);
	lines_data.push_back(this->body->masses[23]->pos);
	lines_data.push_back(this->body->masses[24]->pos);

	/*for(auto body = this->world.bodies.begin(); body != this->world.bodies.end(); ++body) {
		for(auto link = (*body)->links.begin(); link != (*body)->links.end(); ++link) {
			lines_data.push_back((*link)->mass1->pos);
			lines_data.push_back((*link)->mass2->pos);
		}
	} 
	for(auto link = this->body->links.begin(); link != this->body->links.end(); ++link) {
		lines_data.push_back((*link)->mass1->pos);
		lines_data.push_back((*link)->mass2->pos);
	}*/

	this->links.loadData(lines_data);

	this->pointShader.use();
	this->vao.bind();
	this->links.drawArrays(GL_LINES);
}

void Player::hurt() {
	health -= 0.5f;
	if(health < 0)
		this->alive = false;
}

void Player::update() {
	Controller::update();
	//if(this->body->masses[4]->pos.y > 600 - 300) // neck
	//	this->body->masses[4]->pos.y -= 7.0f;

	/*if(this->controller.getButton(0)) {
		this->health = 100.0f;
		this->alive = true;
	} */

	if(this->alive) {
		for(int i = 13; i <= 16; ++i)
			this->body->masses[i]->pos.y -= 4.0f;

		this->body->masses[6]->pos += this->controller.getStick(0, 1) * 4.0f; // left hand
		this->body->masses[8]->pos += this->controller.getStick(3, 4) * 4.0f; // right hand
	}

	//this->body->masses[6]->pos += leftStick[curInput] * 4.0f; // left hand
	//this->body->masses[8]->pos += rightStick[curInput] * 4.0f; // right hand
	//leftStick[curInput] = this->controller.getStick(0, 1);
	//rightStick[curInput] = this->controller.getStick(3, 4);
	//curInput++;
	//curInput %= 60;
}

const glm::vec3 Player::colors[3] = {
	glm::vec3(87.0f / 255.0f, 68.0f / 255.0f, 28.0f / 255.0f),
	glm::vec3(205.0f / 255.0f, 160.0f / 255.0f, 66.0f / 255.0f),
	glm::vec3(179.0f / 255.0f, 153.0f / 255.0f, 100.0f / 255.0f),
};