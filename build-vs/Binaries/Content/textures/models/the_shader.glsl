[[FX]]

// Samplers
sampler2D albedoMap;

OpenGL4
{
	context OVERLAY
	{
		VertexShader = compile GLSL VS_OVERLAY_GL4;
		GeometryShader = compile GLSL GS_OVERLAY_GL4;
		PixelShader = compile GLSL FS_OVERLAY_GL4;
		
		ZWriteEnable = false;
		BlendMode = Blend;
	}
}


[[VS_OVERLAY_GL4]]

layout( location = 0 ) in vec2 vertPos;
layout( location = 1 ) in vec2 texCoords0;
out vec2 texCoords;

uniform mat4 projMat;
void main( void )
{
	texCoords = vec2( texCoords0.s, -texCoords0.t ); 
	gl_Position = projMat * vec4( vertPos.x, vertPos.y, 1, 1 );
}

[[GS_OVERLAY_GL4]]

layout( triangles ) in;
layout( triangle_strip, max_vertices = 240 ) out;
in vec2 texCoords[];


out vec2 the_texCoords;

void main(){
	int i;
	float offset = 0;
	for( offset = 0.0f; offset < 0.1f; offset+=0.1f ){
		for( i = 0; i < gl_in.length(); i++ ){
			the_texCoords = texCoords[i];
			gl_Position = gl_in[i].gl_Position;
			gl_Position.y += offset;
			EmitVertex();
		}
		EndPrimitive();	
	}

	
}




[[FS_OVERLAY_GL4]]

uniform vec4 olayColor;
uniform sampler2D albedoMap;
in vec2 the_texCoords;

out vec4 fragColor;

void main( void )
{
	vec4 albedo = texture2D( albedoMap, the_texCoords );
	
	fragColor = albedo * olayColor;
	//fragColor = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
}