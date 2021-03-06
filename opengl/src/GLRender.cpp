#include "GLRender.hpp"

extern "C"	IGraphic	*load(glm::ivec2 mapsize)
{
	return new GLRender(mapsize);
}

GLRender::GLRender(glm::ivec2 mapsize) : _window(800, 800, "OpenGL")
{
	_data.resize(mapsize.x * mapsize.y * 4);
	_width = mapsize.x;
	_height = mapsize.y;

	// assigning each character a 'random' color
	for (size_t i = 0; i < 255; i++)
	{
		_colors[i] = 256 * 256 * 256 * ((i * 1172) % 255) +
			256 * ((i * 57) % 255) +
			256 * 256 * ((i * 389) % 255) +
			255;
	}
	_colors[(int)'#'] = 0xff222222;
	_colors[(int)'O'] = 0xff337733;
	_colors[(int)'@'] = 0xff333377;
}

void	GLRender::Destroy(void)
{
	_window.Close();
	delete this;
}

void	GLRender::Draw(glm::ivec2 pos, char c)
{
	size_t i = (pos.x + (_height - pos.y - 1) * _width) * 4;

	std::memmove(&_data[i], &_colors[(unsigned char)c], 4);
}

void	GLRender::Display(void)
{
	glClearColor(0.1, 0.15, 0.2, 1);
	_window.Clear();

	float screenAspect = _window.GetAspect();
	float imageAspect = (float)_width / (float)_height;

	if (screenAspect / imageAspect > 1)
	{
		float offset = imageAspect / screenAspect;
		_window.SetRenderMask((1 - offset) / 2.0f, 0, offset, 1);
	}
	else if (screenAspect / imageAspect < 1)
	{
		float offset = screenAspect / imageAspect;
		_window.SetRenderMask(0, (1 - offset) / 2.0f, 1, offset);
	}
	glClearColor(0.4, 0.45, 0.7, 1);
	_window.Clear();
	_image.Render(_data, _width, _height);
	_window.Render();
	std::memset(&_data[0], 0, _width * _height * 4);
	_window.RemoveRenderMask();
}

Input	GLRender::Input(void)
{
	struct Input out = {false, false, false, false, false, false, false, false};
	
	if (_window.KeyPress(GLFW_KEY_LEFT))
		out.left = true;
	if (_window.KeyPress(GLFW_KEY_RIGHT))
		out.right = true;
	if (_window.KeyPress(GLFW_KEY_UP))
		out.up = true;
	if (_window.KeyPress(GLFW_KEY_DOWN))
		out.down = true;
	if (_window.ShouldClose() || _window.KeyPress(GLFW_KEY_ESCAPE))
		out.close = true;
	if (_window.KeyPress('1'))
		out.one = true;
	if (_window.KeyPress('2'))
		out.two = true;
	if (_window.KeyPress('3'))
		out.three = true;

	return out;
}
