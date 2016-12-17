//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>
#include <map>
// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "Texture.h"
#include "Box3D.h"



using namespace std;

static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;

extern int statevar;
extern int statevarleft;
extern int statevarright;
// this is a helper variable to allow us to change the texture blend model
extern int g_change_texture_blend;

///keyframe stuff

typedef struct _keyframe
{
	float               _t; // the time fraction
	glm::vec3           _p; // the position
	glm::quat           _q; // the orientation

							/*
							Constructor
							*/
	_keyframe(float t, glm::vec3 p, glm::quat q)
	{
		_t = t;
		_p = p;
		_q = q;
	}

	/*
	Default constructor
	*/
	_keyframe()
	{
		_t = -1.0;
		_p = glm::vec3(0.0, 0.0, 0.0);
		_q = glm::quat(0.0, 0.0, 0.0, 0.0);
	}

	// prints the data into a terminal
	void print(void)
	{
		cout << "t: " << _t << "\tp: " << _p.x << ", " << _p.y << ", " << _p.z << "\tq: " << _q.x << ", " << _q.y << ", " << _q.z << ", " << _q.w << endl;
	}

}Keyframe;


/*
Type for the keyframe animation
*/
typedef map<double, Keyframe> KeyframeAnimation;

// Variable to store the keyframes
KeyframeAnimation myKeyframes;


/*!
@brief returns the time fraction for a given time and animation duration
@param time - the current animation time, application runtime, etc. in seconds
@param duration - the duration of the animation in seconds
@return the time fraction in an interval between 0 and 1.
*/
float getTimeFraction(const float time, const float duration)
{
	// we cast to an int. this results in the number of
	float interval = floor(time / duration);

	// return the current interval time
	float current_interval = time - interval*duration;

	// return the fraction / position in our current timeline
	float fraction = current_interval / duration;

	return fraction;
}



/*!
@brief returns the two keyframes for a given time.
@param keyframes - a map with all keyframes of type KeyframeAnimation
@param time - the time fraction between 0 and 1.
@param k0, reference to the first keyframe
@param k2, reference to the second keyframe
@return the number of keyframes. 1 if the time is equal to a keyframe, otherwise 2.
*/
int getKeyframes(KeyframeAnimation& keyframes, const double time, Keyframe& k0, Keyframe& k1)
{
	int num_keyframes = 0;

	// get a keyframe iterator
	KeyframeAnimation::iterator k_itr = keyframes.lower_bound(time);

	Keyframe k0_temp, k1_temp;

	// Obtain the first keyframe
	k1 = (*k_itr).second; num_keyframes++;


	// Check whether we are not at the beginning of this map
	if (k_itr != keyframes.begin())
	{
		k_itr--;  // decrement
		k0 = (*k_itr).second; // obtain the second keyframe
		num_keyframes++;
	}

	// write the first keyframe into k0 if we only have one
	if (num_keyframes == 1)
	{
		k0 = k1;
	}

	return num_keyframes;

}

/*!
@brief Interpolate between two keyframes
@param fraction - the time fraction for the interpolation / the location between two keyframes.
The value must be between 0 and 1.
@param k0, the start keyframe
@param k1, the end keyframe,
@param res, reference to a variable for the result.
*/
bool interpolateKeyframe(const float fraction, const Keyframe& k0, const Keyframe& k1, Keyframe& res)
{
	/////////////////////////////////////////////////////////////////////////
	// 1. Check the time delta

	// delta time
	float delta_t = k1._t - k0._t;

	// Check whether we have a delta time. Otherwise, we are at the location of exactly one keyframe
	if (delta_t == 0.0f) {
		res = k0;
		return true;
	}

	/////////////////////////////////////////////////////////////////////////
	// 2. Interpolat the position

	// get the delta
	glm::vec3 delta_p = k1._p - k0._p;

	// position interpolation
	glm::vec3 p_int = k0._p + delta_p * (fraction - k0._t) / (delta_t);


	/////////////////////////////////////////////////////////////////////////
	// 3. Rotation interpolation

	// Calculate the distance between the target angle and the current angle.
	float delta_angle = sqrt((k1._q.x - k0._q.x)*(k1._q.x - k0._q.x) +
		(k1._q.y - k0._q.y)*(k1._q.y - k0._q.y) +
		(k1._q.z - k0._q.z)*(k1._q.z - k0._q.z) +
		(k1._q.w - k0._q.w)*(k1._q.w - k0._q.w));


	// Linear interpolation of the rotation using slerp
	glm::quat r_int = glm::slerp(k0._q, k1._q, (fraction - k0._t) / (delta_t));


	/////////////////////////////////////////////////////////////////////////
	// 4. Write the result
	res = Keyframe(fraction, p_int, r_int);

	return true;
}

/*!
This initializes the keyframes.
*/
void initKeyframeAnimation(void)
{
	myKeyframes[0.0] = Keyframe(0.0, glm::vec3(0.0, 0.0, 0.0), angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.25] = Keyframe(0.25, glm::vec3(0, 0.0, 10.0), angleAxis(0.57f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.5] = Keyframe(0.5, glm::vec3(0, 0.0, 20.0), angleAxis(1.28f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.75] = Keyframe(0.75, glm::vec3(0, 0.0, 30.0), angleAxis(1.53f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[1.0] = Keyframe(1.0, glm::vec3(0, 0.0, 40.0), angleAxis(1.98f, glm::vec3(0.0, 0.1, 1.0)));



}

//endkeyframestuff




//cuboidal primitive end









int main(int argc, const char * argv[])
{
	int j;
	int jk;
	j = 0;
	jk = 0;
	statevar = 0;
	statevarleft = 0;
	statevarright = 0;
	GLObjectObj* loadedModel1 = NULL;



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Set the version which we expect, here, 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    




    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_texture.vs", "../../data/shaders/multi_texture.fs");
	GLAppearance* apperance_1 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");

    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(00.0,20.0,20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 5.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
	apperance_1->addLightSource(light_source);
    
    GLSpotLightSource spotlight_source;
    spotlight_source._lightPos = glm::vec4(0.0,00.0,50.0, 1.0);
    spotlight_source._ambient_intensity = 0.2;
    spotlight_source._specular_intensity = 30.5;
    spotlight_source._diffuse_intensity = 10.0;
    spotlight_source._attenuation_coeff = 0.0002;
    spotlight_source._cone_direction = glm::vec3(-1.0, -1.0,-1.0);
    spotlight_source._cone_angle = 20.0;
    
    apperance_0->addLightSource(spotlight_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.2, 0.0, 1.0);
    material_0._ambient_material = glm::vec3(0.2, 0.0, 1.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 12.0;
    material_0._transparency = 0.5;
    
//	setupScene();
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
	apperance_1->setMaterial(material_0);
    
    //************************************************************************************************
    // Add a texture





	GLMultiTexture* texture = new GLMultiTexture();
	int texid = texture->loadAndCreateTextures("road2.bmp", "Harambe_Closeup.bmp", "sky.bmp");
	//int texid = texture->loadAndCreateTexture("../../data/textures/texture_earth_128x128_a.bmp");
	apperance_0->setTexture(texture);




    //GLTexture* texture = new GLTexture();
    //int texid = texture->loadAndCreateTexture("road2.bmp");
    //apperance_0->setTexture(texture);
    
    //************************************************************************************************
    // Finalize the appearance object
    apperance_0->finalize();
	apperance_1->finalize();

    
    // create the sphere geometry
	//increase dimensions of background texture
    GLPlane3D* plane_0 = new GLPlane3D(0.0, 0.0, 0.0, 500.0, 500.0);
    plane_0->setApperance(*apperance_0);
    plane_0->init();
    
    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
	apperance_1->updateLightSources();

	//load the object
	//GLObjectObj object_1;
    
	loadedModel1 = new GLObjectObj("../data/lamborghini.obj");
	loadedModel1->setApperance(*apperance_0);

	GLBox3D* loadedModel2 = new GLBox3D(10, 10, 10);	
	GLBox3D* loadedModel3 = new GLBox3D(10, 10, 10);	// BOX
	GLBox3D* loadedModel4 = new GLBox3D(10, 10, 10);
	GLBox3D* loadedModel5 = new GLBox3D(10, 10, 10);
	GLBox3D* loadedModel6 = new GLBox3D(10, 10, 10);
	GLBox3D* loadedModel7 = new GLBox3D(10, 10, 10);
	GLBox3D* loadedModel8 = new GLBox3D(10, 10, 10);
	GLBox3D* loadedModel9 = new GLBox3D(10, 10, 10);
	loadedModel2->setApperance(*apperance_1);
	loadedModel3->setApperance(*apperance_1);
	loadedModel4->setApperance(*apperance_1);
	loadedModel5->setApperance(*apperance_1);
	loadedModel6->setApperance(*apperance_1);
	loadedModel7->setApperance(*apperance_1);
	loadedModel8->setApperance(*apperance_1);
	loadedModel9->setApperance(*apperance_1);
	//translate the model
	glm::mat4 translate1 = glm::translate(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 rotate1 = glm::rotate(glm::mat4(1.0), -1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
	loadedModel1->init();
	loadedModel2->init();
	loadedModel3->init();
	loadedModel4->init();
	loadedModel5->init();
	loadedModel6->init();
	loadedModel7->init();
	loadedModel8->init();
	loadedModel9->init();
	//badri_change
	//glm::mat4 _rotatedMatrix = glm::rotate(glm::mat4(1.0), -1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 translate1 = glm::rotate(glm::vec3(1.0f, 1.0f, 1.0f));
	loadedModel1->setMatrix(translate1);
	glm::mat4 init_mat = rotate1*translate1;
	loadedModel1->setMatrix(init_mat);
	glm::mat4 translate75 = glm::translate(glm::vec3(0.0f, 5.0f, -100.0f));
	glm::mat4 translate76 = glm::translate(glm::vec3(50.0f, 5.0f, -100.0f));
	glm::mat4 translate77 = glm::translate(glm::vec3(-100.0f, 5.0f, -100.0f));
	glm::mat4 translate78 = glm::translate(glm::vec3(-150.0f, 5.0f, -100.0f));
	glm::mat4 translate79 = glm::translate(glm::vec3(25.0f, 5.0f, -200.0f));
	glm::mat4 translate80 = glm::translate(glm::vec3(-25.0f, 5.0f, -200.0f));
	glm::mat4 translate81 = glm::translate(glm::vec3(75.0f, 5.0f, -200.0f));
	glm::mat4 translate82 = glm::translate(glm::vec3(-75.0f, 5.0f, -200.0f));
	glm::mat4 translate83 = glm::translate(glm::vec3(-125.0f, 5.0f, -200.0f));
	//glm::mat4 translate84 = glm::translate(glm::vec3(0.0f, 5.0f, -100.0f));
	glm::mat4 initmat1 = translate75*init_mat;
	loadedModel2->setMatrix(initmat1);
	glm::mat4 initmat2= translate76*init_mat;
	glm::mat4 initmat3 = translate77*init_mat;
	glm::mat4 initmat4 = translate78*init_mat;
	glm::mat4 initmat5 = translate79*init_mat;
	glm::mat4 initmat6 = translate80*init_mat;
	glm::mat4 initmat7 = translate81*init_mat;
	glm::mat4 initmat8 = translate82*init_mat;
	glm::mat4 initmat9 = translate83*init_mat;
	loadedModel3->setMatrix(initmat2);
	loadedModel4->setMatrix(initmat3);
	loadedModel5->setMatrix(initmat4);
	loadedModel6->setMatrix(initmat5);
	loadedModel7->setMatrix(initmat6);
	loadedModel8->setMatrix(initmat7);
	loadedModel9->setMatrix(initmat8);
	//loadedModel3->setMatrix(initmat1);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // sphere->enableNormalVectorRenderer();
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
	initKeyframeAnimation();
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        plane_0->draw();
		loadedModel1->draw();
		loadedModel2->draw();
		loadedModel3->draw();
		loadedModel4->draw();
		loadedModel5->draw();
		loadedModel6->draw();
		loadedModel7->draw();
		loadedModel8->draw();
		loadedModel9->draw();
        // change the texture appearance blend mode
		//badri_change
		if (statevar == 1)
		{bool ret = texture->setTextureBlendMode(g_change_texture_blend);
		
        if(ret)apperance_0->updateTextures();
		
		     
			glm::mat4 translate2 = glm::translate(glm::vec3(-0.1f, 0.0f, 0.0f));
			init_mat = init_mat*translate2;
			loadedModel1->setMatrix(init_mat);
		}
        
		if (statevarleft == 1)
		{
			j = 0;
			if (j == 0)
			{
				glm::mat4 translate3 = glm::translate(glm::vec3(0.0f, 0.0f, 10.0f));
				init_mat = init_mat*translate3;
				loadedModel1->setMatrix(init_mat);
				j++;
				statevarleft = 0;
			}
		}
			

		if (statevarright == 1)
		{
			jk = 0;
			if (jk == 0)
			{
				glm::mat4 translate3 = glm::translate(glm::vec3(0.0f, 0.0f, -10.0f));
				init_mat = init_mat*translate3;
				loadedModel1->setMatrix(init_mat);
				j++;
				statevarright = 0;
			}
		}

        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		

		Keyframe k0, k1, k_res;

		float time = glfwGetTime();
		
		if (time <= 60.0)
		{
			cout << time << endl;
			float f = getTimeFraction(time, 60.0); // we assume that the animation takes 8 seconds

			int num = getKeyframes(myKeyframes, f, k0, k1);

			bool ret = interpolateKeyframe(f, k0, k1, k_res);


			//k_res.print();

			glm::mat4 translate7 = glm::translate(k_res._p);
			//glm::mat4 matrix = mat4_cast(k_res._q);

			//glm::mat4 rotate9 = glm::rotate(matrix);
			//init_mat = init_mat*translate7;
			//loadedModel1->setMatrix(translate7);
		}

        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;
    
    
}

