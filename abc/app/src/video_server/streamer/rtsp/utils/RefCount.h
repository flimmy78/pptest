#ifndef __REF_COUNT_H__
#define __REF_COUNT_H__

#define DECLARE_REF_COUNT										\
private:														\
    RefCount refCount;											\
public:															\
    int addRef() { return refCount.addRef(); };					\
	int getRef() const { return refCount.getRef(); };			\
    virtual int release() {										\
        int count = refCount.release();							\
        if(count <= 0)											\
        {														\
            delete this;										\
        }														\
		return count;											\
    };															\
	void setRef(const int count) { refCount.setRef(count); };

class RefCount
{
private:
	int count;

public:	
	RefCount();
	~RefCount();

	int addRef();
	int getRef() const;
	int release();
	void setRef(const int refCount);
};

#endif //__REF_COUNT_H__
