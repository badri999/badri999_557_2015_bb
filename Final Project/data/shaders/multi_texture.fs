#version 410 core                                                 

uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture
uniform sampler2D texture_between;
in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{//texture_blend=2;
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(texture_background, pass_TexCoord);
    
    vec4 tex_color_light =  texture(texture_foreground, pass_TexCoord);
    
    vec4 tex_color_light1 = texture(texture_between, pass_TexCoord);
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {          //blend combination one (I have used this to obtain the output "compgraph.jpg")

        //color = 0.1* pass_Color + 0.4*tex_color + 1.0*tex_color_light+0.6*tex_color_light1 ;

        //blend combination two (I have used this to obtain the output "compgraph2.jpg")
         color = (tex_color_light.r)*tex_color+2*0.25*tex_color_light1;
         //color=0.1*pass_Color+1.0*tex_color+0.1*tex_color_light+0.5*tex_color_light1;
    }
    else if(texture_blend == 1)
    {
        color = tex_color * tex_color_light+tex_color_light1;
    }
    else if(texture_blend == 2)
    {
        color = (tex_color_light.r)*tex_color+tex_color_light1 ;
    }
    else
    {
        color = 0.1 * pass_Color + tex_color+tex_color_light1;
    }
    
}