#include <v8/v8.h>

#define USE_X11
#include <node/node.h>

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "atom/browser/atom_browser_client.h"
#include "atom/browser/native_window.h"
#include "atom/browser/window_list.h"

#include "atom/browser/native_window_gtk.h"

#include "atom/browser/atom_browser_main_parts.h"
#include "atom/browser/browser.h"
#include "atom/common/api/atom_bindings.h"
#include "atom/common/node_bindings.h"
#include "atom/common/node_includes.h"

#include "atom/app/atom_library_main.h"

#include "atom/app/atom_main_delegate.h"
#include "content/public/app/content_main.h"

#include "native_mate/dictionary.h"
#include "native_mate/converter.h"
#include "native_mate/wrappable.h"
#include "native_mate/function_template.h"
#include "native_mate/handle.h"

#include "atom/common/native_mate_converters/v8_value_converter.h"
#include "atom/common/native_mate_converters/function_converter.h"
#include "atom/common/native_mate_converters/string16_converter.h"

#include "base/callback.h"
#include "base/callback_forward.h"
#include "base/strings/string16.h"

#include "v8/include/v8.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>
#include <gdk/gdkx.h>


#include "ghbcompositor.h"

#include <gdk/gdkcairo.h>
#include <cairo/cairo.h>

#include "content/browser/renderer_host/render_widget_host_view_gtk.h"
#include "content/public/browser/render_view_host.h"
#include "SkBitmap.h"
#include "SkCanvas.h"

#include <gtk/gtk.h>
#include <gdk/gdkscreen.h>
#include <cairo.h>

SDL_Surface *screen, *hello;

GtkWidget *blender;
GtkWidget *sdl_canvas;
GtkWidget *chrome_canvas;
GtkWidget *test_canvas;
atom::WindowList *list;
GtkWindow *win_;
GtkWidget *vbox_;
GtkWidget *native_view;

content::RenderViewHost* host;
SkBitmap background;

int sdl(void *dummy) {
  SDL_BlitSurface( hello, NULL, screen, NULL );
  SDL_Flip( screen );
  return 0;
  SDL_Init( SDL_INIT_EVERYTHING ); //Set up screen
  SDL_Surface* screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);
  SDL_Surface* hello = SDL_LoadBMP( "lolcats.bmp" );
  SDL_BlitSurface( hello, NULL, screen, NULL );
  SDL_Flip( screen );
  SDL_Delay( 30000 );
  SDL_FreeSurface( screen);
  SDL_Quit();

  return 0;
}


gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkColormap *colormap = gdk_screen_get_rgba_colormap(screen);

    if (!colormap)
    {
        printf("Your screen does not support alpha channels!\n");
        colormap = gdk_screen_get_rgb_colormap(screen);
        supports_alpha = FALSE;
  }
  else
    {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_colormap(widget, colormap);
}

static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer userdata)
{
   cairo_t *cr = gdk_cairo_create(widget->window);

    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */

    /* draw the background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    printf("PAINT test\n");
    cairo_paint(cr);

    cairo_destroy(cr);


    return FALSE;
}

static gboolean expose_vbox(GtkWidget *widget, GdkEventExpose *event, gpointer userdata)
{
   cairo_t *cr = gdk_cairo_create(widget->window);
    printf("PAINT vbox\n");

    /* draw the background */
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */
    //cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    //cairo_paint(cr);

    cairo_set_source_rgba (cr, 0.0, 1.0, 0.0, 1.0); /* transparent */
    cairo_move_to(cr, 30, 30);
    cairo_show_text(cr, "This is the fnord!");

    cairo_destroy(cr);

    return FALSE;
}

static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data)
{
    /* toggle window manager frames */
    //gtk_window_set_decorated(win, !gtk_window_get_decorated(win));
}

static void Hello(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  std::cout << "NULL = " << NULL << std::endl;

  // Focus on the first visible window.
  atom::WindowList *list = atom::WindowList::GetInstance();
  size_t size = list->size();
  for (atom::WindowList::iterator iter = list->begin(); iter != list->end(); ++iter) {
    atom::NativeWindowGtk *win_atom_gtk = (atom::NativeWindowGtk*) *iter;

    win_ = win_atom_gtk->GetGtkWindow();
    vbox_ = NULL;
    native_view = NULL;

    if (win_ != NULL) {
      std::cout << "RECOVER! i=" << std::endl;
      vbox_ = (GtkWidget*)gtk_bin_get_child(GTK_BIN(win_));
    }

    if (vbox_ != NULL) {
      GList *children = gtk_container_get_children(GTK_CONTAINER(vbox_));
      native_view = GTK_WIDGET(g_list_first(children)->data);
    }

    if (win_ != NULL && vbox_ != NULL && native_view != NULL) {

      // STEAL THE VBOX

      vbox_ = GTK_WIDGET(g_object_ref(vbox_));
      gtk_container_remove(GTK_CONTAINER(win_), vbox_);

      // CHROME CANVAS
      
      chrome_canvas = gtk_event_box_new();
      gtk_container_add(GTK_CONTAINER(chrome_canvas), vbox_);

      // TRY TO MAKE THE NATIVE VIEW TRANSPARENT

      host = win_atom_gtk->GetWebContents()->GetRenderViewHost();
      background.setConfig(SkBitmap::kARGB_8888_Config, 1, 1);
      background.allocPixels();
      background.eraseARGB(0, 0, 0, 5);

      host->GetView()->SetBackground(background);

      // TEST VIEW
       
      test_canvas = gtk_event_box_new();

      gtk_widget_set_app_paintable(test_canvas, TRUE);

      g_signal_connect(G_OBJECT(test_canvas), "expose-event", G_CALLBACK(expose), NULL);
      g_signal_connect(G_OBJECT(test_canvas), "screen-changed", G_CALLBACK(screen_changed), NULL);

      gtk_widget_add_events(test_canvas, GDK_BUTTON_PRESS_MASK);
      g_signal_connect(G_OBJECT(test_canvas), "button-press-event", G_CALLBACK(clicked), NULL);

      GtkWidget* fixed_container = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(test_canvas), fixed_container);

      GtkWidget* button = gtk_button_new_with_label("button1");
      gtk_widget_set_size_request(button, 100, 100);
      gtk_container_add(GTK_CONTAINER(fixed_container), button);

      screen_changed(test_canvas, NULL, NULL);

      gtk_widget_show_all(test_canvas);
      gtk_widget_realize(test_canvas);
      gtk_widget_show(test_canvas);

      // SDL CANVAS

      sdl_canvas = gtk_event_box_new();
 
      // INITIALIZE LAYOUT

      blender = ghb_compositor_new();
      ghb_compositor_zlist_insert(GHB_COMPOSITOR(blender), sdl_canvas, 1, 1);
      ghb_compositor_zlist_insert(GHB_COMPOSITOR(blender), test_canvas, 2, 1);
      //ghb_compositor_zlist_insert(GHB_COMPOSITOR(blender), chrome_canvas, 3, 0.0);

      gtk_container_add(GTK_CONTAINER(win_), GTK_WIDGET(blender));

      // SET BACKGROUNDS FOR DEBUGGING

	    GdkColor bg[] = {{0, 0xffff, 0xffff, 0}, {0, 0, 0xffff, 0}, {0, 0, 0, 0xffff}, {0, 0xffff, 0, 0xffff}, {0, 0, 0xffff, 0xffff}};
	    gtk_widget_modify_bg(sdl_canvas, GTK_STATE_NORMAL, &bg[0]);
	    gtk_widget_modify_bg(native_view, GTK_STATE_NORMAL, &bg[1]);
	    gtk_widget_modify_bg(vbox_, GTK_STATE_NORMAL, &bg[2]);
	    gtk_widget_modify_bg(chrome_canvas, GTK_STATE_NORMAL, &bg[3]);
	    gtk_widget_modify_bg(GTK_WIDGET(gtk_widget_get_window(native_view)), GTK_STATE_NORMAL, &bg[4]);

      // TRY SHOW ALL THAT SHIT

      gtk_widget_set_size_request(sdl_canvas, 640, 480);
      gtk_widget_realize(sdl_canvas);
      gtk_widget_show(sdl_canvas);

      gtk_widget_realize(GTK_WIDGET(blender));
      gtk_widget_show(GTK_WIDGET(blender));

      gtk_widget_realize(GTK_WIDGET(chrome_canvas));
      gtk_widget_show(GTK_WIDGET(chrome_canvas));

      // SDL INITIALIZATION ////////////////////////////////

			while (gtk_events_pending())
				gtk_main_iteration_do(FALSE);
        
			char SDL_windowhack[128];
			sprintf(SDL_windowhack, "SDL_WINDOWID=%u", (unsigned int)GDK_WINDOW_XID(gtk_widget_get_window(sdl_canvas)));
			SDL_putenv(SDL_windowhack);
        
			// init SDL video
      SDL_Init( SDL_INIT_EVERYTHING ); //Set up screen
			if (SDL_WasInit(SDL_INIT_VIDEO)) {
				SDL_QuitSubSystem(SDL_INIT_VIDEO);
      }

			if ( SDL_InitSubSystem(SDL_INIT_VIDEO) < 0 )
			{
				fprintf(stderr, "Couldn't init SDL video: %s\n", SDL_GetError());
				gtk_main_quit();
			} else {
        fprintf(stderr, "Initialized SDL video on %s\n", SDL_windowhack);
      }

      screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);
      hello = SDL_LoadBMP( "lolcats.bmp" );
      sdl(NULL);
    }
  }

  args.GetReturnValue().Set(v8::String::New("world"));
}


static void SDL_Open(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  std::cout << "IN SDL_OPen" << std::endl;

  SDL_CreateThread(sdl, NULL);

  args.GetReturnValue().Set(v8::String::New("SDL"));
}

// NodeModuleInit ////////

void init(v8::Handle<v8::Object> exports) {
  NODE_SET_METHOD(exports, "hello", Hello);
  NODE_SET_METHOD(exports, "sdlOpen", SDL_Open);
}

NODE_MODULE(boop, init)




// BoopBrowserMainParts ////////////////////////////////////

class BoopBrowserMainParts : public atom::AtomBrowserMainParts {
 protected:
  virtual void PostEarlyInitialization() OVERRIDE;
};

void BoopBrowserMainParts::PostEarlyInitialization() {
  atom::AtomBrowserMainParts::PostEarlyInitialization();
  
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);


  v8::Handle<v8::Object> glob = atom::global_env->process_object();
  
  v8::Local<v8::Object> exports = v8::Object::New();
  boop_module.register_func(exports, v8::Undefined());

  v8::Handle<v8::Value> foo = v8::String::New("boop");
  glob->Set(foo, exports);
}

// BoopBrowserClient ///////////////////////////////////////

class BoopBrowserClient : public atom::AtomBrowserClient {
 private:
  virtual brightray::BrowserMainParts* OverrideCreateBrowserMainParts(
      const content::MainFunctionParams&) OVERRIDE;

};

brightray::BrowserMainParts* BoopBrowserClient::OverrideCreateBrowserMainParts(
    const content::MainFunctionParams&) {
  brightray::BrowserMainParts* po =(brightray::BrowserMainParts*) new BoopBrowserMainParts();
  return po;
}

// BoopMainDelegate ////////////////////////////////////////

class BoopMainDelegate : public atom::AtomMainDelegate {
  private:
    content::ContentBrowserClient* CreateContentBrowserClient() OVERRIDE;
  scoped_ptr<content::ContentBrowserClient> browser_client_;
};

content::ContentBrowserClient* BoopMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new BoopBrowserClient);
  return browser_client_.get();
}

// MAIN ////////////////////////////////////////////////////

/**
 * The main function just starts the (our extension of the)
 * atom shell
 */
int main(int argc, const char** argv) {
  BoopMainDelegate delegate;
  return content::ContentMain(argc, argv, &delegate);
}
