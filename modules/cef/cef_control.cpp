#include "cef_control.h"
#include "cef_control_ico.h"
#include "core/resource.h"
//#include "core/object/gdvirtual.gen.inc"
//#include "core/object/script_language.h"

int CefControl::err_status = -1;



/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

CefControl::CefControl() {
}

CefControl::~CefControl() {
}

void CefControl::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			_draw_error("Not supported!");
		} break;
		default: {
			//NOP
		} break;
	}
}

void CefControl::get_snapshot(int p_width) {}

void CefControl::set_no_background(bool p_bg) {
	no_background = p_bg;
}

bool CefControl::get_no_background() const {
	return no_background;
}

void CefControl::set_url(const String& p_url) {
	home_url = p_url;
}

String CefControl::get_url() const {
	return home_url;
}

void CefControl::set_user_agent(const String& p_user_agent) {
	user_agent = p_user_agent;
}

String CefControl::get_user_agent() const {
	return user_agent;
}

double CefControl::get_zoom_level() const {
	return zoom;
}

void CefControl::set_zoom_level(double p_zoom) {
	zoom = p_zoom;
}

String CefControl::get_title() const {
	return "";
}

void CefControl::execute_java_script(const String &p_script) {}

void CefControl::load_string(const String &p_source) {}

bool CefControl::can_go_back() const {
	return false;
}

bool CefControl::can_go_forward() const {
	return false;
}

bool CefControl::is_ready() const {
	return false;
}

bool CefControl::is_loading() const {
	return false;
}

bool CefControl::is_secure_content() const {
	return false;
}

void CefControl::go_back() {}

void CefControl::go_forward() {}

void CefControl::reload() {}

void CefControl::stop() {}

void CefControl::init() {}

void CefControl::finish() {}


/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

void CefControl::_bind_methods() {
	ClassDB::bind_method(D_METHOD("can_go_back"), &CefControl::can_go_back);
	ClassDB::bind_method(D_METHOD("can_go_forward"), &CefControl::can_go_forward);

	ClassDB::bind_method(D_METHOD("is_loading"), &CefControl::is_loading);
	ClassDB::bind_method(D_METHOD("is_ready"), &CefControl::is_ready);
	ClassDB::bind_method(D_METHOD("is_secure_content"), &CefControl::is_secure_content);

	ClassDB::bind_method(D_METHOD("go_back"), &CefControl::go_back);
	ClassDB::bind_method(D_METHOD("go_forward"), &CefControl::go_forward);
	ClassDB::bind_method(D_METHOD("reload"), &CefControl::reload);
	ClassDB::bind_method(D_METHOD("stop"), &CefControl::stop);

	ClassDB::bind_method(D_METHOD("set_no_background", "no_background"), &CefControl::set_no_background);
	ClassDB::bind_method(D_METHOD("get_no_background"), &CefControl::get_no_background);

	ClassDB::bind_method(D_METHOD("set_url", "url"), &CefControl::set_url);
	ClassDB::bind_method(D_METHOD("get_url"), &CefControl::get_url);

	ClassDB::bind_method(D_METHOD("set_user_agent", "user_agent"), &CefControl::set_user_agent);
	ClassDB::bind_method(D_METHOD("get_user_agent"), &CefControl::get_user_agent);

	ClassDB::bind_method(D_METHOD("get_zoom_level"), &CefControl::get_zoom_level);
	ClassDB::bind_method(D_METHOD("set_zoom_level", "zoom"), &CefControl::set_zoom_level);

	ClassDB::bind_method(D_METHOD("load_string", "source"), &CefControl::load_string);
	ClassDB::bind_method(D_METHOD("execute_java_script", "script"), &CefControl::execute_java_script);

	ClassDB::bind_method(D_METHOD("get_snapshot", "width"), &CefControl::get_snapshot);

	ClassDB::bind_method(D_METHOD("get_title"), &CefControl::get_title);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_background"), "set_no_background", "get_no_background");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "url"), "set_url", "get_url");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "user_agent"), "set_user_agent", "get_user_agent");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "zoom_level"), "set_zoom_level", "get_zoom_level");

	ADD_SIGNAL(MethodInfo("callback", PropertyInfo(Variant::STRING, "url")));
	ADD_SIGNAL(MethodInfo("new_window", PropertyInfo(Variant::STRING, "url")));
	ADD_SIGNAL(MethodInfo("start_navigation"));
	ADD_SIGNAL(MethodInfo("finish_navigation"));
	ADD_SIGNAL(MethodInfo("snapshot_ready", PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Image")));
}

void CefControl::_draw_placeholder() {
	Ref<Font> font = get_font("font", "Label");
	Size2i size = get_size();

	// Main border
	draw_rect(Rect2(Point2(), size), Color(0, 0, 0), false);
	draw_rect(Rect2(Point2(1, 1), size - Size2(2, 2)), Color(1, 1, 1), false);
	draw_rect(Rect2(Point2(2, 2), size - Size2(4, 4)), Color(1, 1, 1), false);
	draw_rect(Rect2(Point2(3, 3), size - Size2(6, 6)), Color(0, 0, 0), false);

	if (icon_main.is_null()) {
		Ref<Image> image = memnew(Image(__icon_dummy, __icon_dummy_len));
		icon_main.instance();
		icon_main->create_from_image(image, Texture::FLAG_VIDEO_SURFACE);
	}

	icon_main->draw(get_canvas_item(), Vector2((size.x - 64) / 2, 20));

	Size2i url_size = font->get_string_size(home_url);
	font->draw(get_canvas_item(), Vector2((size.x - url_size.x) / 2, 100 +url_size.y) + Vector2(1, 1), home_url, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - url_size.x) / 2, 100 +url_size.y) + Vector2(-1, 1), home_url, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - url_size.x) / 2, 100 +url_size.y) + Vector2(1, -1), home_url, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - url_size.x) / 2, 100 +url_size.y) + Vector2(-1, -1), home_url, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - url_size.x) / 2, 100 +url_size.y), home_url, Color(1, 1, 1));
}

void CefControl::_draw_error(const String &p_error) {
	Ref<Font> font = get_font("font", "Label");
	Size2i size = get_size();

	// Main border
	draw_rect(Rect2(Point2(), size), Color(0, 0, 0), false);
	draw_rect(Rect2(Point2(1, 1), size - Size2(2, 2)), Color(1, 1, 1), false);
	draw_rect(Rect2(Point2(2, 2), size - Size2(4, 4)), Color(1, 1, 1), false);
	draw_rect(Rect2(Point2(3, 3), size - Size2(6, 6)), Color(0, 0, 0), false);

	if (icon_error.is_null()) {
		Ref<Image> image = memnew(Image(__icon_error, __icon_error_len));
		icon_error.instance();
		icon_error->create_from_image(image, Texture::FLAG_VIDEO_SURFACE);
	}

	icon_error->draw(get_canvas_item(), Vector2((size.x - 64) / 2, 20));

	// Error message
	String err_ti = "-- WEBVIEW ERROR --";
	Size2i er_size = font->get_string_size(p_error);
	Size2i ti_size = font->get_string_size(err_ti);

	font->draw(get_canvas_item(), Vector2((size.x - ti_size.x) / 2, 100 + ti_size.y) + Vector2(1, 1), err_ti, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - ti_size.x) / 2, 100 + ti_size.y) + Vector2(-1, 1), err_ti, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - ti_size.x) / 2, 100 + ti_size.y) + Vector2(1, -1), err_ti, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - ti_size.x) / 2, 100 + ti_size.y) + Vector2(-1, -1), err_ti, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - ti_size.x) / 2, 100 + ti_size.y), err_ti, Color(1, 0, 0));

	font->draw(get_canvas_item(), Vector2((size.x - er_size.x) / 2, 100 + er_size.y + 10 + ti_size.y) + Vector2(1, 1), p_error, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - er_size.x) / 2, 100 + er_size.y + 10 + ti_size.y) + Vector2(-1, 1), p_error, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - er_size.x) / 2, 100 + er_size.y + 10 + ti_size.y) + Vector2(1, -1), p_error, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - er_size.x) / 2, 100 + er_size.y + 10 + ti_size.y) + Vector2(-1, -1), p_error, Color(0, 0, 0, 0.5));
	font->draw(get_canvas_item(), Vector2((size.x - er_size.x) / 2, 100 + er_size.y + 10 + ti_size.y), p_error, Color(1, 0, 0));
}
