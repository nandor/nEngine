/**
	@file Shader.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "Lua.hpp"
#include "File.hpp"
#include "Error.hpp"
#include "Resource.hpp"
#include "Resources.hpp"

namespace nEngine {
    /**
        Shader types
    */
    enum ShaderType {
        /** Vertex shader */
        SHADER_VERTEX,
        /** Fragment shader */
        SHADER_FRAGMENT,
        /** Geometry shader - not implemented */
        SHADER_GEOMETRY
    };

    /**
        Shader
    */
    class Shader : public Resource {
    public:

        /**
            Create a new shader
        */
        NAPI Shader(const std::string& id);

        /**
            Destroy the shader
        */
        NAPI ~Shader();

        /**
            Load a shader from a file
            @param fileName         Name of the shader file
            @param type             Type of the shader

            @return                 True on success
        */
        NAPI void load(const std::string& fileName);

        /**
            Compile the shader
            @return                 True on success
        */
        NAPI void compile();

        /**
            Return the program id
            @return                 Program id
        */
        NAPI unsigned getProgramId()
        {
            return mProgramId;
        }

		/**
			Get the names of shader sources
		*/
		NAPI std::vector<std::string> getShaderNames();

    private:

        std::vector<std::pair<std::string, unsigned> > mShaders;
        unsigned mProgramId;

    public:
        NAPI static const ResourceType sType;

        NAPI static void useProgram(const std::string& str);
        NAPI static void popProgram();
        NAPI static void clearStack();
		

		/**
			Get the position of a vertex attribute
		*/
		NAPI static int  getAttribute(const char* id);

        NAPI static void setUniformf(const char* id, int length, float* values);
		NAPI static void setUniformf(const char* id, float value);

        NAPI static void setUniformi(const char* id, int length, int* values);
		NAPI static void setUniformi(const char* id, int value);

		NAPI static void setUniformColor(const char* id, Color& c);
		NAPI static void setUniformVec2(const char* vec, Vec2& v);
    private:

        static std::vector<GLhandleARB> shaderStack;
        static unsigned currentProgram;
    };

	/**
		Register the 'Shader' lua class
	*/
	NAPI bool luaRegisterShader(lua_State* L);
};

#endif /*SHADER_HPP*/

