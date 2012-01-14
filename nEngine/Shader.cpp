/**
	@file Shader.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Shader.hpp"

namespace nEngine {
	unsigned Shader::currentProgram;
	std::vector<unsigned> Shader::shaderStack;
	const ResourceType Shader::sType = RESOURCE_SHADER;

	std::regex fragRegex(".+.frag$");
	std::regex vertRegex(".+.vert$");

	// ------------------------------------------------------------------
	Shader::Shader(const std::string& id)
		:Resource(id, RESOURCE_SHADER)
	{
		setUnloadable(false);
		mProgramId = glCreateProgramObjectARB();
	}

	// ------------------------------------------------------------------
	Shader::~Shader()
	{
		for (std::vector<std::pair<std::string, unsigned> >::iterator it = mShaders.begin(); it != mShaders.end(); ++it) {
			glDetachObjectARB(mProgramId, it->second);
			glDeleteObjectARB(it->second);
		}

		glDeleteObjectARB(mProgramId);
	}


	// ------------------------------------------------------------------
	void Shader::load(const std::string& fileName)
	{
		GLhandleARB id = NULL;
		if (std::regex_match(fileName, fragRegex)) {
			id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
		}

		if (std::regex_match(fileName, vertRegex)) {
			id = glCreateShaderObjectARB(GL_VERTEX_SHADER);
		}
		
		mShaders.push_back(std::make_pair(fileName, id));

		if (!id) {
		    throw Error ("Shader", getID(), "Cannod create a shader object");
		}

		File* file = Resources::inst().require<File>(std::string(fileName));

		if (!file) {
		    throw Error ("Shader", getID(), "Missing shader file");
		}

		uint8* data = file->getData();
		glShaderSourceARB(id, 1, (const GLchar**)&data, NULL);
		glCompileShaderARB(id);

		glAttachObjectARB(mProgramId, id);

		int logSize = 0, status;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

		if (status != GL_TRUE) {
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

			if (logSize > 0) {
				int length  = 0;
				char *infoLog = new char[logSize];

				glGetInfoLogARB(id, logSize, &length, infoLog);

				if (strlen (infoLog) > 0) {
					throw Error ("Shader", getID(), "[OpenGL]" + std::string(infoLog));
				}

				delete[] infoLog;
			}
		}
	}

	// ------------------------------------------------------------------
	void Shader::compile()
	{
		glLinkProgram(mProgramId);

		int logSize = 0, status;
		glGetProgramiv(mProgramId, GL_LINK_STATUS, &status);

		if (status != GL_TRUE) {
			glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &logSize);

			if (logSize > 0) {
				int length  = 0;
				char *infoLog = new char[logSize];

				glGetInfoLogARB(mProgramId, logSize, &length, infoLog);

				if (strlen (infoLog) > 0) {
					throw Error ("Shader", getID(), "[OpenGL]" + std::string(infoLog));
				}

				delete[] infoLog;
			}
		}
	}
	
	// ------------------------------------------------------------------
	std::vector<std::string> Shader::getShaderNames()
	{
		std::vector<std::string> shaders;
		for (std::vector<std::pair<std::string, unsigned> >::iterator it = mShaders.begin(); it != mShaders.end(); ++it) {
			shaders.push_back(it->first);
		}
		return shaders;
	}

	// ------------------------------------------------------------------
	void Shader::setUniformf (const char*  id, int length, float* value)
	{
		GLint location = glGetUniformLocation(currentProgram, id);
		glUniform1fvARB(location, length, value);
	}

	// ------------------------------------------------------------------
	void Shader::setUniformi (const char*  id, int length, int* value)
	{
		GLint location = glGetUniformLocation(currentProgram, id);
		glUniform1ivARB(location, length, value);
	}
	
	// ------------------------------------------------------------------
	void Shader::setUniformColor (const char*  id, Color& c)
	{
		GLint location = glGetUniformLocation(currentProgram, id);
		glUniform4fvARB(location, 1, c.getVec());
	}

	// ------------------------------------------------------------------
	void Shader::setUniformVec2 (const char*  id, Vec2& v)
	{
		GLint location = glGetUniformLocation(currentProgram, id);
		glUniform2fvARB(location, 1, v.getVec());
	}

	// ------------------------------------------------------------------
	void Shader::useProgram (const std::string& name)
	{
		Shader* sh = Resources::inst().get<Shader>(name);

		if (!sh) {
			throw Error ("Shader", name, "Shader does not exist");
		}
		glUseProgramObjectARB(sh->getProgramId());
		shaderStack.push_back(sh->getProgramId());
		currentProgram = sh->getProgramId();
	}

	// ------------------------------------------------------------------
	void Shader::popProgram()
	{
		if (!shaderStack.empty()) {
			shaderStack.pop_back();
		}

		if (!shaderStack.empty()) {
			glUseProgram(*(shaderStack.end() - 1));
			currentProgram = *(shaderStack.end() - 1);
		}
	}

	// ------------------------------------------------------------------
	void Shader::clearStack()
	{
		shaderStack.clear();
	}
	
	luaNewMethod(Shader, new)
	{
		std::string id(luaL_checkstring(L, 1));
		
		Shader *l = Resources::inst().require<Shader>(id);

		luaInstance(L, Shader, l);
		return 1;
	}

	luaNewMethod(Shader, load)
	{
		Shader** l = (Shader**)luaGetInstance(L, 1, "Shader");

		(*l)->load(std::string(luaL_checkstring(L, 2)));

		return 0;
	}

	luaNewMethod(Shader, compile)
	{
		Shader** l = (Shader**)luaGetInstance(L, 1, "Shader");

		(*l)->compile();

		return 0;
	}

	luaBeginMeta(Shader)
		luaMetaMethod(Shader, load)
		luaMetaMethod(Shader, compile)
	luaEndMeta()

	luaBeginMethods(Shader)
		luaMethod(Shader, new)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool Shader::luaRegister(lua_State* L)
	{
		luaClass(L, Shader);
		return true;
	}
};