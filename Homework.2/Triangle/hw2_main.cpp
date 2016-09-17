//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
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




/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




					   // The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

/*!
ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
*/
unsigned int createTriangleStripModel(void)
{
	// use the vertex array object vaoID[0] for this model representation

	float* vertices = new float[156];  // Vertices for our square
	float *colors = new float[156]; // Colors for our vertices

	vertices[0] = 1.0; vertices[1] = 0.0; vertices[2] = 0.0; // Bottom left corner
	colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner

	vertices[3] = 1.0; vertices[4] = 1.0; vertices[5] = 0.0; // Top left corner
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0; // Top left corner

	vertices[6] = 0.0; vertices[7] = 0.0; vertices[8] = 0.0; // Top Right corner
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner
													   //tri1



	vertices[9] = 0.0; vertices[10] = 1.0; vertices[11] = 0.0; // Bottom right corner
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner

														 //tri2


	vertices[12] = 0.0; vertices[13] = 0.0; vertices[14] = -2.0; // Bottom left corner
	colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner
														  //tri3

	vertices[15] = 0.0; vertices[16] = 1.0; vertices[17] = -2.0; // Top Right corner
	colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner
														  //tri4
	vertices[18] = 0.0; vertices[19] = 0.0; vertices[20] = -3.0; // Top Right corner
	colors[18] = 0.0; colors[19] = 1.0; colors[20] = 0.0; // Top Right corner
														  //tri5
	vertices[21] = 0.0; vertices[22] = 1.0; vertices[23] = -3.0; // Top Right corner
	colors[21] = 0.0; colors[22] = 1.0; colors[23] = 0.0; // Top Right corner
														  //tri6 v8
	vertices[24] = 1.0; vertices[25] = 0.0; vertices[26] = -3.0; // Top Right corner
	colors[24] = 0.0; colors[25] = 1.0; colors[26] = 0.0; // Top Right corner
														  //tri7 v9
	vertices[27] = 1.0; vertices[28] = 1.0; vertices[29] = -3.0; // Top Right corner
	colors[27] = 0.0; colors[28] = 1.0; colors[29] = 0.0; // Top Right corner
														  //tri8v10
	vertices[30] = 3.0; vertices[31] = 0.0; vertices[32] = -3.0; // Top Right corner
	colors[30] = 0.0; colors[31] = 1.0; colors[32] = 0.0; // Top Right corner
														  //tri9v11
	vertices[33] = 3.0; vertices[34] = 1.0; vertices[35] = -3.0; // Top Right corner
	colors[33] = 0.0; colors[34] = 1.0; colors[35] = 0.0; // Top Right corner
														  //tri10v12
	vertices[36] = 3.0; vertices[37] = 0.0; vertices[38] = -2.0; // Top Right corner
	colors[36] = 0.0; colors[37] = 1.0; colors[38] = 0.0; // Top Right corner
														  //tri11 13

	vertices[39] = 3.0; vertices[40] = 1.0; vertices[41] = -2.0; // Top Right corner
	colors[39] = 0.0; colors[40] = 1.0; colors[41] = 0.0; // Top Right corner

														  //tri12 14

	vertices[42] = 1.0; vertices[43] = 0.0; vertices[44] = -2.0; // Top Right corner
	colors[42] = 0.0; colors[43] = 1.0; colors[44] = 0.0; // Top Right corner

														  //tri13 15

	vertices[45] = 1.0; vertices[46] = 1.0; vertices[47] = -2.0; // Top Right corner
	colors[45] = 0.0; colors[46] = 1.0; colors[47] = 0.0; // Top Right corner

														  //tri14 v16
	vertices[48] = 1.0; vertices[49] = 0.0; vertices[50] = 0.0; // Top Right corner
	colors[48] = 0.0; colors[49] = 1.0; colors[50] = 0.0; // Top Right corner

														  //tri15 v17
	vertices[51] = 1.0; vertices[52] = 1.0; vertices[53] = 0.0; // Top Right corner
	colors[51] = 0.0; colors[52] = 1.0; colors[53] = 0.0; // Top Right corner

														  //tri16 v18

	vertices[54] = 1.0; vertices[55] = 1.0; vertices[56] = 0.0; // Top Right corner
	colors[54] = 0.0; colors[55] = 1.0; colors[56] = 0.0; // Top Right corner



														  //tri17 v18

	vertices[57] = 0.0; vertices[58] = 1.0; vertices[59] = 0.0; // Top Right corner
	colors[57] = 0.0; colors[58] = 1.0; colors[59] = 0.0; // Top Right corner

														  //tri18 v19

	vertices[60] = 1.0; vertices[61] = 1.0; vertices[62] = -2.0; // Top Right corner
	colors[60] = 0.0; colors[61] = 1.0; colors[62] = 0.0; // Top Right corner

														  //v20 t19

	vertices[63] = 0.0; vertices[64] = 1.0; vertices[65] = -2.0; // Top Right corner
	colors[63] = 0.0; colors[64] = 1.0; colors[65] = 0.0; // Top Right corner

														  //v21 t20

	vertices[66] = 1.0; vertices[67] = 2.0; vertices[68] = -3.0; // Top Right corner
	colors[66] = 0.0; colors[67] = 1.0; colors[68] = 0.0; // Top Right corner

														  //t21 v22
	vertices[69] = 0.0; vertices[70] = 2.0; vertices[71] = -3.0; // Top Right corner
	colors[69] = 0.0; colors[70] = 1.0; colors[71] = 0.0; // Top Right corner

														  //t22 v23
	vertices[72] = 1.0; vertices[73] = 1.0; vertices[74] = -3.0; // Top Right corner
	colors[72] = 0.0; colors[73] = 1.0; colors[74] = 0.0; // Top Right corner
														  //t23 v24

	vertices[75] = 0.0; vertices[76] = 1.0; vertices[77] = -3.0; // Top Right corner
	colors[75] = 0.0; colors[76] = 1.0; colors[77] = 0.0; // Top Right corner

														  //t24 v25

	vertices[78] = 0.0; vertices[79] = 1.0; vertices[80] = -3.0; // Top Right corner
	colors[78] = 0.0; colors[79] = 1.0; colors[80] = 0.0; // Top Right corner

														  //t25 v25

	vertices[81] = 1.0; vertices[82] = 1.0; vertices[83] = -3.0; // Top Right corner
	colors[81] = 0.0; colors[82] = 1.0; colors[83] = 0.0; // Top Right corner

														  //t26 v24

	vertices[84] = 3.0; vertices[85] = 1.0; vertices[86] = -2.0; // Top Right corner
	colors[84] = 0.0; colors[85] = 1.0; colors[86] = 0.0; // Top Right corner

														  //t27 v26
	vertices[87] = 1.0; vertices[88] = 2.0; vertices[89] = -3.0; // Top Right corner
	colors[87] = 0.0; colors[88] = 1.0; colors[89] = 0.0; // Top Right corner
														  //t28 v27

	vertices[90] = 3.0; vertices[91] = 1.0; vertices[92] = -2.0; // Top Right corner
	colors[90] = 0.0; colors[91] = 1.0; colors[92] = 0.0; // Top Right corner

														  //t29 v28
	vertices[93] = 1.0; vertices[94] = 2.0; vertices[95] = -2.0; // Top Right corner
	colors[93] = 0.0; colors[94] = 1.0; colors[95] = 0.0; // Top Right corner

														  //t30 v29

	vertices[96] = 1.0; vertices[97] = 1.0; vertices[98] = -2.0; // Top Right corner
	colors[96] = 0.0; colors[97] = 1.0; colors[98] = 0.0; // Top Right corner

														  //t31 v30

	vertices[99] = 1.0; vertices[100] = 1.0; vertices[101] = -2.0; // Top Right corner
	colors[99] = 0.0; colors[100] = 1.0; colors[101] = 0.0; // Top Right corner

															//t32 v30

	vertices[102] = 0.0; vertices[103] = 1.0; vertices[104] = -2.0; // Top Right corner
	colors[102] = 0.0; colors[103] = 1.0; colors[104] = 0.0; // Top Right corner

															 //t33 v21

	vertices[102] = 0.0; vertices[103] = 1.0; vertices[104] = -2.0; // Top Right corner
	colors[102] = 0.0; colors[103] = 1.0; colors[104] = 0.0; // Top Right corner

															 //t34 v21
	vertices[105] = 0.0; vertices[106] = 2.0; vertices[107] = -2.0; // Top Right corner
	colors[105] = 0.0; colors[106] = 1.0; colors[107] = 0.0; // Top Right corner

															 //t35 v31

	vertices[108] = 0.0; vertices[109] = 1.0; vertices[110] = -3.0; // Top Right corner
	colors[108] = 0.0; colors[109] = 1.0; colors[110] = 0.0; // Top Right corner
															 //t36 v32
	vertices[111] = 0.0; vertices[112] = 2.0; vertices[113] = -3.0; // Top Right corner
	colors[111] = 0.0; colors[112] = 1.0; colors[113] = 0.0; // Top Right corner

															 //t37 v33
	vertices[114] = 0.0; vertices[115] = 2.0; vertices[116] = -3.0; // Top Right corner
	colors[114] = 0.0; colors[115] = 1.0; colors[116] = 0.0; // Top Right corner

															 //t38 v33
	vertices[117] = 0.0; vertices[118] = 1.0; vertices[119] = -3.0; // Top Right corner
	colors[117] = 0.0; colors[118] = 1.0; colors[119] = 0.0; // Top Right corner
															 //t39 v32
	vertices[120] = 0.0; vertices[121] = 1.0; vertices[122] = -3.0; // Top Right corner
	colors[120] = 0.0; colors[121] = 1.0; colors[122] = 0.0; // Top Right corner
															 //t40 v32

	vertices[123] = 0.0; vertices[124] = 0.0; vertices[125] = -3.0; // Top Right corner
	colors[123] = 0.0; colors[124] = 1.0; colors[125] = 0.0; // Top Right corner
															 //t41 v34
	vertices[126] = 0.0; vertices[127] = 0.0; vertices[128] = -3.0; // Top Right corner
	colors[126] = 0.0; colors[127] = 1.0; colors[128] = 0.0; // Top Right corner

															 //t42 v34
	vertices[129] = 3.0; vertices[130] = 0.0; vertices[131] = -3.0; // Top Right corner
	colors[129] = 0.0; colors[130] = 1.0; colors[131] = 0.0; // Top Right corner
															 //t43 v35

	vertices[132] = 0.0; vertices[133] = 0.0; vertices[134] = -2.0; // Top Right corner
	colors[132] = 0.0; colors[133] = 1.0; colors[134] = 0.0; // Top Right corner

															 //t44 v36
	vertices[135] = 3.0; vertices[136] = 0.0; vertices[137] = -2.0; // Top Right corner
	colors[135] = 0.0; colors[136] = 1.0; colors[137] = 0.0; // Top Right corner

															 //t45 v37
	vertices[138] = 3.0; vertices[139] = 0.0; vertices[140] = -2.0; // Top Right corner
	colors[138] = 0.0; colors[139] = 1.0; colors[140] = 0.0; // Top Right corner

															 //t46 v37

	vertices[141] = 1.0; vertices[142] = 0.0; vertices[143] = -2.0; // Top Right corner
	colors[141] = 0.0; colors[142] = 1.0; colors[143] = 0.0; // Top Right corner


															 //t47 v15
	vertices[144] = 1.0; vertices[145] = 0.0; vertices[146] = -2.0; // Top Right corner
	colors[144] = 0.0; colors[145] = 1.0; colors[146] = 0.0; // Top Right corner


															 //t48 v15
	vertices[147] = 1.0; vertices[148] = 0.0; vertices[149] = 0.0; // Top Right corner
	colors[147] = 0.0; colors[148] = 1.0; colors[149] = 0.0; // Top Right corner

															 //t49 v17
	vertices[150] = 0.0; vertices[151] = 0.0; vertices[152] = -2.0; // Top Right corner
	colors[150] = 0.0; colors[151] = 1.0; colors[152] = 0.0; // Top Right corner
															 //t50 v38
	vertices[153] = 0.0; vertices[154] = 0.0; vertices[155] = 0.0; // Top Right corner
	colors[153] = 0.0; colors[154] = 1.0; colors[155] = 0.0; // Top Right corner
															 //t51 v39

	glGenVertexArrays(2, &vaoID[0]);
	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 156 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 156 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	delete[] vertices;

	return 1;
}

/*!
ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
*/
unsigned int createTriangleModel(void)
{
	// use the vertex array object vaoID[1] for this model representation
	float* vertices = new float[273];  // Vertices for our square
	float *colors = new float[273]; // Colors for our vertices

	vertices[0] = 1.0; vertices[1] = 0.0; vertices[2] = 0.0; // Bottom left corner
	colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner

	vertices[3] = 1.0; vertices[4] = 1.0; vertices[5] = 0.0; // Top left corner
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 1.0; // Top left corner

	vertices[6] = 0.0; vertices[7] = 1.0; vertices[8] = 0.0; // Top Right corner
	colors[6] = 0.0; colors[7] = 0.0; colors[8] = 1.0; // Top Right corner
													   //tri1



	vertices[9] = 0.0; vertices[10] = 0.0; vertices[11] = 0.0; // Bottom right corner
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner

														


	vertices[12] = 1.0; vertices[13] = 0.0; vertices[14] = 0.0; // Bottom left corner
	colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner
														  

	vertices[15] = 0.0; vertices[16] = 1.0; vertices[17] = 0.0; // Top Right corner
	colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner
														  //tri2
	vertices[18] = 1.0; vertices[19] = 0.0; vertices[20] = 0.0; // Top Right corner
	colors[18] = 0.0; colors[19] = 1.0; colors[20] = 0.0; // Top Right corner
														  
	vertices[21] = 1.0; vertices[22] = 1.0; vertices[23] = 0.0; // Top Right corner
	colors[21] = 0.0; colors[22] = 1.0; colors[23] = 0.0; // Top Right corner
														  
	vertices[24] = 1.0; vertices[25] = 0.0; vertices[26] = -2.0; // Top Right corner
	colors[24] = 0.0; colors[25] = 1.0; colors[26] = 0.0; // Top Right corner
														  //tri3
	vertices[27] = 1.0; vertices[28] = 0.0; vertices[29] = -2.0; // Top Right corner
	colors[27] = 0.0; colors[28] = 0.0; colors[29] = 1.0; // Top Right corner
														 
	vertices[30] = 1.0; vertices[31] = 1.0; vertices[32] = -2.0; // Top Right corner
	colors[30] = 0.0; colors[31] = 1.0; colors[32] = 0.0; // Top Right corner
														  
	vertices[33] = 1.0; vertices[34] = 1.0; vertices[35] = 0.0; // Top Right corner
	colors[33] = 0.0; colors[34] = 1.0; colors[35] = 0.0; // Top Right corner
														  //tri4
	vertices[36] = 1.0; vertices[37] = 1.0; vertices[38] = 0.0; // Top Right corner
	colors[36] = 0.0; colors[37] = 1.0; colors[38] = 0.0; // Top Right corner
														  

	vertices[39] = 1.0; vertices[40] = 1.0; vertices[41] = -2.0; // Top Right corner
	colors[39] = 0.0; colors[40] = 1.0; colors[41] = 0.0; // Top Right corner

														  

	vertices[42] = 0.0; vertices[43] = 1.0; vertices[44] = 0.0; // Top Right corner
	colors[42] = 0.0; colors[43] = 1.0; colors[44] = 0.0; // Top Right corner

														  //tri5

	vertices[45] = 1.0; vertices[46] = 1.0; vertices[47] = -2.0; // Top Right corner
	colors[45] = 0.0; colors[46] = 1.0; colors[47] = 0.0; // Top Right corner

														  
	vertices[48] = 0.0; vertices[49] = 1.0; vertices[50] = -2.0; // Top Right corner
	colors[48] = 0.0; colors[49] = 1.0; colors[50] = 0.0; // Top Right corner

														  
	vertices[51] = 0.0; vertices[52] = 1.0; vertices[53] = 0.0; // Top Right corner
	colors[51] = 0.0; colors[52] = 1.0; colors[53] = 0.0; // Top Right corner

														  //tri6

	vertices[54] = 0.0; vertices[55] = 1.0; vertices[56] = -2.0; // Top Right corner
	colors[54] = 0.0; colors[55] = 1.0; colors[56] = 0.0; // Top Right corner




	vertices[57] = 0.0; vertices[58] = 0.0; vertices[59] = 0.0; // Top Right corner
	colors[57] = 0.0; colors[58] = 1.0; colors[59] = 0.0; // Top Right corner

														  

	vertices[60] = 0.0; vertices[61] = 0.0; vertices[62] = -2.0; // Top Right corner
	colors[60] = 0.0; colors[61] = 1.0; colors[62] = 0.0; // Top Right corner

														  //tri7

	vertices[63] = 0.0; vertices[64] = 1.0; vertices[65] = -2.0; // Top Right corner
	colors[63] = 0.0; colors[64] = 1.0; colors[65] = 0.0; // Top Right corner

														 
	vertices[66] = 0.0; vertices[67] = 1.0; vertices[68] = 0.0; // Top Right corner
	colors[66] = 0.0; colors[67] = 1.0; colors[68] = 0.0; // Top Right corner

														 
	vertices[69] = 0.0; vertices[70] = 0.0; vertices[71] = 0.0; // Top Right corner
	colors[69] = 0.0; colors[70] = 1.0; colors[71] = 0.0; // Top Right corner
															//tri8

														  
	vertices[72] = 1.0; vertices[73] = 0.0; vertices[74] = -2.0; // Top Right corner
	colors[72] = 0.0; colors[73] = 1.0; colors[74] = 0.0; // Top Right corner
														  

	vertices[75] = 1.0; vertices[76] = 0.0; vertices[77] = 0.0; // Top Right corner
	colors[75] = 0.0; colors[76] = 1.0; colors[77] = 0.0; // Top Right corner

														  

	vertices[78] = 0.0; vertices[79] = 0.0; vertices[80] = 0.0; // Top Right corner
	colors[78] = 0.0; colors[79] = 1.0; colors[80] = 0.0; // Top Right corner

														  //tri9

	vertices[81] = 1.0; vertices[82] = 0.0; vertices[83] = -2.0; // Top Right corner
	colors[81] = 0.0; colors[82] = 1.0; colors[83] = 0.0; // Top Right corner

														  

	vertices[84] = 0.0; vertices[85] = 0.0; vertices[86] = 0.0; // Top Right corner
	colors[84] = 0.0; colors[85] = 1.0; colors[86] = 0.0; // Top Right corner

														  
	vertices[87] = 0.0; vertices[88] = 0.0; vertices[89] = -2.0; // Top Right corner
	colors[87] = 0.0; colors[88] = 1.0; colors[89] = 0.0; // Top Right corner
														  //tri10

	vertices[90] = 0.0; vertices[91] = 1.0; vertices[92] = -2.0; // Top Right corner
	colors[90] = 0.0; colors[91] = 1.0; colors[92] = 0.0; // Top Right corner

														  
	vertices[93] = 1.0; vertices[94] = 1.0; vertices[95] = -2.0; // Top Right corner
	colors[93] = 0.0; colors[94] = 1.0; colors[95] = 0.0; // Top Right corner

														  

	vertices[96] = 1.0; vertices[97] = 2.0; vertices[98] = -2.0; // Top Right corner
	colors[96] = 0.0; colors[97] = 1.0; colors[98] = 0.0; // Top Right corner

														  //tri11

	vertices[99] = 0.0; vertices[100] = 1.0; vertices[101] = -2.0; // Top Right corner
	colors[99] = 0.0; colors[100] = 1.0; colors[101] = 0.0; // Top Right corner

															

	vertices[102] = 1.0; vertices[103] = 2.0; vertices[104] = -2.0; // Top Right corner
	colors[102] = 0.0; colors[103] = 1.0; colors[104] = 0.0; // Top Right corner

															 

	vertices[102] = 0.0; vertices[103] = 2.0; vertices[104] = -2.0; // Top Right corner
	colors[102] = 0.0; colors[103] = 1.0; colors[104] = 0.0; // Top Right corner

															 //tri12
	vertices[105] = 1.0; vertices[106] = 2.0; vertices[107] = -2.0; // Top Right corner
	colors[105] = 0.0; colors[106] = 1.0; colors[107] = 0.0; // Top Right corner

															

	vertices[108] = 0.0; vertices[109] = 2.0; vertices[110] = -2.0; // Top Right corner
	colors[108] = 0.0; colors[109] = 1.0; colors[110] = 0.0; // Top Right corner
															
	vertices[111] = 1.0; vertices[112] = 2.0; vertices[113] = -3.0; // Top Right corner
	colors[111] = 0.0; colors[112] = 1.0; colors[113] = 0.0; // Top Right corner
														//tri13
															 
	vertices[114] = 0.0; vertices[115] = 2.0; vertices[116] = -2.0; // Top Right corner
	colors[114] = 0.0; colors[115] = 1.0; colors[116] = 0.0; // Top Right corner

															 //t38 v33
	vertices[117] = 1.0; vertices[118] = 2.0; vertices[119] = -3.0; // Top Right corner
	colors[117] = 0.0; colors[118] = 1.0; colors[119] = 0.0; // Top Right corner
															 //t39 v32
	vertices[120] = 0.0; vertices[121] = 2.0; vertices[122] = -3.0; // Top Right corner
	colors[120] = 0.0; colors[121] = 1.0; colors[122] = 0.0; // Top Right corner
															 //tri14

	vertices[123] = 0.0; vertices[124] = 1.0; vertices[125] = -2.0; // Top Right corner
	colors[123] = 0.0; colors[124] = 1.0; colors[125] = 0.0; // Top Right corner
															 
	vertices[126] = 1.0; vertices[127] = 1.0; vertices[128] = -3.0; // Top Right corner
	colors[126] = 0.0; colors[127] = 1.0; colors[128] = 0.0; // Top Right corner

															 
	vertices[129] = 0.0; vertices[130] = 2.0; vertices[131] = -3.0; // Top Right corner
	colors[129] = 0.0; colors[130] = 1.0; colors[131] = 0.0; // Top Right corner
															 //tri15

	vertices[132] = 0.0; vertices[133] = 2.0; vertices[134] = -2.0; // Top Right corner
	colors[132] = 0.0; colors[133] = 1.0; colors[134] = 0.0; // Top Right corner

	vertices[135] = 0.0; vertices[136] = 2.0; vertices[137] = -3.0; // Top Right corner
	colors[135] = 0.0; colors[136] = 1.0; colors[137] = 0.0; // Top Right corner

															 
	vertices[138] = 0.0; vertices[139] = 1.0; vertices[140] = -2.0; // Top Right corner
	colors[138] = 0.0; colors[139] = 1.0; colors[140] = 0.0; // Top Right corner

															 //tri16

	vertices[141] = 0.0; vertices[142] = 0.0; vertices[143] = -2.0; // Top Right corner
	colors[141] = 0.0; colors[142] = 1.0; colors[143] = 0.0; // Top Right corner


															 
	vertices[144] = 0.0; vertices[145] = 0.0; vertices[146] = -3.0; // Top Right corner
	colors[144] = 0.0; colors[145] = 1.0; colors[146] = 0.0; // Top Right corner


															 
	vertices[147] = 0.0; vertices[148] = 1.0; vertices[149] = -3.0; // Top Right corner
	colors[147] = 0.0; colors[148] = 1.0; colors[149] = 0.0; // Top Right corner

															 //tri17
	vertices[150] = 0.0; vertices[151] = 0.0; vertices[152] = -2.0; // Top Right corner
	colors[150] = 0.0; colors[151] = 1.0; colors[152] = 0.0; // Top Right corner
															 
	vertices[153] = 0.0; vertices[154] = 1.0; vertices[155] = -2.0; // Top Right corner
	colors[153] = 0.0; colors[154] = 1.0; colors[155] = 0.0; // Top Right corner
															
	vertices[153] = 0.0; vertices[154] = 1.0; vertices[155] = -3.0; // Top Right corner
	colors[153] = 0.0; colors[154] = 1.0; colors[155] = 0.0; 
	//tri18

	vertices[156] = 0.0; vertices[157] = 0.0; vertices[158] = -3.0; // Top Right corner
	colors[156] = 0.0; colors[157] = 1.0; colors[158] = 0.0;
	
	vertices[159] = 1.0; vertices[160] = 0.0; vertices[161] = -3.0; // Top Right corner
	colors[159] = 0.0; colors[160] = 1.0; colors[161] = 0.0;

	vertices[162] = 1.0; vertices[163] = 2.0; vertices[164] = -3.0; // Top Right corner
	colors[162] = 0.0; colors[163] = 1.0; colors[164] = 0.0;

	//tri19

	vertices[165] = 0.0; vertices[166] = 0.0; vertices[167] = -3.0; // Top Right corner
	colors[165] = 0.0; colors[166] = 1.0; colors[167] = 0.0;

	vertices[168] = 1.0; vertices[169] = 2.0; vertices[170] = -3.0; // Top Right corner
	colors[168] = 1.0; colors[169] = 1.0; colors[170] = 0.0;

	vertices[171] = 0.0; vertices[172] = 2.0; vertices[173] = -3.0; // Top Right corner
	colors[171] = 1.0; colors[172] = 1.0; colors[173] = 0.0;

	//tri20
	vertices[174] = 3.0; vertices[175] = 1.0; vertices[176] = -3.0; // Top Right corner
	colors[174] = 1.0; colors[175] = 1.0; colors[176] = 0.0;

	vertices[177] = 3.0; vertices[178] = 1.0; vertices[179] = -2.0; // Top Right corner
	colors[177] = 1.0; colors[178] = 1.0; colors[179] = 0.0;

	vertices[180] = 1.0; vertices[181] = 2.0; vertices[182] = -2.0; // Top Right corner
	colors[180] = 1.0; colors[181] = 1.0; colors[182] = 0.0;

	//tri21
	vertices[183] = 1.0; vertices[184] = 2.0; vertices[185] = -2.0; // Top Right corner
	colors[183] = 1.0; colors[184] = 1.0; colors[185] = 0.0;
	
	vertices[186] = 1.0; vertices[187] = 2.0; vertices[188] = -3.0; // Top Right corner
	colors[186] = 1.0; colors[187] = 1.0; colors[188] = 0.0;

	vertices[189] = 3.0; vertices[190] = 1.0; vertices[191] = -3.0; // Top Right corner
	colors[189] = 1.0; colors[190] = 1.0; colors[191] = 0.0;

	//tri22

	vertices[192] = 3.0; vertices[193] = 1.0; vertices[194] = -2.0; // Top Right corner
	colors[192] = 1.0; colors[193] = 1.0; colors[194] = 0.0;

	vertices[195] = 1.0; vertices[196] = 2.0; vertices[197] = -2.0; // Top Right corner
	colors[195] = 1.0; colors[196] = 1.0; colors[197] = 0.0;

	vertices[198] = 1.0; vertices[199] = 1.0; vertices[200] = -2.0; // Top Right corner
	colors[198] = 1.0; colors[199] = 1.0; colors[200] = 0.0;

	//tri23

	vertices[201] = 3.0; vertices[202] = 1.0; vertices[203] = -3.0; // Top Right corner
	colors[192] = 1.0; colors[193] = 1.0; colors[194] = 0.0;

	vertices[204] = 1.0; vertices[205] = 2.0; vertices[206] = -3.0; // Top Right corner
	colors[195] = 1.0; colors[196] = 1.0; colors[197] = 0.0;

	vertices[207] = 1.0; vertices[208] = 1.0; vertices[209] = -3.0; // Top Right corner
	colors[198] = 1.0; colors[199] = 1.0; colors[200] = 0.0;

	//tri24


	vertices[201] = 1.0; vertices[202] = 0.0; vertices[203] = -2.0; // Top Right corner
	colors[201] = 1.0; colors[202] = 1.0; colors[203] = 0.0;

	vertices[204] = 3.0; vertices[205] = 0.0; vertices[206] = -2.0; // Top Right corner
	colors[204] = 1.0; colors[205] = 1.0; colors[206] = 0.0;

	vertices[207] = 3.0; vertices[208] = 1.0; vertices[209] = -2.0; // Top Right corner
	colors[207] = 1.0; colors[208] = 1.0; colors[209] = 0.0;

	//tri25

	vertices[210] = 1.0; vertices[211] = 0.0; vertices[212] = -2.0; // Top Right corner
	colors[210] = 1.0; colors[211] = 1.0; colors[212] = 0.0;

	vertices[213] = 1.0; vertices[214] = 1.0; vertices[215] = -2.0; // Top Right corner
	colors[213] = 1.0; colors[214] = 1.0; colors[215] = 0.0;

	vertices[216] = 3.0; vertices[217] = 1.0; vertices[218] = -2.0; // Top Right corner
	colors[216] = 1.0; colors[217] = 1.0; colors[218] = 0.0;


	//tri26

	vertices[219] = 3.0; vertices[220] = 0.0; vertices[221] = -2.0; // Top Right corner
	colors[219] = 1.0; colors[220] = 1.0; colors[221] = 0.0;

	vertices[222] = 3.0; vertices[223] = 0.0; vertices[224] = -3.0; // Top Right corner
	colors[222] = 1.0; colors[223] = 1.0; colors[224] = 0.0;

	vertices[225] = 3.0; vertices[226] = 1.0; vertices[227] = -3.0; // Top Right corner
	colors[225] = 1.0; colors[226] = 1.0; colors[227] = 0.0;

	//tri27

	vertices[228] = 3.0; vertices[229] = 0.0; vertices[230] = -2.0; // Top Right corner
	colors[228] = 1.0; colors[229] = 1.0; colors[230] = 0.0;

	vertices[231] = 3.0; vertices[232] = 1.0; vertices[233] = -3.0; // Top Right corner
	colors[231] = 1.0; colors[232] = 1.0; colors[233] = 0.0;

	vertices[234] = 3.0; vertices[235] = 1.0; vertices[236] = -2.0; // Top Right corner
	colors[234] = 1.0; colors[235] = 1.0; colors[236] = 0.0;

	//tri28

	vertices[237] = 3.0; vertices[238] = 0.0; vertices[239] = -3.0; // Top Right corner
	colors[237] = 1.0; colors[238] = 1.0; colors[239] = 0.0;

	vertices[240] = 3.0; vertices[241] = 1.0; vertices[242] = -3.0; // Top Right corner
	colors[240] = 1.0; colors[241] = 1.0; colors[242] = 0.0;

	vertices[243] = 1.0; vertices[244] = 0.0; vertices[245] = -3.0; // Top Right corner
	colors[243] = 1.0; colors[244] = 1.0; colors[245] = 0.0;
	//tri29
	vertices[246] = 1.0; vertices[247] = 0.0; vertices[248] = -3.0; // Top Right corner
	colors[246] = 1.0; colors[247] = 1.0; colors[248] = 0.0;

	vertices[249] = 3.0; vertices[250] = 1.0; vertices[251] = -3.0; // Top Right corner
	colors[249] = 1.0; colors[250] = 1.0; colors[251] = 0.0;

	vertices[252] = 1.0; vertices[253] = 1.0; vertices[254] = -3.0; // Top Right corner
	colors[252] = 1.0; colors[253] = 1.0; colors[254] = 0.0;

	//tri30

	vertices[255] = 0.0; vertices[256] = 0.0; vertices[257] = -2.0; // Top Right corner
	colors[255] = 1.0; colors[256] = 1.0; colors[257] = 0.0;

	vertices[258] = 3.0; vertices[259] = 0.0; vertices[260] = -2.0; // Top Right corner
	colors[258] = 1.0; colors[259] = 1.0; colors[260] = 0.0;

	vertices[261] = 3.0; vertices[262] = 0.0; vertices[263] = -3.0; // Top Right corner
	colors[261] = 1.0; colors[262] = 1.0; colors[263] = 0.0;

	//tri31

	vertices[264] = 0.0; vertices[265] = 0.0; vertices[266] = -2.0; // Top Right corner
	colors[264] = 1.0; colors[265] = 1.0; colors[266] = 0.0;

	vertices[267] = 3.0; vertices[268] = 0.0; vertices[269] = -2.0; // Top Right corner
	colors[267] = 1.0; colors[268] = 1.0; colors[269] = 0.0;

	vertices[270] = 3.0; vertices[271] = 0.0; vertices[272] = -3.0; // Top Right corner
	colors[270] = 1.0; colors[271] = 1.0; colors[272] = 0.0;
	//tri32

	glGenVertexArrays(2, &vaoID[0]);

	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);



	glBufferData(GL_ARRAY_BUFFER, 273 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);



	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);


	glBufferData(GL_ARRAY_BUFFER, 273 * sizeof(GLfloat), colors, GL_STATIC_DRAW);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete[] vertices;

	//return 1;

	//TODO:
	//vaoID[1];

	return 1;
}



/*!
ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
*/
void renderTriangleStripModel(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
	// Draw the triangles
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 52);


	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}



/*!
ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
*/
void renderTriangleModel(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	glDrawArrays(GL_TRIANGLES, 0, 91);


	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY POLYGONS YOU HAVE.
	// COMPLETE THE LINE
	// Draw the triangles
	//   glDrawArrays(GL_POLYGON, ? , ?);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
This function creates the two models
*/
void setupScene(void) {
	createTriangleModel();
	//createTriangleStripModel();
	//renderTriangleStripModel();

}




int main(int argc, const char * argv[])
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();

	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// The Shader Program starts here

	// Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
	// Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	static const string vertex_code = vs_string;
	static const char * vs_source = vertex_code.c_str();

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fragment_code = fs_string;
	static const char * fs_source = fragment_code.c_str();

	// This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
	program = glCreateProgram();

	// We create a shader with our fragment shader source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	// We create a shader with our vertex shader source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	// We'll attach our two compiled shaders to the OpenGL program.
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// We'll specify that we want to use this program that we've attached the shaders to.
	glUseProgram(program);

	//// The Shader Program ends here
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/// IGNORE THE NEXT PART OF THIS CODE
	/// IGNORE THE NEXT PART OF THIS CODE
	/// IGNORE THE NEXT PART OF THIS CODE
	// It controls the virtual camera

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
	viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader


	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


																			  //// The Shader Program ends here
																			  //// START TO READ AGAIN
																			  //// START TO READ AGAIN
																			  //// START TO READ AGAIN
																			  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_Color");



	// this creates the scene
	setupScene();

	int i = 0;

	// Enable depth test
	// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
	glEnable(GL_DEPTH_TEST);

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		// this draws the coordinate system
		coordinate_system_renderer->draw();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// This generate the object
		// Enable the shader program
		glUseProgram(program);

		// this changes the camera location
		glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader


																				  // This moves the model to the right
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


																				  // This line renders your triangle strip model
		//renderTriangleStripModel();


		// This moves the model to the left
		//	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
		//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

		// This line renders your Ppolygon model
			renderTriangleModel();


		// disable the shader program
		glUseProgram(0);


		//// This generate the object
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Swap the buffers so that what we drew will appear on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// delete the coordinate system object
	delete coordinate_system_renderer;

	// Program clean up when the window gets closed.
	glDeleteVertexArrays(2, vaoID);
	glDeleteProgram(program);
}

