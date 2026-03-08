/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "message_box.h"
#include "stdio.h"
#include "stdlib.h"

struct _Message_Box* Create_Message_Box()
{
    struct _Message_Box* Message_Box = (struct _Message_Box*) calloc(1,sizeof (struct _Message_Box));
    return Message_Box;
};
void Initialize_Message_Box(struct _Message_Box* Message_Box, struct _Text_Engine* Text_Engine, unsigned int Type, char Text[256])
{
    Message_Box->Message_Box_Background = Create_Image();

    Message_Box->x = 640-320-160;
    Message_Box->y = 360-180-90;

    Message_Box->Type = Type;

    char Text2[256];
    sprintf(Text2, "Message BOX!");
    Text[13] = '\0';

    Load_Image(Message_Box->Message_Box_Background, "resources/artwork/gui/buttonstock.bmp");
    Message_Box->Message_Box_Text = Create_Image();
    Initialize_Text_Image(Text_Engine, Message_Box->Message_Box_Text,Text2,13,13*17,24);



    switch(Message_Box->Type)
    {
        case UNDERSTOOD:
            Message_Box->Understood = Create_Image();
            Load_Image(Message_Box->Understood, "resources/artwork/gui/understood.bmp");
        break;

        case UNDERSTOOD_CANCEL:
            Message_Box->Understood = Create_Image();
            Load_Image(Message_Box->Understood, "resources/artwork/gui/understood.bmp");

            Message_Box->Cancel = Create_Image();
            Load_Image(Message_Box->Cancel, "resources/artwork/gui/cancel.bmp");
        break;

        case ABORT_RETRY_IGNORE:
            Message_Box->Abort = Create_Image();
            Load_Image(Message_Box->Abort, "resources/artwork/gui/abort.bmp");

            Message_Box->Retry = Create_Image();
            Load_Image(Message_Box->Retry, "resources/artwork/gui/retry.bmp");

            Message_Box->Ignore = Create_Image();
            Load_Image(Message_Box->Ignore, "resources/artwork/gui/ignore.bmp");
        break;

        case YES_NO_CANCEL:
            Message_Box->Yes = Create_Image();
            Load_Image(Message_Box->Yes, "resources/artwork/gui/yes.bmp");

            Message_Box->No = Create_Image();
            Load_Image(Message_Box->No, "resources/artwork/gui/no.bmp");

            Message_Box->Cancel = Create_Image();
            Load_Image(Message_Box->Cancel, "resources/artwork/gui/cancel.bmp");
        break;

        case YES_NO:
            Message_Box->Yes= Create_Image();
            Load_Image(Message_Box->Yes, "resources/artwork/gui/yes.bmp");

            Message_Box->No = Create_Image();
            Load_Image(Message_Box->No, "resources/artwork/gui/no.bmp");
        break;

        case RETRY_CANCEL:
            Message_Box->Retry = Create_Image();
            Load_Image(Message_Box->Retry, "resources/artwork/gui/retry.bmp");

            Message_Box->Cancel = Create_Image();
            Load_Image(Message_Box->Cancel, "resources/artwork/gui/cancel.bmp");
        break;
    }
};

void Process_Message_Box(struct _Message_Box* Message_Box)
{

};
void Render_Message_Box(struct _Message_Box* Message_Box)
{
    Render_Image(Message_Box->Message_Box_Background,Message_Box->x,Message_Box->y,1);
    switch(Message_Box->Type)
    {
        case UNDERSTOOD:
            Render_Image(Message_Box->Understood,Message_Box->x+160-48,Message_Box->y+96+16+8,1);
        break;

        case UNDERSTOOD_CANCEL:
            Render_Image(Message_Box->Understood,Message_Box->x+8+32,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->Cancel,Message_Box->x+311-96-32,Message_Box->y+96+16+8,1);
        break;

        case ABORT_RETRY_IGNORE:
            Render_Image(Message_Box->Abort,Message_Box->x+8,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->Retry,Message_Box->x+96+8+8,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->Ignore,Message_Box->x+311-96,Message_Box->y+96+16+8,1);
        break;

        case YES_NO_CANCEL:
            Render_Image(Message_Box->Yes,Message_Box->x+8,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->No,Message_Box->x+96+8+8,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->Cancel,Message_Box->x+311-96,Message_Box->y+96+16+8,1);
        break;

        case YES_NO:
            Render_Image(Message_Box->Yes,Message_Box->x+8+32,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->No,Message_Box->x+311-96-32,Message_Box->y+96+16+8,1);
        break;

        case RETRY_CANCEL:
            Render_Image(Message_Box->Retry,Message_Box->x+8+32,Message_Box->y+96+16+8,1);
            Render_Image(Message_Box->Cancel,Message_Box->x+311-96-32,Message_Box->y+96+16+8,1);
        break;
    }
    Render_Image(Message_Box->Message_Box_Text,Message_Box->x+8,Message_Box->y+8,1);
};
