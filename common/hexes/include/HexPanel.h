#ifndef _HEXES_HEXPANEL_H_
#define _HEXES_HEXPANEL_H_

#include <string>
#include <list>

#include <panel.h>
#include <HexApp.h>


namespace hexes {

class HexApp;
class HexWindow;
class HexOutputInterface;
class HexInputInterface;

typedef std::list<std::string> TextList;


class HexPanel {

  public:

    HexPanel ( const std::string  & title = "" );

    HexPanel ( const std::string  & title,
               int   height, int    width,
               int   starty, int    startx );
    
    HexPanel ( const std::string  & title,
               HexOutputInterface * output,
               HexInputInterface  * input,
               int   height, int    width,
               int   starty, int    startx );

    virtual ~HexPanel();

    virtual int         handleDisplay();
    virtual int         handleInput ( int ch );

    virtual int         redraw();
    virtual int         poll();

    void                setOutputHandler ( HexOutputInterface * output );
    void                setInputHandler  ( HexInputInterface  * input  );

    int                 print ( const std::string & str, bool wrap = false );
    int                 print ( const char ch );
    int                 echo  ( const char ch );

    void                show();
    void                hide();
    void                erase();
    void                refresh();
    void                scrollLine();
    void                timeout      ( int delay_ms );
    int                 move         ( int y, int x );
    int                 wrap();

    int                 width();
    int                 height();
    int                 curY();
    int                 curX();
    int                 maxY();
    int                 maxX();

    TextList&           getTextList();
    void                setTextList  ( TextList & textlist );
    void                addText      ( const std::string & str );
    //, bool wrap = false );
    void                setText      ( const std::string & str );


    void                setTopPanel();
    void                enableBorder ( bool border );
    void                enableTitle  ( bool title );
    void                enableScroll ( bool scroll );
    bool                scrollable() const;
    const std::string&  getTitle() const;


  protected:

    void                initPanel();


  private:
   
    HexWindow *          _hwin;
    PANEL *              _panel;

    HexOutputInterface * _output;
    HexInputInterface  * _input;

    std::string          _title;
    TextList             _textlist;

    int                  _height, _width;
    int                  _starty, _startx;
    int                  _selected;
    bool                 _scrollable;
    bool                 _drawBorder;
    bool                 _drawTitle;
};


}  // namespace

#endif // _HEXES_HEXPANEL_H_

