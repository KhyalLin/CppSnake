#include "Snake.h"
#include "Render.h"

int main(void)
{
	Render render;

	while (true)
	{
		Snake snake;
		Food food;

		while (!snake.isdead())
		{
			snake.wait();
			snake.getDirect();
			snake.goForward();
			snake.collide();
			snake.collide(food);

			render.draw();
			render.draw(snake);
			render.draw(food);
			render.show();
		}

		render.wait();
	}
}
