/**
	@file Font.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Font.hpp"

namespace nEngine {
	/**
		Get the next power of 2
		@param i        A number
		@return         Next power of two
	*/
	inline int nextP2(int i)
	{
		int res = 1;
		while (res < i) {
			res <<= 1;
		}
		return res;
	}

	FT_Library Font::sLibrary;
	FT_Face Font::sFace;
	bool Font::sHasFace;


	// ------------------------------------------------------------------
	Font::Font(const std::string& _id, unsigned _height)
		:Resource(_id, RESOURCE_FONT)
	{
		setUnloadable(false);
		mTextures = new GLuint[128];
		mCharWidth = new unsigned[128];
		
		mHeight = _height;
		mListBase = glGenLists(128);

		FT_Set_Char_Size(sFace, mHeight << 6, mHeight << 6, 96, 96);
		glGenTextures(128, mTextures);

		for (unsigned char i = 0; i < 127;i++) {
			makeDisplayList(i);
		}
	}

	// ------------------------------------------------------------------
	Font::~Font()
	{
		glDeleteLists(mListBase, 128);
		glDeleteTextures(128, mTextures);

		if (mTextures != NULL) {
			delete[] mTextures;
			mTextures = NULL;
		}

		if (mCharWidth != NULL) {
			delete[] mCharWidth;
			mCharWidth = NULL;
		}
	}


	// ------------------------------------------------------------------
	bool Font::makeDisplayList(char ch)
	{
		if (FT_Load_Glyph(sFace, FT_Get_Char_Index (sFace, ch), FT_LOAD_DEFAULT)) {
			return false;
		}

		FT_Glyph glyph;
		if(FT_Get_Glyph(sFace->glyph, &glyph )) {
			return false;
		}

		FT_Glyph_To_Bitmap (&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int width = nextP2 (bitmap.width);
		int height = nextP2 (bitmap.rows);

		GLubyte* expanded_data = new GLubyte[2 * width * height];
		memset(expanded_data, 0, 2 * width * height);

		for (int j = 0; j < bitmap.rows; j++) {
			for (int i = 0; i < bitmap.width; i++){
				expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] = bitmap.buffer[i + bitmap.width * j];
			}
		}

		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glBindTexture( GL_TEXTURE_2D, mTextures[(int)ch]);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

		delete [] expanded_data;

		glNewList(mListBase + ch, GL_COMPILE);
		
		
		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glBindTexture(GL_TEXTURE_2D, mTextures[(int)ch]);

		glPushMatrix();
		glTranslatef(bitmap_glyph->left, mHeight - bitmap_glyph->top, 0);

		float x = (float)bitmap.width / width;
		float y = (float)bitmap.rows / height;

		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(0, 0);
			glTexCoord2d(x, 0); glVertex2f(bitmap.width, 0);
			glTexCoord2d(x, y); glVertex2f(bitmap.width, bitmap.rows);
			glTexCoord2d(0, y); glVertex2f(0, bitmap.rows);
		glEnd();


		glPopMatrix();
		glTranslatef(sFace->glyph->advance.x >> 6, 0, 0);
		glEndList();
		
		mCharWidth[(int)ch] = sFace->glyph->advance.x >> 6;

		return true;
	}
	
	// ------------------------------------------------------------------
	int Font::getMemoryUsage()
	{
		return sizeof(*this) + sizeof(unsigned) * 256;
	}

	// ------------------------------------------------------------------
	bool Font::initLibrary()
	{
		sHasFace = false;
		FT_Init_FreeType (&sLibrary);
		return true;
	}

	// ------------------------------------------------------------------
	bool Font::closeLibrary()
	{
		FT_Done_FreeType(sLibrary);
		return true;
	}


	// ------------------------------------------------------------------
	bool Font::loadFace(const std::string& name)
	{
		if (sHasFace) {
			FT_Done_Face(sFace);
			sHasFace = false;
		}

		File* file =  Resources::inst().require<File> (name);
    
		FT_New_Memory_Face(sLibrary, file->getData(), file->getSize(), 0, &sFace);
		sHasFace = true;

		return true;
	}
	
	// ------------------------------------------------------------------
	bool Font::unloadFace()
	{
		if (sHasFace) {
			FT_Done_Face(sFace);
			sHasFace = false;
			return true;
		}

		return false;
	}
	
	
	// ------------------------------------------------------------------
	unsigned Font::getCharWidth(char ch)
	{
		if (0 <= ch && ch <= 128) {
			return mCharWidth[(int)ch];
		}

		return 0;
	}


	// ------------------------------------------------------------------
	unsigned Font::getTextWidth(const std::string& str)
	{
		unsigned size = 0;
		for (unsigned i = 0; i < str.length(); ++i) {
			size += mCharWidth[str[i]];
		}

		return size;
	}
	// ------------------------------------------------------------------
	luaNewMethod(Font, loadFace)
	{
		Font::loadFace(std::string(luaL_checkstring(L, 1)));
		return 0;
	}

	
	// ------------------------------------------------------------------
	luaNewMethod(Font, loadFont)
	{
		Resources::inst().addResource(new Font(std::string(luaL_checkstring(L, 1)), luaL_checkinteger(L, 2)));
		return 0;
	}


	luaBeginMeta(Font)
	luaEndMeta()

	luaBeginMethods(Font)
		luaMethod(Font, loadFace)
		luaMethod(Font, loadFont)
	luaEndMethods()
	
	// ------------------------------------------------------------------
	bool Font::luaRegister(lua_State* L)
	{		
		luaClass(L, Font);
		return true;
	}
};