/**
	@file Particles.cpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "JSONObject.hpp"
#include "Particles.hpp"
#include "Scene.hpp"
#include "Timer.hpp"
using namespace boost::property_tree;

namespace nEngine {
	
	// ------------------------------------------------------------------
	Particles::Particles(const std::string& id)
		:SceneNode(id, "Particles"),
		 mParticles(NULL),
		 mLastGenTime(0.0f),
		 mGenPeriod(1e10)
	{

	}

	
	// ------------------------------------------------------------------
	Particles::Particles(const std::string& id, const std::string& confID)
		:SceneNode(id, "Particles")
	{
		JSONObject* obj = Resources::inst().require<JSONObject> (confID);

		try {
			mLifetime = obj->getValue<float> ("lifetime");
			mLifetimeSpread = obj->getValue<float> ("lifetimeSpread");

			mEmitterPos = obj->getVec("emitter");
			mEmitterSpread = obj->getVec("emitterSpread");

			mVelocity = obj->getVec("velocity");
			mVelocitySpread = obj->getVec("velocitySpread");

			mGravity = obj->getVec("gravity");

			mIsTextured = obj->getValue<bool> ("isTextured");

			mGenPeriod = obj->getValue<float> ("genPeriod");
			mGenNumber = obj->getValue<int> ("genNumber");

			BOOST_FOREACH(ptree::value_type& v, obj->getChild("gradient")) {
				float frame = boost::lexical_cast<float>(v.first);

				mGradient[frame].mSize = Vec2(v.second.get<float>("size.x"), v.second.get<float>("size.y"));
				
				mGradient[frame].mColor = Color (
					v.second.get("color.r", 0.0f), 
					v.second.get("color.g", 0.0f),
					v.second.get("color.b", 0.0f),
					v.second.get("color.a", 1.0f)
				);

				mGradient[frame].mTime = frame;

				if (mIsTextured) {
					mGradient[frame].mTexture = v.second.get<std::string> ("texture");
				}
			}

			if (mGradient.find(0.0) == mGradient.end() || mGradient.find(1.0) == mGradient.end()) {
				throw Error("Particles", getID(), "0.0 and 1.0 gradient stops are required!");
			}

		} catch (std::exception except) {
			throw Error("Particles", getID(), "[json]" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	Particles::~Particles()
	{
		mParticles.clear();
	}


	// ------------------------------------------------------------------
	void Particles::draw()
	{
		glPushMatrix();

		float time = Timer::inst().getTime();
		glDisable(GL_DEPTH_TEST);

		Vec2 pos(
			(mTile.getY() + mTile.getX()) * TILE_WIDTH / 2,
			(mTile.getY() - mTile.getX()) * TILE_HEIGHT / 2
		);

		glScalef(1.0f, -1.0f, 1.0f);

		Shader::useProgram("particle");

		for (tIter it = mParticles.begin(); it != mParticles.end(); ++it) {
			float d = (time - it->mInitTime) / (it->mExpire - it->mInitTime);

			glPushMatrix();
			glTranslatef(pos.getX() + it->mPos.getX() + 60, pos.getY() + it->mPos.getY(), 0.0f);

			std::pair<tGradientIter, tGradientIter> stops = getGradientStops(d);

			Gradient& first = stops.first->second;
			Gradient& second = stops.second->second;
			
			float dt = (d - first.mTime) / (second.mTime - first.mTime);

			Shader::setUniformf("dt", 1, &dt);
			Shader::setUniformColor("colorA", first.mColor);
			Shader::setUniformColor("colorB", second.mColor);

			int isTextured = mIsTextured;
			Shader::setUniformi("isTextured", 1, &isTextured);

			Vec2 iSize = first.mSize * (1.0 - dt) + second.mSize * dt;
			
			if (mIsTextured) {
				Image* texA = Resources::inst().require<Image> (first.mTexture);
				Image* texB = Resources::inst().require<Image> (second.mTexture);
				
				glActiveTextureARB(GL_TEXTURE0_ARB); 
				glBindTexture(GL_TEXTURE_2D, texA->getTextureID()); 

				glActiveTextureARB(GL_TEXTURE1_ARB); 
				glBindTexture(GL_TEXTURE_2D, texB->getTextureID()); 
			}
			
			glColor3f(0.0, 1.0, 0.0f);
			glBegin(GL_QUADS);
				glVertex2f(0, 0);
				glVertex2f(iSize.getX(), 0);
				glVertex2f(iSize.getX(), iSize.getY());
				glVertex2f(0, iSize.getY());
			glEnd();

			glPopMatrix();
		}
		
		glPopMatrix();
	}

	// ------------------------------------------------------------------
	void Particles::update()
	{
		float time = Timer::inst().getTime();

		tIter it = mParticles.begin();
		while (it != mParticles.end()) {
			it->mPos += it->mVel;

			if (it->mExpire <= time) {
				mParticles.erase(it++);
			} else {
				it++;
			}
		}

		if (mLastGenTime + mGenPeriod <= time) {
			mLastGenTime = time;

			for (int i = 0; i < mGenNumber; ++i) {
				Particle p;
				initParticle(&p);
				mParticles.push_back(p);
			}
		}
	}

	// ------------------------------------------------------------------
	void Particles::initParticle(Particle* p)
	{
		p->mInitTime = Timer::inst().getTime();
		p->mExpire = p->mInitTime + mLifetime + mLifetimeSpread * ((rand() % 1000) / 1000.0f - 0.5f);

		p->mPos.setX(mEmitterPos.getX() + mEmitterSpread.getX() * ((rand() % 1000) / 1000.0f) - 0.5f);
		p->mPos.setY(mEmitterPos.getY() + mEmitterSpread.getY() * ((rand() % 1000) / 1000.0f) - 0.5f);
		
		p->mVel.setX(mVelocity.getX() + mVelocitySpread.getX() * ((rand() % 1000) / 1000.0f) - 0.5f);
		p->mVel.setY(mVelocity.getY() + mVelocitySpread.getY() * ((rand() % 1000) / 1000.0f) - 0.5f);
	}


	// ------------------------------------------------------------------
	std::pair<Particles::tGradientIter, Particles::tGradientIter> Particles::getGradientStops(float f)
	{
		for (tGradientIter it = mGradient.begin(); it != mGradient.end(); ++it) {
			tGradientIter jt = it;
			if (++jt == mGradient.end()) {
				break;
			}

			if (it->first <= f && f <= jt->first) {
				return std::make_pair(it, jt);
			}
		}

		return std::make_pair(mGradient.begin(), mGradient.begin());
	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Particles, new)
	{
		Particles* p = new Particles(std::string(luaL_checkstring(L, 1)), std::string(luaL_checkstring(L, 2)));
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