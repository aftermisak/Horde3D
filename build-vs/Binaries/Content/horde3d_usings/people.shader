[[FX]]

// Samplers
//需要在配置中访问的uniform,sampler需要也在FX段定义
sampler2D albedoMap; //TODO 不定义这个的情况下，material里面设置这个采样器居然不报错

// Contexts
OpenGL4
{

	context OVERLAY
	{
		VertexShader = compile GLSL VS_GL4;
		PixelShader = compile GLSL FS_DRAW_ORIGIN_GL4;
		ZWriteEnable = false;
		BlendMode = Blend;
	}

}


///////////////////////////////////////////////////////////////////////////////////
[[VS_GL4]]
layout( location = 0 ) in vec2 vertPos;
layout( location = 1 ) in vec2 texCoords0;
out vec2 texCoords;
out vec4 pos;
uniform mat4 projMat;
void main( void )
{
	texCoords = vec2( texCoords0.s, -texCoords0.t ); 
	gl_Position = projMat * vec4( vertPos.x, vertPos.y, 1, 1 );
	pos = gl_Position;
}

///////////////////////////////////////////////////////////////////////////////////
[[FS_DRAW_ORIGIN_GL4]]
uniform vec4 olayColor;
uniform sampler2D albedoMap;
in vec2 texCoords;
in vec4 pos;

out vec4 fragColor;
const float PI = 3.1416;
const float _2PI = PI * 2;
bool test_pass( ){
	float theta = 0.0f;
	const float dtheta = PI/8;
	const float radious = 0.01f;
	for( theta = 0.0f; theta < _2PI; theta+=dtheta ){
		vec2 aTexCords = texCoords + vec2( radious*cos(theta), radious*sin(theta) ) ;
		vec4 sample = texture2D( albedoMap, aTexCords );
		if( sample.a > 0.0f ){ return true; }
	}
	return false;
}

float tan_vec2( vec2 v2 ){
	return v2.y/v2.x;
}

void main( void )
{
	vec4 albedo = texture2D( albedoMap, texCoords );
	fragColor = albedo * olayColor;
	if( fragColor.a <= 0.0f ){
		if(test_pass()){

			float theta = atan( tan_vec2(vec2(pos) - vec2( 0.4, 0.4 )) );
			float green = abs(sin( theta ));
			fragColor = vec4(1.0f,0.0f,0.0f,green);
		}else{
			discard;
		}
	}
}
