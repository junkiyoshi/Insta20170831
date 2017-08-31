#pragma once

#include "ofMain.h"

class Particle 
{
public:
	Particle();
	Particle(ofVec3f location, int size, float r, float g, float b);
	~Particle();

	void flok(vector<Particle*> particles);
	ofVec3f seek(ofVec3f target);
	void borders();
	void think();
	void update();
	void draw();
private:
	void applyForce(ofVec3f force);
	ofVec3f separate(vector<Particle*> particles);
	ofVec3f align(vector<Particle*> particles);
	ofVec3f cohesion(vector<Particle*> particles);

	ofVec3f location;
	ofVec3f velocity;
	ofVec3f acceleration;
	
	float max_speed;
	float max_force;
	float angle;
	float radius;
	float body_size;

	ofImage img;
	
	int img_size;
	float r, g, b;

};
