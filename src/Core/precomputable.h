#pragma once

class precomputable {

public:
	precomputable() {}
	virtual ~precomputable() {}

	void precomputableInvalidate() {
		mPrecomputableValid = false;
	}

	void precomputableCheck() {
		if(!mPrecomputableValid) {
			Precompute();
			mPrecomputableValid = true;
		}
	}

	virtual void Precompute() {}

private:
	bool mPrecomputableValid;

};
