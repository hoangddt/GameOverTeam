#pragma once

class ITouchListener {
public:
	virtual void touchPerformed(int buttonId) = 0;
};