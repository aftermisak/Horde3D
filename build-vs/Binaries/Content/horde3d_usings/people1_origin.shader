[[FX]]

// Samplers
sampler2D albedoMap;

// Contexts
OpenGL4
{
	context OVERLAY
	{
		VertexShader = compile GLSL VS_DRAW_ORIGIN_GL4;
		PixelShader = compile GLSL FS_DRAW_ORIGIN_GL4;
		ZWriteEnable = true; //画原图时写入zbuffer
		BlendMode = Blend;

	}
}


///////////////////////////////////////////////////////////////////////////////////
[[VS_DRAW_ORIGIN_GL4]]
layout( location = 0 ) in vec2 vertPos;
layout( location = 1 ) in vec2 texCoords0;
out vec2 texCoords;

uniform mat4 projMat;
void main( void )
{
	texCoords = vec2( texCoords0.s, -texCoords0.t ); 
	gl_Position = projMat * vec4( vertPos.x, vertPos.y, 1, 1 );
}

///////////////////////////////////////////////////////////////////////////////////
[[FS_DRAW_ORIGIN_GL4]]
uniform vec4 olayColor;
uniform sampler2D albedoMap;
in vec2 texCoords;

out vec4 fragColor;

void main( void )
{
	vec4 albedo = texture2D( albedoMap, texCoords );
	if( albedo.a <= 0.0 ){ discard; }
	fragColor = albedo * olayColor;
}
