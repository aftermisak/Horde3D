// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2016 Nicolas Schulz and Horde3D team
//
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/legal/epl-v10.html
//
// *************************************************************************************************

#ifndef _egRenderer_H_
#define _egRenderer_H_

#include "egPrerequisites.h"
#include "egRendererBase.h"
#include "egPrimitives.h"
#include "egModel.h"
#include <vector>
#include <algorithm>


namespace Horde3D {

class MaterialResource;
class LightNode;
class CameraNode;
struct ShaderContext;

const uint32 MaxNumOverlayVerts = 2048;
const uint32 ParticlesPerBatch = 64;	// Warning: The GPU must have enough registers
const uint32 QuadIndexBufCount = MaxNumOverlayVerts * 6;

#define OCCPROXYLIST_RENDERABLES 0
#define OCCPROXYLIST_LIGHTS 1

extern const char *vsOccBox;
extern const char *fsOccBox;
	

// =================================================================================================
// Renderer
// =================================================================================================

typedef void (*RenderFunc)( uint32 firstItem, uint32 lastItem, const std::string &shaderContext,
                            const std::string &theClass, bool debugView, const Frustum *frust1,
                            const Frustum *frust2, RenderingOrder::List order, int occSet );

struct RenderFuncListItem
{
	int         nodeType;
	RenderFunc  renderFunc;
};

struct RenderBackendType
{
	enum List
	{
		OpenGL2 = 2,
		OpenGL4 = 4,
		OpenGLES = 8
	};
};

// =================================================================================================

struct OverlayBatch
{
	PMaterialResource  materialRes;
	uint32             firstVert, vertCount;
	float              colRGBA[4];
	int                flags;
	
	OverlayBatch() {}

	OverlayBatch( uint32 firstVert, uint32 vertCount, float *col, MaterialResource *materialRes, int flags ) :
		materialRes( materialRes ), firstVert( firstVert ), vertCount( vertCount ), flags( flags )
	{
		colRGBA[0] = col[0]; colRGBA[1] = col[1]; colRGBA[2] = col[2]; colRGBA[3] = col[3];
	}
 };

struct OverlayVert
{
	float  x, y;  // Position
	float  u, v;  // Texture coordinates
};


struct ParticleVert
{
	float  u, v;         // Texture coordinates
	float  index;        // Index in property array

	ParticleVert() {}

	ParticleVert( float u, float v ):
		u( u ), v( v ), index( 0 )
	{
	}
};

// =================================================================================================

struct OccProxy
{
	Vec3f   bbMin, bbMax;
	uint32  queryObj;

	OccProxy() {}
	OccProxy( const Vec3f &bbMin, const Vec3f &bbMax, uint32 queryObj ) :
		bbMin( bbMin ), bbMax( bbMax ), queryObj( queryObj )
	{
	}
};

struct PipeSamplerBinding
{
	char    sampler[64];
	uint32  rbObj;
	uint32  bufIndex;
};

struct DefaultVertexLayouts
{
	enum List
	{
		Position = 0,
		Particle,
		Model,
		Overlay
	};
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void registerRenderFunc( int nodeType, RenderFunc rf );

	inline RenderDeviceInterface *getRenderDevice() const { return _renderDevice; }

	unsigned char *useScratchBuf( uint32 minSize, uint32 alignment );
	/*
		this->createRenderDevice 创建渲染接口实例
		检查对一些渲染技术的支持，比如 floating point texture，  No non-Power-of-two texture，multisampling
		_renderDevice->registerVertexLayout 注册顶点属性信息 attribsPosOnly   attribsOverlay  attribsModel  attribsParticle
		编译链接默认着色器，并保存期uniform等信息
		创建shadow map的renderbuffer
		创建默认的 shadow map texture
		创建用来绘制overlay quads的 vertex array object ->_overlayGeo
			创建index buffer  -> _quadIdxBuf,并且设置相应的值
			创建vertex buffer -> _overlayVB
			将上面的信息注册给_overlayGeo
		同样创建 粒子系统用的 vertex arra object ->_particleGeo
			...相关顶点信息
		this->createPrimitives 创建了一堆东西
		this->useScratchBuf
		Reset states by this->finishRendering

	*/
	bool init( RenderBackendType::List type );
	// only call _renderDevice->initStates();
	void initStates();
	
	//用 _cubeGeo 绘制一个正交正六面体
	void drawAABB( const Vec3f &bbMin, const Vec3f &bbMax );
	//用 _sphereGeo 绘制一个椭球
	void drawSphere( const Vec3f &pos, float radius );
	//用 _coneGeo绘制一个椎体
	void drawCone( float height, float fov, const Matrix4f &transMat );
	
	//创建新的program和一套shader集合, 并且获取兵保存一堆预定义uniform信息
	bool createShaderComb( ShaderCombination &sc, const char *vertexShader, const char *fragmentShader, const char *geometryShader,
						   const char *tessControlShader, const char *tessEvaluationShader, const char *computeShader );
	//释放program
	void releaseShaderComb( ShaderCombination &sc );
	//使用一个新的program
	void setShaderComb( ShaderCombination *sc );
	//提交一堆预定义uniform(如果标记发生了改变需要提交)
	void commitGeneralUniforms();
	//设置一个新的材质 this->setMaterialRec
	bool setMaterial( MaterialResource *materialRes, const std::string &shaderContext );
	
	bool createShadowRB( uint32 width, uint32 height );
	void releaseShadowRB();

	int registerOccSet();
	void unregisterOccSet( int occSet );
	void drawOccProxies( uint32 list );
	void pushOccProxy( uint32 list, const Vec3f &bbMin, const Vec3f &bbMax, uint32 queryObj )
		{ _occProxies[list].push_back( OccProxy( bbMin, bbMax, queryObj ) ); }
	
	void showOverlays( const float *verts, uint32 vertCount, float *colRGBA,
	                   MaterialResource *matRes, int flags );
	void clearOverlays();
	
	static void drawMeshes( uint32 firstItem, uint32 lastItem, const std::string &shaderContext, const std::string &theClass,
		bool debugView, const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
	static void drawParticles( uint32 firstItem, uint32 lastItem, const std::string &shaderContext, const std::string &theClass,
		bool debugView, const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
	static void drawComputeResults( uint32 firstItem, uint32 lastItem, const std::string &shaderContext, const std::string &theClass, 
									bool debugView, const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );

	void render( CameraNode *camNode );
	void finalizeFrame();

	void dispatchCompute( MaterialResource *materialRes, const std::string &context, uint32 groups_x, uint32 groups_y, uint32 groups_z );

	uint32 getFrameID() const { return _frameID; }
	ShaderCombination *getCurShader() const { return _curShader; }
	CameraNode *getCurCamera() const { return _curCamera; }
	uint32 getQuadIdxBuf() const { return _quadIdxBuf; }
	uint32 getParticleVBO() const { return _particleVBO; }
	uint32 getParticleGeometry() const { return _particleGeo; }
	uint32 getDefaultVertexLayout( DefaultVertexLayouts::List vl ) const;

	int getRenderDeviceType() { return _renderDeviceType; }

protected:
	void setupViewMatrices( const Matrix4f &viewMat, const Matrix4f &projMat );
	
	void createPrimitives();
	
	bool setMaterialRec( MaterialResource *materialRes, const std::string &shaderContext, ShaderResource *shaderRes );
	
	void setupShadowMap( bool noShadows );
	Matrix4f calcCropMatrix( const Frustum &frustSlice, const Vec3f lightPos, const Matrix4f &lightViewProjMat );
	void updateShadowMap();

	void drawOverlays( const std::string &shaderContext );

	void bindPipeBuffer( uint32 rbObj, const std::string &sampler, uint32 bufIndex );
	void clear( bool depth, bool buf0, bool buf1, bool buf2, bool buf3, float r, float g, float b, float a );
	void drawFSQuad( Resource *matRes, const std::string &shaderContext );
	void drawGeometry( const std::string &shaderContext, const std::string &theClass,
	                   RenderingOrder::List order, int occSet );
	void drawLightGeometry( const std::string &shaderContext, const std::string &theClass,
	                        bool noShadows, RenderingOrder::List order, int occSet );
	void drawLightShapes( const std::string &shaderContext, bool noShadows, int occSet );
	
	void drawRenderables( const std::string &shaderContext, const std::string &theClass, bool debugView,
		const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
	
	void renderDebugView();
	void finishRendering();

	RenderDeviceInterface *createRenderDevice( int type );

	void		  releaseRenderDevice();
protected:
	
	RenderDeviceInterface		       *_renderDevice;

	std::vector< RenderFuncListItem >  _renderFuncRegistry;//保存着一些回调监听，在某个渲染的函数中，会回调
	
	unsigned char                      *_scratchBuf;//一个堆上的buff，但不是Render内部用，提供给接口给外不用
	uint32                             _scratchBufSize;//上面那个buff的当前容量

	//视图变换，投影变化等相关矩阵
	Matrix4f                           _viewMat, _viewMatInv, _projMat, _viewProjMat, _viewProjMatInv;
	
	std::vector< PipeSamplerBinding >  _pipeSamplerBindings;
	std::vector< char >                _occSets;  // Actually bool
	std::vector< OccProxy >            _occProxies[2];  // 0: renderables, 1: lights
	
	std::vector< OverlayBatch >        _overlayBatches;//需要绘制的overlay元素，在添加新overlay绘制元素时，通过能不能和当前最后一个信息进行batch来实现batch draw
	OverlayVert                        *_overlayVerts;//overlay的顶点数据，预先分配了一个很大的尺寸
	uint32							   _overlayGeo;//overlay的vertex array object
	uint32                             _overlayVB;//overlay的vertex buffer
	
	// standard geometry
	uint32								_particleGeo;
	uint32								_cubeGeo;
	uint32								_sphereGeo;
	uint32								_coneGeo;
	uint32								_FSPolyGeo;

	uint32                             _shadowRB;//阴影render buffer
	uint32                             _frameID;//计数，每一帧结束后，自增1
	uint32                             _defShadowMap;//默认的shadow map texture
	uint32                             _quadIdxBuf;//particle和overlay公用的index buffer
	uint32                             _particleVBO;//particle的vertex buffer object
	MaterialResource                   *_curStageMatLink;
	CameraNode                         *_curCamera;
	LightNode                          *_curLight;
	ShaderCombination                  *_curShader;
	RenderTarget                       *_curRenderTarget;
	uint32                             _curShaderUpdateStamp;
	
	uint32                             _maxAnisoMask;//更阴影类型相关
	float                              _smSize;//shadow mapp size
	float                              _splitPlanes[5];//和阴影计算相关
	Matrix4f                           _lightMats[4];//和阴影计算相关

	uint32                             _vlPosOnly, _vlOverlay, _vlModel, _vlParticle;
	ShaderCombination                  _defColorShader;
	int                                _defColShader_color;  // Uniform location
	
	uint32                             _vbCube, _ibCube, _vbSphere, _ibSphere;
	uint32                             _vbCone, _ibCone, _vbFSPoly;
	
	int									_renderDeviceType;
	
};

}
#endif // _egRenderer_H_
