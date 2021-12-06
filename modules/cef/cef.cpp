#include "cef.h"
#include <iostream>

#include "core/math/vector2.h"
#include "core/color.h"

Cef::Cef()
{
    std::cout << "Hello ProxyCEF" << std::endl;	
}

Cef::~Cef()
{
    std::cout << "Bye ProxyCEF" << std::endl;

	// Shut down CEF.
	// CefShutdown();
}

void Cef::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("do_action", "value"), &Cef::do_action);
}

void Cef::do_action(int value)
{
    std::cout << "ProxyCEF do action " << value << std::endl;
}

void Cef::_init(){

}

void Cef::_process(float delta){

}

void Cef::_draw() {
	//RenderingServer::get_singleton()->canvas_item_add_line(this->get_viewport_rid(),Vector2(200, 100), Vector2(200, 1000), Color(255, 255, 255, 1), 1);
}

