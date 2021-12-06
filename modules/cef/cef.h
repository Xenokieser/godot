#ifndef STIGMEE_CEF_H
#define STIGMEE_CEF_H

#include "scene/main/node.h"



class Cef : public Node
{
    GDCLASS(Cef, Node);

public:

    Cef();
	~Cef();

	void do_action(int value);
	


protected:
	void _init();
    void _process(float delta);
    void _draw();
    static void _bind_methods();
};

#endif // STIGMEE_CEF_H
