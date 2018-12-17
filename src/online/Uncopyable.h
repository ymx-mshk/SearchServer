
#ifndef __MYSP_UNCOPYABLE_H__
#define __MYSP_UNCOPYABLE_H__

namespace mysp
{

class Uncopyable{
	protected:
		Uncopyable() = default;
		~Uncopyable() = default;

		Uncopyable(const Uncopyable &) = delete;
		Uncopyable & operator=(const Uncopyable &) = delete;
};

}
#endif
