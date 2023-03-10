#pragma once
#include<string>
#include<memory>

namespace TurboGE
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const std::string&);
		static std::shared_ptr<Texture2D> Create();
		virtual bool operator!=(const Texture2D&) = 0;
	};

}