
#include <cmath>

namespace easing
{
	static const int PI = 3.1415926545
	template<typename Fun,class T>
	T In(Fun f,float time,const T& start,const T& end,float duration=1)
	{
		return f(time,start,end,duration);
	}
	template<typename Fun,class T>
	T Out(Fun f,float time,const T& start,const T& end,float duration=1)
	{
		return f(duration - time,end,start,duration);
	}
	template<typename InFun,typename OutFun,class T>
	T InOut(InFun in_fn,OutFun out_fn,float time,const T& start,const T& end,float duration=1)
	{
		if(time < duration*0.5)
		{
			return In(in_fn,time*2,start,(end - start) *0.5 + start,duration);
		}
		else
		{
			return Out(out_fn,(time - 0.5) * 2,(end - start) *0.5 + start, end,duration*0.5);
		}
	}
	
	template<class T>
	T Liner(float time,const T& start,const T& end,float duration=1)
	{
		float progress = time / duration;
		T diff = end - start;
		return diff * progress + start;
	}
	
	template<class T>
	T Sine(float time,const T& start,const T& end,float duration=1)
	{
		float progress = (duration - time) / duration;
		T diff = start - end;
		return diff * sin(progress * (PI/2)) + end;
	}
	template<class T>
	T Quad(float time,const T& start,const T& end,float duration=1)
	{
		float progress = time / duration;
		T diff = end - start;
		return diff*progress*progress + start;
	}
}

#include <iostream>
int main()
{
	std::vector<float> v;
	const int N = 10;
	std::cout << "Liner(v)" << std::endl;
	for (int i=0;i<N;++i)
	{
		float v = float(i)/N;
		std::cout << Liner(v) << std::endl;
	}
	
}

