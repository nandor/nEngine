/**
	@file Particles.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "Lua.hpp"
#include "SceneNode.hpp"

namespace nEngine {
	class Particles : public SceneNode {
	public:
		/**
			Constructor
		*/
		Particles(const std::string& id);

		/**
			Build a new particle system from a json config
		*/
		Particles(const std::string& id, const std::string& confID);

		/**
			Destructor
		*/
		~Particles();

		/**
			Draw the particles
		*/
		void draw();
		
		/**
			Update the particles
		*/
		void update();

		/**
			Set the velocity
			@param v			Velocity
			@param p			Velocity spread
		*/
		void setVelocity(Vec2 v, Vec2 p)
		{
			mVelocity = v;
			mVelocitySpread = p;
		}

		/**
			Set the gravity
			@param g			Gravity
		*/
		void setGravity(Vec2 g)
		{
			mGravity = g;
		}
		
		/**
			Set the position of the emitter
			@param e			Emitter position
			@param p			Emitter spread
		*/
		void setEmitter(Vec2 e, Vec2 p)
		{
			mEmitterPos = e;
			mEmitterSpread = p;
		}
		
		/**
			Set the lifetime of the particle
			@param l			Lifetime
			@param s			Lifetime spread
		*/
		void setLifetime(float l, float s) 
		{
			mLifetime = l;
			mLifetimeSpread = s;
		}

		/**
			Set generation parameters
			@param f			Frequency
			@param t			How many particles should be generated each round
		*/
		void setGenPeriod(float f, int num)
		{
			mGenPeriod = f;
			mGenNumber = num;
		}

	private:
		/// Structure holding data for a single particle
		struct Particle {
			Vec2 mPos;
			Vec2 mVel;
			float mExpire;
			float mInitTime;
		};

		/// Structure holding gradient data
		struct Gradient {
			Vec2 mSize;
			Color mColor;
			std::string mTexture;
			float mTime;
		};

		/// Iterator for the gradient
		typedef std::map<float, Gradient>::iterator tGradientIter;

		/// Iterator for the particles
		typedef std::list<Particle>::iterator tIter;

	private:

		/**
			Initialise a particle
			@param p			Pointer to particle
		*/
		void initParticle(Particle* p);

		/**
			Get the two required gradient stops
		*/
		std::pair<tGradientIter, tGradientIter> getGradientStops(float f);
	private:
		/// Position of the emitter
		Vec2 mEmitterPos;

		/// Spread of the emitter
		Vec2 mEmitterSpread;
				
		/// Gravity
		Vec2 mGravity;

		/// Velocity
		Vec2 mVelocity;

		/// Velocity spread
		Vec2 mVelocitySpread;
		
		/// Lifetime
		float mLifetime;

		/// Lifetime spread
		float mLifetimeSpread;

		/// Generation period
		float mGenPeriod;

		/// Last time we generated particles
		float mLastGenTime;

		/// How many should be generated
		int mGenNumber;

		/// Does the particles have textures?
		bool mIsTextured;

		/// Particles
		std::list<Particle> mParticles;

		/// Gradient
		std::map<float, Gradient> mGradient;
	};

	bool luaRegisterParticles(lua_State* L);
};

#endif /*PARTICLES_HPP*/