/*=============================================================================
    Name    : FeReg.c
    Purpose : Front End Texture registry logic and data
              "/FETextures to activate"

    Created 3/6/1998 by fpoiker
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "FEReg.h"

#include <string.h>

#include "Color.h"
#include "Debug.h"
#include "glinc.h"
#include "HorseRace.h"
#include "main.h"
#include "Memory.h"
#include "prim2d.h"
#include "render.h"


#define DRAW_CUTOUTS 1

#define SUB_ADJUST 1
//#define FER_CUTOUT_COLOR    colRGB(11, 27, 66)

#define slider_end_width 8
#define slider_vert_end_width 0

/*=============================================================================
    Data:
=============================================================================*/

//to speed up texture loading, the registry refers to the texture number
//rather than the texture name.  The names are stored in an array and the number
//corresponds to their array location.
//box textures
typedef enum
{
    BOX_OUTER_CORNER,
    BOX_INNER_CORNER,
    BOX_OUTGLOW_STRAIGHT,
    BOX_INGLOW_STRAIGHT,
    BOX_SMALL_OUTER_CORNER,
    BOX_SMALL_INNER_CORNER,
    BOX_SMALL_OUTGLOW_STRAIGHT,
    BOX_SMALL_INGLOW_STRAIGHT,
    BOX_LARGE_OUTER_CORNER,
    BOX_LARGE_INNER_CORNER,
    BOX_LARGE_OUTGLOW_STRAIGHT,
    BOX_LARGE_INGLOW_STRAIGHT,
    BOX_NOGLOW_CORNER,
    BOX_NOGLOW_SMALL_CORNER,
    BOX_NOGLOW_LARGE_CORNER,
    BOX_NOGLOW_STRAIGHT,
    BUTTON_OFF_LEFT,
    BUTTON_OFF_MID,
    BUTTON_OFF_RIGHT,
    BUTTON_OFF_FOCUS_LEFT,
    BUTTON_OFF_FOCUS_MID,
    BUTTON_OFF_FOCUS_RIGHT,
    BUTTON_OFF_FOCUS_MOUSE_LEFT,
    BUTTON_OFF_FOCUS_MOUSE_MID,
    BUTTON_OFF_FOCUS_MOUSE_RIGHT,
    BUTTON_OFF_MOUSE_LEFT,
    BUTTON_OFF_MOUSE_MID,
    BUTTON_OFF_MOUSE_RIGHT,
    BUTTON_OFF_DISABLED_LEFT,
    BUTTON_OFF_DISABLED_MID,
    BUTTON_OFF_DISABLED_RIGHT,
    BUTTON_ON_FOCUS_MOUSE_LEFT,
    BUTTON_ON_FOCUS_MOUSE_MID,
    BUTTON_ON_FOCUS_MOUSE_RIGHT,
    TEXT_OFF_LEFT,
    TEXT_OFF_MID,
    TEXT_OFF_RIGHT,
    TEXT_OFF_FOCUS_LEFT,
    TEXT_OFF_FOCUS_MID,
    TEXT_OFF_FOCUS_RIGHT,
    TEXT_OFF_FOCUS_MOUSE_LEFT,
    TEXT_OFF_FOCUS_MOUSE_MID,
    TEXT_OFF_FOCUS_MOUSE_RIGHT,
    TEXT_OFF_MOUSE_LEFT,
    TEXT_OFF_MOUSE_MID,
    TEXT_OFF_MOUSE_RIGHT,
    TEXT_OFF_DISABLED_LEFT,
    TEXT_OFF_DISABLED_MID,
    TEXT_OFF_DISABLED_RIGHT,
    TEXT_ON_FOCUS_MOUSE_LEFT,
    TEXT_ON_FOCUS_MOUSE_MID,
    TEXT_ON_FOCUS_MOUSE_RIGHT,


    TOGGLE_ON_LEFT,
    TOGGLE_ON_MID,
    TOGGLE_ON_RIGHT,
    TOGGLE_ON_FOCUS_LEFT,
    TOGGLE_ON_FOCUS_MID,
    TOGGLE_ON_FOCUS_RIGHT,
    TOGGLE_ON_MOUSE_LEFT,
    TOGGLE_ON_MOUSE_MID,
    TOGGLE_ON_MOUSE_RIGHT,
    TOGGLE_ON_FOCUS_MOUSE_LEFT,
    TOGGLE_ON_FOCUS_MOUSE_MID,
    TOGGLE_ON_FOCUS_MOUSE_RIGHT,
    TOGGLE_ON_DISABLED_LEFT,
    TOGGLE_ON_DISABLED_MID,
    TOGGLE_ON_DISABLED_RIGHT,
    TOGGLE_OFF_LEFT,
    TOGGLE_OFF_MID,
    TOGGLE_OFF_RIGHT,
    TOGGLE_OFF_FOCUS_LEFT,
    TOGGLE_OFF_FOCUS_MID,
    TOGGLE_OFF_FOCUS_RIGHT,
    TOGGLE_OFF_MOUSE_LEFT,
    TOGGLE_OFF_MOUSE_MID,
    TOGGLE_OFF_MOUSE_RIGHT,
    TOGGLE_OFF_FOCUS_MOUSE_LEFT,
    TOGGLE_OFF_FOCUS_MOUSE_MID,
    TOGGLE_OFF_FOCUS_MOUSE_RIGHT,
    TOGGLE_OFF_DISABLED_LEFT,
    TOGGLE_OFF_DISABLED_MID,
    TOGGLE_OFF_DISABLED_RIGHT,

    CHECK_ON,
    CHECK_ON_FOCUS,
    CHECK_ON_MOUSE,
    CHECK_ON_FOCUS_MOUSE,
    CHECK_ON_DISABLED,
    CHECK_OFF,
    CHECK_OFF_FOCUS,
    CHECK_OFF_MOUSE,
    CHECK_OFF_FOCUS_MOUSE,
    CHECK_OFF_DISABLED,

    RADIO_ON,
    RADIO_ON_FOCUS,
    RADIO_ON_MOUSE,
    RADIO_ON_FOCUS_MOUSE,
    RADIO_ON_DISABLED,
    RADIO_OFF,
    RADIO_OFF_FOCUS,
    RADIO_OFF_MOUSE,
    RADIO_OFF_FOCUS_MOUSE,
    RADIO_OFF_DISABLED,

    VERTSB_TOP,
    VERTSB_MID,
    VERTSB_BOTTOM,
    VERTST_OFF_TOP,
    VERTST_OFF_MID,
    VERTST_OFF_BOTTOM,
    VERTST_OFF_MOUSE_TOP,
    VERTST_OFF_MOUSE_MID,
    VERTST_OFF_MOUSE_BOTTOM,
    VERTST_ON_TOP,
    VERTST_ON_MID,
    VERTST_ON_BOTTOM,
    VERTSA_OFF_UP,
    VERTSA_OFF_DOWN,
    VERTSA_OFF_MOUSE_UP,
    VERTSA_OFF_MOUSE_DOWN,
    VERTSA_ON_UP,
    VERTSA_ON_DOWN,

    LIST_NW_CORNER,
    LIST_NE_CORNER,
    LIST_SE_CORNER,
    LIST_SW_CORNER,
    LIST_N_STRAIGHT,
    LIST_E_STRAIGHT,
    LIST_S_STRAIGHT,
    LIST_W_STRAIGHT,
    LIST_MID,
    LIST_FOCUS_NW_CORNER,
    LIST_FOCUS_NE_CORNER,
    LIST_FOCUS_SE_CORNER,
    LIST_FOCUS_SW_CORNER,
    LIST_FOCUS_N_STRAIGHT,
    LIST_FOCUS_E_STRAIGHT,
    LIST_FOCUS_S_STRAIGHT,
    LIST_FOCUS_W_STRAIGHT,
    LIST_FOCUS_MID,

    MENU_MOUSEOVER_CENTRE,
    MENU_MOUSEOVER_LEFT_CAP,
    MENU_MOUSEOVER_RIGHT_CAP,
    MENU_SELECTED_DOT,
    MENU_POPOUT_ARROW,

    ARROW_LEFT_OFF,
    ARROW_LEFT_MOUSE,
    ARROW_LEFT_ON,
    ARROW_RIGHT_OFF,
    ARROW_RIGHT_MOUSE,
    ARROW_RIGHT_ON,

    BUILD_ARROW_LEFT_OFF,
    BUILD_ARROW_LEFT_MOUSE,
    BUILD_ARROW_LEFT_ON,
    BUILD_ARROW_RIGHT_OFF,
    BUILD_ARROW_RIGHT_MOUSE,
    BUILD_ARROW_RIGHT_ON,

    SHIPVIEW_LIGHTS_OFF,
    SHIPVIEW_LIGHTS_TOP,
    SHIPVIEW_LIGHTS_BOTTOM,
    SHIPVIEW_LIGHTS_BOTH,

    VOLUME_BUTTON,
    VOLUME_DISABLED_LEFTCAP,
    VOLUME_DISABLED_MID,
    VOLUME_DISABLED_RIGHTCAP,
    VOLUME_OFF_LEFTCAP,
    VOLUME_OFF_MID,
    VOLUME_OFF_RIGHTCAP,
    VOLUME_MOUSE_LEFTCAP,
    VOLUME_MOUSE_MID,
    VOLUME_MOUSE_RIGHTCAP,
    VOLUME_ON_LEFTCAP,
    VOLUME_ON_MID,
    VOLUME_ON_RIGHTCAP,

    EQ_BAR_BUTTON,
    EQ_BAR_MOUSE_STRAIGHT,
    EQ_BAR_ON_STRAIGHT,
    EQ_BAR_OFF_STRAIGHT,
    EQ_BAR_MOUSE_MIDDLE,
    EQ_BAR_ON_MIDDLE,
    EQ_BAR_OFF_MIDDLE,
    EQ_BAR_MOUSE_BOTCAP,
    EQ_BAR_ON_BOTCAP,
    EQ_BAR_OFF_BOTCAP,
    EQ_BAR_MOUSE_TOPCAP,
    EQ_BAR_ON_TOPCAP,
    EQ_BAR_OFF_TOPCAP,

    end_texholder
} tex_holder;

#define UNINITIALIZED_TEX_HOLDER  end_texholder

char *tex_names[end_texholder] =
{
    FER_BOX_OUTER_CORNER,
    FER_BOX_INNER_CORNER,
    FER_BOX_OUTGLOW_STRAIGHT,
    FER_BOX_INGLOW_STRAIGHT,
    FER_BOX_SMALL_OUTER_CORNER,
    FER_BOX_SMALL_INNER_CORNER,
    FER_BOX_SMALL_OUTGLOW_STRAIGHT,
    FER_BOX_SMALL_INGLOW_STRAIGHT,
    FER_BOX_LARGE_OUTER_CORNER,
    FER_BOX_LARGE_INNER_CORNER,
    FER_BOX_LARGE_OUTGLOW_STRAIGHT,
    FER_BOX_LARGE_INGLOW_STRAIGHT,
    FER_BOX_NOGLOW_CORNER,
    FER_BOX_NOGLOW_SMALL_CORNER,
    FER_BOX_NOGLOW_LARGE_CORNER,
    FER_BOX_NOGLOW_STRAIGHT,

    FER_BUTTON_OFF_LEFT,
    FER_BUTTON_OFF_MID,
    FER_BUTTON_OFF_RIGHT,
    FER_BUTTON_OFF_FOCUS_LEFT,
    FER_BUTTON_OFF_FOCUS_MID,
    FER_BUTTON_OFF_FOCUS_RIGHT,
    FER_BUTTON_OFF_FOCUS_MOUSE_LEFT,
    FER_BUTTON_OFF_FOCUS_MOUSE_MID,
    FER_BUTTON_OFF_FOCUS_MOUSE_RIGHT,
    FER_BUTTON_OFF_MOUSE_LEFT,
    FER_BUTTON_OFF_MOUSE_MID,
    FER_BUTTON_OFF_MOUSE_RIGHT,
    FER_BUTTON_OFF_DISABLED_LEFT,
    FER_BUTTON_OFF_DISABLED_MID,
    FER_BUTTON_OFF_DISABLED_RIGHT,
    FER_BUTTON_ON_FOCUS_MOUSE_LEFT,
    FER_BUTTON_ON_FOCUS_MOUSE_MID,
    FER_BUTTON_ON_FOCUS_MOUSE_RIGHT,

    FER_TEXT_OFF_LEFT,
    FER_TEXT_OFF_MID,
    FER_TEXT_OFF_RIGHT,
    FER_TEXT_OFF_FOCUS_LEFT,
    FER_TEXT_OFF_FOCUS_MID,
    FER_TEXT_OFF_FOCUS_RIGHT,
    FER_TEXT_OFF_FOCUS_MOUSE_LEFT,
    FER_TEXT_OFF_FOCUS_MOUSE_MID,
    FER_TEXT_OFF_FOCUS_MOUSE_RIGHT,
    FER_TEXT_OFF_MOUSE_LEFT,
    FER_TEXT_OFF_MOUSE_MID,
    FER_TEXT_OFF_MOUSE_RIGHT,
    FER_TEXT_OFF_DISABLED_LEFT,
    FER_TEXT_OFF_DISABLED_MID,
    FER_TEXT_OFF_DISABLED_RIGHT,
    FER_TEXT_ON_FOCUS_MOUSE_LEFT,
    FER_TEXT_ON_FOCUS_MOUSE_MID,
    FER_TEXT_ON_FOCUS_MOUSE_RIGHT,

    FER_TOGGLE_ON_LEFT,
    FER_TOGGLE_ON_MID,
    FER_TOGGLE_ON_RIGHT,
    FER_TOGGLE_ON_FOCUS_LEFT,
    FER_TOGGLE_ON_FOCUS_MID,
    FER_TOGGLE_ON_FOCUS_RIGHT,
    FER_TOGGLE_ON_MOUSE_LEFT,
    FER_TOGGLE_ON_MOUSE_MID,
    FER_TOGGLE_ON_MOUSE_RIGHT,
    FER_TOGGLE_ON_FOCUS_MOUSE_LEFT,
    FER_TOGGLE_ON_FOCUS_MOUSE_MID,
    FER_TOGGLE_ON_FOCUS_MOUSE_RIGHT,
    FER_TOGGLE_ON_DISABLED_LEFT,
    FER_TOGGLE_ON_DISABLED_MID,
    FER_TOGGLE_ON_DISABLED_RIGHT,
    FER_TOGGLE_OFF_LEFT,
    FER_TOGGLE_OFF_MID,
    FER_TOGGLE_OFF_RIGHT,
    FER_TOGGLE_OFF_FOCUS_LEFT,
    FER_TOGGLE_OFF_FOCUS_MID,
    FER_TOGGLE_OFF_FOCUS_RIGHT,
    FER_TOGGLE_OFF_MOUSE_LEFT,
    FER_TOGGLE_OFF_MOUSE_MID,
    FER_TOGGLE_OFF_MOUSE_RIGHT,
    FER_TOGGLE_OFF_FOCUS_MOUSE_LEFT,
    FER_TOGGLE_OFF_FOCUS_MOUSE_MID,
    FER_TOGGLE_OFF_FOCUS_MOUSE_RIGHT,
    FER_TOGGLE_OFF_DISABLED_LEFT,
    FER_TOGGLE_OFF_DISABLED_MID,
    FER_TOGGLE_OFF_DISABLED_RIGHT,

    FER_CHECK_ON,
    FER_CHECK_ON_FOCUS,
    FER_CHECK_ON_MOUSE,
    FER_CHECK_ON_FOCUS_MOUSE,
    FER_CHECK_ON_DISABLED,
    FER_CHECK_OFF,
    FER_CHECK_OFF_FOCUS,
    FER_CHECK_OFF_MOUSE,
    FER_CHECK_OFF_FOCUS_MOUSE,
    FER_CHECK_OFF_DISABLED,

    FER_RADIO_ON,
    FER_RADIO_ON_FOCUS,
    FER_RADIO_ON_MOUSE,
    FER_RADIO_ON_FOCUS_MOUSE,
    FER_RADIO_ON_DISABLED,
    FER_RADIO_OFF,
    FER_RADIO_OFF_FOCUS,
    FER_RADIO_OFF_MOUSE,
    FER_RADIO_OFF_FOCUS_MOUSE,
    FER_RADIO_OFF_DISABLED,

    FER_VERTSB_TOP,
    FER_VERTSB_MID,
    FER_VERTSB_BOTTOM,
    FER_VERTST_OFF_TOP,
    FER_VERTST_OFF_MID,
    FER_VERTST_OFF_BOTTOM,
    FER_VERTST_OFF_MOUSE_TOP,
    FER_VERTST_OFF_MOUSE_MID,
    FER_VERTST_OFF_MOUSE_BOTTOM,
    FER_VERTST_ON_TOP,
    FER_VERTST_ON_MID,
    FER_VERTST_ON_BOTTOM,
    FER_VERTSA_OFF_UP,
    FER_VERTSA_OFF_DOWN,
    FER_VERTSA_OFF_MOUSE_UP,
    FER_VERTSA_OFF_MOUSE_DOWN,
    FER_VERTSA_ON_UP,
    FER_VERTSA_ON_DOWN,

    FER_LIST_NW_CORNER,
    FER_LIST_NE_CORNER,
    FER_LIST_SE_CORNER,
    FER_LIST_SW_CORNER,
    FER_LIST_N_STRAIGHT,
    FER_LIST_E_STRAIGHT,
    FER_LIST_S_STRAIGHT,
    FER_LIST_W_STRAIGHT,
    FER_LIST_MID,
    FER_LIST_FOCUS_NW_CORNER,
    FER_LIST_FOCUS_NE_CORNER,
    FER_LIST_FOCUS_SE_CORNER,
    FER_LIST_FOCUS_SW_CORNER,
    FER_LIST_FOCUS_N_STRAIGHT,
    FER_LIST_FOCUS_E_STRAIGHT,
    FER_LIST_FOCUS_S_STRAIGHT,
    FER_LIST_FOCUS_W_STRAIGHT,
    FER_LIST_FOCUS_MID,

    FER_MENU_MOUSEOVER_CENTRE,
    FER_MENU_MOUSEOVER_LEFT_CAP,
    FER_MENU_MOUSEOVER_RIGHT_CAP,
    FER_MENU_SELECTED_DOT,
    FER_MENU_POPOUT_ARROW,

    FER_ARROW_LEFT_OFF,
    FER_ARROW_LEFT_MOUSE,
    FER_ARROW_LEFT_ON,
    FER_ARROW_RIGHT_OFF,
    FER_ARROW_RIGHT_MOUSE,
    FER_ARROW_RIGHT_ON,

    FER_BUILD_ARROW_LEFT_OFF,
    FER_BUILD_ARROW_LEFT_MOUSE,
    FER_BUILD_ARROW_LEFT_ON,
    FER_BUILD_ARROW_RIGHT_OFF,
    FER_BUILD_ARROW_RIGHT_MOUSE,
    FER_BUILD_ARROW_RIGHT_ON,

    FER_SHIPVIEW_LIGHTS_OFF,
    FER_SHIPVIEW_LIGHTS_TOP,
    FER_SHIPVIEW_LIGHTS_BOTTOM,
    FER_SHIPVIEW_LIGHTS_BOTH,

    FER_VOLUME_BUTTON,
    FER_VOLUME_DISABLED_LEFTCAP,
    FER_VOLUME_DISABLED_MID,
    FER_VOLUME_DISABLED_RIGHTCAP,
    FER_VOLUME_OFF_LEFTCAP,
    FER_VOLUME_OFF_MID,
    FER_VOLUME_OFF_RIGHTCAP,
    FER_VOLUME_MOUSE_LEFTCAP,
    FER_VOLUME_MOUSE_MID,
    FER_VOLUME_MOUSE_RIGHTCAP,
    FER_VOLUME_ON_LEFTCAP,
    FER_VOLUME_ON_MID,
    FER_VOLUME_ON_RIGHTCAP,

    FER_EQ_BAR_BUTTON,
    FER_EQ_BAR_MOUSE_STRAIGHT,
    FER_EQ_BAR_ON_STRAIGHT,
    FER_EQ_BAR_OFF_STRAIGHT,
    FER_EQ_BAR_MOUSE_MIDDLE,
    FER_EQ_BAR_ON_MIDDLE,
    FER_EQ_BAR_OFF_MIDDLE,
    FER_EQ_BAR_MOUSE_BOTCAP,
    FER_EQ_BAR_ON_BOTCAP,
    FER_EQ_BAR_OFF_BOTCAP,
    FER_EQ_BAR_MOUSE_TOPCAP,
    FER_EQ_BAR_ON_TOPCAP,
    FER_EQ_BAR_OFF_TOPCAP,

};

#define FER_MaxFileName 64

//font registry entry
typedef struct
{
    Node              node;
    tex_holder        name;
    char              stringname[FER_MaxFileName];
    lifheader         *lif;
    textype           type;
    udword            glhandle;
    sdword            nUsageCount;
}
textureregistry;


//front end registry
LinkedList ferTextureRegistry[FER_NumTextures];
LinkedList *globalCutouts;

textureregistry* g_Entry = NULL;

sdword ferSelectedDotMarginX = 3;
sdword ferSelectedDotMarginY = 1;
sdword ferPopoutArrowMarginX = 13;
sdword ferPopoutArrowMarginY = 1;

#define NUM_CUTS 8

/*=============================================================================
    Functions:
=============================================================================*/
//headers

//register a font
lifheader *ferTextureRegister(tex_holder holder, textype newtype, textype origtype);
void ferDrawBox(rectangle dimensions, tex_holder outerCornerName, tex_holder innerCornerName,
                tex_holder sideName, udword kludgyflag);



// ============================================================================
// Utility functions
// ============================================================================
/*-----------------------------------------------------------------------------
    Name        : ferFlipBitmap90
    Description : Flips a 16 pixel by 16 pixel bitmap 90 degrees
    Inputs      : bitmap - pointer to the bitmap data
                  width - width of the bitmap (assumed to be square, so
                          height value is not needed
    Outputs     : replaces "bitmap" with a bitmap flipped 90 degrees clockwise
    Return      :
----------------------------------------------------------------------------*/
#define PIXEL(bm, x, y, offset, width) bm[((x) + (y)*(width))+(offset)]

void ferFlipBitmap90(udword *bitmap, sdword width)
{
    udword i, j;
    udword new_bitmap[1024];

    for (i = 0; i < (udword)width; i++)
    {
        for (j = 0; j < (udword)width; j++)
        {
            PIXEL(new_bitmap, i, (width-1)-j, 0, width) = PIXEL(bitmap, j, i, 0, width);
        }
    }

    memcpy(bitmap,new_bitmap,sizeof(udword)*width*width);
}
#undef PIXEL

/*-----------------------------------------------------------------------------
    Name        : ferMirrorBitmapVert
    Description : Flips a 16 pixel by 16 pixel bitmap 90 degrees
    Inputs      : bitmap - pointer to the bitmap data
                  width - width of the bitmap (assumed to be square, so
                          height value is not needed
    Outputs     : replaces "bitmap" with a bitmap flipped 90 degrees clockwise
    Return      :
----------------------------------------------------------------------------*/

void ferMirrorBitmapVert(udword *bitmap, sdword width, sdword height)
{
    udword i, j;
    udword line[640];  // one scanline

    for (j=(height-1)*width,i=0; i < ((height/2)*width); i+=width, j-=width)
    {
        memcpy(line,bitmap+i,width*sizeof(udword));
        memcpy(bitmap+i, bitmap+j, width*sizeof(udword));
        memcpy(bitmap+j, line, width*sizeof(udword));
    }
}


/*-----------------------------------------------------------------------------
    Name        : ferFindCutoutType
    Description : Checks the corner/side array to find a specific corner/side
    Inputs      : cuts - array of corners/sides
                  find - the desired corner/side to be found
    Outputs     :
    Return      : TRUE if the corner/side is found
----------------------------------------------------------------------------*/
bool32 ferFindCutoutType(textype cuts[NUM_CUTS], textype find)
{
    udword i;

    if (cuts == NULL)
    {
        return FALSE;
    }

    for (i = 0; i < NUM_CUTS; i++)
    {
        if (cuts[i] == find)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*-----------------------------------------------------------------------------
    Name        : ferFindCutoutDimensions
    Description : Finds the cutout specified by type and returns it's rectangular
                  dimensions
    Inputs      : cutout - the type of cutout to find
    Outputs     :
    Return      : rectangle defining the dimensions of the cutout
----------------------------------------------------------------------------*/
rectangle ferGetCutoutDimensions(textype cutout)
{
    Node *node = globalCutouts->head;
    cutouttype *element;
    rectangle bad_rect = {-1, -1, -1, -1};

    while (node != NULL)
    {
        element = listGetStructOfNode(node);
        if (element->type == cutout)
        {
            return element->rect;
        }
        node = node->next;
    }
    return bad_rect;
}


// ============================================================================
// Front End Texture Registry code
// ============================================================================

/*-----------------------------------------------------------------------------
    Name        : ferStartup
    Description : Start front end registry module
    Inputs      :
    Outputs     : init the front end registry
    Return      :
----------------------------------------------------------------------------*/
void ferStartup(void)
{
    uword i;

    for (i = 0; i < FER_NumTextures; i++)
    {
        listInit(&ferTextureRegistry[i]);
    }
}

/*-----------------------------------------------------------------------------
    Name        : ferReset
    Description : reset front end registry module
    Inputs      :
    Outputs     : reset the front end registry
    Return      :
----------------------------------------------------------------------------*/
void ferReset(void)
{
    sdword i;
    Node* node;
    textureregistry* element;

    for (i = 0; i < FER_NumTextures; i++)
    {
        node = ferTextureRegistry[i].head;

        while (node != NULL)
        {
            element = listGetStructOfNode(node);
            if (element->glhandle != 0)
            {
                glDeleteTextures(1, &element->glhandle);
                element->glhandle = 0;
            }
            node = node->next;
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : ferShutdown
    Description : Shuts down the front end registry module
    Inputs      :
    Outputs     : Frees all bitmaps and linked list entries in the registry
    Return      :
----------------------------------------------------------------------------*/
void ferShutdown(void)
{
    udword i;
    Node *node;
    textureregistry *element;

    for (i = 0; i < FER_NumTextures; i++)
    {
        node = ferTextureRegistry[i].head;

        while (node != NULL)
        {
            element = listGetStructOfNode(node);
            if (element->glhandle != 0)
            {
                glDeleteTextures(1, &element->glhandle);
                element->glhandle = 0;
            }
            memFree(element->lif);
            node = node->next;
        }
        listDeleteAll(&ferTextureRegistry[i]);
    }
}


/*-----------------------------------------------------------------------------
    Name        : ferTextureRegisterType
    Description : Register usage of a texture with a different type than that
                  in the original entry, generating it if needed
                  Note: this program currently assumes that the corner bitmap
                        starts as a topleft bitmap and the side bitmap starts
                        as a left bitmap.
    Inputs      : arrayloc - the location of the texture in the registry array
                  newtype - the orientation of the new texture to be registered
                  origtype - the orientation of the original texture
    Outputs     :
    Return      : the texture structure corresponding to the requested type
----------------------------------------------------------------------------*/
#define NEW_ELEMENT_DATA_SIZE (element->lif->height*element->lif->width*sizeof(color))
#define NEW_ELEMENT_LIF_SIZE  sizeof(lifheader)+NEW_ELEMENT_DATA_SIZE
#define NEW_ELEMENT_SIZE      sizeof(textureregistry)

lifheader *ferTextureRegisterType(textureregistry *element, textype newtype, textype origtype)
{
    textureregistry *new_element;
    Node *node;


    //look for a list element that already has the same type
    // as the one requested and return it
    for (node = &element->node; node != NULL; node = node->next)
    {
        new_element = listGetStructOfNode(node);
        if (new_element->type == newtype)
        {
            ++new_element->nUsageCount;
            g_Entry = new_element;
            return new_element->lif;
        }
    }


    //create a new element and add it to the list
    //  allocate memory space for the element and the lif structure
    new_element = memAlloc(NEW_ELEMENT_SIZE, "fertype", NonVolatile);
    new_element->lif = memAlloc(NEW_ELEMENT_LIF_SIZE, "ferlif", NonVolatile);

    //copy the old lif into the new location and point the data pointer
    //to the location where the new texture is stored
    memcpy(new_element->lif, element->lif, NEW_ELEMENT_LIF_SIZE);
    new_element->lif->data = (ubyte *)new_element->lif+sizeof(lifheader);
    new_element->type = newtype;
    new_element->glhandle = 0;
    new_element->name = element->name;


    //corner texture
    if (element->type == topleft)
    {
        switch (newtype)
        {
            case bottomright:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
            case bottomleft:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
            case topright:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
                break;
            default:
                break;
        }
    }
    //side texture
    else if (element->type == left)
    {
        switch (newtype)
        {
            case bottom:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
            case right:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
            case top:
                ferFlipBitmap90((udword *)new_element->lif->data, new_element->lif->width);
                break;
            default:
                break;
        }
    }

    listAddNodeAfter(&element->node, &new_element->node, new_element);

    g_Entry = new_element;
    return new_element->lif;
}
#undef NEW_ELEMENT_DATA_SIZE
#undef NEW_ELEMENT_LIF_SIZE
#undef NEW_ELEMENT_SIZE



/*-----------------------------------------------------------------------------
    Name        : ferTextureRegister
    Description : Register usage of a texture, loading it if needed
    Inputs      : holder - the placeholder of the name of the texture file to be loaded
                  newtype - the orientation of the new texture to be registered
                  origtype - the orientation of the original texture
    Outputs     :
    Return      : the texture structure corresponding to the filename and type
----------------------------------------------------------------------------*/
lifheader *ferTextureRegister(tex_holder holder, textype newtype, textype origtype)
{
//    udword i;
    Node *node;
    textureregistry *element;

    g_Entry = NULL;

    // find the entry in the array the matches the filename
    // or the end of the list
/*    for (i = 0; (i < FER_NumTextures) &&
                 (ferTextureRegistry[i].num != 0); i++)
    {
        node    = ferTextureRegistry[i].head;
        element = listGetStructOfNode(node);

        if (element->name == holder)
        {
            // found the entry
            break;
        }
    }

    if (i == (FER_NumTextures - 1))
    {
        //end of array... can't read in anymore
        return NULL;
    }*/

    // if the requested texture hasn't been registered before
    // add it to the registry

    dbgAssertOrIgnore(holder < FER_NumTextures);

    if (ferTextureRegistry[holder].num == 0)
    {
        element = memAlloc(sizeof(textureregistry), "fer", NonVolatile);

        element->lif = trLIFFileLoad(tex_names[holder], NonVolatile);

        ferMirrorBitmapVert((udword *)element->lif->data, element->lif->width, element->lif->height);

        if (element->lif == NULL)
        {
            //error loading file
            return NULL;
        }
        element->name        = holder;
        element->nUsageCount = 0;
        element->glhandle    = 0;
        element->type        = origtype;
        listAddNode(&ferTextureRegistry[holder], &element->node, element);
    }
    else
    {
        node    = ferTextureRegistry[holder].head;
        element = listGetStructOfNode(node);

        dbgAssertOrIgnore(element->name == holder);
    }

    // check for type match
    if (element->type != newtype)
    {
        return ferTextureRegisterType(element, newtype, origtype);
    }
    else
    {
        ++element->nUsageCount;
    }

    g_Entry = element;
    return element->lif;
}


/*-----------------------------------------------------------------------------
    Name        : ferTextureRegisterSpecial
    Description : Register usage of a texture, loading it if needed.  Generates
                  a name placeholder using the filename of the texture
    Inputs      : fileName - the name of the texture file to be loaded
                  newtype - the orientation of the new texture to be registered
                  origtype - the orientation of the original texture
    Outputs     :
    Return      : the texture structure corresponding to the filename and type
----------------------------------------------------------------------------*/
lifheader *ferTextureRegisterSpecial(char *fileName, textype newtype, textype origtype)
{
    udword i;
    Node *node;
    textureregistry *element;
    bool32    found=FALSE;
    tex_holder holder;
    udword new_th_accum=0, end = (udword)end_texholder;

    // generate a placeholder
    // only use up to 10 characters from the name of the texture for the hash table

    for (i = 0; (fileName[i] != '\0')&&(i<20); i++)
    {
        new_th_accum += fileName[i];
    }

    // user a pseudo hash table from the name of the texture.
    holder = (tex_holder)((new_th_accum % FER_NumDecorative) + end);

    dbgAssertOrIgnore(holder < FER_NumTextures);

    if (ferTextureRegistry[holder].num>0)
    {
        // it isn't so we have to search throught the linked list to find the texture we need.
        // or else if it isn't found then we have to load it.
        node = ferTextureRegistry[holder].head;

        while (node != NULL)
        {
            element = listGetStructOfNode(node);

            if (strncmp(element->stringname, fileName, FER_MaxFileName)==0)
            {
                found = TRUE;
                break;
            }
            node = node->next;
        }
    }

    // find the entry in the array the matches the filename
    // or the end of the list
/*    for (i = end; (i < FER_NumTextures) &&
                 (ferTextureRegistry[i].num != 0); i++)
    {
        node    = ferTextureRegistry[i].head;
        element = listGetStructOfNode(node);

        if (element->name == holder)
        {
            // found the entry
            break;
        }
    }*/

/*    if (i == (FER_NumTextures - 1))
    {
        //end of array... can't read in anymore
        dbgAssertOrIgnore(FALSE);
        // front end texture registry is full
    }*/

    // if the requested texture hasn't been registered before
    // add it to the registry
    if (!found)
    {
        element = memAlloc(sizeof(textureregistry), "ferspecial", NonVolatile);

        element->lif = trLIFFileLoad(fileName, NonVolatile);

        if (newtype==decorative)
        {
            ferMirrorBitmapVert((udword *)element->lif->data, element->lif->width, element->lif->height);
        }

        if (element->lif == NULL)
        {
            //error loading file
            return NULL;
        }
        element->name        = holder;
        memStrncpy(element->stringname, fileName, FER_MaxFileName);
        element->nUsageCount = 0;
        element->glhandle    = 0;
        element->type        = origtype;
        listAddNode(&ferTextureRegistry[holder], &element->node, element);
    }

    // check for type match
/*    if (element->type != newtype)
    {
        return ferTextureRegisterType(element, newtype, origtype);
    }
    else*/
//    {
        ++element->nUsageCount;
//    }

    dbgAssertOrIgnore(strncmp(element->stringname,fileName,FER_MaxFileName)==0);

    g_Entry = element;

    return element->lif;
}


/*-----------------------------------------------------------------------------
    Name        : ferTextureDeregister
    Description : Remove texture entirely from the register
    Inputs      : textureName - name of the texture to remove from the register
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferTextureDeregister(tex_holder holder)
{
    uword i, j;
    Node *node;
    textureregistry *element;

    for (i = 0; (i < FER_NumTextures) && (ferTextureRegistry[i].num != 0); i++)
    {
        node    = ferTextureRegistry[i].head;
        element = listGetStructOfNode(node);

        if (element->name == holder)
        {
            // found the entry
            break;
        }
    }

    if ((i < FER_NumTextures) && (ferTextureRegistry[i].num != 0))
    {
        // find the last entry of the list, and insert it in the newly emptied slot
        for (j = 0; ((j < FER_NumTextures) && (ferTextureRegistry[j].num != 0)); j++);
        j--;

        listDeleteAll(&ferTextureRegistry[i]);

        node    = ferTextureRegistry[j].head;
        element = listGetStructOfNode(node);
        listAddNode(&ferTextureRegistry[i], &element->node, element);
        listDeleteAll(&ferTextureRegistry[j]);
    }
}


// ============================================================================
// Front end Texture code
// ============================================================================

static sdword nextLog2(sdword n)
{
    sdword r = 1;
    while (r < n) r <<= 1;
    return r;
}

/*-----------------------------------------------------------------------------
    Name        : ferDraw
    Description : Draws a texturemap.  Checks the texturemap for clipping,
                  then draws it
    Inputs      : x, y - starting coordinates of the Texture
                  texture - texture to be drawn
    Outputs     : Draws a 2D texture on the screen
    Return      :
----------------------------------------------------------------------------*/
void ferDraw(sdword x, sdword y, lifheader *texture)
{
    real32 widthFrac, heightFrac;
    GLint newwidth, newheight;
    sdword oldTex;
    udword oldMode;

    if (g_Entry == NULL)
    {
        dbgFatal(DBG_Loc, "g_Entry is NULL in ferDraw");
        return;
    }

    newwidth  = nextLog2(texture->width);
    newheight = nextLog2(texture->height);

    if (g_Entry->glhandle == 0)
    {
        GLuint    thandle;
        sdword    ix, iy;
        GLubyte*  cp;
        GLushort* data;
        GLushort* rp;

        data = (GLushort*)memAlloc(4 * newwidth * newheight, "fer data", 0);

        for (iy = 0; iy < texture->height; iy++)
        {
            GLushort* dp = &data[2 * newwidth * ((texture->height - 1) - iy)];
            cp = texture->data + 4 * texture->width * iy;

            memcpy(dp, cp, 4 * texture->width);
        }

        glGenTextures(1, &thandle);
        trClearCurrent();
        glBindTexture(GL_TEXTURE_2D, thandle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newwidth, newheight,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        memFree(data);

        g_Entry->glhandle = thandle;
    }
    else
    {
        trClearCurrent();
        glBindTexture(GL_TEXTURE_2D, g_Entry->glhandle);
    }

    widthFrac  = (real32)texture->width / (real32)newwidth;
    heightFrac = (real32)texture->height / (real32)newheight;

    oldTex = rndTextureEnable(TRUE);
    oldMode = rndTextureEnvironment(RTE_Replace);

    GLfloat t[8] = { 0.0f, heightFrac, widthFrac, heightFrac, 0.0f, 0.0f, widthFrac, 0.0f };
    GLfloat v[8] = { primScreenToGLX(x), primScreenToGLY(y),
                        primScreenToGLX(x + texture->width), primScreenToGLY(y),
                        primScreenToGLX(x), primScreenToGLY(y - texture->height),
                        primScreenToGLX(x + texture->width), primScreenToGLY(y - texture->height) };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, v);
    glTexCoordPointer(2, GL_FLOAT, 0, t);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    rndTextureEnvironment(oldMode);
    rndTextureEnable(oldTex);
}


/*-----------------------------------------------------------------------------
    Name        : ferDrawCorner
    Description : Draws a corner of a box
    Inputs      : x, y - coordinates of the center of the texture
                  texture - pointer to the lif structure containing the texture
    Outputs     : draws a corner bitmap on the screen
    Return      :
----------------------------------------------------------------------------*/
void ferDrawCorner(sdword x, sdword y, lifheader *texture, curvetype curve, textype corner)
{
    if (curve == convex)
    {
        switch (corner)
        {
            case bottomleft:
                ferDraw(x - FER_CCBM, y + FER_CCBM, texture);
                break;
            case topleft:
                ferDraw(x - FER_CCBM, y + texture->height - FER_CCBM, texture);
                break;
            case topright:
                ferDraw(x - texture->height + FER_CCBM, y + texture->height - FER_CCBM, texture);
                break;
            case bottomright:
                ferDraw(x - texture->height + FER_CCBM, y + FER_CCBM, texture);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (corner)
        {
            case bottomleft:
                ferDraw(x, y, texture);
                break;
            case topleft:
                ferDraw(x, y + texture->height, texture);
                break;
            case topright:
                ferDraw(x - texture->width, y + texture->height, texture);
                break;
            case bottomright:
                ferDraw(x - texture->width, y, texture);
                break;
            default:
                break;
        }
    }
}


// ============================================================================
// Front End Box Drawing Code
// ============================================================================
/*-----------------------------------------------------------------------------
    Name        : ferDrawBoxLine
    Description : Draws the side of a box - note either x0, x1 or y0, y1
                  have to be the same (draws a vertical or horizonatal line)
    Inputs      : x0, y0 - start of the line
                  x1, y1 - end of the line
                  corner_width - size of the corner texturemap the line is
                                 starting and ending from
                  texture - pointer to the lif structure containing the texture
                  kludgyflag - decides whether the line is in the default position
                               with respect to the texturemap, or if it is one
                               texture width over - this is a tweaking value
    Outputs     : draws a textured line
    Return      :
----------------------------------------------------------------------------*/
void ferDrawBoxLine(sdword x0, sdword y0, sdword x1, sdword y1, sdword corner_offset,
                 sdword corner_width, lifheader *texture, udword kludgyflag)
{
    sdword sub = 0;
    sdword dist_x = x1 - x0, dist_y = y1 - y0,                    // distance to travel
           dist_tx = 0, dist_ty = 0;                              // distance traveled

    // set the starting values with respect to the corner texturemap
    if (y0 == y1)
    {
        x0     += (corner_width - corner_offset);
        x1     -= (corner_width - corner_offset);
        dist_x -= (texture->width + 2*(corner_width - corner_offset));

    }
    else
    {
        y0     += (corner_width + texture->height - corner_offset);
        y1     -= (corner_width + texture->height - corner_offset);
        dist_y -= (texture->height + 2*(corner_width - corner_offset));

    }

    // draw the line as a series of tiled textures
    while ((dist_tx <= dist_x) && (dist_ty <= dist_y))
    {
        if (kludgyflag)
        {
            if (dist_x)
                ferDraw(x0, y0 + texture->height, texture);
            else
                ferDraw(x0 - texture->width, y0, texture);
        }
        else
        {
            ferDraw(x0, y0, texture);
        }

        // increment the location values
        if (dist_x)
        {
            x0      += texture->width - sub;
            dist_tx += texture->width - sub;
        }
        else
        {
            y0      += texture->height - sub;
            dist_ty += texture->height - sub;
        }
    }

    // drawing stops just before the end bitmap is reached then another texture tile
    // is drawn using the end corner texturemap as a starting point.  This keeps the
    // tiled line from overshooting because the texturemaps don't divide into the
    // line evenly
    if (kludgyflag)
    {
        if (dist_y)
            ferDraw(x1 - texture->width, y1 + texture->height, texture);
        else
            ferDraw(x1 - texture->width, y1 + texture->height, texture);
    }
    else
    {
        if (dist_x)
        {
            ferDraw(x1 - texture->width, y1, texture);
        }
        else
        {
            ferDraw(x1, y1 + texture->height, texture);
        }
    }
}



/*-----------------------------------------------------------------------------
    Name        : ferDrawBoxCorners
    Description : Draws the corners of a textured box
    Inputs      : dimensions - dimensions of the box
                  cornerName - name of the texturemap to draw the corners with
    Outputs     : draws 4 corner bitmaps in the corners of the rectangles
    Return      : size of the corner bitmaps to aid in drawing the lines
----------------------------------------------------------------------------*/
udword ferDrawBoxCorners(rectangle dimensions, tex_holder cornerName, textype cuts[NUM_CUTS])
{
    lifheader *texture;

    texture = ferTextureRegister(cornerName, topleft, topleft);

    if (!ferFindCutoutType(cuts, bottomleft))
    {
        texture = ferTextureRegister(cornerName, bottomleft, topleft);
        ferDrawCorner(dimensions.x0, dimensions.y1, texture, convex, bottomleft);
    }

    if (!ferFindCutoutType(cuts, topleft))
    {
        texture = ferTextureRegister(cornerName, topleft, topleft);
        ferDrawCorner(dimensions.x0, dimensions.y0, texture, convex, topleft);
    }

    if (!ferFindCutoutType(cuts, topright))
    {
        texture = ferTextureRegister(cornerName, topright, topleft);
        ferDrawCorner(dimensions.x1, dimensions.y0, texture, convex, topright);
    }

    if (!ferFindCutoutType(cuts, bottomright))
    {
        texture = ferTextureRegister(cornerName, bottomright, topleft);
        ferDrawCorner(dimensions.x1, dimensions.y1, texture, convex, bottomright);
    }


    return texture->width;
}


/*-----------------------------------------------------------------------------
    Name        : ferDrawBoxSides
    Description : Draws the sides of a textured box
    Inputs      : dimensions - dimensions of the box
                  sideName - name of the texturemap to draw the side lines
                  corner_width - width of the corner texturemaps
                  kludgyflag - tweak flag - lines up the line properly
    Outputs     : draws 4 lines on the screen
    Return      :
----------------------------------------------------------------------------*/
#define LINE_ENDPOINTS endpoints.x0, endpoints.y0, endpoints.x1, endpoints.y1

void ferDrawBoxSides(rectangle dimensions, tex_holder sideName, udword corner_width,
                     textype cuts[NUM_CUTS], udword kludgyflag)
{
    lifheader *texture;
    rectangle endpoints, cutout;

//    corner_width /= 2;

    //draw the left side
    endpoints    = dimensions;
    endpoints.x1 = dimensions.x0;

    texture = ferTextureRegister(sideName, left, left);
    if (ferFindCutoutType(cuts, topleft))
    {
        cutout       = ferGetCutoutDimensions(topleft);
        endpoints.y0 = cutout.y1;
    }

    if (ferFindCutoutType(cuts, left))
    {
        cutout       = ferGetCutoutDimensions(left);
        endpoints.y1 = cutout.y0;
        ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, kludgyflag);
        endpoints.y0 = cutout.y1;
        endpoints.y1 = dimensions.y1;
    }

    if (ferFindCutoutType(cuts, bottomleft))
    {
        cutout       = ferGetCutoutDimensions(bottomleft);
        endpoints.y1 = cutout.y0;
    }
    ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, kludgyflag);

    //draw the top side
    endpoints    = dimensions;
    endpoints.y1 = dimensions.y0;

    texture = ferTextureRegister(sideName, top, left);
    if (ferFindCutoutType(cuts, topleft))
    {
        cutout       = ferGetCutoutDimensions(topleft);
        endpoints.x0 = cutout.x1;
    }

    if (ferFindCutoutType(cuts, top))
    {
        cutout       = ferGetCutoutDimensions(top);
        endpoints.x1 = cutout.x0;
        ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, !kludgyflag);
        endpoints.x0 = cutout.x1;
        endpoints.x1 = dimensions.x1;
    }

    if (ferFindCutoutType(cuts, topright))
    {
        cutout       = ferGetCutoutDimensions(topright);
        endpoints.x1 = cutout.x0;
    }
    ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, !kludgyflag);

    //draw the right side
    endpoints    = dimensions;
    endpoints.x0 = dimensions.x1;

    texture = ferTextureRegister(sideName, right, left);
    if (ferFindCutoutType(cuts, topright))
    {
        cutout       = ferGetCutoutDimensions(topright);
        endpoints.y0 = cutout.y1;
    }

    if (ferFindCutoutType(cuts, right))
    {
        cutout       = ferGetCutoutDimensions(right);
        endpoints.y1 = cutout.y0;
        ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, !kludgyflag);
        endpoints.y0 = cutout.y1;
        endpoints.y1 = dimensions.y1;
    }

    if (ferFindCutoutType(cuts, bottomright))
    {
        cutout       = ferGetCutoutDimensions(bottomright);
        endpoints.y1 = cutout.y0;
    }
    ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, !kludgyflag);

    //draw the bottom side
    endpoints    = dimensions;
    endpoints.y0 = dimensions.y1;

    texture = ferTextureRegister(sideName, bottom, left);
    if (ferFindCutoutType(cuts, bottomleft))
    {
        cutout       = ferGetCutoutDimensions(bottomleft);
        endpoints.x0 = cutout.x1;
    }

    if (ferFindCutoutType(cuts, bottom))
    {
        cutout       = ferGetCutoutDimensions(bottom);
        endpoints.x1 = cutout.x0;
        ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, kludgyflag);
        endpoints.x0 = cutout.x1;
        endpoints.x1 = dimensions.x1;
    }

    if (ferFindCutoutType(cuts, bottomright))
    {
        cutout       = ferGetCutoutDimensions(bottomright);
        endpoints.x1 = cutout.x0;
    }
    ferDrawBoxLine(LINE_ENDPOINTS, FER_CCBM, corner_width, texture, kludgyflag);
}

#undef LINE_ENDPOINTS

/*-----------------------------------------------------------------------------
    Name        : ferDrawCutout
    Description : Draws the extra corners and sides for a cutout region
    Inputs      : dimensions - dimensions of box
                  cutDimensions - dimensions of the cutout region
                  outerCornerName - name of the outside corner texturemap
                  innerCornerName - name of the inside corner texturemap
                  sideName - name of the side texturemap
                  kludgyflag - tweak flag - lines up the line properly
    Outputs     : draws the corners and lines that make up a cutout
    Return      : the texture type that was cut out (left, top, topleft, bottomright, etc.)
----------------------------------------------------------------------------*/
textype ferDrawCutout(rectangle dimensions, rectangle cutDimensions,
                      tex_holder outerCornerName, tex_holder innerCornerName,
                      tex_holder sideName, udword kludgyflag)
{
#if DRAW_CUTOUTS
    lifheader *texture;
    udword corner_width;
#endif

    //check if the top left corner is in the cutout box
    if (IS_IN_RECT(dimensions.x0, dimensions.y0, cutDimensions.x0,
                   cutDimensions.y0, cutDimensions.x1, cutDimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, topleft, topleft);
        ferDrawCorner(dimensions.x0, cutDimensions.y1, texture, convex, topleft);
        ferDrawCorner(cutDimensions.x1, dimensions.y0, texture, convex, topleft);

        texture = ferTextureRegister(innerCornerName, bottomright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y1, texture, concave, bottomright);
        corner_width = texture->width;

        //draw the sides
        texture = ferTextureRegister(sideName, left, left);
        ferDrawBoxLine(cutDimensions.x1, dimensions.y0, cutDimensions.x1, cutDimensions.y1-corner_width,
                    0, corner_width, texture, kludgyflag);

        texture = ferTextureRegister(sideName, top, left);
        ferDrawBoxLine(dimensions.x0, cutDimensions.y1, cutDimensions.x1-corner_width, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
#endif

        return topleft;
    }

    //check if the top right corner is in the cutout box
    if (IS_IN_RECT(dimensions.x1, dimensions.y0, cutDimensions.x0,
                   cutDimensions.y0, cutDimensions.x1, cutDimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, topright, topleft);
        ferDrawCorner(cutDimensions.x0, dimensions.y0, texture, convex, topright);
        ferDrawCorner(dimensions.x1, cutDimensions.y1, texture, convex, topright);

        texture = ferTextureRegister(innerCornerName, bottomleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y1, texture, concave, bottomleft);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, right, left);
        ferDrawBoxLine(cutDimensions.x0, dimensions.y0, cutDimensions.x0, cutDimensions.y1-corner_width,
                    0, corner_width, texture, !kludgyflag);

        texture = ferTextureRegister(sideName, top, left);
        ferDrawBoxLine(cutDimensions.x0+corner_width, cutDimensions.y1, dimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
#endif

        return topright;
    }

    //check if the bottom right corner is in the cutout box
    if (IS_IN_RECT(dimensions.x1, dimensions.y1, cutDimensions.x0,
                   cutDimensions.y0, cutDimensions.x1, cutDimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, bottomright, topleft);
        ferDrawCorner(cutDimensions.x0, dimensions.y1, texture, convex, bottomright);
        ferDrawCorner(dimensions.x1, cutDimensions.y0, texture, convex, bottomright);

        texture = ferTextureRegister(innerCornerName, topleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y0, texture, concave, topleft);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, right, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y0+corner_width, cutDimensions.x0, dimensions.y1,
                    0, corner_width, texture, !kludgyflag);

        texture = ferTextureRegister(sideName, bottom, left);
        ferDrawBoxLine(cutDimensions.x0+corner_width, cutDimensions.y0, dimensions.x1, cutDimensions.y0,
                    0, corner_width, texture, kludgyflag);
#endif

        return bottomright;
    }

    //check if the bottom left corner is in the cutout box
    if (IS_IN_RECT(dimensions.x0, dimensions.y1, cutDimensions.x0,
                   cutDimensions.y0, cutDimensions.x1, cutDimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, bottomleft, topleft);
        ferDrawCorner(dimensions.x0, cutDimensions.y0, texture, convex, bottomleft);
        ferDrawCorner(cutDimensions.x1, dimensions.y1, texture, convex, bottomleft);

        texture = ferTextureRegister(innerCornerName, topright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y0, texture, concave, topright);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, left, left);
        ferDrawBoxLine(cutDimensions.x1, cutDimensions.y0+corner_width, cutDimensions.x1, dimensions.y1,
                    0, corner_width, texture, kludgyflag);

        texture = ferTextureRegister(sideName, bottom, left);
        ferDrawBoxLine(dimensions.x0, cutDimensions.y0, cutDimensions.x1-corner_width, cutDimensions.y0,
                    0, corner_width, texture, kludgyflag);
#endif

        return bottomleft;
    }

    //check if the entire bottom of the cutout box is in the region
    if (IS_IN_RECT(cutDimensions.x0, cutDimensions.y1, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1) &&
        IS_IN_RECT(cutDimensions.x1, cutDimensions.y1, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, topright, topleft);
        ferDrawCorner(cutDimensions.x0, dimensions.y0, texture, convex, topright);
        texture = ferTextureRegister(outerCornerName, topleft, topleft);
        ferDrawCorner(cutDimensions.x1, dimensions.y0, texture, convex, topleft);
        texture = ferTextureRegister(innerCornerName, bottomleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y1, texture, concave, bottomleft);
        texture = ferTextureRegister(innerCornerName, bottomright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y1, texture, concave, bottomright);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, top, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y1, cutDimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
        texture = ferTextureRegister(sideName, right, left);
        ferDrawBoxLine(cutDimensions.x0, dimensions.y0, cutDimensions.x0, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
        texture = ferTextureRegister(sideName, left, left);
        ferDrawBoxLine(cutDimensions.x1, dimensions.y0, cutDimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, kludgyflag);
#endif

        return top;
    }

    //check if the entire top of the cutout box is in the region
    if (IS_IN_RECT(cutDimensions.x0, cutDimensions.y0, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1) &&
        IS_IN_RECT(cutDimensions.x1, cutDimensions.y0, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(innerCornerName, topright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y0, texture, concave, topright);
        texture = ferTextureRegister(innerCornerName, topleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y0, texture, concave, topleft);
        texture = ferTextureRegister(outerCornerName, bottomleft, topleft);
        ferDrawCorner(cutDimensions.x1, dimensions.y1, texture, convex, bottomleft);
        texture = ferTextureRegister(outerCornerName, bottomright, topleft);
        ferDrawCorner(cutDimensions.x0, dimensions.y1, texture, convex, bottomright);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, bottom, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y0, cutDimensions.x1, cutDimensions.y0,
                    0, corner_width, texture, kludgyflag);
        texture = ferTextureRegister(sideName, right, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y0, cutDimensions.x0, dimensions.y1,
                    0, corner_width, texture, !kludgyflag);
        texture = ferTextureRegister(sideName, left, left);
        ferDrawBoxLine(cutDimensions.x1, cutDimensions.y0, cutDimensions.x1, dimensions.y1,
                    0, corner_width, texture, kludgyflag);
#endif

        return bottom;
    }

    //check if the entire right of the cutout box is in the region
    if (IS_IN_RECT(cutDimensions.x0, cutDimensions.y0, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1) &&
        IS_IN_RECT(cutDimensions.x0, cutDimensions.y1, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(outerCornerName, topright, topleft);
        ferDrawCorner(dimensions.x1, cutDimensions.y1, texture, convex, topright);
        texture = ferTextureRegister(innerCornerName, topleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y0, texture, concave, topleft);
        texture = ferTextureRegister(innerCornerName, bottomleft, topleft);
        ferDrawCorner(cutDimensions.x0, cutDimensions.y1, texture, concave, bottomleft);
        texture = ferTextureRegister(outerCornerName, bottomright, topleft);
        ferDrawCorner(dimensions.x1, cutDimensions.y0, texture, convex, bottomright);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, right, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y0, cutDimensions.x0, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
        texture = ferTextureRegister(sideName, bottom, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y0, dimensions.x1, cutDimensions.y0,
                    0, corner_width, texture, kludgyflag);
        texture = ferTextureRegister(sideName, top, left);
        ferDrawBoxLine(cutDimensions.x0, cutDimensions.y1, dimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
#endif

        return right;
    }
    //check if the entire left of the cutout box is in the region
    if (IS_IN_RECT(cutDimensions.x1, cutDimensions.y0, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1) &&
        IS_IN_RECT(cutDimensions.x1, cutDimensions.y1, dimensions.x0,
                   dimensions.y0, dimensions.x1, dimensions.y1))
    {
#if DRAW_CUTOUTS
        //draw the corners
        texture = ferTextureRegister(innerCornerName, topright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y0, texture, concave, topright);
        texture = ferTextureRegister(outerCornerName, topleft, topleft);
        ferDrawCorner(dimensions.x0, cutDimensions.y1, texture, convex, topleft);
        texture = ferTextureRegister(outerCornerName, bottomleft, topleft);
        ferDrawCorner(dimensions.x0, cutDimensions.y0, texture, convex, bottomleft);
        texture = ferTextureRegister(innerCornerName, bottomright, topleft);
        ferDrawCorner(cutDimensions.x1, cutDimensions.y1, texture, concave, bottomright);
        corner_width = texture->width/2;

        //draw the sides
        texture = ferTextureRegister(sideName, left, left);
        ferDrawBoxLine(cutDimensions.x1, cutDimensions.y0, cutDimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, kludgyflag);
        texture = ferTextureRegister(sideName, bottom, left);
        ferDrawBoxLine(dimensions.x0, cutDimensions.y0, cutDimensions.x1, cutDimensions.y0,
                    0, corner_width, texture, kludgyflag);
        texture = ferTextureRegister(sideName, top, left);
        ferDrawBoxLine(dimensions.x0, cutDimensions.y1, cutDimensions.x1, cutDimensions.y1,
                    0, corner_width, texture, !kludgyflag);
#endif

        return left;
    }
    
    // NB: should never get here; this is just to keep the compiler happy
    return none; 
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawBox
    Description : Draws a textured box
    Inputs      : dimensions - dimensions of box
                  outerCornerName - name of the outside corner texturemap
                  innnerCornerName - name of the inside corner texturemap
                  sideName - name of the side texturemap
                  kludgyflag - tweak flag - lines up the line properly
    Outputs     : calls the functions to draw the corners and lines of the box
    Return      :
----------------------------------------------------------------------------*/
void ferDrawBox(rectangle dimensions, tex_holder outerCornerName, tex_holder innerCornerName,
                tex_holder sideName, udword kludgyflag)
{
    bool32 primModeOn = TRUE;
    udword width, i;
    textype cutout_corners[NUM_CUTS];
    Node *node;
    cutouttype *cut_element;

    if (globalCutouts == NULL)
    {
        node = NULL;
    }
    else
    {
        node = globalCutouts->head;
    }

    for (i = 0; i < NUM_CUTS; i++)
    {
        cutout_corners[i] = none;
    }
    i = 0;

    if (!primModeEnabled)
    {
        primModeOn = FALSE;
        primModeSetFunction2();
    }

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    while (node != NULL)
    {
        cut_element = listGetStructOfNode(node);

        //primRectSolid2(&cut_element->rect, FER_CUTOUT_COLOR);

        cutout_corners[i] = ferDrawCutout(dimensions, cut_element->rect, outerCornerName,
                                          innerCornerName, sideName, kludgyflag);
        cut_element->type = cutout_corners[i];
        i++;
        node = node->next;
    }

/* add in check for cutout corners in drawing normal stuff */

    width = ferDrawBoxCorners(dimensions, outerCornerName, cutout_corners);

    ferDrawBoxSides(dimensions, sideName, width, cutout_corners, kludgyflag);

    glDisable(GL_ALPHA_TEST);

    if (!primModeOn)
    {
        primModeClearFunction2();
    }
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawBoxRegion
    Description : Draws a textured box region for the front end
    Inputs      : dimensions - dimensions of BaseRegion
                  bUseAlpha - enable alpha when drawing
    Outputs     : calls ferDrawBox to draw the base region
    Return      :
----------------------------------------------------------------------------*/
void ferDrawBoxRegion(rectangle dimensions, drawtype textures,
                       drawtype glow, LinkedList *cutouts, bool32 bUseAlpha)
{
    tex_holder  outerCorner = BOX_OUTER_CORNER,
                innerCorner = BOX_INNER_CORNER,
                side = BOX_OUTGLOW_STRAIGHT;
    udword lineup = 1;

    globalCutouts = cutouts;

    // every now and then, for some reason, an unitialized
    // texture and glow type slips in, this is a fix for that
    // now also set default texture if texture is ferNormal
    if ((textures < ferNormal) || (textures >= ferLast))
    {
        textures = ferMediumTextures;
    }
    if ((glow < ferNormal) || (glow >= ferLast))
    {
        glow = ferExternalGlow;
#ifdef jthornton
        dbgMessage("#");
#endif

    }

    switch (textures)
    {
        case ferSmallTextures:
            switch (glow)
            {
                case ferNoGlow:
                    outerCorner = BOX_NOGLOW_SMALL_CORNER;
                    innerCorner = BOX_NOGLOW_SMALL_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
                break;
                case ferInternalGlow:
                    outerCorner = BOX_SMALL_INNER_CORNER;
                    innerCorner = BOX_SMALL_OUTER_CORNER;
                    side = BOX_SMALL_INGLOW_STRAIGHT;
                break;
                case ferExternalGlow:
                    outerCorner = BOX_SMALL_OUTER_CORNER;
                    innerCorner = BOX_SMALL_INNER_CORNER;
                    side = BOX_SMALL_OUTGLOW_STRAIGHT;
                break;
                default:
                    outerCorner = BOX_NOGLOW_SMALL_CORNER;
                    innerCorner = BOX_NOGLOW_SMALL_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
            }
            break;
        case ferMediumTextures:
            switch (glow)
            {
                case ferNoGlow:
                    outerCorner = BOX_NOGLOW_CORNER;
                    innerCorner = BOX_NOGLOW_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
                break;
                case ferInternalGlow:
                    outerCorner = BOX_INNER_CORNER;
                    innerCorner = BOX_OUTER_CORNER;
                    side = BOX_INGLOW_STRAIGHT;
                break;
                case ferExternalGlow:
                    outerCorner = BOX_OUTER_CORNER;
                    innerCorner = BOX_INNER_CORNER;
                    side = BOX_OUTGLOW_STRAIGHT;
                break;
                default:
                    outerCorner = BOX_NOGLOW_CORNER;
                    innerCorner = BOX_NOGLOW_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
            }
            break;
        case ferLargeTextures:
            switch (glow)
            {
                case ferNoGlow:
                    outerCorner = BOX_NOGLOW_LARGE_CORNER;
                    innerCorner = BOX_NOGLOW_LARGE_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
                break;
                case ferInternalGlow:
                    outerCorner = BOX_LARGE_INNER_CORNER;
                    innerCorner = BOX_LARGE_OUTER_CORNER;
                    side = BOX_LARGE_INGLOW_STRAIGHT;
                    lineup = 0;
                break;
                case ferExternalGlow:
                    outerCorner = BOX_LARGE_OUTER_CORNER;
                    innerCorner = BOX_LARGE_INNER_CORNER;
                    side = BOX_LARGE_OUTGLOW_STRAIGHT;
                break;
                default:
                    outerCorner = BOX_NOGLOW_LARGE_CORNER;
                    innerCorner = BOX_NOGLOW_LARGE_CORNER;
                    side = BOX_NOGLOW_STRAIGHT;
            }
            break;
        default:
            //use default textures
            break;
    }

    if (bUseAlpha)
    {
        glEnable(GL_BLEND);
    }
    ferDrawBox(dimensions, outerCorner, innerCorner, side, lineup);
    glDisable(GL_BLEND);
}


// ============================================================================
// Front End Button Drawing Code
// ============================================================================

/*-----------------------------------------------------------------------------
    Name        : ferDrawLine
    Description : Draws a line consisting of a series of tiled texturemaps
                  have to be the same (draws a vertical or horizonatal line)
    Inputs      : x0, y0 - start of the line
                  x1, y1 - end of the line
                  corner_width - size of the corner texturemap the line is
                                 starting and ending from
                  texture - pointer to the lif structure containing the texture
                  kludgyflag - decides whether the line is in the default position
                               with respect to the texturemap, or if it is one
                               texture width over - this is a tweaking value
    Outputs     : draws a textured line
    Return      :
----------------------------------------------------------------------------*/
void ferDrawLine(sdword x0, sdword y0, sdword x1, sdword y1,
                 sdword corner_width, lifheader *texture, udword kludgyflag)
{
    sdword sub = 0;
    sdword dist_x = x1 - x0, dist_y = y1 - y0,                    // distance to travel
           dist_tx = 0, dist_ty = 0;                              // distance traveled

    // set the starting values with respect to the corner texturemap
    if (y0 == y1)
    {
        x0     += (corner_width - sub);
        x1     -= (corner_width - sub);
        dist_x -= (texture->width + 2*corner_width);

    }
    else
    {
        y0     += (corner_width + texture->height - sub);
        y1     -= (corner_width + texture->height - sub);
        dist_y -= (texture->height + 2*corner_width);

    }

    // draw the line as a series of tiled textures
    while ((dist_tx <= dist_x) && (dist_ty <= dist_y))
    {
/*        if (kludgyflag)
        {
            if (dist_x)
                ferDraw(x0, y0 + texture->height, texture);
            else
                ferDraw(x0 - texture->width, y0, texture);
        }
        else*/
        {
            ferDraw(x0, y0, texture);
        }

        // increment the location values
        if (dist_x)
        {
            x0      += texture->width - sub;
            dist_tx += texture->width - sub;
        }
        else
        {
            y0      += texture->height - sub;
            dist_ty += texture->height - sub;
        }
    }

    // drawing stops just before the end bitmap is reached then another texture tile
    // is drawn using the end corner texturemap as a starting point.  This keeps the
    // tiled line from overshooting if the texturemaps don't divide into the
    // line evenly
/*    if (kludgyflag)
    {
        if (dist_y)
            ferDraw(x1 - texture->width, y1 + texture->height, texture);
        else
            ferDraw(x1 - texture->width, y1 + texture->height, texture);
    }
    else
    {*/
        if (dist_x)
        {
            ferDraw(x1 - texture->width, y1, texture);
        }
        else
        {
            ferDraw(x1, y1 + texture->height, texture);
        }
//    }
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawMenuItemSelected
    Description : Draw a menu item in it's selected state
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawMenuItemSelected(rectangle *rect)
{
    lifheader *texture;
    udword end_width;

    //draw the left endcap
    texture = ferTextureRegister(MENU_MOUSEOVER_LEFT_CAP, none, none);
    ferDraw(rect->x0, rect->y1, texture);

    //draw the right endcap
    texture = ferTextureRegister(MENU_MOUSEOVER_RIGHT_CAP, none, none);
    ferDraw(rect->x1 - texture->width, rect->y1, texture);
    end_width = texture->width;

    //draw the middle part
    texture = ferTextureRegister(MENU_MOUSEOVER_CENTRE, none, none);
    ferDrawLine(rect->x0, rect->y1, rect->x1, rect->y1, end_width, texture, 0);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawPopoutArrow
    Description : Draw a selected dot for a menu item
    Inputs      : rect - dimensions of a menu item
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawPopoutArrow(rectangle *rect)
{
    lifheader *texture;

    texture = ferTextureRegister(MENU_POPOUT_ARROW, none, none);
    ferDraw(rect->x1 - ferPopoutArrowMarginX, rect->y1 - ferPopoutArrowMarginY, texture);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawSelectedDot
    Description : Draw a selected dot for a menu item
    Inputs      : rect - dimensions of a menu item
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawSelectedDot(rectangle *rect)
{
    lifheader *texture;

    texture = ferTextureRegister(MENU_SELECTED_DOT, none, none);
    ferDraw(rect->x0 + ferSelectedDotMarginX, rect->y1 - ferSelectedDotMarginY, texture);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawButton
    Description : Draws a textured button for the front end
    Inputs      : dimensions - dimensions of button
                  state - the current button state
    Outputs     : draws a button on the screen
    Return      :
----------------------------------------------------------------------------*/
void ferDrawButton(rectangle dimensions, ferbuttonstate state)
{
    lifheader *texture;
    udword end_width;
    tex_holder left, right, mid;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    //choose textures
    switch (state)
    {
        case b_off:
            left    = BUTTON_OFF_LEFT;
            mid     = BUTTON_OFF_MID;
            right   = BUTTON_OFF_RIGHT;
            break;
        case b_off_focus:
            left    = BUTTON_OFF_FOCUS_LEFT;
            mid     = BUTTON_OFF_FOCUS_MID;
            right   = BUTTON_OFF_FOCUS_RIGHT;
            break;
        case b_off_focus_mouse:
            left    = BUTTON_OFF_FOCUS_MOUSE_LEFT;
            mid     = BUTTON_OFF_FOCUS_MOUSE_MID;
            right   = BUTTON_OFF_FOCUS_MOUSE_RIGHT;
            break;
        case b_off_mouse:
            left    = BUTTON_OFF_MOUSE_LEFT;
            mid     = BUTTON_OFF_MOUSE_MID;
            right   = BUTTON_OFF_MOUSE_RIGHT;
            break;
        case b_off_disabled:
            left    = BUTTON_OFF_DISABLED_LEFT;
            mid     = BUTTON_OFF_DISABLED_MID;
            right   = BUTTON_OFF_DISABLED_RIGHT;
            break;
        case b_on_focus_mouse:
            left    = BUTTON_ON_FOCUS_MOUSE_LEFT;
            mid     = BUTTON_ON_FOCUS_MOUSE_MID;
            right   = BUTTON_ON_FOCUS_MOUSE_RIGHT;
            break;

        case te_off:
            left    = TEXT_OFF_LEFT;
            mid     = TEXT_OFF_MID;
            right   = TEXT_OFF_RIGHT;
            break;
        case te_off_focus:
            left    = TEXT_OFF_FOCUS_LEFT;
            mid     = TEXT_OFF_FOCUS_MID;
            right   = TEXT_OFF_FOCUS_RIGHT;
            break;
        case te_off_focus_mouse:
            left    = TEXT_OFF_FOCUS_MOUSE_LEFT;
            mid     = TEXT_OFF_FOCUS_MOUSE_MID;
            right   = TEXT_OFF_FOCUS_MOUSE_RIGHT;
            break;
        case te_off_mouse:
            left    = TEXT_OFF_MOUSE_LEFT;
            mid     = TEXT_OFF_MOUSE_MID;
            right   = TEXT_OFF_MOUSE_RIGHT;
            break;
        case te_off_disabled:
            left    = TEXT_OFF_DISABLED_LEFT;
            mid     = TEXT_OFF_DISABLED_MID;
            right   = TEXT_OFF_DISABLED_RIGHT;
            break;
        case te_on_focus_mouse:
            left    = TEXT_ON_FOCUS_MOUSE_LEFT;
            mid     = TEXT_ON_FOCUS_MOUSE_MID;
            right   = TEXT_ON_FOCUS_MOUSE_RIGHT;
            break;

        case tb_on:
            left    = TOGGLE_ON_LEFT;
            mid     = TOGGLE_ON_MID;
            right   = TOGGLE_ON_RIGHT;
            break;
        case tb_on_focus:
            left    = TOGGLE_ON_FOCUS_LEFT;
            mid     = TOGGLE_ON_FOCUS_MID;
            right   = TOGGLE_ON_FOCUS_RIGHT;
            break;
        case tb_on_mouse:
            left    = TOGGLE_ON_MOUSE_LEFT;
            mid     = TOGGLE_ON_MOUSE_MID;
            right   = TOGGLE_ON_MOUSE_RIGHT;
            break;
        case tb_on_focus_mouse:
            left    = TOGGLE_ON_FOCUS_MOUSE_LEFT;
            mid     = TOGGLE_ON_FOCUS_MOUSE_MID;
            right   = TOGGLE_ON_FOCUS_MOUSE_RIGHT;
            break;
        case tb_on_disabled:
            left    = TOGGLE_ON_DISABLED_LEFT;
            mid     = TOGGLE_ON_DISABLED_MID;
            right   = TOGGLE_ON_DISABLED_RIGHT;
            break;
        case tb_off:
            left    = TOGGLE_OFF_LEFT;
            mid     = TOGGLE_OFF_MID;
            right   = TOGGLE_OFF_RIGHT;
            break;
        case tb_off_focus:
            left    = TOGGLE_OFF_FOCUS_LEFT;
            mid     = TOGGLE_OFF_FOCUS_MID;
            right   = TOGGLE_OFF_FOCUS_RIGHT;
            break;
        case tb_off_mouse:
            left    = TOGGLE_OFF_MOUSE_LEFT;
            mid     = TOGGLE_OFF_MOUSE_MID;
            right   = TOGGLE_OFF_MOUSE_RIGHT;
            break;
        case tb_off_focus_mouse:
            left    = TOGGLE_OFF_FOCUS_MOUSE_LEFT;
            mid     = TOGGLE_OFF_FOCUS_MOUSE_MID;
            right   = TOGGLE_OFF_FOCUS_MOUSE_RIGHT;
            break;
        case tb_off_disabled:
            left    = TOGGLE_OFF_DISABLED_LEFT;
            mid     = TOGGLE_OFF_DISABLED_MID;
            right   = TOGGLE_OFF_DISABLED_RIGHT;
            break;
        default:
            left    = BUTTON_OFF_LEFT;
            mid     = BUTTON_OFF_MID;
            right   = BUTTON_OFF_RIGHT;
            break;

    }

    //left endcap
    texture = ferTextureRegister(left, none, none);
    ferDraw(dimensions.x0, dimensions.y1, texture);

    //right endcap
    texture = ferTextureRegister(right, none, none);
    ferDraw(dimensions.x1 - texture->width, dimensions.y1, texture);
    end_width = texture->width;

    //middle thingies
    texture = ferTextureRegister(mid, none, none);
    ferDrawLine(dimensions.x0, dimensions.y1, dimensions.x1, dimensions.y1, end_width, texture, 0);

    glDisable(GL_ALPHA_TEST);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawCheckbox
    Description : Draws a textured checkboxbutton for the front end
    Inputs      : dimensions - dimensions of Check box
                  state - the current button state
    Outputs     : draws uh buh-ton on da skreen
    Return      :
----------------------------------------------------------------------------*/
void ferDrawCheckbox(rectangle dimensions, ferbuttonstate state)
{
    lifheader *texture = NULL;
    uword x,y;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    //Rockin'!  This is a simple on - load the texture, then draw!
    switch (state)
    {
        case c_on :
            texture = ferTextureRegister(CHECK_ON, none, none);
            break;
        case c_on_focus :
            texture = ferTextureRegister(CHECK_ON_FOCUS, none, none);
            break;
        case c_on_mouse :
            texture = ferTextureRegister(CHECK_ON_MOUSE, none, none);
            break;
        case c_on_focus_mouse :
            texture = ferTextureRegister(CHECK_ON_FOCUS_MOUSE, none, none);
            break;
        case c_on_disabled :
            texture = ferTextureRegister(CHECK_ON_DISABLED, none, none);
            break;
        case c_off :
            texture = ferTextureRegister(CHECK_OFF, none, none);
            break;
        case c_off_focus :
            texture = ferTextureRegister(CHECK_OFF_FOCUS, none, none);
            break;
        case c_off_mouse :
            texture = ferTextureRegister(CHECK_OFF_MOUSE, none, none);
            break;
        case c_off_focus_mouse :
            texture = ferTextureRegister(CHECK_OFF_FOCUS_MOUSE, none, none);
            break;
        case c_off_disabled :
            texture = ferTextureRegister(CHECK_OFF_DISABLED, none, none);
            break;
        default:
            break;
    }
    x = texture->width;
    y = texture->height;
    ferDraw(dimensions.x0, dimensions.y1, texture);

    glDisable(GL_ALPHA_TEST);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawRadioButton
    Description : Draws a textured RAdiobutton for the front end
    Inputs      : dimensions - dimensions of Radio Button
                  state - the current button state
    Outputs     : draws uh buh-ton on da skreen - note that the radio is *not* included
    Return      :
----------------------------------------------------------------------------*/
void ferDrawRadioButton(rectangle dimensions, ferbuttonstate state)
{
    lifheader *texture = NULL;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    //Rockin'!  This is a simple one - load the texture, then draw!
    switch (state)
    {
        case r_on :
            texture = ferTextureRegister(RADIO_ON, none, none);
            break;
        case r_on_focus :
            texture = ferTextureRegister(RADIO_ON_FOCUS, none, none);
            break;
        case r_on_mouse :
            texture = ferTextureRegister(RADIO_ON_MOUSE, none, none);
            break;
        case r_on_focus_mouse :
            texture = ferTextureRegister(RADIO_ON_FOCUS_MOUSE, none, none);
            break;
        case r_on_disabled :
            texture = ferTextureRegister(RADIO_ON_DISABLED, none, none);
            break;
        case r_off :
            texture = ferTextureRegister(RADIO_OFF, none, none);
            break;
        case r_off_focus :
            texture = ferTextureRegister(RADIO_OFF_FOCUS, none, none);
            break;
        case r_off_mouse :
            texture = ferTextureRegister(RADIO_OFF_MOUSE, none, none);
            break;
        case r_off_focus_mouse :
            texture = ferTextureRegister(RADIO_OFF_FOCUS_MOUSE, none, none);
            break;
        case r_off_disabled :
            texture = ferTextureRegister(RADIO_OFF_DISABLED, none, none);
            break;
        default:
            break;
    }
    ferDraw(dimensions.x0, dimensions.y1, texture);

    glDisable(GL_ALPHA_TEST);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawHorizSlider
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawHorizSlider(sliderhandle shandle, uword state)
{
#define FER_HSLIDER_X - 4
    uword x,y;
    lifheader *texleft   = NULL,
              *texmid    = NULL,
              *texright  = NULL,
              *texmarker = NULL;
    //rectangle rect;
    real32 spos;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    switch (state)
    {
    case s_disabled:
        texleft = ferTextureRegister(VOLUME_DISABLED_LEFTCAP, none, none);
        ferDraw(shandle->reg.rect.x0 - slider_end_width + FER_HSLIDER_X, shandle->reg.rect.y1, texleft);
        texright = ferTextureRegister(VOLUME_DISABLED_RIGHTCAP, none, none);
        ferDraw(shandle->reg.rect.x1 + FER_HSLIDER_X, shandle->reg.rect.y1, texright);
        texmid = ferTextureRegister(VOLUME_DISABLED_MID, none, none);
        ferDrawLine(shandle->reg.rect.x0 + FER_HSLIDER_X, shandle->reg.rect.y1,
                    shandle->reg.rect.x1 + slider_end_width + FER_HSLIDER_X,
                    shandle->reg.rect.y1, slider_end_width, texmid, 0);
        break;

    case s_off:
        texleft = ferTextureRegister(VOLUME_OFF_LEFTCAP, none, none);
        ferDraw(shandle->reg.rect.x0 - slider_end_width + FER_HSLIDER_X, shandle->reg.rect.y1, texleft);
        texright = ferTextureRegister(VOLUME_OFF_RIGHTCAP, none, none);
        ferDraw(shandle->reg.rect.x1 + FER_HSLIDER_X, shandle->reg.rect.y1, texright);
        texmid = ferTextureRegister(VOLUME_OFF_MID, none, none);
        ferDrawLine(shandle->reg.rect.x0 + FER_HSLIDER_X, shandle->reg.rect.y1,
                    shandle->reg.rect.x1 + slider_end_width + FER_HSLIDER_X,
                    shandle->reg.rect.y1, slider_end_width, texmid, 0);
        break;

    case s_off_mouse:
        texleft = ferTextureRegister(VOLUME_MOUSE_LEFTCAP, none, none);
        ferDraw(shandle->reg.rect.x0 - slider_end_width + FER_HSLIDER_X, shandle->reg.rect.y1, texleft);
        texright = ferTextureRegister(VOLUME_MOUSE_RIGHTCAP, none, none);
        ferDraw(shandle->reg.rect.x1 + FER_HSLIDER_X, shandle->reg.rect.y1, texright);
        texmid = ferTextureRegister(VOLUME_MOUSE_MID, none, none);
        ferDrawLine(shandle->reg.rect.x0 + FER_HSLIDER_X, shandle->reg.rect.y1,
                    shandle->reg.rect.x1 + slider_end_width + FER_HSLIDER_X,
                    shandle->reg.rect.y1, slider_end_width, texmid, 0);
        break;

    case s_on_mouse:
        texleft = ferTextureRegister(VOLUME_ON_LEFTCAP, none, none);
        ferDraw(shandle->reg.rect.x0 - slider_end_width + FER_HSLIDER_X, shandle->reg.rect.y1, texleft);
        texright = ferTextureRegister(VOLUME_ON_RIGHTCAP, none, none);
        ferDraw(shandle->reg.rect.x1 + FER_HSLIDER_X, shandle->reg.rect.y1, texright);
        texmid = ferTextureRegister(VOLUME_ON_MID, none, none);
        ferDrawLine(shandle->reg.rect.x0 + FER_HSLIDER_X, shandle->reg.rect.y1,
                    shandle->reg.rect.x1 + slider_end_width + FER_HSLIDER_X,
                    shandle->reg.rect.y1, slider_end_width, texmid, 0);
        break;
    }

    spos =  ((real32) (shandle->value) / shandle->maxvalue);
    spos = spos * (shandle->reg.rect.x1-shandle->reg.rect.x0);
    spos = spos + shandle->reg.rect.x0;

    y = shandle->reg.rect.y1;
    x = (uword)spos + FER_HSLIDER_X;

    texmarker = ferTextureRegister(VOLUME_BUTTON, none, none);
    ferDraw(x, y, texmarker);

    glDisable(GL_ALPHA_TEST);
#undef FER_HSLIDER_X
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawVertSlider
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawVertSlider(sliderhandle shandle, uword state)
{
#define FER_VSLIDER_Y (-5)
#define FER_VSLIDER_MARKER 11
    uword x,y;
    lifheader *textop    = NULL,
              *texmid    = NULL,
              *texbot    = NULL,
              *texcenter = NULL,
              *texmarker = NULL;
    real32 spos;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    switch (state)
    {
    case s_off:
        textop = ferTextureRegister(EQ_BAR_OFF_TOPCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y, textop);
        texmid = ferTextureRegister(EQ_BAR_OFF_STRAIGHT, none, none);
        ferDrawLine(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y,
                    shandle->reg.rect.x0,
                    shandle->reg.rect.y1 + slider_vert_end_width + FER_VSLIDER_Y,
                    slider_vert_end_width, texmid, 0);
        texbot = ferTextureRegister(EQ_BAR_OFF_BOTCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width+FER_VSLIDER_Y, texbot);
        texcenter = ferTextureRegister(EQ_BAR_OFF_MIDDLE, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width/2+FER_VSLIDER_Y, texcenter);
        break;

    case s_off_mouse:
        textop = ferTextureRegister(EQ_BAR_MOUSE_TOPCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y, textop);
        texmid = ferTextureRegister(EQ_BAR_MOUSE_STRAIGHT, none, none);
        ferDrawLine(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y,
                    shandle->reg.rect.x0,
                    shandle->reg.rect.y1 + slider_vert_end_width + FER_VSLIDER_Y,
                    slider_vert_end_width, texmid, 0);
        texbot = ferTextureRegister(EQ_BAR_MOUSE_BOTCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width+FER_VSLIDER_Y, texbot);
        texcenter = ferTextureRegister(EQ_BAR_MOUSE_MIDDLE, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width/2+FER_VSLIDER_Y, texcenter);
        break;

    case s_on_mouse:
        textop = ferTextureRegister(EQ_BAR_ON_TOPCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y, textop);
        texmid = ferTextureRegister(EQ_BAR_ON_STRAIGHT, none, none);
        ferDrawLine(shandle->reg.rect.x0, shandle->reg.rect.y0+FER_VSLIDER_Y,
                    shandle->reg.rect.x0,
                    shandle->reg.rect.y1 + slider_vert_end_width + FER_VSLIDER_Y,
                    slider_vert_end_width, texmid, 0);
        texbot = ferTextureRegister(EQ_BAR_ON_BOTCAP, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width+FER_VSLIDER_Y, texbot);
        texcenter = ferTextureRegister(EQ_BAR_ON_MIDDLE, none, none);
        ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1-slider_vert_end_width/2+FER_VSLIDER_Y, texcenter);
        break;
    }

    spos =  ((real32) (shandle->value) / shandle->maxvalue);
    spos = spos * (shandle->reg.rect.y1-shandle->reg.rect.y0);
    spos = spos + shandle->reg.rect.y0;

    x = shandle->reg.rect.x0 + FER_VSLIDER_MARKER;
    y = (uword)spos + FER_VSLIDER_Y;

    texmarker = ferTextureRegister(EQ_BAR_BUTTON, none, none);
    ferDraw(x, y, texmarker);

    glDisable(GL_ALPHA_TEST);
#undef FER_VSLIDER_Y
#undef FER_VSLIDER_MARKER
}


/*-----------------------------------------------------------------------------
    Name        : ferDrawScrollbar
    Description : Draws a textured ScrollBar for the front end
    Inputs      : shandle - dimensions of Scrollbar (plus lotsa other info that we don't need
    Outputs     : draws uh skrol bar on da skreen
    Return      :
----------------------------------------------------------------------------*/
void ferDrawScrollbar(scrollbarhandle shandle, ferscrollbarstate state)
{
    tex_holder bottom  = UNINITIALIZED_TEX_HOLDER,
               mid     = UNINITIALIZED_TEX_HOLDER,
               top     = UNINITIALIZED_TEX_HOLDER,
               tab_bot = UNINITIALIZED_TEX_HOLDER,
               tab_mid = UNINITIALIZED_TEX_HOLDER,
               tab_top = UNINITIALIZED_TEX_HOLDER;
    lifheader *texture[6];
    sdword end_width;
    sdword height, textureHeight;

    glEnable(GL_BLEND);

    top      = VERTSB_TOP;
    mid      = VERTSB_MID;
    bottom   = VERTSB_BOTTOM;

    if (shandle->isVertical)
    {
        switch (state)
        {
            case s_off:
                tab_top  = VERTST_OFF_TOP;
                tab_mid  = VERTST_OFF_MID;
                tab_bot  = VERTST_OFF_BOTTOM;
            break;
            case s_off_mouse:
                tab_top  = VERTST_OFF_MOUSE_TOP;
                tab_mid  = VERTST_OFF_MOUSE_MID;
                tab_bot  = VERTST_OFF_MOUSE_BOTTOM;
            break;
            case s_on_mouse:
                tab_top  = VERTST_ON_TOP;
                tab_mid  = VERTST_ON_MID;
                tab_bot  = VERTST_ON_BOTTOM;
            break;
            default:
                break;
        }
        height = shandle->reg.rect.y1 - shandle->reg.rect.y0;

        //note - because the region is overwritten by the up and down buttons,
        //       the region is drawn to the top of the buttons
        texture[0] = ferTextureRegister(top, none, none);
        texture[1] = ferTextureRegister(mid, none, none);
        texture[2] = ferTextureRegister(bottom, none, none);
        texture[3] = ferTextureRegister(tab_top, none, none);
        texture[4] = ferTextureRegister(tab_mid, none, none);
        texture[5] = ferTextureRegister(tab_bot, none, none);

        textureHeight =
            texture[0]->height +
            texture[1]->height +
            texture[2]->height +
            texture[3]->height +
            texture[4]->height +
            texture[5]->height;

        if(textureHeight < height)
        {
            texture[0] = ferTextureRegister(top, none, none);
            ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y0+texture[0]->height, texture[0]);

            end_width = texture[0]->height;
            texture[1] = ferTextureRegister(mid, none, none);
            ferDrawLine(shandle->reg.rect.x0, shandle->reg.rect.y0, shandle->reg.rect.x0,
                        shandle->reg.rect.y1, end_width, texture[1], 0);

            //draw the thumbwheel region
            texture[2] = ferTextureRegister(bottom, none, none);
            ferDraw(shandle->reg.rect.x0, shandle->reg.rect.y1, texture[2]);

            //draw the thumbwheel
            texture[3] = ferTextureRegister(tab_top, none, none);
            ferDraw(shandle->thumb.x0, shandle->thumb.y0+texture[3]->height, texture[3]);

            end_width = texture[3]->height;
            texture[4] = ferTextureRegister(tab_mid, none, none);
            ferDrawLine(shandle->thumb.x0, shandle->thumb.y0, shandle->thumb.x0,
                        shandle->thumb.y1, end_width, texture[4], 0);

            texture[5] = ferTextureRegister(tab_bot, none, none);
            ferDraw(shandle->thumb.x0, shandle->thumb.y1, texture[5]);
        }
    }

    glDisable(GL_BLEND);
}







/*-----------------------------------------------------------------------------
    Name        : ferDrawScrollbarButton
    Description : Draws a textured ScrollBar for the front end
    Inputs      : shandle - dimensions of Scrollbar (plus lotsa other info that we don't need
    Outputs     : draws uh skrol bar on da skreen
    Return      :
----------------------------------------------------------------------------*/
void ferDrawScrollbarButton(regionhandle region, ferscrollbarbuttonstate state)
{
    lifheader *texture;
    scrollbarbuttonhandle sbutton = (scrollbarbuttonhandle)region;

    glEnable(GL_BLEND);

    if (sbutton->scrollbar->isVertical)
    {
        switch (state)
        {
            case sub_off:
                texture = ferTextureRegister(VERTSA_OFF_UP, none, none);
                ferDraw(region->rect.x0, region->rect.y0+texture->height, texture);
            break;
            case sub_off_mouse:
                texture = ferTextureRegister(VERTSA_OFF_MOUSE_UP, none, none);
                ferDraw(region->rect.x0, region->rect.y0+texture->height, texture);
            break;
            case sub_on_mouse:
                texture = ferTextureRegister(VERTSA_ON_UP, none, none);
                ferDraw(region->rect.x0, region->rect.y0+texture->height, texture);
            break;
            case sdb_off:
                texture = ferTextureRegister(VERTSA_OFF_DOWN, none, none);
                ferDraw(region->rect.x0, region->rect.y1, texture);
            break;
            case sdb_off_mouse:
                texture = ferTextureRegister(VERTSA_OFF_MOUSE_DOWN, none, none);
                ferDraw(region->rect.x0, region->rect.y1, texture);
            break;
            case sdb_on_mouse:
                texture = ferTextureRegister(VERTSA_ON_DOWN, none, none);
                ferDraw(region->rect.x0, region->rect.y1, texture);
            break;
        }
    }

    glDisable(GL_BLEND);
}


/*-----------------------------------------------------------------------------
    Name        : ferDrawFocusWindow
    Description : draws a window by doing the callbacks for each line.  This
                  window can have a focus border
    Inputs      : region
    Outputs     : none
    Return      : void
----------------------------------------------------------------------------*/
void ferDrawFocusWindow(regionhandle region, ferfocuswindowstate state)
{
    rectangle *rect = &region->rect;
    tex_holder nw_corner  = UNINITIALIZED_TEX_HOLDER,
               n_straight = UNINITIALIZED_TEX_HOLDER,
               ne_corner  = UNINITIALIZED_TEX_HOLDER,
               e_straight = UNINITIALIZED_TEX_HOLDER,
               se_corner  = UNINITIALIZED_TEX_HOLDER,
               s_straight = UNINITIALIZED_TEX_HOLDER,
               sw_corner  = UNINITIALIZED_TEX_HOLDER,
               w_straight = UNINITIALIZED_TEX_HOLDER,
               mid        = UNINITIALIZED_TEX_HOLDER;
    sdword corner_width, corner_height;
    lifheader *texture;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    switch (state)
    {
        case lw_normal:
            nw_corner   = LIST_NW_CORNER;
            ne_corner   = LIST_NE_CORNER;
            se_corner   = LIST_SE_CORNER;
            sw_corner   = LIST_SW_CORNER;
            n_straight  = LIST_N_STRAIGHT;
            e_straight  = LIST_E_STRAIGHT;
            s_straight  = LIST_S_STRAIGHT;
            w_straight  = LIST_W_STRAIGHT;
            mid         = LIST_MID;
            break;
        case lw_focus:
            nw_corner   = LIST_FOCUS_NW_CORNER;
            ne_corner   = LIST_FOCUS_NE_CORNER;
            se_corner   = LIST_FOCUS_SE_CORNER;
            sw_corner   = LIST_FOCUS_SW_CORNER;
            n_straight  = LIST_FOCUS_N_STRAIGHT;
            e_straight  = LIST_FOCUS_E_STRAIGHT;
            s_straight  = LIST_FOCUS_S_STRAIGHT;
            w_straight  = LIST_FOCUS_W_STRAIGHT;
            mid         = LIST_FOCUS_MID;
            break;
    }

    texture = ferTextureRegister(nw_corner, none, none);
    ferDraw(rect->x0, rect->y0+texture->height, texture);

    texture = ferTextureRegister(ne_corner, none, none);
    ferDraw(rect->x1-texture->width, rect->y0+texture->height, texture);

    texture = ferTextureRegister(se_corner, none, none);
    ferDraw(rect->x1-texture->width, rect->y1, texture);

    texture = ferTextureRegister(sw_corner, none, none);
    ferDraw(rect->x0, rect->y1, texture);

    corner_width    = texture->width;
    corner_height   = texture->height;

    texture = ferTextureRegister(n_straight, none, none);
    ferDrawLine(rect->x0, rect->y0+texture->height, rect->x1, rect->y0+texture->height, corner_width, texture, 1);

    texture = ferTextureRegister(e_straight, none, none);
    ferDrawLine(rect->x1-texture->width, rect->y0, rect->x1-texture->width, rect->y1, corner_height, texture, 1);

    texture = ferTextureRegister(s_straight, none, none);
    ferDrawLine(rect->x0, rect->y1, rect->x1, rect->y1, corner_width, texture, 1);

    texture = ferTextureRegister(w_straight, none, none);
    ferDrawLine(rect->x0, rect->y0, rect->x0, rect->y1, corner_height, texture, 1);

/*    texture = ferTextureRegister(mid, none, none);

    for (y=rect->y0+corner_height+texture->height; y < rect->y1-texture->height; y+=texture->height)
    {
        ferDrawLine(rect->x0+corner_width, y, rect->x1-corner_width, y, 0, texture, 1);
    }*/

    glDisable(GL_ALPHA_TEST);
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawDecorative
    Description : Draws a decorative region
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawDecorative(regionhandle region)
{
    lifheader *texture;

    texture = ferTextureRegisterSpecial((char *)region->userID, decorative, none);
    if (texture->width > 256 || texture->height > 256)
    {
        if (hrRunning && bitTest(texture->flags, TRF_Alpha))
        {
            glEnable(GL_BLEND);
            glDisable(GL_ALPHA_TEST);
            rndAdditiveBlends(FALSE);
        }
    }
    else
    {
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 4e-3f);
    }

    ferDraw(region->rect.x0, region->rect.y1, texture);
    glDisable(GL_ALPHA_TEST);

    if (hrRunning && bitTest(texture->flags, TRF_Alpha))
    {
        glDisable(GL_BLEND);
    }
}

void ferDrawOpaqueDecorative(regionhandle region)
{
#if 1
    ferDrawDecorative(region);
#else
    lifheader* texture;
    featom* atom;
    rectangle r;
    color c;

    texture = ferTextureRegisterSpecial((char*)region->userID, decorative, none);

    atom = region->atom;
    dbgAssertOrIgnore(atom != NULL);
    r = region->rect;
    r.x1 = r.x0 + texture->width;
    r.y1 = r.y0 + texture->height;
    c = atom->contentColor;
    primRectSolid2(&r, c);
    ferDrawDecorative(region);
#endif
}

/*-----------------------------------------------------------------------------
    Name        : ferDrawBitmapButton
    Description : Draws a bitmapbutton.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ferDrawBitmapButton(regionhandle region, ferbitmapbuttonstate state)
{
    lifheader *texture;
    featom    *atom;
    char       name[128];

    atom = region->atom;
    dbgAssertOrIgnore(atom->type == FA_BitmapButton);

    strcpy(name, (char *)atom->attribs);
    switch (state)
    {
        case bb_off:
        {
            strcat(name, "_off.lif");
        }
        break;
        case bb_off_mouse:
        {
            strcat(name, "_off_mouse.lif");
        }
        break;
        case bb_on:
        {
            strcat(name, "_on.lif");
        }
        break;
        case bb_on_mouse:
        {
            strcat(name, "_on_mouse.lif");
        }
        break;
    }
    texture = ferTextureRegisterSpecial(name, decorative, none);
    ferDraw(region->rect.x0, region->rect.y1, texture);
}
