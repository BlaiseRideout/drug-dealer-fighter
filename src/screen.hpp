#ifndef SCREEN_H
#define SCREEN_H

class Screen {
	public:
		Screen();
		boolean isRunning;
		boolean isDestroyed;

		virtual void update(double time) = 0;
		void destroy();
};

#endif