#include <Nebula.h>

class App : public Nebula::Application {
public:
	App() {
	}

	~App() {
	}
};

int main() {
	App* app = new App();
	app->Run();
	delete app;
}