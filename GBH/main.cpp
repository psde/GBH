#include "include.hpp"

using namespace std;

class GameWindow : public Gosu::Window
{
	private:
		Gosu::Font *font;
		Gosu::fpsCounter *fps;

		Style* style;
		Map* map;

		double x, y, z, a;

	public:
		GameWindow()
		 :	Gosu::Window(1280, 1024, false)
		{
			glewInit();
			this->font = new Gosu::Font(graphics(), Gosu::defaultFontName(), 20);
			this->fps = new Gosu::fpsCounter(&graphics(), Gosu::realWidth(graphics())-203, 3, 200, 100, 0.03f);

			this->style = new Style("data/styles/wil.sty");

			this->map = new Map("data/maps/wil.gmp", this->style);

			this->x = -12;
			this->y = 30;
			this->z = -18;
			this->a = -45;
		}

		void draw()
		{
			graphics().beginGL();

			glViewport(0, 0, Gosu::realWidth(graphics()), Gosu::realHeight(graphics()));

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, (Gosu::realWidth(graphics())/Gosu::realHeight(graphics())), 1.0f, 700.0f);
			
			glMatrixMode(GL_MODELVIEW);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glRotatef(this->a, 1, 0, 0);
			glTranslatef(this->x, this->y, this->z);

			glEnable(GL_TEXTURE_2D);
			
			// z-ordering
			glEnable(GL_DEPTH_TEST);
			glDepthRange(0,1);
			glDepthFunc(GL_LEQUAL);

			// Alpha-testing is now done in shader
			/*glAlphaFunc(GL_GREATER, 0.4f);
			glEnable(GL_ALPHA_TEST);*/

			// Investigate this, maybe It can help with fliping of transparent stuff
			//glEnable(GL_CULL_FACE);

			map->draw(graphics());

			// Koordinatenkreuz:
			glDisable(GL_TEXTURE_2D);
			glLineWidth(5);
			glColor3f(1,0,0);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0,0,0);
				glVertex3f(0,0,999);
			glEnd();
			glColor3f(0,1,0);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0,0,0);
				glVertex3f(0,-999,0);
			glEnd();
			glColor3f(0,0,1);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0,0,0);
				glVertex3f(999,0,0);
			glEnd();
			glColor3f(1,1,1);

			graphics().endGL();

			this->font->draw(L"x/y/z/a: " + boost::lexical_cast<wstring>(this->x) + L", " + boost::lexical_cast<wstring>(this->y) + L", " + boost::lexical_cast<wstring>(this->z) + L" @ " + boost::lexical_cast<wstring>(this->a) + L"", 0, 0, 1);

			this->font->draw(L"no. of vertices: " + boost::lexical_cast<wstring>(this->map->getVerticeSize()) , 0, 20, 1);

			fps->updateFPS();
			fps->draw();
		}

		void update()
		{
			if(input().down(Gosu::kbLeft)) this->x += 1;
			if(input().down(Gosu::kbRight)) this->x -= 1;

			if(input().down(Gosu::kbDown)) this->y += 1;
			if(input().down(Gosu::kbUp)) this->y -= 1;

			if(input().down(Gosu::kbPageDown)) this->z += 1;
			if(input().down(Gosu::kbPageUp)) this->z -= 1;

			if(input().down(Gosu::kbS)) this->a += 1;
			if(input().down(Gosu::kbW)) this->a -= 1;

			map->update();
		}

		void buttonDown(Gosu::Button button)
		{
			if(button == Gosu::kbEscape) close();

			if(button == Gosu::kb1) {
				delete this->style;
				delete this->map;
				this->style = new Style("data/styles/wil.sty");
            	this->map = new Map("data/maps/wil.gmp", this->style);
			}

            if(button == Gosu::kb2) {
				delete this->style;
				delete this->map;
				this->style = new Style("data/styles/ste.sty");
                this->map = new Map("data/maps/ste.gmp", this->style);
            }

            if(button == Gosu::kb3) {
				delete this->style;
				delete this->map;
				this->style = new Style("data/styles/bil.sty");
                this->map = new Map("data/maps/bil.gmp", this->style);
            }

            if(button == Gosu::kbSpace) {
				this->x = -126;
				this->y = 126;
				this->z = -323;
				this->a = 0;
            }

		}
};

int main(int argc, char* argv[])
{

    GameWindow win;
    win.show();

	return 0;
}
