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
        Shader(const std::string& id);

        /**
            Destroy the shader
        */
        ~Shader();

        /**
            Load a shader from a file
            @param fileName         Name of the shader file
            @param type             Type of the shader

            @return                 True on success
        */
        void load(const std::string& fileName);

        /**
            Compile the shader
            @return                 True on success
        */
        void compile();

        /**
            Return the program id
            @return                 Program id
        */
        unsigned getProgramId()
        {
            return mProgramId;
        }

		/**
			Register the 'Shader' lua class
		*/
		static bool luaRegister(lua_State* L);
    private:

        std::vector<unsigned> mShaders;
        unsigned mProgramId;

    public:
        static const ResourceType sType;

        static void useProgram(const std::string& str);
        static void popProgram();
        static void clearStack();

        static void setUniformf (const char* id, int length, float* values);
        static void setUniformi (const char* id, int length, int* values);
		static void setUniformColor (const char* id, Color& c);
		static void setUniformVec2 (const char* vec, Vec2& v);
    private:

        static std::vector<GLhandleARB> shaderStack;
        static unsigned currentProgram;
    };
};

#endif /*SHADER_HPP*/

