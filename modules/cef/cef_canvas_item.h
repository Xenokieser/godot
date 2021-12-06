#pragma once
#include "scene/main/node.h"

#include "scene/2d/node_2d.h"



class CefCanvasItem : public Node2D
{

	GDCLASS(CefCanvasItem, Node2D);

	public:
		void loadUrl(String* url);
	protected:
		void _init();
        void _process(float delta);
        void _draw();
};

