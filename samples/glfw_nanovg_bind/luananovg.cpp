#include <iostream>
#include <cassert>

#include "kaguya/kaguya.hpp"
#ifdef USE_GLES2
#include <GLES2/gl2.h>
#else
#include <glad/glad.h>
#endif
#include "nanovg.h"

#ifdef USE_GLES2
#define NANOVG_GLES2_IMPLEMENTATION
#else
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"


using namespace kaguya;
namespace
{
#ifdef USE_GLES2
	NVGcontext* create(int flag)
	{
		return nvgCreateGLES2(flag);
	}
	void Delete(NVGcontext* context)
	{
		nvgDeleteGLES2(context);
	}
#else
	NVGcontext* create(int flag)
	{
		if (!gladLoadGL()) {
			printf("Something went wrong!\n");
			exit(-1);
		}
//		printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);
		return nvgCreateGL3(flag);
	}
	void Delete(NVGcontext* context)
	{
		nvgDeleteGL3(context);
	}
#endif

	void myglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glViewport(x,y,width,height);
	}
}



extern "C"
#ifdef _MSC_VER
__declspec(dllexport)
#endif
int luaopen_luananovg(lua_State *L)
{
	State state(L);
	LuaRef luananovg = state.newLib();
	
	//not nanovg but need
	luananovg["glViewport"] = function(myglViewport);
	
	
	luananovg["Create"] = function(create);
	luananovg["Delete"] = function(Delete);

	luananovg["Color"].setClass(ClassMetatable<NVGcolor>());
	

#define NANOVG_LUA_REGFUNC(FNAME) luananovg[#FNAME + 3] = function(&FNAME)
	NANOVG_LUA_REGFUNC(nvgBeginFrame);
	NANOVG_LUA_REGFUNC(nvgCancelFrame);
	NANOVG_LUA_REGFUNC(nvgEndFrame);
	NANOVG_LUA_REGFUNC(nvgRGB);
	NANOVG_LUA_REGFUNC(nvgRGBf);
	NANOVG_LUA_REGFUNC(nvgRGBA);
	NANOVG_LUA_REGFUNC(nvgRGBAf);
	NANOVG_LUA_REGFUNC(nvgLerpRGBA);
	NANOVG_LUA_REGFUNC(nvgTransRGBA);
	NANOVG_LUA_REGFUNC(nvgTransRGBAf);
	NANOVG_LUA_REGFUNC(nvgHSL);
	NANOVG_LUA_REGFUNC(nvgHSLA);
	NANOVG_LUA_REGFUNC(nvgSave);
	NANOVG_LUA_REGFUNC(nvgRestore);
	NANOVG_LUA_REGFUNC(nvgReset);
	NANOVG_LUA_REGFUNC(nvgStrokeColor);
	NANOVG_LUA_REGFUNC(nvgStrokePaint);
	NANOVG_LUA_REGFUNC(nvgFillColor);
	NANOVG_LUA_REGFUNC(nvgFillPaint);
	NANOVG_LUA_REGFUNC(nvgMiterLimit);
	NANOVG_LUA_REGFUNC(nvgStrokeWidth);
	NANOVG_LUA_REGFUNC(nvgLineCap);
	NANOVG_LUA_REGFUNC(nvgLineJoin);
	NANOVG_LUA_REGFUNC(nvgGlobalAlpha);
	NANOVG_LUA_REGFUNC(nvgResetTransform);
	NANOVG_LUA_REGFUNC(nvgTransform);
	NANOVG_LUA_REGFUNC(nvgTranslate);
	NANOVG_LUA_REGFUNC(nvgRotate);
	NANOVG_LUA_REGFUNC(nvgSkewX);
	NANOVG_LUA_REGFUNC(nvgSkewY);
	NANOVG_LUA_REGFUNC(nvgScale);
	NANOVG_LUA_REGFUNC(nvgCurrentTransform);
	NANOVG_LUA_REGFUNC(nvgTransformIdentity);
	NANOVG_LUA_REGFUNC(nvgTransformTranslate);
	NANOVG_LUA_REGFUNC(nvgTransformScale);
	NANOVG_LUA_REGFUNC(nvgTransformRotate);
	NANOVG_LUA_REGFUNC(nvgTransformSkewX);
	NANOVG_LUA_REGFUNC(nvgTransformSkewY);
	NANOVG_LUA_REGFUNC(nvgTransformMultiply);
	NANOVG_LUA_REGFUNC(nvgTransformPremultiply);
	NANOVG_LUA_REGFUNC(nvgTransformInverse);
	NANOVG_LUA_REGFUNC(nvgTransformPoint);
	NANOVG_LUA_REGFUNC(nvgDegToRad);
	NANOVG_LUA_REGFUNC(nvgRadToDeg);
	NANOVG_LUA_REGFUNC(nvgCreateImage);
	NANOVG_LUA_REGFUNC(nvgCreateImageMem);
	NANOVG_LUA_REGFUNC(nvgCreateImageRGBA);
	NANOVG_LUA_REGFUNC(nvgUpdateImage);
	NANOVG_LUA_REGFUNC(nvgImageSize);
	NANOVG_LUA_REGFUNC(nvgDeleteImage);
	NANOVG_LUA_REGFUNC(nvgLinearGradient);
	NANOVG_LUA_REGFUNC(nvgBoxGradient);
	NANOVG_LUA_REGFUNC(nvgRadialGradient);
	NANOVG_LUA_REGFUNC(nvgImagePattern);
	NANOVG_LUA_REGFUNC(nvgScissor);
	NANOVG_LUA_REGFUNC(nvgIntersectScissor);
	NANOVG_LUA_REGFUNC(nvgResetScissor);
	NANOVG_LUA_REGFUNC(nvgBeginPath);
	NANOVG_LUA_REGFUNC(nvgMoveTo);
	NANOVG_LUA_REGFUNC(nvgLineTo);
	NANOVG_LUA_REGFUNC(nvgBezierTo);
	NANOVG_LUA_REGFUNC(nvgQuadTo);
	NANOVG_LUA_REGFUNC(nvgArcTo);
	NANOVG_LUA_REGFUNC(nvgClosePath);
	NANOVG_LUA_REGFUNC(nvgPathWinding);
	NANOVG_LUA_REGFUNC(nvgArc);
	NANOVG_LUA_REGFUNC(nvgRect);
	NANOVG_LUA_REGFUNC(nvgRoundedRect);
	NANOVG_LUA_REGFUNC(nvgEllipse);
	NANOVG_LUA_REGFUNC(nvgCircle);
	NANOVG_LUA_REGFUNC(nvgFill);
	NANOVG_LUA_REGFUNC(nvgStroke);
	NANOVG_LUA_REGFUNC(nvgCreateFont);
	NANOVG_LUA_REGFUNC(nvgCreateFontMem);
	NANOVG_LUA_REGFUNC(nvgFindFont);
	NANOVG_LUA_REGFUNC(nvgFontSize);
	NANOVG_LUA_REGFUNC(nvgFontBlur);
	NANOVG_LUA_REGFUNC(nvgTextLetterSpacing);
	NANOVG_LUA_REGFUNC(nvgTextLineHeight);
	NANOVG_LUA_REGFUNC(nvgTextAlign);
	NANOVG_LUA_REGFUNC(nvgFontFaceId);
	NANOVG_LUA_REGFUNC(nvgFontFace);
	NANOVG_LUA_REGFUNC(nvgText);
	NANOVG_LUA_REGFUNC(nvgTextBox);
	NANOVG_LUA_REGFUNC(nvgTextBounds);
	NANOVG_LUA_REGFUNC(nvgTextBoxBounds);
	NANOVG_LUA_REGFUNC(nvgTextGlyphPositions);
	NANOVG_LUA_REGFUNC(nvgTextMetrics);
	NANOVG_LUA_REGFUNC(nvgTextBreakLines);


#define NANOVG_LUA_REGENUM(NAME) luananovg[#NAME + 4] =NAME
	NANOVG_LUA_REGENUM(NVG_CCW);
	NANOVG_LUA_REGENUM(NVG_CW);
	NANOVG_LUA_REGENUM(NVG_SOLID);
	NANOVG_LUA_REGENUM(NVG_HOLE);
	NANOVG_LUA_REGENUM(NVG_BUTT);
	NANOVG_LUA_REGENUM(NVG_ROUND);
	NANOVG_LUA_REGENUM(NVG_SQUARE);
	NANOVG_LUA_REGENUM(NVG_BEVEL);
	NANOVG_LUA_REGENUM(NVG_MITER);

	NANOVG_LUA_REGENUM(NVG_ALIGN_LEFT);
	NANOVG_LUA_REGENUM(NVG_ALIGN_CENTER);
	NANOVG_LUA_REGENUM(NVG_ALIGN_RIGHT);
	NANOVG_LUA_REGENUM(NVG_ALIGN_TOP);
	NANOVG_LUA_REGENUM(NVG_ALIGN_MIDDLE);
	NANOVG_LUA_REGENUM(NVG_ALIGN_BOTTOM);
	NANOVG_LUA_REGENUM(NVG_ALIGN_BASELINE);


	NANOVG_LUA_REGENUM(NVG_IMAGE_GENERATE_MIPMAPS);
	NANOVG_LUA_REGENUM(NVG_IMAGE_REPEATX);
	NANOVG_LUA_REGENUM(NVG_IMAGE_REPEATY);
	NANOVG_LUA_REGENUM(NVG_IMAGE_FLIPY);
	NANOVG_LUA_REGENUM(NVG_IMAGE_PREMULTIPLIED);

	NANOVG_LUA_REGENUM(NVG_ANTIALIAS);
	NANOVG_LUA_REGENUM(NVG_STENCIL_STROKES);
	return 1;
}
