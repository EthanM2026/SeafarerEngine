/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MESSAGE_BOX_H_INCLUDED
#define MESSAGE_BOX_H_INCLUDED

#define UNDERSTOOD 1
#define UNDERSTOOD_CANCEL 2
#define ABORT_RETRY_IGNORE 3
#define YES_NO_CANCEL 4
#define YES_NO 5
#define RETRY_CANCEL 6
#include "image.h"
#include "../engine.h"

struct _Message_Box
{
    struct _Image* Message_Box_Background;
    struct _Image* Message_Box_Text;
    struct _Image* Yes;
    struct _Image* No;
    struct _Image* Understood;
    struct _Image* Cancel;
    struct _Image* Abort;
    struct _Image* Retry;
    struct _Image* Ignore;
    bool Selection_Made;
    int Selection_ID;
    int Number_Of_Buttons;
    int Selector_X;
    int Type;

    float x;
    float y;
};

struct _Message_Box* Create_Message_Box();
void Initialize_Message_Box(struct _Message_Box* Message_Box, struct _Text_Engine* Text_Engine, unsigned int Type, char Text[256]);

void Process_Message_Box(struct _Message_Box* Message_Box);
void Render_Message_Box(struct _Message_Box* Message_Box);



#endif // MESSAGE_BOX_H_INCLUDED







