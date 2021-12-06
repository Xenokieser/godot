#include "cef_canvas_item.h"

#include "core/math/vector2.h"
#include "core/color.h"



void CefCanvasItem::loadUrl(String* url) {
	print_line("CefCanvasItem::loadUrl");
}

void CefCanvasItem::_init(){
	print_line("CefCanvasItem::_init");
}

void CefCanvasItem::_process(float delta){
	print_line("CefCanvasItem::_process");
	update();
}

void  CefCanvasItem::_draw() {
	print_line("CefCanvasItem::_draw");
	draw_line(Vector2(200, 100), Vector2(200, 1000), Color(255, 255, 255, 1), 1);
}
