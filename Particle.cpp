
#include "Particle.h"

Particle::Particle() : Particle(ofVec3f(0, 0, 0), 100, 255, 255, 255)
{

}

Particle::Particle(ofVec3f location, int size, float r, float g, float b)
{
	this->location = location;
	this->velocity = ofVec3f(0, 0, 0);
	this->acceleration = ofVec3f(0, 0, 0);

	this->max_speed = 30;
	this->max_force = 10;
	this->radius = 100;
	this->angle = ofRandom(360);
	this->body_size = 20;

	this->img_size = size;
	this->r = r;
	this->g = g;
	this->b = b;

	this->img.allocate(this->img_size, this->img_size, OF_IMAGE_COLOR);
	unsigned char* pixels = this->img.getPixels().getData();

	for (int x = 0; x < this->img_size; x++) {
		for (int y = 0; y < this->img_size; y++) {
			float distance = ((this->img_size / 2 - x) * (this->img_size / 2 - x) + (this->img_size / 2 - y) * (this->img_size / 2 - y)) / 50.f;
			if (distance == 0) { distance = 0.003; }

			int r_value = this->r / distance - 3;
			r_value = max(0, min(r_value, 255));

			int g_value = this->g / distance - 3;
			g_value = max(0, min(g_value, 255));

			int b_value = this->b / distance - 3;
			b_value = max(0, min(b_value, 255));

			pixels[x * 3 * size + y * 3 + 0] = r_value;
			pixels[x * 3 * size + y * 3 + 1] = g_value;
			pixels[x * 3 * size + y * 3 + 2] = b_value;
		}
	}

	this->img.update();
}

Particle::~Particle()
{

}

void Particle::applyForce(ofVec3f force)
{
	this->acceleration += force;
}

ofVec3f Particle::seek(ofVec3f target)
{
	ofVec3f desired = this->location - target;
	float distance = desired.length();
	desired.normalize();
	if (distance < this->radius)
	{
		float m = ofMap(distance, 0, this->radius, 0, this->max_speed);
		desired *= m;
	}
	else
	{
		desired *= this->max_speed;
	}

	ofVec3f steer = this->velocity - desired;
	steer.limit(this->max_force);
	return steer;
}

ofVec3f Particle::separate(vector<Particle*> particles)
{
	float desiredseparation = this->body_size * 2.5;
	ofVec3f sum;
	int count = 0;
	for (Particle* p : particles) {
		float distance = this->location.distance(p->location);
		if (distance > 0 && distance < desiredseparation) {
			ofVec3f diff = this->location - p->location;
			diff.normalize();
			diff /= distance;
			sum += diff;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		sum.limit(this->max_speed);
		ofVec3f steer = this->velocity - sum;
		steer.limit(this->max_force);

		return steer;
	}

	return ofVec3f(0, 0, 0);
}

ofVec3f Particle::align(vector<Particle*> particles)
{
	float neighbordist = this->body_size * 15.0;
	ofVec3f sum;
	int count = 0;
	for (Particle* p : particles) {
		float distance = this->location.distance(p->location);
		if (distance > 0 && distance < neighbordist) {
			sum += p->velocity;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		sum.normalize();
		sum.limit(this->max_speed);
		ofVec3f steer = this->velocity - sum;
		steer.limit(this->max_force);

		return steer;
	}

	return ofVec3f(0, 0, 0);
}

ofVec3f Particle::cohesion(vector<Particle*> particles)
{
	float neighbordist = this->body_size * 15.0;
	ofVec3f sum;
	int count = 0;
	for (Particle* p: particles) {
		float distance = this->location.distance(p->location);
		if (distance > 0 && distance < neighbordist) {
			sum += p->location;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		return seek(sum);
	}

	return ofVec3f(0, 0, 0);
}


void Particle::flok(vector<Particle*> particles)
{
	ofVec3f sep = this->separate(particles);
	ofVec3f ali = this->align(particles);
	ofVec3f coh = this->cohesion(particles);

	sep *= 1.0;
	ali *= 1.2;
	coh *= 1.0;

	this->applyForce(sep);
	this->applyForce(ali);
	this->applyForce(coh);
}

void Particle::borders()
{
	ofVec3f distance = this->location - ofVec3f(0, 0, 0);
	if (distance.length() > 720) {
		distance.normalize();
		distance *= -this->max_force;
		applyForce(distance);
	}
}

void Particle::think()
{
	ofVec3f future;
	future.set(this->velocity);
	future.normalize();
	future *= this->radius * 2;
	future += this->location;

	ofVec3f random = ofVec3f(ofRandom(-ofGetWidth() / 2, ofGetWidth() / 2), ofRandom(-ofGetHeight() / 2, ofGetHeight() / 2), ofRandom(-512, 512));
	random.normalize();
	random *= this->radius * 2;
	random += future;

	this->applyForce(this->seek(random));
}


void Particle::update()
{
	this->velocity += this->acceleration;
	this->velocity.limit(max_speed);
	this->location += this->velocity;

	this->acceleration *= 0;
	this->velocity *= 0.98;
}

void Particle::draw()
{
	ofPushMatrix();
	ofTranslate(this->location.x - this->img.getWidth() / 2, this->location.y - this->img.getHeight() / 2, this->location.z);

	this->img.draw(0, 0);

	ofPopMatrix();
}

