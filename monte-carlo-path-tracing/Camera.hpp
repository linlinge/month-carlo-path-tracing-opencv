#pragma once
#include "V3.hpp"
class Camera
{
public:
	V3 position_;
	V3 lookat_;
	V3 applied_up_;
	V3 actual_up_;
	V3 forward_;
	V3 left_;

	float fovy_degree_; // represent with degree
	float fovy_arc_;
	float focal_length_;
	int image_pixel_width_, image_pixel_height_;
	float image_actual_width_, image_actual_height_;
	V3 image_origin_;
	float pixel_width_, pixel_height_;

	Camera operator=(Camera& dat)
	{
		position_ = dat.position_;
		lookat_ = dat.lookat_;
		applied_up_ = dat.applied_up_;
		actual_up_ = dat.actual_up_;
		forward_ = dat.forward_;
		left_ = dat.left_;
		fovy_degree_ = dat.fovy_degree_;
		fovy_arc_ = dat.fovy_arc_;
		focal_length_ = dat.focal_length_;		
		image_pixel_width_ = dat.image_pixel_width_;
		image_pixel_height_ = dat.image_pixel_height_;
		image_actual_width_ = dat.image_actual_height_;
		image_actual_height_ = dat.image_actual_height_;
		image_origin_ = dat.image_origin_;
		pixel_width_ = dat.pixel_width_;
		pixel_height_ = dat.pixel_height_;
		return *this;
	}

	Camera(V3 pos, V3 lookat, V3 up, float fovy_degree, float focal_length, int width, int height)
	{
		position_ = pos;
		lookat_ = lookat;
		applied_up_ = up;
		fovy_degree_ = fovy_degree;
		fovy_arc_ = fovy_degree * PI / 180.0;
		focal_length_ = focal_length;
		image_pixel_width_ = width;
		image_pixel_height_ = height;

		// actual up, left and forward diretion
		float arc = GetArc(position_ - lookat_, applied_up_);
		actual_up_ = (applied_up_*sin(arc)).GetNorm();
		forward_ = (lookat_ - position_).GetNorm();
		left_ = (Cross(forward_, actual_up_)).GetNorm();

		// image origin, pixel height, pixel width
		float r = tan(fovy_arc_ / 2.0f)*focal_length_*2.0f;
		float alpha = atan(1.0f*image_pixel_width_ / image_pixel_height_);
		image_actual_width_ = r * cos(alpha);
		image_actual_height_ = r * sin(alpha);
		pixel_width_ = image_actual_width_ * 1.0f / image_pixel_width_;
		pixel_height_ = image_actual_height_ * 1.0f / image_pixel_height_;
		image_origin_ = position_ + focal_length_ * forward_ + image_actual_width_ / 2.0f*left_ + image_actual_height_ / 2.0f*actual_up_;
	}
	Camera() {};
};