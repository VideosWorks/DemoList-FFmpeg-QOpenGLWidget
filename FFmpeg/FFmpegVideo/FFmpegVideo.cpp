#include "FFmpegVideo.h"
#include <QDebug>

#define Get_Str(x) #x

//顶点Shader //gl_Position、attribute、varing？
static const char* vertextShader = Get_Str(
attribute vec4 vertexIn;
attribute vec2 textIn;
varying vec2 textOut;

void main(void)
{
	gl_Position = vertexIn;
	textOut = textIn;
}
);

//片元Shader  //转化公式如何来的？uniform、texture2D、gl_FragColor具体意义？
static const char* fragmentShader = Get_Str(
varying vec2 textOut;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, textOut).r;
	yuv.y = texture2D(tex_u, textOut).r - 0.5;
	yuv.z = texture2D(tex_v, textOut).r - 0.5;
	rgb = mat3(1.0, 1.0, 1.0,
			0, -0.39465, 2.03211,
			1.13983, -0.58060, 0) * yuv;
	gl_FragColor = vec4(rgb, 1);
}
);

int WIDTH = 960;
int HEIGHT = 540;

//ffmpeg -i 123.mp4 -t 20 -pix_fmt yuv420p 123.yuv 生成yuv数据
FFmpegVideo::FFmpegVideo(QWidget *parent)
: QOpenGLWidget(parent)
{
	resize(WIDTH, HEIGHT);
	startTimer((double)1/ (double)25 * 1000);  //帧率 25帧/s
}

FFmpegVideo::~FFmpegVideo()
{
	glDeleteTextures(3, m_textures);
	fclose(file);
}

void FFmpegVideo::initializeGL()
{
	file = fopen("123.yuv", "rb");

	//初始化opengl函数
	initializeOpenGLFunctions();

	qDebug() << "m_shaderProgram add Vertex" << m_shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, vertextShader);
	qDebug() << "m_shaderProgram add Fragment " << m_shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, fragmentShader);

	m_shaderProgram.bindAttributeLocation("vertextIn", 0);
	m_shaderProgram.bindAttributeLocation("textureIn", 1);

	qDebug() << "m_shaderProgram.link() " << m_shaderProgram.link();
	qDebug() << "m_shaderProgram.bind() " << m_shaderProgram.bind();

	glLoadIdentity();
	static const GLfloat vertextArr[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	static const GLfloat textureArr[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	//顶点数据
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, vertextArr);
	glEnableVertexAttribArray(0);

	//材质数据
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, textureArr);
	glEnableVertexAttribArray(1);

	//获取片元Shader中的变量
	m_yuv[0] = m_shaderProgram.uniformLocation("tex_y");
	m_yuv[1] = m_shaderProgram.uniformLocation("tex_u");
	m_yuv[2] = m_shaderProgram.uniformLocation("tex_v");

	//Shader的整个运作流程？？？？

	//自定义rgba数据
// 	unsigned char* data = new unsigned char[100 * 100 * 4];
// 	for (int index = 0; index < 100 * 100 * 4; )
// 	{
// 		data[index + 0] = 0;
// 		data[index + 1] = 0;
// 		data[index + 2] = 255;
// 		data[index + 3] = 30;
// 		index += 4;
// 	}

	data[0] = new unsigned char[WIDTH * HEIGHT];  //创建内存空间
	data[1] = new unsigned char[WIDTH * HEIGHT / 4];
	data[2] = new unsigned char[WIDTH * HEIGHT / 4];

	glGenTextures(3, m_textures);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	//每个参数的意义？
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, 0);  //创建材质显卡空间

	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH / 2, HEIGHT / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH / 2, HEIGHT / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
}

void FFmpegVideo::resizeGL(int w, int h)
{
	
}

void FFmpegVideo::paintGL()
{
	if (feof(file))
	{ 
		fseek(file, 0, SEEK_SET);
	}
	fread(data[0], 1, WIDTH * HEIGHT, file);
	fread(data[1], 1, WIDTH * HEIGHT / 4, file);
	fread(data[2], 1, WIDTH * HEIGHT / 4, file);


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, data[0]);
	glUniform1i(m_yuv[0], 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH / 2, HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, data[1]);
	glUniform1i(m_yuv[1], 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH / 2, HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, data[2]);
	glDisable(GL_TEXTURE_2D);
	glUniform1i(m_yuv[2], 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  //这个是如何绘制的？？？ GL_TRIANGLE_STRIP/GL_TRIANGLE_FAN/GL_TRIANGLES区别？？？
}

void FFmpegVideo::timerEvent(QTimerEvent *event)
{
	update();
	qDebug() << "FFmpegVideo::timerEvent: " << event->timerId();
}
