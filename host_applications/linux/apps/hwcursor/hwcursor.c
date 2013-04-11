// A simple demo using dispmanx to display an overlay

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#include <X11/Xlib.h>

#include "bcm_host.h"

#define ALIGN_UP(x,y)  ((x + (y)-1) & ~((y)-1))

static void FillRect( VC_IMAGE_TYPE_T type, void *image, int pitch, int aligned_height, int x, int y, int w, int h, int val )
{
  int         row;
  int         col;

  uint16_t *line = (uint16_t *)image + y * (pitch>>1) + x;

  for ( row = 0; row < h; row++ )
    {
      for ( col = 0; col < w; col++ )
        {
	  line[col] = val;
        }
      line += (pitch>>1);
    }
}

int main(void)
{
  uint32_t                    vc_image_ptr;
  VC_RECT_T                   src_rect;
  VC_RECT_T                   dst_rect;
  VC_IMAGE_TYPE_T             type =      VC_IMAGE_RGB565;
  DISPMANX_DISPLAY_HANDLE_T   dispmanx_display;
  DISPMANX_RESOURCE_HANDLE_T  resource;
  DISPMANX_MODEINFO_T         info;
  DISPMANX_UPDATE_HANDLE_T    update;
  DISPMANX_ELEMENT_HANDLE_T   element;
  void                       *image;
  int                         width = 10; 
  int                         height = 10;
  int                         pitch = ALIGN_UP(width*2, 32);
  int                         aligned_height = ALIGN_UP(height, 16);
  int                         ret;

  //  Display                    *x_display;
  //  Window                      root, child;
  int                         cursor_X, cursor_Y, winX, winY;
  unsigned int                mask;

  bcm_host_init();

  dispmanx_display = vc_dispmanx_display_open(0);
 
  ret = vc_dispmanx_display_get_info(dispmanx_display, &info);
   
  image = calloc(1, pitch * height);
  
  FillRect(type, image, pitch, aligned_height, 0, 0, width, height, 0x0000);
  
  resource = vc_dispmanx_resource_create(type, width, height, &vc_image_ptr);
  
  vc_dispmanx_rect_set(&dst_rect, 0, 0, width, height);

  vc_dispmanx_resource_write_data(resource, type, pitch, image, &dst_rect);

  update = vc_dispmanx_update_start(0);

  vc_dispmanx_rect_set(&src_rect, 0, 0, width << 16, height << 16);
  
  vc_dispmanx_rect_set(&dst_rect, (info.width - width)/2, (info.height - height)/2, width, height);
    
  element = vc_dispmanx_element_add(          update,
					      dispmanx_display,
					      2000,               // layer
					      &dst_rect,
					      resource,
					      &src_rect,
					      DISPMANX_PROTECTION_NONE,
					      NULL,
					      NULL,             // clamp
					      VC_IMAGE_ROT0 );

  vc_dispmanx_update_submit_sync(update);

  x_display = XOpenDisplay(NULL);

  while (1){ 

    XQueryPointer(x_display,DefaultRootWindow(x_display),&root,&child,&cursor_X,&cursor_Y,&winX,&winY,&mask);    
    
    update = vc_dispmanx_update_start(0);

    vc_dispmanx_rect_set(&dst_rect, cursor_X, cursor_Y, width, height);

    vc_dispmanx_element_change_attributes(update, element, 0x4, 2000, NULL, &dst_rect, &src_rect, NULL, VC_IMAGE_ROT0);

    vc_dispmanx_update_submit_sync(update);

  }
  return 0;
}
