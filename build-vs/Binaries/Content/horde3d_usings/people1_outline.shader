[[FX]]

// Samplers
sampler2D albedoMap;

//Uniforms
float4 outlineColor <
> = {1.0,1.0,1.0,1.0};

// Contexts
OpenGL4
{
	context OVERLAY
	{
		VertexShader = compile GLSL VS_DRAW_OUTLINE_GL4;
		PixelShader = compile GLSL FS_DRAW_OUTLINE_GL4;
		ZWriteEnable = false; //画描边写zbuffer也没有什么实际意义
		zEnable = true;
		zFunc = GreaterEqual;
		BlendMode = Blend;	
	}
}

////////////////////////////////////////////////////////////////////////////////////////
[[VS_DRAW_OUTLINE_GL4]]
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
[[FS_DRAW_OUTLINE_GL4]]
uniform vec4 outlineColor;
uniform sampler2D albedoMap;
in vec2 texCoords;

out vec4 fragColor;
void main( void ){
	
	vec4 albedo = texture2D( albedoMap, texCoords );
	// fragColor = outlineColor;
	if(albedo.a > 0.0f){
		fragColor = outlineColor;
	}else{
		discard;
	}
	
}