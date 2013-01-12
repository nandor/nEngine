/**
	@file Particles.cpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "DataSource.hpp"
#include "Particles.hpp"
#include "Scene.hpp"
#include "Timer.hpp"
using namespace boost::property_tree;

namespace nEngine {
	
	// ------------------------------------------------------------------
	Particles::Particles(const std::string& id)
		:SceneNode(id, "Particles"),
		 mData(NULL),
		 mNumParticles(10)
	{

	}

	
	// ------------------------------------------------------------------
	Particles::Particles(const std::string& id, const std::string& confID)
		:SceneNode(id, "Particles"),
		 mData(NULL)
	{
		DataSource* obj = Resources::inst().require<DataSource> (confID);
		 
		try {

			mLifetime = obj->getValue<float> ("lifetime");
			mLifetimeSpread = obj->getValue<float> ("lifetimeSpread");

			mEmitterPos = obj->getVec("emitter");
			mEmitterSpread = obj->getVec("emitterSpread");

			mVelocity = obj->getVec("velocity");
			mVelocitySpread = obj->getVec("velocitySpread");

			mGravity = obj->getVec("gravity");
			mParticleSize = obj->getVec("size");

			mNumParticles = obj->getValue<int> ("numParticles");
			mGenLimit = obj->getValue<int> ("genLimit");

			BOOST_FOREACH(ptree::value_type& v, obj->getChild("gradient")) {
				float frame = boost::lexical_cast<float>(v.first);
				mGradient[frame].mColor = Color (
					v.second.get("color.r", 0.0f), 
					v.second.get("color.g", 0.0f),
					v.second.get("color.b", 0.0f),
					v.second.get("color.a", 1.0f)
				);

				mGradient[frame].mTexture = v.second.get<std::string> ("texture");
				mGradient[frame].mTime = frame;
			}

			if (mGradient.find(0.0) == mGradient.end() || mGradient.find(1.0) == mGradient.end()) {
				throw Error("Particles", getID(), "0.0 and 1.0 gradient stops are required!");
			}

		} catch (std::exception except) {
			throw Error("Particles", getID(), "[json]" + std::string(except.what()));
		}
		
		mGenTime = 0.0f;
		mGenWait = mLifetime / mGenLimit;

		mData = new Particle[mNumParticles];
		mPosBuffer = new float[mNumParticles << 3];
		mTexBuffer = new float[mNumParticles << 3];
		mActiveBuffer = new float[mNumParticles << 2];
		mTimeBuffer = new float[mNumParticles << 2];
		
		for (int i = 0; i < mNumParticles; ++i) {
			mData[i].expire = 0.0f;
			mData[i].active = false;

			mTexBuffer[(i << 3) + 0] = 0.0f;
			mTexBuffer[(i << 3) + 1] = 0.0f;
			
			mTexBuffer[(i << 3) + 2] = 1.0f;			
			mTexBuffer[(i << 3) + 3] = 0.0f;
			
			mTexBuffer[(i << 3) + 4] = 1.0f;
			mTexBuffer[(i << 3) + 5] = 1.0f;
		
			mTexBuffer[(i << 3) + 6] = 0.0f;
			mTexBuffer[(i << 3) + 7] = 1.0f;
		}
	}

	// ------------------------------------------------------------------
	Particles::~Particles()
	{
		if (mData != NULL) {
			delete[] mData;
		}

		if (mPosBuffer != NULL) {
			delete[] mPosBuffer;
		}

		if (mActiveBuffer != NULL) {
			delete[] mActiveBuffer;
		}

		if (mTimeBuffer != NULL) {
			delete[] mTimeBuffer;
		}
	}


	// ------------------------------------------------------------------
	void Particles::draw()
	{
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);

		/// Translate to the position of the system
		Vec2 pos = getPosition();
		glTranslatef(pos.getX() + 60, pos.getY(), 0.0f);
		
		/// Activate the shader
		Shader::useProgram("particle");

		/// Pass the gradient
		int number = 0;
		for (tGradientIter it = mGradient.begin(); it != mGradient.end(); ++it, number++) {
			Resources::inst().require<Image> (it->second.mTexture)->bind(number);

			std::string timeIdx("grdTime[" + boost::lexical_cast<std::string> (number) + "]");
			Shader::setUniformf(timeIdx.c_str(), 1, &it->second.mTime);

			std::string colorIdx("color[" + boost::lexical_cast<std::string> (number) + "]");
			Shader::setUniformColor(colorIdx.c_str(), it->second.mColor);
		}
		
		float time = Timer::inst().getTime();
		for (int i = 0; i < mNumParticles; ++i) {
			if (mData[i].active) {
				float dt = time - mData[i].initTime;
				
				float posX = mData[i].initX + mData[i].velX * dt;
				float posY = mData[i].initY + mData[i].velY * dt;

				mPosBuffer[(i << 3) + 0] = posX;
				mPosBuffer[(i << 3) + 1] = -posY;	

				mPosBuffer[(i << 3) + 2] = posX + mParticleSize.getX();
				mPosBuffer[(i << 3) + 3] = -posY;	

				mPosBuffer[(i << 3) + 4] = posX + mParticleSize.getX();
				mPosBuffer[(i << 3) + 5] = -posY + mParticleSize.getY();	

				mPosBuffer[(i << 3) + 6] = posX;
				mPosBuffer[(i << 3) + 7] = -posY + mParticleSize.getY();	

				float deltaTime = dt / (mData[i].expire - mData[i].initTime);

				mTimeBuffer[(i << 2) + 0] = deltaTime;	
				mTimeBuffer[(i << 2) + 1] = deltaTime;	
				mTimeBuffer[(i << 2) + 2] = deltaTime;		
				mTimeBuffer[(i << 2) + 3] = deltaTime;

				mActiveBuffer[(i << 2) + 0] = 1.0f;
				mActiveBuffer[(i << 2) + 1] = 1.0f;
				mActiveBuffer[(i << 2) + 2] = 1.0f;
				mActiveBuffer[(i << 2) + 3] = 1.0f;

			} else {

				mActiveBuffer[(i << 2) + 0] = 0.0f;
				mActiveBuffer[(i << 2) + 1] = 0.0f;
				mActiveBuffer[(i << 2) + 2] = 0.0f;
				mActiveBuffer[(i << 2) + 3] = 0.0f;
			}
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		int aTime = Shader::getAttribute("aTime");
		int aActive = Shader::getAttribute("aActive");

		glEnableVertexAttribArray(aTime);
		glEnableVertexAttribArray(aActive);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, 0, mPosBuffer);
		glTexCoordPointer(2, GL_FLOAT, 0, mTexBuffer);
		glVertexAttribPointer(aActive, 1, GL_FLOAT, GL_FALSE, 0, mActiveBuffer);
		glVertexAttribPointer(aTime,   1, GL_FLOAT, GL_FALSE, 0, mTimeBuffer);

		glDrawArrays(GL_QUADS, 0, mNumParticles);		

		glDisableVertexAttribArray(aTime);
		glDisableVertexAttribArray(aActive);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	// ------------------------------------------------------------------
	void Particles::drawMarker()
	{

	}


	// ------------------------------------------------------------------
	void Particles::update()
	{
		float time = Timer::inst().getTime();

		int generated = 0;
		for (int i = 0; i < mNumParticles; ++i) {
			Particle* p = mData + i;

			if (p->active && p->expire < time) {
				p->active = false;
			}

			if (!p->active && generated < mGenLimit && mGenTime + mGenWait <= time) {
				generated ++;

				p->initTime = time;
				p->expire = p->initTime + mLifetime + mLifetimeSpread * ((rand() % 1000) / 1000.0f - 0.5f);

				p->initX = mEmitterPos.getX() + mEmitterSpread.getX() * ((rand() % 1000) / 1000.0f - 0.5f);
				p->initY = mEmitterPos.getY() + mEmitterSpread.getY() * ((rand() % 1000) / 1000.0f - 0.5f);
		
				p->velX = mVelocity.getX() + mVelocitySpread.getX() * ((rand() % 1000) / 1000.0f - 0.5f);
				p->velY = mVelocity.getY() + mVelocitySpread.getY() * ((rand() % 1000) / 1000.0f - 0.5f);

				p->active = true;
			}
		}

		if (mGenTime + mGenWait <= time) {
			mGenTime = time;
		}
	}
		
	// ------------------------------------------------------------------
	luaDeclareMethod(Particles, new)
	{
		std::string id(luaL_checkstring(L, 1));
		std::string script(luaL_checkstring(L, 2));

		Particles* p = new Particles(id, script);
		luaInstance(L, Particles, p);
		return 1;
	}
	
	// ------------------------------------------------------------------
	luaBeginMeta(Particles)

	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(Particles)
		luaMethod(Particles, new)
	luaEndMethods()
 
	// ------------------------------------------------------------------
	bool luaRegisterParticles(lua_State* L)
	{
		luaInheritClass(L, Particles, SceneNode);
		return true;
	}
};