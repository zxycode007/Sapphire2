#pragma once


class IImage
{
public:
     
	void SetWidth(int width);

	int GetWidth(int width);

protected:

	int mWidth;
	int mHeight;

	int mDepth;
	int mFormat;

	

};