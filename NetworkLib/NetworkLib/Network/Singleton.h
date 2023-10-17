#pragma once

template <typename Ty_>
class Singleton
{
public:
	static Ty_* getInstance()
	{
		if (NULL == instance_)
			instance_ = new Ty_;

		return instance_;
	}

	static void destroyInstance()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = NULL;
		}
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static Ty_* instance_;
};

template <typename Ty_>
Ty_* Singleton<Ty_>::instance_ = 0;