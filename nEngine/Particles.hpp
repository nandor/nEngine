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
			Draw a marker to represent the emitter
		*/
		void drawMarker();
		
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
			Check if the character is on a certain position
			@param v				Cursor position
			@return					Always true
		*/
		bool intersects(Vec2& v)
		{
			return false;
		}
	private:
		/// Particle data
		struct Particle {
			float initX, initY;
			float velX, velY;
			float expire, initTime;
			bool active;
		};
		
		/// Structure holding gradient data
		struct Gradient {
			float mTime;
			Color mColor;
			std::string mTexture;
		};

		/// Iterator for the gradient
		typedef std::map<float, Gradient>::iterator tGradientIter;

	private:
		/// Size of a partcle
		Vec2 mParticleSize;

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
		int mNumParticles;

		/// How many should be generated
		int mGenLimit;

		/// Last gen time
		float mGenTime;

		/// Gen timeout
		float mGenWait;
		
		/// Particle data
		Particle* mData;

		/// Vertex array
		float *mPosBuffer, *mTexBuffer, *mActiveBuffer, *mTimeBuffer;

		/// Gradient
		std::map<float, Gradient> mGradient;

		/// Texture
		std::string mTexture;
	};

	bool luaRegisterParticles(lua_State* L);
};

#endif /*PARTICLES_HPP*/