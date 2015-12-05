
#include <cmath>

namespace Easing
{
	template<class T,class Fun >
	T In(Fun f,float time,const T& start,const T& end,float duration=1)
	{
		if (time==0) return start;	
		if (time==duration) return end;	
		float progress = time / duration;
		T diff = end - start;
		return f(progress) * diff + start;
	}
	template<typename Fun,class T>
	T Out(Fun f,float time,const T& start,const T& end,float duration=1)
	{
		if (time==0) return start;	
		if (time==duration) return end;	
		float progress = (duration - time) / duration;
		T diff = start - end;
		
		
		return f(progress) * diff + end;
	}
	template<typename InFun,typename OutFun,class T>
	T InOut(InFun in_fn,OutFun out_fn,float time,const T& start,const T& end,float duration=1)
	{
		if (time==0) return start;	
		if (time==duration) return end;	
		T center = T((end - start) *0.5) + start;
		float halfd = duration*0.5;
		if(time < halfd)
		{
			return In(in_fn,time,start,center,halfd);
		}
		else
		{
			return Out(out_fn,(time - halfd),center, end,halfd);
		}
	}
	
	template<class T>
	T Liner(float time,const T& start,const T& end,float duration=1)
	{
		float progress = time / duration;
		T diff = end - start;
		return diff * progress + start;
	}
	float Liner(float progress)
	{
		return progress;
	}
	
	float Sine(float progress)
	{
		const float PI = 3.1415926535;
		return -1 * cos(progress* (PI/2)) +1;
	}
	template<class T>
	T InSine(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Sine,time,start,end,duration);
	}
	template<class T>
	T OutSine(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Sine,time,start,end,duration);
	}
	template<class T>
	T InOutSine(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Sine,&Sine,time,start,end,duration);
	}
	
	float Quad(float progress)
	{
		return progress*progress;
	}
	template<class T>
	T InQuad(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Quad,time,start,end,duration);
	}
	template<class T>
	T OutQuad(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Quad,time,start,end,duration);
	}
	template<class T>
	T InOutQuad(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Quad,&Quad,time,start,end,duration);
	}
	
	float Cubic(float progress)
	{
		return progress*progress*progress;
	}
	template<class T>
	T InCubic(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Cubic,time,start,end,duration);
	}
	template<class T>
	T OutCubic(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Cubic,time,start,end,duration);
	}
	template<class T>
	T InOutCubic(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Cubic,&Cubic,time,start,end,duration);
	}
	
	
	float Quart(float progress)
	{
		return progress*progress*progress*progress;
	}
	template<class T>
	T InQuart(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Quart,time,start,end,duration);
	}
	template<class T>
	T OutQuart(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Quart,time,start,end,duration);
	}
	template<class T>
	T InOutQuart(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Quart,&Quart,time,start,end,duration);
	}
	
	
	float Quint(float progress)
	{
		return progress*progress*progress*progress*progress;
	}
	template<class T>
	T InQuint(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Quint,time,start,end,duration);
	}
	template<class T>
	T OutQuint(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Quint,time,start,end,duration);
	}
	template<class T>
	T InOutQuint(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Quint,&Quint,time,start,end,duration);
	}
	
	float Expo(float progress)
	{
		return progress*progress*progress*progress*progress;
	}
	template<class T>
	T InExpo(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Expo,time,start,end,duration);
	}
	template<class T>
	T OutExpo(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Expo,time,start,end,duration);
	}
	template<class T>
	T InOutExpo(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Expo,&Expo,time,start,end,duration);
	}
	
	
	float Circ(float progress)
	{
		return 1 - sqrt(1 - (progress * progress));
	}
	template<class T>
	T InCirc(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Circ,time,start,end,duration);
	}
	template<class T>
	T OutCirc(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Circ,time,start,end,duration);
	}
	template<class T>
	T InOutCirc(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Circ,&Circ,time,start,end,duration);
	}
	
	
	float Back(float progress)
	{
		float s = 1.70158f;
		return progress*progress*((s+1)*progress - s);
	}
	template<class T>
	T InBack(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Back,time,start,end,duration);
	}
	template<class T>
	T OutBack(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Back,time,start,end,duration);
	}
	template<class T>
	T InOutBack(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Back,&Back,time,start,end,duration);
	}
	
	
	float Elastic(float progress)
	{	
		const float PI = 3.;
		return sin(13 * PI * progress) * pow(2, 10 * (progress - 1));
	}
	template<class T>
	T InElastic(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Elastic,time,start,end,duration);
	}
	template<class T>
	T OutElastic(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Elastic,time,start,end,duration);
	}
	template<class T>
	T InOutElastic(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Elastic,&Elastic,time,start,end,duration);
	}
	
	
	float Bounce(float progress)
	{
		progress = 1-progress;
		if (progress < (1/2.75f)) {
			return 1-(7.5625f*progress*progress);
		} else if (progress < (2/2.75f)) {
			return 1-((9.075 * progress * progress) - (9.9 * progress) + 3.4);
		} else if (progress < (2.5/2.75)) {
			return 1-((4356/361.0 * progress * progress) - (35442/1805.0 * progress) + 16061/1805.0);
		} else {
			return 1-((54/5.0 * progress * progress) - (513/25.0 * progress) + 268/25.0);
		}
	}
	template<class T>
	T InBounce(float time,const T& start,const T& end,float duration=1)
	{
		return In(&Bounce,time,start,end,duration);
	}
	template<class T>
	T OutBounce(float time,const T& start,const T& end,float duration=1)
	{
		return Out(&Bounce,time,start,end,duration);
	}
	template<class T>
	T InOutBounce(float time,const T& start,const T& end,float duration=1)
	{
		return InOut(&Bounce,&Bounce,time,start,end,duration);
	}
}
