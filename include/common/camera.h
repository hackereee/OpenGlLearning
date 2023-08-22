#pragma once
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "glm/ext/matrix_transform.hpp"


/**
 * 摄像机移动方向
 */
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


//region 常量
/**
 * 偏航角
 */
const float YAW = -90.0f;
/**
 * 俯仰角，默认0度
 */
const float PITCH = 0.0f;
/**
 * 鼠标灵敏度
 */
const float SENSITIVITY = 0.1f;
/**
 * 移动速度
 */
const float SPEED = 25.f;
/**
 *
 */
const float ZOOM = 45.f;
//endregion

class Camera
{
public:
	//摄像机位置
	glm::vec3 Position;
	//前 旋转矩阵
	glm::vec3 Front;
	//上 
	glm::vec3 Up;
	//右
	glm::vec3 Right;
	float Speed;
	float Zoom;
	float Sensitivity;
	//偏航角
	float Yaw;
	//俯仰角
	float Pitch;

	glm::vec3 WorldUp;

	/**  // NOLINT(clang-diagnostic-documentation)
	 * \brief
	 * \param position 摄像机位置
	 * \param worldUp 在世界坐标中的上向量
	 * \param speed 移动速度
	 * \param zoom
	 * \param sensitivity
	 * \param yaw 偏航角
	 * \param pitch 俯仰角
	 */

	Camera(glm::vec3 position, glm::vec3 worldUp,
		float speed = SPEED,
		float zoom = ZOOM,
		float sensitivity = SENSITIVITY,
		float yaw = YAW,
		float pitch = PITCH) : Front(0.0f, 0.0f, -1.0f)
	{

		Position = position;
		Speed = speed;
		Zoom = zoom;
		Sensitivity = sensitivity;
		Pitch = pitch;
		Yaw = yaw;
		WorldUp = worldUp;
		UpdateVectors();
	}


	/**
	 * \brief 获取观察矩阵
	 * \return观察矩阵, lookat 矩阵
	 * Position 方向向量 + Front 旋转矩阵中的前向量，可以得到摄像机中心点
	 */
	glm::mat4 ViewMartrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}


	/**
	 * \brief
	 * \param movement 移动方向
	 * \param deltaTime 单位时间，每一帧的间隔时间，通过该时间同步不同设备间的移动距离
	 */
	void Keyboard(Camera_Movement movement, float deltaTime)
	{
		float velocity = Speed * deltaTime;
		switch (movement)
		{
		case FORWARD:
			Position += Front * velocity;
			break;
		case BACKWARD:
			Position -= Front * velocity;
			break;
		case LEFT:
			Position -= Right * velocity;
			break;
		case RIGHT:
			Position += Right * velocity;
			break;
		}

		UpdateVectors();
	}

	/**
	 * \brief
	 * \param xOff x轴旋转
	 * \param yOff y轴旋转
	 * \param contraintPitch 约束俯仰，如果不约束则会出现超过90度
	 */
	void MouseMove(float xOff, float yOff, bool contraintPitch = true)
	{
		//移动距离*灵敏度
		xOff *= Sensitivity;
		yOff *= Sensitivity;
		// 偏航角弧长
		Yaw *= xOff;
		// 俯仰角弧长
		Pitch *= yOff;

		if (contraintPitch)
		{
			if (Pitch > 89.f)
			{
				Pitch = 89.f;
			}
			else if (Pitch < -89.f)
			{
				Pitch = -89.f;
			}
			UpdateVectors();
		}
	}

	/**
	 * \brief 滚轮缩放
	 * \param depth
	 */
	void MouseScroll(float depth)
	{
		Zoom -= depth;
		if (Zoom < 1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f)
		{
			Zoom = 45.0f;
		}
	}

private:
	/**
	 * \brief 初始化旋转矩阵， 即组成标准化的Front, Right, Up 观察坐标系

	 *
	 */
	void UpdateVectors()
	{
		//front 向量，就是旋转矩阵中的“前的位置”
		glm::vec3 front;
		//首先是摄像机的y轴，它最简单，因为他其实就是俯仰角（pitch）的角度
		front.y = sin(glm::radians(Pitch));
		//然后我们设置x轴，这个轴就相对特殊，因为它首先要经过偏航角（yaw）的角度变换，同时它又受到俯仰角（pitch）的影响
		//试想一下一个摄像机在拍摄的时候，它的左右（x轴）并不仅仅受到xz平面的角度影响，同时还受到上下（xy平面）的影响，因为摄像机是以yaw、pitch两个角度发散的
		//假如我们只考虑xz平面而不考虑xy的话，那最终视野并不符合摄像机的观察范围，所以x轴应当是xz平面及xy平面上x向量的叠加
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		//最后设置z轴，z轴逻辑和x轴同理，都是受到xz平面和yz平面的影响的
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		//标准化坐标
		Front = glm::normalize(front);
		//将Front 与传入的up向量叉乘，即可得到右坐标
		Right = glm::normalize(glm::cross(Front, WorldUp));
		//最后再将标准化的右坐标和Front坐标叉乘，即可等到真正的up
		Up = glm::normalize(glm::cross(Right, Front));
	}
};