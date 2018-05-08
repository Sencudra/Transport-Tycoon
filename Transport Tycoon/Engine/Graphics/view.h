#ifndef VIEW_H
#define VIEW_H

#include "resources.h"



using namespace sf;

class ScreenView : public sf::View
{
public:

    ScreenView();
    ScreenView(sf::Vector2f size);
    ~ScreenView();

	void update(float time);
    rs::ScreenRectangle getViewRect(){return m_screenPos;}

private:

	void viewMoving(float time);
	void viewScaling();
    void updatePos();
    rs::ScreenRectangle m_screenPos; // view position by TL and BR corners

    int m_scrX;
    int m_scrY;
};



#endif // VIEW_H
