/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#include "ofxChipmunk.h"

#define LINE_COLOR 0.0f, 0.0f, 0.0f
#define COLLISION_COLOR 1.0f, 0.0f, 0.0f
#define BODY_COLOR 0.0f, 0.0f, 1.0f

void
glColor_from_pointer(void *ptr)
{
	unsigned long val = (long)ptr;
	
	// hash the pointer up nicely
	val = (val+0x7ed55d16) + (val<<12);
	val = (val^0xc761c23c) ^ (val>>19);
	val = (val+0x165667b1) + (val<<5);
	val = (val+0xd3a2646c) ^ (val<<9);
	val = (val+0xfd7046c5) + (val<<3);
	val = (val^0xb55a4f09) ^ (val>>16);
	
	//	GLfloat v = (GLfloat)val/(GLfloat)ULONG_MAX;
	//	v = 0.95f - v*0.15f;
	//	
	//	glColor3f(v, v, v);
	
	GLubyte r = (val>>0) & 0xFF;
	GLubyte g = (val>>8) & 0xFF;
	GLubyte b = (val>>16) & 0xFF;
	
	GLubyte max = r>g ? (r>b ? r : b) : (g>b ? g : b);
	
	const int mult = 127;
	const int add = 63;
	r = (r*mult)/max + add;
	g = (g*mult)/max + add;
	b = (b*mult)/max + add;
	
	glColor3ub(r, g, b);
}

static const GLfloat circleVAR[] = {
	0.0000f,  1.0000f,
	0.2588f,  0.9659f,
	0.5000f,  0.8660f,
	0.7071f,  0.7071f,
	0.8660f,  0.5000f,
	0.9659f,  0.2588f,
	1.0000f,  0.0000f,
	0.9659f, -0.2588f,
	0.8660f, -0.5000f,
	0.7071f, -0.7071f,
	0.5000f, -0.8660f,
	0.2588f, -0.9659f,
	0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	0.0000f,  1.0000f,
	0.0f, 0.0f, // For an extra line to see the rotation.
};
static const int circleVAR_count = sizeof(circleVAR)/sizeof(GLfloat)/2;

void
drawCircleShape(cpBody *body, cpCircleShape *circle)
{
	glVertexPointer(2, GL_FLOAT, 0, circleVAR);
	
	glPushMatrix(); {
		cpVect center = circle->tc;
		glTranslatef(center.x, center.y, 0.0f);
		glRotatef(body->a*180.0f/M_PI, 0.0f, 0.0f, 1.0f);
		glScalef(circle->r, circle->r, 1.0f);
		
		if(!circle->shape.sensor){
			glColor_from_pointer(circle);
			glDrawArrays(GL_TRIANGLE_FAN, 0, circleVAR_count - 1);
		}
		
		glColor3f(LINE_COLOR);
		glDrawArrays(GL_LINE_STRIP, 0, circleVAR_count);
	} glPopMatrix();
	
}

static const GLfloat pillVAR[] = {
	0.0000f,  1.0000f, 1.0f,
	0.2588f,  0.9659f, 1.0f,
	0.5000f,  0.8660f, 1.0f,
	0.7071f,  0.7071f, 1.0f,
	0.8660f,  0.5000f, 1.0f,
	0.9659f,  0.2588f, 1.0f,
	1.0000f,  0.0000f, 1.0f,
	0.9659f, -0.2588f, 1.0f,
	0.8660f, -0.5000f, 1.0f,
	0.7071f, -0.7071f, 1.0f,
	0.5000f, -0.8660f, 1.0f,
	0.2588f, -0.9659f, 1.0f,
	0.0000f, -1.0000f, 1.0f,
	
	0.0000f, -1.0000f, 0.0f,
	-0.2588f, -0.9659f, 0.0f,
	-0.5000f, -0.8660f, 0.0f,
	-0.7071f, -0.7071f, 0.0f,
	-0.8660f, -0.5000f, 0.0f,
	-0.9659f, -0.2588f, 0.0f,
	-1.0000f, -0.0000f, 0.0f,
	-0.9659f,  0.2588f, 0.0f,
	-0.8660f,  0.5000f, 0.0f,
	-0.7071f,  0.7071f, 0.0f,
	-0.5000f,  0.8660f, 0.0f,
	-0.2588f,  0.9659f, 0.0f,
	0.0000f,  1.0000f, 0.0f,
};
static const int pillVAR_count = sizeof(pillVAR)/sizeof(GLfloat)/3;

void
drawSegmentShape(cpBody *body, cpSegmentShape *seg)
{
	cpVect a = seg->ta;
	cpVect b = seg->tb;
	
	if(seg->r){
		glVertexPointer(3, GL_FLOAT, 0, pillVAR);
		glPushMatrix(); {
			GLfloat x = a.x;
			GLfloat y = a.y;
			
			cpVect d = cpvsub(b, a);
			cpFloat len = cpvlength(d)/seg->r;
			GLfloat cos = d.x/len;
			GLfloat sin = d.y/len;
			
			const GLfloat matrix[] = {
				cos, sin, 0.0f, 0.0f,
				-sin, cos, 0.0f, 0.0f,
				d.x, d.y, 1.0f, 0.0f,
				x,   y, 0.0f, 1.0f,
			};
			glMultMatrixf(matrix);
			
			if(!seg->shape.sensor){
				glColor_from_pointer(seg);
				glDrawArrays(GL_TRIANGLE_FAN, 0, pillVAR_count);
			}
			
			glColor3f(LINE_COLOR);
			glDrawArrays(GL_LINE_LOOP, 0, pillVAR_count);
		} glPopMatrix();
	} else {
		glColor3f(LINE_COLOR);
		glBegin(GL_LINES); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
	}
}

void
drawPolyShape(cpBody *body, cpPolyShape *poly)
{
	int count = count=poly->numVerts;
	glVertexPointer(2, GL_DOUBLE, 0, poly->tVerts);
	
	if(!poly->shape.sensor){
		glColor_from_pointer(poly);
		glDrawArrays(GL_TRIANGLE_FAN, 0, count);
	}
	
	glColor3f(LINE_COLOR);
	glDrawArrays(GL_LINE_LOOP, 0, count);
}

void
drawObject(void *ptr, void *unused)
{
	cpShape *shape = (cpShape *)ptr;
	cpBody *body = shape->body;
	
	switch(shape->klass->type){
		case CP_CIRCLE_SHAPE:
			drawCircleShape(body, (cpCircleShape *)shape);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(body, (cpSegmentShape *)shape);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(body, (cpPolyShape *)shape);
			break;
		default:
			printf("Bad enumeration in drawObject().\n");
	}
}

static const GLfloat springVAR[] = {
	0.00f, 0.0f,
	0.20f, 0.0f,
	0.25f, 3.0f,
	0.30f,-6.0f,
	0.35f, 6.0f,
	0.40f,-6.0f,
	0.45f, 6.0f,
	0.50f,-6.0f,
	0.55f, 6.0f,
	0.60f,-6.0f,
	0.65f, 6.0f,
	0.70f,-3.0f,
	0.75f, 6.0f,
	0.80f, 0.0f,
	1.00f, 0.0f,
};
static const int springVAR_count = sizeof(springVAR)/sizeof(GLfloat)/2;

void
drawSpring(cpDampedSpring *spring, cpBody *body_a, cpBody *body_b)
{
	cpVect a = cpvadd(body_a->p, cpvrotate(spring->anchr1, body_a->rot));
	cpVect b = cpvadd(body_b->p, cpvrotate(spring->anchr2, body_b->rot));
	
	glPointSize(5.0f);
	glBegin(GL_POINTS); {
		glVertex2f(a.x, a.y);
		glVertex2f(b.x, b.y);
	} glEnd();
	
	cpVect delta = cpvsub(b, a);
	
	glVertexPointer(2, GL_FLOAT, 0, springVAR);
	glPushMatrix(); {
		GLfloat x = a.x;
		GLfloat y = a.y;
		GLfloat cos = delta.x;
		GLfloat sin = delta.y;
		GLfloat s = 1.0f/cpvlength(delta);
		
		const GLfloat matrix[] = {
			cos,    sin, 0.0f, 0.0f,
			-sin*s,  cos*s, 0.0f, 0.0f,
			0.0f,   0.0f, 1.0f, 0.0f,
			x,      y, 0.0f, 1.0f,
		};
		
		glMultMatrixf(matrix);
		glDrawArrays(GL_LINE_STRIP, 0, springVAR_count);
	} glPopMatrix();
}

void
drawConstraint(cpConstraint *constraint)
{
	cpBody *body_a = constraint->a;
	cpBody *body_b = constraint->b;
	
	const cpConstraintClass *klass = constraint->klass;
	if(klass == cpPinJointGetClass()){
		cpPinJoint *joint = (cpPinJoint *)constraint;
		
		cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
		cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));
		
		glPointSize(5.0f);
		glBegin(GL_POINTS); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
		
		glBegin(GL_LINES); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
	} else if(klass == cpSlideJointGetClass()){
		cpSlideJoint *joint = (cpSlideJoint *)constraint;
		
		cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
		cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));
		
		glPointSize(5.0f);
		glBegin(GL_POINTS); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
		
		glBegin(GL_LINES); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
	} else if(klass == cpPivotJointGetClass()){
		cpPivotJoint *joint = (cpPivotJoint *)constraint;
		
		cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
		cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));
		
		glPointSize(10.0f);
		glBegin(GL_POINTS); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
	} else if(klass == cpGrooveJointGetClass()){
		cpGrooveJoint *joint = (cpGrooveJoint *)constraint;
		
		cpVect a = cpvadd(body_a->p, cpvrotate(joint->grv_a, body_a->rot));
		cpVect b = cpvadd(body_a->p, cpvrotate(joint->grv_b, body_a->rot));
		cpVect c = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));
		
		glPointSize(5.0f);
		glBegin(GL_POINTS); {
			glVertex2f(c.x, c.y);
		} glEnd();
		
		glBegin(GL_LINES); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();
	} else if(klass == cpDampedSpringGetClass()){
		drawSpring((cpDampedSpring *)constraint, body_a, body_b);
	} else {
		//		printf("Cannot draw constraint\n");
	}
}

void
drawBB(void *ptr, void *unused)
{
	cpShape *shape = (cpShape *)ptr;
	
	glBegin(GL_LINE_LOOP); {
		glVertex2f(shape->bb.l, shape->bb.b);
		glVertex2f(shape->bb.l, shape->bb.t);
		glVertex2f(shape->bb.r, shape->bb.t);
		glVertex2f(shape->bb.r, shape->bb.b);
	} glEnd();
}

void
drawCollisions(void *ptr, void *data)
{
	cpArbiter *arb = (cpArbiter *)ptr;
	for(int i=0; i<arb->numContacts; i++){
		cpVect v = arb->contacts[i].p;
		glVertex2f(v.x, v.y);
	}
}

// copied from cpSpaceHash.c
inline cpHashValue
hash_func(cpHashValue x, cpHashValue y, cpHashValue n)
{
	return (x*1640531513ul ^ y*2654435789ul) % n;
}

void
drawSpatialHash(cpSpaceHash *hash)
{
	cpBB bb = cpBBNew(-320, -240, 320, 240);
	
	cpFloat dim = hash->celldim;
	int n = hash->numcells;
	
	int l = (int)floor(bb.l/dim);
	int r = (int)floor(bb.r/dim);
	int b = (int)floor(bb.b/dim);
	int t = (int)floor(bb.t/dim);
	
	for(int i=l; i<=r; i++){
		for(int j=b; j<=t; j++){
			int cell_count = 0;
			
			int index = hash_func(i,j,n);
			for(cpSpaceHashBin *bin = hash->table[index]; bin; bin = bin->next)
				cell_count++;
			
			GLfloat v = 1.0f - (GLfloat)cell_count/10.0f;
			glColor3f(v,v,v);
			glRectf(i*dim, j*dim, (i + 1)*dim, (j + 1)*dim);
		}
	}
}

void drawInfo(cpSpace *space) {
	static int maxArbiters = 0;
	static int maxPoints = 0;
	static int maxConstraints = 0;
	
	
	int arbiters = space->arbiters->num;
	int points = 0;
	
	for(int i=0; i<arbiters; i++)
		points += ((cpArbiter *)(space->arbiters->arr[i]))->numContacts;
	
	int constraints = (space->constraints->num + points)*(space->iterations + space->elasticIterations);
	
	maxArbiters = arbiters > maxArbiters ? arbiters : maxArbiters;
	maxPoints = points > maxPoints ? points : maxPoints;
	maxConstraints = constraints > maxConstraints ? constraints : maxConstraints;
	
	char buffer[1000];
	char *format = 
	"Arbiters: %d (%d) - "
	"Contact Points: %d (%d)\n"
	"Other Constraints: %d, Iterations: %d\n"
	"Constraints x Iterations: %d (%d)";
	
	snprintf(buffer, 1000, format,
			 arbiters, maxArbiters,
			 points, maxPoints,
			 space->constraints->num, space->iterations + space->elasticIterations,
			 constraints, maxConstraints
			 );
	
	ofDrawBitmapString(buffer, ofGetWidth() - 400, 50);
}
