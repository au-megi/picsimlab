/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2018  Luis Claudio Gambôa Lopes

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

#include"../picsimlab1.h"
#include"../picsimlab4.h"
#include"../picsimlab5.h"
#include"part_gamepad.h"

/* outputs */
enum
{
 O_P1, O_P2, O_P3, O_P4, O_P5, O_P6, O_P7, O_P8, O_B1, O_B2, O_B3, O_B4, O_B5, O_B6, O_B7, O_B8, O_J1
};

/* inputs */
enum
{
 I_B1, I_B2, I_B3, I_B4, I_B5, I_B6, I_B7, I_B8, I_J1
};

cpart_gamepad::cpart_gamepad (unsigned x, unsigned y)
{
 X = x;
 Y = y;
 ReadMaps ();

 lxImage image;
 image.LoadFile (Window1.GetSharePath () + lxT ("parts/") + GetPictureFileName ());

 Bitmap = new lxBitmap (image, &Window5);
 image.Destroy(); 

 canvas.Create (Window1.GetWWidget(),Bitmap);



 output_pins[0] = 0;
 output_pins[1] = 0;
 output_pins[2] = 0;
 output_pins[3] = 0;
 output_pins[4] = 0;
 output_pins[5] = 0;
 output_pins[6] = 0;
 output_pins[7] = 0;

 output_value[0] = 1;
 output_value[1] = 1;
 output_value[2] = 1;
 output_value[3] = 1;
 output_value[4] = 1;
 output_value[5] = 1;
 output_value[6] = 1;
 output_value[7] = 1;

 active = 0;

 jr = 10;
 for (int i = 0; i < outputc; i++)
  {
   switch (output[i].id)
    {
    case O_J1:
     jr = ((output[i].x2 - output[i].x1) / 2) - 10;
     break;
    }
  }

 valuex = jr;
 valuey = jr;
}

cpart_gamepad::~cpart_gamepad (void)
{
 delete Bitmap;
}

void
cpart_gamepad::Draw (void)
{

 int i;

 board *pboard = Window1.GetBoard ();

 canvas.Init ();

 lxFont font (9, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD);
 canvas.SetFont (font);

 for (i = 0; i < outputc; i++)
  {

   switch (output[i].id)
    {
    case O_P1:
    case O_P2:
    case O_P3:
    case O_P4:
    case O_P5:
    case O_P6:
    case O_P7:
    case O_P8:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     if (output_pins[output[i].id - O_P1] == 0)
      canvas.Text ("NC", output[i].x1, output[i].y1);
     else
      canvas.Text (pboard->MGetPinName (output_pins[output[i].id - O_P1]), output[i].x1, output[i].y1);
     break;
    case O_J1:
     canvas.SetColor (50, 50, 50);
     //canvas.Rectangle (1,output[i].x1,output[i].y1,output[i].x2-output[i].x1,output[i].y2-output[i].y1);
     canvas.Circle (1, output[i].x1 + jr + 10, output[i].y1 + jr + 10, jr + 10);
     canvas.SetColor (250, 250, 250);
     //canvas.Rectangle (1,output[i].x1+valuex,output[i].y1+valuey,10,10);
     canvas.Circle (1, output[i].x1 + valuex + 10, output[i].y1 + valuey + 10, 8);
     break;
    }


  };

 canvas.End ();

}

void
cpart_gamepad::Process (void)
{

 if (refresh > 1000)
  {
   board *pboard = Window1.GetBoard ();
   refresh = 0;

   pboard->MSetPin (output_pins[0], output_value[0]);
   pboard->MSetPin (output_pins[1], output_value[1]);
   pboard->MSetPin (output_pins[2], output_value[2]);
   pboard->MSetPin (output_pins[3], output_value[3]);
   pboard->MSetPin (output_pins[4], output_value[4]);
   pboard->MSetPin (output_pins[5], output_value[5]);
   pboard->MSetAPin (output_pins[6], 2.5 * (valuex) / jr);
   pboard->MSetAPin (output_pins[7], 2.5 * (valuey) / jr);
  }
 refresh++;
}

void
cpart_gamepad::EvMouseButtonPress (uint button, uint x, uint y, uint state)
{
 int i;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {

     switch (input[i].id)
      {
      case I_B1: output_value[0] = 0;
       break;
      case I_B2: output_value[1] = 0;
       break;
      case I_B3: output_value[2] = 0;
       break;
      case I_B4: output_value[3] = 0;
       break;
      case I_B5: output_value[4] = 0;
       break;
      case I_B6: output_value[5] = 0;
       break;
      case I_J1:

       float cx = ((float) (x - input[i].x1)) - jr;
       float cy = ((float) (y - input[i].y1)) - jr;

       float module = sqrt (cx * cx + cy * cy);
       float angle = atan2 ((cy), (cx));

       if (module > jr)module = jr;
       valuex = module * cos (angle) + jr;
       valuey = module * sin (angle) + jr;

       active = 1;

       break;
      }
    }
  }
};

void
cpart_gamepad::EvMouseButtonRelease (uint button, uint x, uint y, uint state)
{
 int i;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {
     switch (input[i].id)
      {
      case I_B1: output_value[0] = 1;
       break;
      case I_B2: output_value[1] = 1;
       break;
      case I_B3: output_value[2] = 1;
       break;
      case I_B4: output_value[3] = 1;
       break;
      case I_B5: output_value[4] = 1;
       break;
      case I_B6: output_value[5] = 1;
       break;
      case I_B7: output_value[6] = 1;
       break;
      case I_J1: active = 0;
       valuex = jr;
       valuey = jr;
       break;
      }
    }
  }
}

void
cpart_gamepad::EvMouseMove (uint button, uint x, uint y, uint state)
{

 int i;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {
     switch (input[i].id)
      {
      case I_J1:
       if (active)
        {

         float cx = ((float) (x - input[i].x1)) - jr;
         float cy = ((float) (y - input[i].y1)) - jr;

         float module = sqrt (cx * cx + cy * cy);
         float angle = atan2 ((cy), (cx));

         if (module > jr)module = jr;
         valuex = module * cos (angle) + jr;
         valuey = module * sin (angle) + jr;

        }
       break;
      }
    }
  }
}

void
cpart_gamepad::EvKeyPress (uint key, uint x, uint y, uint mask)
{

 switch (key)
  {
  case 'W':
   valuex = jr;
   valuey = 0;
   break;
  case 'A':
   valuex = 0;
   valuey = jr;
   break;
  case 'S':
   valuex = jr;
   valuey = 2 * jr;
   break;
  case 'D':
   valuex = 2 * jr;
   valuey = jr;
   break;
  case 'I':
   output_value[0] = 0;
   break;
  case 'J':
   output_value[3] = 0;
   break;
  case 'K':
   output_value[2] = 0;
   break;
  case 'L':
   output_value[1] = 0;
   break;
  case 'E':
   output_value[4] = 0;
   break;
  case 'O':
   output_value[5] = 0;
   break;
  case 'R':
   output_value[6] = 0;
   break;
  }
}

void
cpart_gamepad::EvKeyRelease (uint key, uint x, uint y, uint mask)
{
 switch (key)
  {
  case 'W':
  case 'A':
  case 'S':
  case 'D':
   valuex = jr;
   valuey = jr;
   break;
  case 'I':
   output_value[0] = 1;
   break;
  case 'J':
   output_value[3] = 1;
   break;
  case 'K':
   output_value[2] = 1;
   break;
  case 'L':
   output_value[1] = 1;
   break;
  case 'E':
   output_value[4] = 1;
   break;
  case 'O':
   output_value[5] = 1;
   break;
  case 'R':
   output_value[6] = 1;
   break;
  }
}

unsigned short
cpart_gamepad::get_in_id (char * name)
{
 if (strcmp (name, "B1") == 0)return I_B1;
 if (strcmp (name, "B2") == 0)return I_B2;
 if (strcmp (name, "B3") == 0)return I_B3;
 if (strcmp (name, "B4") == 0)return I_B4;
 if (strcmp (name, "B5") == 0)return I_B5;
 if (strcmp (name, "B6") == 0)return I_B6;

 if (strcmp (name, "J1") == 0)return I_J1;

 printf ("Erro input '%s' don't have a valid id! \n", name);
 return -1;
};

unsigned short
cpart_gamepad::get_out_id (char * name)
{

 if (strcmp (name, "P1") == 0)return O_P1;
 if (strcmp (name, "P2") == 0)return O_P2;
 if (strcmp (name, "P3") == 0)return O_P3;
 if (strcmp (name, "P4") == 0)return O_P4;
 if (strcmp (name, "P5") == 0)return O_P5;
 if (strcmp (name, "P6") == 0)return O_P6;
 if (strcmp (name, "P7") == 0)return O_P7;
 if (strcmp (name, "P8") == 0)return O_P8;

 if (strcmp (name, "B1") == 0)return O_B1;
 if (strcmp (name, "B2") == 0)return O_B2;
 if (strcmp (name, "B3") == 0)return O_B3;
 if (strcmp (name, "B4") == 0)return O_B4;
 if (strcmp (name, "B5") == 0)return O_B5;
 if (strcmp (name, "B6") == 0)return O_B6;
 if (strcmp (name, "B7") == 0)return O_B7;
 if (strcmp (name, "B8") == 0)return O_B8;

 if (strcmp (name, "J1") == 0)return O_J1;

 printf ("Erro output '%s' don't have a valid id! \n", name);
 return 1;
};

String
cpart_gamepad::WritePreferences (void)
{
 char prefs[256];

 sprintf (prefs, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", output_pins[0], output_pins[1], output_pins[2], output_pins[3], output_pins[4], output_pins[5], output_pins[6], output_pins[7]);

 return prefs;
};

void
cpart_gamepad::ReadPreferences (String value)
{
 sscanf (value.c_str (), "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &output_pins[0], &output_pins[1], &output_pins[2], &output_pins[3], &output_pins[4], &output_pins[5], &output_pins[6], &output_pins[7]);
};

CPWindow * WProp_gamepad;

void
cpart_gamepad::ConfigurePropertiesWindow (CPWindow * wprop)
{
 String Items = "0  NC,";
 String spin;
 WProp_gamepad = wprop;
 board *pboard = Window1.GetBoard ();

 for (int i = 1; i <= pboard->MGetPinCount (); i++)
  {
   spin = pboard->MGetPinName (i);

   if (spin.Cmp (lxT ("error")))
    {
     Items = Items + itoa (i) + "  " + spin + ",";
    }
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo1"))->SetItems (Items);
 if (output_pins[0] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo1"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[0]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo1"))->SetText (itoa (output_pins[0]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo2"))->SetItems (Items);
 if (output_pins[1] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo2"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[1]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo2"))->SetText (itoa (output_pins[1]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo3"))->SetItems (Items);
 if (output_pins[2] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo3"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[2]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo3"))->SetText (itoa (output_pins[2]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo4"))->SetItems (Items);
 if (output_pins[3] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo4"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[3]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo4"))->SetText (itoa (output_pins[3]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo5"))->SetItems (Items);
 if (output_pins[4] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo5"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[4]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo5"))->SetText (itoa (output_pins[4]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo6"))->SetItems (Items);
 if (output_pins[5] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo6"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[5]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo6"))->SetText (itoa (output_pins[5]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo7"))->SetItems (Items);
 if (output_pins[6] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo7"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[6]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo7"))->SetText (itoa (output_pins[6]) + "  " + spin);
  }

 ((CCombo*) WProp_gamepad->GetChildByName ("combo8"))->SetItems (Items);
 if (output_pins[7] == 0)
  ((CCombo*) WProp_gamepad->GetChildByName ("combo8"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (output_pins[7]);
   ((CCombo*) WProp_gamepad->GetChildByName ("combo8"))->SetText (itoa (output_pins[7]) + "  " + spin);
  }

 ((CButton*) WProp_gamepad->GetChildByName ("button1"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & cpart_gamepad::PropButton;
 ((CButton*) WProp_gamepad->GetChildByName ("button1"))->SetTag (1);

 ((CButton*) WProp_gamepad->GetChildByName ("button2"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & cpart_gamepad::PropButton;
}

void
cpart_gamepad::ReadPropertiesWindow (void)
{
 if (WProp_gamepad->GetTag ())
  {
   output_pins[0] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo1"))->GetText ());
   output_pins[1] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo2"))->GetText ());
   output_pins[2] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo3"))->GetText ());
   output_pins[3] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo4"))->GetText ());
   output_pins[4] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo5"))->GetText ());
   output_pins[5] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo6"))->GetText ());
   output_pins[6] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo7"))->GetText ());
   output_pins[7] = atoi (((CCombo*) WProp_gamepad->GetChildByName ("combo8"))->GetText ());
  }
}

void
cpart_gamepad::PropButton (CControl * control, uint button, uint x, uint y, uint state)
{
 WProp_gamepad->SetTag (control->GetTag ());
 WProp_gamepad->HideExclusive ();
};