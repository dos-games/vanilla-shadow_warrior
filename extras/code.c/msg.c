//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2005 - 3D Realms Entertainment

This file is NOT part of Shadow Warrior version 1.2
However, it is either an older version of a file that is, or is
some test code written during the development of Shadow Warrior.
This file is provided purely for educational interest.

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

msg_junk()
    {

    if (typemode != 0)
        {
        charsperline = 40;

        for (i = 0; i <= typemessageleng; i += charsperline)
            {
            for (j = 0; j < charsperline; j++)
                tempbuf[j] = typemessage[i + j];

            if (typemessageleng < i + charsperline)
                {
                tempbuf[(typemessageleng - i)] = '_';
                tempbuf[(typemessageleng - i) + 1] = 0;
                }
            else
                tempbuf[charsperline] = 0;

            printext256(0L, (i / charsperline) << 3, 183, -1, tempbuf, 0);
            }
        }

    if (getmessageleng > 0)
        {
        charsperline = 40;

        for (i = 0; i <= getmessageleng; i += charsperline)
            {
            for (j = 0; j < charsperline; j++)
                tempbuf[j] = getmessage[i + j];

            if (getmessageleng < i + charsperline)
                tempbuf[(getmessageleng - i)] = 0;
            else
                tempbuf[charsperline] = 0;

            printext256(0L, ((i / charsperline) << 3) + (200 - 32 - 8) - (((getmessageleng - 1) / charsperline) << 3), 151, -1, tempbuf, 0);
            }

        if (totalclock > getmessagetimeoff)
            getmessageleng = 0;
        }

    while (keyfifoplc != keyfifoend)
        {
        ch = keyfifo[keyfifoplc];
        keystate = keyfifo[(keyfifoplc + 1) & (KEYFIFOSIZ - 1)];
        keyfifoplc = ((keyfifoplc + 2) & (KEYFIFOSIZ - 1));

        if (keystate != 0)
            {
            if (ch == 0xe)              // Backspace
                {
                if (typemessageleng == 0)
                    {
                    typemode = 0;
                    break;
                    }
                typemessageleng--;
                }
            if (ch == 0xf)
                {
                keystatus[0xf] = 0;
                typemode = 0;
                break;
                }
            if ((ch == 0x1c) || (ch == 0x9c))   // Either ENTER
                {
                keystatus[0x1c] = 0;
                keystatus[0x9c] = 0;
                if (typemessageleng > 0)
                    {
                    tempbuf[0] = 2;     // Sending text is message type 4
                    for (j = typemessageleng - 1; j >= 0; j--)
                        tempbuf[j + 1] = typemessage[j];

					netbroadcastpacket(tempbuf, typemessageleng + 1);	// TENSW

                    typemessageleng = 0;
                    }
                typemode = 0;
                break;
                }

            if ((typemessageleng < 159) && (ch < 128))
                {
                if ((keystatus[0x2a] | keystatus[0x36]) != 0)
                    ch = scantoascwithshift[ch];
                else
                    ch = scantoasc[ch];

                if (ch != 0)
                    typemessage[typemessageleng++] = ch;
                }
            }
        }

    // Here's a trick of making key repeat after a 1/2 second
    if (keystatus[0xe] > 0)
        {
        if (keystatus[0xe] < 30)
            keystatus[0xe] += TICSPERFRAME;
        else
            {
            if (typemessageleng == 0)
                typemode = 0;
            else
                typemessageleng--;
            }
        }
    }

#if 0
//getpackets
case 2:
getmessageleng = tempbufleng - 1;
for (j = getmessageleng - 1; j >= 0; j--)
    getmessage[j] = tempbuf[j + 1];
getmessagetimeoff = totalclock + 360 + (getmessageleng << 4);
break;
#endif
