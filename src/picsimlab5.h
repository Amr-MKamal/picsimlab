/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2017  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#ifndef CPWINDOW5 
#define CPWINDOW5

#include<lxrad/lxrad.h>
#include"part.h"
#include"parts_defs.h"

class CPWindow5:public CPWindow
{
  public:
//lxrad automatic generated block start, don't edit below!
  /*#Controls*/
  CMenu menu1;
  CDraw draw1;
  CPMenu menu1_File;
  CPMenu menu1_Add;
  CPMenu pmenu2;
  CItemMenu pmenu2_Propierties;
  CTimer timer1;
  CItemMenu menu1_File_Saveconfiguration;
  CItemMenu menu1_File_Loadconfiguration;
  CItemMenu pmenu2_Move;
  CItemMenu pmenu2_Delete;
  /*#Events*/
  void _EvOnCreate(CControl * control);
  void _EvOnShow(CControl * control);
  void _EvOnHide(CControl * control);
  void draw1_EvMouseMove(CControl * control, uint button, uint x, uint y,uint state);
  void draw1_EvMouseButtonPress(CControl * control, uint button, uint x, uint y,uint state);
  void draw1_EvMouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state);
  void draw1_EvKeyboardPress(CControl * control, uint key, uint x, uint y,uint mask);
  void draw1_EvKeyboardRelease(CControl * control, uint key, uint x, uint y,uint mask);
  void pmenu2_Propierties_EvMenuActive(CControl * control);
  void timer1_EvOnTime(CControl * control);
  void menu1_File_Saveconfiguration_EvMenuActive(CControl * control);
  void menu1_File_Loadconfiguration_EvMenuActive(CControl * control);
  void pmenu2_Move_EvMenuActive(CControl * control);
  void pmenu2_Delete_EvMenuActive(CControl * control);

  /*#Others*/
  CPWindow5(void);
//lxrad automatic generated block end, don't edit above!
  void menu1_EvMenuActive(CControl * control);
  void Process(void);
  private:
    int partsc;  
    part *parts[MAX_PARTS];
    CItemMenu MParts[NUM_PARTS];
    String PartToCreate;
    int PartSelected;
    int PartToMove;
    int mdx,mdy;
    CStringList PinNames;
};

extern CPWindow5 Window5 ;

#endif /*#CPWINDOW5*/
