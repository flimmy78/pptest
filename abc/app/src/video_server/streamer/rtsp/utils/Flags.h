#ifndef __FLAGS_H__
#define __FLAGS_h__

class Flags
{
protected:
	UINT flags;

public:
	Flags(void);
	Flags(UINT uFlags);
	~Flags(void);

	void setFlags(UINT uFlags, bool bValue);
	void setFlags(UINT uFlags);
	UINT getFlags(UINT uMask=0xFFFFFFFFu);
	void toggleFlags(UINT uFlags);
};

#endif
