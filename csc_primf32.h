#pragma once




static void primf32_make_rectangle (float v[], unsigned stride, float x, float y, float w, float h)
{
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + 0; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + h; v += stride;
}


static void primf32_make_rectangle4 (float v[], unsigned stride, float x, float y, float w, float h, float vz, float vw)
{
	v[0] = x + 0; v[1] = y + h;	v[2] = vz; v[3] = vw; v += stride;
	v[0] = x + w; v[1] = y + 0;	v[2] = vz; v[3] = vw; v += stride;
	v[0] = x + 0; v[1] = y + 0;	v[2] = vz; v[3] = vw; v += stride;
	v[0] = x + w; v[1] = y + 0;	v[2] = vz; v[3] = vw; v += stride;
	v[0] = x + 0; v[1] = y + h;	v[2] = vz; v[3] = vw; v += stride;
	v[0] = x + w; v[1] = y + h;	v[2] = vz; v[3] = vw; v += stride;
}
