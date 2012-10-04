#include "app.hpp"
#include "frame.hpp"


IMPLEMENT_APP(App);

bool App::OnInit() {


	Frame* frame = new Frame();
	frame->Show();


	return true;

}
